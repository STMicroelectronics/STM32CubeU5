/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_vrefbuf.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for VREFBUF peripheral.
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
          (+) stm32_adv_lpbam_vrefbuf.c file
              (++) This file provides the implementation of the advanced LPBAM VREFBUF functions.
          (+) stm32_adv_lpbam_vrefbuf.h file
              (++) This file is the header file of stm32_adv_lpbam_vrefbuf.c. It provides advanced LPBAM VREFBUF
                   functions prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_vrefbuf.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides only one model of API :
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following VREFBUF features :
          (+) Configure the VREFBUF peripheral buffer mode.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by a DMA channel in applicative side.

      The VREFBUF peripheral mode is configured.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() API to build a linked-list queue that configures the VREFBUF and
      start execution according to parameters in LPBAM_VREFBUF_BufferModeFullAdvConf_t structure.
      The configuration parameters are :
          (+) BufferMode : Specifies the VREFBUF buffer mode.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      This driver user sequence is :
          (+) Initialize used GPIO when configuring VREFBUF mode as internal voltage reference. (Mandatory)
          (+) Configure the VREFBUF peripheral (Using HAL/LL). (Optional)
          (+) Call, when needed, ADV_LPBAM_VREFBUF_BufferMode_SetFullQ(). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() output queue.
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

    *** Recommendation ***
    ======================
    [..]
      It's mandatory to configure the VREFBUF trimming and the internal voltage reference scale using (HAL/LL) when :
          (+) The VREFBUF peripheral is configured in internal voltage mode using HAL or LL.
          (+) The VREFBUF peripheral is configured in internal voltage mode in LPBAM scenario using
              ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() API.

    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      It's mandatory to configure the VREFBUF mode as "internal voltage reference" using HAL/LL, when setting BufferMode
      parameter in LPBAM_VREFBUF_BufferModeFullAdvConf_t structure as LPBAM_VREFBUF_INTERNAL_VOLTAGE_MODE.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same VREFBUF instance.

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

#ifdef LPBAM_VREFBUF_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup VREFBUF_Advanced VREFBUF_Advanced
  * @brief    VREFBUF Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_VREFBUF_Advanced_Exported_Functions LPBAM VREFBUF Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to setup the VREFBUF buffer mode according to parameters in
  *         LPBAM_VREFBUF_BufferModeFullAdvConf_t.
  * @param  pInstance       : [IN]  Pointer to a VREFBUF_TypeDef structure that selects VREFBUF instance.
  * @param  pDMAListInfo    : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance
  *                                 and linked-list queue type information.
  * @param  pBufferModeFull : [IN]  Pointer to a LPBAM_VREFBUF_BufferModeFullAdvConf_t structure that contains mode
  *                                 configuration nodes information.
  * @param  pDescriptor     : [IN]  Pointer to a LPBAM_VREFBUF_BufferModeFullDesc_t structure that contains
  *                                 configuration buffer mode descriptor information.
  * @param  pQueue          : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                 information.
  * @retval LPBAM Status    : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_VREFBUF_BufferMode_SetFullQ(VREFBUF_TypeDef                       *const pInstance,
                                                     LPBAM_DMAListInfo_t                   const *const pDMAListInfo,
                                                     LPBAM_VREFBUF_BufferModeFullAdvConf_t const *const pBufferModeFull,
                                                     LPBAM_VREFBUF_BufferModeFullDesc_t    *const pDescriptor,
                                                     DMA_QListTypeDef                      *const pQueue)
{
  LPBAM_VREFBUF_ConfNode_t config_node;
  DMA_NodeConfTypeDef      dma_node_conf;

  /*
   *               ######## VREFBUF configuration node ########
   */

  /* Set VREFBUF instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_VREFBUF_MODE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set VREFBUF configuration */
  config_node.Config.BufferMode          = pBufferModeFull->BufferMode;

  /* Fill node configuration */
  if (LPBAM_VREFBUF_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to VREFBUF Queue */
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

#endif /* LPBAM_VREFBUF_MODULE_ENABLED */
