/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_uart.c
  * @author  MCD Application Team
  * @brief   This file provides the basic LPBAM layer for UART peripheral.
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
  */

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_ll_lpbam.h"

#ifdef LPBAM_UART_MODULE_ENABLED

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup UART_Basic
  * @brief      UART LPBAM module driver
  * @{
  */

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_UART_Basic_Exported_Functions LPBAM UART Basic Exported Functions
  * @{
  */

/**
  * @brief  Fill linked-list node configuration according to configured parameters in the LPBAM_UART_ConfNode_t.
  * @param  pConfNode      : Pointer to a LPBAM_UART_ConfNode_t structure that contains node information.
  * @param  pDMANodeConfig : Pointer to a DMA_NodeConfTypeDef structure that contains the configuration information for
  *                          the specified DMA linked-list node.
  * @retval Return LPBAM Status.
  */
LPBAM_Status_t LPBAM_UART_FillNodeConfig(LPBAM_UART_ConfNode_t const *const pConfNode,
                                         DMA_NodeConfTypeDef   *const pDMANodeConfig)
{
  LPBAM_Status_t   status;
  LPBAM_InfoDesc_t desc_info;

  /* Check parameters */
  if ((pConfNode == NULL) || (pDMANodeConfig == NULL))
  {
    /* Return lpbam status */
    return LPBAM_ERROR;
  }

  /* Set node type */
  pDMANodeConfig->NodeType                            = pConfNode->NodeDesc.NodeInfo.NodeType;

  /* Set common node parameters */
  pDMANodeConfig->Init.Request                        = DMA_REQUEST_SW;
  pDMANodeConfig->Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  pDMANodeConfig->Init.Direction                      = DMA_MEMORY_TO_MEMORY;
  pDMANodeConfig->Init.SrcInc                         = DMA_SINC_FIXED;
  pDMANodeConfig->Init.DestInc                        = DMA_DINC_FIXED;
  pDMANodeConfig->Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_WORD;
  pDMANodeConfig->Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_WORD;
  pDMANodeConfig->Init.Priority                       = DMA_HIGH_PRIORITY;
  if ((pDMANodeConfig->NodeType & DMA_CHANNEL_TYPE_GPDMA) == DMA_CHANNEL_TYPE_GPDMA)
  {
    pDMANodeConfig->Init.SrcBurstLength               = 1U;
    pDMANodeConfig->Init.DestBurstLength              = 1U;
    pDMANodeConfig->Init.TransferAllocatedPort        = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  }
  pDMANodeConfig->Init.TransferEventMode              = DMA_TCEM_LAST_LL_ITEM_TRANSFER;

  /* Set node data handling parameters */
  if ((pDMANodeConfig->NodeType & DMA_CHANNEL_TYPE_GPDMA) == DMA_CHANNEL_TYPE_GPDMA)
  {
    pDMANodeConfig->DataHandlingConfig.DataExchange   = DMA_EXCHANGE_NONE;
  }
  pDMANodeConfig->DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;

  /* Set node trigger parameters */
  pDMANodeConfig->TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

  /* Set node repeated block parameters */
  if ((pDMANodeConfig->NodeType & DMA_CHANNEL_TYPE_2D_ADDR) == DMA_CHANNEL_TYPE_2D_ADDR)
  {
    pDMANodeConfig->RepeatBlockConfig.RepeatCount       = 1U;
    pDMANodeConfig->RepeatBlockConfig.SrcAddrOffset     = 0;
    pDMANodeConfig->RepeatBlockConfig.DestAddrOffset    = 0;
    pDMANodeConfig->RepeatBlockConfig.BlkSrcAddrOffset  = 0;
    pDMANodeConfig->RepeatBlockConfig.BlkDestAddrOffset = 0;
  }
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Set node attribute parameters */
  pDMANodeConfig->SrcSecure                           = DMA_CHANNEL_SRC_NSEC;
  pDMANodeConfig->DestSecure                          = DMA_CHANNEL_DEST_NSEC;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Fill the structure information for UART peripheral */
  status = LPBAM_UART_FillStructInfo(pConfNode, &desc_info);

  /* Check LPBAM status */
  if (status != LPBAM_OK)
  {
    /* return status */
    return status;
  }

  /* Check Node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    case  LPBAM_UART_TRANSMIT_DATA_ID:
    {
      /* Update transfer fields for UART peripheral */
      pDMANodeConfig->Init.Request       = desc_info.Request;

      /* Update transfer fields for UART peripheral */
      pDMANodeConfig->Init.Direction     = DMA_MEMORY_TO_PERIPH;
      pDMANodeConfig->Init.SrcInc        = DMA_SINC_INCREMENTED;
      pDMANodeConfig->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      pDMANodeConfig->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;

      break;
    }

    case LPBAM_UART_RECEIVE_DATA_ID :
    {
      /* Update transfer fields for UART peripheral */
      pDMANodeConfig->Init.Request       = desc_info.Request;

      /* Update transfer fields for UART peripheral */
      pDMANodeConfig->Init.Direction     = DMA_PERIPH_TO_MEMORY;
      pDMANodeConfig->Init.DestInc       = DMA_DINC_INCREMENTED;
      pDMANodeConfig->Init.SrcDataWidth  = DMA_SRC_DATAWIDTH_BYTE;
      pDMANodeConfig->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;

      break;
    }

    default:
    {
      break;
    }
  }

  /* Update transfer fields for UART peripheral */
  pDMANodeConfig->SrcAddress = desc_info.SrcAddr;
  pDMANodeConfig->DstAddress = desc_info.DstAddr;
  pDMANodeConfig->DataSize   = desc_info.DataSize;

  /* Prevent unused argument(s) compilation warning */
  (void)(pDMANodeConfig);

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

#endif /* LPBAM_UART_MODULE_ENABLED */
