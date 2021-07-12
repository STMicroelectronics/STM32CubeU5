/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_adc.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_adc.c file
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
#ifndef STM32_LPBAM_ADC_H
#define STM32_LPBAM_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_adc.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup ADC_Basic
  * @brief      ADC LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_ADC_Basic_Exported_Types LPBAM ADC Basic Exported Types
  * @brief    LPBAM ADC Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM ADC Configuration Structure definition.
  */
typedef struct
{
  LPBAM_FunctionalState State;                     /*!< Specifies the ADC state.
                                                        This parameter can be set to ENABLE or DISABLE.               */

  uint32_t ScanConvMode;                           /*!< Configure the sequencer of ADC groups regular.
                                                        This parameter can be a value of
                                                        @ref LPBAM_ADC_Sequence_Scan_Mode.                            */

  LPBAM_FunctionalState ContinuousConvMode;        /*!< Specify whether the conversion is performed in single mode (one
                                                        conversion) or continuous mode for ADC group regular, after the
                                                        first ADC conversion start trigger occurred (software start or
                                                        external trigger).
                                                        This parameter can be ENABLE or DISABLE                       */

  LPBAM_FunctionalState DiscontinuousConvMode;     /*!< Specify whether the conversions sequence of ADC group regular is
                                                        performed in Complete-sequence/Discontinuous-sequence (main
                                                        sequence subdivided in successive parts).
                                                        Discontinuous mode is used only if sequencer is enabled
                                                        Discontinuous mode can be enabled only if continuous mode is
                                                        disabled.
                                                        If continuous mode is enabled, this parameter setting is
                                                        discarded.
                                                        This parameter can be ENABLE or DISABLE                       */

  uint32_t ExternalTrigConv;                       /*!< Select the external event source used to trigger ADC group
                                                        regular conversion start.
                                                        This parameter can be a value of
                                                        @ref LPBAM_ADC_Trigger_Selection                              */

  uint32_t ExternalTrigConvEdge;                   /*!< Select the external event edge used to trigger ADC group regular
                                                        conversion start.
                                                        This parameter can be a value of
                                                        @ref LPBAM_ADC_Trigger_Polarity                               */

  uint32_t TriggerFrequencyMode;                   /*!< Set ADC trigger frequency mode.
                                                        This parameter can be a value of
                                                        @ref LPBAM_ADC_Trigger_LowFrequency.
                                                        Note: ADC trigger frequency mode must be set to low frequency
                                                              when a duration is exceeded before ADC conversion start
                                                              trigger event (between ADC enable and ADC conversion start
                                                              trigger event or between two ADC conversion start trigger
                                                              event).
                                                              Duration value: Refer to device datasheet, parameter
                                                              "Idle".
                                                        Note: When ADC trigger frequency mode is set to low frequency,
                                                              some rearm cycles are inserted before performing ADC
                                                              conversion start, inducing a delay of 2 ADC clock
                                                              cycles.                                                 */

  LPBAM_FunctionalState LowPowerAutoWait;          /*!< Select the dynamic low power Auto Delay: new conversion start
                                                        only when the previous conversion (for ADC group regular) has
                                                        been retrieved by user software.
                                                        This feature automatically adapts the frequency of ADC
                                                        conversions triggers to the speed of the system that reads the
                                                        data. Moreover, this avoids risk of overrun for low frequency
                                                        applications.
                                                        This parameter can be ENABLE or DISABLE                       */

  LPBAM_FunctionalState DMAContinuousRequests;     /*!< Specify whether the DMA requests are performed in one shot mode
                                                        (DMA transfer stops when number of conversions is reached) or in
                                                        continuous mode (DMA transfer unlimited, whatever number of
                                                        conversions).
                                                        This parameter can be set to ENABLE or DISABLE.
                                                        Note: In continuous mode, DMA must be configured in circular
                                                              mode.
                                                              Otherwise an overrun will be triggered when DMA buffer
                                                              maximum pointer is reached.                             */

  uint32_t UpdateAnalogWDG;                        /*!< Specify the analog watchdog to be updated
                                                        This parameter can be a value of
                                                        @ref LPBAM_ADC_AnalogWatchDog_Number                          */


  LPBAM_ADC_AnalogWDGConfig_t AnalogWDGConfig[3U]; /*!< Structure definition of ADC analog watchdog.
                                                        Please refer to LPBAM_ADC_AnalogWDGConfig_t definition in
                                                        stm32_platform_lpbam_adc.h for more information.              */

  uint16_t Size;                                   /*!< Specifies DMA transfer number of ADC conversion data          */

} LPBAM_ADC_Config_t;

/**
  * @brief LPBAM ADC Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;     /*!< Defines the node descriptor fields */

  ADC_TypeDef *pInstance;    /*!< Defines the ADC instance           */

  LPBAM_ADC_Config_t Config; /*!< Defines the ADC user configuration */

} LPBAM_ADC_ConfNode_t;

/**
  * @brief LPBAM ADC Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_ADC_STATE_ID,             /*!< LPBAM ADC State ID                 */

  LPBAM_ADC_CONFIG_ID,            /*!< LPBAM ADC Configuration ID         */

  LPBAM_ADC_CHANNELSELECTION_ID,  /*!< LPBAM ADC Channel Selection ID     */

  LPBAM_ADC_AWDG_THRESHOLD_ID,    /*!< LPBAM ADC AWDG Threshold ID        */

  LPBAM_ADC_AWDG_MONITOR_ID,      /*!< LPBAM ADC AWDG Monitor ID          */

  LPBAM_ADC_TRIG_LOWFREQUENCY_ID, /*!< LPBAM ADC Trigger Low Frequency ID */

  LPBAM_ADC_CLKPRESCALER_ID,      /*!< LPBAM ADC Prescaler ID             */

  LPBAM_ADC_CLEARFLAG_ID,         /*!< LPBAM ADC Interrupt Clear Flag ID  */

  LPBAM_ADC_WAKEUP_IT_ID,         /*!< LPBAM ADC Wakeup Interrupt ID      */

  LPBAM_ADC_DATA_ID               /*!< LPBAM ADC Data transfer ID         */

} LPBAM_ADC_ID_t;
/**
  * @}
  */

/* Include LPBAM ADC Advanced module */
#include "stm32_adv_lpbam_adc.h"

/** @defgroup LPBAM_ADC_Basic_Exported_Functions LPBAM ADC Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_ADC_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_ADC_FillNodeConfig(LPBAM_ADC_ConfNode_t const *const  pConfNode,
                                        DMA_NodeConfTypeDef  *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_ADC_H */
