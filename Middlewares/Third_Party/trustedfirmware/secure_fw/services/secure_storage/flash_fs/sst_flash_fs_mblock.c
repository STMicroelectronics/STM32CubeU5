/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sst_flash_fs_mblock.h"

#include <stddef.h>

#include "cmsis_compiler.h"
#include "secure_fw/services/secure_storage/sst_object_defs.h"
#include "secure_fw/services/secure_storage/sst_utils.h"
#include "tfm_memory_utils.h"

#if ((SST_TOTAL_NUM_OF_BLOCKS < 2) || (SST_TOTAL_NUM_OF_BLOCKS == 3))
  /* The minimum number of blocks is 2. In this case, metadata and data are
   * stored in the same physical block, and the other block is required for
   * power failure safe operation.
   * If at least 1 data block is available, 1 data scratch block is required for
   * power failure safe operation. So, in this case, the minimum number of
   * blocks is 4 (2 metadata block + 2 data blocks).
   */
  #error "Total number of blocks should be 2 or bigger than 3"
#endif

/* The numbers in the defines are physical block indexes, starting from 0,
 * except for SST_NUM_DEDICATED_DBLOCKS.
 */
#if (SST_TOTAL_NUM_OF_BLOCKS == 2)
  /* Metadata and data are stored in the same physical block, and the other
   * block is required for power failure safe operation.
   */

  /* Initial position of scratch block is the scratch metadata block */
  #define SST_INIT_SCRATCH_DBLOCK 1

  /* Metadata and data are stored in the same block */
  #define SST_INIT_DBLOCK_START 0

  /* There are no dedicated data blocks when only two blocks are available */
  #define SST_NUM_DEDICATED_DBLOCKS 0

#else

  /* Initial position of scratch block is immediately after metadata blocks */
  #define SST_INIT_SCRATCH_DBLOCK 2

  /* One metadata block and two scratch blocks are reserved. One scratch block
   * for metadata operations and the other for files data operations.
   */
  #define SST_INIT_DBLOCK_START 3

  /* Number of blocks dedicated just for data is the number of blocks available
   * beyond the initial datablock start index.
   */
  #define SST_NUM_DEDICATED_DBLOCKS (SST_TOTAL_NUM_OF_BLOCKS - \
                                     SST_INIT_DBLOCK_START)
#endif /* SST_TOTAL_NUM_OF_BLOCKS == 2 */

/* Total number of datablocks is the number of dedicated datablocks plus
 * logical datablock 0 stored in the metadata block.
 */
#define SST_NUM_ACTIVE_DBLOCKS (SST_NUM_DEDICATED_DBLOCKS + 1)

/* Physical ID of the two metadata blocks */
/* NOTE: the earmarked area may not always start at block number 0.
 *       However, the flash interface can always add the required offset.
 */
#define SST_METADATA_BLOCK0  0
#define SST_METADATA_BLOCK1  1

/*!
 * \def SST_OTHER_META_BLOCK
 *
 * \brief Macro to get the the swap metadata block.
 */
#define SST_OTHER_META_BLOCK(metablock) \
((metablock == SST_METADATA_BLOCK0) ? \
(SST_METADATA_BLOCK1) : (SST_METADATA_BLOCK0))

/*!
 * \struct sst_metadata_block_header_t
 *
 * \brief Structure to store the metadata block header.
 *
 * \note  The active_swap_count must be the last member to allow it to be
 *        programmed last.
 */
struct sst_metadata_block_header_t {
    uint32_t scratch_dblock;    /*!< Physical block ID of the data
                                 *   section's scratch block
                                 */
    uint8_t fs_version;         /*!< SST system version */
    uint8_t active_swap_count;  /*!< Physical block ID of the data */
#if SST_FLASH_PROGRAM_UNIT > 2
    uint8_t pad[SST_FLASH_PROGRAM_UNIT-(sizeof(uint32_t)+2)%SST_FLASH_PROGRAM_UNIT];
#endif

};

/*!
 * \struct sst_flash_fs_context_t
 *
 * \brief Structure to store the flash file system context.
 *
 */
struct sst_flash_fs_context_t {
    struct sst_metadata_block_header_t meta_block_header; /*!< Block metadata
                                                           *   header
                                                           */
    uint32_t active_metablock;           /*!< Active metadata block */
    uint32_t scratch_metablock;          /*!< Scratch meta block */
};

static struct sst_flash_fs_context_t sst_flash_fs_ctx;

