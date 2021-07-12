/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SST_FLASH_H__
#define __SST_FLASH_H__

#include <stdint.h>
#include "psa/protected_storage.h"
#include "flash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Adjust to match your system's block size */
#ifndef SST_SECTOR_SIZE
#error "SST_SECTOR_SIZE must be defined by the target in flash_layout.h file"
#define SST_BLOCK_SIZE 0
#else
#define SST_BLOCK_SIZE SST_SECTOR_SIZE
#endif

/* Adjust to a size that will allow all assets to fit */
#ifndef SST_NBR_OF_SECTORS
#error "SST_NBR_OF_SECTORS must be defined by the target in flash_layout.h file"
#define SST_TOTAL_NUM_OF_BLOCKS 0
#else
#define SST_TOTAL_NUM_OF_BLOCKS SST_NBR_OF_SECTORS
#endif

#ifndef SST_FLASH_PROGRAM_UNIT
#error "SST_FLASH_PROGRAM_UNIT must be defined in flash_layout.h"
#endif

#if (SST_FLASH_PROGRAM_UNIT < 1)
#error "SST_FLASH_PROGRAM_UNIT must be bigger than 0"
#endif

/* Default value of flash when erased */
#define SST_FLASH_DEFAULT_VAL 0xFF
/* A single host specific sst_flash_xxx.c should be included during compile */

/* Invalid block index */
#define SST_BLOCK_INVALID_ID 0xFFFFFFFF

/**
 * \brief  Initialize the Flash Interface.
 *
 * \return Returns PSA_PS_SUCCESS if the function is executed correctly.
 *         Otherwise, it returns PSA_PS_ERROR_STORAGE_FAILURE.
 */
psa_ps_status_t sst_flash_init(void);

/**
 * \brief Reads block data from the position specified by block ID and offset.
 *
 * \param[in]  block_id  Block ID
 * \param[out] buff      Buffer pointer to store the data read
 * \param[in]  offset    Offset position from the init of the block
 * \param[in]  size      Number of bytes to read
 *
 * \note This function considers all input values are valid. That means,
 *       the range of address, based on blockid + offset + size, are always
 *       valid in the memory.
 *
 * \return Returns PSA_PS_SUCCESS if the function is executed correctly.
 *         Otherwise, it returns PSA_PS_ERROR_STORAGE_FAILURE.
 */
psa_ps_status_t sst_flash_read(uint32_t block_id, uint8_t *buff,
                               uint32_t offset, uint32_t size);

/**
 * \brief Writes block data to the position specified by block ID and offset.
 *
 * \param[in] block_id  Block ID
 * \param[in] buff      Buffer pointer to the write data
 * \param[in] offset    Offset position from the init of the block
 * \param[in] size      Number of bytes to write
 *
 * \note This function considers all input values are valid. That means,
 *       the range of address, based on blockid + offset + size, are always
 *       valid in the memory.
 *
 * \return Returns PSA_PS_SUCCESS if the function is executed correctly.
 *         Otherwise, it returns PSA_PS_ERROR_STORAGE_FAILURE.
 */
psa_ps_status_t sst_flash_write(uint32_t block_id, const uint8_t *buff,
                                uint32_t offset, uint32_t size);

/**
 * \brief Moves data from src block ID to destination block ID.
 *
 * \param[in] dst_block  Destination block ID
 * \param[in] dst_offset Destination offset position from the init of the
 *                       destination block
 * \param[in] src_block  Source block ID
 * \param[in] src_offset Source offset position from the init of the source
 *                       block
 * \param[in] size       Number of bytes to moves
 *
 * \note This function considers all input values are valid. That means,
 *       the range of address, based on block_id + offset + size, are always
 *       valid in the memory.
 *       It also considers that the destination block is already erased and
 *       ready to be written.
 *
 * \return Returns PSA_PS_SUCCESS if the function is executed correctly.
 *         Otherwise, it returns PSA_PS_ERROR_STORAGE_FAILURE.
 */
psa_ps_status_t sst_flash_block_to_block_move(uint32_t dst_block,
                                              uint32_t dst_offset,
                                              uint32_t src_block,
                                              uint32_t src_offset,
                                              uint32_t size);

/**
 * \brief Erases block ID data.
 *
 * \param[in] block_id  Block ID
 *
 * \note This function considers all input values valids.
 *
 * \return Returns PSA_PS_SUCCESS if the function is executed correctly.
 *         Otherwise, it returns PSA_PS_ERROR_STORAGE_FAILURE.
 */
psa_ps_status_t sst_flash_erase_block(uint32_t block_id);

#ifdef __cplusplus
}
#endif

#endif /* __SST_FLASH_H__ */
