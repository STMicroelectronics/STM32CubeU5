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
/* vector 1 inputs and outputs*/
uint32_t input1_modulusSize                                   = 24;
uint8_t input1_1PKA_ECC_ConvToAffine_IN_MODULUS[24]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_X[24]   = {0x18, 0x0D, 0x8B, 0xA2, 0xAE, 0x50, 0x13, 0xF4, 0x98, 0xCA, 0x7E, 0xE6, 0x89, 0x09, 0xD4, 0x16, 0x72, 0x22, 0xA3, 0x3A, 0xD8, 0x08, 0x8A, 0x37};
uint8_t input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_Y[24]   = {0x95, 0x5F, 0x3B, 0xDE, 0xAD, 0x2C, 0x03, 0xAA, 0x12, 0xF2, 0x4B, 0x6E, 0xE5, 0x68, 0x35, 0x46, 0x3B, 0x2F, 0xD8, 0x33, 0x59, 0x42, 0x7D, 0x9B};
uint8_t input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_Z[24]   = {0x16, 0xC8, 0x05, 0xA7, 0x19, 0xB0, 0xBE, 0xC3, 0x80, 0xCC, 0x78, 0x9F, 0x8E, 0xED, 0x49, 0x08, 0x97, 0xAE, 0x78, 0xB7, 0x95, 0x58, 0x30, 0x2B};
uint32_t input1_1PKA_ECC_ConvToAffine_IN_MONTGOMERY_PARAM[24] = {0x00000001, 0x00000000, 0x00000002, 0x00000000, 0x00000002, 0x00000000};
uint8_t output1_X[24]                                         = {0xB3, 0x67, 0xA9, 0x1D, 0x79, 0xD2, 0xFF, 0x8A, 0x18, 0x6E, 0xF6, 0x08, 0x0F, 0x0F, 0x71, 0xEB, 0xA7, 0x5A, 0x9F, 0x2A, 0x80, 0xED, 0xD9, 0x48};
uint8_t output1_Y[24]                                         = {0x6A, 0x86, 0xD5, 0xAA, 0x2D, 0xC3, 0x88, 0x2F, 0xF1, 0x72, 0x69, 0x46, 0x54, 0xC8, 0x93, 0x8B, 0x75, 0xF9, 0x1F, 0x8F, 0x76, 0x37, 0xA6, 0x8D};

uint8_t buffer_x[24];
uint8_t buffer_y[24];

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
  PKA_ECCProjective2AffineInTypeDef in;
  PKA_ECCProjective2AffineOutTypeDef out;

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
  /* Configure LED6, LED7 */
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED7);

  /* Turn LED6, LED7 off */
  BSP_LED_Off(LED6);
  BSP_LED_Off(LED7);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_RNG_Init();
  MX_PKA_Init();
  /* USER CODE BEGIN 2 */
  /* Set input parameters */
  in.modulusSize = input1_modulusSize;
  in.modulus = input1_1PKA_ECC_ConvToAffine_IN_MODULUS;
  in.basePointX = input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_X;
  in.basePointY = input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_Y;
  in.basePointZ = input1_1PKA_ECC_ConvToAffine_IN_INITIAL_POINT_Z;
  in.pMontgomeryParam = input1_1PKA_ECC_ConvToAffine_IN_MONTGOMERY_PARAM;

  /* Set output parameters */
  out.ptX = &buffer_x[0];
  out.ptY = &buffer_y[0];

  /* Start PKA ECC Projective to Affine operation */
  if (HAL_PKA_ECCProjective2Affine(&hpka, &in, 5000) != HAL_OK)
  {
    /* HAL PKA Operation error */
    Error_Handler();
  }
  /* Retrieve computation result */
  HAL_PKA_ECCProjective2Affine_GetResult(&hpka, &out);

  /* Check retrieved result with expected result */
  if ((Buffercmp((uint8_t*)out.ptX, (uint8_t*)output1_X, 24) != 0) ||
      (Buffercmp((uint8_t*)out.ptY, (uint8_t*)output1_Y, 24) != 0))
  {
    /* HAL PKA Operation error */
    Error_Handler();
  }

  /* PKA DeInit */
  HAL_PKA_DeInit(&hpka);

  /* RNG DeInit */
  HAL_RNG_DeInit(&hrng);

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

  /* Switch to SMPS regulator instead of LDO */
  if(HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
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
  __disable_irq();
  /* Turn LED6 on */
  BSP_LED_On(LED6);
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
