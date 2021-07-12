/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_I2C_SequentialTransfer/Src/stm32u5xx_hal_msp.c
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

/** @defgroup LPBAM_I2C_SequentialTransfer
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
  /* Enable I2C autonomous mode clock */
  __HAL_RCC_I2C3_CLKAM_ENABLE();

  /* Enable RTC autonomous mode clock */
  __HAL_RCC_RTCAPB_CLKAM_ENABLE();

  /* Enable LPDMA1 autonomous mode clock */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 autonomous mode clock */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();
}

/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit (I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct                = {0};
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Re-target the HSI to Clock the I2C3 peripheral */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
  RCC_PeriphCLKInitStruct.I2c3ClockSelection   = RCC_I2C3CLKSOURCE_HSI;
  /* Initialize the RCC extended peripherals clocks */
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable peripherals and GPIO Clocks */
  /* Enable GPIO SCL/SDA clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /* Enable I2Cx clock */
  __HAL_RCC_I2C3_CLK_ENABLE();
  /* Enable DMAx clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Reset I2Cx peripheral */
  __HAL_RCC_I2C3_FORCE_RESET();
  __HAL_RCC_I2C3_RELEASE_RESET();

  /* Configure peripheral GPIO */
  /* I2C SCL GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_0;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* I2C SDA GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmatx, DMAHandle);

    /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmarx, DMAHandle);

  /* -5- Configure the NVIC for I2C */
  /* NVIC for I2Cx */
  HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);

  HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
}

/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /* De-initialize the GPIO peripheral */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

  /* De-initialize the GPIO peripheral */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

  /* Reset I2Cx peripheral */
  __HAL_RCC_I2C3_FORCE_RESET();
  __HAL_RCC_I2C3_RELEASE_RESET();

  /* Disable I2Cx clock */
  __HAL_RCC_I2C3_CLK_DISABLE();

  /* Disable the NVIC for I2C */
  HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
  HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
}

/**
  * @brief RTC MSP Initialization
  * @param hrtc: RTC handle pointer
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_CSR register are set to their reset values.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Enable access to backup domain */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Reset the whole backup domain, RTC included */
  __HAL_RCC_BACKUPRESET_FORCE();
  __HAL_RCC_BACKUPRESET_RELEASE();

  /* Enable the LSI oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct.LSIDiv         = RCC_LSI_DIV1;
  RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Re-target the LSI to Clock the I2C3 peripheral */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  /* Initialize the RCC extended peripherals clocks */
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /* Enable RTC APB clock  */
  __HAL_RCC_RTCAPB_CLK_ENABLE();
}

/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /* Disable the RTC peripheral */
  __HAL_RCC_RTC_DISABLE();
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
