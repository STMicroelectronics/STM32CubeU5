/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_comp.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for COMP peripheral.
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
      It is composed of 3 files :
          (+) stm32_adv_lpbam_comp.c file
              (++) This file provides the implementation of the advanced LPBAM COMP functions.
          (+) stm32_adv_lpbam_comp.h file
              (++) This file is the header file of stm32_adv_lpbam_comp.c. It provides advanced LPBAM COMP functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_comp.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides only one model of API :
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration and one data transfer.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following COMP features :
          (+) Configure and start the comparator peripheral.
          (+) Stop the comparator peripheral.
          (+) Read the comparator output comparison value.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The comparator output can be read and stored in a buffer thanks to a built DMA linked-list queue.

      The comparator peripheral is configured and started.
      Only the comparator polarity, the input minus and the input plus can be configured.

      The comparator peripheral is stopped.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_COMP_Start_SetFullQ() API to build a linked-list queue that configures and starts the comparator
      peripheral according to parameters in the LPBAM_COMP_StartFullAdvConf_t structure.
      The configuration parameters are :
          (+) OutputPol  : Specifies the comparator output polarity.
          (+) InputPlus  : Specifies the comparator input plus.
          (+) InputMinus : Specifies the comparator input minus.

    [..]
      Use ADV_LPBAM_COMP_Stop_SetFullQ() API to build a linked-list queue that stop the comparator peripheral.

    [..]
      Use ADV_LPBAM_COMP_OutputLevel_SetFullQ() API to build a linked-list queue to get the comparator output level
      according to configured parameters in the LPBAM_COMP_OutLevelFullAdvConf_t structure.
      The configuration parameters are :
          (+) pData : Specifies the buffer address to store data.
          (+) Size  : Specifies the number of samples to be read.
      The data node default configuration is as follow:
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_COMP_OutputLevel_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
      To extract the output level value, it's mandatory to use __LPBAM_COMP_GET_OUTPUTLEVEL() macro to get the state of
      the comparator output for one data read.

      These APIs must be called when the comparator is initialized.
          (+) Recommended comparator initialization sequence is :
              (++) call HAL_COMP_Init to initialize the comparator. (Mandatory)
                   Initialization parameters can be :
                   (+++) InputMinus   : COMP_INPUT_MINUS_VREFINT.
                   (+++) InputPlus    : COMP_INPUT_PLUS_IO1.
                   (+++) OutputPol    : COMP_OUTPUTPOL_NONINVERTED.
                   (+++) Mode         : COMP_POWERMODE_HIGHSPEED.
                   (+++) Hysteresis   : COMP_HYSTERESIS_NONE.
                   (+++) BlankingSrce : COMP_BLANKINGSRC_NONE.
                   (+++) WindowMode   : COMP_WINDOWMODE_DISABLE.
                   (+++) WindowOutput : COMP_WINDOWOUTPUT_EACH_COMP.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      This driver user sequence is :
          (+) Initialize all used GPIOs for InputPlus, InputMinus. (Mandatory)
          (+) Initialize used GPIO for Output. (Optional)
          (+) Initialize the comparator (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_COMP_Start_SetFullQ() to configure and start the comparator instance. (Mandatory)
          (+) Repeat calling ADV_LPBAM_COMP_Start_SetFullQ() or/and ADV_LPBAM_COMP_OutputLevel_SetFullQ().
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing of
              ADV_LPBAM_COMP_Start_SetFullQ() or ADV_LPBAM_COMP_OutputLevel_SetFullQ() output queue. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue
              for infinite scenarios cases. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)
          (+) Call, when needed, ADV_LPBAM_COMP_Stop_SetFullQ() to stop the comparator instance.

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      When the comparator is configured on input window mode, the InputPlus parameter configuration has no effect.

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

#ifdef LPBAM_COMP_MODULE_ENABLED

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup COMP_Advanced COMP_Advanced
  * @brief    COMP Advanced LPBAM module driver
  * @{
  */

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMP_Advanced_Exported_Functions LPBAM COMP Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to start comparator according to parameters in LPBAM_COMP_StartFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a COMP_TypeDef structure that selects COMP instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pStartFull   : [IN]  Pointer to a LPBAM_COMP_StartFullAdvConf_t structure that contains comparator start
  *                              configuration.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_COMP_StartFullDesc_t structure that contains descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_COMP_Start_SetFullQ(COMP_TypeDef                  *const pInstance,
                                             LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                             LPBAM_COMP_StartFullAdvConf_t const *const pStartFull,
                                             LPBAM_COMP_StartFullDesc_t    *const pDescriptor,
                                             DMA_QListTypeDef              *const pQueue)
{
  LPBAM_COMP_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## COMP configuration node ########
   */

  /* Set COMP instance */
  config_node.Instance = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_COMP_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set COMP configuration */
  config_node.Config.State      = ENABLE;
  config_node.Config.OutputPol  = pStartFull->OutputPol;
  config_node.Config.InputPlus  = pStartFull->InputPlus;
  config_node.Config.InputMinus = pStartFull->InputMinus;

  /* Fill node configuration */
  if (LPBAM_COMP_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to COMP Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to stop comparator.
  * @param  pInstance    : [IN]  Pointer to a COMP_TypeDef structure that selects COMP instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_COMP_StopFullDesc_t structure that contains descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_COMP_Stop_SetFullQ(COMP_TypeDef              *const pInstance,
                                            LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                            LPBAM_COMP_StopFullDesc_t *const pDescriptor,
                                            DMA_QListTypeDef          *const pQueue)
{
  LPBAM_COMP_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## COMP Stop node ########
   */

  /* Set COMP instance */
  config_node.Instance                   = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_COMP_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set COMP configuration */
  config_node.Config.State               = DISABLE;
  config_node.Config.OutputPol           = LPBAM_COMP_OUTPUTPOL_NONINVERTED;
  config_node.Config.InputPlus           = LPBAM_COMP_INPUT_PLUS_IO1;
  config_node.Config.InputMinus          = LPBAM_COMP_INPUT_MINUS_1_4VREFINT;

  /* Fill node configuration */
  if (LPBAM_COMP_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect stop node to COMP Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to get the output level (high or low) according to parameters in the
  *         LPBAM_COMP_OutLevelFullAdvConf_t.
  * @param  pInstance     : [IN]  Pointer to a COMP_TypeDef structure that selects COMP instance.
  * @param  pDMAListInfo  : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance
  *                               and linked-list queue type information.
  * @param  pOutLevelFull : [IN]  Pointer to a LPBAM_COMP_OutLevelFullAdvConf_t structure that contains output
  *                               configuration.
  * @param  pDescriptor   : [IN]  Pointer to a LPBAM_COMP_OutLevelFullDesc_t structure that contains descriptor
  *                               information.
  * @param  pQueue        : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                               information.
  * @retval LPBAM Status  : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP_TypeDef                     *const pInstance,
                                                   LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                   LPBAM_COMP_OutLevelFullAdvConf_t const *const pOutLevelFull,
                                                   LPBAM_COMP_OutLevelFullDesc_t    *const pDescriptor,
                                                   DMA_QListTypeDef                 *const pQueue)
{
  LPBAM_COMP_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## COMP Read Output level node ########
   */

  /* Set COMP instance */
  config_node.Instance = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_COMP_READOUTPUT_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = pOutLevelFull->pData;

  /* Set COMP configuration */
  config_node.Config.Size = pOutLevelFull->Size;

  /* Fill node configuration */
  if (LPBAM_COMP_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to COMP Queue */
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

#endif /* LPBAM_COMP_MODULE_ENABLED */
