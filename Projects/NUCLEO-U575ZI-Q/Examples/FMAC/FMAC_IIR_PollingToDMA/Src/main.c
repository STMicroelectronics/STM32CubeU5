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

FMAC_HandleTypeDef hfmac;
DMA_HandleTypeDef handle_GPDMA1_Channel1;

/* USER CODE BEGIN PV */
/* FMAC configuration structure */
FMAC_FilterConfigTypeDef sFmacConfig;

/* Array of filter coefficients A (feedback coefficients) in Q1.15 format */
static int16_t aFilterCoeffA[COEFF_VECTOR_A_SIZE] =
{
   10432,-17296, 17696,-12512,  5536, -1296
};

/* Array of filter coefficients B (feed-forward taps) in Q1.15 format */
static int16_t aFilterCoeffB[COEFF_VECTOR_B_SIZE] =
{
     828,  1170,  2360,  2293,  2360,  1170,   828
};

/* Array of input values in Q1.15 format */
static int16_t aInputValues[ARRAY_SIZE] =
{
       0,   660,  -194,  1731,     1,  2194,   725,  2018,  1775,  1501,
    2703,  1085,  3072,  1084,  2701,  1499,  1772,  2014,   721,  2189,
      -5,  1725,  -200,   653,    -6,  -666,   187, -1737,    -7, -2199,
    -730, -2022, -1778, -1504, -2705, -1086, -3072, -1083, -2700, -1496,
   -1768, -2010,  -716, -2184,    10, -1719,   206,  -647,    13,   672,
    -181,  1742,    12,  2204,   734,  2026,  1781,  1506,  2706,  1086,
    3072,  1082,  2698,  1494,  1765,  2006,   712,  2179,   -16,  1713,
    -212,   640,   -19,  -679,   175, -1748,   -18, -2209,  -739, -2030,
   -1784, -1509, -2708, -1087, -3072, -1081, -2696, -1491, -1762, -2002,
    -707, -2173,    21, -1707,   218,  -634,    26,   685,  -169,  1754,
      23,  2214,   743,  2033,  1787,  1511,  2710,  1088,  3072,  1080,
    2695,  1489,  1758,  1998,   702,  2168,   -27,  1701,  -225,   628,
     -32,  -691,   163, -1760,   -29, -2219,  -748, -2037, -1791, -1513,
   -2711, -1089, -3072, -1080, -2693, -1486, -1755, -1994,  -698, -2163,
      33, -1695,   231,  -621,    39,   698,  -156,  1766,    34,  2224,
     752,  2041,  1794,  1516,  2713,  1089,  3072,  1079,  2691,  1484,
    1752,  1990,   693,  2158,   -38,  1689,  -237,   615,   -45,  -704,
     150, -1772,   -40, -2229,  -757, -2045, -1797, -1518, -2714, -1090,
   -3071, -1078, -2689, -1481, -1749, -1986,  -688, -2153,    44, -1683,
     243,  -608,    52,   710,  -144,  1778,    45,  2234,   761,  2049,
    1800,  1520,  2716,  1091,  3071,  1077,  2687,  1478,  1745,  1982,
     684,  2148,   -50,  1677,  -250,   602,   -58,  -717,   138, -1784,
     -51, -2239,  -766, -2052, -1803, -1523, -2717, -1092, -3071, -1076,
   -2686, -1476, -1742, -1978,  -679, -2142,    55, -1671,   256,  -596,
      64,   723,  -132,  1790,    56,  2244,   770,  2056,  1806,  1525,
    2719,  1092,  3071,  1075,  2684,  1473
};

/* Array of output data to preload in Q1.15 format */
static int16_t aOutputDataToPreload[COEFF_VECTOR_A_SIZE] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

/* Array of calculated filtered data in Q1.15 format */
static int16_t aCalculatedFilteredData[ARRAY_SIZE];

/* Expected number of calculated samples */
uint16_t ExpectedCalculatedOutputSize = MEMORY_PARAMETER_D2;

/* Status of the FMAC callbacks */
__IO uint32_t HalfOutputDataReadyCallback = CALLBACK_NOT_CALLED;
__IO uint32_t OutputDataReadyCallback     = CALLBACK_NOT_CALLED;
__IO uint32_t ErrorCount                  = 0;

