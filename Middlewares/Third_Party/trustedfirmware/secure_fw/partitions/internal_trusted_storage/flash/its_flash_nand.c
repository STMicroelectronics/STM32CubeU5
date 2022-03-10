/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash_nand.h"

#include "flash_fs/its_flash_fs.h"
#include "tfm_memory_utils.h"

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

static psa_status_t its_flash_nand_init(const struct its_flash_fs_config_t *cfg)
{
    int32_t err;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    if (flash_dev->buf_size < cfg->block_size) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    err = flash_dev->driver->Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_read(const struct its_flash_fs_config_t *cfg,
                                        uint32_t block_id, uint8_t *buff,
                                        size_t offset, size_t size)
{
    int32_t err;
    uint32_t addr = get_phys_address(cfg, block_id, offset);
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    err = flash_dev->driver->ReadData(addr, buff, size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_write(const struct its_flash_fs_config_t *cfg,
                                         uint32_t block_id, const uint8_t *buff,
                                         size_t offset, size_t size)
{
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    if (flash_dev->buf_block_id == ITS_BLOCK_INVALID_ID) {
        flash_dev->buf_block_id = block_id;
    } else if (flash_dev->buf_block_id != block_id) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* Buffer the write data */
    (void)tfm_memcpy(flash_dev->write_buf + offset, buff, size);

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_flush(const struct its_flash_fs_config_t *cfg)
{
    int32_t err;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;
    uint32_t addr = get_phys_address(cfg, flash_dev->buf_block_id, 0);

    /* Flush the buffered write data to flash*/
    err = flash_dev->driver->ProgramData(addr, flash_dev->write_buf,
                                         cfg->block_size);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Clear the write buffer */
    (void)tfm_memset(flash_dev->write_buf, 0, flash_dev->buf_size);
    flash_dev->buf_block_id = ITS_BLOCK_INVALID_ID;

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nand_erase(const struct its_flash_fs_config_t *cfg,
                                         uint32_t block_id)
{
    int32_t err;
    uint32_t addr;
    size_t offset;
    struct its_flash_nand_dev_t *flash_dev =
        (struct its_flash_nand_dev_t *)cfg->flash_dev;

    for (offset = 0; offset < cfg->block_size; offset += cfg->sector_size) {
        addr = get_phys_address(cfg, block_id, offset);

        err = flash_dev->driver->EraseSector(addr);
        if (err != ARM_DRIVER_OK) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }

    return PSA_SUCCESS;
}

const struct its_flash_fs_ops_t its_flash_fs_ops_nand = {
    .init = its_flash_nand_init,
    .read = its_flash_nand_read,
    .write = its_flash_nand_write,
    .flush = its_flash_nand_flush,
    .erase = its_flash_nand_erase,
};
