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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This driver is dedicated for DMA that supports linked-list feature.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_dma.c
              (++) This file provides the DMA advanced files body.
          (+) stm32_adv_lpbam_dma.h
              (++) This file is the header file of stm32_adv_lpbam_dma.c. It provides advanced used types.
          (+) stm32_platform_lpbam_dma.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Links a built linked-list queue and starts a DMA channel execution in linked-list mode.

    *** Functional description ***
    ==============================
    [..]
      A master DMA channel can link a built linked-list queue and start a slave DMA channel execution in linked-list
      mode. At first step, the DMA master will update the slave DMA channel registers to point on the queue address
      (queue to be executed by the slave DMA).

      The output of this driver is the queue to be executed by the DMA master channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_DMA_Start_SetFullQ() API to build a linked-list queue (to be executed by master DMA channel) that
      links a built linked-list queue (to be executed by slave DMA channel) to a DMA channel and start execution
      according to configured parameters in LPBAM_DMA_StartFullAdvConf_t structure.
      Configured parameters are :
          (+) HeadQAddress : Specifies the head node address of queue to be executed.
          (+) WakeupIT     : Specifies the wake up source interrupt.
                             This parameter can be one or a combination of @ref LPBAM_DMA_Wakeup_Interrupt.

      This API must be called when the slave DMA channel is well initialization in linked-list mode.
          (+) Recommended DMA channel initialization sequence
              (++) Call HAL_DMAEx_List_Init() to initialize the DMA in linked-list mode.
              (++) Call __HAL_DMA_ENABLE_IT() to enable DMA channel error interrupts.
                   Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
                   DMA error interrupts can be :
                   (+++) DMA_IT_DTE : data transfer error.
                   (+++) DMA_IT_ULE : update link error.
                   (+++) DMA_IT_USE : user setting error.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Initialize the slave DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable DMA channel error interrupts.
          (+) Repeat calling ADV_LPBAM_DMA_Start_SetFullQ() until complete LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_DMA_Start_SetFullQ() output queue.
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios cases.
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Initialize the master DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_Link() to link the output queue to a master DMA channel.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution.

    *** Constraints ***
    ===================
    [..]
      Access to DMA slave channel must be done, only by one DMA channel master at the same time.
      When called to build a linked-list queue scenario, recalling ADV_LPBAM_DMA_Start_SetFullQ() in another queue shall
      be studied carefully. In fact, output queues that starts linked-list execution for the same slave DMA channel must
      not be executed simultaneously.

    [..]
      When calling ADV_LPBAM_DMA_Start_SetFullQ() API to start two different DMA channel but for the same queue, it is
      useless to execute them simultaneously as the two masters will access to the same memories to transmit the same
      data content.
      It's strongly not recommended to execute the same linked-list queue that contains nodes ensuring data transfer
      to/from a peripheral by two different slave DMA channels as unexpected behavior can appear.

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
  * @brief  Build DMA linked-list queue to link and start executing an existing queues in memory according to configured
  *         parameters in the LPBAM_DMA_StartFullAdvConf_t.
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
   *               ######## Set DMA configuration and enable channel ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_DMA_CONFIG_ID;

  /* Set node descriptor */
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[4U];

  /* Set DMA configuration */
  config_node.Config.State             = ENABLE;
  config_node.Config.WakeupIT          = pStartFull->WakeupIT;

  /* Fill node configuration */
  if (LPBAM_DMA_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[4U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to DMA Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[4U]) != HAL_OK)
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