/* Array of reference filtered data for IIR "7 feed-forward taps, 6 feedback coefficients, gain = 1" in Q1.15 format */
static const int16_t aRefFilteredData[ARRAY_SIZE] =
{
  0x01c6, 0x0403, 0x03a5, 0x03e6, 0x05b0, 0x0588, 0x051d, 0x0664, 0x06c7, 0x057a,
  0x0631, 0x06c8, 0x0539, 0x0500, 0x05a5, 0x0453, 0x02f5, 0x03b2, 0x02a0, 0x00b3,
  0x00fe, 0x008c, 0xfe75, 0xfe03, 0xfe6e, 0xfc64, 0xfb84, 0xfc4c, 0xfb0a, 0xf9ba,
  0xfa9c, 0xfa87, 0xf8df, 0xf9d2, 0xfaa0, 0xf95d, 0xf9da, 0xfb7d, 0xfaf9, 0xfac9,
  0xfd0d, 0xfd3b, 0xfcc5, 0xfecf, 0xfffd, 0xff53, 0x00ae, 0x02be, 0x01f7, 0x029c,
  0x04cb, 0x0487, 0x041b, 0x0609, 0x066e, 0x0504, 0x0663, 0x0722, 0x0576, 0x0598,
  0x06be, 0x0521, 0x03f7, 0x0544, 0x03f1, 0x01fe, 0x02b2, 0x0246, 0xffbf, 0xffb1,
  0x0031, 0xfd99, 0xfcdc, 0xfdc8, 0xfc12, 0xfa6f, 0xfbac, 0xfb1e, 0xf8f9, 0xfa31,
  0xfaba, 0xf8e4, 0xf963, 0xfb2a, 0xf9ea, 0xf9ac, 0xfc34, 0xfbdb, 0xfb28, 0xfd85,
  0xfe95, 0xfd63, 0xff3a, 0x0162, 0x0027, 0x011e, 0x03b3, 0x032b, 0x02bf, 0x056e,
  0x05a2, 0x0426, 0x0638, 0x071b, 0x0536, 0x05d6, 0x0788, 0x057c, 0x04b2, 0x0696,
  0x04fd, 0x0307, 0x0458, 0x03e4, 0x00e7, 0x0179, 0x0201, 0xfede, 0xfe5d, 0xff91,
  0xfd45, 0xfb6b, 0xfd2c, 0xfbff, 0xf96e, 0xfb01, 0xfb45, 0xf8b6, 0xf963, 0xfb49,
  0xf91d, 0xf8f0, 0xfbb9, 0xfabb, 0xf9b5, 0xfc8b, 0xfd49, 0xfb76, 0xfdeb, 0x0004,
  0xfe37, 0xff85, 0x028d, 0x017c, 0x0122, 0x04a4, 0x046f, 0x02e1, 0x05bb, 0x06b8,
  0x046c, 0x05c4, 0x07f7, 0x0556, 0x0516, 0x07a2, 0x05ae, 0x03b5, 0x05e6, 0x0542,
  0x01d6, 0x0342, 0x03c8, 0x0010, 0xfff2, 0x0195, 0xfe7f, 0xfca1, 0xff07, 0xfd19,
  0xfa2a, 0xfc41, 0xfc38, 0xf8c9, 0xf9e7, 0xfbd7, 0xf89e, 0xf8a2, 0xfbb2, 0xf9e3,
  0xf881, 0xfc01, 0xfc27, 0xf9a9, 0xfcd9, 0xfec2, 0xfc37, 0xfdf0, 0x0173, 0xff87,
  0xff61, 0x03bb, 0x02e9, 0x0141, 0x0506, 0x05fc, 0x031a, 0x056e, 0x0801, 0x04ab,
  0x051b, 0x0864, 0x05e5, 0x03f9, 0x074f, 0x0640, 0x0277, 0x04f3, 0x0570, 0x0109,
  0x018d, 0x03b9, 0xff9d, 0xfe01, 0x0128, 0xfe56, 0xfb14, 0xfdf5, 0xfd7c, 0xf90e,
  0xfaf7, 0xfccf, 0xf864, 0xf8c4, 0xfc36, 0xf94a, 0xf79e, 0xfc01, 0xfb37, 0xf810,
  0xfc24, 0xfdb8, 0xfa2e, 0xfc8d, 0x0081, 0xfd5b, 0xfd9c, 0x02d3, 0x0120, 0xff50,
  0x043e, 0x04e9, 0x0148, 0x04e8, 0x07af, 0x0377, 0x04c1, 0x08e7, 0x058c, 0x03d3,
  0x088c, 0x06c6, 0x02b3, 0x0682, 0x06e5, 0x019d
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_FMAC_Init(void);
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
  /* Configure LED1 */
  BSP_LED_Init(LED1);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_GPDMA1_Init();
  MX_FMAC_Init();
  /* USER CODE BEGIN 2 */
/*## Configure the FMAC peripheral ###########################################*/
  sFmacConfig.InputBaseAddress  = INPUT_BUFFER_BASE;
  sFmacConfig.InputBufferSize   = INPUT_BUFFER_SIZE;
  sFmacConfig.InputThreshold    = INPUT_THRESHOLD;
  sFmacConfig.CoeffBaseAddress  = COEFFICIENT_BUFFER_BASE;
  sFmacConfig.CoeffBufferSize   = COEFFICIENT_BUFFER_SIZE;
  sFmacConfig.OutputBaseAddress = OUTPUT_BUFFER_BASE;
  sFmacConfig.OutputBufferSize  = OUTPUT_BUFFER_SIZE;
  sFmacConfig.OutputThreshold   = OUTPUT_THRESHOLD;
  sFmacConfig.pCoeffA           = aFilterCoeffA;
  sFmacConfig.CoeffASize        = COEFF_VECTOR_A_SIZE;
  sFmacConfig.pCoeffB           = aFilterCoeffB;
  sFmacConfig.CoeffBSize        = COEFF_VECTOR_B_SIZE;
  sFmacConfig.Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1;
  sFmacConfig.InputAccess       = FMAC_BUFFER_ACCESS_POLLING;
  sFmacConfig.OutputAccess      = FMAC_BUFFER_ACCESS_DMA;
  sFmacConfig.Clip              = FMAC_CLIP_DISABLED;
  sFmacConfig.P                 = COEFF_VECTOR_B_SIZE;
  sFmacConfig.Q                 = COEFF_VECTOR_A_SIZE;
  sFmacConfig.R                 = GAIN;

  if (HAL_FMAC_FilterConfig(&hfmac, &sFmacConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*## Preload the input and output buffers ####################################*/
  if (HAL_FMAC_FilterPreload(&hfmac, aInputValues,         INPUT_BUFFER_SIZE,
                             aOutputDataToPreload, COEFF_VECTOR_A_SIZE) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*## Start calculation of IIR filter in polling/DMA mode #####################*/
  if (HAL_FMAC_FilterStart(&hfmac, aCalculatedFilteredData, &ExpectedCalculatedOutputSize) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Wait for the end of the handling (no new data written) #################*/
  /*  For simplicity reasons, this example is just waiting till the end of the
  calculation, but the application may perform other tasks while the operation
  is ongoing. */
  while(HalfOutputDataReadyCallback == CALLBACK_NOT_CALLED)
  {
  }
  while(OutputDataReadyCallback == CALLBACK_NOT_CALLED)
  {
  }

  /*## Stop the calculation of IIR filter in polling/DMA mode ##################*/
  if (HAL_FMAC_FilterStop(&hfmac) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Check the final error status ############################################*/
  if(ErrorCount != 0)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Compare FMAC results to the reference values #####################*/
  for (uint32_t i = 0; i < ExpectedCalculatedOutputSize; i++)
  {
    if (aCalculatedFilteredData[i]  != aRefFilteredData[i])
    {
      /* Processing Error */
      Error_Handler();
    }
  }

  /* There is no error in the output values: Turn LED1 on */
  BSP_LED_On(LED1);
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
  * @brief FMAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_FMAC_Init(void)
{

  /* USER CODE BEGIN FMAC_Init 0 */

  /* USER CODE END FMAC_Init 0 */

  /* USER CODE BEGIN FMAC_Init 1 */

  /* USER CODE END FMAC_Init 1 */
  hfmac.Instance = FMAC;
  if (HAL_FMAC_Init(&hfmac) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FMAC_Init 2 */

  /* USER CODE END FMAC_Init 2 */

}

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

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

/* USER CODE BEGIN 4 */
/**
  * @brief FMAC half output data ready callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_HalfOutputDataReadyCallback(FMAC_HandleTypeDef *hfmac)
{
  HalfOutputDataReadyCallback = CALLBACK_CALLED;
}

/**
  * @brief FMAC output data ready callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_OutputDataReadyCallback(FMAC_HandleTypeDef *hfmac)
{
  OutputDataReadyCallback = CALLBACK_CALLED;
}

/**
  * @brief FMAC error callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_ErrorCallback(FMAC_HandleTypeDef *hfmac)
{
  ErrorCount++;
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
  /* LED1 is blinking */
  BSP_LED_Toggle(LED1);
  HAL_Delay(500);
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
