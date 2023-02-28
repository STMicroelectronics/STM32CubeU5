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

OSPI_HandleTypeDef hospi1;

/* USER CODE BEGIN PV */
__IO uint8_t *mem_addr;
uint8_t CmdCplt, TxCplt , StatusMatch , RxCplt;
OSPI_MemoryMappedTypeDef sMemMappedCfg;
LL_DLYB_CfgTypeDef dlyb_cfg,dlyb_cfg_test;

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFERSIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_OCTOSPI1_Init(void);
/* USER CODE BEGIN PFP */
void Configure_APMemory(void);
void EnableCompensationCell(void);
uint32_t APS6408_WriteReg(OSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value);
uint32_t APS6408_ReadReg(OSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode);
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
  OSPI_RegularCmdTypeDef sCommand = {0};
  uint32_t index, index_K;
  uint16_t errorBuffer = 0;
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
  /* Configure LED6, LED7 */
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED7);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DCACHE1_Init();
  MX_ICACHE_Init();
  MX_OCTOSPI1_Init();
  /* USER CODE BEGIN 2 */
  /* Enable Compensation cell */
  EnableCompensationCell();
  /* Delay block configuration ------------------------------------------------ */
  if (HAL_OSPI_DLYB_GetClockPeriod(&hospi1,&dlyb_cfg) != HAL_OK)
  {
    Error_Handler() ;
  }

  /*when DTR, PhaseSel is divided by 4 (emperic value)*/
  dlyb_cfg.PhaseSel /=4;

  /* save the present configuration for check*/
  dlyb_cfg_test = dlyb_cfg;

  /*set delay block configuration*/
  HAL_OSPI_DLYB_SetConfig(&hospi1,&dlyb_cfg);

  /*check the set value*/
  HAL_OSPI_DLYB_GetConfig(&hospi1,&dlyb_cfg);
  if ((dlyb_cfg.PhaseSel != dlyb_cfg_test.PhaseSel) || (dlyb_cfg.Units != dlyb_cfg_test.Units))
  {
    Error_Handler() ;
  }

  /*Configure AP memory register */
  Configure_APMemory();

  /*Configure Memory Mapped mode*/
  sCommand.OperationType      = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction        = WRITE_CMD;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_16_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_WRITE;
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.Instruction   = READ_CMD;
  sCommand.DummyCycles   = DUMMY_CLOCK_CYCLES_READ;

  if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_ENABLE;
  sMemMappedCfg.TimeOutPeriod     = 0x34;

  if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK)
  {
    Error_Handler();
  }

   /*fill aTxBuffer */
  for (index_K = 0; index_K < 10; index_K++)
  {
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      aTxBuffer[index]=index + index_K;
    }
  }

  /* Writing Sequence ----------------------------------------------- */
  index_K=0;
  for (index_K = 0; index_K < 10; index_K++)
  {
    mem_addr = (uint8_t *)(OCTOSPI1_BASE + (index_K * KByte));
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      *mem_addr = aTxBuffer[index];
      mem_addr++;
    }

  }

  /* Reading Sequence ----------------------------------------------- */
  index_K=0;
  for (index_K = 0; index_K < 10; index_K++)
  {
    mem_addr = (uint8_t *)(OCTOSPI1_BASE + (index_K * KByte));
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      if (*mem_addr != aTxBuffer[index])
      {
        BSP_LED_On(LED6);
        errorBuffer++;
      }
      mem_addr++;
    }
  }
  if (errorBuffer == 0)
  {
    /* Turn LED7 on */
    BSP_LED_On(LED7);
  }

  /* Abort OctoSPI driver to stop the memory-mapped mode ------------ */
  if (HAL_OSPI_Abort(&hospi1) != HAL_OK)
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
  * @brief OCTOSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */

  /* USER CODE END OCTOSPI1_Init 0 */

  OSPIM_CfgTypeDef sOspiManagerCfg = {0};
  HAL_OSPI_DLYB_CfgTypeDef HAL_OSPI_DLYB_Cfg_Struct = {0};

  /* USER CODE BEGIN OCTOSPI1_Init 1 */

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThreshold = 1;
  hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_APMEMORY;
  hospi1.Init.DeviceSize = 23;
  hospi1.Init.ChipSelectHighTime = 2;
  hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi1.Init.ClockPrescaler = 2;
  hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  hospi1.Init.ChipSelectBoundary = 10;
  hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_USED;
  hospi1.Init.MaxTran = 0;
  hospi1.Init.Refresh = 320;
  if (HAL_OSPI_Init(&hospi1) != HAL_OK)
  {
    Error_Handler();
  }
  sOspiManagerCfg.ClkPort = 1;
  sOspiManagerCfg.DQSPort = 1;
  sOspiManagerCfg.NCSPort = 1;
  sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
  sOspiManagerCfg.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_OSPI_DLYB_Cfg_Struct.Units = 0;
  HAL_OSPI_DLYB_Cfg_Struct.PhaseSel = 0;
  if (HAL_OSPI_DLYB_SetConfig(&hospi1, &HAL_OSPI_DLYB_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */

  /* USER CODE END OCTOSPI1_Init 2 */

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
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Rx Transfer completed callbacks.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  RxCplt++;
}

/**
  * @brief  Command completed callbacks.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_CmdCpltCallback(OSPI_HandleTypeDef *hospi)
{
  CmdCplt++;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hospi: OSPI handle
  * @retval None
  */
 void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  TxCplt++;
}

