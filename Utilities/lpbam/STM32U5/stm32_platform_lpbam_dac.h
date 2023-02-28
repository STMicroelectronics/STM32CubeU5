/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_dac.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM DAC defines
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
#ifndef STM32_PLATFORM_LPBAM_DAC_H
#define STM32_PLATFORM_LPBAM_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup DAC_Platform
  * @brief      DAC Platform LPBAM module driver
  * @{
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_DAC_Platform_Exported_Constants LPBAM DAC Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_DAC_Channel LPBAM DAC Channel
  * @{
  */
#define LPBAM_DAC_CHANNEL_1 (0x01U) /*!< LPBAM DAC Channel 1 */
#define LPBAM_DAC_CHANNEL_2 (0x02U) /*!< LPBAM DAC Channel 2 */
/**
  * @}
  */

/** @defgroup LPBAM_DAC_Trigger_Selection LPBAM DAC Trigger Selection
  * @{
  */
#define LPBAM_DAC_TRIGGER_NONE       (0x00) /*!< conversion is automatic once the DAC_DHRxxxx register has been
                                                 loaded                                                             */
#define LPBAM_DAC_TRIGGER_T1_TRGO    (0x01) /*!< TIM1 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T2_TRGO    (0x02) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T4_TRGO    (0x03) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T5_TRGO    (0x04) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T6_TRGO    (0x05) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T7_TRGO    (0x06) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T8_TRGO    (0x07) /*!< TIM8 TRGO selected as external conversion trigger for DAC channel  */
#define LPBAM_DAC_TRIGGER_T15_TRGO   (0x08) /*!< TIM15 TRGO selected as external conversion trigger for DAC channel */
#define LPBAM_DAC_TRIGGER_LPTIM1_CH1 (0x0B) /*!< LPTIM1 CH1 selected as external conversion trigger for DAC
                                                 channel                                                            */
#define LPBAM_DAC_TRIGGER_LPTIM3_CH1 (0x0C) /*!< LPTIM3 CH1 selected as external conversion trigger for DAC
                                                 channel                                                            */
#define LPBAM_DAC_TRIGGER_EXT_IT9    (0x0D) /*!< EXTI Line9 event selected as external conversion trigger for DAC
                                                 channel                                                            */
/**
  * @}
  */

/** @defgroup LPBAM_DAC_DataAlignment LPBAM DAC Data alignment
  * @{
  */
#define LPBAM_DAC_ALIGN_12B_R (0x00U) /*!< DAC data 12 bit right alignment */
#define LPBAM_DAC_ALIGN_12B_L (0x04U) /*!< DAC data 12 bit left alignment  */
#define LPBAM_DAC_ALIGN_8B_R  (0x08U) /*!< DAC data 8 bit right alignment  */
/**
  * @}
  */

/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_DAC_Platform_Private_Constants LPBAM DAC Platform Private Constants
  * @{
  */
#define LPBAM_DAC_CHANNEL1_DMAEN DAC_CR_DMAEN1 /*!< DAC channel 1 DMA enable */
#define LPBAM_DAC_CHANNEL2_DMAEN DAC_CR_DMAEN2 /*!< DAC channel 2 DMA enable */
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

#endif /* STM32_PLATFORM_LPBAM_DAC_H */
