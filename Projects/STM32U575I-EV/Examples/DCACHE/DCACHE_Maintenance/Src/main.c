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

DCACHE_HandleTypeDef hdcache1;

DMA_HandleTypeDef handle_GPDMA1_Channel15;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
static const uint32_t aSRC_Const_Buffer1[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

/* Allocate Destination Buffer 2 in Non_cacheable memory */
static uint32_t aDST_NonCacheable_Buffer2[BUFFER_SIZE];

#if defined ( __ICCARM__ )
/* Allocate Destination Buffer 1 in cacheable memory */
#pragma location = 0x60000000
#elif defined(__ARMCC_VERSION) ||defined(__GNUC__)
__attribute__((section(".aDST_Buffer1section")))
#endif
static uint32_t aDST_Buffer1[BUFFER_SIZE];

#if defined ( __ICCARM__ )
/* Allocate Source Buffer 2 in cacheable memory */
#pragma location = 0x60100000
#elif defined(__ARMCC_VERSION) ||defined(__GNUC__)
__attribute__((section(".aSRC_Buffer2section")))
#endif
static uint32_t aSRC_Buffer2[BUFFER_SIZE];

static uint32_t i ;
static uint32_t tempValue;
__IO uint32_t j=0;
static __IO uint32_t transferErrorDetected = 0;
static __IO uint32_t transferCompleteDetected = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_FMC_Init(void);
/* USER CODE BEGIN PFP */
static void HAL_TransferError(DMA_HandleTypeDef *DMAHandle);
static void HAL_TransferComplete(DMA_HandleTypeDef *DMAHandle);
static uint32_t DMA_StartAndCompare(uint32_t* SrcBuffer, uint32_t* DstBuffer, uint32_t BufferLength);
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
#if defined(MPU_WRITE_THROUGH_ATTRIBUTE)
   MPU_Config_WT();
#elif defined(MPU_WRITE_BACK_WRITE_ALLOCATE_ATTRIBUTE)
  MPU_Config_WB_WA();
#endif
  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_DCACHE1_Init();
  MX_ICACHE_Init();
  MX_FMC_Init();
  /* USER CODE BEGIN 2 */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  /* Configure the user-button in GPIO Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Turn off leds */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_RED);

  /* Initialize buffers */
  memset(aDST_Buffer1, 0x00, BUFFER_SIZE * 4);
  memset(aSRC_Buffer2, 0x00, BUFFER_SIZE * 4);

  /*
   Clean and invalidate operation
   This guarantees that the initialized buffers are loaded into the External SRAM
  */
  HAL_DCACHE_CleanInvalidByAddr(&hdcache1, aDST_Buffer1, BUFFER_SIZE * 4);
  HAL_DCACHE_CleanInvalidByAddr(&hdcache1, aSRC_Buffer2, BUFFER_SIZE * 4);

  /*## -1- DMA Transfer 1: aSRC_Const_Buffer1 (FLASH) --> aDST_Buffer1  (External SRAM) #######*/
  /*###########################################################################################*/

  /* CPU Read access of the Destination Buffer with DCACHE enabled */
  for (i=0; i<BUFFER_SIZE; i++)
  {
    tempValue += aDST_Buffer1[i];
  }

  /* Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare((uint32_t *)aSRC_Const_Buffer1, aDST_Buffer1 , BUFFER_SIZE * 4) != 0)
  {
    /* Wrong Comparison: Turn off LED_GREEN */
    BSP_LED_Off(LED_GREEN);
  }
  else
  {
    /* Right Comparison: Turn on LED_GREEN */
    BSP_LED_On(LED_GREEN);
  }

  /*## -2- DMA Transfer 2: aSRC_Buffer2 (External SRAM) --> aDST_NonCacheable_Buffer2 (Internal SRAM) */
  /*#################################################################################################*/

  /* CPU Write access of the Source Buffer with DCACHE enabled */
  for (i=0; i<BUFFER_SIZE; i++)
  {
    aSRC_Buffer2[i] = i;
  }

  /* Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare(aSRC_Buffer2, aDST_NonCacheable_Buffer2, BUFFER_SIZE * 4) != 0)
  {
    /* Wrong Comparison: Turn off LED_RED */
      BSP_LED_Off(LED_RED);
  }
  else
  {
    /* Right Comparison: Turn on LED_RED */
      BSP_LED_On(LED_RED);
  }

  /*## -3- Wait for User Key press / release  #########################################*/
  /*#####################################################################################*/

  /* Wait for user-button Key press before starting the Cache maintenance */
  while (BSP_PB_GetState(BUTTON_USER) == RESET)
  {
  }

  /* Wait for user-button Key to be released before starting the Cache maintenance */
  while (BSP_PB_GetState(BUTTON_USER) == SET)
  {
  }

  /* Turn off leds */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_RED);

  /*## -4- Do Cache maintenance for Transfer 1  #########################################*/
  /*#####################################################################################*/

  /*
  CPU Data Cache maintenance:
  It is recommended to invalidate the destination buffer in Data cache after a DMA transfer.
  This guarantees up-to-date data when CPU access to the destination buffer located in the
  External SRAM (which is cacheable).
  */

  /* Invalidate aDST_Buffer1 in DCACHE: (BUFFER_SIZE * 4) bytes */
  HAL_DCACHE_InvalidateByAddr(&hdcache1, aDST_Buffer1, BUFFER_SIZE * 4);

  /* Compare Source and Destination buffers */
  if(memcmp(aSRC_Const_Buffer1, aDST_Buffer1, BUFFER_SIZE * 4) != 0)
  {
    /* Wrong Comparison: Turn off LED_GREEN */
    BSP_LED_Off(LED_GREEN);
  }
  else
  {
    /* Right Comparison: Turn on LED_GREEN */
    BSP_LED_On(LED_GREEN);
  }

  /*## -5- Do Cache maintenance for Transfer 2  #########################################*/
  /*#####################################################################################*/

  /*
  CPU Data Cache maintenance:
  It is recommended to clean the source buffer in CPU Data cache before starting
  the DMA transfer. As the source buffer may be used by the CPU and modified
  locally in DCACHE. The cache clean guarantees up-to-date data when DMA accesses
  to the source buffer located in the External SRAM (which is cacheable).
  */

  /* Clean aSRC_Buffer2 in DCACHE: (BUFFER_SIZE * 4) bytes */
  HAL_DCACHE_CleanByAddr(&hdcache1, aSRC_Buffer2, BUFFER_SIZE * 4);

  /* Re-Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare(aSRC_Buffer2, aDST_NonCacheable_Buffer2, BUFFER_SIZE) != 0)
  {
    /* Wrong Comparison: Turn off LED_RED */
    BSP_LED_Off(LED_RED);
  }
  else
  {
    /* Right comparison: Turn on LED_RED */
    BSP_LED_On(LED_RED);
  }
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
  HAL_PWREx_EnableVddIO2();

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
  * @brief DCACHE1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DCACHE1_Init(void)
{

  /* USER CODE BEGIN DCACHE1_Init 0 */

  /* USER CODE END DCACHE1_Init 0 */

  /* USER CODE BEGIN DCACHE1_Init 1 */

  /* USER CODE END DCACHE1_Init 1 */
  hdcache1.Instance = DCACHE1;
  hdcache1.Init.ReadBurstType = DCACHE_READ_BURST_INCR;
  if (HAL_DCACHE_Init(&hdcache1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCACHE1_Init 2 */

  /* USER CODE END DCACHE1_Init 2 */

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
    HAL_NVIC_SetPriority(GPDMA1_Channel15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel15_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel15.Instance = GPDMA1_Channel15;
  handle_GPDMA1_Channel15.Init.Request = DMA_REQUEST_SW;
  handle_GPDMA1_Channel15.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  handle_GPDMA1_Channel15.Init.Direction = DMA_MEMORY_TO_MEMORY;
  handle_GPDMA1_Channel15.Init.SrcInc = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel15.Init.DestInc = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel15.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  handle_GPDMA1_Channel15.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  handle_GPDMA1_Channel15.Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel15.Init.SrcBurstLength = 1;
  handle_GPDMA1_Channel15.Init.DestBurstLength = 1;
  handle_GPDMA1_Channel15.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
  handle_GPDMA1_Channel15.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel15.Init.Mode = DMA_NORMAL;
  if (HAL_DMA_Init(&handle_GPDMA1_Channel15) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel15, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel15, HAL_DMA_XFER_CPLT_CB_ID, HAL_TransferComplete);
  HAL_DMA_RegisterCallback(&handle_GPDMA1_Channel15, HAL_DMA_XFER_ERROR_CB_ID, HAL_TransferError);
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

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.NBLSetupTime = 0;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  hsram1.Init.MaxChipSelectPulse = DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 9;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 6;
  Timing.DataHoldTime = 0;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Configure the MPU attributes as Write Through for external SRAM.
  * @note   The external SRAM Base Address is 0x60000000.
  *         The Configured Region Size is 512 KB, as it corresponds to the size of Bank1
  *         of the external SRAM.
  * @param  None
  * @retval None
  */
void MPU_Config_WT(void)
{
  MPU_Attributes_InitTypeDef   attr;
  MPU_Region_InitTypeDef       region;

  /* Disable MPU before perloading and config update */
  HAL_MPU_Disable();

  /* Define cacheable memory via MPU */
  attr.Number             = MPU_ATTRIBUTES_NUMBER0;
  attr.Attributes         = MPU_NOT_CACHEABLE | OUTER(MPU_WRITE_THROUGH | MPU_TRANSIENT | MPU_R_ALLOCATE);
  HAL_MPU_ConfigMemoryAttributes(&attr);

  /* BaseAddress-LimitAddress configuration */
  region.Enable           = MPU_REGION_ENABLE;
  region.Number           = MPU_REGION_NUMBER0;
  region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
  region.BaseAddress      = FMC_BANK1_1;
  region.LimitAddress     = FMC_BANK1_1 + REGION_SIZE - 1;
  region.AccessPermission = MPU_REGION_ALL_RW;
  region.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  HAL_MPU_ConfigRegion(&region);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Configure the MPU attributes as Write Back - Write Allocate for External SRAM.
  * @note   The external SRAM Base Address is 0x60000000.
  *         The Configured Region Size is 512 KB, as it corresponds to the size of Bank1
  *         of the external SRAM.
  * @param  None
  * @retval None
  */
void MPU_Config_WB_WA(void)
{
  MPU_Attributes_InitTypeDef   attr;
  MPU_Region_InitTypeDef       region;

  /* Disable MPU before perloading and config update */
  HAL_MPU_Disable();

  /* Define cacheable memory via MPU */
  attr.Number             = MPU_ATTRIBUTES_NUMBER0;
  attr.Attributes         = MPU_NOT_CACHEABLE | OUTER(MPU_WRITE_BACK | MPU_TRANSIENT | MPU_W_ALLOCATE | MPU_R_ALLOCATE);
  HAL_MPU_ConfigMemoryAttributes(&attr);

  /* BaseAddress-LimitAddress configuration */
  region.Enable           = MPU_REGION_ENABLE;
  region.Number           = MPU_REGION_NUMBER0;
  region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
  region.BaseAddress      = FMC_BANK1_1;
  region.LimitAddress     = FMC_BANK1_1 + REGION_SIZE - 1;
  region.AccessPermission = MPU_REGION_ALL_RW;
  region.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  HAL_MPU_ConfigRegion(&region);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Start the DMA Transfer and compares source and destination buffers
  * @note  This function is used to :
  *        -1- Reset transfer variables
  *        -2- Start the DMA transfer using the interrupt mode
  *        -3- wait until DMA transfer complete or transfer error
  *        -4- Turn LED_ORANGE on in case of DMA transfer error
  *        -5- Compare Source and Destination buffers
  * @param  SrcBuffer: The source memory Buffer address
  * @param  DstBuffer: The destination memory Buffer address
  * @param  BuuferLength: The length of data to be transferred from source to destination
  * @retval Status (0 or 1)
  */
static uint32_t DMA_StartAndCompare(uint32_t* SrcBuffer, uint32_t* DstBuffer, uint32_t BufferLength)
{
  /* Reset transfer variables */
  transferCompleteDetected = 0;
  transferErrorDetected    = 0;

  /* Start the DMA transfer */
  if (HAL_DMA_Start_IT(&handle_GPDMA1_Channel15, (uint32_t)SrcBuffer, (uint32_t)DstBuffer, BufferLength) != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }

  while ((transferCompleteDetected == 0) && (transferErrorDetected == 0))
  {
    /* wait until DMA transfer complete or transfer error */
  }

  if (transferErrorDetected == 1)
  {
    /* DMA transfer error */
    return 1;
  }

  /* Compare Source and Destination buffers */
  if(memcmp(SrcBuffer, DstBuffer, BufferLength) != 0)
  {
    /* Wrong comparison */
    return 1;
  }
  else
  {
    /* Right comparison */
    return 0;
  }
}
/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void  HAL_TransferComplete(DMA_HandleTypeDef *DMAHandle)
{
  transferCompleteDetected = 1;
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void HAL_TransferError(DMA_HandleTypeDef *DMAHandle)
{
  transferErrorDetected = 1;
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
