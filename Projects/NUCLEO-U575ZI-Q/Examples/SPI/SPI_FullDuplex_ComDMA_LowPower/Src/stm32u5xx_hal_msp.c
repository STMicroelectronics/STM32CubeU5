/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComDMA_LowPower/Src/stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *          This file template is located in the HAL folder and should be copied
  *          to the user folder.
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
#include "stm32u5xx_hal.h"
#include "main.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup SPI_FullDuplex_ComDMA_LowPower
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;
/* DMA node configuration declaration */
DMA_NodeConfTypeDef node_tx_config;
DMA_NodeConfTypeDef node_rx_config;

/* Queue variable declaration */
DMA_QListTypeDef  Queue_tx;
DMA_QListTypeDef  Queue_rx;

/* DMA nodes declaration */
DMA_NodeTypeDef Node_tx, Node_rx;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
GPIO_InitTypeDef  GPIO_InitStruct;

  if (hspi->Instance == SPIx)
  {
    /*##-1- Enable peripherals and GPIO Clocks ###############################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();

    /* Enable SPI1 clocks */
    SPIx_CLK_ENABLE();
    SPIx_CLKAM_ENABLE();
    SPIx_CLK_SLEEP_ENABLE();

    /* Enable DMA clocks */
    DMAx_CLK_ENABLE();
    DMAx_CLKAM_ENABLE();
    DMAx_CLK_SLEEP_ENABLE();

    /* Enable SRAMx & Power clocks */
    SRAMx_CLK_ENABLE();
    SRAMx_CLKAM_ENABLE();
    SRAMx_CLK_SLEEP_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the DMA ################################################*/
    /* Configure the DMA handler for Transmission process */
    /* Set node type */
    node_tx_config.NodeType                            = DMA_GPDMA_LINEAR_NODE;

    /* Set common node parameters */
    node_tx_config.Init.Request                        = SPIx_TX_DMA_REQUEST;
    node_tx_config.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    node_tx_config.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
    node_tx_config.Init.SrcInc                         = DMA_SINC_INCREMENTED;
    node_tx_config.Init.DestInc                        = DMA_DINC_FIXED;
    node_tx_config.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_BYTE;
    node_tx_config.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_BYTE;
    node_tx_config.Init.SrcBurstLength                 = 1U;
    node_tx_config.Init.DestBurstLength                = 1U;
    node_tx_config.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    node_tx_config.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;

    /* Set node data handling parameters */
    node_tx_config.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    node_tx_config.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;

    /* Set node trigger parameters */
    node_tx_config.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build Node_tx */
    HAL_DMAEx_List_BuildNode(&node_tx_config, &Node_tx);

    /* Insert Node_tx to Queue */
    HAL_DMAEx_List_InsertNode_Tail(&Queue_tx, &Node_tx);

    /* Set DMA instance */
    hdma_tx.Instance                               = SPIx_TX_DMA_STREAM;

    hdma_tx.InitLinkedList.Priority                = DMA_HIGH_PRIORITY;
    hdma_tx.InitLinkedList.LinkStepMode            = DMA_LSM_FULL_EXECUTION;
    hdma_tx.InitLinkedList.LinkAllocatedPort       = DMA_LINK_ALLOCATED_PORT1;
    hdma_tx.InitLinkedList.TransferEventMode       = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hdma_tx.InitLinkedList.LinkedListMode          = DMA_LINKEDLIST_NORMAL;

    /* DMA linked list init */
    HAL_DMAEx_List_Init(&hdma_tx);

    /* Link created queue to DMA channel #######################################*/
    HAL_DMAEx_List_LinkQ(&hdma_tx, &Queue_tx);

    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    /* Set node type */
    node_rx_config.NodeType                            = DMA_GPDMA_LINEAR_NODE;

    /* Set common node parameters */
    node_rx_config.Init.Request                        = SPIx_RX_DMA_REQUEST;
    node_rx_config.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    node_rx_config.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
    node_rx_config.Init.SrcInc                         = DMA_SINC_FIXED;
    node_rx_config.Init.DestInc                        = DMA_DINC_INCREMENTED;
    node_rx_config.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_BYTE;
    node_rx_config.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_BYTE;
    node_rx_config.Init.SrcBurstLength                 = 1U;
    node_rx_config.Init.DestBurstLength                = 1U;
    node_rx_config.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    node_rx_config.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;

    /* Set node data handling parameters */
    node_rx_config.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    node_rx_config.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;

    /* Set node trigger parameters */
    node_rx_config.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build Node_rx */
    HAL_DMAEx_List_BuildNode(&node_rx_config, &Node_rx);

    /* Insert Node_rx to Queue */
    HAL_DMAEx_List_InsertNode_Tail(&Queue_rx, &Node_rx);

    /* Set DMA instance */
    hdma_rx.Instance                               = SPIx_RX_DMA_STREAM;

    hdma_rx.InitLinkedList.Priority                = DMA_HIGH_PRIORITY;
    hdma_rx.InitLinkedList.LinkStepMode            = DMA_LSM_FULL_EXECUTION;
    hdma_rx.InitLinkedList.LinkAllocatedPort       = DMA_LINK_ALLOCATED_PORT1;
    hdma_rx.InitLinkedList.TransferEventMode       = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hdma_rx.InitLinkedList.LinkedListMode          = DMA_LINKEDLIST_NORMAL;

    /* DMA linked list init */
    HAL_DMAEx_List_Init(&hdma_rx);

    /* Link created queue to DMA channel #######################################*/
    HAL_DMAEx_List_LinkQ(&hdma_rx, &Queue_rx);
    /* Associate the initialized DMA handle to the the SPI handle */
    __HAL_LINKDMA(hspi, hdmarx, hdma_rx);

    /*##-4- Configure the NVIC for DMA #######################################*/
    /* NVIC configuration for DMA transfer complete interrupt (SPI1_TX) */
    HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (SPI1_RX) */
    HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);

    /*##-5- Configure the NVIC for SPI #######################################*/
    /* NVIC configuration for SPI transfer complete interrupt (SPI1) */
    HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPIx_IRQn);
  }
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ##############################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Deconfigure SPI MISO */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Deconfigure SPI MOSI */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
    /*##-3- Disable the DMA ##################################################*/
    /* De-Initialize the DMA associated to transmission process */
    HAL_DMA_DeInit(&hdma_tx);
    /* De-Initialize the DMA associated to reception process */
    HAL_DMA_DeInit(&hdma_rx);

    /*##-4- Disable the NVIC for DMA #########################################*/
    HAL_NVIC_DisableIRQ(SPIx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(SPIx_DMA_RX_IRQn);

    /*##-5- Disable the NVIC for SPI #########################################*/
    HAL_NVIC_DisableIRQ(SPIx_IRQn);
  }
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
