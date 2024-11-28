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
/* DMA flags declaration */
__IO uint32_t TransferCompleteDetected = 0U, TransferErrorDetected = 0U;

/* Source buffer */
__ALIGNED(32) static const uint8_t aSRC_Buffer[BUFFER_SIZE] = {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7};

/* Destination buffer */
__ALIGNED(32) static uint8_t aDEST_Buffer[BUFFER_SIZE * 2U];

/* Right aligned zero padding data expected result buffer */
static const uint8_t aRightAlign_ZeroPadding_ExpectedResult_Buffer[BUFFER_SIZE * 2U] = {0xB0, 0x00, 0xB1, 0x00, 0xB2, 0x00, 0xB3, 0x00, 0xB4, 0x00, 0xB5, 0x00, 0xB6, 0x00, 0xB7, 0x00};

/* Right aligned sign extend data expected result buffer */
static const uint8_t aRightAlign_SignExtend_ExpectedResult_Buffer[BUFFER_SIZE * 2U]  = {0xB0, 0xFF, 0xB1, 0xFF, 0xB2, 0xFF, 0xB3, 0xFF, 0xB4, 0xFF, 0xB5, 0xFF, 0xB6, 0xFF, 0xB7, 0xFF};

/* Right aligned left truncated data expected result buffer */
static const uint8_t aRightAlign_LeftTrunc_ExpectedResult_Buffer[BUFFER_SIZE]   = {0xB0, 0xB2, 0xB4, 0xB6, 0x00, 0x00, 0x00, 0x00};

/* Left aligned right truncated data expected result buffer */
static const uint8_t aLeftAlign_RightTrunc_ExpectedResult_Buffer[BUFFER_SIZE]   = {0xB1, 0xB3, 0xB5, 0xB7, 0x00, 0x00, 0x00, 0x00};


/* Source byte exchange data expected result buffer */
static const uint8_t aSrcByte_Exchange_ExpectedResult_Buffer[BUFFER_SIZE]       = {0xB0, 0xB2, 0xB1, 0xB3, 0xB4, 0xB6, 0xB5, 0xB7};

/* Destination byte exchange data expected result buffer */
static const uint8_t aDestByte_Exchange_ExpectedResult_Buffer[BUFFER_SIZE]      = {0xB1, 0xB0, 0xB3, 0xB2, 0xB5, 0xB4, 0xB7, 0xB6};

/* Destination half word exchange data expected result buffer */
static const uint8_t aDestHalfWord_Exchange_ExpectedResult_Buffer[BUFFER_SIZE]  = {0xB2, 0xB3, 0xB0, 0xB1, 0xB6, 0xB7, 0xB4, 0xB5};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DMA_HandleTypeDef handle_GPDMA1_Channel12;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static void RightAlign_SignExtend_DMATransmit(void);
static void RightAlign_LeftTrunc_DMATransmit(void);
static void LeftAlign_RightTrunc_DMATransmit(void);
static void SrcByte_Exchange_DMATransmit(void);
static void DestByte_Exchange_DMATransmit(void);
static void DestHalfWord_Exchange_DMATransmit(void);

