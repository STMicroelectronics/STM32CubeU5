/**
  ******************************************************************************
  * @file    stm32l562e_discovery_conf.h
  * @author  MCD Application Team
  * @brief   STM32L562E-DK discovery board configuration file.
  *          This file should be copied to the application folder and renamed
  *          to stm32l562e_discovery_conf.h .
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L562E_DISCOVERY_CONF_H
#define STM32L562E_DISCOVERY_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal.h"

/* Usage of COM feature */
#define USE_BSP_COM_FEATURE 0U
#define USE_COM_LOG         0U

/* Button interrupt priority */
#define BSP_BUTTON_USER_IT_PRIORITY 0x07UL  /* Default is lowest priority level */

/* IDD interrupt priority */
#define BSP_IDD_IT_PRIORITY         0x07UL  /* Default is lowest priority level */

/* TS interrupt priority */
#define BSP_TS_IT_PRIORITY          0x07UL  /* Default is lowest priority level */

/* Audio interrupt priorities */
#define BSP_AUDIO_OUT_IT_PRIORITY   0x07UL  /* Default is lowest priority level */
#define BSP_AUDIO_IN_IT_PRIORITY    0x07UL  /* Default is lowest priority level */

/* SD card interrupt priority */
#define BSP_SD_IT_PRIORITY          0x07UL  /* Default is lowest priority level */

/* Bus frequencies */
#define BUS_I2C1_FREQUENCY          100000UL /* Frequency of I2C1 = 100 KHz */

/* Default AUDIO IN internal buffer size in 32-bit words per micro */
#define BSP_AUDIO_IN_DEFAULT_BUFFER_SIZE 2048UL /* 2048*4 = 8Kbytes */

#ifdef __cplusplus
}
#endif

#endif /* STM32L562E_DISCOVERY_CONF_H */
