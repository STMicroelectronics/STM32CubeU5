/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : linked_list.c
  * Description        : This file provides code for the configuration
  *                      of the LinkedList.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "linked_list.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

DMA_NodeTypeDef DCMINode1;
DMA_QListTypeDef DCMIQueue;
DMA_NodeTypeDef DCMINode2;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/**
  * @brief  DMA Linked-list DCMIQueue configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_DCMIQueue_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_2D_NODE;
  pNodeConfig.Init.Request = GPDMA1_REQUEST_DCMI;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.RepeatBlockConfig.RepeatCount = 1;
  pNodeConfig.RepeatBlockConfig.SrcAddrOffset = 0;
  pNodeConfig.RepeatBlockConfig.DestAddrOffset = 0;
  pNodeConfig.RepeatBlockConfig.BlkSrcAddrOffset = 0;
  pNodeConfig.RepeatBlockConfig.BlkDestAddrOffset = 0;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = 0;
  pNodeConfig.DstAddress = 0;
  pNodeConfig.DataSize = 0;

  /* Build DCMINode1 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &DCMINode1);

  /* Insert DCMINode1 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&DCMIQueue, &DCMINode1);

  /* Set node configuration ################################################*/

  /* Build DCMINode2 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &DCMINode2);

  /* Insert DCMINode2 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&DCMIQueue, &DCMINode2);

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&DCMIQueue, &DCMINode1);

   return ret;
}

