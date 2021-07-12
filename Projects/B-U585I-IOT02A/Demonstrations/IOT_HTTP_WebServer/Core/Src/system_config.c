/**
  **********************************************************************************************************************
  * @file    system_config.c
  * @author  MCD Application Team
  * @brief   This file implements the system configuration services
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
#include "system_config.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
#define PERIOD_VALUE 0x667U
#define PULSE_VALUE  PERIOD_VALUE/2U

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
LPTIM_HandleTypeDef LPTIMHandle;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static WebServer_StatusTypeDef lptim_config(void);
static void LPTIM_UpdateEventCallback(LPTIM_HandleTypeDef *hlptim);

/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  Initialize the system application
  * @param  None
  * @retval Web Server status
  */
WebServer_StatusTypeDef system_init(void)
{
  /* Configure the console */
  if (webserver_console_config() != WEBSERVER_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the LPTIM peripheral */
  if (lptim_config() != WEBSERVER_OK)
  {
    return SYSTEM_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  Enable ICACHE with 1-way set-associative configuration.
  * @param  None
  * @retval None
  */
void instruction_cache_enable(void)
{
  /* Configure ICACHE associativity mode */
  HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);

  /* Enable ICACHE */
  HAL_ICACHE_Enable();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_MBOOST                     = 4
  *            PLL_M                          = 3
  *            PLL_N                          = 15
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval Web Server status
  */
WebServer_StatusTypeDef system_clock_config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Configure the main internal regulator output voltage */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.LSIState            = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_0;
  RCC_OscInitStruct.LSIDiv              = RCC_LSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST       = RCC_PLLMBOOST_DIV4;
  RCC_OscInitStruct.PLL.PLLM            = 3;
  RCC_OscInitStruct.PLL.PLLN            = 15;
  RCC_OscInitStruct.PLL.PLLP            = 2;
  RCC_OscInitStruct.PLL.PLLQ            = 2;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN        = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                     RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the Systick */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  return WEBSERVER_OK;
}

/**
  * @brief  Initialize the LPTIM peripheral
  * @param  None
  * @retval Web Server status
  */
static WebServer_StatusTypeDef lptim_config(void)
{
  /* Channel_config structure declaration */
  LPTIM_OC_ConfigTypeDef sConfig;

  /*
   *  Instance        = LPTIM1
   *  Clock Source    = APB or LowPowerOSCillator (in this example LSE is
   *                    already selected from the RCC stage)
   *  Counter source  = External event.
   *  Clock prescaler = 1 (No division)
   *  Counter Trigger = Software trigger
   *  Output Polarity = High
   *  Update mode     = Immediate (Registers are immediately updated after any
   *                    write access)
   */
  LPTIMHandle.Instance = LPTIM1;

  LPTIMHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIMHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIMHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIMHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIMHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIMHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIMHandle.Init.Period                        = PERIOD_VALUE;
  LPTIMHandle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LPTIMHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Register LPTIM callback */
  if (HAL_LPTIM_RegisterCallback(&LPTIMHandle, HAL_LPTIM_UPDATE_EVENT_CB_ID, LPTIM_UpdateEventCallback) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /*
  *  Period = 99
  *  Pulse  = 49
  *  According to this configuration, the duty cycle will be equal to 50%
  */
  sConfig.Pulse      = PULSE_VALUE;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_LOW;
  if (HAL_LPTIM_OC_ConfigChannel(&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Start the LPTIM PWM */
  if (HAL_LPTIM_PWM_Start_IT(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  Update event callback in non-blocking mode.
  * @param  hlptim : Pointer to LPTIM handle
  * @retval None
  */
static void LPTIM_UpdateEventCallback(LPTIM_HandleTypeDef *hlptim)
{
  /* Toggle GREEN led */
  BSP_LED_Toggle(LED_GREEN);
}
