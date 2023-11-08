/**
  ******************************************************************************
  * @file    stm32u5g9j_discovery_conf_template.h
  * @author  MCD Application Team
  * @brief   STM32U5G9J_DISCOVERY board configuration file.
  *          This file should be copied to the application folder and renamed
  *          to stm32u5g9j_discovery_conf.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5G9J_DISCOVERY_CONF_H
#define STM32U5G9J_DISCOVERY_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5G9J_DK2
  * @{
  */

/** @defgroup STM32U5G9J_DK2_CONFIG Config
  * @{
  */

/** @defgroup STM32U5G9J_DK2_CONFIG_Exported_Constants Exported Constants
  * @{
  */

/* COM defines */
#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 0U

/* Touch Sensing controller defines */
#define USE_GT911_TS_CTRL                   1U

/* TS supported features defines */
#define USE_TS_GESTURE                      1U
#define USE_TS_MULTI_TOUCH                  1U

/* Default TS touch number */
#define TS_TOUCH_NBR                        2U

/* LCD defines */
#define LCD_LAYER_0_ADDRESS                 0x20000000U
#define LCD_LAYER_1_ADDRESS                 0x20200000U

/* IRQ priorities (Default is 15 as lowest priority level) */
#define BSP_BUTTON_USER_IT_PRIORITY         15U
#define BSP_TS_IT_PRIORITY                  15U

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5G9J_DISCOVERY_CONF_H */