/**
  * @brief  Status Match callbacks
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_StatusMatchCallback(OSPI_HandleTypeDef *hospi)
{
  StatusMatch++;
}

/**
  * @brief  Transfer Error callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef *hospi)
{
  Error_Handler();
}

/**
  * @brief  Write mode register
  * @param  Ctx Component object pointer
  * @param  Address Register address
  * @param  Value Register value pointer
  * @retval error status
  */
uint32_t APS6408_WriteReg(OSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value)
{
  OSPI_RegularCmdTypeDef sCommand1={0};

  /* Initialize the write register command */
  sCommand1.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand1.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand1.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand1.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand1.Instruction        = WRITE_REG_CMD;
  sCommand1.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand1.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand1.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand1.Address            = Address;
  sCommand1.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand1.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand1.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand1.NbData             = 2;
  sCommand1.DummyCycles        = 0;
  sCommand1.DQSMode            = HAL_OSPI_DQS_DISABLE;
  sCommand1.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_OSPI_Command(Ctx, &sCommand1, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Transmission of the data */
  if (HAL_OSPI_Transmit(Ctx, (uint8_t *)(Value), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Read mode register value
  * @param  Ctx Component object pointer
  * @param  Address Register address
  * @param  Value Register value pointer
  * @param  LatencyCode Latency used for the access
  * @retval error status
  */
uint32_t APS6408_ReadReg(OSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode)
{
  OSPI_RegularCmdTypeDef sCommand;

  /* Initialize the read register command */
  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.Instruction        = READ_REG_CMD;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_ENABLE;
  sCommand.Address            = Address;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_8_LINES;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_ENABLE;
  sCommand.NbData             = 2;
  sCommand.DummyCycles        = (LatencyCode - 1U);
  sCommand.DQSMode            = HAL_OSPI_DQS_ENABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_OSPI_Command(Ctx, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(Ctx, (uint8_t *)Value, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief Enable Compensation Cell
  * @retval None
  */
void EnableCompensationCell(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  HAL_SYSCFG_EnableVddCompensationCell();
}

/**
  * @brief Configure AP memory
  * @retval None
  */
void Configure_APMemory(void)
{
  /* MR0 register for read and write */
  uint8_t regW_MR0[2]={0x24,0x0D}; /* To configure AP memory Latency Type and drive Strength */
  uint8_t regR_MR0[2]={0};

  /* MR8 register for read and write */
  uint8_t regW_MR8[2]={0x0B,0x08}; /* To configure AP memory Burst Type */
  uint8_t regR_MR8[2]={0};

  /*Read Latency */
  uint8_t latency=6;

  /* Configure Read Latency and drive Strength */
  if (APS6408_WriteReg(&hospi1, MR0, regW_MR0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Check MR0 configuration */
  if (APS6408_ReadReg(&hospi1, MR0, regR_MR0, latency ) != HAL_OK)
  {
    Error_Handler();
  }

    /* Check MR0 configuration */
  if (regR_MR0 [0] != regW_MR0 [0])
  {
    Error_Handler() ;
  }

  /* Configure Burst Length */
  if (APS6408_WriteReg(&hospi1, MR8, regW_MR8) != HAL_OK)
  {
    Error_Handler();
  }

  /* Check MR8 configuration */
  if (APS6408_ReadReg(&hospi1, MR8, regR_MR8, 6) != HAL_OK)
  {
    Error_Handler();
  }

  if (regR_MR8[0] != regW_MR8[0])
  {
    Error_Handler() ;
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
