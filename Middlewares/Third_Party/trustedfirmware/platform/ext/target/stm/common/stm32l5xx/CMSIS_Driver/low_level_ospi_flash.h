/**
  ******************************************************************************
  * @file    low_level_ospi_flash.h
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_ospi_flash driver
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
#ifndef __LOW_LEVEL_OSPI_FLASH_H
#define __LOW_LEVEL_OSPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l5xx_hal.h"

struct ospi_flash_range
{
  uint32_t base;
  uint32_t limit;
};
struct ospi_flash_vect
{
  uint32_t nb;
  struct ospi_flash_range *range;
};
struct low_level_ospi_device
{
  struct ospi_flash_vect erase;
  struct ospi_flash_vect write;
  uint32_t read_error;
};

extern struct low_level_ospi_device OSPI_FLASH0_DEV;

/* Additional api, not included in cmsis arm driver flash interface */
int32_t Ospi_Flash_Config_Exe(void);

#endif /* __LOW_LEVEL_OSPI_FLASH_H */


