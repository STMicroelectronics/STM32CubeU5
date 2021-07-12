/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sst_flash_fs.h"

#include <stddef.h>

#include "sst_flash_fs_dblock.h"
#include "sst_flash_fs_mblock.h"
#include "secure_fw/services/secure_storage/flash/sst_flash.h"
#include "secure_fw/services/secure_storage/sst_object_defs.h"
#include "secure_fw/services/secure_storage/sst_utils.h"

#define SST_FLASH_FS_INIT_FILE 0

static psa_ps_status_t sst_flash_fs_file_write_aligned_data(
                                        const struct sst_file_meta_t *file_meta,
                                        uint32_t offset,
                                        uint32_t size,
                                        const uint8_t *data)
{
    psa_ps_status_t err;
    uint32_t f_offset;

#if (SST_FLASH_PROGRAM_UNIT != 1)
    /* Check if offset is aligned with SST_FLASH_PROGRAM_UNIT */
    if (GET_ALIGNED_FLASH_BYTES(offset) != offset) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    /* Check if size is aligned with SST_FLASH_PROGRAM_UNIT */
    if (GET_ALIGNED_FLASH_BYTES(size) != size) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }
#endif /* (SST_FLASH_PROGRAM_UNIT != 1) */

    /* Set offset inside the file where to start to write the content */
    f_offset = (file_meta->data_idx + offset);

    err = sst_flash_fs_dblock_write_file(file_meta->lblock, f_offset,
                                         size, data);
    return err;
}

psa_ps_status_t sst_flash_fs_prepare(void)
{
    /* Initialize metadata block with the valid/active metablock */
    return sst_flash_fs_mblock_init();
}

psa_ps_status_t sst_flash_fs_wipe_all(void)
{
    /* Clean and initialize the metadata block */
    return sst_flash_fs_mblock_reset_metablock();
}

