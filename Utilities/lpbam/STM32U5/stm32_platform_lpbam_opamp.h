/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_opamp.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM OPAMP defines
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
#ifndef STM32_PLATFORM_LPBAM_OPAMP_H
#define STM32_PLATFORM_LPBAM_OPAMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup OPAMP_Platform
  * @brief      OPAMP Platform LPBAM module driver
  * @{
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_OPAMP_Platform_Exported_Constants LPBAM OPAMP Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_OPAMP_Mode LPBAM OPAMP Mode
  * @{
  */
#define LPBAM_OPAMP_STANDALONE_MODE (0x00U) /*!< LPBAM standalone mode */
#define LPBAM_OPAMP_PGA_MODE        (0x08U) /*!< LPBAM PGA mode        */
#define LPBAM_OPAMP_FOLLOWER_MODE   (0x0CU) /*!< LPBAM follower mode   */
/**
  * @}
  */

/** @defgroup LPBAM_OPAMP_NonInvertingInput LPBAM OPAMP Non Inverting Input
  * @{
  */
#define LPBAM_OPAMP_NONINVERTINGINPUT_IO0    (0x0000U) /*!< LPBAM OPAMP non-inverting input connected to dedicated IO
                                                            pin                                                       */
#define LPBAM_OPAMP_NONINVERTINGINPUT_DAC_CH (0x0400U) /*!< LPBAM OPAMP non-inverting input connected internally to DAC
                                                            channel                                                   */
/**
  * @}
  */

/** @defgroup LPBAM_OPAMP_InvertingInput LPBAM OPAMP Inverting Input
  * @{
  */
#define LPBAM_OPAMP_INVERTINGINPUT_IO0        (0x0000U) /*!< LPBAM OPAMP inverting input connected to dedicated IO pin
                                                             low-leakage                                              */
#define LPBAM_OPAMP_INVERTINGINPUT_IO1        (0x0100U) /*!< LPBAM OPAMP inverting input connected to alternative IO
                                                             pin available on some device packages                    */
#define LPBAM_OPAMP_INVERTINGINPUT_CONNECT_NO (0x0200U) /*!< LPBAM OPAMP inverting input not connected externally (PGA
                                                             mode only)                                               */
/**
  * @}
  */

/** @defgroup LPBAM_OPAMP_PgaGain LPBAM OPAMP Pga Gain
  * @{
  */
#define LPBAM_OPAMP_PGA_GAIN_2  (0x000U)  /*!< LPBAM PGA gain =  2 */
#define LPBAM_OPAMP_PGA_GAIN_4  (0x010U)  /*!< LPBAM PGA gain =  4 */
#define LPBAM_OPAMP_PGA_GAIN_8  (0x020U)  /*!< LPBAM PGA gain =  8 */
#define LPBAM_OPAMP_PGA_GAIN_16 (0x030U)  /*!< LPBAM PGA gain = 16 */
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

#endif /* STM32_PLATFORM_LPBAM_OPAMP_H */
