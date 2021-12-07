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
#if defined(__ARMCC_VERSION)
extern uint32_t Load$$OSPI_RAM$$Base;
extern uint32_t Load$$OSPI_RAM$$Length;
#elif defined(__ICCARM__)
#pragma section =".ospi_ram"
#pragma section =".ospi_ram_init"
#elif defined(__GNUC__)
uint32_t _ospi_ram_init_base;
uint32_t _ospi_ram_init_length;
#endif
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DCACHE_HandleTypeDef hdcache1;

OSPI_HandleTypeDef hospi1;

/* USER CODE BEGIN PV */
__IO uint8_t CmdCplt;
LL_DLYB_CfgTypeDef dlyb_cfg,dlyb_cfg_test;
static void GpioToggle(void);
OSPI_MemoryMappedTypeDef sMemMappedCfg;
__IO uint8_t *mem_addr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_GPIO_Init(void);
static void MX_OCTOSPI1_Init(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef OSPI_ClockConfig(OSPI_HandleTypeDef *hospi);
int32_t APS256XX_WriteReg(OSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t Value);
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
  OSPI_RegularCmdTypeDef sCommand;
  __IO uint8_t *ospi_addr = 0;
  uint8_t *flash_addr;
  uint32_t max_size;
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
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_DCACHE1_Init();
  MX_GPIO_Init();
  MX_OCTOSPI1_Init();
  /* USER CODE BEGIN 2 */
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

  /* Configure PSRAM memory ***************************/
  sCommand.OperationType      = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction        = WRITE_CMD;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
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

  /* Initialize the variables for the data writing -------------------------- */
  ospi_addr = (__IO uint8_t *)(OCTOSPI1_BASE);
#if defined(__ARMCC_VERSION)
  max_size = (uint32_t)(&Load$$OSPI_RAM$$Length);
  flash_addr = (uint8_t *)(&Load$$OSPI_RAM$$Base);
#elif defined(__ICCARM__)
  max_size = __section_size(".ospi_ram_init");
  flash_addr = (uint8_t *)(__section_begin(".ospi_ram_init"));
#elif defined(__GNUC__)
  max_size = (uint32_t)((uint8_t *)(&_ospi_ram_init_length));
  flash_addr = (uint8_t *)(&_ospi_ram_init_base);
#endif

  /* Writing Sequence ------------------------------------------------------- */
  for (; max_size > 0; max_size--, flash_addr++, ospi_addr++)
  {
    *ospi_addr = *flash_addr;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Execute the code from OSPI PSRAM memory --------------------------- */
    GpioToggle();
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
  HAL_DCACHE_Enable(&hdcache1);
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
  hospi1.Init.ChipSelectHighTime = 1;
  hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_OSPI_WRAP_32_BYTES;
  hospi1.Init.ClockPrescaler = 2;
  hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  hospi1.Init.ChipSelectBoundary = 0;
  hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_USED;
  hospi1.Init.MaxTran = 0;
  hospi1.Init.Refresh = 0;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/**
* @brief  Command completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_CmdCpltCallback(OSPI_HandleTypeDef *hospi)
{
  CmdCplt++;
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
  * @brief  Toggle the GPIOs
  * @param  None
  * @retval None
  */
#if defined(__ICCARM__)
static void GpioToggle(void) @ ".ospi_ram"
#elif defined(__ARMCC_VERSION) || defined(__GNUC__)
static void __attribute__((section(".ospi_ram"), noinline)) GpioToggle(void)
#endif
{
  BSP_LED_Toggle(LED7);
  /* Insert delay 100 ms */
  HAL_Delay(100);
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
    HAL_Delay(100);
    BSP_LED_Toggle(LED6);
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
