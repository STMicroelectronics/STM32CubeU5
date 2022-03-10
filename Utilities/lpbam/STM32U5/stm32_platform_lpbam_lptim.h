/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_lptim.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM LPTIM defines
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
#ifndef STM32_PLATFORM_LPBAM_LPTIM_H
#define STM32_PLATFORM_LPBAM_LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup LPTIM_Platform
  * @brief      LPTIM Platform LPBAM module driver
  * @{
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_LPTIM_Platform_Exported_Constants LPBAM LPTIM Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_LPTIM_Channel LPBAM LPTIM Channel
  * @{
  */
#define LPBAM_LPTIM_CHANNEL_1 (0x01UL) /*!< LPBAM channel 1 */
#define LPBAM_LPTIM_CHANNEL_2 (0x02UL) /*!< LPBAM channel 2 */
/**
  * @}
  */

/** @defgroup LPBAM_LPTIM_Start_Mode LPBAM LPTIM Start Mode
  * @{
  */
#define LPBAM_LPTIM_START_SINGLE     (0x00U) /*!< LPBAM LPTIM single mode start     */
#define LPBAM_LPTIM_START_CONTINUOUS (0x01U) /*!< LPBAM LPTIM continuous mode start */
/**
  * @}
  */

/** @defgroup LPBAM_LPTIM_Stop_Mode LPBAM LPTIM Stop Mode
  * @{
  */
#define LPBAM_LPTIM_NO_UE_MODE (0x00UL) /*!< LPBAM LPTIM NO Update Event detection mode  */
#define LPBAM_LPTIM_UE_MODE    (0x01UL) /*!< LPBAM LPTIM Update Event detection mode     */
/**
  * @}
  */

/** @defgroup LPBAM_LPTIM_Wakeup_Interrupt LPBAM LPTIM Wakeup Interrupt
  * @{
  */
#define LPBAM_LPTIM_IT_NONE   (0x00U)         /*!< LPBAM LPTIM wake up interrupt none              */
#define LPBAM_LPTIM_IT_UPDATE LPTIM_DIER_UEIE /*!< LPBAM LPTIM update event match interrupt enable */
/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_LPTIM_Platform_Private_Constants LPBAM LPTIM Platform Private Constants
  * @{
  */
#define LPBAM_LPTIM_MODE_DEFAULT (0xFFU)          /*!< LPBAM LPTIM Mode Default                                */
#define LPBAM_LPTIM_UEDE         LPTIM_DIER_UEDE  /*!< LPBAM LPTIM enable DMA update event request             */
#define LPBAM_LPTIM_CC1E         LPTIM_CCMR1_CC1E /*!< LPBAM LPTIM channel 1 enable                            */
#define LPBAM_LPTIM_CC1DE        LPTIM_DIER_CC1DE /*!< LPBAM LPTIM channel 1 enable DMA input capture request  */
#define LPBAM_LPTIM_CC2E         LPTIM_CCMR1_CC2E /*!< LPBAM LPTIM channel 2 enable                            */
#define LPBAM_LPTIM_CC2DE        LPTIM_DIER_CC2DE /*!< LPBAM LPTIM channel 2 enable DMA input capture request  */
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

#endif /* STM32_PLATFORM_LPBAM_LPTIM_H */
