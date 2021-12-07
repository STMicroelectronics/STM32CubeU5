/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_DAC_OPAMP_ContinuousConversion
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
  __HAL_RCC_LPTIM3_CLKAM_ENABLE();

  /* Enable DAC1 autonomous mode clock */
  __HAL_RCC_DAC1_CLKAM_ENABLE();

  /* Enable OPAMP autonomous mode clock */
  __HAL_RCC_OPAMP_CLKAM_ENABLE();
}

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Select the LSE clock as LPTIM3 peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_LPTIM34;
  RCC_PeriphCLKInitStruct.Lptim34ClockSelection = RCC_LPTIM34CLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable LPTIM3 clock */
  __HAL_RCC_LPTIM3_CLK_ENABLE();

  /* Force & Release the LPTIM3 Periheral Clock Reset */
  /* Force the LPTIM3 Peripheral Clock Reset */
  __HAL_RCC_LPTIM3_FORCE_RESET();

  /* Release the LPTIM3 Peripheral Clock Reset */
  __HAL_RCC_LPTIM3_RELEASE_RESET();

  /* Disable LPTIM3 Interrupt */
  HAL_NVIC_DisableIRQ(LPTIM3_IRQn);
}

/**
  * @brief  LPTIM MSP DeInit
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef *hlptim)
{
  /* Disable LPTIM3 clock */
  __HAL_RCC_LPTIM3_CLK_DISABLE();

  /* Disable LPTIM3 Interrupt */
  HAL_NVIC_DisableIRQ(LPTIM3_IRQn);
}

/**
  * @brief OPAMP MSP Initialization
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_ENABLE();

  /* Force & Release the OPAMP Periheral Clock Reset */
  /* Force the OPAMP Peripheral Clock Reset */
  __HAL_RCC_OPAMP_FORCE_RESET();

  /* Release the OPAMP Peripheral Clock Reset */
  __HAL_RCC_OPAMP_RELEASE_RESET();
}

/**
  * @brief OPAMP MSP DeInit
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_DISABLE();
}

/**
  * @brief  DAC MSP Init
  * @param  hdac : DAC handle
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};
  RCC_OscInitTypeDef       RCC_OscInitStruct;

  /* Enable MSIK clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIKState      = RCC_MSIK_ON;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_4;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select kernel peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
  RCC_PeriphCLKInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_MSIK;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set LSE kernel clock for DAC peripheral */
  __HAL_RCC_DAC1_CONFIG(RCC_DAC1CLKSOURCE_LSE);

  /* Enable DAC1 clock */
  __HAL_RCC_DAC1_CLK_ENABLE();
}

/**
  * @brief  DAC MSP DeInit
  * @param  hdac : DAC handle
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /* Disable DAC1 clock */
  __HAL_RCC_DAC1_CLK_DISABLE();
}
/**
  * @}
  */

/**
  * @}
  */
