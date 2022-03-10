/**
  ******************************************************************************
  * @file    nv_counters_device.h
  * @author  MCD Application Team
  * @brief   Tick device definition
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
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef NV_COUNTERS_DEVICE_H
#define NV_COUNTERS_DEVICE_H
#include "flash_layout.h"

#define DEVICE_NV_COUNTERS_AREA_SIZE FLASH_NV_COUNTER_AREA_SIZE

#define  DEVICE_NV_COUNTERS_AREA_OFFSET TFM_NV_COUNTERS_AREA_ADDR

#define DEVICE_NV_COUNTERS_SECTOR_SIZE TFM_NV_COUNTERS_SECTOR_SIZE

#define DEVICE_NV_COUNTERS_FLASH_NAME TFM_Driver_FLASH0

#define DEVICE_NUM_NV_COUNTERS        6
#endif /* NV_COUNTERS_DEVICE_H*/