#define SST_BLOCK_META_HEADER_SIZE  sizeof(struct sst_metadata_block_header_t)
#define SST_BLOCK_METADATA_SIZE     sizeof(struct sst_block_meta_t)
#define SST_FILE_METADATA_SIZE      sizeof(struct sst_file_meta_t)

#define SST_MAX_NUM_FILES  SST_MAX_NUM_OBJECTS

#define SST_ALL_METADATA_SIZE (SST_BLOCK_META_HEADER_SIZE + \
                          (SST_NUM_ACTIVE_DBLOCKS * SST_BLOCK_METADATA_SIZE) + \
                          (SST_MAX_NUM_FILES * SST_FILE_METADATA_SIZE))

#define SST_ALIGNED_MAX_FILE_SIZE   GET_ALIGNED_FLASH_BYTES(SST_MAX_OBJECT_SIZE)

/* It is not required that all files fit in SST flash area at the same time.
 * So, it is possible that a create action fails because flash is full.
 * However, the larger file must have enough space in the SST flash area to be
 * created, at least, when the SST flash area is empty.
 */
/* Checks at compile time if the largest file fits in the data area */
SST_UTILS_BOUND_CHECK(LARGEST_FILE_NOT_FIT_IN_DATA_BLOCK,
                      SST_ALIGNED_MAX_FILE_SIZE, SST_BLOCK_SIZE);

#if (SST_TOTAL_NUM_OF_BLOCKS == 2)
SST_UTILS_BOUND_CHECK(FILE_NOT_FIT_IN_DATA_AREA, SST_ALIGNED_MAX_FILE_SIZE,
                      (SST_BLOCK_SIZE - SST_ALL_METADATA_SIZE));
#endif

/* Checks at compile time if the metadata fits in a flash block */
SST_UTILS_BOUND_CHECK(METADATA_NOT_FIT_IN_METADATA_BLOCK,
                      SST_ALL_METADATA_SIZE, SST_BLOCK_SIZE);

/**
 * \brief Gets offset of a logical block's metadata in metadata block.
 *
 * \param[in] lblock  Logical block number
 *
 * \return Return offset value in metadata block
 */
static uint32_t sst_mblock_block_meta_offset(uint32_t lblock)
{
    uint32_t offset;

    offset = SST_BLOCK_META_HEADER_SIZE +
             (lblock * SST_BLOCK_METADATA_SIZE);

    return offset;
}

/**
 * \brief Gets offset of an file metadata in metadata block.
 *
 * \param[in] idx  File metadata entry index
 *
 * \return Return offset value in metadata block
 */
static uint32_t sst_mblock_file_meta_offset(uint32_t idx)
{
    uint32_t offset;

    offset = SST_BLOCK_META_HEADER_SIZE
             + (SST_NUM_ACTIVE_DBLOCKS * SST_BLOCK_METADATA_SIZE)
             + (idx * SST_FILE_METADATA_SIZE);

    return offset;
}

/**
 * \brief Gets current scratch metadata block.
 *
 * \return scratch metablock's id
 */
__attribute__((always_inline))
__STATIC_INLINE uint32_t sst_cur_meta_scratch_id(void)
{
    return sst_flash_fs_ctx.scratch_metablock;
}

/**
 * \brief Gets current active metadata block.
 *
 * \return scratch metablock's ID
 */
__attribute__((always_inline))
__STATIC_INLINE uint32_t sst_mblock_cur_meta_active(void)
{
    return sst_flash_fs_ctx.active_metablock;
}

/**
 * \brief Swaps metablocks. Scratch becomes active and active becomes scratch.
 */
static void sst_mblock_swap_metablocks(void)
{
    uint32_t tmp_block;

    tmp_block = sst_flash_fs_ctx.scratch_metablock;
    sst_flash_fs_ctx.scratch_metablock = sst_flash_fs_ctx.active_metablock;
    sst_flash_fs_ctx.active_metablock = tmp_block;
}

/**
 * \brief Finds the potential most recent valid metablock.
 *
 * \param[in] h_meta0  Header metadata of meta block 0
 * \param[in] h_meta1  Header metadata of meta block 1
 *
 * \return most recent metablock
 */
