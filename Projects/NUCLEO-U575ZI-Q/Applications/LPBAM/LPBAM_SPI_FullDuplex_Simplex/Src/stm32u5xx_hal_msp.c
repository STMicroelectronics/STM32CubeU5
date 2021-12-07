/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_SPI_FullDuplex_Simplex/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_SPI_FullDuplex_Simplex
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handler declaration */
//extern DMA_HandleTypeDef DMAHandle;

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
  /* Enable LPDMA Autonomous Mode */
  LPDMA_CLKAM_ENABLE ();

  /* Enable SRAMx Autonomous Mode */
  SRAMx_CLKAM_ENABLE ();

  /* Enable LPTIM autonomous mode clock */
  LPTIMx_CLKAM_ENABLE();

  /* Enable SPIx autonomous mode clock */
  SPIx_CLKAM_ENABLE();
}

/**
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit (SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (hspi->Instance == SPIy)
  {
    /*##-1- Enable peripherals and GPIO Clocks ###############################*/
    /* Enable GPIO TX/RX clock */
    SPIy_SCK_GPIO_CLK_ENABLE();
    SPIy_MISO_GPIO_CLK_ENABLE();
    SPIy_MOSI_GPIO_CLK_ENABLE();

    /* Enable SPIy clock */
    SPIy_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ########################################*/
    /* SPIy SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIy_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPIy_SCK_AF;
    HAL_GPIO_Init(SPIy_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPIy MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIy_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIy_MISO_AF;
    HAL_GPIO_Init(SPIy_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPIy MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIy_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIy_MOSI_AF;
    HAL_GPIO_Init(SPIy_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for SPI #######################################*/
    /* NVIC configuration for SPI transfer complete interrupt (SPI1) */
    HAL_NVIC_SetPriority(SPIy_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPIy_IRQn);
  }
  else
  {
    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};
    RCC_OscInitTypeDef       RCC_OscInitStruct       = {0};

    /* Enable MSIK clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIK;
    RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_7;
    RCC_OscInitStruct.MSIKState      = RCC_MSIK_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Re-target the HSI to Clock the SPI3 peripheral */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI3;
    RCC_PeriphCLKInitStruct.Spi3ClockSelection   = RCC_SPI3CLKSOURCE_MSIK;
    /* Initialize the RCC extended peripherals clocks */
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /*##-1- Enable peripherals and GPIO Clocks ###############################*/
    /* Enable SPIx GPIOs */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();

    /* Enable SPIx clock */
    SPIx_CLK_ENABLE();

    /* Enable DMA clock */
    DMAx_CLK_ENABLE();

    /* Reset SPIx peripheral */
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Configure peripheral GPIO ########################################*/
    /* SPIx SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPIx MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPIx MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for SPI #######################################*/
    /* Configure the NVIC for SPIx */
    HAL_NVIC_SetPriority(SPI3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(SPI3_IRQn);
  }
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPIy)
  {
    /*##-1- Reset SPIy peripheral ############################################*/
    SPIy_FORCE_RESET();
    SPIy_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ##############################*/
    /* Deconfigure SPIy SCK */
    HAL_GPIO_DeInit(SPIy_SCK_GPIO_PORT, SPIy_SCK_PIN);
    /* Deconfigure SPIy MISO */
    HAL_GPIO_DeInit(SPIy_MISO_GPIO_PORT, SPIy_MISO_PIN);
    /* Deconfigure SPIy MOSI */
    HAL_GPIO_DeInit(SPIy_MOSI_GPIO_PORT, SPIy_MOSI_PIN);
  }
  else
  {
    /*##-1- Reset SPIx peripherals ###########################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ##############################*/
    /* Deconfigure SPIx SCK */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Deconfigure SPIx MISO */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Deconfigure SPIx MOSI */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  }
}

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit (LPTIM_HandleTypeDef *hlptim)
{
  RCC_OscInitTypeDef       RCC_OscInitStruct       = {0};
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Enable LSI clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  /* Initialize the RCC Oscillators according to the specified parameters */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Re-target the LSI to Clock the LPTIMx Counter */
  /* Select the LSI clock as LPTIMx peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
  /* Initialize the RCC extended peripherals clocks */
  if (HAL_RCCEx_PeriphCLKConfig (&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Enable LPTIMx clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* Force & Release the LPTIMx Periheral Clock Reset */
  /* Force the LPTIMx Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIMx Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

#if defined (DEBUG_CONFIGURATION)
    GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIO PORT */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PB.2 */
  GPIO_InitStruct.Pin       = GPIO_PIN_2;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif /* defined (DEBUG_CONFIGURATION) */

  /* Disable LPTIM1 Interrupt */
  HAL_NVIC_DisableIRQ (LPTIM1_IRQn);

  /* Disable LPTIMx Interrupt */
  HAL_NVIC_DisableIRQ (LPTIM1_IRQn);
}

/**
  * @brief  LPTIM MSP DeInit
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspDeInit (LPTIM_HandleTypeDef *hlptim)
{
  /* Force & Release the LPTIMx Periheral Clock Reset */
  /* Force the LPTIMx Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIMx Peripheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

  /* Disable LPTIMx clock */
  __HAL_RCC_LPTIM1_CLK_DISABLE();

  /* Disable LPTIMx Interrupt */
  HAL_NVIC_DisableIRQ(LPTIM1_IRQn);
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
