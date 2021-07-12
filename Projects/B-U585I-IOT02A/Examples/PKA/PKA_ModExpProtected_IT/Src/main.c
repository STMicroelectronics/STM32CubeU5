/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

PKA_HandleTypeDef hpka;

RNG_HandleTypeDef hrng;

/* USER CODE BEGIN PV */
PKA_ModExpProtectModeInTypeDef in = {0};

/* Input vectors */
uint32_t input1_OpSize  = 32;
uint32_t input1_ExpSize = 32;
uint8_t input1_1[32]    = {0xE4, 0x88, 0xD8, 0x11, 0x76, 0xE5, 0x06, 0xFA, 0xB7, 0xC2, 0xFC, 0x5D, 0xF3, 0xCB, 0x75, 0x55, 0x34, 0x3F, 0x45, 0xB4, 0x78, 0x52, 0xBA, 0x7E, 0xFB, 0xF5, 0xB1, 0x2A, 0xF4, 0x0B, 0xF4, 0xB3};
uint8_t input1_2[32]    = {0x12, 0x28, 0x49, 0x3A, 0x81, 0xFE, 0xCA, 0x62, 0x2B, 0x2D, 0x83, 0xCD, 0x97, 0x2C, 0x28, 0x23, 0x39, 0x76, 0xF1, 0xF3, 0x71, 0xCE, 0x16, 0x84, 0xA0, 0x37, 0x98, 0xE0, 0xC7, 0x0B, 0xF4, 0x39};
uint8_t input1_3[32]    = {0x45, 0xAB, 0x14, 0x95, 0x48, 0x22, 0x69, 0xC4, 0x8F, 0x1E, 0xCA, 0x23, 0x0C, 0x1F, 0x5A, 0xB4, 0xBC, 0xE7, 0x12, 0xD8, 0x50, 0x09, 0x54, 0xBB, 0xE2, 0x1D, 0x2A, 0x39, 0x86, 0x3E, 0xDB, 0xFB};
uint8_t input1_4[32]    = {0xE4, 0x88, 0xD8, 0x11, 0x76, 0xE5, 0x06, 0xFA, 0xB7, 0xC2, 0xFC, 0x5D, 0xF3, 0xCB, 0x75, 0x53, 0x50, 0x7D, 0xA2, 0xCD, 0x98, 0x24, 0x33, 0x76, 0x14, 0x8F, 0xCF, 0xA3, 0xF5, 0xCF, 0x4A, 0x88};
uint8_t output1[32]     = {0xD1, 0x2F, 0x36, 0x6C, 0x61, 0xB5, 0x66, 0x48, 0x61, 0x17, 0x8A, 0x1E, 0x6B, 0xD0, 0xE7, 0xBF, 0x66, 0x0A, 0x2B, 0x07, 0x9D, 0x4D, 0x82, 0x68, 0x7E, 0xB1, 0x9D, 0x29, 0x94, 0x43, 0x6D, 0x39};

uint8_t buffer[32];

__IO uint32_t hal_EndOfProcess = 0;
__IO uint32_t hal_ErrorCallback = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_RNG_Init(void);
static void MX_PKA_Init(void);
/* USER CODE BEGIN PFP */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
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
  /* STM32U5xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Configure LED6 */
  BSP_LED_Init(LED6);
  /* Configure LED7 */
  BSP_LED_Init(LED7);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_RNG_Init();
  MX_PKA_Init();
  /* USER CODE BEGIN 2 */

  /* Set input parameters */
  in.OpSize  = input1_OpSize;
  in.expSize = input1_ExpSize;
  in.pMod    = input1_1;
  in.pExp    = input1_2;
  in.pOp1    = input1_3;
  in.pPhi    = input1_4;

  /* Start PKA protected Modular exponentiation operation */
  if (HAL_PKA_ModExpProtectMode_IT(&hpka, &in) != HAL_OK)
  {
    /* HAL PKA Operation error */
    Error_Handler();
  }

  /* Wait until operation finish */
  while (!hal_EndOfProcess);
  hal_EndOfProcess = 0;

  /* Retrieve computation result */
  HAL_PKA_ModExp_GetResult(&hpka, buffer);

  /* Check retrieved result with expected result */
  if ((Buffercmp((uint8_t*)buffer, (uint8_t*)output1, 32) != 0) || (hal_ErrorCallback == 1))
  {
    /* HAL PKA Operation error */
    Error_Handler();
  }

    /* Turn LED7 on */
    BSP_LED_On(LED7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
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
  * @brief PKA Initialization Function
  * @param None
  * @retval None
  */
static void MX_PKA_Init(void)
{

  /* USER CODE BEGIN PKA_Init 0 */

  /* USER CODE END PKA_Init 0 */

  /* USER CODE BEGIN PKA_Init 1 */

  /* USER CODE END PKA_Init 1 */
  hpka.Instance = PKA;
  if (HAL_PKA_Init(&hpka) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN PKA_Init 2 */

  /* USER CODE END PKA_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Process completed callback.
  * @param  hpka PKA handle
  * @retval None
  */
void HAL_PKA_OperationCpltCallback(PKA_HandleTypeDef *hpka)
{
  hal_EndOfProcess = 1;
}

/**
  * @brief  Error callback.
  * @param  hpka PKA handle
  * @retval None
  */
void HAL_PKA_ErrorCallback(PKA_HandleTypeDef *hpka)
{
  hal_ErrorCallback = 1;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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
  /* Turn LED6 on */
  BSP_LED_On(LED6);
  __disable_irq();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