static uint8_t sst_mblock_latest_meta_block(
                              const struct sst_metadata_block_header_t *h_meta0,
                              const struct sst_metadata_block_header_t *h_meta1)
{
    uint8_t cur_meta;
    uint8_t meta0_swap_count = h_meta0->active_swap_count;
    uint8_t meta1_swap_count = h_meta1->active_swap_count;

    /* FIXME:  Currently, it returns the potential most recent valid metablock
     *         based on the swap count. This function will be updated later when
     *         the rollback protection feature is introduced.
     */

    /* Logic: if the swap count is 0, then it has rolled over. The metadata
     * block with a swap count of 0 is the latest one, unless the other block
     * has a swap count of 1, in which case the roll over occurred in the
     * previous update. In all other cases, the block with the highest swap
     * count is the latest one.
     */
    if ((meta1_swap_count == 0) && (meta0_swap_count != 1)) {
        /* Metadata block 1 swap count has rolled over and metadata block 0
         * swap count has not, so block 1 is the latest
         */
        cur_meta = SST_METADATA_BLOCK1;

    } else if ((meta0_swap_count == 0) && (meta1_swap_count != 1)) {
        /* Metadata block 0 swap count has rolled over and metadata block 1
         * swap count has not, so block 0 is the latest.
         */
        cur_meta = SST_METADATA_BLOCK0;

    } else if (meta1_swap_count > meta0_swap_count) {
        /* Neither swap count has just rolled over and metadata block 1 has a
         * higher swap count, so block 1 is the latest.
         */
        cur_meta = SST_METADATA_BLOCK1;

    } else {
        /* Neither swap count has just rolled over and metadata block 0 has a
         * higher or equal swap count, so block 0 is the latest.
         */
        cur_meta = SST_METADATA_BLOCK0;
    }

    return cur_meta;
}

#ifdef SST_VALIDATE_METADATA_FROM_FLASH
/**
 * \brief Validates file metadata in order to guarantee that a corruption or
 *        malicious change in stored metadata doesn't result in an invalid
 *        access.
 *
 * \param[in] file_meta  Pointer to file meta structure
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
__attribute__((always_inline))
__STATIC_INLINE psa_ps_status_t sst_mblock_validate_file_meta(
                                        const struct sst_file_meta_t *file_meta)
{
    psa_ps_status_t err;

    /* Logical block ID can not be bigger or equal than number of
     * active blocks.
     */
    if (file_meta->lblock >= SST_NUM_ACTIVE_DBLOCKS) {
        return PSA_PS_ERROR_DATA_CORRUPT;
    }

    /* meta->id can be 0 if the file is not in use. If it is in
     * use, check the metadata.
     */
    if (sst_utils_validate_fid(file_meta->id) == PSA_PS_SUCCESS) {
        /* validate files values if file is in use */
        if (file_meta->max_size > SST_MAX_OBJECT_SIZE) {
            return PSA_PS_ERROR_DATA_CORRUPT;
        }

        /* The current file data size must be smaller or equal than
         * file data max size.
         */
        if (file_meta->cur_size > file_meta->max_size) {
            return PSA_PS_ERROR_DATA_CORRUPT;
        }

        if (file_meta->lblock == SST_LOGICAL_DBLOCK0) {
            /* In block 0, data index must be located after the metadata */
            if (file_meta->data_idx <
                sst_mblock_file_meta_offset(SST_MAX_NUM_OBJECTS)) {
                return PSA_PS_ERROR_DATA_CORRUPT;
            }
        }

        /* Boundary check the incoming request */
        err = sst_utils_check_contained_in(SST_BLOCK_SIZE,
                                           file_meta->data_idx,
                                           file_meta->max_size);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_DATA_CORRUPT;
        }
    }

    return PSA_PS_SUCCESS;
}

/**
 * \brief Validates block metadata in order to guarantee that a corruption or
 *        malicious change in stored metadata doesn't result in an invalid
 *        access.
 *
 * \param[in] block_meta  Pointer to block meta structure
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
__attribute__((always_inline))
__STATIC_INLINE psa_ps_status_t sst_mblock_validate_block_meta(
                                      const struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;
    /* Data block's data start at position 0 */
    uint32_t valid_data_start_value = 0;

    if (block_meta->phy_id >= SST_TOTAL_NUM_OF_BLOCKS) {
        return PSA_PS_ERROR_DATA_CORRUPT;
    }

    /* Boundary check: block data start + free size can not be bigger
     * than max block size.
     */
    err = sst_utils_check_contained_in(SST_BLOCK_SIZE,
                                       block_meta->data_start,
                                       block_meta->free_size);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_DATA_CORRUPT;
    }

    if (block_meta->phy_id == SST_METADATA_BLOCK0 ||
        block_meta->phy_id == SST_METADATA_BLOCK1) {

        /* For metadata + data block, data index must start after the
         * metadata area.
         */
        valid_data_start_value = sst_mblock_file_meta_offset(
                                                           SST_MAX_NUM_OBJECTS);
    }

    if (block_meta->data_start != valid_data_start_value) {
        return PSA_PS_ERROR_DATA_CORRUPT;
    }

    return PSA_PS_SUCCESS;
}
#endif

