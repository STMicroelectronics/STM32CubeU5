/**
  ******************************************************************************
  * @file    k_config.h
  * @author  MCD Application Team
  * @brief   Header for k_config.c file
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
#ifndef K_CONFIG_H
#define K_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define EVAL_BOARD   "STM32U575I-EV"
#define DEMO_VERSION "Demo V1.1.0"
#define DEMO_DATE    "7 - Sept - 2021"

#define DEMO_INFO1   "MCD Application Team"
#define DEMO_INFO2   "(C) COPYRIGHT 2021"
#define DEMO_INFO3   "STMicroelectronics"

#define CHOOSEFILE_MAXLEN   255

typedef enum{
 MODULE_MAIN_APP,
 MODULE_LPBAM,
 MODULE_SECURITY,
 MODULE_AUDIOFILT,
 MODULE_USBPD,
 MODULE_LOWPOWER,
 MODULE_CAMERA,
 MODULE_CALENDAR, 
 MODULE_SYSINFO,
 MODULE_APPLICATIONS,
 MODULE_MAX,
 MODULE_NONE = 0xFF
} MODULES_INFO;

#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* K_LOG_H */
