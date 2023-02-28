/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_dac.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for DAC peripheral.
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
          (+) stm32_adv_lpbam_dac.c file
              (++) This file provides the implementation of the advanced LPBAM DAC functions.
          (+) stm32_adv_lpbam_dac.h file
              (++) This file is the header file of stm32_adv_lpbam_dac.c. It provides advanced LPBAM DAC functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_dac.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 3 model of APIs :
          (+) ADV_LPBAM_{Module}_{Mode}_SetConfigQ() : provides one peripheral configuration queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetDataQ()   : provides one peripheral data transfer queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ()   : provides one peripheral configuration and one data transfer
                                                       queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following DAC features :
          (+) Configures the DAC peripheral conversion.
          (+) Starts the DAC peripheral conversion.
          (+) Configures and starts the DAC peripheral conversion.
          (+) Stop the DAC peripheral conversion.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by a DMA channel in applicative side.

      The DAC peripheral is configured and started.
      The DAC conversion hardware trigger signal is configurable.
      The DAC input data is stored in a buffer then sent by a DMA channel to be converted in an analog output signal.
      The DAC peripheral is stopped.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_DAC_Conversion_SetConfigQ() API to build a linked-list queue that configures the DAC
      peripheral according to parameters in the LPBAM_DAC_ConfigAdvConf_t structure.
      The configuration parameters are :
          (+) DAC_Trigger : Specifies the DAC source trigger.
                            This parameter can be a value of @ref DAC_trigger_selection.

    [..]
      Use ADV_LPBAM_DAC_Conversion_SetDataQ() API to build a linked-list queue that configures data transfer
      according to parameters in the LPBAM_DAC_DataAdvConf_t structure.
      The configuration parameters are :
          (+) Alignment : Specifies the DAC alignment.
                          This parameter can be a value of @ref LPBAM_DAC_DataAlignment.
          (+) pData     : Specifies the source buffer of data .
          (+) Size      : Specifies the size of pData.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_DAC_Conversion_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.

    [..]
      Use ADV_LPBAM_DAC_Conversion_SetFullQ() API to build a linked-list queue that configures DAC peripheral and
      configures data transfer according to parameters in LPBAM_DAC_FullAdvConf_t structure.
      The configuration parameters are :
          (+) DAC_Trigger : Specifies the DAC source trigger.
                            This parameter can be a value of @ref DAC_trigger_selection.
          (+) Alignment   : Specifies the DAC alignment.
                            This parameter can be a value of @ref LPBAM_DAC_DataAlignment
          (+) pData       : Specifies the source buffer of data .
          (+) Size        : Specifies the size of pData.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_DAC_Conversion_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.

    [..]
      Use ADV_LPBAM_DAC_Stop_SetFullQ() API to build a linked-list queue that stop the DAC peripheral.

    [..]
      These API must be called when the DAC is initialized.
           (+) Recommended DAC channel initialization sequence
               (++) Call HAL_DAC_Init() to initialize the DAC. (Mandatory)
               (++) Call HAL_DAC_ConfigChannel() to configure selected channel. (Mandatory)
                    Configuration parameters can be :
                    (+++) DAC_HighFrequency           : DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE. (Mandatory)
                    (+++) DAC_DMADoubleDataMode       : DISABLE.
                    (+++) DAC_SignedFormat            : DISABLE.
                    (+++) DAC_Trigger                 : DISABLE.
                    (+++) DAC_OutputBuffer            : DAC_OUTPUTBUFFER_DISABLE.
                    (+++) DAC_ConnectOnChipPeripheral : DAC_CHIPCONNECT_BOTH.
                    (+++) DAC_UserTrimming            : DAC_TRIMMING_USER.
                    (+++) DAC_TrimmingValue           : 16U.
                    (+++) DAC_SampleAndHold           : DAC_SAMPLEANDHOLD_ENABLE. (Mandatory value)
                    Sample and hold mode is mandatory in stop mode and its configuration parameters can be :
                    (+++) DAC_SampleTime  : 10U.
                    (+++) DAC_HoldTime    : 20U.
                    (+++) DAC_RefreshTime : 5U.
               (++) Call HAL_DACEx_SetConfigAutonomousMode() to enable the autonomous mode. (Mandatory)
                    Autonomous mode parameters can be :
                    (+++) AutonomousModeState : DAC_AUTONOMOUS_MODE_ENABLE. (Mandatory value)
               (++) Call __HAL_DAC_ENABLE_IT() to enable error interrupts.
                    DAC interrupts can be :
                    (+++) DAC_IT_DMAUDR1 : DMA underrun error for DAC channel 1.
                    (+++) DAC_IT_DMAUDR2 : DMA underrun error for DAC channel 2.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      This driver user sequence when need to use one of two channels separately is:
          (+) Initialize the DAC (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_DAC_EnableDMARequests() to enable DAC channel DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Single API making configuration and data setup :
                   (+++) Call, when needed, ADV_LPBAM_DAC_Conversion_SetFullQ(). (Mandatory)
              (++) Two APIs making configuration and data setup separately :
                   (+++) Call, when needed, ADV_LPBAM_DAC_Conversion_SetConfigQ() and/or
                         ADV_LPBAM_DAC_Conversion_SetDataQ(). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_DAC_Conversion_SetConfigQ(), ADV_LPBAM_DAC_Conversion_SetDataQ() or
              ADV_LPBAM_DAC_Conversion_SetFullQ() output queue.
          (+) Call, when needed, ADV_LPBAM_DAC_Stop_SetFullQ().
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)
          (+) Call, when needed, ADV_LPBAM_DAC_Stop_SetFullQ() to stop the DAC conversion.
          (+) Call, when needed, ADV_LPBAM_DAC_Stop_SetFullQ() to stop the DAC conversion.

    [..]
      This driver user sequence when need to use both channels simultanuously is:
          (+) Initialize the DAC (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_DAC_EnableDMARequests() to enable DAC channels DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_DAC_Conversion_SetDataQ() for DAC channel 2. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Single API making configuration for both channels and data channel 1 setup :
                   (+++) Call, when needed, ADV_LPBAM_DAC_Conversion_SetFullQ() with both channels as parameter.
                         (Mandatory)
              (++) Two APIs making configuration for both channels and data channel 1 setup separately :
                   (+++) Call, when needed, ADV_LPBAM_DAC_Conversion_SetConfigQ() with both channels as parameter and/or
                         ADV_LPBAM_DAC_Conversion_SetDataQ() with channel 1 as parameter. (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_DAC_Conversion_SetConfigQ(), ADV_LPBAM_DAC_Conversion_SetDataQ() or
              ADV_LPBAM_DAC_Conversion_SetFullQ() output DAC channel 2 queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for infinite scenarios cases DAC channel 2 queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_DAC_Conversion_SetConfigQ(), ADV_LPBAM_DAC_Conversion_SetDataQ() or
              ADV_LPBAM_DAC_Conversion_SetFullQ() output DAC channel 1 queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for infinite scenarios cases DAC chanel1 queue.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode for DAC channel 2 queue.
              (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output DAC channel 2 queue to the initialized DMA channel.
              (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list DAC channel 2 queue execution.
              (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode for DAC channel 1 queue.
              (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output DAC channel 1 queue to the initialized DMA channel.
              (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list DAC channel 1 queue execution.
              (Mandatory)
          (+) Call, when needed, ADV_LPBAM_DAC_Stop_SetFullQ() to stop the DAC conversion.

    *** Recommendations ***
    =======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      For memory optimization purpose, when a re-configuration is necessary, it's recommended to use
      ADV_LPBAM_DAC_Conversion_SetFullQ() instead of ADV_LPBAM_DAC_Conversion_SetConfigQ() then
      ADV_LPBAM_DAC_Conversion_SetDataQ().

    [..]
      For memory optimization purpose, when no re-configuration is necessary, it's recommended to use
      ADV_LPBAM_DAC_Conversion_SetDataQ().

    [..]
      It's forbidden to call ADV_LPBAM_DAC_Conversion_SetConfigQ() or ADV_LPBAM_DAC_Conversion_SetFullQ() on DAC
      channelx when DAC channelx or DAC channely is operating.

    [..]
      It's allowed to call ADV_LPBAM_DAC_Conversion_SetDataQ() by two different linked-list queues executed
      simultaneously for 2 different channel.

    [..]
      It's allowed to call ADV_LPBAM_DAC_Conversion_SetDataQ() and ADV_LPBAM_DAC_Conversion_SetConfigQ()
      or ADV_LPBAM_DAC_Conversion_SetDataQ() and ADV_LPBAM_DAC_Conversion_SetFullQ()
      by two different linked-list queues executed simultaneously for 2 different channel.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

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

#ifdef LPBAM_DAC_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup DAC_Advanced DAC_Advanced
  * @brief    DAC Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_DAC_Advanced_Exported_Functions LPBAM DAC Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to setup the configuration of DAC conversion according to parameters in
  *         LPBAM_DAC_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be one or combilation of @ref LPBAM_DAC_Channel values.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pConfig      : [IN]  Pointer to a LPBAM_DAC_ConfigAdvConf_t structure that contains conversion configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DAC_ConvConfigDesc_t structure that contains conversion
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetConfigQ(DAC_TypeDef                *const pInstance,
                                                   uint32_t                   Channel,
                                                   LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                   LPBAM_DAC_ConfigAdvConf_t  const *const pConfig,
                                                   LPBAM_DAC_ConvConfigDesc_t *const pDescriptor,
                                                   DMA_QListTypeDef           *const pQueue)
{
  LPBAM_DAC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t              channel_num = 1U;

  /*
   *               ######## Disable and configure DAC ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Check channel number */
  if (Channel > 2U)
  {
    channel_num = 2U;
    config_node.Config[0U].Channel = LPBAM_DAC_CHANNEL_1;
    config_node.Config[1U].Channel = LPBAM_DAC_CHANNEL_2;
  }
  else
  {
    config_node.Config[0U].Channel = Channel;
    config_node.Config[1U].Channel = 0U;
  }

  /* Set DAC channels configuration */
  for (uint8_t ch_idx = 0U; ch_idx < channel_num; ch_idx++)
  {
    config_node.Config[ch_idx].State = DISABLE;
  }

  /* Fill node configuration */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Enable DAC ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg  = &pDescriptor->pReg[1U];

  /* Set DAC channels configuration */
  for (uint8_t ch_idx = 0U; ch_idx < channel_num; ch_idx++)
  {
    config_node.Config[ch_idx].State       = ENABLE;
    config_node.Config[ch_idx].DAC_Trigger = pConfig[ch_idx].DAC_Trigger;
  }

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data DAC conversion according to parameters in
  *         LPBAM_DAC_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be a value of @ref LPBAM_DAC_Channel.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pData        : [IN]  Pointer to a LPBAM_DAC_DataAdvConf_t structure that contains conversion data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DAC_ConvDataDesc_t structure that contains conversion data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetDataQ(DAC_TypeDef              *const pInstance,
                                                 uint32_t                 Channel,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_DAC_DataAdvConf_t  const *const pData,
                                                 LPBAM_DAC_ConvDataDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue)
{
  LPBAM_DAC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;

  /*
   *               ######## DAC data node ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_DATA_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;

  /* Set DAC data configuration */
  config_node.Config->Channel             = Channel;
  config_node.Config->Alignment           = (uint8_t)pData->Alignment;
  config_node.NodeDesc.pBuff              = (uint32_t *)pData->pData;
  config_node.Config->Size                = pData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect data node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and the data DAC conversion according to parameters
  *         in LPBAM_DAC_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be one or combilation of @ref LPBAM_DAC_Channel values.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pFull        : [IN]  Pointer to a LPBAM_DAC_FullAdvConf_t structure that contains conversion full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DAC_ConvFullDesc_t structure that contains conversion full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetFullQ(DAC_TypeDef              *const pInstance,
                                                 uint32_t                 Channel,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_DAC_FullAdvConf_t  const *const pFull,
                                                 LPBAM_DAC_ConvFullDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue)
{
  LPBAM_DAC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t              channel_num = 1U;

  /*
   *               ######## Disable and configure DAC ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Check channel number */
  if (Channel > 2U)
  {
    channel_num = 2U;
    config_node.Config[0U].Channel = LPBAM_DAC_CHANNEL_1;
    config_node.Config[1U].Channel = LPBAM_DAC_CHANNEL_2;
  }
  else
  {
    config_node.Config[0U].Channel = Channel;
    config_node.Config[1U].Channel = 0U;
  }

  /* Set DAC channels configuration */
  for (uint8_t ch_idx = 0U; ch_idx < channel_num; ch_idx++)
  {
    config_node.Config[ch_idx].State = DISABLE;
  }

  /* Fill node configuration */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /*
   *               ######## Enable DAC ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg  = &pDescriptor->pReg[1U];

  /* Set DAC channels configuration */
  for (uint8_t ch_idx = 0U; ch_idx < channel_num; ch_idx++)
  {
    config_node.Config[ch_idx].State       = ENABLE;
    config_node.Config[ch_idx].DAC_Trigger = pFull[ch_idx].DAC_Trigger;
  }

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /*
   *               ######## DAC data node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DAC_DATA_ID;

  /* Set DAC data configuration */
  config_node.Config->Alignment         = (uint8_t)pFull->Alignment;
  config_node.NodeDesc.pBuff           = (uint32_t *)pFull->pData;
  config_node.Config->Size              = pFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build nodes */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[2U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect data node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[2U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to stop the DAC peripheral.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DAC_StopConvDesc_t structure that contains stop conversion data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Stop_SetFullQ(DAC_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_DAC_StopConvDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue)
{
  LPBAM_DAC_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;

  /*
   *               ######## DAC Stop node ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set DAC channels configuration */
  config_node.Config[0U].Channel = LPBAM_DAC_CHANNEL_1;
  config_node.Config[0U].State   = DISABLE;
  config_node.Config[1U].Channel = LPBAM_DAC_CHANNEL_2;
  config_node.Config[1U].State   = DISABLE;

  /* Fill node configuration */
  if (LPBAM_DAC_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build stop node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect stop node to DAC Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the DAC DMA requests.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be one or combilation of @ref LPBAM_DAC_Channel values.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_EnableDMARequests(DAC_TypeDef *const pInstance,
                                               uint32_t    Channel)
{
  /* Check channel index */
  if ((Channel & LPBAM_DAC_CHANNEL_1) == LPBAM_DAC_CHANNEL_1)
  {
    /* Enable DMA requests for DAC channel 1 */
    pInstance->CR |= LPBAM_DAC_CHANNEL1_DMAEN;

    /* Enable DAC channel 1 */
    pInstance->CR |= DAC_CR_EN1;
  }

  /* Check channel index */
  if ((Channel & LPBAM_DAC_CHANNEL_2) == LPBAM_DAC_CHANNEL_2)
  {
    /* Enable DMA requests for DAC channel 2 */
    pInstance->CR |= LPBAM_DAC_CHANNEL2_DMAEN;

    /* Enable DAC channel 2 */
    pInstance->CR |= DAC_CR_EN2;
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

#endif /* LPBAM_DAC_MODULE_ENABLED */
