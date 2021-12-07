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
#define START_ADRESS_OCOTOSPI2                   0x70000000
#define START_ADRESS_OTFDEC2_REGION1             0x70000000
#define END_ADRESS_OTFDEC2_REGION1               0x700003FF
#define VERSION_NUMBER                           4

#define PLAIN_SIZE                               0x100
#define START_ADRESS_WRITING_REGION              0
#define NONCE_NUMBER                             2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp;
uint32_t pKeyAES[4] = {0x00000000,0x00000000,0x00000000,0x00000000};
uint32_t pInitVectAES[4] = {0x00000000,0x00000000,0x00000000,0x00000000};

DCACHE_HandleTypeDef hdcache1;

OSPI_HandleTypeDef hospi2;

OTFDEC_HandleTypeDef hotfdec2;

/* USER CODE BEGIN PV */
OTFDEC_RegionConfigTypeDef Config;
OSPI_MemoryMappedTypeDef sMemMappedCfg;

uint32_t OSPIAddressMode;
uint32_t OSPIDataMode;

__ALIGN_BEGIN uint32_t Cipher[PLAIN_SIZE] __ALIGN_END ;
uint32_t Plain_AES_test[PLAIN_SIZE] = {0};

uint16_t FirmwareVersion[VERSION_NUMBER] = { 0xABBA, 0xA5A5, 0x5A5A,0xF119 };

uint32_t Nonce[NONCE_NUMBER][2]       = {    {0xA5A5A5A5, 0xC3C3C3C3},
                                             {0x11111111, 0x55555555}
                                         };

uint32_t Region1_Range[2] = {START_ADRESS_OTFDEC2_REGION1, END_ADRESS_OTFDEC2_REGION1};

uint32_t Key[4]  = { 0x71234567, 0x89ABCDEF, 0x71234567, 0x89ABCDEF };

