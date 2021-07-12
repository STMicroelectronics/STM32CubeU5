/**
  ******************************************************************************
  * @file    tick_device.h
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

#ifndef TICK_DEVICE_H
#define TICK_DEVICE_H
#include "region.h"
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
#define DEVICE_GET_TICK \
    do { __IO uint32_t TickVal=0; /* TickVal is placed on stack */\
      if ((uint32_t)&TickVal <= (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base))\
      return 0 ;\
    }while(0)
#endif /* TICK_DEVICE_H */
