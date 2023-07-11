/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u5g9j_discovery.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

GFXTIM_HandleTypeDef hgfxtim;

/* USER CODE BEGIN PV */
__IO uint32_t Time1Result = 0;
__IO uint32_t Time2Result = 0;
uint32_t tickstart;
static __IO uint32_t CL1Tick = 0;
static __IO uint32_t CL2Tick = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GFXTIM_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GFXTIM_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  HAL_GFXTIM_AbsoluteTimer_Start(&hgfxtim);
  tickstart = HAL_GetTick();
  while (((HAL_GetTick() - tickstart) < 1520) && (CL1Tick == 0))
  {
  }
  if (CL1Tick - tickstart == 1500)
  {
    Time1Result = 1;
  }

  while (((HAL_GetTick() - tickstart) < 3020) && (CL2Tick == 0))
  {
  }
  if (CL2Tick - tickstart == 3000)
  {
    Time2Result = 1;
  }

  HAL_GFXTIM_AbsoluteTimer_Stop(&hgfxtim);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if ((Time1Result != 0) && (Time2Result != 0))
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

  /** Initializes the CPU, AHB and APB buses clocks
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
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/**
  * @brief GFXTIM Initialization Function
  * @param None
  * @retval None
  */
static void MX_GFXTIM_Init(void)
{

  /* USER CODE BEGIN GFXTIM_Init 0 */

  /* USER CODE END GFXTIM_Init 0 */

  GFXTIM_ClockGeneratorConfigTypeDef pClockGeneratorConfig = {0};
  GFXTIM_AbsoluteTimerConfigTypeDef pAbsoluteTimerConfig = {0};

  /* USER CODE BEGIN GFXTIM_Init 1 */

  /* USER CODE END GFXTIM_Init 1 */
  hgfxtim.Instance = GFXTIM;
  hgfxtim.Init.SynchroSrc = GFXTIM_SYNC_SRC_HSYNC_VSYNC_0;
  if (HAL_GFXTIM_Init(&hgfxtim) != HAL_OK)
  {
    Error_Handler();
  }
  pClockGeneratorConfig.LCCHwReloadSrc = GFXTIM_LCC_HW_RELOAD_SRC_NONE;
  pClockGeneratorConfig.LCCReloadValue = 159999;
  pClockGeneratorConfig.LCCClockSrc = GFXTIM_LCC_CLK_SRC_SYSCLOCK;
  pClockGeneratorConfig.LineClockSrc = GFXTIM_LINE_CLK_SRC_LCC_UNDERFLOW;
  pClockGeneratorConfig.FrameClockSrc = GFXTIM_FRAME_CLK_SRC_FCC_UNDERFLOW;
  pClockGeneratorConfig.LineClockCalib = GFXTIM_LINE_CLK_CALIB_DISABLE;
  pClockGeneratorConfig.FrameClockCalib = GFXTIM_FRAME_CLK_CALIB_DISABLE;
  if (HAL_GFXTIM_ClockGenerator_Config(&hgfxtim, &pClockGeneratorConfig) != HAL_OK)
  {
    Error_Handler();
  }
  pAbsoluteTimerConfig.FrameCompare1Value = 0;
  pAbsoluteTimerConfig.LineCompare1Value = 1500;
  pAbsoluteTimerConfig.LineCompare2Value = 3000;
  pAbsoluteTimerConfig.FrameCounterValue = 0;
  pAbsoluteTimerConfig.LineCounterValue = 0;
  pAbsoluteTimerConfig.FrameOverflowInterrupt = DISABLE;
  pAbsoluteTimerConfig.FrameCompare1Interrupt = DISABLE;
  pAbsoluteTimerConfig.LineOverflowInterrupt = DISABLE;
  pAbsoluteTimerConfig.LineCompare1Interrupt = ENABLE;
  pAbsoluteTimerConfig.LineCompare2Interrupt = ENABLE;
  if (HAL_GFXTIM_AbsoluteTimer_Config(&hgfxtim, &pAbsoluteTimerConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GFXTIM_Init 2 */

  /* USER CODE END GFXTIM_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_RED_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GFXTIM_AbsoluteTimer_ALCC1Callback(GFXTIM_HandleTypeDef *hgfxtim)
{
  CL1Tick = HAL_GetTick();
}

void HAL_GFXTIM_AbsoluteTimer_ALCC2Callback(GFXTIM_HandleTypeDef *hgfxtim)
{
  CL2Tick = HAL_GetTick();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  HAL_GPIO_WritePin(GPIOD, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
