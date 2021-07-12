/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_gpio.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM GPIO defines
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------------------------------------------- */
#ifndef STM32_PLATFORM_LPBAM_GPIO_H
#define STM32_PLATFORM_LPBAM_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup GPIO_Platform
  * @brief      GPIO Platform LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_GPIO_Platform_Exported_Types LPBAM GPIO Platform Exported Types
  * @{
  */

/**
  * @brief LPBAM GPIO bit SET and bit RESET Enumeration definition.
  */
typedef enum
{
  LPBAM_GPIO_PIN_RESET = 0U,   /*!< LPBAM GPIO pin reset state  */
  LPBAM_GPIO_PIN_SET           /*!< LPBAM GPIO pin set state    */
} LPBAM_GPIO_PinState;
/**
  * @}
  */


/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_GPIO_Platform_Exported_Constants LPBAM GPIO Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_GPIO_Pins LPBAM GPIO Pin
  * @{
  */
#define LPBAM_GPIO_PIN_0  (0x0001U) /*!< LPBAM GPIO PIN 0  */
#define LPBAM_GPIO_PIN_1  (0x0002U) /*!< LPBAM GPIO PIN 1  */
#define LPBAM_GPIO_PIN_2  (0x0004U) /*!< LPBAM GPIO PIN 2  */
#define LPBAM_GPIO_PIN_3  (0x0008U) /*!< LPBAM GPIO PIN 3  */
#define LPBAM_GPIO_PIN_4  (0x0010U) /*!< LPBAM GPIO PIN 4  */
#define LPBAM_GPIO_PIN_5  (0x0020U) /*!< LPBAM GPIO PIN 5  */
#define LPBAM_GPIO_PIN_6  (0x0040U) /*!< LPBAM GPIO PIN 6  */
#define LPBAM_GPIO_PIN_7  (0x0080U) /*!< LPBAM GPIO PIN 7  */
#define LPBAM_GPIO_PIN_8  (0x0100U) /*!< LPBAM GPIO PIN 8  */
#define LPBAM_GPIO_PIN_9  (0x0200U) /*!< LPBAM GPIO PIN 9  */
#define LPBAM_GPIO_PIN_10 (0x0400U) /*!< LPBAM GPIO PIN 10 */
#define LPBAM_GPIO_PIN_11 (0x0800U) /*!< LPBAM GPIO PIN 11 */
#define LPBAM_GPIO_PIN_12 (0x1000U) /*!< LPBAM GPIO PIN 12 */
#define LPBAM_GPIO_PIN_13 (0x2000U) /*!< LPBAM GPIO PIN 13 */
#define LPBAM_GPIO_PIN_14 (0x4000U) /*!< LPBAM GPIO PIN 14 */
#define LPBAM_GPIO_PIN_15 (0x8000U) /*!< LPBAM GPIO PIN 15 */
/**
  * @}
  */

/** @defgroup LPBAM_GPIO_SequenceMode LPBAM GPIO Sequence Mode
  * @{
  */
#define LPBAM_GPIO_IS_SINGLE_MODE   (0x00U) /*!< LPBAM GPIO is single mode   */
#define LPBAM_GPIO_IS_SEQUENCE_MODE (0x01U) /*!< LPBAM GPIO is sequence mode */
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

#endif /* STM32_PLATFORM_LPBAM_GPIO_H */
