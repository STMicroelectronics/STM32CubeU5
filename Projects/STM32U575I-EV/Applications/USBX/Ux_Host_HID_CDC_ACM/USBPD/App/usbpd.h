/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd.h
  * @author  MCD Application Team
  * @brief   This file contains the device define.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef __usbpd_H
#define __usbpd_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_hw_if.h"

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

unsigned int USBPD_PreInitOs(void);

/* USBPD init function */
unsigned int    MX_USBPD_Init(void *memory_ptr);

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

#ifdef __cplusplus
}
#endif
#endif /*__usbpd_H */

/**
  * @}
  */

/**
  * @}
  */
