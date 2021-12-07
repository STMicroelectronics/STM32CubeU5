/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPUART_TransmitReceive/Src/stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
#include "main.h"

/** @addtogroup STM32U5xx_Application
  * @{
  */

/** @defgroup LPBAM_LPUART_TransmitReceive
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
  /* Enable LPDMA1 Autonomous Mode */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 Autonomous Mode */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();

  /* Enable LPTIM1 autonomous mode clock */
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();

  /* Enable LPUART1 autonomous mode clock */
  __HAL_RCC_LPUART1_CLKAM_ENABLE();
}

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit (LPTIM_HandleTypeDef *hlptim)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Select MSIK as LPTIM1 clock source */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    while(1);
  }

  /* Enable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* Force & Release the LPTIM1 Periheral Clock Reset */
  /* Force the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

  /* Disable LPTIM1 Interrupt */
  HAL_NVIC_DisableIRQ (LPTIM1_IRQn);
}

/**
  * @brief  LPTIM MSP DeInit
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspDeInit (LPTIM_HandleTypeDef *hlptim)
{
  /* Force & Release the LPTIM1 Periheral Clock Reset */
  /* Force the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

  /* Disable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_DISABLE();

  /* Disable LPTIM1 Interrupt */
  HAL_NVIC_DisableIRQ (LPTIM1_IRQn);
}


/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef            GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef    PeriphClkInit;

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* USART Kernel Clock Config */
  PeriphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  /* Enable MSIK clock in stop */
  __HAL_RCC_MSIKSTOP_ENABLE();

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Enable USART clock */
  __HAL_RCC_LPUART1_CLK_ENABLE();

  /* Enable DMA clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_11;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /* Reset peripherals */
  __HAL_RCC_LPUART1_FORCE_RESET();
  __HAL_RCC_LPUART1_RELEASE_RESET();

  /* Disable peripherals and GPIO Clocks */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
}
/**
  * @}
  */

/**
  * @}
  */