uint32_t Plain[PLAIN_SIZE] ={ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                              0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                              0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                              0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                            };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_OCTOSPI2_Init(void);
static void MX_OTFDEC2_Init(void);
static void MX_AES_Init(void);
/* USER CODE BEGIN PFP */
static void CRYP_Cipher(uint32_t index,uint32_t* pRange, uint32_t StartAdressWritingRegion);
static uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size);
static uint32_t OSPI_MemoryMap(void);
static void OSPI_ResetMemory(void);
static uint32_t OSPI_Erase(void);
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
  uint32_t i,errorBuffer = 0;
  __IO uint32_t *mem_addr_32;
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

  /* Fill Cipher Buffer*/
  for (i=0; i <PLAIN_SIZE; i++)
  {
    Cipher[i] = 0xFF ;
  }
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_DCACHE1_Init();
  MX_OCTOSPI2_Init();
  /* USER CODE BEGIN 2 */
  /* Erase the Flash Memory   */
  OSPI_Erase();

  /* Preload external HyperRAM with ciphred data issued from crypto into dedicated address*/
  CRYP_Cipher(OTFDEC_REGION1,(uint32_t*)Region1_Range,START_ADRESS_WRITING_REGION);
  OSPI_Write( Cipher,START_ADRESS_OTFDEC2_REGION1- START_ADRESS_OCOTOSPI2, PLAIN_SIZE);

  /* Activate memory mapping */
  OSPI_MemoryMap();
    MX_OTFDEC2_Init();
  __HAL_OTFDEC_ENABLE_IT(&hotfdec2, OTFDEC_ALL_INT);

  /* Set OTFDEC Mode */
  if (HAL_OTFDEC_RegionSetMode(&hotfdec2, OTFDEC_REGION1, OTFDEC_REG_MODE_INSTRUCTION_OR_DATA_ACCESSES) != HAL_OK)
  {
    Error_Handler();
  }
  /* Set OTFDEC Key */
  if (HAL_OTFDEC_RegionSetKey(&hotfdec2, OTFDEC_REGION1, Key)  != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure and lock region,enable OTFDEC decryption */
  Config.Nonce[0]     = Nonce[OTFDEC_REGION1/4][0];
  Config.Nonce[1]     = Nonce[OTFDEC_REGION1/4][1];
  Config.StartAddress = START_ADRESS_OTFDEC2_REGION1 ;
  Config.EndAddress   = END_ADRESS_OTFDEC2_REGION1;
  Config.Version      = FirmwareVersion[OTFDEC_REGION1];
  if (HAL_OTFDEC_RegionConfig(&hotfdec2, OTFDEC_REGION1, &Config, OTFDEC_REG_CONFIGR_LOCK_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  mem_addr_32= (uint32_t *)START_ADRESS_OTFDEC2_REGION1;

  /* OTFDEC activated, should read decrypted DATA */
  for (uint16_t index = 0; index < PLAIN_SIZE/4; index++)
  {
    if (*mem_addr_32 != Plain[index])
    {
      BSP_LED_On(LED6);
      errorBuffer++;
    }
    mem_addr_32++;
  }
  /* DeInit OTFDEC */
  HAL_OTFDEC_DeInit(&hotfdec2);

  /* DeInit OCTOSPI */
  HAL_OSPI_DeInit(&hospi2);

  if (errorBuffer == 0)
  {
    /* Turn LED7 on */
    BSP_LED_On(LED7);
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
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_NO_SWAP;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_CTR;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  hcryp.Init.KeyMode = CRYP_KEYMODE_NORMAL;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

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
  * @brief OCTOSPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI2_Init(void)
{

  /* USER CODE BEGIN OCTOSPI2_Init 0 */
  uint8_t id[20];
  OSPI_RegularCmdTypeDef  sCommand;
  /* USER CODE END OCTOSPI2_Init 0 */

  OSPIM_CfgTypeDef sOspiManagerCfg = {0};
  HAL_OSPI_DLYB_CfgTypeDef HAL_OSPI_DLYB_Cfg_Struct = {0};

  /* USER CODE BEGIN OCTOSPI2_Init 1 */
  /* USER CODE END OCTOSPI2_Init 1 */
  /* OCTOSPI2 parameter configuration*/
  hospi2.Instance = OCTOSPI2;
  hospi2.Init.FifoThreshold = 32;
  hospi2.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi2.Init.MemoryType = HAL_OSPI_MEMTYPE_MACRONIX;
  hospi2.Init.DeviceSize = 26;
  hospi2.Init.ChipSelectHighTime = 2;
  hospi2.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi2.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi2.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi2.Init.ClockPrescaler = 4;
  hospi2.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  hospi2.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  hospi2.Init.ChipSelectBoundary = 0;
  hospi2.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_USED;
  hospi2.Init.MaxTran = 0;
  hospi2.Init.Refresh = 0;
  if (HAL_OSPI_Init(&hospi2) != HAL_OK)
  {
    Error_Handler();
  }
  sOspiManagerCfg.ClkPort = 2;
  sOspiManagerCfg.DQSPort = 2;
  sOspiManagerCfg.NCSPort = 2;
  sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_2_LOW;
  sOspiManagerCfg.IOHighPort = HAL_OSPIM_IOPORT_2_HIGH;
  if (HAL_OSPIM_Config(&hospi2, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_OSPI_DLYB_Cfg_Struct.Units = 0;
  HAL_OSPI_DLYB_Cfg_Struct.PhaseSel = 0;
  if (HAL_OSPI_DLYB_SetConfig(&hospi2, &HAL_OSPI_DLYB_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI2_Init 2 */
  OSPI_ResetMemory();

  /* Initialize the reset enable command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction           = READ_ID_CMD;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.NbData                = 20;
  sCommand.DataMode              = HAL_OSPI_DATA_1_LINE;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Receive(&hospi2, id, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END OCTOSPI2_Init 2 */

}

/**
  * @brief OTFDEC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OTFDEC2_Init(void)
{

  /* USER CODE BEGIN OTFDEC2_Init 0 */

  /* USER CODE END OTFDEC2_Init 0 */

  /* USER CODE BEGIN OTFDEC2_Init 1 */

  /* USER CODE END OTFDEC2_Init 1 */
  hotfdec2.Instance = OTFDEC2;
  if (HAL_OTFDEC_Init(&hotfdec2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OTFDEC2_Init 2 */

  /* USER CODE END OTFDEC2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/******************************************************************************/
/*                           OCTOSPI  Helper                                  */
/******************************************************************************/

static uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  if (Size == 0)
  {
    Error_Handler();
  }

  sConfig.MatchMode              = HAL_OSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop          = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.Interval               = 0x10;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* 1- Enable write operations ----------------------------------------- */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_SetFifoThreshold(&hospi2, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&hospi2, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Writing Sequence ----------------------------------------------- */
  if (HAL_OSPI_SetFifoThreshold(&hospi2, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = PAGE_PROG_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = FlashAddress;
  sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData      = Size;
  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(&hospi2,(uint8_t *) pBuffer,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for end of program ---- */
  sConfig.Match           = MEMORY_READY_MATCH_VALUE;
  sConfig.Mask            = MEMORY_READY_MASK_VALUE;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.AddressMode    = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&hospi2, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}

/**
* @brief  This function set memory mapped mode.
* @param  None
* @retval None
*/
static uint32_t OSPI_MemoryMap(void)
{

  OSPI_RegularCmdTypeDef   sCommand;
  OSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_SetFifoThreshold(&hospi2, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.Instruction   = PAGE_PROG_CMD;
  sCommand.AddressMode   = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize   = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.DataMode      = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles   = 0;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType      = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.Instruction        = 0x03;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles        = 0;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
  sMemMappedCfg.TimeOutPeriod     = 0x20;

  if (HAL_OSPI_MemoryMapped(&hospi2, &sMemMappedCfg))
  {
    Error_Handler();
  }

  return 0;
}

/**
* @brief  This function reset the OSPI memory.
* @param  None
* @retval None
*/
static void OSPI_ResetMemory(void)
{
  OSPI_RegularCmdTypeDef  sCommand;

  /* Initialize the reset enable command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.DataMode              = HAL_OSPI_DATA_NONE;

  sCommand.Instruction = RESET_ENABLE_CMD;
  /* Send the command */
  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }


  /* Send the reset memory command */
  sCommand.Instruction = RESET_MEMORY_CMD;
  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(40);
}

static uint32_t OSPI_Erase(void)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  sConfig.MatchMode              = HAL_OSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop          = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.Interval               = 0x10;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* 1- Enable write operations ----------------------------------------- */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&hospi2, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Erasing Sequence ----------------------------------------------- */

  sCommand.Instruction = BLOCK_ERASE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = 0;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;
  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for end of erase ------ */
  sConfig.Match           = MEMORY_READY_MATCH_VALUE;
  sConfig.Mask            = MEMORY_READY_MASK_VALUE;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.AddressMode    = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&hospi2, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}

/****************************************************************************/
/*                           Cryp Helper                                    */
/****************************************************************************/

void CRYP_Cipher(uint32_t index,uint32_t* pRange, uint32_t StartAdressWritingRegion)
{
  uint32_t tmp;
  uint32_t AESIV[4] = {0};
  uint32_t  j = 0;
  uint32_t Key_tmp[4]={Key[0],Key[1],Key[2],Key[3]};
  CRYP_ConfigTypeDef AES_Conf = {0};

  tmp       = Key_tmp[0];
  Key_tmp[0] = Key_tmp[3];
  Key_tmp[3] = tmp;
  tmp       = Key_tmp[1];
  Key_tmp[1] = Key_tmp[2];
  Key_tmp[2] = tmp;

  AESIV [3] = ((index%4)<<28) | ((pRange[index*2]+StartAdressWritingRegion)>>4);
  AESIV [2] = FirmwareVersion[index];
  AESIV [1] = Nonce[index/4][0];
  AESIV [0] = Nonce[index/4][1];

  MX_AES_Init();

  HAL_CRYP_GetConfig(&hcryp, &AES_Conf);
  AES_Conf.pKey =  Key_tmp;
  AES_Conf.pInitVect = AESIV;
  HAL_CRYP_SetConfig(&hcryp, &AES_Conf);

  for (j=0; j<(PLAIN_SIZE)/4;j++)
  {
    Plain_AES_test[4*j] = Plain[4*j+3+(StartAdressWritingRegion/4)];
    Plain_AES_test[4*j+3] = Plain[4*j+(StartAdressWritingRegion/4)];
    Plain_AES_test[4*j+1] = Plain[4*j+2+(StartAdressWritingRegion/4)];
    Plain_AES_test[4*j+2] = Plain[4*j+1+(StartAdressWritingRegion/4)] ;
  }
  HAL_CRYP_Encrypt(&hcryp, Plain_AES_test, (PLAIN_SIZE), Cipher, 0x1000);

  for (j=0; j<(PLAIN_SIZE)/4;j++)
  {
    tmp = Cipher[4*j];
    Cipher[4*j] = Cipher[4*j+3];
    Cipher[4*j+3] = tmp;
    tmp = Cipher[4*j+1];
    Cipher[4*j+1] = Cipher[4*j+2];
    Cipher[4*j+2] = tmp;
  }
  HAL_CRYP_DeInit(&hcryp);
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
    BSP_LED_On(LED6);
    HAL_Delay(500);
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

