/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
/* USER CODE BEGIN PV */
TX_THREAD MainThread;
TX_SEMAPHORE Semaphore;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
//void ThreadOne_Entry(ULONG thread_input);
void MainThread_Entry(ULONG thread_input);
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

  /* USER CODE BEGIN App_ThreadX_Init */
#if (USE_MEMORY_POOL_ALLOCATION == 1)
  CHAR *pointer;
  
  /* Allocate the stack for MainThread.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create MainThread.  */
  if (tx_thread_create(&MainThread, "Main Thread", MainThread_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       MAIN_THREAD_PRIO, MAIN_THREAD_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }
  
  /* Create the semaphore.  */
  if (tx_semaphore_create(&Semaphore, "Semaphore", 0) != TX_SUCCESS)
  {
    ret = TX_GROUP_ERROR;
  }
  
#endif
  /* USER CODE END App_ThreadX_Init */

  return ret;
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
void App_ThreadX_LowPower_Enter()
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
void App_ThreadX_LowPower_Exit()
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */
  Exit_LowPower_Mode();
  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Restore system clock after wake-up from STOP: enable MSI, PLL
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
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    /* Add additional checks to avoid multiple semaphore puts by successively 
    clicking on the user button */
    tx_semaphore_info_get(&Semaphore, NULL, &currentValue, NULL, NULL, NULL);
    if (currentValue == 0)
    {
      /* Put the semaphore to release the MainThread */
      tx_semaphore_put(&Semaphore);
    }
  }
}

/**
  * @brief  Function implementing the MainThread thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  (void) thread_input;
  UINT i = 0;
  /* Infinite loop */
  while (1)
  {
    if (tx_semaphore_get(&Semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      for (i=0; i<10; i++)
      {
      /* Toggle LED to indicate status*/
      BSP_LED_Toggle(LED_GREEN);
      App_Delay(50);
      }
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
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
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

#ifdef Rev_2_1
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable ultra low power mode mode in
  Stop 1, Stop 2, Stop 3 and Standby modes
  and when the regulator is in Range 4 (Run, Sleep or Stop 0 mode) */
  HAL_PWREx_EnableUltraLowPowerMode();
#endif /* Rev_2_1 */

  /* Enable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  
  /* Set parameters to be configured */
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = GPIO_PIN_ALL;
  
  /* Initialize all GPIO pins */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  
  /* Disable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
  
  /* Configure user button as wake up source */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
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
  
  /* Enter to the stop mode */
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
  /* Initialize LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
}
/* USER CODE END 1 */