/**
 * \brief Gets a free file metadata table entry.
 *
 * \return Return index of a free file meta entry
 */
static uint32_t sst_get_free_file_index(void)
{
    psa_ps_status_t err;
    uint32_t i;
    struct sst_file_meta_t tmp_metadata;

    for (i = 0; i < SST_MAX_NUM_OBJECTS; i++) {
        err = sst_flash_fs_mblock_read_file_meta(i, &tmp_metadata);
        if (err != PSA_PS_SUCCESS) {
            return SST_METADATA_INVALID_INDEX;
        }

        /* Check if this entry is free by checking if ID values is an
         * invalid ID.
         */
        if (sst_utils_validate_fid(tmp_metadata.id) != PSA_PS_SUCCESS) {
            /* Found */
            return i;
        }
    }

    return SST_METADATA_INVALID_INDEX;
}

/**
 * \brief Writes an file metadata entry into scratch metadata block.
 *
 * \param[in] idx        File metadata entry index in the metadata table
 * \param[in] file_meta  Metadata pointer
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_update_scratch_file_meta(uint32_t idx,
                                             struct sst_file_meta_t *file_meta)
{
    psa_ps_status_t err;
    uint32_t pos;
    uint32_t scratch_block;

    scratch_block = sst_cur_meta_scratch_id();

    /* Calculate the position */
    pos = sst_mblock_file_meta_offset(idx);
    err = sst_flash_write(scratch_block, (uint8_t *)file_meta, pos,
                          SST_FILE_METADATA_SIZE);
    return err;
}

/**
 * \brief Erases data and meta scratch blocks.
 */
static psa_ps_status_t sst_mblock_erase_scratch_blocks(void)
{
    psa_ps_status_t err;
    uint32_t scratch_datablock;
    uint32_t scratch_metablock;

    scratch_metablock = sst_cur_meta_scratch_id();

    /* For the atomicity of the data update process
     * and power-failure-safe operation, it is necessary that
     * metadata scratch block is erased before data block.
     */
    err = sst_flash_erase_block(scratch_metablock);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* If the number of blocks is bigger than 2, the code needs to erase the
     * scratch block used to process any change in the data block which contains
     * only data. Otherwise, if the number of blocks is equal to 2, it means
     * that all data is stored in the metadata block.
     */
    if (SST_TOTAL_NUM_OF_BLOCKS > 2) {
        scratch_datablock = sst_flash_fs_mblock_cur_data_scratch_id(
                                                     (SST_LOGICAL_DBLOCK0 + 1));
        err = sst_flash_erase_block(scratch_datablock);
    }

    return err;
}

/**
 * \brief Updates scratch block meta.
 *
 * \param[in] lblock      Logical block number
 * \param[in] block_meta  Pointer to the block metadata data to write in the
 *                        scratch block
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_update_scratch_block_meta(uint32_t lblock,
                                            struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;
    uint32_t meta_block;
    uint32_t pos;

    meta_block = sst_cur_meta_scratch_id();
    /* Calculate the position */
    pos = sst_mblock_block_meta_offset(lblock);
    err = sst_flash_write(meta_block, (uint8_t *)block_meta, pos,
                          SST_BLOCK_METADATA_SIZE);
    return err;
}

/**
 * \brief Copies rest of the block metadata.
 *
 * \param[in] lblock  Logical block number to skip
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_copy_remaining_block_meta(uint32_t lblock)
{
    struct sst_block_meta_t block_meta;
    psa_ps_status_t err;
    uint32_t meta_block;
    uint32_t pos;
    uint32_t scratch_block;
    uint32_t size;

    scratch_block = sst_cur_meta_scratch_id();
    meta_block = sst_mblock_cur_meta_active();

    if (lblock != SST_LOGICAL_DBLOCK0) {
        /* The file data in the logical block 0 is stored in same physical
         * block where the metadata is stored. A change in the metadata requires
         * a swap of physical blocks. So, the physical block ID of logical block
         * 0 needs to be updated to reflect this change, if the file processed
         * is not located in logical block 0. If it is located in block 0,
         * the physical block ID has been updated while processing the file
         * data.
         */
        err = sst_flash_fs_mblock_read_block_metadata(SST_LOGICAL_DBLOCK0,
                                                      &block_meta);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        /* Update physical ID for logical block 0 to match with the
         * metadata block physical ID.
         */
        block_meta.phy_id = scratch_block;
        err = sst_mblock_update_scratch_block_meta(SST_LOGICAL_DBLOCK0,
                                                   &block_meta);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        /* Copy the rest of metadata blocks between logical block 0 and
         * the logical block provided in the function.
         */
        if (lblock > 1) {
            pos = sst_mblock_block_meta_offset(SST_LOGICAL_DBLOCK0 + 1);

            size = sst_mblock_block_meta_offset(lblock) - pos;

            /* Copy rest of the block data from previous block */
            /* Data before updated content */
            err = sst_flash_block_to_block_move(scratch_block, pos, meta_block,
                                                pos, size);
            if (err != PSA_PS_SUCCESS) {
                return err;
            }
        }
    }

    /* Move meta blocks data after updated content */
    pos = sst_mblock_block_meta_offset(lblock+1);

    size = sst_mblock_file_meta_offset(0) - pos;

    err = sst_flash_block_to_block_move(scratch_block, pos,
                                        meta_block, pos, size);

    return err;
}

