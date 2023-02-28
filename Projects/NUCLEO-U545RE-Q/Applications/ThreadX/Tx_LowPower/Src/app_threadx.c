/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_azure_rtos_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DUMMY_VALUE (0xFFFFFFFFU)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
  TX_THREAD tx_app_thread;
  TX_SEMAPHORE tx_app_semaphore;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void SystemClock_Restore(void);
void Enter_LowPower_Mode(void);
void Exit_LowPower_Mode(void);
static VOID App_Delay(ULONG Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
CHAR *pointer;

  /* Allocate the stack for Main Thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
   /* Create Main Thread.  */
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create Semaphore.  */
  if (tx_semaphore_create(&tx_app_semaphore, "Semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the MainThread_Entry thread.
  * @param  thread_input: Not used.
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_Entry */
(void) thread_input;
  UINT i = 0;
  /* Infinite loop */
  while (1)
  {
    if (tx_semaphore_get(&tx_app_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      for (i=0; i<10; i++)
      {
      /* Toggle LED to indicate status*/
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      App_Delay(50);
      }
    }
  }
  /* USER CODE END MainThread_Entry */
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/**
  * @brief  App_ThreadX_LowPower_Enter
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Enter(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Enter */
  Enter_LowPower_Mode();
  /* USER CODE END  App_ThreadX_LowPower_Enter */
}

/**
  * @brief  App_ThreadX_LowPower_Exit
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Exit(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */
  Exit_LowPower_Mode();
  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Restore system clock after wake-up from STOP2: enable MSI, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SystemClock_Restore(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Deinitialize the RCC */
  HAL_RCC_DeInit();

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  ULONG currentValue = 0;
  if (GPIO_Pin == BUTTON_USER_Pin)
  {
    /* Add additional checks to avoid multiple semaphore puts by successively
    clicking on the user button */
    tx_semaphore_info_get(&tx_app_semaphore, NULL, &currentValue, NULL, NULL, NULL);
    if (currentValue == 0)
    {
      /* Put the semaphore to release the MainThread */
      tx_semaphore_put(&tx_app_semaphore);
    }
  }
}

/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(ULONG Delay)
{
  ULONG initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}

/**
  * @brief  Enter LowPower Mode configuration
  * @param  None
  * @retval None
  */
void Enter_LowPower_Mode(void)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable only 8KB SRAM2 and icache full retention in Stop2 mode */
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
  HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
  HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE1_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_PAGE2_STOP_RETENTION);

  /* Switch to SMPS */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configuration of the LPM read mode */
  if (HAL_FLASHEx_ConfigLowPowerRead(FLASH_LPM_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set all GPIO in analog state to reduce power consumption,                */
  /*   except GPIOC to keep user button interrupt enabled                     */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  /* Enable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Set parameters to be configured */
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = GPIO_PIN_ALL;

  /* Initialize all GPIO pins */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* Disable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();

  /* Disable all unused peripheral clocks */
  MODIFY_REG(RCC->AHB1ENR, DUMMY_VALUE, RCC_AHB1ENR_FLASHEN);
  MODIFY_REG(RCC->AHB2ENR1, DUMMY_VALUE, RCC_AHB2ENR1_SRAM2EN);
  MODIFY_REG(RCC->AHB2ENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB3ENR, DUMMY_VALUE, RCC_AHB3ENR_PWREN);
  MODIFY_REG(RCC->APB1ENR1, DUMMY_VALUE, RCC_APB1ENR1_TIM6EN);
  MODIFY_REG(RCC->APB1ENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB2ENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB3ENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB1SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB2SMENR1, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB2SMENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB3SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB1SMENR1, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB1SMENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB2SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB3SMENR, DUMMY_VALUE, 0U);

  /* Disable AHB APB Peripheral Clock */
  __HAL_RCC_AHB1_CLK_DISABLE();
  __HAL_RCC_AHB2_1_CLK_DISABLE();
  __HAL_RCC_AHB2_2_CLK_DISABLE();

  __HAL_RCC_APB1_CLK_DISABLE();
  __HAL_RCC_APB2_CLK_DISABLE();

  /* Enter to the stop2 mode */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

/**
  * @brief  Exit LowPower Mode configuration
  * @param  None
  * @retval None
  */
void Exit_LowPower_Mode(void)
{
  /* Enable AHB APB Peripheral Clock */
  __HAL_RCC_AHB1_CLK_ENABLE();
  __HAL_RCC_AHB2_1_CLK_ENABLE();
  __HAL_RCC_AHB2_2_CLK_ENABLE();

  __HAL_RCC_APB1_CLK_ENABLE();
  __HAL_RCC_APB2_CLK_ENABLE();

  /* Reconfigure the system clock*/
  SystemClock_Restore();

  /**/
  MX_GPIO_Init();
}
/* USER CODE END 1 */
