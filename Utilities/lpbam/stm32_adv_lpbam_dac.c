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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_dac.c
              (++) This file provides the DAC advanced files body.
          (+) stm32_adv_lpbam_dac.h
              (++) This file is the header file of stm32_adv_lpbam_dac.c. It provides advanced used types.
          (+) stm32_platform_lpbam_dac.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configures the DAC peripheral conversion.
          (+) Starts the DAC peripheral conversion.
          (+) Configures and starts the DAC peripheral conversion.

    *** Functional description ***
    ==============================
    [..]
      The DAC peripheral is configured and started through a DMA channel thanks to a build DMA linked-list queue.
      The DAC conversion hardware trigger signal is configurable by a DMA channel.
      The DAC input data is stored in a buffer then sent by a DMA channel to be converted in an analog output signal
      thanks to a built linked-list queue.

      The output of this driver is a queue to be executed by a DMA channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_DAC_Conversion_SetConfigQ() API to build a linked-list queue that configures the DAC
      peripheral according to parameters in the LPBAM_DAC_ConfigAdvConf_t structure.
      Configured parameters are :
          (+) DAC_Trigger : Specifies the DAC source trigger selection.
                            This parameter can be a value of @ref DAC_trigger_selection.

    [..]
      Use ADV_LPBAM_DAC_Conversion_SetDataQ() API to build a linked-list queue that configures data nodes
      according to parameters in the LPBAM_DAC_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configured parameters are :
          (+) Alignment : Specifies the DAC alignment.
                          This parameter can be a value of @ref LPBAM_DAC_DataAlignment.
          (+) pData     : Specifies the source buffer of data .
          (+) Size      : Specifies the size of pData.

    [..]
      Use ADV_LPBAM_DAC_Conversion_SetFullQ() API to build a linked-list queue that configures and starts the DAC and
      start execution according to parameters in LPBAM_DAC_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configured parameters are :
          (+) DAC_Trigger : Specifies the DAC source trigger selection.
                            This parameter can be a value of @ref DAC_trigger_selection.
          (+) Alignment   : Specifies the DAC alignment.
                            This parameter can be a value of @ref LPBAM_DAC_DataAlignment
          (+) pData       : Specifies the source buffer of data .
          (+) Size        : Specifies the size of pData.

      These API must be called when the DAC is well initialized.
           (+) Recommended DAC channel initialization sequence
               (++) Call HAL_DAC_Init() to initialize the DAC. (Mandatory)
               (++) Call HAL_DAC_ConfigChannel() to configure selected channel. (Mandatory)
                    Configuration parameters can be :
                    (+++) DAC_HighFrequency           : DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC.
                    (+++) DAC_DMADoubleDataMode       : DISABLE.
                    (+++) DAC_SignedFormat            : DISABLE.
                    (+++) DAC_Trigger                 : DISABLE.
                    (+++) DAC_OutputBuffer            : DAC_OUTPUTBUFFER_DISABLE.
                    (+++) DAC_ConnectOnChipPeripheral : DAC_CHIPCONNECT_BOTH.
                    (+++) DAC_UserTrimming            : DAC_TRIMMING_USER.
                    (+++) DAC_TrimmingValue           : 16U.
                    (+++) DAC_SampleAndHold           : DAC_SAMPLEANDHOLD_ENABLE. (Mandatory)
                    Sample and hold mode is mandatory in stop mode and its configuration parameters can be :
                    (+++) DAC_SampleTime  : 10U.
                    (+++) DAC_HoldTime    : 20U.
                    (+++) DAC_RefreshTime : 5U.
               (++) Call HAL_DACEx_SetConfigAutonomousMode() to enable the autonomous mode. (Mandatory)
                    Autonomous mode parameters can be :
                    (+++) AutonomousModeState : DAC_AUTONOMOUS_MODE_ENABLE. (Mandatory)
               (++) Call __HAL_DAC_ENABLE_IT() to enable error interrupts.
                    DAC interrupts can be :
                    (+++) DAC_IT_DMAUDR1 : DMA underrun error for DAC channel 1.
                    (+++) DAC_IT_DMAUDR2 : DMA underrun error for DAC channel 2.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Initialize the DAC (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_DAC_EnableDMARequests() to enable DAC DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Conversion with full API :
                   (+++) Repeat calling ADV_LPBAM_DAC_Conversion_SetFullQ() until complete LPBAM scenario. (Mandatory)
              (++) Conversion with configuration and data APIs :
                   (+++) Repeat calling ADV_LPBAM_DAC_Conversion_SetConfigQ() and ADV_LPBAM_DAC_Conversion_SetDataQ()
                         until complete LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_DAC_Conversion_SetConfigQ(), ADV_LPBAM_DAC_Conversion_SetDataQ() or
              ADV_LPBAM_DAC_Conversion_SetFullQ() output queue. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue
              for infinite scenarios cases. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    *** Constraints ***
    ===================
    [..]
      It's strongly not recommended to call ADV_LPBAM_DAC_Conversion_SetConfigQ() consecutively.

    [..]
      It's strongly recommended to use ADV_LPBAM_DAC_Conversion_SetFullQ() instead of
      ADV_LPBAM_DAC_Conversion_SetConfigQ() then ADV_LPBAM_DAC_Conversion_SetDataQ() for memory optimization purpose.

    [..]
      It's useless to call ADV_LPBAM_DAC_Conversion_SetConfigQ() before ADV_LPBAM_DAC_Conversion_SetFullQ() as full API
      contains configuration part.

    [..]
      When current DAC configuration matches with application needs, it's strongly recommended to use
      ADV_LPBAM_DAC_Conversion_SetDataQ() for memory optimization purpose.

    [..]
      It's not allowed to use the 2 DAC channels in LPBAM mode simultaneously. Ensure that only one DAC channel is
      running each time otherwise unexpected behavior will appear.

    [..]
      When using the 2 DAC channel to run simultaneously, ensure that the two channels are configured before calling
      LPBAM DAC API.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains DAC configuration and data
      nodes by two different DMA channels as unexpected behavior can appear.

    [..]
      It's strongly not recommended to call the DAC advanced APIs listed belong with the same instance by more than one
      linked-list queue. When the DAC nodes will be executed simultaneously unexpected behavior will appear.

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
  * @brief  Build the DAC conversion configuration nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_DAC_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be a value of @ref LPBAM_DAC_Channel.
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

  /*
   *               ######## Disable and configure DAC ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set DAC configuration */
  config_node.Config.State               = DISABLE;
  config_node.Config.Channel             = Channel;
  config_node.Config.DAC_Trigger         = pConfig->DAC_Trigger;

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

  /* Set DAC configuration */
  config_node.Config.State         = ENABLE;

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
  * @brief  Build the DAC conversion data nodes in DMA linked-list queue according to configured parameters in
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
  config_node.Config.Channel             = Channel;
  config_node.Config.Alignment           = (uint8_t)pData->Alignment;
  config_node.NodeDesc.pBuff             = (uint32_t *)pData->pData;
  config_node.Config.Size                = pData->Size;

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
  * @brief  Build the DAC conversion full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_DAC_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be a value of @ref LPBAM_DAC_Channel.
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

  /*
   *               ######## Disable and configure DAC ########
   */

  /* Set DAC instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DAC_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set DAC configuration */
  config_node.Config.State               = DISABLE;
  config_node.Config.Channel             = Channel;
  config_node.Config.DAC_Trigger         = pFull->DAC_Trigger;

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

  /* Set DAC configuration */
  config_node.Config.State         = ENABLE;

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
  config_node.Config.Alignment         = (uint8_t)pFull->Alignment;
  config_node.NodeDesc.pBuff           = (uint32_t *)pFull->pData;
  config_node.Config.Size              = pFull->Size;

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
  * @brief  Enable the DAC DMA requests.
  * @param  pInstance    : [IN]  Pointer to a DAC_TypeDef structure that selects DAC instance.
  * @param  Channel      : [IN]  Specifies the DAC channel.
  *                              This parameter can be a value of @ref LPBAM_DAC_Channel.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DAC_EnableDMARequests(DAC_TypeDef *const pInstance,
                                               uint32_t    Channel)
{
  /* Check channel index */
  if (Channel == LPBAM_DAC_CHANNEL_1)
  {
    /* Enable DMA requests for DAC channel 1 */
    pInstance->CR |= LPBAM_DAC_CHANNEL1_DMAEN;

    /* Enable DAC channel 1 */
    pInstance->CR |= DAC_CR_EN1;
  }
  else
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
