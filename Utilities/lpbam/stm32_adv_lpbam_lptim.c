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
          (+) stm32_adv_lpbam_lptim.c file
              (++) This file provides the implementation of the advanced LPBAM LPTIM functions.
          (+) stm32_adv_lpbam_lptim.h file
              (++) This file is the header file of stm32_adv_lpbam_lptim.c. It provides advanced LPBAM LPTIM functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_lptim.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      An advanced functionality in this module can be inserted using 1 main type of functions :
          (+) ADV_LPBAM_{Peripheral}_{Operation}_SetFullQ() : Inserts a {Peripheral} functional and data transfer
                                                              configuration for the selected {Operation} in a queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following LPTIM features :
          (+) Starts the LPTIM peripheral counter in single or continuous mode.
          (+) Stops the LPTIM peripheral counter.
          (+) Update the pulse, the period and the repetition values for PWM waveform.
          (+) Get capture values for LPTIM input signal.
          (+) Get counter values for LPTIM.
          (+) Update the period and the repetition values and starts the LPTIM counter to generate an update event.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by the DMA channel.

      The LPTIM peripheral configuration to start and stop depend on the following three LPTIM modes :
          (+) PWM update mode when the LPTIM channel is configured as output.
          (+) Input Capture (IC) mode when the LPTIM channel is configured as input.
          (+) Update Event (UE) detection mode when the LPTIM peripheral is already initialized, and channel
              configuration is not needed.
      According to the chosen LPTIM mode the following configurations can be performed:
          (+) PWM mode : Update the period, the pulse and the repetition in order to change the PWM Waveform.
          (+) IC mode  : Read and store in a buffer an external Input Capture signal values.
          (+) UE mode  : Configure the Update Event period and repetition values.

      Independent from the chosen LPTIM mode the following operation can be performed:
          (+) Get Counetr : Read and store in a buffer the LPTIM counter values.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_LPTIM_Start_SetFullQ() API to build a linked-list queue that starts the LPTIM peripheral counter in
      single or in continuous mode according to parameters in the LPBAM_LPTIM_StartFullAdvConf_t structure.
      The configuration parameters are :
          (+) StartMode  : Specifies the LPTIM Start Mode.
                           This parameter can be a value of @ref LPBAM_LPTIM_Start_Mode.
          (+) WakeupIT   : Specifies the wake up source interrupt.
                           This parameter can be one or a combination of @ref LPBAM_LPTIM_Wakeup_Interrupt.
      The LPTIM peripheral shall be disabled before calling this API.

    [..]
      Use ADV_LPBAM_LPTIM_Stop_SetFullQ() API to build a linked-list queue that stops the LPTIM peripheral counter
      according to parameters in the LPBAM_LPTIM_StopFullAdvConf_t structure.
      The configuration parameters are :
          (+) Mode  : Specifies the LPTIM Mode.
                      This parameter can be a value of @ref LPBAM_LPTIM_Stop_Mode.
      The LPTIM peripheral shall be enabled before calling this API.

    [..]
      Use ADV_LPBAM_LPTIM_PWM_SetFullQ() API to build a linked-list queue that updates the PWM waveform parameters
      values at each update event request generated according to parameters in the LPBAM_LPTIM_PWMFullAdvConf_t
      structure.
      The configuration parameters are :
          (+) UpdatePeriod     : Specifies the period update state.
                                 This parameter can be ENABLE or DISABLE.
          (+) PeriodValue      : Specifies the period values.
          (+) UpdatePulse      : Specifies the pulse update state.
                                 This parameter can be ENABLE or DISABLE.
          (+) PulseValue       : Specifies the pulse values.
          (+) UpdateRepetition : Specifies the repetition update state.
                                 This parameter can be ENABLE or DISABLE.
          (+) RepetitionValue  : Specifies the repetition values.

      This API must be called when the LPTIM channel is initialized in output mode.
          (+) Recommended LPTIM peripheral initialization sequence
              (++) Call HAL_LPTIM_Init() to initialize LPTIM peripheral instance. (Mandatory)
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
      Use ADV_LPBAM_LPTIM_IC_SetFullQ() API to build a linked-list queue that starts the Input Capture mode of an LPTIM
      input signal according to parameters in the LPBAM_LPTIM_ICFullAdvConf_t structure.
      The configuration parameters are :
          (+) pData  : Specifies the address of data buffer.
          (+) Size   : Specifies the size of data to be read.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after calling
      ADV_LPBAM_LPTIM_IC_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.

      This API must be called when the LPTIM channel is initialized in input mode.
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
      Use ADV_LPBAM_LPTIM_UE_SetFullQ() API to build a linked-list queue that starts the LPTIM Update Event detection
      according to parameters in the LPBAM_LPTIM_UEFullAdvConf_t structure.
      The configuration parameters are :
          (+) Period     : Specifies the period value.
          (+) Repetition : Specifies the period repetition number.
      This API must be called when the LPTIM peripheral instance is initialized as follow.
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

      The purpose of this API is to generate an update event after (Period * Repetition) delay that can be used to
      trigger or/and wake up the system.
      This API is used to implement a delay between 2 successive LPBAM APIs execution.
      This API is used independently of LPTIM channel settings.

    [..]
      Use ADV_LPBAM_LPTIM_GetCounter_SetFullQ() API to build a linked-list queue that reads the LPTIM counter and store
      data in a buffer according to parameters in the LPBAM_LPTIM_GetCounterFullAdvConf_t structure.
      The configuration parameters are :
          (+) pData  : Specifies the address of data buffer.
          (+) Size   : Specifies the size of data to be read.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after calling
      ADV_LPBAM_LPTIM_GetCounter_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      PWM generation user sequence is :
          (+) Initialize all needed GPIOs. (Depends on application needs)
          (+) Initialize LPTIM channel in output mode (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM peripheral counter. (Mandatory)
          (+) Call n times, where n >= 1, ADV_LPBAM_LPTIM_PWM_SetFullQ() to update PWM previous parameters. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ().
              (++) This call is mandatory when the generated PWM signal is not in an infinite loop.
              (++) This call is forbidden when generating PWM signal in an infinite loop.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for an infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Input Capture user sequence is :
          (+) Initialize all needed GPIOs. (Depends on application needs)
          (+) Initialize LPTIM channel in input mode (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM channel. (Mandatory)
          (+) Call  n times, where n >= 1, ADV_LPBAM_LPTIM_IC_SetFullQ() to measure the Input Capture signal.
              (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ().
              (++) This call is mandatory when the measure of the Input Capture signal is not in an infinite loop.
              (++) This call is forbidden when measuring Input Capture signal in an infinite loop.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for an infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Update Event user sequence is :
          (+) Initialize the LPTIM peripheral (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_EnableDMARequests() to enable DMA requests and enable channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Start_SetFullQ() to start the LPTIM channel. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_UE_SetFullQ() to configure Update Event parameters. (Mandatory)
          (+) Call ADV_LPBAM_LPTIM_Stop_SetFullQ(). (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Get Counter user sequence is :
          (+) Read LPTIM counter can be performed for :
              (++) PWM mode.
              (++) IC mode.
              (++) UE mode.

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      The period and repetition parameters are common for all LPTIM channels.
      Case 1: An LPTIM channel is configured in PWM mode:
          (+) When configuring another channel in PWM mode, ensure the consistency of updating PWM period and repetition
              for both channels.
          (+) There are no constraints when configuring another channel in IC mode.
          (+) It's forbidden to use the LPTIM in update event detection mode.

      Case 2: An LPTIM channel is configured in IC mode:
          (+) There are no constraints when configuring another LPTIM channel in PWM mode or IC mode.
          (+) It's forbidden to use the LPTIM in update event detection mode.

      Case 3: The LPTIM peripheral is configured to generate an Update Event:
          (+) It's forbidden to use any LPTIM channel in any mode.

    [..]
      It's not possible to disable a single LPTIM channel. When calling ADV_LPBAM_LPTIM_Stop_SetFullQ() API, all LPTIM
      channels will be disabled.
      If the LPTIM peripheral is used to detect an Update Event, LPBAM_LPTIM_StopFullAdvConf_t structure mode field
      must be configured as LPBAM_LPTIM_UE_MODE.
      If the LPTIM peripheral is not used to detect an Update Event, LPBAM_LPTIM_StopFullAdvConf_t structure mode
      field must be configured as LPBAM_LPTIM_NO_UE_MODE.

    [..]
      It's possible to call any advanced API before or/and after ADV_LPBAM_LPTIM_PWM_SetFullQ() or/and
      ADV_LPBAM_LPTIM_IC_SetFullQ() APIs.

    [..]
      It's forbidden to call any advanced API between calling ADV_LPBAM_LPTIM_Start_SetFullQ() and
      ADV_LPBAM_LPTIM_UE_SetFullQ().

    [..]
      It's forbidden to call any advanced API between calling ADV_LPBAM_LPTIM_UE_SetFullQ() and
      ADV_LPBAM_LPTIM_Stop_SetFullQ().

    [..]
      It's mandatory to ensure the necessary timing between stopping and starting the LPTIM peripheral.

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
  * @brief  Build DMA linked-list queue to setup PWM parameters according to parameters in LPBAM_LPTIM_PWMFullAdvConf_t.
  * @param  pInstance         : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  Channel           : [IN]  Specifies the LPTIM channel.
  *                                   This parameter can be a value of @ref LPBAM_LPTIM_Channel.
  * @param  pDMAListInfo      : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                   linked-list queue type information.
  * @param  pPWMFull          : [IN]  Pointer to a LPBAM_LPTIM_PWMFullAdvConf_t structure that contains update PWM
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
  * @brief  Build DMA linked-list queue to setup IC parameters according to parameters in LPBAM_LPTIM_ICFullAdvConf_t.
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
  * @brief  Build DMA linked-list queue to setup UE detection parameters according to parameters in
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
  * @brief  Build DMA linked-list queue to read the LPTIM counter according to parameters in
  *         LPBAM_LPTIM_GetCounterFullAdvConf_t.
  * @param  pInstance       : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
  * @param  pDMAListInfo    : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                 linked-list queue type information.
  * @param  pGetCounterFull : [IN]  Pointer to a LPBAM_LPTIM_GetCounterFullAdvConf_t structure that contains get counter
  *                                 full information.
  * @param  pDescriptor     : [IN]  Pointer to a LPBAM_LPTIM_GetCounterFullDesc_t structure  that contains start full
  *                                 descriptor information.
  * @param  pQueue          : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                 information.
  * @retval LPBAM Status    : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_GetCounter_SetFullQ(LPTIM_TypeDef                       *const pInstance,
                                                   LPBAM_DMAListInfo_t                 const *const pDMAListInfo,
                                                   LPBAM_LPTIM_GetCounterFullAdvConf_t const *const pGetCounterFull,
                                                   LPBAM_LPTIM_GetCounterFullDesc_t    *const pDescriptor,
                                                   DMA_QListTypeDef                    *const pQueue)
{
  LPBAM_LPTIM_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  /*
   *               ######## LPTIM read counter value node ########
   */

  /* Get Counter buffer address */
  uint32_t buffer_address = (uint32_t)&pGetCounterFull->pData[0U];

  /* Set LPTIM instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_LPTIM_GET_COUNTER_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)buffer_address;
  config_node.Config.CaptureCount        = pGetCounterFull->Size;

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
  * @brief  Build DMA linked-list queue to start the LPTIM channel according to parameters in
  *         LPBAM_LPTIM_StartFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a LPTIM_TypeDef structure that selects LPTIM instance.
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

  /* Set LPTIM instance */
  config_node.pInstance                  = pInstance;

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
  * @brief  Build DMA linked-list queue to stop the LPTIM according to parameters in
  *         LPBAM_LPTIM_StopFullAdvConf_t.
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

  /* Check channel index */
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