/**
 * \brief Checks the validity of the metadata block's swap count.
 *
 * \param[in] swap_count  Swap count to validate
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
__attribute__((always_inline))
__STATIC_INLINE psa_ps_status_t sst_mblock_validate_swap_count(
                                                             uint8_t swap_count)
{
    psa_ps_status_t err = PSA_PS_SUCCESS;

    /* When a flash block is erased, the default value
     * is usually 0xFF (i.e. all 1s). Since the swap count
     * is updated last (when encryption is disabled), it is
     * possible that due to a power failure, the swap count
     * value in metadata header is 0xFFFF..., which mean
     * it will appear to be most recent block. Which isn't
     * a problem in itself, as the rest of the metadata is fully
     * valid (as it would have been written before swap count).
     * However, this also means that previous update process
     * wasn't complete. So, if the value is 0xFF..., revert
     * back to previous metablock instead.
     */
    if (swap_count == SST_FLASH_DEFAULT_VAL) {
        err = PSA_PS_ERROR_OPERATION_FAILED;
    }

    return err;
}

/**
 * \brief Checks the validity of FS version.
 *
 * \param[in] fs_version  File system version.
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
__attribute__((always_inline))
__STATIC_INLINE psa_ps_status_t sst_mblock_validate_fs_version(
                                                             uint8_t fs_version)
{
    psa_ps_status_t err = PSA_PS_SUCCESS;

    /* Looks for exact version number.
     * FIXME: backward compatibility could be considered in future revisions.
     */
    if (fs_version != SST_SUPPORTED_VERSION) {
        err = PSA_PS_ERROR_OPERATION_FAILED;
    }

    return err;
}

/**
 * \brief Validates header metadata in order to guarantee that a corruption or
 *        malicious change in stored metadata doesn't result in an invalid
 *        access and the header version is correct.
 *
 * \param[in] h_meta  Pointer to metadata block header
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_validate_header_meta(
                                     struct sst_metadata_block_header_t *h_meta)
{
    psa_ps_status_t err;

    err = sst_mblock_validate_fs_version(h_meta->fs_version);
    if (err == PSA_PS_SUCCESS) {
        err = sst_mblock_validate_swap_count(h_meta->active_swap_count);
    }

    return err;
}

/**
 * \brief Writes the scratch metadata's header.
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_write_scratch_meta_header(void)
{
    psa_ps_status_t err;
    uint32_t scratch_metablock;

    scratch_metablock = sst_cur_meta_scratch_id();

    /* Increment the swap count */
    sst_flash_fs_ctx.meta_block_header.active_swap_count += 1;

    err = sst_mblock_validate_swap_count(
                        sst_flash_fs_ctx.meta_block_header.active_swap_count);
    if (err != PSA_PS_SUCCESS) {
        /* Reset the swap count to 0 */
        sst_flash_fs_ctx.meta_block_header.active_swap_count = 0;
    }

    /* Write the metadata block header */
    err = sst_flash_write(scratch_metablock,
                          (uint8_t *)(&sst_flash_fs_ctx.meta_block_header),
                          0, SST_BLOCK_META_HEADER_SIZE);

    return err;
}