psa_ps_status_t sst_flash_fs_file_exist(uint32_t fid)
{
    psa_ps_status_t err;
    uint32_t idx;

    err = sst_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_flash_fs_file_create(uint32_t fid,
                                         uint32_t max_size,
                                         uint32_t data_size,
                                         const uint8_t *data)
{
    struct sst_block_meta_t block_meta;
    uint32_t cur_phys_block;
    psa_ps_status_t err;
    uint32_t idx;
    struct sst_file_meta_t file_meta;

    /* Check if file already exists */
    err = sst_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err == PSA_PS_SUCCESS) {
        /* If it exits return an error as needs to be removed first */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    /* Check if max_size is aligned with SST_FLASH_PROGRAM_UNIT */
    if (GET_ALIGNED_FLASH_BYTES(max_size) != max_size) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    /* Try to reserve an file based on the input parameters */
    err = sst_flash_fs_mblock_reserve_file(fid, max_size, &idx,
                                           &file_meta, &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Check if data needs to be stored in the new file */
    if (data_size != 0) {
        if ((data_size > max_size) || (data == NULL)) {
            return PSA_PS_ERROR_INVALID_ARGUMENT;
        }

        /* Write the content into scratch data block */
        err = sst_flash_fs_file_write_aligned_data(&file_meta,
                                                   SST_FLASH_FS_INIT_FILE,
                                                   data_size,
                                                   data);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        /* Add current size the file metadata */
        file_meta.cur_size = data_size;

        err = sst_flash_fs_dblock_cp_remaining_data(&block_meta, &file_meta);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }

        cur_phys_block = block_meta.phy_id;

        /* Cur scratch block become the active datablock */
        block_meta.phy_id = sst_flash_fs_mblock_cur_data_scratch_id(
                                                              file_meta.lblock);

        /* Swap the scratch data block */
        sst_flash_fs_mblock_set_data_scratch(cur_phys_block, file_meta.lblock);

    }

    /* Update metadata block information */
    err = sst_flash_fs_mblock_update_scratch_block_meta(file_meta.lblock,
                                                        &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Add file metadata in the metadata block */
    err = sst_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Copy rest of the file metadata entries */
    err = sst_flash_fs_mblock_cp_remaining_file_meta(idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if ((file_meta.lblock != SST_LOGICAL_DBLOCK0) || (data_size == 0)) {
        err = sst_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    /* Write metadata header, swap metadata blocks and erase scratch blocks */
    err = sst_flash_fs_mblock_meta_update_finalize();

    return err;
}

psa_ps_status_t sst_flash_fs_file_get_info(uint32_t fid,
                                           struct sst_file_info_t *info)
{
    psa_ps_status_t err;
    uint32_t idx;
    struct sst_file_meta_t tmp_metadata;

    /* Get the meta data index */
    err = sst_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Read file metadata */
    err = sst_flash_fs_mblock_read_file_meta(idx, &tmp_metadata);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Check if index is still referring to same file */
    if (fid != tmp_metadata.id) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    info->size_max = tmp_metadata.max_size;
    info->size_current = tmp_metadata.cur_size;

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_flash_fs_file_write(uint32_t fid, uint32_t size,
                                        uint32_t offset, const uint8_t *data)
{
    struct sst_block_meta_t block_meta;
    uint32_t cur_phys_block;
    psa_ps_status_t err;
    uint32_t idx;
    struct sst_file_meta_t file_meta;

    /* Get the file index */
    err = sst_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Read file metadata */
    err = sst_flash_fs_mblock_read_file_meta(idx, &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Read block metadata */
    err = sst_flash_fs_mblock_read_block_metadata(file_meta.lblock,
                                                  &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Write the content into scratch data block */
    err = sst_flash_fs_file_write_aligned_data(&file_meta, offset, size, data);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    if (size > file_meta.cur_size) {
        /* Update the file metadata */
        file_meta.cur_size = size;
    }

    err = sst_flash_fs_dblock_cp_remaining_data(&block_meta, &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    cur_phys_block = block_meta.phy_id;

    /* Cur scratch block become the active datablock */
    block_meta.phy_id = sst_flash_fs_mblock_cur_data_scratch_id(
                                                              file_meta.lblock);

    /* Swap the scratch data block */
    sst_flash_fs_mblock_set_data_scratch(cur_phys_block, file_meta.lblock);

    /* Update block metadata in scratch metadata block */
    err = sst_flash_fs_mblock_update_scratch_block_meta(file_meta.lblock,
                                                        &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Update file metadata to reflect new attributes */
    err = sst_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Copy rest of the file metadata entries */
    err = sst_flash_fs_mblock_cp_remaining_file_meta(idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* The file data in the logical block 0 is stored in same physical block
     * where the metadata is stored. A change in the metadata requires a
     * swap of physical blocks. So, the file data stored in the current
     * metadata block needs to be copied in the scratch block, if the data
     * of the file processed is not located in the logical block 0. When an
     * file data is located in the logical block 0, that copy has been done
     * while processing the file data.
     */
    if (file_meta.lblock != SST_LOGICAL_DBLOCK0) {
        err = sst_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    /* Update the metablock header, swap scratch and active blocks,
     * erase scratch blocks.
     */
    err = sst_flash_fs_mblock_meta_update_finalize();

    return err;
}

psa_ps_status_t sst_flash_fs_file_delete(uint32_t fid)
{
    uint32_t del_file_data_idx;
    uint32_t del_file_lblock;
    uint32_t del_file_idx;
    uint32_t del_file_max_size;
    psa_ps_status_t err;
    uint32_t src_offset = SST_BLOCK_SIZE;
    uint32_t nbr_bytes_to_move = 0;
    uint32_t idx;
    struct sst_file_meta_t file_meta;

    /* Get the file index */
    err = sst_flash_fs_mblock_get_file_idx(fid, &del_file_idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    err = sst_flash_fs_mblock_read_file_meta(del_file_idx, &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    if (sst_utils_validate_fid(file_meta.id) != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Save logical block, data_index and max_size to be used later on */
    del_file_lblock = file_meta.lblock;
    del_file_data_idx = file_meta.data_idx;
    del_file_max_size = file_meta.max_size;

    /* Remove file metadata */
    file_meta.id = SST_INVALID_FID;
    file_meta.lblock = 0;
    file_meta.max_size = 0;
    file_meta.cur_size = 0;

    /* Update file metadata in to the scratch block */
    err = sst_flash_fs_mblock_update_scratch_file_meta(del_file_idx,
                                                       &file_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Read all file metadata */
    for (idx = 0; idx < SST_MAX_NUM_OBJECTS; idx++) {
        if (idx == del_file_idx) {
            /* Skip deleted file */
            continue;
        }

        /* Read file meta for the given file index */
        err = sst_flash_fs_mblock_read_file_meta(idx, &file_meta);
        if (err != PSA_PS_SUCCESS) {
            return err;
        }

        /* Check if the file is located in the same logical block and has a
         * valid FID.
         */
        if ((file_meta.lblock == del_file_lblock) &&
            (file_meta.id != SST_INVALID_FID)) {
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
        err = sst_flash_fs_mblock_update_scratch_file_meta(idx, &file_meta);
        if (err != PSA_PS_SUCCESS) {
            return err;
        }
    }

    /* Compact data block */
    err = sst_flash_fs_dblock_compact_block(del_file_lblock, del_file_max_size,
                                            src_offset, del_file_data_idx,
                                            nbr_bytes_to_move);
    if (err != PSA_PS_SUCCESS) {
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
    if (del_file_lblock != SST_LOGICAL_DBLOCK0) {
        err = sst_flash_fs_mblock_migrate_lb0_data_to_scratch();
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    /* Update the metablock header, swap scratch and active blocks,
     * erase scratch blocks.
     */
    err = sst_flash_fs_mblock_meta_update_finalize();

    return err;
}

psa_ps_status_t sst_flash_fs_file_read(uint32_t fid, uint32_t size,
                                       uint32_t offset, uint8_t *data)
{
    psa_ps_status_t err;
    uint32_t idx;
    struct sst_file_meta_t tmp_metadata;

    /* Get the file index */
    err = sst_flash_fs_mblock_get_file_idx(fid, &idx);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Read file metadata */
    err = sst_flash_fs_mblock_read_file_meta(idx, &tmp_metadata);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Check if index is still referring to same file */
    if (fid != tmp_metadata.id) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    /* Boundary check the incoming request */
    err = sst_utils_check_contained_in(tmp_metadata.cur_size, offset, size);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Read the file from flash */
    err = sst_flash_fs_dblock_read_file(&tmp_metadata, offset, size, data);
    if (err != PSA_PS_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    return PSA_PS_SUCCESS;
}