static void TransferComplete(DMA_HandleTypeDef *hdma);
static void TransferError(DMA_HandleTypeDef *hdma);

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

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */
  /* Initialize LED1 and LED3 : GREEN and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE * 2U);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aRightAlign_ZeroPadding_ExpectedResult_Buffer,  BUFFER_SIZE * 2U) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE * 2U);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;


  /*##-2- Right aligned / Sign extended data handling ########################*/
  RightAlign_SignExtend_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aRightAlign_SignExtend_ExpectedResult_Buffer, BUFFER_SIZE * 2U) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE * 2U);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;


  /*##-3- Right aligned / Left truncated data handling #######################*/
  RightAlign_LeftTrunc_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aRightAlign_LeftTrunc_ExpectedResult_Buffer, BUFFER_SIZE) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;


  /*##-4- Left aligned / Right truncated data handling #######################*/
  LeftAlign_RightTrunc_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aLeftAlign_RightTrunc_ExpectedResult_Buffer, BUFFER_SIZE) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;


  /*##-5- Source byte exchange data handling #################################*/
  SrcByte_Exchange_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aSrcByte_Exchange_ExpectedResult_Buffer, BUFFER_SIZE) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;


  /*##-6- Destination byte exchange data handling #################################*/
  DestByte_Exchange_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aDestByte_Exchange_ExpectedResult_Buffer, BUFFER_SIZE) != 0U)
  {
    Error_Handler();
  }

  /* Flush destination buffer */
  memset((void *)aDEST_Buffer, 0U, BUFFER_SIZE);
  TransferCompleteDetected = 0U;
  TransferErrorDetected    = 0U;

  /*##-7- Destination byte exchange data handling #################################*/
  DestHalfWord_Exchange_DMATransmit();

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Check destination buffer */
  if (Buffercmp((uint8_t*)aDEST_Buffer, (uint8_t*)aDestHalfWord_Exchange_ExpectedResult_Buffer, BUFFER_SIZE) != 0U)
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
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel12.Instance = GPDMA1_Channel12;
  handle_GPDMA1_Channel12.Init.Request = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  handle_GPDMA1_Channel12.Init.Direction = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  handle_GPDMA1_Channel12.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel12.Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.SrcBurstLength = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength = 1;
  handle_GPDMA1_Channel12.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.Mode = DMA_NORMAL;
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel12, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */
  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
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
  * @brief  Right aligned and sign extended data DMA transmit
  * @param  None
  * @retval None
  */
static void RightAlign_SignExtend_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_BYTE;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_NONE;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_SIGNEXT;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Right aligned and left truncated data DMA transmit
  * @param  None
  * @retval None
  */
static void RightAlign_LeftTrunc_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_BYTE;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_NONE;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_LEFTTRUNC;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Left aligned and right truncated data DMA transmit
  * @param  None
  * @retval None
  */
static void LeftAlign_RightTrunc_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_BYTE;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }


  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_NONE;
  DataHandlingConfig.DataAlignment = DMA_DATA_LEFTALIGN_RIGHTTRUNC;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }


  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Source byte exchange data DMA transmit
  * @param  None
  * @retval None
  */
static void SrcByte_Exchange_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_SRC_BYTE;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Destination byte exchange data DMA transmit
  * @param  None
  * @retval None
  */
static void DestByte_Exchange_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_DEST_BYTE;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Destination half word exchange data DMA transmit
  * @param  None
  * @retval None
  */
static void DestHalfWord_Exchange_DMATransmit(void)
{
    DMA_DataHandlingConfTypeDef DataHandlingConfig = {0};

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Set DMA Instance */
  handle_GPDMA1_Channel12.Instance                    = GPDMA1_Channel12;

  /* Set parameter to be configured */
  handle_GPDMA1_Channel12.Init.Request                = DMA_REQUEST_SW;
  handle_GPDMA1_Channel12.Init.Direction              = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel12.Init.SrcInc                 = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.DestInc                = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel12.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_WORD;
  handle_GPDMA1_Channel12.Init.SrcBurstLength         = 1;
  handle_GPDMA1_Channel12.Init.DestBurstLength        = 1;
  handle_GPDMA1_Channel12.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel12.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel12.Init.Mode                   = DMA_NORMAL;

  /* Initialize the DMA channel */
  if (HAL_DMA_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set data handling block configuration */
  DataHandlingConfig.DataExchange  = DMA_EXCHANGE_DEST_HALFWORD;
  DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;

  /* Data handling configuration */
  if (HAL_DMAEx_ConfigDataHandling(&handle_GPDMA1_Channel12, &DataHandlingConfig) != HAL_OK)
  {
    while (1); /* KO */
  }

  /* Select Callbacks functions called after Half Transfer Complete, Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel12, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  /* Enable All the DMA interrupts */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel12, (uint32_t)&aSRC_Buffer, (uint32_t)&aDEST_Buffer, BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(DMA_HandleTypeDef *hdma)
{
  TransferCompleteDetected = 1U;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void TransferError(DMA_HandleTypeDef *hdma)
{
  TransferErrorDetected = 1U;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0   : pBuffer1 identical to pBuffer2
  *         > 0 : pBuffer1 differs from pBuffer2
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
