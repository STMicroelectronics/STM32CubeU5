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
      It is strongly recommended to read carefully the LPBAM_Utility_GettingStarted.html document before starting
      developing an LPBAM application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_vrefbuf.c
              (++) This file provides the VREFBUF advanced files body.
          (+) stm32_adv_lpbam_vrefbuf.h
              (++) This file is the header file of stm32_adv_lpbam_vrefbuf.c. It provides advanced used types.
          (+) stm32_platform_lpbam_vrefbuf.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure the VREFBUF peripheral.

    *** Functional description ***
    ==============================
    [..]
      The VREFBUF peripheral mode is configured through a DMA channel thanks to a built DMA linked-list queue.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() API to build a linked-list queue that configures the VREFBUF and
      start execution according to parameters in LPBAM_VREFBUF_BufferModeFullAdvConf_t structure.
      Configured parameters are :
          (+) BufferMode : Specifies the VREFBUF buffer mode.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Repeat calling ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() until complete LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() output queue. (Optional)
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

    *** Recommendation ***
    ======================
    [..]
      It's strongly not recommended to call ADV_LPBAM_VREFBUF_BufferMode_SetFullQ() with the same instance by more than
      one linked-list queue. When the VREFBUF nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's strongly not recommended, to execute the same linked-list queue that contains VREFBUF configuration nodes
      by two different DMA channels as unexpected behavior can appear.

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
  * @brief  Build the VREFBUF buffer mode full nodes in DMA linked-list queue according to configured parameters in
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
