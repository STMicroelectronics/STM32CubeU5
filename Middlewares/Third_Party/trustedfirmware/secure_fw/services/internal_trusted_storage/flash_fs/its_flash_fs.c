/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_fs.h"

#include "its_flash_fs_dblock.h"
#include "its_flash_fs_mblock.h"
#include "tfm_memory_utils.h"
#include "secure_fw/services/internal_trusted_storage/flash/its_flash.h"
#include "secure_fw/services/internal_trusted_storage/its_utils.h"

#define ITS_FLASH_FS_INIT_FILE 0

static psa_status_t its_flash_fs_file_write_aligned_data(
                                        const struct its_file_meta_t *file_meta,
                                        size_t offset,
                                        size_t size,
                                        const uint8_t *data)
{
    size_t f_offset;

#if (ITS_FLASH_PROGRAM_UNIT != 1)
    /* Check if offset is aligned with ITS_FLASH_PROGRAM_UNIT */
    if (GET_ALIGNED_FLASH_BYTES(offset) != offset) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set the size to be aligned with ITS_FLASH_PROGRAM_UNIT */
    size = GET_ALIGNED_FLASH_BYTES(size);
#endif /* (ITS_FLASH_PROGRAM_UNIT != 1) */

    /* Set offset inside the file where to start to write the content */
    f_offset = (file_meta->data_idx + offset);

    return its_flash_fs_dblock_write_file(file_meta->lblock, f_offset,
                                          size, data);
}

psa_status_t its_flash_fs_prepare(void)
{
    /* Initialize metadata block with the valid/active metablock */
    return its_flash_fs_mblock_init();
}

psa_status_t its_flash_fs_wipe_all(void)
{
    /* Clean and initialize the metadata block */
    return its_flash_fs_mblock_reset_metablock();
}

