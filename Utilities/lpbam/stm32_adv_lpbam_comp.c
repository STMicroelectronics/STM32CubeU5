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
      It is strongly recommended to read carefully the LPBAM_Utility_GettingStarted.html document before starting
      developing an LPBAM application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_comp.c
              (++) This file provides the comp advanced files body.
          (+) stm32_adv_lpbam_comp.h
              (++) This file is the header file of stm32_adv_lpbam_comp.c. It provides advanced used types.
          (+) stm32_platform_lpbam_comp.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure and start the comparator peripheral.
          (+) Read the comparator output comparison value.

    *** Functional description ***
    ==============================
    [..]
      The comparator peripheral is configured and started through a DMA channel thanks to a built DMA linked-list queue.
      The comparator polarity, the input minus and the input plus can be configured by a DMA channel.
      The comparator output can be read and stored in a buffer thanks to a built DMA linked-list queue.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_COMP_Start_SetFullQ() API to build a linked-list queue that configures and starts the comparator
      peripheral according to parameters in the LPBAM_COMP_StartFullAdvConf_t structure.
      Configuration parameters are :
          (+) OutputPol  : Specifies the comparator output polarity.
          (+) InputPlus  : Specifies the comparator input plus.
          (+) InputMinus : Specifies the comparator input minus.

    [..]
      Use ADV_LPBAM_COMP_OutputLevel_SetFullQ() API to build a linked-list queue which is linked to a DMA channel to get
      the comparator output level (high or low) according to configured parameters in the
      LPBAM_COMP_OutLevelFullAdvConf_t.
      Configuration parameters are :
          (+) pData : Specifies the buffer address to store data.
          (+) Size  : Specifies the number of samples to be read.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      These APIs must be called when the comparator is well initialized.
          (+) Recommended comparator initialization sequence
              (++) call HAL_COMP_Init to initialize the comparator. (Mandatory)
                   Initialization parameters can be :
                   (+++) InputMinus   : COMP_INPUT_MINUS_VREFINT.
                   (+++) InputPlus    : COMP_INPUT_PLUS_IO1.
                   (+++) OutputPol    : COMP_OUTPUTPOL_NONINVERTED.
                   (+++) Mode         : COMP_POWERMODE_HIGHSPEED.
                   (+++) Hysteresis   : COMP_HYSTERESIS_NONE.
                   (+++) BlankingSrce : COMP_BLANKINGSRC_NONE.
                   (+++) WindowMode   : COMP_WINDOWMODE_DISABLE.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Initialize the comparator (Using HAL/LL). (Mandatory)
          (+) Repeat calling ADV_LPBAM_COMP_Start_SetFullQ() or/and ADV_LPBAM_COMP_OutputLevel_SetFullQ() until complete
              LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              ADV_LPBAM_COMP_Start_SetFullQ() or ADV_LPBAM_COMP_OutputLevel_SetFullQ() output queue. (Optional)
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
      It's strongly not recommended to call ADV_LPBAM_COMP_Start_SetFullQ() with the same instance by more than one
      linked-list queue. When the comparator nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains comparator configuration by two
      different DMA channels simultaneously as unexpected behavior can appear.

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
  * @brief  Build DMA linked-list queue to start comparator according to configured parameters in the
  *         LPBAM_COMP_StartFullAdvConf_t.
  * @param  pInstance         : [IN]  Pointer to a COMP_TypeDef structure that selects COMP instance.
  * @param  pDMAListInfo      : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                   linked-list queue type information.
  * @param  pStartFull        : [IN]  Pointer to a LPBAM_COMP_StartFullAdvConf_t structure that contains comparator
  *                                   start configuration.
  * @param  pDescriptor       : [IN]  Pointer to a LPBAM_COMP_StartFullDesc_t structure that contains descriptor
  *                                   information.
  * @param  pQueue            : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                   information.
  * @retval LPBAM Status      : [OUT] Value from LPBAM_Status_t enumeration.
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
  * @brief  Build DMA linked-list queue to get output level (high or low) according to configured parameters in the
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
