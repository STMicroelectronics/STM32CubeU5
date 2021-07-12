/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ITS_FLASH_FS_DBLOCK_H__
#define __ITS_FLASH_FS_DBLOCK_H__

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "its_flash_fs_mblock.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Compacts block data for the given logical block.
 *
 * \param[in] lblock      Logical data block to compact
 * \param[in] free_size   Available data size to compact
 * \param[in] src_offset  Offset in the current data block which points to the
 *                        data position to reallocate
 * \param[in] dst_offset  Offset in the scratch block which points to the
 *                        data position to store the data to be reallocated
 * \param[in] size        Number of bytes to be reallocated
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_dblock_compact_block(uint32_t lblock,
                                               size_t free_size,
                                               size_t src_offset,
                                               size_t dst_offset,
                                               size_t size);

/**
 * \brief Copies data from logical block to scratch data block.
 *
 * \param[in] lblock      Logical data block to compact
 * \param[in] offset      Offset in the logical block which points to the
 *                        start position to copy
 * \param[in] size        Number of bytes to be copied from logical block to
 *                        scratch data block
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_dblock_cp_data_to_scratch(uint32_t lblock,
                                                    size_t offset,
                                                    size_t size);

/**
 * \brief Reads the file content.
 *
 * \param[in]  file_meta  File metadata
 * \param[in]  offset     Offset in the file
 * \param[in]  size       Size to be read
 * \param[out] buf        Buffer pointer to store the data
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_dblock_read_file(struct its_file_meta_t *file_meta,
                                           size_t offset,
                                           size_t size,
                                           uint8_t *buf);

/**
 * \brief Writes scratch data block content with requested data
 *        and the rest of the data from the given logical block.
 *
 * \param[in] lblock      Current logical data block
 * \param[in] offset      Offset in the scratch data block where to start the
 *                        copy of the incoming data
 * \param[in] size        Size of the incoming data
 * \param[in] data        Pointer to data buffer to copy in the scratch data
 *                        block
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_dblock_write_file(uint32_t lblock,
                                            size_t offset,
                                            size_t size,
                                            const uint8_t *data);

/**
 * \brief Writes logical block data, which is not related with the file
 *        maipulated, into the scratch block.
 *
 * \param[in] block_meta  Pointer to block meta to process
 * \param[in] file_meta   Pointer to file's metadata manipulated
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_dblock_cp_remaining_data(
                                      const struct its_block_meta_t *block_meta,
                                      const struct its_file_meta_t *file_meta);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_FS_DBLOCK_H__ */
