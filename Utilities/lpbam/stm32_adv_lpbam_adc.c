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
      It is strongly recommended to read carefully the LPBAM_Utility_GettingStarted.html document before starting
      developing an LPBAM application.

    *** Driver description ***
    ==========================
    [..]
      This driver is dedicated for ADC allowing to change configuration and operate in low power mode.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_adc.c
              (++) This file provides the ADC advanced files body.
          (+) stm32_adv_lpbam_adc.h
              (++) This file is the header file of stm32_adv_lpbam_adc.c. It provides advanced used types.
          (+) stm32_platform_lpbam_adc.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure the ADC peripheral for conversion.
          (+) Starts the ADC conversion.
          (+) Configure and starts the ADC conversion.
          (+) Configure and starts only the watchdog input signal monitoring without transfer to SRAM.

    *** Functional description ***
    ==============================
    [..]
      The ADC peripheral conversion is configured through a DMA channel thanks to a build DMA linked-list queue.
      The ADC peripheral conversion is started through a DMA channel thanks to a build DMA linked-list queue.
      The ADC peripheral conversion is configured and started through a DMA channel thanks to a build DMA linked-list
      queue.
      The ADC peripheral analog watchdog monitoring is configured and started without any data transfer to SRAM
      through a DMA channel thanks to a build DMA linked-list queue.

      The output of this driver is a queue to be executed by a DMA channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_ADC_Conversion_SetConfigQ() API to build a linked-list queue that setup the ADC configuration
      operation according to parameters in the LPBAM_ADC_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetDataQ() API to build a linked-list queue that setup the ADC conversion
      starting operation according to parameters in the LPBAM_ADC_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetFullQ() API to build a linked-list queue that setup the ADC configuration
      and conversion starting operation according to parameters in the LPBAM_ADC_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() API to build a linked-list queue that setup the ADC analog watchdog
      configuration operation according to parameters in the LPBAM_ADC_AWDGFullAdvConf_t structure (without any data
      transfer to SRAM).

    [..]
      Configuration parameters through ADV_LPBAM_ADC_Conversion_SetConfigQ are :
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

      Configuration parameters through ADV_ADC_Conversion_SetDataQ() are :
          (+) pData : Specifies the address of data buffer
          (+) Size  : Specifies the size of data to be read.

      Configuration parameters through ADV_LPBAM_ADC_Conversion_SetFullQ() are :
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
          (+) Size                  : Specifies DMA transfer number of ADC conversion data.

      Configuration parameters through ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() are :
          (+) WatchdogNumber : Select which ADC analog watchdog is monitoring the selected channel.
          (+) WatchdogMode   : Configure the ADC analog watchdog mode.
          (+) Channel        : Select which ADC channel to monitor by analog watchdog.
          (+) ITMode         : Specify whether the analog watchdog is configured in interrupt or polling mode.
          (+) LowThreshold   : Specifies the ADC watchdog low threshold.
          (+) HighThreshold  : Specifies the ADC watchdog high threshold.

      Configuration parameters through ADV_LPBAM_ADC_ChannelConfig() are :
          (+) Channel : Specify the channel to configure into ADC regular group.
          (+) Rank    : Specify the rank in the regular group sequencer.

    [..]
      These APIs must be called when the ADC instance is well initialized.
          (+) Recommended ADC instance initialization sequence
              (++) Call HAL_ADC_Init() to initialize the ADC instance. (Mandatory).
                   (+++) ClockPrescaler           : ADC_CLOCK_ASYNC_DIV4.
                   (+++) Resolution               : ADC_RESOLUTION_12B.
                   (+++) ScanConvMode             : ADC_SCAN_DIRECTION_FORWARD.
                   (+++) EOCSelection             : ADC_EOC_SINGLE_CONV.
                   (+++) LowPowerAutoWait         : DISABLE.
                   (+++) ContinuousConvMode       : ENABLE.
                   (+++) DiscontinuousConvMode    : DISABLE.
                   (+++) ExternalTrigConv         : ADC4_EXTERNALTRIG_LPTIM1_CH1.
                   (+++) ExternalTrigConvEdge     : ADC_EXTERNALTRIGCONVEDGE_RISING.
                   (+++) ConversionDataManagement : ADC_CONVERSIONDATA_DMA_CIRCULAR.
                   (+++) DMAContinuousRequests    : ENABLE.
                   (+++) Overrun                  : ADC_OVR_DATA_OVERWRITTEN.
                   (+++) OversamplingMode         : DISABLE.
                   (+++) Oversampling             : Specify the Oversampling parameters.
                   (+++) SamplingTimeCommon1      : ADC4_SAMPLETIME_160CYCLES_5.
                   (+++) LowPowerAutoPowerOff     : ADC_LOW_POWER_AUTOFF_DPD.
              (++) Call HAL_ADCEx_Calibration_Start() to start the ADC instance calibration.
                   (+++) CalibrationMode : ADC_CALIB_OFFSET.
                   (+++) SingleDiff      : ADC_SINGLE_ENDED.
                                           No linearity calibration neither differential mode for ADC4.
              (++) Call HAL_ADC_ConfigChannel() to configure the channels to be used for ADC conversion. (Optional).
                   (+++) Channel      : ADC_CHANNEL_4.
                   (+++) Rank         : ADC4_REGULAR_RANK_1.
                   (+++) SamplingTime : ADC4_SAMPLINGTIME_COMMON_1.
              (++) Call HAL_ADC_AnalogWDGConfig() to configure the ADC analog watchdog settings. (Optional).
                   (+++) WatchdogNumber : ADC_ANALOGWATCHDOG_1.
                   (+++) WatchdogMode   : ADC_ANALOGWATCHDOG_ALL_REG.
                   (+++) ITMode         : ENABLE.
                   (+++) HighThreshold  : ADC_AWD_THRESHOLD_HIGH_VAL.
                   (+++) LowThreshold   : ADC_AWD_THRESHOLD_LOW_VAL.
              (++) Call __HAL_ADC_ENABLE_IT() to enable error interrupts. (Optional).
                   ADC error interrupts can be :
                   (+++) ADC_IT_OVR   : ADC overrun interrupt source.

    [..]
      When using ADV_LPBAM_ADC_Conversion_SetConfigQ(), ADV_LPBAM_ADC_Conversion_SetDataQ()
      and ADV_LPBAM_ADC_Conversion_SetFullQ() APIs, the following parameters must be configured
      carefully when initializing ADC peripheral as they are not configurable by LPBAM APIs :
          (+) ClockPrescaler       : This parameter can be a value of @ref ADC_HAL_EC_COMMON_CLOCK_SOURCE.
          (+) Resolution           : This parameter can be a value of @ref ADC_HAL_EC_RESOLUTION.
          (+) Overrun              : This parameter can be a value of @ref ADC_HAL_EC_REG_OVR_DATA_BEHAVIOR.
          (+) OversamplingMode     : This parameter can be set to ENABLE or DISABLE.
          (+) Oversampling         : This parameter can be set by filling the structure @ref ADC_OversamplingTypeDef.
          (+) SamplingTimeCommon1  : This parameter can be value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.
          (+) LowPowerAutoPowerOff : This parameter can be value of @ref ADC_HAL_LowPower_DPD.
          (+) CalibrationMode      : This parameter should be @ref ADC_CALIB_OFFSET.
          (+) SingleDiff           : This parameter should be @ref ADC_SINGLE_ENDED.
                                     No linearity calibration neither differential mode for ADC4.

    [..]
      When using only ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() API, the following parameters must be configured
      carefully when initializing ADC peripheral as they are not configurable by LPBAM API :
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
          (+) LowPowerAutoPowerOff  : This parameter can be value of @ref ADC_HAL_LowPower_DPD.
          (+) CalibrationMode       : This parameter should be @ref ADC_CALIB_OFFSET.
          (+) SingleDiff              This parameter should be @ref ADC_SINGLE_ENDED.
                                      No linearity calibration neither differential mode for ADC4.
          (+) Channel               : This parameter can be a value of @ref ADC_HAL_EC_CHANNEL.
          (+) Rank                  : This parameter can be a value of @ref ADC_HAL_EC_REG_SEQ_RANKS.
          (+) SamplingTime          : This parameter can be a value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME.

    [..]
      The channels configuration for the ADC peripheral is optional as it could be reconfigured by LPBAM APIs except for
      analog watchdog API (where channels configuration for the ADC peripheral is mandatory).

    [..]
      The channels configuration for the ADC peripheral depends from the sequencer mode :
          (+) In case of sequencer configured to fully configurable, ADV_LPBAM_ADC_ChannelConfig() API could be called
              for 8 time maximum (number of possible sequencer ranks).
          (+) In case of sequencer configured to not fully configurable, ADV_LPBAM_ADC_ChannelConfig() API could
              be called for 23 time maximum (number of possible channels).

    [..]
      The analog watchdog configuration for the ADC peripheral is optional as it could be reconfigured by LPBAM APIs.

    [..]
      When the WatchdogMode is LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG (only for AWD2 and AWD3), one or a combination of
      @ref LPBAM_ADC_Channel_Selection can be selected.

    *** Driver user sequence ***
    ============================
    [..]
      Generic user sequence is :
          (+) Initialize the ADC (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_ADC_EnableDMARequests() to enable ADC DMA requests. (Mandatory)
          (+) There are three possibilities to call advanced API:
              (++) Conversion with full API :
                   (+++) Repeat calling ADV_LPBAM_ADC_Conversion_SetFullQ() until complete LPBAM scenario. (Mandatory)
                   (+++) Repeat calling ADV_LPBAM_ADC_ChannelConfig() after each ADV_LPBAM_ADC_Conversion_SetFullQ()
                         call to add ADC channels for conversion. (Mandatory)
              (++) Conversion with configuration and data APIs :
                   (+++) Call ADV_LPBAM_ADC_Conversion_SetConfigQ() only one time. (Mandatory)
                   (+++) Repeat calling ADV_LPBAM_ADC_ChannelConfig() after each ADV_LPBAM_ADC_Conversion_SetConfigQ()
                         call to add ADC channels for conversion. (Mandatory)
                   (+++) Repeat calling ADV_LPBAM_ADC_Conversion_SetDataQ() until complete LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios
              cases. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_ADC_Conversion_SetFullQ() output queue. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Init() to initialize the DMA in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the built queue to the DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    [..]
      When the ADC configuration matches with application needs, it's strongly recommended to use
      ADV_LPBAM_ADC_Conversion_SetDataQ() API instead of ADV_LPBAM_ADC_Conversion_SetFullQ() for memory optimization
      purpose.

    [..]
      Use ADV_LPBAM_ADC_Conversion_SetFullQ() when ADC reconfiguration is needed instead of calling
      ADV_LPBAM_ADC_Conversion_SetConfigQ() then ADV_LPBAM_ADC_Conversion_SetDataQ() for memory optimization purpose.

    [..]
      Use ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() when only ADC analog watchdog reconfiguration is needed instead of
      calling ADV_LPBAM_ADC_Conversion_SetConfigQ() or ADV_LPBAM_ADC_Conversion_SetFullQ() for memory optimization
      purpose if the ADC configuration matches with application needs (channels).

    *** Recommendation ***
    ======================
    [..]
      It's useless to call ADV_LPBAM_ADC_Conversion_SetConfigQ() consecutively as the first configuration will be
      overwritten.

    [..]
      It's useless to call ADV_LPBAM_ADC_Conversion_SetConfigQ() before ADV_LPBAM_ADC_Conversion_SetFullQ() as
      ADV_LPBAM_ADC_Conversion_SetFullQ() API contains configuration process.

    [..]
      It's mandatory to call ADV_LPBAM_ADC_ChannelConfig() after ADV_LPBAM_ADC_Conversion_SetFullQ()
      or ADV_LPBAM_ADC_Conversion_SetConfigQ() to configure ADC channels to be used.

    [..]
      It's strongly recommended to call ADV_LPBAM_ADC_ChannelConfig() staring from LPBAM_ADC_REGULAR_RANK_1
      incrementally to LPBAM_ADC_REGULAR_RANK_8 in case of "ScanConvMode" is set to LPBAM_ADC_SCAN_ENABLE.

    [..]
      It's strongly not recommended to call ADV_LPBAM_ADC_Conversion_SetFullQ() with the same instance by more than one
      linked-list queue. When the ADC nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains ADC configuration and data
      nodes by two different DMA channels as unexpected behavior can appear.

    [..]
      When using ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ() with LPBAM_ADC_ANALOGWATCHDOG_NONE value, it's worth to
      mention that registers will be reset to default values and the Analog Watchdog will be disabled.

    [..]
      When using ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ(), it's worth to mention that previous settings will be
      overwritten (for all analog watchdogs). Therefore, modifying one AWD settings will erase other AWDs settings.

    *** Driver status description ***
    =================================
    [..]
      This driver detects and reports any detected issue.
          (+) returned values are :
              (++) LPBAM_OK when no error is detected.
              (++) LPBAM_ERROR when error is detected.
              (++) LPBAM_INVALID_ID when an invalid node ID is detected. This error value is specific for LPBAM basic
                   layer.

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
  * @brief  Build the ADC conversion configuration nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the ADC conversion data nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the ADC conversion full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_ADC_FullAdvConf_t.
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
  * @brief  Build the ADC analog watchdog full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_ADC_AWDGFullAdvConf_t.
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
