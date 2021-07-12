/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_ADC_InetrnalExternalChannelSwitch
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handler declaration */
extern DMA_HandleTypeDef DMAHandle;

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
  /* Enable ADC4 autonomous mode clock   */
  __HAL_RCC_ADC4_CLKAM_ENABLE();

  /* Enable LPTIM1 autonomous mode clock */
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();

  /* Enable LPDMA1 autonomous mode clock */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 autonomous mode clock  */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();
}

/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  GPIO_InitTypeDef GPIO_InitStruct     = {0};

  /* Enable PWR alimentation for ADC */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddA();

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC Periph clock enable */
  ADC_CLK_ENABLE();

  /* Enable MSIK clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIKState      = RCC_MSIK_ON;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable MSI STOP */
  __HAL_RCC_MSIKSTOP_ENABLE();
  /* Set MSIK as ADC clock source */
  __HAL_RCC_ADCDAC_CONFIG(RCC_ADCDACCLKSOURCE_MSIK);

  /*##- 2- Configure peripheral GPIO #########################################*/
  /* Enable GPIO clock  */
  ADC_CHANNEL_GPIO_CLK_ENABLE();
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct.Pin  = ADC_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  /*##-1- Reset peripherals ##################################################*/
  ADC_FORCE_RESET();
  ADC_RELEASE_RESET();

  /* ADC Periph clock disable
  (automatically reset all ADC instances of the ADC common group) */
  __HAL_RCC_ADC4_CLK_DISABLE();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADC_CHANNEL_GPIO_PORT, ADC_CHANNEL_PIN);
}

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit (LPTIM_HandleTypeDef *hlptim)
{
  RCC_OscInitTypeDef RCC_OscInitStruct             = {0};
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Enable LSI clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  /* Initialize the RCC Oscillators according to the specified parameters */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* LPTIM1 MSP initialization */
  if (hlptim->Instance == LPTIM1)
  {
    /* Re-target the LSI to Clock the LPTIM1 Counter */
    /* Select the LSI clock as LPTIM1 peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
    /* Initialize the RCC extended peripherals clocks */
    if (HAL_RCCEx_PeriphCLKConfig (&RCC_PeriphCLKInitStruct) != HAL_OK)
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

    /* Disable LPTIM1 Interrupt */
    HAL_NVIC_DisableIRQ (LPTIM1_IRQn);
  }
  /* LPTIM3 MSP initialization */
  else
  {
    /* Re-target the LSI to Clock the LPTIM3 Counter */
    /* Select the LSI clock as LPTIM3 peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_LPTIM34;
    RCC_PeriphCLKInitStruct.Lptim34ClockSelection = RCC_LPTIM34CLKSOURCE_LSI;
    /* Initialize the RCC extended peripherals clocks */
    if (HAL_RCCEx_PeriphCLKConfig (&RCC_PeriphCLKInitStruct) != HAL_OK)
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
    HAL_NVIC_DisableIRQ (LPTIM3_IRQn);
  }
}

/**
  * @brief  LPTIM MSP DeInit
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspDeInit (LPTIM_HandleTypeDef *hlptim)
{
  /* LPTIM1 MSP de-initialization */
  if (hlptim->Instance == LPTIM1)
  {
    /* Force & Release the LPTIM1 Periheral Clock Reset */
    /* Force the LPTIM1 Peripheral Clock Reset */
    __HAL_RCC_LPTIM1_FORCE_RESET();

    /* Release the LPTIM1 Peripheral Clock Reset */
    __HAL_RCC_LPTIM1_RELEASE_RESET();

    /* Disable LPTIM1 clock */
    __HAL_RCC_LPTIM1_CLK_DISABLE();

    /* Disable LPTIM1 Interrupt */
    HAL_NVIC_DisableIRQ(LPTIM1_IRQn);
  }
  else
  {
    /* Force & Release the LPTIM3 Periheral Clock Reset */
    /* Force the LPTIM3 Peripheral Clock Reset */
    __HAL_RCC_LPTIM3_FORCE_RESET();

    /* Release the LPTIM3 Peripheral Clock Reset */
    __HAL_RCC_LPTIM3_RELEASE_RESET();

    /* Disable LPTIM3 clock */
    __HAL_RCC_LPTIM3_CLK_DISABLE();

    /* Disable LPTIM3 Interrupt */
    HAL_NVIC_DisableIRQ(LPTIM3_IRQn);
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
