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
      It is strongly recommended to read carefully the LPBAM_Utility_GettingStarted.html document before starting
      developing an LPBAM application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_opamp.c
              (++) This file provides the opamp advanced files body.
          (+) stm32_adv_lpbam_opamp.h
              (++) This file is the header file of stm32_adv_lpbam_opamp.c. It provides advanced used types.
          (+) stm32_platform_lpbam_opamp.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure and start the opamp peripheral.

    *** Functional description ***
    ==============================
    [..]
      The opamp peripheral is configured and started through a DMA channel thanks to a build DMA linked-list queue.
      The opamp non inverting input, the inverting input, the mode and the gain can be configured by a DMA channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_OPAMP_Start_SetFullQ() API to build a linked-list queue that configures and starts the opamp and
      start execution according to parameters in LPBAM_OPAMP_StartFullAdvConf_t structure.
      Configuration parameters are :
          (+) NonInvertingInput : Specifies the opamp non inverting input.
          (+) InvertingInput    : Specifies the opamp inverting input.
          (+) Mode              : Specifies the opamp mode.
          (+) PgaGain           : Specifies the opamp gain.

      This API must be called when the opamp is well initialized.
          (+) Recommended opamp initialization sequence
              (++) call HAL_OPAMP_Init to initialize the OPAMP. (Mandatory)
                   Initialization parameters can be :
                   (+++) PowerMode         : OPAMP_POWERMODE_LOWPOWER_HIGHSPEED.
                   (+++) UserTrimming      : OPAMP_TRIMMING_FACTORY.
                   (+++) Mode              : OPAMP_FOLLOWER_MODE.
                   (+++) NonInvertingInput : OPAMP_NONINVERTINGINPUT_IO0.
                   (+++) InvertingInput    : OPAMP_INVERTINGINPUT_IO0.
                   (+++) PgaGain           : OPAMP_PGA_GAIN_2.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Initialize the opamp (Using HAL/LL). (Mandatory)
          (+) Repeat calling ADV_LPBAM_OPAMP_Start_SetFullQ() until complete LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_OPAMP_Start_SetFullQ() output queue. (Optional)
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
      It's strongly not recommended to call ADV_LPBAM_OPAMP_Start_SetFullQ() with the same instance by more than one
      linked-list queue. When the opamp nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's strongly not recommended, to execute the same linked-list queue that contains OPAMP configuration nodes
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
  * @brief  Build DMA linked-list queue to start OPAMP according to configured parameters in the
  *         LPBAM_OPAMP_StartFullAdvConf_t.
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
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_OPAMP_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
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
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* LPBAM_OPAMP_MODULE_ENABLED */
