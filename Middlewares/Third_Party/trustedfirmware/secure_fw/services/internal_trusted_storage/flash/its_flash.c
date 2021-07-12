/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash.h"

#include "Driver_Flash.h"
#include "secure_fw/services/internal_trusted_storage/its_utils.h"
#include "tfm_memory_utils.h"

#ifndef ITS_FLASH_AREA_ADDR
#error "ITS_FLASH_AREA_ADDR must be defined in flash_layout.h file"
#endif

#ifndef ITS_FLASH_DEV_NAME
#error "ITS_FLASH_DEV_NAME must be defined in flash_layout.h file"
#endif

/* Import the CMSIS flash device driver */
extern ARM_DRIVER_FLASH ITS_FLASH_DEV_NAME;

#define BLOCK_START_OFFSET  0
#define MAX_BLOCK_DATA_COPY 256

#ifdef ITS_RAM_FS
#define BLOCK_DATA_SIZE (ITS_BLOCK_SIZE * ITS_TOTAL_NUM_OF_BLOCKS)

static uint8_t block_data[BLOCK_DATA_SIZE] = {0};
#endif

/**
 * \brief Gets physical address of the given block ID.
 *
 * \param[in] block_id  Block ID
 * \param[in] offset    Offset position from the init of the block
 *
 * \returns Returns physical address for the given block ID.
 */
__attribute__((always_inline))
static inline uint32_t get_phys_address(uint32_t block_id, size_t offset)
{
    return (ITS_FLASH_AREA_ADDR + (block_id * ITS_BLOCK_SIZE) + offset);
}

#ifdef ITS_RAM_FS
static psa_status_t flash_init(void)
{
    /* Nothing needs to be done in case of flash emulated in RAM */
    return PSA_SUCCESS;
}

static psa_status_t flash_read(uint32_t flash_addr, size_t size, uint8_t *buff)
{
    uint32_t idx = flash_addr - ITS_FLASH_AREA_ADDR;

    (void)tfm_memcpy(buff, &block_data[idx], size);

    return PSA_SUCCESS;
}

static psa_status_t flash_write(uint32_t flash_addr, size_t size,
                                const uint8_t *buff)
{
    uint32_t idx = flash_addr - ITS_FLASH_AREA_ADDR;

    (void)tfm_memcpy(&block_data[idx], buff, size);

    return PSA_SUCCESS;
}

static psa_status_t flash_erase(uint32_t flash_addr)
{
    uint32_t idx = flash_addr - ITS_FLASH_AREA_ADDR;

    (void)tfm_memset(&block_data[idx], ITS_FLASH_DEFAULT_VAL, ITS_BLOCK_SIZE);

    return PSA_SUCCESS;
}
#else /* ITS_RAM_FS */
static psa_status_t flash_init(void)
{
    int32_t err;

    err = ITS_FLASH_DEV_NAME.Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t flash_read(uint32_t flash_addr, size_t size, uint8_t *buff)
{
    int32_t err;

    err = ITS_FLASH_DEV_NAME.ReadData(flash_addr, buff, size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t flash_write(uint32_t flash_addr, size_t size,
                                const uint8_t *buff)
{
    int32_t err;

    err = ITS_FLASH_DEV_NAME.ProgramData(flash_addr, buff, size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t flash_erase(uint32_t flash_addr)
{
    int32_t err;

    err = ITS_FLASH_DEV_NAME.EraseSector(flash_addr);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}
#endif /* ITS_RAM_FS */

psa_status_t its_flash_init(void)
{
    return flash_init();
}

psa_status_t its_flash_read(uint32_t block_id, uint8_t *buff,
                            size_t offset, size_t size)
{
    uint32_t flash_addr;

    /* Gets flash address location defined by block ID and offset
     * parameters.
     */
    flash_addr = get_phys_address(block_id, offset);

    return flash_read(flash_addr, size, buff);
}

psa_status_t its_flash_write(uint32_t block_id, const uint8_t *buff,
                             size_t offset, size_t size)
{
    uint32_t flash_addr;

    /* Gets flash address location defined by block ID and offset
     * parameters.
     */
    flash_addr = get_phys_address(block_id, offset);

    return flash_write(flash_addr, size, buff);
}

psa_status_t its_flash_block_to_block_move(uint32_t dst_block,
                                           size_t dst_offset,
                                           uint32_t src_block,
                                           size_t src_offset,
                                           size_t size)
{
    psa_status_t err;
    uint8_t dst_block_data_copy[MAX_BLOCK_DATA_COPY];
    uint32_t dst_flash_addr;
    uint32_t src_flash_addr;
    size_t bytes_to_move;

    /* Gets flash addresses defined by block ID and offset parameters */
    src_flash_addr = get_phys_address(src_block, src_offset);
    dst_flash_addr = get_phys_address(dst_block, dst_offset);

    while (size > 0) {
        /* Calculates the number of bytes to move */
        bytes_to_move = ITS_UTILS_MIN(size, MAX_BLOCK_DATA_COPY);

        /* Reads data from source block and store it in the in-memory copy of
         * destination content.
         */
        err = flash_read(src_flash_addr, bytes_to_move, dst_block_data_copy);
        if (err != PSA_SUCCESS) {
            return err;
        }

        /* Writes in flash the in-memory block content after modification */
        err = flash_write(dst_flash_addr, bytes_to_move, dst_block_data_copy);
        if (err != PSA_SUCCESS) {
            return err;
        }

        /* Updates pointers to the source and destination flash regions */
        src_flash_addr += bytes_to_move;
        dst_flash_addr += bytes_to_move;

        /* Decrement remaining size to move */
        size -= bytes_to_move;
    };

    return PSA_SUCCESS;
}

psa_status_t its_flash_erase_block(uint32_t block_id)
{
    uint32_t flash_addr;

    /* Calculate flash address location defined by block ID and
     * BLOCK_START_OFFSET parameters.
     */
    flash_addr = get_phys_address(block_id, BLOCK_START_OFFSET);

    return flash_erase(flash_addr);
}
