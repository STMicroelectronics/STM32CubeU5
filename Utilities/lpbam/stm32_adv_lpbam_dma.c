/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_dma.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for DMA peripheral.
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
          (+) stm32_adv_lpbam_dma.c file
              (++) This file provides the implementation of the advanced LPBAM DMA functions.
          (+) stm32_adv_lpbam_dma.h file
              (++) This file is the header file of stm32_adv_lpbam_dma.c. It provides advanced LPBAM DMA functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_dma.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 1 model of API :
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following DMA features :
          (+) Links a built linked-list queue and starts a slave DMA channel execution in linked-list mode.
          (+) Stops a slave DMA channel execution.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by the DMA master channel.

      A master DMA channel can link a built linked-list queue and start a slave DMA channel execution in linked-list
      mode. At first step, the DMA master will update the slave DMA channel registers to point on the queue address
      (queue to be executed by the slave DMA). Then, the DMA master will start the slave DMA channel in linked-list
      mode.

      A master DMA channel can stop a slave DMA channel execution.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_DMA_Start_SetFullQ() API to build a linked-list queue (to be executed by a master DMA channel) that
      links a built linked-list queue to a slave DMA channel and start execution according to configured parameters in
      LPBAM_DMA_StartFullAdvConf_t structure.
      The configuration parameters are :
          (+) HeadQAddress : Specifies the head node address of queue to be executed.
          (+) WakeupIT     : Specifies the wake up source interrupt.
                             This parameter can be one or a combination of @ref LPBAM_DMA_Wakeup_Interrupt.
      Example : HeadQAddress = (uint32_t)User_Q.Head where User_Q is a variable of type DMA_QListTypeDef.

      This API must be called when the slave DMA channel is initialization in linked-list mode.
          (+) Recommended slave DMA channel initialization sequence
              (++) Call HAL_DMAEx_List_Init() to initialize the slave DMA in linked-list mode.
              (++) Call __HAL_DMA_ENABLE_IT() to enable slave DMA channel error interrupts.
                   Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
                   DMA error interrupts can be :
                   (+++) DMA_IT_DTE : data transfer error.
                   (+++) DMA_IT_ULE : update link error.
                   (+++) DMA_IT_USE : user setting error.

    [..]
      Use ADV_LPBAM_DMA_Stop_SetFullQ() API to build a linked-list queue that stop a slave DMA channel.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      This driver user sequence is :
          (+) Initialize the slave DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable slave DMA channel error interrupts.
          (+) Call ADV_LPBAM_DMA_Start_SetFullQ() to link queue and start a DMA channel in linked-list mode. (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_DMA_Start_SetFullQ() output queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list master queue for infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a master DMA channel in linked-list mode. (Mandatory)
              (++) Master DMA channel must be configured as privileged. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized master DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the master DMA channel linked-list execution. (Mandatory)
          (+) Call, when needed, ADV_LPBAM_DMA_Stop_SetFullQ() to stop a slave DMA channel.

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      It's recommended to ensure the consistency of used master DMA channel and linked-list queues when using
      ADV_LPBAM_DMA_Start_SetFullQ() API at run time.

    [..]
      Ensure the consistency between slave queue circularity mode to be linked and the slave DMA channel linked-list
      circularity configuration.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different slave DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same slave DMA channel.

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

#ifdef LPBAM_DMA_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup DMA_Advanced DMA_Advanced
  * @brief    DMA Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_DMA_Advanced_Exported_Functions LPBAM DMA Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to link and start executing an existing queues in memory according to parameters
  *         in the LPBAM_DMA_StartFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a DMA_Channel_TypeDef structure that selects DMA channel instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pStartFull   : [IN]  Pointer to a LPBAM_DMA_StartFullAdvConf_t structure that contains DMA start full
  *                              configuration.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DMA_StartFullDesc_t structure that contains DMA start descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DMA_Start_SetFullQ(DMA_Channel_TypeDef          *const pInstance,
                                            LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                            LPBAM_DMA_StartFullAdvConf_t const *const pStartFull,
                                            LPBAM_DMA_StartFullDesc_t    *const pDescriptor,
                                            DMA_QListTypeDef             *const pQueue)
{
  LPBAM_DMA_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;

  /*
   *               ######## Disable DMA channel ########
   */

  /* Set DMA instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DMA_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set DMA configuration */
  config_node.Config.State               = DISABLE;

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable DMA channel node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect disable DMA channel to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Clear DMA channel flags ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[1U];

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear DMA channel flags node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect clear DMA channel flags node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[1U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Set queue base address ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_QUEUE_BASEADDRESS_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[2U];

  /* Set DMA configuration */
  config_node.Config.HeadQAddress      = pStartFull->HeadQAddress;

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set queue base address node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[2U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect set queue base address node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[2U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Set queue offset address ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_QUEUE_OFFSETADDRESS_ID;

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[3U];

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set queue offset address node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[3U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect set queue offset address node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[3U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Clear DMA data size ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_CLEARDATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[4U];

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear DMA channel flags node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[4U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect clear DMA channel flags node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[4U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## Set DMA configuration and enable channel ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_CONFIG_ID;

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[5U];

  /* Set DMA configuration */
  config_node.Config.State             = ENABLE;
  config_node.Config.WakeupIT          = pStartFull->WakeupIT;

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[5U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[5U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}


/**
  * @brief  Build DMA linked-list queue to stop executing linked queue to a DMA channel.
  * @param  pInstance    : [IN]  Pointer to a DMA_Channel_TypeDef structure that selects DMA channel instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_DMA_StopFullDesc_t structure that contains DMA stop descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_DMA_Stop_SetFullQ(DMA_Channel_TypeDef      *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_DMA_StopFullDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue)
{
  LPBAM_DMA_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;

  /*
   *               ######## DMA stop node ########
   */

  /* Set DMA instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_DMA_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set DMA configuration */
  config_node.Config.State               = DISABLE;

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect disable node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
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

#endif /* LPBAM_DMA_MODULE_ENABLED */
