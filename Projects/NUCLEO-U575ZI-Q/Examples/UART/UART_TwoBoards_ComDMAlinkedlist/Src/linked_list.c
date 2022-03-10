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

DMA_NodeTypeDef Node1_tx;
DMA_QListTypeDef UART_Tx_Queue;
DMA_NodeTypeDef Node2_tx;
DMA_NodeTypeDef Node3_tx;
DMA_NodeTypeDef Node1_rx;
DMA_QListTypeDef UART_Rx_Queue;
DMA_NodeTypeDef Node2_rx;
DMA_NodeTypeDef Node3_rx;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern UART_HandleTypeDef huart2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/**
  * @brief  DMA Linked-list UART_Tx_Queue configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_UART_Tx_Queue_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
  pNodeConfig.Init.Request = GPDMA1_REQUEST_USART2_TX;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
  pNodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = 0;
  pNodeConfig.DstAddress = 0;
  pNodeConfig.DataSize = 0;

  /* Build Node1_tx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node1_tx);

  /* Insert Node1_tx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Tx_Queue, &Node1_tx);

  /* Set node configuration ################################################*/
  pNodeConfig.SrcAddress = (uint32_t) aTxBuffer1;
  pNodeConfig.DstAddress = (uint32_t)&huart2.Instance->TDR;
  pNodeConfig.DataSize = 128;

  /* Build Node2_tx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node2_tx);

  /* Insert Node2_tx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Tx_Queue, &Node2_tx);

  /* Set node configuration ################################################*/
  pNodeConfig.SrcAddress = (uint32_t) aTxBuffer2;

  /* Build Node3_tx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node3_tx);

  /* Insert Node3_tx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Tx_Queue, &Node3_tx);

   return ret;
}

/**
  * @brief  DMA Linked-list UART_Rx_Queue configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_UART_Rx_Queue_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
  pNodeConfig.Init.Request = GPDMA1_REQUEST_USART2_RX;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = 0;
  pNodeConfig.DstAddress = 0;
  pNodeConfig.DataSize = 0;

  /* Build Node1_rx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node1_rx);

  /* Insert Node1_rx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Rx_Queue, &Node1_rx);

  /* Set node configuration ################################################*/
  pNodeConfig.SrcAddress = (uint32_t)&huart2.Instance->RDR;
  pNodeConfig.DstAddress = (uint32_t) aRxBuffer1;
  pNodeConfig.DataSize = 128;

  /* Build Node2_rx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node2_rx);

  /* Insert Node2_rx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Rx_Queue, &Node2_rx);

  /* Set node configuration ################################################*/
  pNodeConfig.DstAddress = (uint32_t) aRxBuffer2;

  /* Build Node3_rx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node3_rx);

  /* Insert Node3_rx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&UART_Rx_Queue, &Node3_rx);

   return ret;
}

