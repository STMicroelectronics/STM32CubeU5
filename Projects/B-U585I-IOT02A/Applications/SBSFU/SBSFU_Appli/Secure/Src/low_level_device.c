/**
  ******************************************************************************
  * @file    low_level_device.c
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "region_defs.h"
#include "low_level_flash.h"
#if !defined(MCUBOOT_OVERWRITE_ONLY) && ((MCUBOOT_APP_IMAGE_NUMBER == 2) || (MCUBOOT_S_DATA_IMAGE_NUMBER == 1))
static struct flash_range write_vect[] =
{
  /* Area for writing confirm flag on in installed image */
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
  { S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 32, S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 16 - 1},
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  { S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_DATA_PARTITION_SIZE - 32, S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_DATA_PARTITION_SIZE - 16 - 1},
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
};
struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = 0, .range = NULL},
  .write = { .nb = sizeof(write_vect) / sizeof(struct flash_range), .range = write_vect}
};
#else
struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = 0, .range = NULL},
  .write = { .nb = 0, .range = NULL}
};
#endif /* defined(MCUBOOT_OVERWRITE_ONLY) */
