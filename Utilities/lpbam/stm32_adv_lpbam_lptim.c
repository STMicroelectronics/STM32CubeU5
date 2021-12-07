/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_lptim.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for LPTIM peripheral.
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
      This driver is dedicated for LPTIM that supports linked-list feature.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_lptim.c
              (++) This file provides the LPTIM advanced files body.
          (+) stm32_adv_lpbam_lptim.h
              (++) This file is the header file of stm32_adv_lpbam_lptim.c. It provides used types
          (+) stm32_platform_lpbam_lptim.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Starts the LPTIM channel for PWM, input capture and update event detection modes.
          (+) Stops the LPTIM peripheral for PWM, input capture and update event detection modes.
          (+) Update the pulse, the period and the repetition values for PWM waveform.
          (+) Get capture values for LPTIM input signal.
          (+) Update the period and the repetition values and starts the LPTIM counter to generate an update event.

    *** Functional description ***
    ==============================
    [..]
      The LPTIM peripheral can be started and stopped through a DMA channel thanks to a build DMA linked-list queue in
      three different mode :
          (+) PWM mode where the LPTIM channel is configured as output.
          (+) Input capture mode where the LPTIM channel is configured as input.
          (+) update event detection mode where the LPTIM is initialized (No need for channel configuration).
      The PWM Waveform can be changed by updating the period, pulse and the repetition thanks to a build DMA linked-list
      queue.
      A input capture of external signal can be read and stored in a buffer thanks to a build DMA linked-list queue.
      An update event detection can be configured and started thanks to a build DMA linked-list queue.

      The output of this driver is a queue to be executed by the DMA channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_LPTIM_Start_SetFullQ() API to build a linked-list queue that starts the LPTIM channel in single or
      in continuous mode according to parameters in the LPBAM_LPTIM_StartFullAdvConf_t structure.
      Configured parameters are :
          (+) StartMode  : Specifies the LPTIM Start Mode.
                           This parameter can be a value of @ref LPBAM_LPTIM_ContinuousMode.
          (+) WakeupIT   : Specifies the wake up source interrupt.
                           This parameter can be one or a combination of @ref LPBAM_LPTIM_Wakeup_Interrupt.
      This API must be called when the LPTIM is disabled for each mode.

    [..]
      Use ADV_LPBAM_LPTIM_Stop_SetFullQ() API to build a linked-list queue that stops the LPTIM peripheral in single or
      in continuous mode according to parameters in the LPBAM_LPTIM_StopFullAdvConf_t structure.
      Configured parameters are :
          (+) Mode  : Specifies the LPTIM Mode.
                      This parameter can be a value of @ref LPBAM_LPTIM_Stop_Mode.
      This API must be called when the LPTIM is enabled for each mode in start a new operation.

    [..]
      Use ADV_LPBAM_LPTIM_PWM_SetFullQ() API to build a linked-list queue that updates at each update event request
      generated the the period or/and the pulse or/and the repetition values according to parameters in the
      LPBAM_LPTIM_StartFullAdvConf_t structure.
      Configuration parameters are :
          (+) UpdatePeriod     : Specifies the period update state
                                 This parameter can be ENABLE or DISABLE.
          (+) PeriodValue      : Specifies the period values.
          (+) UpdatePulse      : Specifies the pulse update state.
                                 This parameter can be ENABLE or DISABLE.
          (+) PulseValue       : Specifies the pulse values.
          (+) UpdateRepetition : Specifies the repetition update state.
                                 This parameter can be ENABLE or DISABLE.
          (+) RepetitionValue  : Specifies the repetition values.

      This API must be called when the LPTIM channel is well initialized PWM mode.
          (+) Recommended LPTIM initialization sequence
              (++) Call HAL_LPTIM_Init() to initialize LPTIM Instance. (Mandatory)
                   Initialization parameters can be :
                   (+++) CounterSource                 : LPTIM_COUNTERSOURCE_INTERNAL.
                   (+++) UpdateMode                    : LPTIM_UPDATE_IMMEDIATE.
                   (+++) Clock Source                  : LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC.
                   (+++) Clock Prescaler               : LPTIM_PRESCALER_DIV1.
                   (+++) UltraLowPowerClock Polarity   : LPTIM_CLOCKPOLARITY_RISING.
                   (+++) UltraLowPowerClock SampleTime : LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION.
                   (+++) Trigger Source                : LPTIM_TRIGSOURCE_SOFTWARE.
                   (+++) Trigger ActiveEdge            : LPTIM_ACTIVEEDGE_RISING.
                   (+++) Trigger SampleTime            : LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION.
                   (+++) Period                        : Initial period value.
                   (+++) RepetitionCounter             : Initial period repetition value.
              (++) Call HAL_LPTIM_OC_ConfigChannel() to configure the LPTIM channel as Output. (Mandatory)
                   Initialization parameters can be :
                   (+++) Pulse      : Initial pulse value.
                   (+++) OCPolarity : LPTIM_OCPOLARITY_HIGH.

    [..]
      Use ADV_LPBAM_LPTIM_IC_SetFullQ() API to build a linked-list queue that starts the input capture mode of an LPTIM
      input signal according to parameters in the LPBAM_LPTIM_ICFullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configuration parameters are :
          (+) pData  : Specifies the address of data buffer.
          (+) Size   : Specifies the size of data to be read.
      This API must be called when the LPTIM Instance is initialized in input mode.
          (+) Recommended LPTIM initialization sequence
              Initialization parameters can be :
              (++) Call HAL_LPTIM_Init() to initialize LPTIM Instance. (Mandatory)
                   (+++) CounterSource                 : LPTIM_COUNTERSOURCE_INTERNAL.
                   (+++) UpdateMode                    : LPTIM_UPDATE_IMMEDIATE.
                   (+++) Clock Source                  : LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC.
                   (+++) Clock.Prescaler               : LPTIM_PRESCALER_DIV1 .
                   (+++) UltraLowPowerClock Polarity   : LPTIM_CLOCKPOLARITY_RISING.
                   (+++) UltraLowPowerClock SampleTime : LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION.
                   (+++) Trigger Source                : LPTIM_TRIGSOURCE_SOFTWARE.
                   (+++) Trigger ActiveEdge            : LPTIM_ACTIVEEDGE_RISING.
                   (+++) Trigger SampleTime            : LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION.
                   (+++) TInput1Source                 : LPTIM_INPUT1SOURCE_GPIO.
                   (+++) TInput2Source                 : LPTIM_INPUT2SOURCE_GPIO.
                   (+++) Period                        : Initial period value.
                   (+++) RepetitionCounter             : Initial period repetition value.
              (++) Call HAL_LPTIM_IC_ConfigChannel() to configure the LPTIM channel as input. (Mandatory)
                   Initialization parameters can be :
                   (+++) ICFilter      : LPTIM_ICFLT_CLOCK_DIV1.
                   (+++) ICPolarity    : LPTIM_ICPOLARITY_RISING.
                   (+++) ICPrescaler   : LPTIM_ICPSC_DIV1.
                   (+++) ICInputSource : LPTIM_IC2SOURCE_GPIO.
              (++) Call __HAL_LPTIM_ENABLE_IT() to enable LPTIM Over Capture interrupt.
                   Any LPTIM error occurred in low power block the LPBAM sub-system mechanisms.
                   LPTIM  interrupts can be :
                   (+++) LPTIM_IT_CC1O : Capture/compare 1 over-capture interrupt enable.
                   (+++) LPTIM_IT_CC2O : Capture/compare 2 over-capture interrupt enable.

    [..]
      Use ADV_LPBAM_LPTIM_UE_SetFullQ() API to build a linked-list queue that starts the LPTIM update event detection
      according to parameters in the LPBAM_LPTIM_UEFullAdvConf_t structure.
      Configuration parameters are :
          (+) Period     : Specifies the period value.
          (+) Repetition : Specifies the period repetition number.
      This API must be called when the LPTIM Instance is well initialized as follow.
          (+) Recommended LPTIM initialization sequence
              (++) Call HAL_LPTIM_Init() to initialize LPTIM Instance. (Mandatory)
                   Initialization parameters can be :
                   (+++) CounterSource                 : LPTIM_COUNTERSOURCE_INTERNAL.
                   (+++) UpdateMode                    : LPTIM_UPDATE_IMMEDIATE.
                   (+++) Clock Source                  : LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC.
                   (+++) Clock Prescaler               : LPTIM_PRESCALER_DIV1.
                   (+++) UltraLowPowerClock Polarity   : LPTIM_CLOCKPOLARITY_RISING.
                   (+++) UltraLowPowerClock SampleTime : LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION.
                   (+++) Trigger Source                : LPTIM_TRIGSOURCE_SOFTWARE.
                   (+++) Trigger ActiveEdge            : LPTIM_ACTIVEEDGE_RISING.
                   (+++) Trigger SampleTime            : LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION.
                   (+++) Period                        : Initial period value.
                   (+++) RepetitionCounter             : Initial period repetition value.

      The purpose of this API is to generate an update event after (Period * Repetition) delay that can be used for
      trigger and system wake up purpose.
      This API is used to implement a delay between 2 successives LPBAM APIs execution.
      The channels outputs are not enabled and no LPTIM channel signal is propagated.

    *** Driver user sequence ***
    ============================
    [..]
      PWM generation user sequence is :
          (+) Initialize LPTIM channel in output mode (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM channel. (Mandatory)
          (+) Repeat calling of ADV_LPBAM_LPTIM_PWM_SetFullQ() API to customize PWM signal for your application.
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ(). (When your scenario is not circular/Mandatory to restart another
              LPTIM mode)
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios cases.
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Link() to link the output queue to a master DMA channel. (Mandatory)
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Input capture user sequence is :
          (+) Initialize LPTIM channel in input mode (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM channel. (Mandatory)
          (+) Repeat calling of ADV_LPBAM_LPTIM_IC_SetFullQ() API according to application.
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ(). (When your scenario is not circular/Mandatory to restart another
              LPTIM mode)
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios cases.
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Link() to link the output queue to a master DMA channel. (Mandatory)
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Update event user sequence is :
          (+) Initialize the LPTIM (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_UE_SetFullQ() API according to application.
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ(). (Mandatory)
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios cases.
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Link() to link the output queue to a master DMA channel. (Mandatory)
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    *** Recommendation ***
    ======================
    [..]
      The period and repetition parameters are common for all LPTIM channels.
      Case 1: An LPTIM channel is configured in PWM mode:
          (+) When configuring another channel in PWM mode, it's mandatory to ensure that the same period and repetition
              are applied for all LPTIM channels when channels operates simultaneously.
          (+) There are no constraints when configuring another channel in IC mode.
          (+) When configuring the LPTIM to generate an update event, it's mandatory to ensure that the same period and
              repetition are applied for all LPTIM channels.

      Case 2: An LPTIM channel is configured in IC mode:
          (+) There are no constraints when configuring another LPTIM channel in PWM mode.
          (+) There are no constraints when configuring another LPTIM channel in IC mode.
          (+) There are no constraints when configuring the LPTIM to generate an update event.

      Case 3: The LPTIM is configured to generate an update event:
          (+) When configuring a channel in PWM mode, it's mandatory to ensure that the same period and repetition used
              for update event generation are applied for the LPTIM channels.
          (+) There are no constraints when configuring another LPTIM channel in IC mode.

    [..]
      When calling ADV_LPBAM_LPTIM_Stop_SetFullQ(), all LPTIM channels are disabled. So, disabling one LPTIM channel is
      not possible.
      If the LPTIM is used to generate an update event, Mode field in the LPBAM_LPTIM_StopFullAdvConf_t structure must
      be configured as LPBAM_LPTIM_UE_MODE independently of channels mode configuration otherwise LPBAM_LPTIM_NO_UE_MODE
      value for Mode field must be configured.

    [..]
      It's strongly not recommended to call any LPTIM API with the same channel by more than one linked-list queue. When
      the LPTIM nodes will be executed simultaneously unexpected behavior will appear.

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

/* Includes ------------------------------------------------------------------*/
#include "stm32_lpbam.h"

#ifdef LPBAM_LPTIM_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup LPTIM_Advanced LPTIM_Advanced
  * @brief    LPTIM Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_LPTIM_Advanced_Exported_Functions LPBAM LPTIM Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build the LPTIM PWM full DMA linked-list queue according to configured parameters in
  *         LPBAM_LPTIM_PWMFullAdvConf_t.
  * @param  pInstance         : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  Channel           : [IN]  Specifies the LPTIM channel.
  *                                   This parameter can be a value of @ref LPBAM_LPTIM_Channel.
  * @param  pDMAListInfo      : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                   linked-list queue type information.
  * @param  pPWMFull          : [IN]  Pointer to a LPBAM_LPTIM_FullAdvConf_t structure that contains update PWM
  *                                   information.
  * @param  pDescriptor       : [IN]  Pointer to a LPBAM_LPTIM_PWMFullDesc_t structure  that contains update PWM
  *                                   descriptor information.
  * @param  pQueue            : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                   information.
  * @retval LPBAM Status      : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM_TypeDef                *const pInstance,
                                            uint32_t                     Channel,
                                            LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                            LPBAM_LPTIM_PWMFullAdvConf_t const *const pPWMFull,
                                            LPBAM_LPTIM_PWMFullDesc_t    *const pDescriptor,
                                            DMA_QListTypeDef             *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  uint32_t reg_idx  = 0U;
  uint32_t node_idx = 0U;

  /* Set LPTIM instance and channel */
  config_node.pInstance                  = pInstance;
  config_node.Config.Channel             = Channel;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;

  /* Set LPTIM configuration */
  config_node.Config.Mode                = LPBAM_LPTIM_NO_UE_MODE;
  config_node.Config.Size                = 1U;

  if (pPWMFull->UpdatePeriod == ENABLE)
  {
    /*
     *               ######## LPTIM period node ########
     */

    /* Set period value */
    pDescriptor->pReg[reg_idx]           = pPWMFull->PeriodValue;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_UPDATE_PERIOD_ID;
    config_node.NodeDesc.pSrcVarReg      = (uint32_t *)&pDescriptor->pReg[reg_idx];
    config_node.NodeDesc.pBuff           = NULL;

    /* Fill node configuration */
    if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build clear flags nodes */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert Node to LPTIM queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update register and node indexes */
    node_idx++;
    reg_idx++;
  }


  if (pPWMFull->UpdatePulse == ENABLE)
  {
    /*
     *               ######## LPTIM pulse node ########
     */

    /* Set pulse value */
    pDescriptor->pReg[reg_idx]            = pPWMFull->PulseValue;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID  = (uint32_t)LPBAM_LPTIM_UPDATE_PULSE_ID;
    config_node.NodeDesc.pSrcVarReg       = (uint32_t *)&pDescriptor->pReg[reg_idx];
    config_node.NodeDesc.pBuff            = NULL;

    /* Fill node configuration */
    if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    if (pPWMFull->UpdatePeriod == ENABLE)
    {
      dma_node_conf.Init.Request   = DMA_REQUEST_SW;
      dma_node_conf.Init.Direction = DMA_MEMORY_TO_MEMORY;
    }

    /* Build clear flags nodes */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert Node to LPTIM queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update register and node indexes */
    node_idx++;
    reg_idx++;
  }


  if (pPWMFull->UpdateRepetition == ENABLE)
  {
    /*
     *               ######## LPTIM repetition node ########
     */


    /* Set repetition value */
    pDescriptor->pReg[reg_idx]            = pPWMFull->RepetitionValue;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID  = (uint32_t)LPBAM_LPTIM_UPDATE_REPETITION_ID;
    config_node.NodeDesc.pSrcVarReg       = (uint32_t *)&pDescriptor->pReg[reg_idx];
    config_node.NodeDesc.pBuff            = NULL;

    /* Fill node configuration */
    if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update DMA request and direction */
    dma_node_conf.Init.Request   = DMA_REQUEST_SW;
    dma_node_conf.Init.Direction = DMA_MEMORY_TO_MEMORY;

    /* Build clear flags nodes */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert Node to LPTIM queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update register and node indexes */
    node_idx++;
    reg_idx++;
  }


  if (pPWMFull->UpdatePeriod == DISABLE)
  {
    /*
     *               ######## LPTIM clear flag node ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_CLEARFLAG_ID;
    config_node.NodeDesc.pSrcVarReg      = (uint32_t *)&pDescriptor->pReg[reg_idx];

    /* Fill node configuration */
    if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update DMA request and direction */
    dma_node_conf.Init.Request   = DMA_REQUEST_SW;
    dma_node_conf.Init.Direction = DMA_MEMORY_TO_MEMORY;

    /* Build clear flags nodes */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert Node to LPTIM queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the LPTIM input capture full DMA linked-list queue according to configured parameters in
  *         LPBAM_LPTIM_ICFullAdvConf_t.
  * @param  pInstance          : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  Channel            : [IN]  Specifies the LPTIM channel.
  *                                    This parameter can be a value of @ref LPBAM_LPTIM_Channel.
  * @param  pDMAListInfo       : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                    linked-list queue type information.
  * @param  pICFull            : [IN]  Pointer to a LPBAM_LPTIM_ICFullAdvConf_t structure that contains input capture
  *                                    information.
  * @param  pDescriptor        : [IN]  Pointer to a LPBAM_LPTIM_ICFullDesc_t structure  that contains input capture
  *                                    descriptor information.
  * @param  pQueue             : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                    information.
  * @retval LPBAM Status       : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_IC_SetFullQ(LPTIM_TypeDef               *const pInstance,
                                           uint32_t                    Channel,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_LPTIM_ICFullAdvConf_t const *const pICFull,
                                           LPBAM_LPTIM_ICFullDesc_t    *const pDescriptor,
                                           DMA_QListTypeDef            *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  /* Get repetition buffer address */
  uint32_t buffer_address = (uint32_t)&pICFull->pData[0U];

  /*
   *               ######## LPTIM input capture node ########
   */

  /* Set LPTIM instance and channel */
  config_node.pInstance                  = pInstance;
  config_node.Config.Channel             = Channel;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_LPTIM_INPUT_CAPTURE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)buffer_address;

  /* Set LPTIM configuration */
  config_node.Config.Mode                = (uint32_t)LPBAM_LPTIM_NO_UE_MODE;
  config_node.Config.CaptureCount        = pICFull->Size;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flags nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the LPTIM update event detection full DMA linked-list queue according to configured parameters in
  *         LPBAM_LPTIM_UEFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pUEFull      : [IN]  Pointer to a LPBAM_LPTIM_UEFullAdvConf_t structure that contains update event detection
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_LPTIM_UEFullDesc_t structure that contains update event detection
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_UE_SetFullQ(LPTIM_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_LPTIM_UEFullAdvConf_t const *const pUEFull,
                                           LPBAM_LPTIM_UEFullDesc_t    *const pDescriptor,
                                           DMA_QListTypeDef            *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /* Set LPTIM instance */
  config_node.pInstance                   = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeType  = pDMAListInfo->QueueType;

  /*
   *               ######## LPTIM period node ########
   */

  /* Set period value */
  pDescriptor->pReg[reg_idx]           = pUEFull->Period;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_UPDATE_PERIOD_ID;
  config_node.NodeDesc.pSrcVarReg      = (uint32_t *)&pDescriptor->pReg[reg_idx];
  config_node.NodeDesc.pBuff           = NULL;

  /* Set LPTIM configuration */
  config_node.Config.Mode              = LPBAM_LPTIM_UE_MODE;
  config_node.Config.Size              = 1U;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flags nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update register and node indexes */
  node_idx++;
  reg_idx++;


  /*
   *               ######## LPTIM repetition node ########
   */

  /* Set repetition value */
  pDescriptor->pReg[reg_idx]           = pUEFull->Repetition;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_UPDATE_REPETITION_ID;
  config_node.NodeDesc.pSrcVarReg      = (uint32_t *)&pDescriptor->pReg[reg_idx];

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flags nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the LPTIM start full DMA linked-list queue according to configured parameters in
  *         LPBAM_LPTIM_StartFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  Channel      : [IN]  Specifies the LPTIM channel.
  *                              This parameter can be a value of @ref LPBAM_LPTIM_Channel.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pStartFull   : [IN]  Pointer to a LPBAM_LPTIM_StartFullAdvConf_t structure that contains start full
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_LPTIM_StartFullDesc_t structure  that contains start full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_Start_SetFullQ(LPTIM_TypeDef                  *const pInstance,
                                              uint32_t                       Channel,
                                              LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                              LPBAM_LPTIM_StartFullAdvConf_t const *const pStartFull,
                                              LPBAM_LPTIM_StartFullDesc_t    *const pDescriptor,
                                              DMA_QListTypeDef               *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  uint32_t reg_idx  = 0U;
  uint32_t node_idx = 0U;

  /*
   *               ######## LPTIM enable node ########
   */

  /* Set LPTIM instance and channel */
  config_node.pInstance                  = pInstance;
  config_node.Config.Channel             = Channel;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_LPTIM_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set LPTIM configuration */
  config_node.Config.State               = ENABLE;
  config_node.Config.UpdateStartMode     = DISABLE;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update register and node indexes */
  node_idx++;
  reg_idx++;


  /*
   *               ######## LPTIM wake up it node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set LPTIM configuration */
  config_node.Config.WakeupIT          = pStartFull->WakeupIT;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update register and node indexes */
  node_idx++;
  reg_idx++;


  /*
   *               ######## LPTIM configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_LPTIM_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set LPTIM configuration */
  config_node.Config.UpdateStartMode   = ENABLE;
  config_node.Config.StartMode         = pStartFull->StartMode;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the LPTIM stop full DMA linked-list queue.
  * @param  pInstance    : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  pStopFull    : [IN]  Pointer to a LPBAM_LPTIM_StopFullAdvConf_t structure that contains stop full
  *                              information.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_LPTIM_StopFullDesc_t structure  that contains start full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_Stop_SetFullQ(LPTIM_TypeDef                 *const pInstance,
                                             LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                             LPBAM_LPTIM_StopFullAdvConf_t const *const pStopFull,
                                             LPBAM_LPTIM_StopFullDesc_t    *const pDescriptor,
                                             DMA_QListTypeDef              *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  /*
   *               ######## LPTIM stop node ########
   */

  /* Set LPTIM instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_LPTIM_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set LPTIM configuration */
  config_node.Config.State               = DISABLE;
  config_node.Config.Mode                = pStopFull->Mode;

  /* Fill node configuration */
  if (LPBAM_LPTIM_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Node to LPTIM queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the LPTIM DMA requests.
  * @param  pInstance    : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  Channel      : [IN]  Specifies the LPTIM channel.
  *                              This parameter can be a value or a combination of @ref LPBAM_LPTIM_Channel.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_EnableDMARequests(LPTIM_TypeDef *const pInstance,
                                                 uint32_t      Channel)
{
  /* Enable update event request */
  pInstance->DIER |= LPBAM_LPTIM_UEDE;

  /* Check channel index */
  if ((Channel & LPBAM_LPTIM_CHANNEL_1) == LPBAM_LPTIM_CHANNEL_1)
  {
    /* Enable Channel */
    pInstance->CCMR1 |= LPBAM_LPTIM_CC1E;

    /* Enable input capture DMA request */
    pInstance->DIER |= LPBAM_LPTIM_CC1DE;
  }

  if ((Channel & LPBAM_LPTIM_CHANNEL_2) == LPBAM_LPTIM_CHANNEL_2)
  {
    /* Enable Channel */
    pInstance->CCMR1 |= LPBAM_LPTIM_CC2E;

    /* Enable input capture DMA request */
    pInstance->DIER |= LPBAM_LPTIM_CC2DE;
  }

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
#endif /* LPBAM_LPTIM_MODULE_ENABLED */
