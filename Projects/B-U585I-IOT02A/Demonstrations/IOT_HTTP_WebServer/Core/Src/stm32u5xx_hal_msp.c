/**
  **********************************************************************************************************************
  * @file    stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization and de-Initialization codes.
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
extern DMA_HandleTypeDef handle_GPDMA1_Channel4;
extern DMA_HandleTypeDef handle_GPDMA1_Channel5;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* External functions ------------------------------------------------------------------------------------------------*/

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* Enable VDDIO2 supply */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
  HAL_PWREx_DisableUCPDDeadBattery();
}

/**
* @brief  SPI MSP Initialization
* @param  hspi : SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct       = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Check SPI instance */
  if (hspi->Instance == SPI2)
  {
    /* Peripherals clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPDMA1_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure peripheral kernel clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
    PeriphClkInit.Spi2ClockSelection   = RCC_SPI2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      webserver_process_error();
    }

    /* SPI2 GPIO Configuration */
    GPIO_InitStruct.Pin       = GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Set Rx DMA channel configuration */
    handle_GPDMA1_Channel4.Instance                   = GPDMA1_Channel4;
    handle_GPDMA1_Channel4.Init.Request               = GPDMA1_REQUEST_SPI2_RX;
    handle_GPDMA1_Channel4.Init.BlkHWRequest          = DMA_BREQ_SINGLE_BURST;
    handle_GPDMA1_Channel4.Init.Direction             = DMA_PERIPH_TO_MEMORY;
    handle_GPDMA1_Channel4.Init.SrcInc                = DMA_SINC_FIXED;
    handle_GPDMA1_Channel4.Init.DestInc               = DMA_DINC_INCREMENTED;
    handle_GPDMA1_Channel4.Init.SrcDataWidth          = DMA_SRC_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel4.Init.DestDataWidth         = DMA_DEST_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel4.Init.Priority              = DMA_LOW_PRIORITY_HIGH_WEIGHT;
    handle_GPDMA1_Channel4.Init.SrcBurstLength        = 1U;
    handle_GPDMA1_Channel4.Init.DestBurstLength       = 1U;
    handle_GPDMA1_Channel4.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    handle_GPDMA1_Channel4.Init.TransferEventMode     = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel4.Init.Mode                  = DMA_NORMAL;
    /* Initialize Rx DMA channel */
    if (HAL_DMA_Init(&handle_GPDMA1_Channel4) != HAL_OK)
    {
      webserver_process_error();
    }
    /* Link Rx DMA channel handle */
    __HAL_LINKDMA(hspi, hdmarx, handle_GPDMA1_Channel4);

    /* Set Tx DMA channel configuration */
    handle_GPDMA1_Channel5.Instance                   = GPDMA1_Channel5;
    handle_GPDMA1_Channel5.Init.Request               = GPDMA1_REQUEST_SPI2_TX;
    handle_GPDMA1_Channel5.Init.BlkHWRequest          = DMA_BREQ_SINGLE_BURST;
    handle_GPDMA1_Channel5.Init.Direction             = DMA_MEMORY_TO_PERIPH;
    handle_GPDMA1_Channel5.Init.SrcInc                = DMA_SINC_INCREMENTED;
    handle_GPDMA1_Channel5.Init.DestInc               = DMA_DINC_FIXED;
    handle_GPDMA1_Channel5.Init.SrcDataWidth          = DMA_SRC_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel5.Init.DestDataWidth         = DMA_DEST_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel5.Init.Priority              = DMA_LOW_PRIORITY_HIGH_WEIGHT;
    handle_GPDMA1_Channel5.Init.SrcBurstLength        = 1U;
    handle_GPDMA1_Channel5.Init.DestBurstLength       = 1U;
    handle_GPDMA1_Channel5.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    handle_GPDMA1_Channel5.Init.TransferEventMode     = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel5.Init.Mode                  = DMA_NORMAL;
    /* Initialize Tx DMA channel */
    if (HAL_DMA_Init(&handle_GPDMA1_Channel5) != HAL_OK)
    {
      webserver_process_error();
    }
    /* Link Tx DMA channel handle */
    __HAL_LINKDMA(hspi, hdmatx, handle_GPDMA1_Channel5);

    /* SPI2 interrupt Init */
    HAL_NVIC_SetPriority(SPI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
    /* GPDMA1 channel4 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
    /* GPDMA1 channel5 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);
  }
}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  /* Check SPI instance */
  if (hspi->Instance == SPI2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /* SPI2 GPIO Configuration */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_1);

    /* De-Initialize DMA channels */
    HAL_DMA_DeInit(hspi->hdmarx);
    HAL_DMA_DeInit(hspi->hdmatx);

    /* SPI2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI2_IRQn);
  }
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Initializes the peripherals clock */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  /* Peripheral clock enable */
  __HAL_RCC_USART1_CLK_ENABLE();

  /* USART1 GPIO Configuration */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  /* Peripheral clock disable */
  __HAL_RCC_USART1_CLK_DISABLE();

  /* USART1 GPIO Configuration */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10|GPIO_PIN_9);

  /* USART1 interrupt DeInit */
  HAL_NVIC_DisableIRQ(USART1_IRQn);
}

/**
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  RCC_OscInitTypeDef       RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    webserver_process_error();
  }

  /* Select the LSE clock as LPTIM peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    webserver_process_error();
  }

  /* Enable LPTIM clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* Force the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

  /* USART1 interrupt DeInit */
  HAL_NVIC_SetPriority(LPTIM1_IRQn, 15, 15);
  HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
}
