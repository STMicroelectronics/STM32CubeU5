/**
  ******************************************************************************
  * @file    low_level_device.c
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
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

struct flash_range nvm_psa_its_vect[] = {
	{ FLASH_NV_COUNTERS_AREA_OFFSET, FLASH_ITS_AREA_OFFSET + FLASH_ITS_AREA_SIZE - 1},
};

struct low_level_device FLASH0_DEV =  {
	.erase = { .nb =sizeof(nvm_psa_its_vect)/sizeof(struct flash_range), .range = nvm_psa_its_vect},
	.write = { .nb =sizeof(nvm_psa_its_vect)/sizeof(struct flash_range), .range = nvm_psa_its_vect},
	.secure = { 0, NULL},
	.read_error = 1
};
