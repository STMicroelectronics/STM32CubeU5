/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbxh_platform.h
  * @brief          : Header for usbh_platform.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_PLATFORM_H__
#define __USBH_PLATFORM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_host.h"
#include "stm32u5xx_hal.h"
/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

void USBH_DriverVBUS(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __USBH_PLATFORM_H__ */