/**
 * \brief Reads the active metadata block header into sst_system_ctx.
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_read_meta_header(void)
{
    psa_ps_status_t err;

    err = sst_flash_read(sst_flash_fs_ctx.active_metablock,
                         (uint8_t *)&sst_flash_fs_ctx.meta_block_header, 0,
                         SST_BLOCK_META_HEADER_SIZE);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    err = sst_mblock_validate_header_meta(&sst_flash_fs_ctx.meta_block_header);

    return err;
}

/**
 * \brief Reserves space for an file.
 *
 * \param[in]  fid          File ID
 * \param[in]  size         Size of the file for which space is reserve
 * \param[out] file_meta    File metadata entry
 * \param[out] block_meta   Block metadata entry
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_mblock_reserve_file(uint32_t fid, uint32_t size,
                                          struct sst_file_meta_t *file_meta,
                                          struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;
    uint32_t i;

    for (i = 0; i < SST_NUM_ACTIVE_DBLOCKS; i++) {
        err = sst_flash_fs_mblock_read_block_metadata(i, block_meta);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        if (block_meta->free_size >= size) {
            /* Set file metadata */
            file_meta->lblock = i;
            file_meta->data_idx = SST_BLOCK_SIZE - block_meta->free_size;
            file_meta->max_size = size;
            file_meta->id = fid;
            file_meta->cur_size = 0;

            /* Update block metadata */
            block_meta->free_size -= size;
            return PSA_PS_SUCCESS;
        }
    }

    /* No block has large enough space to fit the requested file */
    return PSA_PS_ERROR_INSUFFICIENT_SPACE;
}

/**
 * \brief Validates and find the valid-active metablock
 *
 * \return Returns value as specified in \ref psa_ps_status_t
 */
