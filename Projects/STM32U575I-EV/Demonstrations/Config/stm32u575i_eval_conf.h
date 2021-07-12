/**
  ******************************************************************************
  * @file    stm32u575i_eval_conf.h
  * @author  MCD Application Team
  * @brief   STM32U575I_EVAL board configuration file.
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
#ifndef STM32U575I_EVAL_CONF_H
#define STM32U575I_EVAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Usage of COM feature */
#define USE_BSP_COM_FEATURE 0U
#define USE_COM_LOG         0U

/* Usage of POT feature */
#define USE_BSP_POT_FEATURE 0U

/* Usage of IO expander */
#define USE_BSP_IO_CLASS 1U

/* TS defines */
#define TS_TOUCH_NBR                  1U
   
/* LCD defines */
#define LCD_LAYER_0_ADDRESS                 0xD0000000U
#define LCD_LAYER_1_ADDRESS                 0xD0200000U
#define USE_DMA2D_TO_FILL_RGB_RECT          0U

/* Button interrupt priorities */
#define BSP_BUTTON_TAMPER_IT_PRIORITY 15UL  /* Default is lowest priority level */
#define BSP_BUTTON_USER_IT_PRIORITY   15UL  /* Default is lowest priority level */

/* Joystick interrupt priority */
#define BSP_JOY1_IT_PRIORITY          15UL  /* Default is lowest priority level */

#if (USE_BSP_IO_CLASS == 1)
/* IOExpander interrupt priority */
#define BSP_IOEXPANDER_IT_PRIORITY    15UL  /* Default is lowest priority level */
#endif

/* CAMERA interrupt priority */
#define BSP_CAMERA_IT_PRIORITY       14UL  /* Default is lowest priority level */
   
/* Touch screen interrupt priority */
#define BSP_TS_IT_PRIORITY            15UL  /* Default is lowest priority level */

/* Audio interrupt priorities */
#define BSP_AUDIO_IN_IT_PRIORITY      2UL  /* Default is lowest priority level */
#define BSP_AUDIO_OUT_IT_PRIORITY     2UL  /* Default is lowest priority level */

/* SD card interrupt priority */
#define BSP_SD_IT_PRIORITY           15UL  /* Default is lowest priority level */
#define BSP_SD_RX_IT_PRIORITY        14UL  /* Default is lowest priority level */
#define BSP_SD_TX_IT_PRIORITY        15UL  /* Default is lowest priority level */

/* SRAM DMA interrupt priority */
#define BSP_SRAM_DMA_IT_PRIORITY      15UL  /* Default is lowest priority level */

/* OSPI RAM interrupt priority */
#define BSP_OSPI_RAM_IT_PRIORITY      15UL  /* Default is lowest priority level */
#define BSP_OSPI_RAM_DMA_IT_PRIORITY  15UL  /* Default is lowest priority level */

/* Bus frequencies */
#define BUS_I2C2_FREQUENCY            400000U /* Frequency of I2C2 = 400 KHz*/

/* Default AUDIO IN internal buffer size in 32-bit words per micro */
#define BSP_AUDIO_IN_DEFAULT_BUFFER_SIZE  8*2048UL   /* 2048*4 = 8Kbytes */

#ifdef __cplusplus
}
#endif

#endif /* STM32U575I_EVAL_CONF_H */
