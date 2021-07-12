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

/* Includes ------------------------------------------------------------------*/
#include "linked_list.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

DMA_NodeTypeDef Node1;
DMA_QListTypeDef Queue;
DMA_NodeTypeDef Node2;
DMA_NodeTypeDef Node3;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 uint32_t aSRC_Buffer1[BUFFER1_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
};

 uint32_t aSRC_Buffer2[BUFFER2_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
};

 uint32_t aSRC_Buffer3[BUFFER3_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
};

/* Destination buffers declaration */
 uint32_t aDST_Buffer1[BUFFER1_SIZE];
 uint32_t aDST_Buffer2[BUFFER2_SIZE];
 uint32_t aDST_Buffer3[BUFFER3_SIZE];
/* USER CODE END PM */

/**
  * @brief  DMA Linked-list Queue configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_Queue_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_2D_NODE;
  pNodeConfig.Init.Request = DMA_REQUEST_SW;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
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
  pNodeConfig.SrcAddress = (uint32_t) aSRC_Buffer1;
  pNodeConfig.DstAddress = (uint32_t) aDST_Buffer1;
  pNodeConfig.DataSize = BUFFER1_SIZE * 4U;

  /* Build Node1 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node1);

  /* Insert Node1 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&Queue, &Node1);

  /* Set node configuration ################################################*/
  pNodeConfig.SrcAddress = (uint32_t) aSRC_Buffer2;
  pNodeConfig.DstAddress = (uint32_t) aDST_Buffer2;
  pNodeConfig.DataSize = BUFFER2_SIZE * 4U;

  /* Build Node2 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node2);

  /* Insert Node2 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&Queue, &Node2);

  /* Set node configuration ################################################*/
  pNodeConfig.SrcAddress = (uint32_t) aSRC_Buffer3;
  pNodeConfig.DstAddress = (uint32_t) aDST_Buffer3;
  pNodeConfig.DataSize = BUFFER3_SIZE * 4U;

  /* Build Node3 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &Node3);

  /* Insert Node3 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&Queue, &Node3);

   return ret;
}

