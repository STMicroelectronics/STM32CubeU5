/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ITS_FLASH_H__
#define __ITS_FLASH_H__

#include <stddef.h>
#include <stdint.h>

#include "flash_layout.h"
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Adjust to match your system's block size */
#ifndef ITS_SECTOR_SIZE
#error "ITS_SECTOR_SIZE must be defined by the target in flash_layout.h file"
#define ITS_BLOCK_SIZE 0
#else
#define ITS_BLOCK_SIZE ITS_SECTOR_SIZE
#endif

/* Adjust to a size that will allow all assets to fit */
#ifndef ITS_NBR_OF_SECTORS
#error "ITS_NBR_OF_SECTORS must be defined by the target in flash_layout.h file"
#define ITS_TOTAL_NUM_OF_BLOCKS 0
#else
#define ITS_TOTAL_NUM_OF_BLOCKS ITS_NBR_OF_SECTORS
#endif

#ifndef ITS_FLASH_PROGRAM_UNIT
#error "ITS_FLASH_PROGRAM_UNIT must be defined in flash_layout.h"
#endif

/* FIXME: Support other flash program units.*/
#if ((ITS_FLASH_PROGRAM_UNIT != 1) && (ITS_FLASH_PROGRAM_UNIT != 2) \
     && (ITS_FLASH_PROGRAM_UNIT != 4) && (ITS_FLASH_PROGRAM_UNIT != 8) \
     && (ITS_FLASH_PROGRAM_UNIT != 16))
#error "The supported ITS_FLASH_PROGRAM_UNIT values are 1, 2, 4 or 8 bytes"
#endif

/* Default value of flash when erased */
#define ITS_FLASH_DEFAULT_VAL 0xFF

/* Invalid block index */
#define ITS_BLOCK_INVALID_ID 0xFFFFFFFF

/**
 * \brief Initialize the Flash Interface.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_init(void);

/**
 * \brief Reads block data from the position specified by block ID and offset.
 *
 * \param[in]  block_id  Block ID
 * \param[out] buff      Buffer pointer to store the data read
 * \param[in]  offset    Offset position from the init of the block
 * \param[in]  size      Number of bytes to read
 *
 * \note This function assumes all input values are valid. That is, the address
 *       range, based on blockid, offset and size, is a valid range in flash.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_read(uint32_t block_id, uint8_t *buff,
                            size_t offset, size_t size);

/**
 * \brief Writes block data to the position specified by block ID and offset.
 *
 * \param[in] block_id  Block ID
 * \param[in] buff      Buffer pointer to the write data
 * \param[in] offset    Offset position from the init of the block
 * \param[in] size      Number of bytes to write
 *
 * \note This function assumes all input values are valid. That is, the address
 *       range, based on blockid, offset and size, is a valid range in flash.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_write(uint32_t block_id, const uint8_t *buff,
                             size_t offset, size_t size);

/**
 * \brief Moves data from src block ID to destination block ID.
 *
 * \param[in] dst_block   Destination block ID
 * \param[in] dst_offset  Destination offset position from the init of the
 *                        destination block
 * \param[in] src_block   Source block ID
 * \param[in] src_offset  Source offset position from the init of the source
 *                        block
 * \param[in] size        Number of bytes to moves
 *
 * \note This function assumes all input values are valid. That is, the address
 *       range, based on blockid, offset and size, is a valid range in flash.
 *       It also assumes that the destination block is already erased and ready
 *       to be written.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_block_to_block_move(uint32_t dst_block,
                                           size_t dst_offset,
                                           uint32_t src_block,
                                           size_t src_offset,
                                           size_t size);

/**
 * \brief Erases block ID data.
 *
 * \param[in] block_id  Block ID
 *
 * \note This function assumes the input value is valid.
 *
 * \return Returns PSA_SUCCESS if the function is executed correctly. Otherwise,
 *         it returns PSA_ERROR_STORAGE_FAILURE.
 */
psa_status_t its_flash_erase_block(uint32_t block_id);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_H__ */
