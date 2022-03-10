/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "stm32u5xx_ll_rcc.h"
#include "stm32u5xx_ll_bus.h"
#include "stm32u5xx_ll_crs.h"
#include "main.h"
#include "app_threadx.h"
#include "app_openbootloader.h"
#include "interfaces_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;

/**
  * @brief  Main program.
  * @param  None.
  * @retval None.
  */
int main(void)
{
  /* STM32U5xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 160 MHz */
  SystemClock_Config();

  /* Initialize the OpenBootloader */
  OpenBootloader_Init();

  /* Initialize the Threads and start the kernel */
  MX_ThreadX_Init();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 60000000
  *            HCLK(Hz)                       = 60000000
  *            AHB Pre-scaler                 = 1
  *            APB1 Pre-scaler                = 1
  *            APB2 Pre-scaler                = 1
  *            HSE Frequency(Hz)              = 16000000
  *            PLL_M                          = 8
  *            PLL_N                          = 60
  *            PLL_R                          = 2
  *            PLL_Q                          = 6
  *            PLL_P                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale 1 mode
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};

  /* Enable voltage range 1 for frequency above 100 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Switch to SMPS regulator instead of LDO */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    NVIC_SystemReset();
  }

  __HAL_RCC_PWR_CLK_DISABLE();

  /* Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = 8;
  RCC_OscInitStruct.PLL.PLLN            = 60;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  RCC_OscInitStruct.PLL.PLLQ            = 6;
  RCC_OscInitStruct.PLL.PLLP            = 6;
  RCC_OscInitStruct.PLL.PLLFRACN        = 0;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    NVIC_SystemReset();
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
                                      RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    NVIC_SystemReset();
  }

  /* Configures CRS */
  RCC_CRSInitStruct.Prescaler             = RCC_CRS_SYNC_DIV1;
  RCC_CRSInitStruct.Source                = RCC_CRS_SYNC_SOURCE_USB;
  RCC_CRSInitStruct.Polarity              = RCC_CRS_SYNC_POLARITY_RISING;
  RCC_CRSInitStruct.ReloadValue           = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue       = 34;
  RCC_CRSInitStruct.HSI48CalibrationValue = 32;

  HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);
  /* Enable the SYSCFG APB clock */
  __HAL_RCC_CRS_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  This function is used de-initialize the used protocols and their interrupts.
  * @retval None.
  */
void System_DeInit(void)
{
  /* Interfaces de-initialization */
  USARTx_DeInit();
  I2Cx_DeInit();
  SPIx_DeInit();

  HAL_RCC_DeInit();

  /* Disable timer */
  HAL_TIM_Base_DeInit(&htim6);
  HAL_TIM_Base_Stop_IT(&htim6);
  __HAL_RCC_TIM6_CLK_DISABLE();
  HAL_NVIC_DisableIRQ(TIM6_IRQn);

  /* Disable USB interrupt */
  __HAL_RCC_USB_CLK_DISABLE();
  HAL_NVIC_DisableIRQ(OTG_FS_IRQn);

  /* Disable SPI interrupt */
  HAL_NVIC_DisableIRQ(SPIx_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None.
  */
void Error_Handler(void)
{
  while (1)
  {
  }
}

/**
 * @brief  Period elapsed callback in non blocking mode.
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle.
 * @retval None.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
#if !defined(USBPDCORE_LIB_NO_PD)
    USBPD_DPM_TimerCounter();
#endif /* ! USBPDCORE_LIB_NO_PD */
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name.
  * @param  line: assert_param error line source number.
  * @retval None.
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
