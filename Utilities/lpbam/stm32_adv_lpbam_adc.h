/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_adc.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_adc.c file
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32_ADV_LPBAM_ADC_H
#define STM32_ADV_LPBAM_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_adc.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup ADC_Advanced ADC_Advanced
  * @brief    ADC Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_ADC_Advanced_Exported_Types LPBAM Advanced ADC Exported Types
  * @brief    LPBAM ADC Advanced Exported Types
  * @{
  */

/**
  * @brief LPBAM specific advanced ADC information structure definition.
  */
typedef struct
{
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
                                                              Duration value: Refer to device data sheet, parameter
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

  LPBAM_ADC_AnalogWDGConfig_t AnalogWDGConfig[3U]; /*!< Structure definition of ADC analog watchdog.
                                                        Please refer to LPBAM_ADC_AnalogWDGConfig_t definition in
                                                        stm32_platform_lpbam_adc.h for more information.              */

} LPBAM_ADC_ConfigAdvConf_t;

/**
  * @brief LPBAM specific advanced ADC information structure definition.
  */
typedef struct
{
  uint16_t Size;                               /*!< Specifies DMA transfer number of ADC conversion data              */

  uint32_t *pData;                             /*!< Specifies the address of data buffer                              */

  LPBAM_FunctionalState DMAContinuousRequests; /*!< Specify whether the DMA requests are performed in one shot mode
                                                    (DMA transfer stops when number of conversions is reached) or in
                                                     continuous mode (DMA transfer unlimited, whatever number of
                                                     conversions).
                                                     This parameter can be set to ENABLE or DISABLE.
                                                     Note: In continuous mode, DMA must be configured in circular mode.
                                                           Otherwise an overrun will be triggered when DMA buffer
                                                           maximum pointer is reached.                                */
} LPBAM_ADC_DataAdvConf_t;

/**
  * @brief LPBAM specific advanced ADC information structure definition.
  */
typedef struct
{
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
                                                              Duration value: Refer to device data sheet, parameter
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

  LPBAM_ADC_AnalogWDGConfig_t AnalogWDGConfig[3U]; /*!< Structure definition of ADC analog watchdog.
                                                        Please refer to LPBAM_ADC_AnalogWDGConfig_t definition in
                                                        stm32_platform_lpbam_adc.h for more information.              */

  uint16_t Size;                                   /*!< Specifies DMA transfer number of ADC conversion data          */

  uint32_t *pData;                                 /*!< Specifies the address of data buffer                          */

} LPBAM_ADC_FullAdvConf_t;

/**
  * @brief LPBAM specific advanced ADC information structure definition.
  */
typedef struct
{
  LPBAM_ADC_AnalogWDGConfig_t AnalogWDGConfig[3U]; /*!< Structure definition of ADC analog watchdog.
                                                        Please refer to LPBAM_ADC_AnalogWDGConfig_t definition in
                                                        stm32_platform_lpbam_adc.h for more information.          */
} LPBAM_ADC_AWDGFullAdvConf_t;

/**
  * @brief LPBAM specific advanced ADC information structure definition.
  */
typedef struct
{
  LPBAM_ADC_ChannelConfig_t ChannelConfig; /*!< Structure definition of ADC Channel Configuration.
                                                Please refer to LPBAM_ADC_ChannelConfig_t definition in
                                                stm32xx_platform_lpbam_adc.h for more information.        */

} LPBAM_ADC_ChannelAdvConf_t;

/**
  * @brief LPBAM Start conversion descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[12U]; /*!< Specifies the content of nodes required for DMA queue execution : 12 different
                                    nodes are needed                                                                  */

  uint32_t pReg[12U];          /*!< Specifies the content of register to be updated : 12 different values are needed  */

} LPBAM_ADC_ConvConfigDesc_t;

/**
  * @brief LPBAM Start conversion descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for DMA queue execution : 2 different nodes
                                   are needed                                                                         */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_ADC_ConvDataDesc_t;

/**
  * @brief LPBAM Start conversion descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[14U]; /*!< Specifies the content of nodes required for DMA queue execution : 14 different
                                    nodes are needed                                                                  */

  uint32_t pReg[13U];          /*!< Specifies the content of register to be updated : 13 different values are needed  */

} LPBAM_ADC_ConvFullDesc_t;

/**
  * @brief LPBAM Analog WatchDog Settings descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[10U]; /*!< Specifies the content of nodes required for DMA queue execution : 10 different
                                    nodes are needed                                                                  */

  uint32_t pReg[10U];          /*!< Specifies the content of register to be updated : 10 different values are needed  */

} LPBAM_ADC_AWDGFullDesc_t;

/**
  * @brief LPBAM Stop conversion descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_ADC_ConvStopDesc_t;

/**
  * @brief LPBAM Channel Settings descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for DMA queue execution : 2 different nodes
                                   are needed                                                                         */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 different values are needed    */

} LPBAM_ADC_ChannelFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_ADC_Advanced_Exported_Functions LPBAM Advanced ADC Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_ADC_Conversion_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetConfigQ(ADC_TypeDef                *const pInstance,
                                                   LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                   LPBAM_ADC_ConfigAdvConf_t  const *const pConvConfig,
                                                   LPBAM_ADC_ConvConfigDesc_t *const pDescriptor,
                                                   DMA_QListTypeDef           *const pQueue);
/**
  * @brief ADV_LPBAM_ADC_Conversion_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetDataQ(ADC_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_ADC_DataAdvConf_t  const *const pConvData,
                                                 LPBAM_ADC_ConvDataDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_ADC_Conversion_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetFullQ(ADC_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_ADC_FullAdvConf_t  const *const pConvFull,
                                                 LPBAM_ADC_ConvFullDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue);

/**
  * @brief ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ(ADC_TypeDef                 *const pInstance,
                                                      LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                                      LPBAM_ADC_AWDGFullAdvConf_t const *const pAWDGFull,
                                                      LPBAM_ADC_AWDGFullDesc_t    *const pDescriptor,
                                                      DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_ADC_Stop_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Stop_SetFullQ(ADC_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_ADC_ConvStopDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_ADC_ChannelConfig.
  */
LPBAM_Status_t ADV_LPBAM_ADC_ChannelConfig(LPBAM_ADC_ChannelAdvConf_t *const pChannel,
                                           void                       *pDescriptor);
/**
  * @brief ADV_LPBAM_ADC_EnableDMARequests.
  */
LPBAM_Status_t ADV_LPBAM_ADC_EnableDMARequests(ADC_TypeDef *const pInstance);
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


#endif /* STM32_ADV_LPBAM_ADC_H */
