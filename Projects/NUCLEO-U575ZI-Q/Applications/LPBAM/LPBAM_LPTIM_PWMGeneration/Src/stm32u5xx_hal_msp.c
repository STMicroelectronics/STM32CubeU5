/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPTIM_PWMGeneration/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_LPTIM_PWMGeneration
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
  /* Enable LPDMA1 Autonomous Mode       */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 Autonomous Mode        */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();

  /* Enable LPTIM1 autonomous mode clock */
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();
}

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit (LPTIM_HandleTypeDef *hlptim)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Select LSE as LPTIM1 clock source */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* Force & Release the LPTIM1 Periheral Clock Reset */
  /* Force the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM1 Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

#if defined (DEBUG_CONFIGURATION)
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIO PORT */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PB.2 */
  GPIO_InitStruct.Pin       = GPIO_PIN_2;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif /* defined (DEBUG_CONFIGURATION) */

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
  * @}
  */

/**
  * @}
  */
