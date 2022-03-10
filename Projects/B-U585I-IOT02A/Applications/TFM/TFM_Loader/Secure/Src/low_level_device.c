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
#include "flash_layout.h"
#include "low_level_flash.h"

static struct flash_range access_vect[] =
{
  { FLASH_AREA_0_OFFSET, FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE - 1},
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  { FLASH_AREA_4_OFFSET, FLASH_AREA_4_OFFSET + FLASH_AREA_4_SIZE - 1},
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
};

struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
  .write = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
};
