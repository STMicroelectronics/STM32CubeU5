/**
  ******************************************************************************
  * @file    stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *          This file template is located in the HAL folder and should be copied
  *          to the user folder.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "interfaces_conf.h"

/**
  * @brief  Initialize the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief This function configures FDCAN used hardware resources
  * @param hfdcan: FDCAN handle pointer
  * @retval None
  */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /* Select PLL1Q as source of FDCANx clock */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN1;
  RCC_PeriphClkInit.Fdcan1ClockSelection = RCC_FDCAN1CLKSOURCE_PLL1;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    /* Initialization Error */
    NVIC_SystemReset();
  }

  /* Enable FDCANx clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_FDCAN1);

  /* FDCANx pins configuration -------------------------------------------------*/
  /*      +--------------+
          |     FDCAN1   |
  +-------+--------------+
  |  Tx   |     PB8      |
  +-------+--------------+
  |  Rx   |     PB9      |
  +-------+--------------+ */

  /* Common configuration for FDCANx PINs:
      Mode       : Alternate function.
      Output type: Push-Pull (No pull).
      Speed      : High speed.
  */
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate  = FDCANx_TX_AF;

  /* Configure FDCAN Tx PIN */
  GPIO_InitStruct.Pin = FDCANx_TX_PIN;
  (void)LL_GPIO_Init(FDCANx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* Configure FDCAN Rx */
  GPIO_InitStruct.Pin = FDCANx_RX_PIN;
  (void)LL_GPIO_Init(FDCANx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes the FDCAN MSP.
  * @param  hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * @retval None
  */
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan)
{
  /* 1- Reset peripherals */
  FDCANx_FORCE_RESET();
  FDCANx_RELEASE_RESET();

  /* 2- Disable peripherals and GPIO Clocks */
  /* Configure FDCANx Tx as alternate function */
  HAL_GPIO_DeInit(FDCANx_TX_GPIO_PORT, FDCANx_TX_PIN);

  /* Configure FDCANx Rx as alternate function */
  HAL_GPIO_DeInit(FDCANx_RX_GPIO_PORT, FDCANx_RX_PIN);
}

/**
  * @brief PCD MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
  GPIO_InitTypeDef GPIO_InitStruct       = {0U};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0U};

  if (hpcd->Instance == USB_OTG_FS)
  {
    /* Initializes the peripherals clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInit.IclkClockSelection   = RCC_CLK48CLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Enable USB GPIO clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* USB_OTG_FS DM DP GPIOs Configuration
       PA12 ------> USB_OTG_FS_DP
       PA11 ------> USB_OTG_FS_DM */
    GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_USB;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_CLK_ENABLE();

    /* Enable VDDUSB */
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      HAL_PWREx_EnableVddUSB();
      __HAL_RCC_PWR_CLK_DISABLE();
    }
    else
    {
      HAL_PWREx_EnableVddUSB();
    }

    /* USB_OTG_FS interrupt Init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 6U, 0U);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  }
}

/**
  * @brief PCD MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
  if (hpcd->Instance == USB_OTG_FS)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USB_CLK_DISABLE();

    /* USB_OTG_FS DM DP GPIOs Configuration
       PA12 ------> USB_OTG_FS_DP
       PA11 ------> USB_OTG_FS_DM */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12 | GPIO_PIN_11);

    /* USB_OTG_FS interrupt DeInit */
    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
  }
}
