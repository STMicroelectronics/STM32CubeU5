/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_flash.h"

#include "flash_fs/its_flash_fs.h"

#ifdef ITS_RAM_FS
#ifndef ITS_RAM_FS_SIZE
#error "ITS_RAM_FS_SIZE must be defined by the target in flash_layout.h"
#endif
uint8_t its_block_data[ITS_RAM_FS_SIZE];

#elif (TFM_HAL_ITS_PROGRAM_UNIT > 16)
#ifndef ITS_FLASH_NAND_BUF_SIZE
#error "ITS_FLASH_NAND_BUF_SIZE must be defined by the target in flash_layout.h"
#endif
static uint8_t its_write_buf[ITS_FLASH_NAND_BUF_SIZE];
struct its_flash_nand_dev_t its_flash_nand_dev = {
    .driver = &TFM_HAL_ITS_FLASH_DRIVER,
    .buf_block_id = ITS_BLOCK_INVALID_ID,
    .write_buf = its_write_buf,
    .buf_size = sizeof(its_write_buf),
};
#endif

#ifdef TFM_PARTITION_PROTECTED_STORAGE
#ifdef PS_RAM_FS
#ifndef PS_RAM_FS_SIZE
#error "PS_RAM_FS_SIZE must be defined by the target in flash_layout.h"
#endif
uint8_t ps_block_data[PS_RAM_FS_SIZE];

#elif (TFM_HAL_PS_PROGRAM_UNIT > 16)
#ifndef PS_FLASH_NAND_BUF_SIZE
#error "PS_FLASH_NAND_BUF_SIZE must be defined by the target in flash_layout.h"
#endif
static uint8_t ps_write_buf[PS_FLASH_NAND_BUF_SIZE];
struct its_flash_nand_dev_t ps_flash_nand_dev = {
    .driver = &TFM_HAL_PS_FLASH_DRIVER,
    .buf_block_id = ITS_BLOCK_INVALID_ID,
    .write_buf = ps_write_buf,
    .buf_size = sizeof(ps_write_buf),
};
#endif
#endif /* TFM_PARTITION_PROTECTED_STORAGE */
