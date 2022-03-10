/**
  ******************************************************************************
  * @file    boot_hal_imagevalid.h
  * @author  MCD Application Team
  * @brief   Header for image double validation services in boot_hal.c module
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
#ifndef BOOT_HAL_IMAGEVALID_H
#define BOOT_HAL_IMAGEVALID_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
#include "flash_layout.h"

extern uint8_t ImageValidEnable;
extern uint8_t ImageValidIndex;
extern uint8_t ImageValidStatus[MCUBOOT_IMAGE_NUMBER];

#define IMAGE_VALID   (uint8_t)(0x55)
#define IMAGE_INVALID (uint8_t)(0x82)
#endif

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_IMAGEVALID_H */