psa_status_t its_flash_fs_file_exist(const uint8_t *fid)
{
    psa_status_t err;
    uint32_t idx;

    err = its_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_file_create(const uint8_t *fid,
                                      size_t max_size,
                                      size_t data_size,
                                      uint32_t flags,
                                      const uint8_t *data)
{
    struct its_block_meta_t block_meta;
    uint32_t cur_phys_block;
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t file_meta;

    /* Check if file already exists */
    err = its_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err == PSA_SUCCESS) {
        /* If it exits return an error as needs to be removed first */
        return PSA_ERROR_INVALID_ARGUMENT;
    }

#if (ITS_FLASH_PROGRAM_UNIT != 1)
    /* Set the max_size to be aligned with ITS_FLASH_PROGRAM_UNIT */
    max_size = GET_ALIGNED_FLASH_BYTES(max_size);
#endif

    /* Try to reserve an file based on the input parameters */
    err = its_flash_fs_mblock_reserve_file(fid, max_size, flags, &idx,
                                           &file_meta, &block_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Check if data needs to be stored in the new file */
    if (data_size != 0) {
        if ((data_size > max_size) || (data == NULL)) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        /* Write the content into scratch data block */
        err = its_flash_fs_file_write_aligned_data(&file_meta,
                                                   ITS_FLASH_FS_INIT_FILE,
                                                   data_size,
                                                   data);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        /* Add current size to the file metadata */
        file_meta.cur_size = data_size;

        err = its_flash_fs_dblock_cp_remaining_data(&block_meta, &file_meta);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        cur_phys_block = block_meta.phy_id;

        /* Cur scratch block become the active datablock */
        block_meta.phy_id = its_flash_fs_mblock_cur_data_scratch_id(
                                                              file_meta.lblock);

        /* Swap the scratch data block */
        its_flash_fs_mblock_set_data_scratch(cur_phys_block, file_meta.lblock);

    }

    /* Update metadata block information */
    err = its_flash_fs_mblock_update_scratch_block_meta(file_meta.lblock,
                                                        &block_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Add file metadata in the metadata block */
    err = its_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Copy rest of the file metadata entries */
    err = its_flash_fs_mblock_cp_remaining_file_meta(idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if ((file_meta.lblock != ITS_LOGICAL_DBLOCK0) || (data_size == 0)) {
        err = its_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Write metadata header, swap metadata blocks and erase scratch blocks */
    return its_flash_fs_mblock_meta_update_finalize();
}

psa_status_t its_flash_fs_file_get_info(const uint8_t *fid,
                                        struct its_file_info_t *info)
{
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t tmp_metadata;

    /* Get the meta data index */
    err = its_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read file metadata */
    err = its_flash_fs_mblock_read_file_meta(idx, &tmp_metadata);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Check if index is still referring to same file */
    if (tfm_memcmp(fid, tmp_metadata.id, ITS_FILE_ID_SIZE)) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    info->size_max = tmp_metadata.max_size;
    info->size_current = tmp_metadata.cur_size;
    info->flags = tmp_metadata.flags;

    return PSA_SUCCESS;
}

psa_status_t its_flash_fs_file_write(const uint8_t *fid,
                                     size_t size,
                                     size_t offset,
                                     const uint8_t *data)
{
    struct its_block_meta_t block_meta;
    uint32_t cur_phys_block;
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t file_meta;

    /* Get the file index */
    err = its_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read file metadata */
    err = its_flash_fs_mblock_read_file_meta(idx, &file_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read block metadata */
    err = its_flash_fs_mblock_read_block_metadata(file_meta.lblock,
                                                  &block_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Write the content into scratch data block */
    err = its_flash_fs_file_write_aligned_data(&file_meta, offset, size, data);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (size > file_meta.cur_size) {
        /* Update the file metadata */
        file_meta.cur_size = size;
    }

    err = its_flash_fs_dblock_cp_remaining_data(&block_meta, &file_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    cur_phys_block = block_meta.phy_id;

    /* Cur scratch block become the active datablock */
    block_meta.phy_id = its_flash_fs_mblock_cur_data_scratch_id(
                                                              file_meta.lblock);

    /* Swap the scratch data block */
    its_flash_fs_mblock_set_data_scratch(cur_phys_block, file_meta.lblock);

    /* Update block metadata in scratch metadata block */
    err = its_flash_fs_mblock_update_scratch_block_meta(file_meta.lblock,
                                                        &block_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Update file metadata to reflect new attributes */
    err = its_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Copy rest of the file metadata entries */
    err = its_flash_fs_mblock_cp_remaining_file_meta(idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if (file_meta.lblock != ITS_LOGICAL_DBLOCK0) {
        err = its_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Update the metablock header, swap scratch and active blocks,
     * erase scratch blocks.
     */
    return its_flash_fs_mblock_meta_update_finalize();
}

psa_status_t its_flash_fs_file_delete(const uint8_t *fid)
{
    size_t del_file_data_idx;
    uint32_t del_file_lblock;
    uint32_t del_file_idx;
    size_t del_file_max_size;
    psa_status_t err;
    size_t src_offset = ITS_BLOCK_SIZE;
    size_t nbr_bytes_to_move = 0;
    uint32_t idx;
    struct its_file_meta_t file_meta;

    /* Get the file index */
    err = its_flash_fs_mblock_get_file_idx(fid, &del_file_idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    err = its_flash_fs_mblock_read_file_meta(del_file_idx, &file_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    if (its_utils_validate_fid(file_meta.id) != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Save logical block, data_index and max_size to be used later on */
    del_file_lblock = file_meta.lblock;
    del_file_data_idx = file_meta.data_idx;
    del_file_max_size = file_meta.max_size;

    /* Remove file metadata */
    file_meta = (struct its_file_meta_t){0};

    /* Update file metadata in to the scratch block */
    err = its_flash_fs_mblock_update_scratch_file_meta(del_file_idx,
                                                       &file_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Read all file metadata */
    for (idx = 0; idx < ITS_MAX_NUM_FILES; idx++) {
        if (idx == del_file_idx) {
            /* Skip deleted file */
            continue;
        }

        /* Read file meta for the given file index */
        err = its_flash_fs_mblock_read_file_meta(idx, &file_meta);
        if (err != PSA_SUCCESS) {
            return err;
        }

        /* Check if the file is located in the same logical block and has a
         * valid FID.
         */
        if ((file_meta.lblock == del_file_lblock) &&
            (its_utils_validate_fid(file_meta.id) == PSA_SUCCESS)) {
            /* If a file is located after the data to delete, this
             * needs to be moved.
             */
            if (file_meta.data_idx > del_file_data_idx) {
                /* Check if this is the position after the deleted
                 * data. This will be the first file data to move.
                 */
                if (src_offset > file_meta.data_idx) {
                    src_offset = file_meta.data_idx;
                }

                /* Set the new file data index location in the
                 * data block.
                 */
                file_meta.data_idx -= del_file_max_size;

                /* Increase number of bytes to move */
                nbr_bytes_to_move += file_meta.max_size;
            }
        }
        /* Update file metadata in to the scratch block */
        err = its_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
        if (err != PSA_SUCCESS) {
            return err;
        }
    }

    /* Compact data block */
    err = its_flash_fs_dblock_compact_block(del_file_lblock, del_file_max_size,
                                            src_offset, del_file_data_idx,
                                            nbr_bytes_to_move);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if (del_file_lblock != ITS_LOGICAL_DBLOCK0) {
        err = its_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Update the metablock header, swap scratch and active blocks,
     * erase scratch blocks.
     */
    return its_flash_fs_mblock_meta_update_finalize();
}

psa_status_t its_flash_fs_file_read(const uint8_t *fid,
                                    size_t size,
                                    size_t offset,
                                    uint8_t *data)
{
    psa_status_t err;
    uint32_t idx;
    struct its_file_meta_t tmp_metadata;

    /* Get the file index */
    err = its_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Read file metadata */
    err = its_flash_fs_mblock_read_file_meta(idx, &tmp_metadata);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Check if index is still referring to same file */
    if (tfm_memcmp(fid, tmp_metadata.id, ITS_FILE_ID_SIZE)) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Boundary check the incoming request */
    err = its_utils_check_contained_in(tmp_metadata.cur_size, offset, size);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Read the file from flash */
    err = its_flash_fs_dblock_read_file(&tmp_metadata, offset, size, data);
    if (err != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}
