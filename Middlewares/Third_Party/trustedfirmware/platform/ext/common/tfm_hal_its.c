/*
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_hal_its.h"

#include "cmsis_compiler.h"
#include "flash_layout.h"

/* The base address of the dedicated flash area for ITS */
#ifndef TFM_HAL_ITS_FLASH_AREA_ADDR
#error "TFM_HAL_ITS_FLASH_AREA_ADDR must be defined by the target in flash_layout.h"
#endif

/* The size of the dedicated flash area for ITS in bytes */
#ifndef TFM_HAL_ITS_FLASH_AREA_SIZE
#error "TFM_HAL_ITS_FLASH_AREA_SIZE must be defined by the target in flash_layout.h"
#endif

/* The number of contiguous physical flash erase sectors per logical filesystem
 * erase block. Adjust so that the maximum required asset size will fit in one
 * logical block.
 */
#ifndef TFM_HAL_ITS_SECTORS_PER_BLOCK
#error "TFM_HAL_ITS_SECTORS_PER_BLOCK must be defined by the target in flash_layout.h"
#endif

__WEAK enum tfm_hal_status_t
tfm_hal_its_fs_info(struct tfm_hal_its_fs_info_t *fs_info)
{
    if (!fs_info) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    fs_info->flash_area_addr = TFM_HAL_ITS_FLASH_AREA_ADDR;
    fs_info->flash_area_size = TFM_HAL_ITS_FLASH_AREA_SIZE;
    fs_info->sectors_per_block = TFM_HAL_ITS_SECTORS_PER_BLOCK;

    return TFM_HAL_SUCCESS;
}
