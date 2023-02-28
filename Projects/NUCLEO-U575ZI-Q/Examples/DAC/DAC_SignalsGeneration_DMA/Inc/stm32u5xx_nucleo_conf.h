/**
  ******************************************************************************
  * @file    stm32u5xx_nucleo_conf.h
  * @author  MCD Application Team
  * @brief   STM32U5XX nucleo board configuration file.
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
#ifndef STM32U5XX_NUCLEO_CONF_H
#define STM32U5XX_NUCLEO_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5XX_NUCLEO
  * @{
  */

/** @defgroup STM32U5XX_NUCLEO_CONFIG Config
  * @{
  */

/** @defgroup STM32U5XX_NUCLEO_CONFIG_Exported_Constants Exported Constants
  * @{
  */
/* Nucleo pin and part number defines */
#define USE_NUCLEO_144
#define USE_NUCLEO_U575ZI_Q

/* COM define */
#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 0U

/* IRQ priorities */
#define BSP_BUTTON_USER_IT_PRIORITY         15U

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

#endif /* STM32U5XX_NUCLEO_CONF_H */
