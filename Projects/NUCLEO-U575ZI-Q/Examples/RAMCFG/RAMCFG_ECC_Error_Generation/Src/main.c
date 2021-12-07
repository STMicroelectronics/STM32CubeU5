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
/* Single and Double ECC error flags declaration */
__IO uint32_t ECC_SingleError = 0U, ECC_DoubleError = 0U;

/* Single and double error data buffers */
uint32_t SingleErrorData_Buffer[SRAM3_SIGNLE_ERROR_ADDRESS_SIZE];
uint32_t DoubleErrorData_Buffer[SRAM3_DOUBLE_ERROR_ADDRESS_SIZE];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

RAMCFG_HandleTypeDef hramcfg_SRAM3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_RAMCFG_Init(void);
/* USER CODE BEGIN PFP */
static void Fill_wMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data);
static void Read_wMemory(uint32_t *pMemory, uint32_t *pBuffer, uint16_t BufferSize);
static BufferContentStatus Check_wBuffer(uint32_t *pBuffer, uint16_t BufferSize, uint32_t CompareData);

static void SingleErrorDetectionCallback(RAMCFG_HandleTypeDef *hramcfg);
static void DoubleErrorDetectionCallback(RAMCFG_HandleTypeDef *hramcfg);

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
  /* Initialize LED1 and LED3 : GREEN and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_RAMCFG_Init();
  /* USER CODE BEGIN 2 */

    /* Register MSP Init callback for RAMCFG peripheral */
  if (HAL_RAMCFG_RegisterCallback(&hramcfg_SRAM3, HAL_RAMCFG_MSPINIT_CB_ID, HAL_RAMCFG_MspInit) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register MSP Init callback for RAMCFG peripheral */
  if (HAL_RAMCFG_RegisterCallback(&hramcfg_SRAM3, HAL_RAMCFG_MSPDEINIT_CB_ID, HAL_RAMCFG_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Erase the the hole SRAM3 */
  if (HAL_RAMCFG_Erase(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }


  /*##-2- Generate single and double ECC errors ##############################*/
  /* Start ECC for SRAM3 */
  if (HAL_RAMCFG_StartECC(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }

  /* Fill memory with known data */
  Fill_wMemory((uint32_t *)SRAM3_SIGNLE_ERROR_ADDRESS_BASE, (SRAM3_SIGNLE_ERROR_ADDRESS_SIZE * 2U), DATA_VALUE);

  /* Stop ECC for SRAM3 */
  if (HAL_RAMCFG_StopECC(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }

  /* Fill memory with single error data */
  Fill_wMemory((uint32_t *)SRAM3_SIGNLE_ERROR_ADDRESS_BASE, SRAM3_SIGNLE_ERROR_ADDRESS_SIZE, SINGLE_ERROR_DATA_VALUE);

  /* Fill memory with single error data */
  Fill_wMemory((uint32_t *)SRAM3_DOUBLE_ERROR_ADDRESS_BASE, SRAM3_DOUBLE_ERROR_ADDRESS_SIZE, DOUBLE_ERROR_DATA_VALUE);


  /*##-3- Read memory content and check that single and double ECC errors are
          generated ##########################################################*/
  /* Start ECC for SRAM3 */
  if (HAL_RAMCFG_StartECC(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register callback for RAMCFG peripheral single ECC error interrupt */
  if (HAL_RAMCFG_RegisterCallback(&hramcfg_SRAM3, HAL_RAMCFG_SE_DETECT_CB_ID, SingleErrorDetectionCallback) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register callback for RAMCFG peripheral double ECC error interrupt */
  if (HAL_RAMCFG_RegisterCallback(&hramcfg_SRAM3, HAL_RAMCFG_DE_DETECT_CB_ID, DoubleErrorDetectionCallback) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable RAMECC single and double ECC error interrupts */
  if (HAL_RAMCFG_EnableNotification(&hramcfg_SRAM3, (RAMCFG_IT_SINGLEERR | RAMCFG_IT_DOUBLEERR)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Read single error memory area */
  Read_wMemory((uint32_t *)SRAM3_SIGNLE_ERROR_ADDRESS_BASE, SingleErrorData_Buffer, SRAM3_SIGNLE_ERROR_ADDRESS_SIZE);

  /* Read double error memory area */
  Read_wMemory((uint32_t *)SRAM3_DOUBLE_ERROR_ADDRESS_BASE, DoubleErrorData_Buffer, SRAM3_DOUBLE_ERROR_ADDRESS_SIZE);

  /* Check that all single error data are corrected via RAMCFG peripheral */
  if (Check_wBuffer(SingleErrorData_Buffer, SRAM3_SIGNLE_ERROR_ADDRESS_SIZE, DATA_VALUE) != MATCH)
  {
    Error_Handler();
  }

  /* Check that all double error data are not corrected via RAMCFG peripheral */
  if (Check_wBuffer(DoubleErrorData_Buffer, SRAM3_DOUBLE_ERROR_ADDRESS_SIZE, DOUBLE_ERROR_DATA_VALUE) != MATCH)
  {
    Error_Handler();
  }

  /* Check the number of ECC single error detected */
  if (ECC_SingleError != EXPECTED_SINGLE_ERROR_NUM)
  {
    Error_Handler();
  }

  /* Check the number of ECC double error detected */
  if (ECC_DoubleError != EXPECTED_DOUBLE_ERROR_NUM)
  {
    Error_Handler();
  }

  /* DeInitialize the RAMCFG peripheral */
  if (HAL_RAMCFG_DeInit(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(250);
    BSP_LED_Toggle(LED1);
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
  * @brief RAMCFG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RAMCFG_Init(void)
{

  /* USER CODE BEGIN RAMCFG_Init 0 */

  /* USER CODE END RAMCFG_Init 0 */

  /* USER CODE BEGIN RAMCFG_Init 1 */

  /* USER CODE END RAMCFG_Init 1 */
  hramcfg_SRAM3.Instance = RAMCFG_SRAM3;
  if (HAL_RAMCFG_Init(&hramcfg_SRAM3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RAMCFG_Init 2 */

  /* USER CODE END RAMCFG_Init 2 */

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Fill a global 32-bit memory
  * @param  pMemory    : pointer on the Memory to fill
  * @param  MemorySize : size of the memory to fill
  * @param  Data       : data value
  * @retval None
  */
static void Fill_wMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data)
{
  uint32_t IndexTmp;

  /* Repeat for all buffer size */
  for (IndexTmp = 0; IndexTmp < MemorySize; IndexTmp++)
  {
    pMemory[IndexTmp] = Data;
  }
}

/**
  * @brief  Read a global 32-bit memory
  * @param  pMemory    : pointer on the memory to read
  * @param  pBuffer    : pointer on the Buffer to fill
  * @param  BufferSize : size of the data to read
  * @retval None
  */
static void Read_wMemory(uint32_t *pMemory, uint32_t *pBuffer, uint16_t BufferSize)
{
  __IO uint32_t IndexTmp;
  uint32_t VarTmp;

  /* Repeat for all buffer size */
  for (IndexTmp = 0; IndexTmp < BufferSize; IndexTmp++)
  {
    VarTmp = pMemory[IndexTmp];
    pBuffer[IndexTmp] = VarTmp;
  }
}

/**
  * @brief  Check a global 32-bit buffer content
  * @param  pBuffer      : pointer on the Buffer to check
  * @param  BufferSize   : size of the data to check
  * @param  ComparedData : data to be compared with buffer content
  * @retval None
  */
static BufferContentStatus Check_wBuffer(uint32_t *pBuffer, uint16_t BufferSize, uint32_t CompareData)
{
  uint32_t IndexTmp;

  /* Repeat for all buffer size */
  for (IndexTmp = 0; IndexTmp < BufferSize; IndexTmp++)
  {
    if (pBuffer[IndexTmp] != CompareData)
    {
      return NOTMATCH;
    }
  }

  return MATCH;
}

/**
  * @brief  RAMCFG single error detection callback
  * @param  hramcfg : Pointer to a RAMCFG_HandleTypeDef structure that contains
  *                   the configuration information for the specified RAMCFG
  * @retval None
  */
static void SingleErrorDetectionCallback(RAMCFG_HandleTypeDef *hramcfg)
{
  ECC_SingleError++;
}

/**
  * @brief  RAMCFG single error detection callback
  * @param  hramcfg : Pointer to a RAMCFG_HandleTypeDef structure that contains
  *                   the configuration information for the specified RAMCFG
  * @retval None
  */
static void DoubleErrorDetectionCallback(RAMCFG_HandleTypeDef *hramcfg)
{
  ECC_DoubleError++;
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);
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
