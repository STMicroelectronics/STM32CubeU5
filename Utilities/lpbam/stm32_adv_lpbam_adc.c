/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_adc.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for ADC peripheral.
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
  @verbatim
  ======================================================================================================================
                                 ############### How to use this driver ###############
  ======================================================================================================================
    [..]
      It is recommended to read the LPBAM_Utility_GettingStarted.html document, available at the root of LPBAM utility
      folder, prior to any LPBAM application development start.

    *** Driver description ***
    ==========================
    [..]
      This section provide description of the driver files content (refer to LPBAM_Utility_GettingStarted.html document
      for more information)

    [..]
      This LPBAM modules deals with the peripheral instances that support autonomous mode.
      It is composed of 3 files :
          (+) stm32_adv_lpbam_adc.c file
              (++) This file provides the implementation of the advanced LPBAM ADC functions.
          (+) stm32_adv_lpbam_adc.h file
              (++) This file is the header file of stm32_adv_lpbam_adc.c. It provides advanced LPBAM ADC functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_adc.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 4 model of APIs :
          (+) ADV_LPBAM_{Module}_{Mode}_SetConfigQ() : provides one peripheral configuration queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetDataQ()   : provides one peripheral data transfer queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ()   : provides one peripheral configuration and one data transfer
                                                       queue.
          (+) ADV_LPBAM_{Module}_ChannelConfig()     : helper API to tune ADC channel configuration on built queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following ADC features :
          (+) Starts the ADC conversion.
          (+) Configure and starts the ADC conversion and analog watchdog monitoring.
          (+) Configure and starts only the analog watchdog monitoring without data transfer.
          (+) Configure the ADC peripheral for conversion and analog watchdog monitoring.
          (+) Stop the ADC conversion.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by a DMA channel in applicative side.

      The ADC peripheral conversion is configured and started.
      The ADC peripheral channels are configured.
      The ADC peripheral converted data is stored in a buffer.
      The ADC peripheral analog watchdog monitoring is configured and started without any data transfer.
      The ADC peripheral conversion is stopped.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetConfigQ() API to build a linked-list queue that setups the ADC configuration
      according to parameters in the LPBAM_ADC_ConfigAdvConf_t structure.
      The configuration parameters are :
          (+) ScanConvMode          : Specify the scan conversion mode.
                                      This parameter can be a value of @ref LPBAM_ADC_Sequence_Scan_Mode.
          (+) ContinuousConvMode    : Specify whether the conversion is performed in single or continuous mode.
                                      This parameter can be ENABLE or DISABLE.
          (+) DiscontinuousConvMode : Specify whether the conversions sequence of ADC group regular is performed in
                                      Complete-sequence/Discontinuous-sequence.
                                      This parameter can be ENABLE or DISABLE.
          (+) ExternalTrigConv      : Select the external event source used to trigger ADC conversion start.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_Selection.
          (+) ExternalTrigConvEdge  : Select the external event edge used to trigger ADC  conversion start.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_Polarity.
          (+) TriggerFrequencyMode  : Set ADC trigger frequency mode.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_LowFrequency.
          (+) LowPowerAutoWait      : Select the dynamic low power Auto Delay.
                                      This parameter can be ENABLE or DISABLE.
          (+) DMAContinuousRequests : Specify whether the DMA requests are performed in one shot or in continuous
                                      mode.
                                      This parameter can be ENABLE or DISABLE.
          (+) AnalogWDGConfig       : Structure definition of ADC analog watchdog.
              (++) WatchdogNumber       : Select which ADC analog watchdog is monitoring the selected channel.
                                          This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Number.
              (++) WatchdogMode         : Configure the ADC analog watchdog mode.
                                          This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Mode.
              (++) Channel              : Select which ADC channel to monitor by analog watchdog.
                                          This parameter can be a value of @ref LPBAM_ADC_Channel_Selection.
              (++) ITMode               : Specify whether the analog watchdog is configured in interrupt or polling
                                          mode.
                                          This parameter can be ENABLE or DISABLE.
              (++) LowThreshold         : Specifies the ADC watchdog low threshold.
                                          This parameter can be a value from 0 to 0xFFF.
              (++) HighThreshold        : Specifies the ADC watchdog high threshold.
                                          This parameter can be a value from 0 to 0xFFF.

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetDataQ() API to build a linked-list queue that setup the data ADC conversion
      according to parameters in the LPBAM_ADC_DataAdvConf_t structure.
      The configuration parameters are :
          (+) pData : Specifies the address of data buffer.
          (+) Size  : Specifies the number of conversion to be transferred from ADC peripheral to memory.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_ADC_Conversion_SetDataQ().
      The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value :
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetFullQ() API to build a linked-list queue that setup the ADC configuration
      and conversion operation according to parameters in the LPBAM_ADC_FullAdvConf_t structure.
      The configuration parameters are :
          (+) ScanConvMode          : Configure the sequencer of ADC groups regular.
                                      This parameter can be a value of @ref LPBAM_ADC_Sequence_Scan_Mode.
          (+) ContinuousConvMode    : Specify whether the conversion is performed in single or continuous mode.
                                      This parameter can be ENABLE or DISABLE.
          (+) DiscontinuousConvMode : Specify whether the conversions sequence of ADC group regular is performed in
                                      Complete-sequence/Discontinuous-sequence.
                                      This parameter can be ENABLE or DISABLE.
          (+) ExternalTrigConv      : Select the external event source used to trigger ADC conversion start.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_Selection.
          (+) ExternalTrigConvEdge  : Select the external event edge used to trigger ADC  conversion start.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_Polarity .
          (+) TriggerFrequencyMode  : Set ADC trigger frequency mode.
                                      This parameter can be a value of @ref LPBAM_ADC_Trigger_LowFrequency.
          (+) LowPowerAutoWait      : Select the dynamic low power Auto Delay.
                                      This parameter can be ENABLE or DISABLE.
          (+) DMAContinuousRequests : Specify whether the DMA requests are performed in one shot or in continuous
                                      mode.
                                      This parameter can be ENABLE or DISABLE.
          (+) AnalogWDGConfig       : Structure definition of ADC analog watchdog.
              (++) WatchdogNumber       : Select which ADC analog watchdog is monitoring the selected channel.
                                          This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Number.
              (++) WatchdogMode         : Configure the ADC analog watchdog mode.
                                          This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Mode.
              (++) Channel              : Select which ADC channel to monitor by analog watchdog.
                                          This parameter can be a value of @ref LPBAM_ADC_Channel_Selection.
              (++) ITMode               : Specify whether the analog watchdog is configured in interrupt or polling
                                          mode.
                                          This parameter can be ENABLE or DISABLE.
              (++) LowThreshold         : Specifies the ADC watchdog low threshold.
                                          This parameter can be a value from 0 to 0xFFF.
              (++) HighThreshold        : Specifies the ADC watchdog high threshold.
                                          This parameter can be a value from 0 to 0xFFF.
          (+) pData                 : Specifies the address of data buffer.
          (+) Size                  : Specifies the number of conversion to be transferred from ADC peripheral to
                                      memory.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_ADC_Conversion_SetFullQ().
      The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.

    [..]
      Use ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() API to build a linked-list queue that setup the ADC analog watchdog
      monitoring configuration according to parameters in the LPBAM_ADC_AWDGFullAdvConf_t structure (without
      any data transfer).
      The configuration parameters are :
          (+) WatchdogNumber : Select which ADC analog watchdog is monitoring the selected channel.
          (+) WatchdogMode   : Configure the ADC analog watchdog mode.
          (+) Channel        : Select which ADC channel to monitor by analog watchdog.
          (+) ITMode         : Specify whether the analog watchdog is configured in interrupt or polling mode.
          (+) LowThreshold   : Specifies the ADC watchdog low threshold.
          (+) HighThreshold  : Specifies the ADC watchdog high threshold.

    [..]
      Use ADV_LPBAM_ADC_Stop_SetFullQ() API to build a linked-list queue that stop the ADC conversion.

    [..]
      Use ADV_LPBAM_ADC_ChannelConfig() API to build a linked-list queue that setup the ADC channel configuration
      after calling ADV_LPBAM_ADC_Conversion_SetConfigQ() and ADV_LPBAM_ADC_Conversion_SetFullQ() APIs according to
      parameters in the LPBAM_ADC_ChannelConfig_t structure.
      The configuration parameters are :
          (+) Channel : Specify the channel to configure into ADC regular group.
          (+) Rank    : Specify the rank in the regular group sequencer.
                        This parameter can be a value of @ref LPBAM_ADC_Channel_RANK.
                        This parameter is discarded when the ScanConvMode field in LPBAM_ADC_ConfigAdvConf_t or
                        LPBAM_ADC_FullAdvConf_t is configured as LPBAM_ADC_SCAN_DIRECTION_FORWARD or
                        LPBAM_ADC_SCAN_DIRECTION_BACKWARD.
      When ScanConvMode field in LPBAM_ADC_ConfigAdvConf_t or LPBAM_ADC_FullAdvConf_t is configured as
      LPBAM_ADC_SCAN_ENABLE, only one channel can be configured at each
      API call.
      When ScanConvMode field in LPBAM_ADC_ConfigAdvConf_t or LPBAM_ADC_FullAdvConf_t is configured as
      LPBAM_ADC_SCAN_DIRECTION_FORWARD or LPBAM_ADC_SCAN_DIRECTION_BACKWARD, this API can be called only one time by
      oring all needed channels in the user application.
      Example : Channel = LPBAM_ADC_CHANNEL_5 | LPBAM_ADC_CHANNEL_11;

    [..]
      These APIs must be called when the ADC instance is initialized.
          (+) Recommended ADC instance initialization sequence
              (++) Call HAL_ADC_Init() to initialize the ADC instance. (Mandatory).
                   (+++) ClockPrescaler           : Any according to application needs.
                   (+++) Resolution               : Any according to application needs.
                   (+++) ScanConvMode             : Any according to application needs.
                   (+++) EOCSelection             : Any according to application needs.
                   (+++) LowPowerAutoWait         : Any according to application needs.
                   (+++) ContinuousConvMode       : Any according to application needs.
                   (+++) DiscontinuousConvMode    : Any according to application needs.
                   (+++) ExternalTrigConv         : Any according to application needs.
                   (+++) ExternalTrigConvEdge     : Any according to application needs.
                   (+++) DMAContinuousRequests    : ENABLE. (Mandatory value)
                   (+++) Overrun                  : ADC_OVR_DATA_OVERWRITTEN. (Mandatory value)
                   (+++) OversamplingMode         : DISABLE. (Mandatory value)
                   (+++) SamplingTimeCommon1      : Any according to application needs.
                   (+++) SamplingTimeCommon2      : Any according to application needs.
                   (+++) LowPowerAutoPowerOff     : ADC_LOW_POWER_AUTOFF_DPD. (Mandatory value)
                   (+++) VrefProtection           : ADC_VREF_PROT_NONE. (Mandatory value when there no share between
                                                    multiple instance of ADC)
              (++) Call HAL_ADC_ConfigChannel() to configure the channels to be used for ADC conversion. (Optional).
                   (+++) Channel      : Any according to application needs.
                   (+++) Rank         : ADC4_REGULAR_RANK_1. This parameter is discarded when the ScanConvMode field in
                                                             LPBAM_ADC_ConfigAdvConf_t or LPBAM_ADC_FullAdvConf_t is
                                                             configured as LPBAM_ADC_SCAN_ENABLE.
                   (+++) SamplingTime : ADC4_SAMPLINGTIME_COMMON_1. This parameter is discarded when the ScanConvMode
                                                                    field in LPBAM_ADC_ConfigAdvConf_t or
                                                                    LPBAM_ADC_FullAdvConf_t is configured as
                                                                    LPBAM_ADC_SCAN_ENABLE.
              (++) Call HAL_ADC_AnalogWDGConfig() to configure the ADC analog watchdog settings. (Optional).
                   (+++) WatchdogNumber : Any according to application needs.
                   (+++) WatchdogMode   : Any according to application needs.
                   (+++) ITMode         : Any according to application needs.
                   (+++) HighThreshold  : Any according to application needs.
                   (+++) LowThreshold   : Any according to application needs.
              (++) Call __HAL_ADC_ENABLE_IT() to enable error interrupts. (Optional).
                   ADC error interrupts can be :
                   (+++) ADC_IT_OVR   : ADC overrun interrupt source.

    [..]
      When using ADV_LPBAM_ADC_Conversion_SetConfigQ(), ADV_LPBAM_ADC_Conversion_SetDataQ()
      and ADV_LPBAM_ADC_Conversion_SetFullQ() APIs, the following parameters must be configured
      carefully when initializing ADC peripheral as they are not reconfigurable by LPBAM APIs :
          (+) ClockPrescaler       : This parameter can be a value of @ref ADC_HAL_EC_COMMON_CLOCK_SOURCE.
          (+) Resolution           : This parameter can be a value of @ref ADC_HAL_EC_RESOLUTION.
          (+) Overrun              : This parameter can be a value of @ref ADC_HAL_EC_REG_OVR_DATA_BEHAVIOR.
          (+) OversamplingMode     : This parameter can be set to ENABLE or DISABLE.
          (+) Oversampling         : This parameter can be set by filling the structure @ref ADC_OversamplingTypeDef.
          (+) SamplingTimeCommon1  : This parameter can be value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.
          (+) SamplingTimeCommon2  : This parameter can be value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.
          (+) LowPowerAutoPowerOff : This parameter can be value of @ref ADC_HAL_LowPower_DPD.

    [..]
      When using only ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() API, the following parameters must be configured
      carefully when initializing ADC peripheral as they are not reconfigurable by LPBAM API :
          (+) ClockPrescaler        : This parameter can be a value of @ref ADC_HAL_EC_COMMON_CLOCK_SOURCE.
          (+) Resolution            : This parameter can be a value of @ref ADC_HAL_EC_RESOLUTION.
          (+) ScanConvMode          : This parameter can be a value of @ref ADC_Scan_mode.
          (+) LowPowerAutoWait      : This parameter can be set to ENABLE or DISABLE.
          (+) ContinuousConvMode    : This parameter can be set to ENABLE or DISABLE.
          (+) DiscontinuousConvMode : This parameter can be set to ENABLE or DISABLE.
          (+) ExternalTrigConv      : This parameter can be a value of @ref ADC_regular_external_trigger_source.
          (+) ExternalTrigConvEdge  : This parameter can be a value of @ref ADC_regular_external_trigger_edge.
          (+) DMAContinuousRequests : This parameter can be set to ENABLE or DISABLE.
          (+) Overrun               : This parameter can be a value of @ref ADC_HAL_EC_REG_OVR_DATA_BEHAVIOR.
          (+) OversamplingMode      : This parameter can be set to ENABLE or DISABLE.
          (+) Oversampling          : This parameter can be set by filling the structure @ref ADC_OversamplingTypeDef.
          (+) SamplingTimeCommon1   : This parameter can be value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.
          (+) SamplingTimeCommon2   : This parameter can be value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.
          (+) LowPowerAutoPowerOff  : This parameter can be value of @ref ADC_HAL_LowPower_DPD.
          (+) Channel               : This parameter can be a value of @ref ADC_HAL_EC_CHANNEL.
          (+) Rank                  : This parameter can be a value of @ref ADC_HAL_EC_REG_SEQ_RANKS.
          (+) SamplingTime          : This parameter can be a value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.

    [..]
      The channels configuration for the ADC peripheral through HAL or LL is optional as it could be reconfigured by
      LPBAM APIs except for analog watchdog API (where channels configuration for the ADC peripheral is mandatory).

    [..]
      The analog watchdog configuration for the ADC peripheral through HAL or LL is optional as it could be reconfigured
      by LPBAM APIs.

    [..]
      When the WatchdogMode is LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG (only for AWD2 and AWD3), one or a combination of
      @ref LPBAM_ADC_Channel_Selection can be selected.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Generic user sequence is :
          (+) Initialize all used GPIOs for input channels. (Mandatory)
          (+) Initialize the ADC (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_ADC_EnableDMARequests() to enable ADC DMA requests. (Mandatory)
          (+) There are three possibilities to call LPBAM advanced APIs:
              (++) Single API making configuration and data setup :
                   (+++) Call ADV_LPBAM_ADC_Conversion_SetFullQ(). (Mandatory)
                   (+++) Call ADV_LPBAM_ADC_ChannelConfig() after each ADV_LPBAM_ADC_Conversion_SetFullQ() call to add
                         ADC channels for conversion. (Mandatory)
              (++) Two APIs making configuration and data setup separately :
                   (+++) Call ADV_LPBAM_ADC_Conversion_SetConfigQ(). (Mandatory)
                   (+++) Call ADV_LPBAM_ADC_ChannelConfig() after each ADV_LPBAM_ADC_Conversion_SetConfigQ() call to add
                         ADC channels for conversion. (Mandatory)
                   (+++) Call ADV_LPBAM_ADC_Conversion_SetDataQ(). (Mandatory)
              (++) Single API making only the configuration of the ADC analog watchdog :
                   (+++) Call ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ().
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_ADC_Conversion_SetConfigQ(), ADV_LPBAM_ADC_Conversion_SetDataQ(),
              ADV_LPBAM_ADC_Conversion_SetFullQ() and ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() output queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list master queue for infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a master DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized master DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the master DMA channel linked-list execution. (Mandatory)
          (+) Call, when needed, ADV_LPBAM_ADC_Stop_SetFullQ() to stop the ADC conversion.

    *** Recommendations ***
    =======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      For memory optimization purpose, when no reconfiguration is necessary, it's recommended to use
      ADV_LPBAM_ADC_Conversion_SetDataQ().

    [..]
      For memory optimization purpose, when a reconfiguration is necessary, it's recommended to use
      ADV_LPBAM_ADC_Conversion_SetFullQ() instead of ADV_LPBAM_ADC_Conversion_SetConfigQ() then
      ADV_LPBAM_ADC_Conversion_SetDataQ().

    [..]
      For memory optimization purpose, when only ADC analog watchdog reconfiguration is needed, use
      ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() instead of ADV_LPBAM_ADC_Conversion_SetConfigQ() or
      ADV_LPBAM_ADC_Conversion_SetFullQ().

    [..]
      It's mandatory to call ADV_LPBAM_ADC_ChannelConfig() after ADV_LPBAM_ADC_Conversion_SetFullQ()
      or ADV_LPBAM_ADC_Conversion_SetConfigQ() to configure ADC channels to be used.

    [..]
      When ScanConvMode field in LPBAM_ADC_ConfigAdvConf_t structure is set to LPBAM_ADC_SCAN_ENABLE, configuring ranks
      shall be done contiguously.

    [..]
      It's not allowed to configure analog watchdogs separately through ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ().
      At each call, the three watchdogs are reconfigured.
      It's recommended to ensure the consistency between analog watchdog configurations when calling
      ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() or ADV_LPBAM_ADC_Conversion_SetConfigQ() or
      ADV_LPBAM_ADC_Conversion_SetFullQ().

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same peripheral.

    *** Driver status description ***
    =================================
    [..]
      This section provides reported LPBAM module status.

    [..]
      This advanced module reports any detected issue.
          (+) returned values are :
              (++) LPBAM_OK when no error is detected.
              (++) LPBAM_ERROR when any error is detected.

    @endverbatim
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_lpbam.h"

#ifdef LPBAM_ADC_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup ADC_Advanced ADC_Advanced
  * @brief    ADC Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_ADC_Advanced_Exported_Functions LPBAM Advanced ADC Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to setup the configuration of ADC conversion according to parameters in
  *         LPBAM_ADC_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pConvConfig  : [IN]  Pointer to a LPBAM_ADC_ConfigAdvConf_t structure that contains conversion configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_ADC_ConvConfigDesc_t structure that contains conversion
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetConfigQ(ADC_TypeDef                *const pInstance,
                                                   LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                   LPBAM_ADC_ConfigAdvConf_t  const *const pConvConfig,
                                                   LPBAM_ADC_ConvConfigDesc_t *const pDescriptor,
                                                   DMA_QListTypeDef           *const pQueue)
{
  LPBAM_ADC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dmaNodeConfig;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## ADC stop node ########
   */

  /* Set ADC instance */
  config_node.pInstance                   = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID    = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType  = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg         = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State                = DISABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 1 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_AWDG_THRESHOLD_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].WatchdogMode   = pConvConfig->AnalogWDGConfig[0U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[0U].Channel        = pConvConfig->AnalogWDGConfig[0U].Channel;
  config_node.Config.AnalogWDGConfig[0U].LowThreshold   = pConvConfig->AnalogWDGConfig[0U].LowThreshold;
  config_node.Config.AnalogWDGConfig[0U].HighThreshold  = pConvConfig->AnalogWDGConfig[0U].HighThreshold;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 1 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].LowThreshold   = pConvConfig->AnalogWDGConfig[1U].LowThreshold;
  config_node.Config.AnalogWDGConfig[1U].HighThreshold  = pConvConfig->AnalogWDGConfig[1U].HighThreshold;
  config_node.Config.AnalogWDGConfig[1U].WatchdogMode   = pConvConfig->AnalogWDGConfig[1U].WatchdogMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 2 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].LowThreshold   = pConvConfig->AnalogWDGConfig[2U].LowThreshold;
  config_node.Config.AnalogWDGConfig[2U].HighThreshold  = pConvConfig->AnalogWDGConfig[2U].HighThreshold;
  config_node.Config.AnalogWDGConfig[2U].WatchdogMode   = pConvConfig->AnalogWDGConfig[2U].WatchdogMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 3 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC trigger configuration */
  config_node.Config.ExternalTrigConv                   = pConvConfig->ExternalTrigConv;
  config_node.Config.ExternalTrigConvEdge               = pConvConfig->ExternalTrigConvEdge;
  config_node.Config.LowPowerAutoWait                   = pConvConfig->LowPowerAutoWait;

  /* Set ADC channel configuration */
  config_node.Config.ScanConvMode                       = pConvConfig->ScanConvMode;
  config_node.Config.ContinuousConvMode                 = pConvConfig->ContinuousConvMode;
  config_node.Config.DiscontinuousConvMode              = pConvConfig->DiscontinuousConvMode;

  /* Set ADC analog watchdog configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_DEFAULT;

  /* Set ADC DMA circular or one shot mode */
  config_node.Config.DMAContinuousRequests              = pConvConfig->DMAContinuousRequests;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC channel selection node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CHANNELSELECTION_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build channel selection node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC clock prescaler node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CLKPRESCALER_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build prescaler node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC trigger low frequency node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID    = (uint32_t)LPBAM_ADC_TRIG_LOWFREQUENCY_ID;
  config_node.NodeDesc.pSrcVarReg         = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.TriggerFrequencyMode = pConvConfig->TriggerFrequencyMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build trigger low frequency node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 monitor node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_MONITOR_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].WatchdogMode   = pConvConfig->AnalogWDGConfig[1U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[1U].Channel        = pConvConfig->AnalogWDGConfig[1U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 2 monitor node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 monitor node ########
   */

  /* Set ADC instance */
  config_node.pInstance                                 = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_MONITOR_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].WatchdogMode   = pConvConfig->AnalogWDGConfig[2U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[2U].Channel        = pConvConfig->AnalogWDGConfig[2U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 3 monitor node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID          = (uint32_t)LPBAM_ADC_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg               = &pDescriptor->pReg[reg_idx];

  /* Set ADC wakeup interrupt */
  config_node.Config.AnalogWDGConfig[0U].ITMode = pConvConfig->AnalogWDGConfig[0U].ITMode;
  config_node.Config.AnalogWDGConfig[1U].ITMode = pConvConfig->AnalogWDGConfig[1U].ITMode;
  config_node.Config.AnalogWDGConfig[2U].ITMode = pConvConfig->AnalogWDGConfig[2U].ITMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build wake up interrupt node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data ADC conversion according to parameters in
  *         LPBAM_ADC_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pConvData    : [IN]  Pointer to a LPBAM_ADC_DataAdvConf_t structure that contains conversion data
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_ADC_ConvDataDesc_t structure that contains conversion data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetDataQ(ADC_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_ADC_DataAdvConf_t  const *const pConvData,
                                                 LPBAM_ADC_ConvDataDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue)
{
  LPBAM_ADC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dmaNodeConfig;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## ADC start node ########
   */

  /* Set ADC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State               = ENABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build start node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## ADC data node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)&pConvData->pData[0U];

  /* Set Data Size */
  config_node.Config.Size              = pConvData->Size;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and the data ADC conversion according to parameters
  *         in LPBAM_ADC_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pConvFull    : [IN]  Pointer to a LPBAM_ADC_FullAdvConf_t structure that contains conversion full
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_ADC_ConvFullDesc_t structure that contains conversion full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Conversion_SetFullQ(ADC_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_ADC_FullAdvConf_t  const *const pConvFull,
                                                 LPBAM_ADC_ConvFullDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue)
{
  LPBAM_ADC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dmaNodeConfig;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## ADC stop node ########
   */

  /* Set ADC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State               = DISABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## Clear ADC flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 1 threshold node ########
   */


  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_THRESHOLD_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].WatchdogMode   = pConvFull->AnalogWDGConfig[0U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[0U].Channel        = pConvFull->AnalogWDGConfig[0U].Channel;
  config_node.Config.AnalogWDGConfig[0U].LowThreshold   = pConvFull->AnalogWDGConfig[0U].LowThreshold;
  config_node.Config.AnalogWDGConfig[0U].HighThreshold  = pConvFull->AnalogWDGConfig[0U].HighThreshold;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 1 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].LowThreshold   = pConvFull->AnalogWDGConfig[1U].LowThreshold;
  config_node.Config.AnalogWDGConfig[1U].HighThreshold  = pConvFull->AnalogWDGConfig[1U].HighThreshold;
  config_node.Config.AnalogWDGConfig[1U].WatchdogMode   = pConvFull->AnalogWDGConfig[1U].WatchdogMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 2 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].LowThreshold   = pConvFull->AnalogWDGConfig[2U].LowThreshold;
  config_node.Config.AnalogWDGConfig[2U].HighThreshold  = pConvFull->AnalogWDGConfig[2U].HighThreshold;
  config_node.Config.AnalogWDGConfig[2U].WatchdogMode   = pConvFull->AnalogWDGConfig[2U].WatchdogMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 3 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC trigger configuration */
  config_node.Config.ExternalTrigConv                   = pConvFull->ExternalTrigConv;
  config_node.Config.ExternalTrigConvEdge               = pConvFull->ExternalTrigConvEdge;
  config_node.Config.LowPowerAutoWait                   = pConvFull->LowPowerAutoWait;

  /* Set ADC channel configuration */
  config_node.Config.ScanConvMode                       = pConvFull->ScanConvMode;
  config_node.Config.ContinuousConvMode                 = pConvFull->ContinuousConvMode;
  config_node.Config.DiscontinuousConvMode              = pConvFull->DiscontinuousConvMode;

  /* Set ADC analog watchdog configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_DEFAULT;

  /* Set ADC DMA circular or one shot mode */
  config_node.Config.DMAContinuousRequests              = pConvFull->DMAContinuousRequests;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC channel selection node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CHANNELSELECTION_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build channel selection node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC clock prescaler node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CLKPRESCALER_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build prescaler node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC trigger low frequency node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID    = (uint32_t)LPBAM_ADC_TRIG_LOWFREQUENCY_ID;
  config_node.NodeDesc.pSrcVarReg         = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.TriggerFrequencyMode = pConvFull->TriggerFrequencyMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build trigger low frequency node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 monitor node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_MONITOR_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_2;

  /* Set ADC AWDG configuration */
  config_node.Config.AnalogWDGConfig[1U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].Channel        = pConvFull->AnalogWDGConfig[1U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 2 monitor node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 monitor node ########
   */

  /* Set ADC instance */
  config_node.pInstance                                 = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_MONITOR_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_3;

  /* Set ADC AWDG configuration */
  config_node.Config.AnalogWDGConfig[2U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].Channel        = pConvFull->AnalogWDGConfig[2U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 3 monitor node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC interrupt state node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID          = (uint32_t)LPBAM_ADC_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg               = &pDescriptor->pReg[reg_idx];

  /* Set ADC wakeup interrupt */
  config_node.Config.AnalogWDGConfig[0U].ITMode = pConvFull->AnalogWDGConfig[0U].ITMode;
  config_node.Config.AnalogWDGConfig[1U].ITMode = pConvFull->AnalogWDGConfig[1U].ITMode;
  config_node.Config.AnalogWDGConfig[2U].ITMode = pConvFull->AnalogWDGConfig[2U].ITMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC start node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State             = ENABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build start node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;


  /*
   *               ######## ADC data node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)&pConvFull->pData[0U];

  /* Set Data Size */
  config_node.Config.Size              = pConvFull->Size;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration of the ADC analog watchdog according to parameters
  *         in LPBAM_ADC_AWDGFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pAWDGFull    : [IN]  Pointer to a LPBAM_ADC_AWDGFullAdvConf_t structure that contains conversion full
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_ADC_AWDGFullDesc_t structure that contains conversion full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ(ADC_TypeDef                 *const pInstance,
                                                      LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                                      LPBAM_ADC_AWDGFullAdvConf_t const *const pAWDGFull,
                                                      LPBAM_ADC_AWDGFullDesc_t    *const pDescriptor,
                                                      DMA_QListTypeDef            *const pQueue)
{
  LPBAM_ADC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dmaNodeConfig;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;


  /*
   *               ######## ADC stop node ########
   */

  /* Set ADC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State               = DISABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## Clear ADC flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 1 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                  = (uint32_t)LPBAM_ADC_AWDG_THRESHOLD_ID;
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].WatchdogNumber = pAWDGFull->AnalogWDGConfig[0U].WatchdogNumber;
  config_node.Config.AnalogWDGConfig[0U].WatchdogMode   = pAWDGFull->AnalogWDGConfig[0U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[0U].LowThreshold   = pAWDGFull->AnalogWDGConfig[0U].LowThreshold;
  config_node.Config.AnalogWDGConfig[0U].HighThreshold  = pAWDGFull->AnalogWDGConfig[0U].HighThreshold;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 1 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].WatchdogNumber = pAWDGFull->AnalogWDGConfig[1U].WatchdogNumber;
  config_node.Config.AnalogWDGConfig[1U].WatchdogMode   = pAWDGFull->AnalogWDGConfig[1U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[1U].LowThreshold   = pAWDGFull->AnalogWDGConfig[1U].LowThreshold;
  config_node.Config.AnalogWDGConfig[1U].HighThreshold  = pAWDGFull->AnalogWDGConfig[1U].HighThreshold;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 1 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 threshold node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                       = &pDescriptor->pReg[reg_idx];

  /* Set ADC AWDG configuration */
  config_node.Config.UpdateAnalogWDG                    = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].WatchdogNumber = pAWDGFull->AnalogWDGConfig[2U].WatchdogNumber;
  config_node.Config.AnalogWDGConfig[2U].WatchdogMode   = pAWDGFull->AnalogWDGConfig[2U].WatchdogMode;
  config_node.Config.AnalogWDGConfig[2U].LowThreshold   = pAWDGFull->AnalogWDGConfig[2U].LowThreshold;
  config_node.Config.AnalogWDGConfig[2U].HighThreshold  = pAWDGFull->AnalogWDGConfig[2U].HighThreshold;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build analog watchdog 1 threshold node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 1 monitor node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID           = (uint32_t)LPBAM_ADC_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg                = &pDescriptor->pReg[reg_idx];

  /* Set ADC analog watchdog configuration */
  config_node.Config.UpdateAnalogWDG             = LPBAM_ADC_ANALOGWATCHDOG_1;
  config_node.Config.AnalogWDGConfig[0U].Channel = pAWDGFull->AnalogWDGConfig[0U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 2 monitor node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID           = (uint32_t)LPBAM_ADC_AWDG_MONITOR_ID;
  config_node.NodeDesc.pSrcVarReg                = &pDescriptor->pReg[reg_idx];

  /* Set ADC analog watchdog configuration */
  config_node.Config.UpdateAnalogWDG             = LPBAM_ADC_ANALOGWATCHDOG_2;
  config_node.Config.AnalogWDGConfig[1U].Channel = pAWDGFull->AnalogWDGConfig[1U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC analog watchdog 3 monitor node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg                = &pDescriptor->pReg[reg_idx];

  /* Set ADC analog watchdog configuration */
  config_node.Config.UpdateAnalogWDG             = LPBAM_ADC_ANALOGWATCHDOG_3;
  config_node.Config.AnalogWDGConfig[2U].Channel = pAWDGFull->AnalogWDGConfig[2U].Channel;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC interrupt state node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID          = (uint32_t)LPBAM_ADC_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg               = &pDescriptor->pReg[reg_idx];

  /* Set ADC Wakeup interrupt */
  config_node.Config.AnalogWDGConfig[0U].ITMode = pAWDGFull->AnalogWDGConfig[0U].ITMode;
  config_node.Config.AnalogWDGConfig[1U].ITMode = pAWDGFull->AnalogWDGConfig[1U].ITMode;
  config_node.Config.AnalogWDGConfig[2U].ITMode = pAWDGFull->AnalogWDGConfig[2U].ITMode;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## ADC start node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set ADC configuration */
  config_node.Config.State             = ENABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build start node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to stop of the ADC conversion
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_ADC_ConvStopDesc_t structure that contains stop conversion
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_Stop_SetFullQ(ADC_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_ADC_ConvStopDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue)
{
  LPBAM_ADC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dmaNodeConfig;

  /*
   *               ######## ADC stop node ########
   */

  /* Set ADC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_ADC_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0];

  /* Set ADC configuration */
  config_node.Config.State               = DISABLE;

  /* Fill node configuration */
  if (LPBAM_ADC_FillNodeConfig(&config_node, &dmaNodeConfig) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &pDescriptor->pNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to ADC queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Configure the ADC channel in LPBAM_ADC_FullAdvConf_t or LPBAM_ADC_ConfigAdvConf_t according to configured
  *         parameters in LPBAM_ADC_ChannelAdvConf_t.
  * @param  pChannel     : [IN]  Pointer to a LPBAM_ADC_ChannelAdvConf_t structure that contains conversion full
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Void pointer to a LPBAM_ADC_FullAdvConf_t or LPBAM_ADC_ConfigAdvConf_t structure
  *                              that contains conversion descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_ChannelConfig(LPBAM_ADC_ChannelAdvConf_t *const pChannel,
                                           void                       *pDescriptor)
{
  /* Get descriptor base address */
  uint32_t config_adc_node_addr = ((uint32_t)pDescriptor);
  uint32_t channelsel_node_addr = ((uint32_t)pDescriptor);
  uint32_t intchannel_node_addr = ((uint32_t)pDescriptor);

  /* Calculate nodes addresses */
  config_adc_node_addr += (5U * sizeof(DMA_NodeTypeDef));
  channelsel_node_addr += (6U * sizeof(DMA_NodeTypeDef));
  intchannel_node_addr += (7U * sizeof(DMA_NodeTypeDef));

  /* Calculate nodes DMA source address offsets */
  uint32_t preg_config_adc_addr = ((DMA_NodeTypeDef *)config_adc_node_addr)->LinkRegisters[NODE_CSAR_DEFAULT_OFFSET];
  uint32_t preg_channelsel_addr = ((DMA_NodeTypeDef *)channelsel_node_addr)->LinkRegisters[NODE_CSAR_DEFAULT_OFFSET];
  uint32_t preg_intchannel_addr = ((DMA_NodeTypeDef *)intchannel_node_addr)->LinkRegisters[NODE_CSAR_DEFAULT_OFFSET];

  uint32_t channel_sel;
  uint32_t dummy = 0UL;

  /* Check ADC channel */
  switch (pChannel->ChannelConfig.Channel)
  {
    case LPBAM_ADC_CHANNEL_VREFINT:
    {
      /* Set VREFEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) |= (1UL << LPBAM_ADC_VREFEN_POS);

      break;
    }

    case LPBAM_ADC_CHANNEL_TEMPSENSOR:
    {
      /* Set VSENSEEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) |= (1UL << LPBAM_ADC_VSENSEEN_POS);

      break;
    }

    case LPBAM_ADC_CHANNEL_VBAT:
    {
      /* Set VBATEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) |= (1UL << LPBAM_ADC_VBATEN_POS);

      break;
    }

    case LPBAM_ADC_CHANNEL_0:
    {
      /* Clear VREFEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) &= ~(1UL << LPBAM_ADC_VREFEN_POS);

      break;
    }

    case LPBAM_ADC_CHANNEL_13:
    {
      /* Clear VSENSEEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) &= ~(1UL << LPBAM_ADC_VSENSEEN_POS);

      break;
    }

    case LPBAM_ADC_CHANNEL_14:
    {
      /* Clear VBATEN bit in CCR register */
      *(uint32_t *)(preg_intchannel_addr) &= ~(1UL << LPBAM_ADC_VBATEN_POS);

      break;
    }

    default:
    {
      break;
    }
  }

  channel_sel = pChannel->ChannelConfig.Channel & (~CHANNEL_ID_INTERNAL_CH);

  /* Check scan mode */
  if ((*(uint32_t *)preg_config_adc_addr & LPBAM_ADC_CHSELRMOD) == 0U)
  {
    /* Update register with channel */
    *(uint32_t *)(preg_channelsel_addr) |= channel_sel;
  }
  else
  {
    /* Force default value to next RANK to stop sequence */
    dummy |= (LPBAM_ADC_CHANNEL_RANK_DEFAULT << ((pChannel->ChannelConfig.Rank + 4U) & 0x1FU));

    /* Update register with channel in the appropriate RANK */
    MODIFY_REG((dummy), (LPBAM_ADC_CHANNEL_RANK_DEFAULT << ((pChannel->ChannelConfig.Rank) & 0x1FU)),
               (POSITION_VAL(channel_sel) << ((pChannel->ChannelConfig.Rank) & 0x1FU)));

    /* Set register  */
    *(uint32_t *)(preg_channelsel_addr) |= dummy ;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the ADC DMA requests.
  * @param  pInstance    : [IN]  Pointer to a ADC_TypeDef structure that selects ADC instance.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_ADC_EnableDMARequests(ADC_TypeDef *const pInstance)
{
  /* Enable ADC DMA requests */
  pInstance->CFGR1 |= LPBAM_ADC_DMAEN;

  /* Enable ADC instance */
  pInstance->CR |= LPBAM_ADC_ADEN;

  return LPBAM_OK;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* LPBAM_ADC_MODULE_ENABLED */
