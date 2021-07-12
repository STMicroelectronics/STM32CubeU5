/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sst_flash_fs_dblock.h"

#include "secure_fw/services/secure_storage/sst_utils.h"
#include "secure_fw/services/secure_storage/flash/sst_flash.h"

/**
 * \brief Converts logical data block number to physical number.
 *
 * \param[in] lblock  Logical block number
 *
 * \return Return physical block number.
 */
static uint32_t sst_dblock_lo_to_phy(uint32_t lblock)
{
    struct sst_block_meta_t block_meta;
    psa_ps_status_t err;

    err = sst_flash_fs_mblock_read_block_metadata(lblock, &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return SST_BLOCK_INVALID_ID;
    }

    return block_meta.phy_id;
}

psa_ps_status_t sst_flash_fs_dblock_compact_block(uint32_t lblock,
                                                  uint32_t free_size,
                                                  uint32_t src_offset,
                                                  uint32_t dst_offset,
                                                  uint32_t size)
{
    struct sst_block_meta_t block_meta;
    psa_ps_status_t err;
    uint32_t scratch_id = 0;

    /* Read current block meta */
    err = sst_flash_fs_mblock_read_block_metadata(lblock, &block_meta);
    if (err != PSA_PS_SUCCESS) {
        return err;
    }

    /* Release data from block meta */
    block_meta.free_size += free_size;

    /* Save scratch data block physical IDs */
    scratch_id = sst_flash_fs_mblock_cur_data_scratch_id(lblock);

    /* Check if there are bytes to be compacted */
    if (size > 0) {
        /* Move data from source offset in current data block to scratch block
         * destination offset.
         */
        err = sst_flash_block_to_block_move(scratch_id, dst_offset,
                                            block_meta.phy_id, src_offset,
                                            size);
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    if (dst_offset > block_meta.data_start) {
        /* Copy data from the beginning of data block until
         * the position where the data will be reallocated later
         */
        err = sst_flash_block_to_block_move(scratch_id,
                                            block_meta.data_start,
                                            block_meta.phy_id,
                                            block_meta.data_start,
                                            (dst_offset-block_meta.data_start));
        if (err != PSA_PS_SUCCESS) {
            return PSA_PS_ERROR_OPERATION_FAILED;
        }
    }

    /* Swap the scratch and current data blocks. Must swap even with nothing
     * to compact so that deleted file is left in scratch and erased as part
     * of finalization.
     */
    sst_flash_fs_mblock_set_data_scratch(block_meta.phy_id, lblock);

    /* Set scratch block ID as the one which contains the new data block */
    block_meta.phy_id = scratch_id;

    /* Update block metadata in scratch metadata block */
    err = sst_flash_fs_mblock_update_scratch_block_meta(lblock, &block_meta);
    if (err != PSA_PS_SUCCESS) {
        /* Swap back the data block as there was an issue in the process */
        sst_flash_fs_mblock_set_data_scratch(scratch_id, lblock);
        return err;
    }

    return err;
}

psa_ps_status_t sst_flash_fs_dblock_cp_data_to_scratch(uint32_t lblock,
                                                       uint32_t offset,
                                                       uint32_t size)
{
    uint32_t phys_block;
    uint32_t scratch_id;

    /* Get physical block ID from where to read the data */
    phys_block = sst_dblock_lo_to_phy(lblock);
    if (phys_block == SST_BLOCK_INVALID_ID) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    /* Get the scratch data block ID to write the data */
    scratch_id = sst_flash_fs_mblock_cur_data_scratch_id(lblock);

    /* Data after updated content */
    return sst_flash_block_to_block_move(scratch_id, offset,
                                         phys_block, offset,
                                         size);
}

psa_ps_status_t sst_flash_fs_dblock_read_file(struct sst_file_meta_t *file_meta,
                                              uint32_t offset,
                                              uint32_t size,
                                              uint8_t *buf)
{
    uint32_t phys_block;
    uint32_t pos;

    phys_block = sst_dblock_lo_to_phy(file_meta->lblock);
    if (phys_block == SST_BLOCK_INVALID_ID) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    pos = (file_meta->data_idx + offset);

    return sst_flash_read(phys_block, buf, pos, size);
}

psa_ps_status_t sst_flash_fs_dblock_write_file(uint32_t lblock,
                                               uint32_t offset,
                                               uint32_t size,
                                               const uint8_t *data)
{
    uint32_t scratch_id;

    scratch_id = sst_flash_fs_mblock_cur_data_scratch_id(lblock);

    return sst_flash_write(scratch_id, data, offset, size);
}

psa_ps_status_t sst_flash_fs_dblock_cp_remaining_data(
                                      const struct sst_block_meta_t *block_meta,
                                      const struct sst_file_meta_t *file_meta)
{
    uint32_t after_file_offset;
    psa_ps_status_t err;
    uint32_t scratch_id;
    uint32_t wrt_bytes;

    scratch_id = sst_flash_fs_mblock_cur_data_scratch_id(file_meta->lblock);

    if (file_meta->data_idx > block_meta->data_start) {
        /* Move data before the referenced object */
        wrt_bytes = (file_meta->data_idx - block_meta->data_start);

        err = sst_flash_block_to_block_move(scratch_id,
                                            block_meta->data_start,
                                            block_meta->phy_id,
                                            block_meta->data_start,
                                            wrt_bytes);
        if (err != PSA_PS_SUCCESS) {
            return err;
        }

    }

    /* The referenced file data is already in the scratch block, as it is
     * processed before calling of this function.
     */
    after_file_offset = file_meta->data_idx + file_meta->max_size;

    /* Calculate amount of bytes after the manipulated file */
    wrt_bytes = (SST_BLOCK_SIZE - (after_file_offset + block_meta->free_size));

    /* Data after updated content */
    return sst_flash_block_to_block_move(scratch_id, after_file_offset,
                                         block_meta->phy_id, after_file_offset,
                                         wrt_bytes);
}
