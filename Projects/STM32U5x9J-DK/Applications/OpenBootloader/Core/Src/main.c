/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "app_threadx.h"
#include "app_openbootloader.h"
#include "interfaces_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;

/**
  * @brief  Main program.
  * @retval int.
  */
int main(void)
{
  /* STM32U5xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */

  if (HAL_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the System clock to have a frequency of 120 MHz */
  SystemClock_Config();

  /* Initialize the Open Bootloader */
  OpenBootloader_Init();

  /* Initialize the Threads and start the kernel */
  MX_ThreadX_Init();

  /* The code must never reach this point */
  return 0;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Pre-scaler                 = 1
  *            APB1 Pre-scaler                = 1
  *            APB2 Pre-scaler                = 1
  *            HSE Frequency(Hz)              = 16000000
  *            PLL_M                          = 1
  *            PLL_N                          = 15
  *            PLL_R                          = 2
  *            PLL_Q                          = 12
  *            PLL_P                          = 5
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale 3 mode
  *            Flash Latency(WS)              = 2
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0U};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0U};

  /* Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure */

  /* HSE Oscillator enabled at reset (16Mhz), activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 1U;
  RCC_OscInitStruct.PLL.PLLN       = 15U;
  RCC_OscInitStruct.PLL.PLLR       = 2U;
  RCC_OscInitStruct.PLL.PLLQ       = 12U;
  RCC_OscInitStruct.PLL.PLLP       = 5U;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
                                      | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the G port */
  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->SVMCR, PWR_SVMCR_IO2SV);
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief  This function is used de-initialize the used protocols and their interrupts.
  * @retval None.
  */
void System_DeInit(void)
{
  /* Interfaces de-initialization */
  (void)USARTx_DEINIT();
  (void)I2Cx_DEINIT();
  (void)SPIx_DEINIT();

  (void)HAL_RCC_DeInit();

  /* Disable timer */
  (void)HAL_TIM_Base_DeInit(&htim6);
  (void)HAL_TIM_Base_Stop_IT(&htim6);
  __HAL_RCC_TIM6_CLK_DISABLE();
  HAL_NVIC_DisableIRQ(TIM6_IRQn);

  /* Disable USB interrupt */
  HAL_NVIC_DisableIRQ(OTG_HS_IRQn);

  /* Disable SPI interrupt */
  HAL_NVIC_DisableIRQ(SPIx_IRQ);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None.
  */
void Error_Handler(void)
{
  /* Handle the error by performing a System Reset.
     The user can add his own error handler.
  */
  NVIC_SystemReset();
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

#ifdef USE_FULL_ASSERT
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
  while (true)
  {
  }
}
#endif /* USE_FULL_ASSERT */
