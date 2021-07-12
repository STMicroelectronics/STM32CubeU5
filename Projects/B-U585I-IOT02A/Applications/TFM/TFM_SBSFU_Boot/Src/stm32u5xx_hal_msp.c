/**
  ******************************************************************************
  * @file    stm32u5xx_hal_msp.c
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
#include "boot_hal_cfg.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @defgroup RTC_ActiveTamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constant ----------------------------------------------------------*/
/** @defgroup HAL_MSP_Private_Constant
  * @{
  */
const  RCC_OscInitTypeDef RCC_OscInitStruct_RTC =
{
    .OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE,
    .HSEState = 0,
#if defined(RTC_CLOCK_SOURCE_LSE)
    .LSEState = RCC_LSE_ON,
    .LSIState = RCC_LSI_OFF,
#elif defined (RTC_CLOCK_SOURCE_LSI)
    .LSEState = RCC_LSE_OFF,
    .LSIState = RCC_LSI_ON,
#else
#error
#endif /* defined (RTC_CLOCK_SOURCE_LSI) */
    .HSIState = 0,
    .HSICalibrationValue = 0,
    .LSIDiv = 0,
    .MSIState = 0,
    .MSICalibrationValue = 0,
    .MSIClockRange = 0,
    .MSIKClockRange = 0,
    .HSI48State = 0,
    .SHSIState = 0,
    .MSIKState = 0,
    .PLL = {
        .PLLState = RCC_PLL_NONE,
        .PLLSource = 0,
        .PLLM = 0,
        .PLLMBOOST = 0,
        . PLLN = 0,
        .PLLP = 0,
        .PLLQ = 0,
        .PLLR = 0,
        .PLLRGE = 0,
        .PLLFRACN = 0
    }
};

const RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct_RTC =
{
    .PeriphClockSelection = RCC_PERIPHCLK_RTC,
    .PLL2 = {0},
    .PLL3 = {0},
    .Usart1ClockSelection = 0,
    .Usart2ClockSelection = 0,
    .Usart3ClockSelection = 0,
    .Uart4ClockSelection = 0,
    .Uart5ClockSelection = 0,
    .Lpuart1ClockSelection = 0,
    .I2c1ClockSelection = 0,
    .I2c2ClockSelection = 0,
    .I2c3ClockSelection = 0,
    .I2c4ClockSelection = 0,
    .Lptim1ClockSelection = 0,
    .Lptim2ClockSelection = 0,
    .Lptim34ClockSelection = 0,
    .Fdcan1ClockSelection = 0,
    .Mdf1ClockSelection = 0,
    .Adf1ClockSelection = 0,
    .Sai1ClockSelection = 0,
    .Sai2ClockSelection = 0,
    .RngClockSelection = 0,
    .SaesClockSelection = 0,
    .Clk48ClockSelection = 0,
    .SdmmcClockSelection = 0,
    .AdcDacClockSelection = 0,
    .Dac1ClockSelection = 0,
    .OspiClockSelection = 0,
    .Spi1ClockSelection = 0,
    .Spi2ClockSelection = 0,
    .Spi3ClockSelection = 0,
#if defined(RTC_CLOCK_SOURCE_LSE)
    .RTCClockSelection = RCC_RTCCLKSOURCE_LSE,
#elif defined (RTC_CLOCK_SOURCE_LSI)
    .RTCClockSelection = RCC_RTCCLKSOURCE_LSI,
#else
#error
#endif /* RTC_CLOCK_SOURCE_LSE */
};
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_PWR_EnableBkUpAccess();
    /*  not required to be removed */
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
    /* Reset the whole backup domain, RTC included */
    if (HAL_RCC_OscConfig((RCC_OscInitTypeDef *)&RCC_OscInitStruct_RTC) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_RCCEx_PeriphCLKConfig((RCC_PeriphCLKInitTypeDef *)&PeriphClkInitStruct_RTC) != HAL_OK)
    {
        Error_Handler();
    }
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();

    /* Enable RTC APB clock  */
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    HAL_NVIC_SetPriority(TAMP_IRQn, 0x4, 0);
    HAL_NVIC_EnableIRQ(TAMP_IRQn);
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
    /*##-1- Reset peripherals ##################################################*/
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
