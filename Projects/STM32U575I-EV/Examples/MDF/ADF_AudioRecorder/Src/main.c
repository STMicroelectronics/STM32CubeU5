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
#include "linked_list.h"
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
MDF_HandleTypeDef AdfHandle0;
MDF_FilterConfigTypeDef AdfFilterConfig0;

DMA_HandleTypeDef handle_GPDMA1_Channel11;
DMA_HandleTypeDef handle_GPDMA1_Channel10;

SAI_HandleTypeDef hsai_BlockB1;

/* USER CODE BEGIN PV */
MDF_DmaConfigTypeDef         DMAConfig;
CS42L51_Init_t codec_init;
static AUDIO_Drv_t *AudioDrv = NULL;
void *AudioCompObj;

/*Buffer location and size should aligned to cache line size (32 bytes) */
int16_t        RecBuff[REC_BUFF_SIZE * 2U];
__IO uint32_t  DmaRecHalfBuffCplt  = 0;
__IO uint32_t  DmaRecBuffCplt      = 0;
uint32_t       PlaybackStarted         = 0;

__IO int16_t UpdatePointer = -1;
extern DMA_QListTypeDef SAIQueue;
extern DMA_QListTypeDef MDFQueue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_SAI1_Init(void);
static void MX_ADF1_Init(void);
/* USER CODE BEGIN PFP */
static void MDF_DMAConfig(void);
static int32_t CS42L51_Probe(void);
static int32_t CS42L51_PowerUp(void);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_SAI1_Init();
  MX_ADF1_Init();
  /* USER CODE BEGIN 2 */
  CS42L51_PowerUp();
  CS42L51_Probe();

  /* Fill codec_init structure */
  codec_init.InputDevice  = CS42L51_IN_MIC1;
  codec_init.OutputDevice = CS42L51_OUT_HEADPHONE;
  codec_init.Frequency    = AUDIO_FREQUENCY_44K;
  codec_init.Resolution   = CS42L51_RESOLUTION_16B; /* Not used */
  codec_init.Volume       = 100;

  /* Initialize the codec internal registers */
  if(AudioDrv->Init(AudioCompObj, &codec_init) < 0)
  {
    Error_Handler();
  }
  /* Start the playback */
  if(0 != AudioDrv->Play(AudioCompObj))
  {
    Error_Handler();
  }
  MX_SAIQueue_Config();
  MX_MDFQueue_Config();

  /* Set queue circular mode for sai queue */
  HAL_DMAEx_List_SetCircularMode(&SAIQueue);
  /* Set queue circular mode for sai queue */
  HAL_DMAEx_List_SetCircularMode(&MDFQueue);

  /* Link SAI queue to DMA channel */
  HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel10, &SAIQueue);
  /* Link SAI queue to DMA channel */
  HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel11, &MDFQueue);

  MDF_DMAConfig();
  if (HAL_MDF_AcqStart_DMA(&AdfHandle0, &AdfFilterConfig0, &DMAConfig) != HAL_OK)
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
    if(DmaRecHalfBuffCplt == 1)
    {

      if(PlaybackStarted == 0)
      {

        if(HAL_OK != HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *) &RecBuff[0], (REC_BUFF_SIZE * 2U)))
        {
          Error_Handler();
        }
        PlaybackStarted = 1;
      }
      DmaRecHalfBuffCplt  = 0;
    }
    if(DmaRecBuffCplt == 1)
    {
      DmaRecBuffCplt = 0;
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

  /* Switch to SMPS regulator instead of LDO */
  if(HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI|RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_3;
  RCC_OscInitStruct.MSIKState = RCC_MSIK_ON;
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
  * @brief ADF1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADF1_Init(void)
{

  /* USER CODE BEGIN ADF1_Init 0 */

  /* USER CODE END ADF1_Init 0 */

  /* USER CODE BEGIN ADF1_Init 1 */

  /* USER CODE END ADF1_Init 1 */
  /**
    AdfHandle0 structure initialization and HAL_MDF_Init function call
  */
  AdfHandle0.Instance = ADF1_Filter0;
  AdfHandle0.Init.CommonParam.ProcClockDivider = 1;
  AdfHandle0.Init.CommonParam.OutputClock.Activation = ENABLE;
  AdfHandle0.Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
  AdfHandle0.Init.CommonParam.OutputClock.Divider = 4;
  AdfHandle0.Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  AdfHandle0.Init.SerialInterface.Activation = ENABLE;
  AdfHandle0.Init.SerialInterface.Mode = MDF_SITF_NORMAL_SPI_MODE;
  AdfHandle0.Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
  AdfHandle0.Init.SerialInterface.Threshold = 31;
  AdfHandle0.Init.FilterBistream = MDF_BITSTREAM0_FALLING;
  if (HAL_MDF_Init(&AdfHandle0) != HAL_OK)
  {
    Error_Handler();
  }
  /**
    AdfFilterConfig0 structure initialization

    WARNING : only structure is filled, no specific init function call for filter
  */
  AdfFilterConfig0.DataSource = MDF_DATA_SOURCE_BSMX;
  AdfFilterConfig0.Delay = 0;
  AdfFilterConfig0.CicMode = MDF_ONE_FILTER_SINC4;
  AdfFilterConfig0.DecimationRatio = 64;
  AdfFilterConfig0.Gain = 0;
  AdfFilterConfig0.ReshapeFilter.Activation = DISABLE;
  AdfFilterConfig0.HighPassFilter.Activation = DISABLE;
  AdfFilterConfig0.SoundActivity.Activation = DISABLE;
  AdfFilterConfig0.AcquisitionMode = MDF_MODE_ASYNC_CONT;
  AdfFilterConfig0.FifoThreshold = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  AdfFilterConfig0.DiscardSamples = 0;
  /* USER CODE BEGIN ADF1_Init 2 */
  /* USER CODE END ADF1_Init 2 */

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
    HAL_NVIC_SetPriority(GPDMA1_Channel10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel10_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel11_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel11.Instance = GPDMA1_Channel11;
  handle_GPDMA1_Channel11.InitLinkedList.Priority = DMA_HIGH_PRIORITY;
  handle_GPDMA1_Channel11.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel11.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
  handle_GPDMA1_Channel11.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel11.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel11) != HAL_OK)
  {
    Error_Handler();
  }
  handle_GPDMA1_Channel10.Instance = GPDMA1_Channel10;
  handle_GPDMA1_Channel10.InitLinkedList.Priority = DMA_HIGH_PRIORITY;
  handle_GPDMA1_Channel10.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel10.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
  handle_GPDMA1_Channel10.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel10.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */
   /* Associate the DMA handle */
    __HAL_LINKDMA(&hsai_BlockB1, hdmatx, handle_GPDMA1_Channel10);
    __HAL_LINKDMA(&AdfHandle0, hdma, handle_GPDMA1_Channel11);
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

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockB1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockB1.Init.DataSize = SAI_DATASIZE_16;
  hsai_BlockB1.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockB1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
  hsai_BlockB1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hsai_BlockB1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
  hsai_BlockB1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_MONOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockB1.Init.PdmInit.Activation = DISABLE;
  hsai_BlockB1.Init.PdmInit.MicPairsNbr = 1;
  hsai_BlockB1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
  hsai_BlockB1.FrameInit.FrameLength = 32;
  hsai_BlockB1.FrameInit.ActiveFrameLength = 16;
  hsai_BlockB1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockB1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockB1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
  hsai_BlockB1.SlotInit.FirstBitOffset = 0;
  hsai_BlockB1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockB1.SlotInit.SlotNumber = 2;
  hsai_BlockB1.SlotInit.SlotActive = 0x00000003;
  if (HAL_SAI_Init(&hsai_BlockB1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/**
  * @brief  MDF DMA configuration
  * @param  None
  * @retval None
  */
static void MDF_DMAConfig(void)
{
  /* Initialize DMA configuration parameters */
  DMAConfig.Address                              = (uint32_t)&RecBuff[0];
  DMAConfig.DataLength                           = (REC_BUFF_SIZE * 4U);
  DMAConfig.MsbOnly                              = ENABLE;
}

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t CS42L51_Probe(void)
{
  int32_t                   ret = BSP_ERROR_NONE;
  CS42L51_IO_t              IOCtx;
  static CS42L51_Object_t   CS42L51Obj;
  uint32_t                  cs42l51_id;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if(CS42L51_RegisterBusIO (&CS42L51Obj, &IOCtx) != CS42L51_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if(CS42L51_ReadID(&CS42L51Obj, &cs42l51_id) != CS42L51_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if((cs42l51_id & CS42L51_ID_MASK) != CS42L51_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    AudioDrv = (AUDIO_Drv_t *) &CS42L51_Driver;
    AudioCompObj = &CS42L51Obj;
  }

  return ret;
}

/**
  * @brief  Un-reset CS42L51.
  * @param  None.
  * @retval BSP status.
  */
static int32_t CS42L51_PowerUp(void)
{
  int32_t ret = BSP_ERROR_NONE;;


#if (USE_BSP_IO_CLASS == 1)
  BSP_IO_Init_t IO_Init;

  /* Initialize the BSP IO driver and configure the CS42L51 reset pin */
  IO_Init.Pin  = IO_PIN_15;
  IO_Init.Mode = IO_MODE_OUTPUT_PP;
  IO_Init.Pull = IO_PULLDOWN;
  if(BSP_IO_Init(0, &IO_Init) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if (BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_RESET) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    /* Un-reset the CS42L51 */
    HAL_Delay(10);
    if(BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_SET) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      HAL_Delay(10);
    }
  }

  /* Wait 1ms according CS42L51 datasheet */
  HAL_Delay(1);
#endif /* USE_BSP_IO_CLASS == 1 */

  return ret;
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hmdf : MDF andle.
  * @retval None
  */
void HAL_MDF_AcqHalfCpltCallback(MDF_HandleTypeDef *hmdf)
{
    DmaRecHalfBuffCplt = 1;
}

/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_MDF_GetAcqValue.
  * @param  hmdf : MDF handle.
  * @retval None
  */
void HAL_MDF_AcqCpltCallback(MDF_HandleTypeDef *hmdf)
{
    DmaRecBuffCplt = 1;
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
