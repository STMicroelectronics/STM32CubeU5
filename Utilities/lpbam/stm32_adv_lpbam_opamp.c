/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_opamp.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for OPAMP peripheral.
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
          (+) stm32_adv_lpbam_opamp.c file
              (++) This file provides the implementation of the advanced LPBAM OPAMP functions.
          (+) stm32_adv_lpbam_opamp.h file
              (++) This file is the header file of stm32_adv_lpbam_opamp.c. It provides advanced LPBAM OPAMP functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_opamp.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 3 model of APIs :
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following OPAMP features :
          (+) Configure and start the opamp peripheral.
          (+) Stop the opamp peripheral.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by a DMA channel in applicative side.

      The opamp peripheral is configured and started.
      The opamp non inverting input, the inverting input, the mode and the gain can be configured.

      The opamp peripheral is stopped.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_OPAMP_Start_SetFullQ() API to build a linked-list queue that configures and starts the opamp
      according to parameters in LPBAM_OPAMP_StartFullAdvConf_t structure.
      The configuration parameters are :
          (+) NonInvertingInput : Specifies the opamp non inverting input.
          (+) InvertingInput    : Specifies the opamp inverting input.
          (+) Mode              : Specifies the opamp mode.
          (+) PgaGain           : Specifies the opamp gain.

      This API must be called when the opamp is initialized.
          (+) Recommended opamp initialization sequence
              (++) call HAL_OPAMP_Init to initialize the OPAMP. (Mandatory)
                   Initialization parameters can be :
                   (+++) PowerMode         : OPAMP_POWERMODE_LOWPOWER_HIGHSPEED.
                   (+++) UserTrimming      : OPAMP_TRIMMING_FACTORY.
                   (+++) Mode              : OPAMP_FOLLOWER_MODE.
                   (+++) NonInvertingInput : OPAMP_NONINVERTINGINPUT_IO0.
                   (+++) InvertingInput    : OPAMP_INVERTINGINPUT_IO0.
                   (+++) PgaGain           : OPAMP_PGA_GAIN_2.
                   (+++) PowerSupplyRange  : OPAMP_POWERSUPPLY_HIGH.

    [..]
      Use ADV_LPBAM_OPAMP_Stop_SetFullQ() API to build a linked-list queue that stop the opamp peripheral.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      This driver user sequence is :
          (+) Initialize all used GPIOs for NonInvertingInput, InvertingInput. (Mandatory)
          (+) Initialize all used GPIOs for opamp Output. (Optional)
          (+) Initialize the opamp (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_OPAMP_Start_SetFullQ() to configure and start the OPAMP peripheral. (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_OPAMP_Start_SetFullQ() output queue.
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
          (+) Call, when needed, ADV_LPBAM_OPAMP_Stop_SetFullQ() to stop the OPAMP peripheral.

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      It's strongly recommended to ensure that InvertingInput and NonInvertingInput signals are configured when calling
      ADV_LPBAM_OPAMP_Start_SetFullQ().
          (+) Signal can be a DAC out.
          (+) Signal can be a GPIO.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same OPAMP instance.

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

#ifdef LPBAM_OPAMP_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup OPAMP_Advanced OPAMP_Advanced
  * @brief    OPAMP Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_OPAMP_Advanced_Exported_Functions LPBAM OPAMP Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to start OPAMP according to parameters in the LPBAM_OPAMP_StartFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a OPAMP_TypeDef structure that selects OPAMP instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pStartFull   : [IN]  Pointer to a LPBAM_OPAMP_StartFullAdvConf_t structure that contains OPAMP start
  *                              configuration.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_OPAMP_StartFullDesc_t structure that contains descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP_TypeDef                  *const pInstance,
                                              LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                              LPBAM_OPAMP_StartFullAdvConf_t const *const pStartFull,
                                              LPBAM_OPAMP_StartFullDesc_t    *const pDescriptor,
                                              DMA_QListTypeDef               *const pQueue)
{
  LPBAM_OPAMP_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  /*
   *               ######## OPAMP configuration node ########
   */

  /* Set OPAMP instance */
  config_node.Instance                    = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID     = (uint32_t)LPBAM_OPAMP_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType   = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg          = &pDescriptor->pReg[0U];

  /* Set OPAMP configuration */
  config_node.Config.State             = ENABLE;
  config_node.Config.Mode              = pStartFull->Mode;
  config_node.Config.PgaGain           = pStartFull->PgaGain;
  config_node.Config.InvertingInput    = pStartFull->InvertingInput;
  config_node.Config.NonInvertingInput = pStartFull->NonInvertingInput;

  /* Fill node configuration */
  if (LPBAM_OPAMP_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to OPAMP Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to stop the OPAMP peripheral.
  * @param  pInstance    : [IN]  Pointer to a OPAMP_TypeDef structure that selects OPAMP instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_OPAMP_StopFullDesc_t structure that contains descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_OPAMP_Stop_SetFullQ(OPAMP_TypeDef              *const pInstance,
                                             LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                             LPBAM_OPAMP_StopFullDesc_t *const pDescriptor,
                                             DMA_QListTypeDef           *const pQueue)
{
  LPBAM_OPAMP_ConfNode_t config_node;
  DMA_NodeConfTypeDef    dma_node_conf;

  /*
   *               ######## OPAMP Stop node ########
   */

  /* Set OPAMP instance */
  config_node.Instance                   = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_OPAMP_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set OPAMP configuration */
  config_node.Config.State               = DISABLE;
  config_node.Config.Mode                = LPBAM_OPAMP_STANDALONE_MODE;
  config_node.Config.PgaGain             = LPBAM_OPAMP_PGA_GAIN_2;
  config_node.Config.InvertingInput      = LPBAM_OPAMP_INVERTINGINPUT_IO0;
  config_node.Config.NonInvertingInput   = LPBAM_OPAMP_NONINVERTINGINPUT_IO0;

  /* Fill node configuration */
  if (LPBAM_OPAMP_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect stop node to OPAMP Queue */
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

#endif /* LPBAM_OPAMP_MODULE_ENABLED */
