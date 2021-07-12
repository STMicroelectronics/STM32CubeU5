/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_fs_dblock.h"

#include "secure_fw/services/internal_trusted_storage/flash/its_flash.h"

/**
 * \brief Converts logical data block number to physical number.
 *
 * \param[in] lblock  Logical block number
 *
 * \return Return physical block number.
 */
static uint32_t its_dblock_lo_to_phy(uint32_t lblock)
{
    struct its_block_meta_t block_meta;
    psa_status_t err;

    err = its_flash_fs_mblock_read_block_metadata(lblock, &block_meta);
    if (err != PSA_SUCCESS) {
        return ITS_BLOCK_INVALID_ID;
    }

    return block_meta.phy_id;
}

psa_status_t its_flash_fs_dblock_compact_block(uint32_t lblock,
                                               size_t free_size,
                                               size_t src_offset,
                                               size_t dst_offset,
                                               size_t size)
{
    struct its_block_meta_t block_meta;
    psa_status_t err;
    uint32_t scratch_id = 0;

    /* Read current block meta */
    err = its_flash_fs_mblock_read_block_metadata(lblock, &block_meta);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Release data from block meta */
    block_meta.free_size += free_size;

    /* Save scratch data block physical IDs */
    scratch_id = its_flash_fs_mblock_cur_data_scratch_id(lblock);

    /* Check if there are bytes to be compacted */
    if (size > 0) {
        /* Move data from source offset in current data block to scratch block
         * destination offset.
         */
        err = its_flash_block_to_block_move(scratch_id, dst_offset,
                                            block_meta.phy_id, src_offset,
                                            size);
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    if (dst_offset > block_meta.data_start) {
        /* Copy data from the beginning of data block until
         * the position where the data will be reallocated later
         */
        err = its_flash_block_to_block_move(scratch_id,
                                            block_meta.data_start,
                                            block_meta.phy_id,
                                            block_meta.data_start,
                                            (dst_offset-block_meta.data_start));
        if (err != PSA_SUCCESS) {
            return PSA_ERROR_GENERIC_ERROR;
        }
    }

    /* Swap the scratch and current data blocks. Must swap even with nothing
     * to compact so that deleted file is left in scratch and erased as part
     * of finalization.
     */
    its_flash_fs_mblock_set_data_scratch(block_meta.phy_id, lblock);

    /* Set scratch block ID as the one which contains the new data block */
    block_meta.phy_id = scratch_id;

    /* Update block metadata in scratch metadata block */
    err = its_flash_fs_mblock_update_scratch_block_meta(lblock, &block_meta);
    if (err != PSA_SUCCESS) {
        /* Swap back the data block as there was an issue in the process */
        its_flash_fs_mblock_set_data_scratch(scratch_id, lblock);
        return err;
    }

    return err;
}

psa_status_t its_flash_fs_dblock_cp_data_to_scratch(uint32_t lblock,
                                                    size_t offset,
                                                    size_t size)
{
    uint32_t phys_block;
    uint32_t scratch_id;

    /* Get physical block ID from where to read the data */
    phys_block = its_dblock_lo_to_phy(lblock);
    if (phys_block == ITS_BLOCK_INVALID_ID) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Get the scratch data block ID to write the data */
    scratch_id = its_flash_fs_mblock_cur_data_scratch_id(lblock);

    /* Data after updated content */
    return its_flash_block_to_block_move(scratch_id, offset,
                                         phys_block, offset,
                                         size);
}

psa_status_t its_flash_fs_dblock_read_file(struct its_file_meta_t *file_meta,
                                           size_t offset,
                                           size_t size,
                                           uint8_t *buf)
{
    uint32_t phys_block;
    size_t pos;

    phys_block = its_dblock_lo_to_phy(file_meta->lblock);
    if (phys_block == ITS_BLOCK_INVALID_ID) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    pos = (file_meta->data_idx + offset);

    return its_flash_read(phys_block, buf, pos, size);
}

psa_status_t its_flash_fs_dblock_write_file(uint32_t lblock,
                                            size_t offset,
                                            size_t size,
                                            const uint8_t *data)
{
    uint32_t scratch_id;

    scratch_id = its_flash_fs_mblock_cur_data_scratch_id(lblock);

    return its_flash_write(scratch_id, data, offset, size);
}

psa_status_t its_flash_fs_dblock_cp_remaining_data(
                                      const struct its_block_meta_t *block_meta,
                                      const struct its_file_meta_t *file_meta)
{
    size_t after_file_offset;
    psa_status_t err;
    uint32_t scratch_id;
    size_t wrt_bytes;

    scratch_id = its_flash_fs_mblock_cur_data_scratch_id(file_meta->lblock);

    if (file_meta->data_idx > block_meta->data_start) {
        /* Move data before the referenced file */
        wrt_bytes = (file_meta->data_idx - block_meta->data_start);

        err = its_flash_block_to_block_move(scratch_id,
                                            block_meta->data_start,
                                            block_meta->phy_id,
                                            block_meta->data_start,
                                            wrt_bytes);
        if (err != PSA_SUCCESS) {
            return err;
        }

    }

    /* The referenced file data is already in the scratch block, as it is
     * processed before calling of this function.
     */
    after_file_offset = file_meta->data_idx + file_meta->max_size;

    /* Calculate amount of bytes after the manipulated file */
    wrt_bytes = (ITS_BLOCK_SIZE - (after_file_offset + block_meta->free_size));

    /* Data after updated content */
    return its_flash_block_to_block_move(scratch_id, after_file_offset,
                                         block_meta->phy_id, after_file_offset,
                                         wrt_bytes);
}
