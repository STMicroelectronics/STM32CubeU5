/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_COMP_InputSwitch/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_COMP_InputSwitch
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

  /* Enable DMA clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();
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

  /* PWR clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable LPTIM1 clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

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
  * @brief COMP MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp)
{
  GPIO_InitTypeDef      GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Enable COMP clock */
  __HAL_RCC_COMP_CLK_ENABLE();
  __HAL_RCC_COMP12_CLKAM_ENABLE();
  __HAL_RCC_COMP_CLK_SLEEP_ENABLE();


  /*##-2- Configure peripheral GPIO ##########################################*/
  /* COMP GPIO pin configuration */
  GPIO_InitStruct.Pin  = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_0;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = GPIO_AF12_COMP1;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp: pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp)
{
  /*##-1- De-initialize peripheral GPIO ######################################*/
  /* De-initialize the COMPx GPIO pin */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

  /*##-2- Disable peripherals and GPIO clocks ################################*/
  /* Disable COMP clock */
  __HAL_RCC_COMP_CLK_DISABLE();

  /* Disable GPIO clock */
  /* Note: In a real application, by disabling GPIO port, be cautious to not  */
  /* interfere with potential other peripherals using other GPIOs on the same */
  /* port.                                                                    */
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
}
/**
  * @}
  */

/**
  * @}
  */