static psa_ps_status_t sst_init_get_active_metablock(void)
{
    uint32_t cur_meta_block = SST_BLOCK_INVALID_ID;
    psa_ps_status_t err;
    struct sst_metadata_block_header_t h_meta0;
    struct sst_metadata_block_header_t h_meta1;
    uint8_t num_valid_meta_blocks = 0;

    /* First two blocks are reserved for metadata */

    /* Read the header of both the metdata blocks */
    err = sst_flash_read(SST_METADATA_BLOCK0, (uint8_t *)&h_meta0,
                         0, SST_BLOCK_META_HEADER_SIZE);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    err = sst_flash_read(SST_METADATA_BLOCK1, (uint8_t *)&h_meta1,
                         0, SST_BLOCK_META_HEADER_SIZE);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* If there are two potential active metadata blocks,
     * an out of turn power down sequence didn't allow previous
     * update operation to complete. Need to find out the valid
     * metadata block now.
     */
    if (sst_mblock_validate_header_meta(&h_meta0) == PSA_PS_SUCCESS) {
        num_valid_meta_blocks++;
        cur_meta_block = SST_METADATA_BLOCK0;
    }

    if (sst_mblock_validate_header_meta(&h_meta1) == PSA_PS_SUCCESS) {
        num_valid_meta_blocks++;
        cur_meta_block = SST_METADATA_BLOCK1;
    }

    /* If there are more than 1 potential metablocks, the previous
     * update operation was interrupted by power failure. In which case,
     * need to find out which one is potentially latest metablock.
     */
    if (num_valid_meta_blocks > 1) {
        cur_meta_block = sst_mblock_latest_meta_block(&h_meta0, &h_meta1);
    } else if (num_valid_meta_blocks == 0) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    sst_flash_fs_ctx.active_metablock = cur_meta_block;
    sst_flash_fs_ctx.scratch_metablock = SST_OTHER_META_BLOCK(cur_meta_block);

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_flash_fs_mblock_cp_remaining_file_meta(uint32_t idx)
{
    psa_ps_status_t err;
    uint32_t end;
    uint32_t meta_block;
    uint32_t pos;
    uint32_t scratch_block;

    scratch_block = sst_cur_meta_scratch_id();
    meta_block = sst_mblock_cur_meta_active();
    /* Calculate the position */
    pos = sst_mblock_file_meta_offset(0);
    /* Copy rest of the block data from previous block */
    /* Data before updated content */
    err = sst_flash_block_to_block_move(scratch_block, pos, meta_block, pos,
                                        (idx * SST_FILE_METADATA_SIZE));
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Data after updated content */
    pos = sst_mblock_file_meta_offset(idx + 1);

    /* Get end of file meta position which is the position after the last
     * byte of file meta.
     */
    end = sst_mblock_file_meta_offset(SST_MAX_NUM_OBJECTS);
    if (end > pos) {
        err = sst_flash_block_to_block_move(scratch_block, pos, meta_block,
                                            pos, (end - pos));
    }

    return err;
}

uint32_t sst_flash_fs_mblock_cur_data_scratch_id(uint32_t lblock)
{
    if (lblock == SST_LOGICAL_DBLOCK0) {
        /* Scratch logical data block 0 physical IDs */
        return sst_cur_meta_scratch_id();
    }

    return sst_flash_fs_ctx.meta_block_header.scratch_dblock;
}

psa_ps_status_t sst_flash_fs_mblock_get_file_idx(uint32_t fid, uint32_t *idx)
{
    psa_ps_status_t err;
    uint32_t i;
    struct sst_file_meta_t tmp_metadata;

    for (i = 0; i < SST_MAX_NUM_OBJECTS; i++) {
        err = sst_flash_fs_mblock_read_file_meta(i, &tmp_metadata);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        /* ID with value 0x00 means end of file meta section */
        if (tmp_metadata.id == fid) {
            /* Found */
            *idx = i;
            return PSA_PS_SUCCESS;
        }
    }

    return PSA_PS_ERROR_UID_NOT_FOUND;
}

psa_ps_status_t sst_flash_fs_mblock_init(void)
{
    psa_ps_status_t err;

    /* Initialize Flash Interface */
    err = sst_flash_init();
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    err = sst_init_get_active_metablock();
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    err = sst_mblock_read_meta_header();
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Erase the other scratch metadata block */
    err = sst_mblock_erase_scratch_blocks();

    return err;
}

psa_ps_status_t sst_flash_fs_mblock_meta_update_finalize(void)
{
    psa_ps_status_t err;

    /* Write the metadata block header to flash */
    err = sst_mblock_write_scratch_meta_header();
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Update the running context */
    sst_mblock_swap_metablocks();

    /* Erase meta block and current scratch block */
    err = sst_mblock_erase_scratch_blocks();

    return err;
}

psa_ps_status_t sst_flash_fs_mblock_migrate_lb0_data_to_scratch(void)
{
    struct sst_block_meta_t block_meta;
    uint32_t current_metablock;
    uint32_t data_size;
    psa_ps_status_t err;
    uint32_t scratch_metablock;

    scratch_metablock = sst_cur_meta_scratch_id();
    current_metablock = sst_mblock_cur_meta_active();

    err = sst_flash_fs_mblock_read_block_metadata(SST_LOGICAL_DBLOCK0,
                                                  &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Calculate data size stored in the B0 block */
    data_size = ((SST_BLOCK_SIZE - block_meta.data_start)
                                                        - block_meta.free_size);

    err = sst_flash_block_to_block_move(scratch_metablock,
                                        block_meta.data_start,
                                        current_metablock,
                                        block_meta.data_start,
                                        data_size);
    return err;
}

psa_ps_status_t sst_flash_fs_mblock_read_file_meta(uint32_t idx,
                                             struct sst_file_meta_t *file_meta)
{
    psa_ps_status_t err;
    uint32_t offset;

    offset = sst_mblock_file_meta_offset(idx);
    err = sst_flash_read(sst_flash_fs_ctx.active_metablock,
                         (uint8_t *)file_meta, offset,
                         SST_FILE_METADATA_SIZE);

#ifdef SST_VALIDATE_METADATA_FROM_FLASH
    if (err == PSA_PS_SUCCESS) {
        err = sst_mblock_validate_file_meta(file_meta);
    }
#endif

    return err;
}

psa_ps_status_t sst_flash_fs_mblock_read_block_metadata(uint32_t lblock,
                                            struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;
    uint32_t metablock;
    uint32_t pos;

    metablock = sst_mblock_cur_meta_active();
    pos = sst_mblock_block_meta_offset(lblock);
    err = sst_flash_read(metablock, (uint8_t *)block_meta,
                         pos, SST_BLOCK_METADATA_SIZE);

#ifdef SST_VALIDATE_METADATA_FROM_FLASH
    if (err == PSA_PS_SUCCESS) {
        err = sst_mblock_validate_block_meta(block_meta);
    }
#endif

    return err;
}

psa_ps_status_t sst_flash_fs_mblock_reserve_file(uint32_t fid, uint32_t size,
                                            uint32_t *idx,
                                            struct sst_file_meta_t *file_meta,
                                            struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;

    err = sst_mblock_reserve_file(fid, size, file_meta, block_meta);

    *idx = sst_get_free_file_index();
    if ((err != PSA_PS_SUCCESS) ||
        (*idx == SST_METADATA_INVALID_INDEX)) {
        return PSA_PS_ERROR_INSUFFICIENT_SPACE;
    }

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_flash_fs_mblock_reset_metablock(void)
{
    struct sst_block_meta_t block_meta;
    psa_ps_status_t err;
    uint32_t i;
    uint32_t metablock_to_erase_first = SST_METADATA_BLOCK0;
    struct sst_file_meta_t file_metadata;

    /* Erase both metadata blocks. If at least one metadata block is valid,
     * ensure that the active metadata block is erased last to prevent rollback
     * in the case of a power failure between the two erases.
     */
    if (sst_init_get_active_metablock() == PSA_PS_SUCCESS) {
        metablock_to_erase_first = sst_flash_fs_ctx.scratch_metablock;
    }

    err = sst_flash_erase_block(metablock_to_erase_first);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    err = sst_flash_erase_block(SST_OTHER_META_BLOCK(metablock_to_erase_first));
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    sst_flash_fs_ctx.meta_block_header.active_swap_count = 0;
    sst_flash_fs_ctx.meta_block_header.scratch_dblock = SST_INIT_SCRATCH_DBLOCK;
    sst_flash_fs_ctx.meta_block_header.fs_version = SST_SUPPORTED_VERSION;
    sst_flash_fs_ctx.scratch_metablock = SST_METADATA_BLOCK1;
    sst_flash_fs_ctx.active_metablock = SST_METADATA_BLOCK0;

    /* Fill the block metadata for logical datablock 0, which has the physical
     * id of the active metadata block. For this datablock, the space available
     * for data is from the end of the metadata to the end of the block.
     */
    block_meta.data_start = SST_ALL_METADATA_SIZE;
    block_meta.free_size = (SST_BLOCK_SIZE - block_meta.data_start);
    block_meta.phy_id = SST_METADATA_BLOCK0;
    err = sst_mblock_update_scratch_block_meta(SST_LOGICAL_DBLOCK0,
                                               &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Fill the block metadata for the dedicated datablocks, which have logical
     * ids beginning from 1 and physical ids initially beginning from
     * SST_INIT_DBLOCK_START. For these datablocks, the space available for
     * data is the entire block.
     */
    block_meta.data_start = 0;
    block_meta.free_size = SST_BLOCK_SIZE;
#if (SST_NUM_DEDICATED_DBLOCKS != 0)
    for (i = SST_INIT_DBLOCK_START; i < SST_NUM_DEDICATED_DBLOCKS; i++) {
        /* If a flash error is detected, the code erases the rest
         * of the blocks anyway to remove all data stored in them.
         */
        err |= sst_flash_erase_block(i);
    }
#endif

    /* If an error is detected while erasing the flash, then return a
     * system error to abort core wipe process.
     */
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_STORAGE_FAILURE;
    }

#if (SST_NUM_DEDICATED_DBLOCKS != 0)
    for (i = 0; i < SST_NUM_DEDICATED_DBLOCKS; i++) {
        block_meta.phy_id = i + SST_INIT_DBLOCK_START;
        err = sst_mblock_update_scratch_block_meta(i + 1, &block_meta);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }
#endif

    /* Initialize file metadata table */
    (void)tfm_memset(&file_metadata, SST_DEFAULT_EMPTY_BUFF_VAL,
                     SST_FILE_METADATA_SIZE);
    for (i = 0; i < SST_MAX_NUM_OBJECTS; i++) {
        /* In the beginning phys id is same as logical id */
        /* Update file metadata to reflect new attributes */
        err = sst_mblock_update_scratch_file_meta(i, &file_metadata);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    err = sst_mblock_write_scratch_meta_header();
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Swap active and scratch metablocks */
    sst_mblock_swap_metablocks();

    return PSA_PS_SUCCESS;
}

void sst_flash_fs_mblock_set_data_scratch(uint32_t phy_id, uint32_t lblock)
{
    if (lblock != SST_LOGICAL_DBLOCK0) {
        sst_flash_fs_ctx.meta_block_header.scratch_dblock = phy_id;
    }
}

psa_ps_status_t sst_flash_fs_mblock_update_scratch_block_meta(
                                            uint32_t lblock,
                                            struct sst_block_meta_t *block_meta)
{
    psa_ps_status_t err;

    /* If the file is the logical block 0, then update the physical ID to the
     * current scratch metadata block so that it is correct after the metadata
     * blocks are swapped.
     */
    if (lblock == SST_LOGICAL_DBLOCK0) {
        block_meta->phy_id = sst_cur_meta_scratch_id();
    }

    err = sst_mblock_update_scratch_block_meta(lblock, block_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    err = sst_mblock_copy_remaining_block_meta(lblock);

    return err;
}

psa_ps_status_t sst_flash_fs_mblock_update_scratch_file_meta(uint32_t idx,
                                              struct sst_file_meta_t *file_meta)
{
    return sst_mblock_update_scratch_file_meta(idx, file_meta);
}
