/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_nor.h"

#include "flash_fs/its_flash_fs.h"
#include "driver/Driver_Flash.h"

/**
 * \brief Gets physical address of the given block ID.
 *
 * \param[in] cfg       Flash FS configuration
 * \param[in] block_id  Block ID
 * \param[in] offset    Offset position from the init of the block
 *
 * \returns Returns physical address for the given block ID.
 */
static uint32_t get_phys_address(const struct its_flash_fs_config_t *cfg,
                                 uint32_t block_id, size_t offset)
{
    return cfg->flash_area_addr + (block_id * cfg->block_size) + offset;
}

static psa_status_t its_flash_nor_init(const struct its_flash_fs_config_t *cfg)
{
    int32_t err;

    err = ((ARM_DRIVER_FLASH *)cfg->flash_dev)->Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_read(const struct its_flash_fs_config_t *cfg,
                                       uint32_t block_id, uint8_t *buff,
                                       size_t offset, size_t size)
{
    int32_t err;
    uint32_t addr = get_phys_address(cfg, block_id, offset);

    err = ((ARM_DRIVER_FLASH *)cfg->flash_dev)->ReadData(addr, buff, size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_write(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id, const uint8_t *buff,
                                        size_t offset, size_t size)
{
    int32_t err;
    uint32_t addr = get_phys_address(cfg, block_id, offset);

    err = ((ARM_DRIVER_FLASH *)cfg->flash_dev)->ProgramData(addr, buff, size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_flush(const struct its_flash_fs_config_t *cfg)
{
    /* Nothing needs to be done for NOR flash, as writes are commited to flash
     * immediately.
     */
    (void)cfg;
    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_erase(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id)
{
    int32_t err;
    uint32_t addr;
    size_t offset;

    for (offset = 0; offset < cfg->block_size; offset += cfg->sector_size) {
        addr = get_phys_address(cfg, block_id, offset);

        err = ((ARM_DRIVER_FLASH *)cfg->flash_dev)->EraseSector(addr);
        if (err != ARM_DRIVER_OK) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }

    return PSA_SUCCESS;
}

const struct its_flash_fs_ops_t its_flash_fs_ops_nor = {
    .init = its_flash_nor_init,
    .read = its_flash_nor_read,
    .write = its_flash_nor_write,
    .flush = its_flash_nor_flush,
    .erase = its_flash_nor_erase,
};
