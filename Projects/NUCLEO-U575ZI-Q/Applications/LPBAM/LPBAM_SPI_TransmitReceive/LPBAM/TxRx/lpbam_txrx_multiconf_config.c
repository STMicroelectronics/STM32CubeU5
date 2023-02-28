/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_txrx_multiconf_config.c
  * @author  MCD Application Team
  * @brief   Provide LPBAM TxRx application MultiConf configuration services.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */
/* USER CODE END Header */
/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "lpbam_txrx.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define --------------------------------------------------------------------------------------------------*/
#define COUPLED_TRANSMIT_Q_IDX (0U)
#define TRANSMIT_Q_IDX (1U)
#define DMA_TIMEOUT_DURATION (0x1000U)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* External variables ------------------------------------------------------------------------------------------------*/
/* IP handler declaration */
extern LPTIM_HandleTypeDef hlptim1;
extern SPI_HandleTypeDef hspi3;

/* LPBAM queues declaration */
extern DMA_QListTypeDef Transmit_Q;
extern DMA_QListTypeDef Coupled_Transmit_Q;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/

/* LPBAM LPTIM1 management APIs */
static void MX_LPTIM1_Init(void);
static void MX_LPTIM1_MspInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_MspDeInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_DeInit(void);

/* LPBAM SPI3 management APIs */
static void MX_SPI3_Init(void);
static void MX_SPI3_MspInit(SPI_HandleTypeDef *hspi3);
static void MX_SPI3_MspDeInit(SPI_HandleTypeDef *hspi3);
static void MX_SPI3_DeInit(void);

/* LPBAM autonomous mode management APIs */
static void MX_AutonomousMode_Init(void);
static void MX_AutonomousMode_DeInit(void);

/* LPBAM queue linking/unlinking APIs */
static void MX_Transmit_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_Transmit_Q_UnLink(DMA_HandleTypeDef *hdma);
static void MX_Coupled_Transmit_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_Coupled_Transmit_Q_UnLink(DMA_HandleTypeDef *hdma);

/* LPBAM DMA user callback APIs */
static void MX_Transmit_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
static void MX_Coupled_Transmit_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
/* LPBAM DMA NVIC API */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority);

/* USER CODE BEGIN PFP */
static void SPI3_Error_Callback(SPI_HandleTypeDef *hspi3);
static void SPI3_TxCplt_Callback(SPI_HandleTypeDef *hspi3);
/* USER CODE END PFP */

/* Exported functions ----------------------------------------------------------------------------------------------*/
/**
  * @brief  TxRx application MultiConf scenario initialization
  * @param  None
  * @retval None
  */
void MX_TxRx_MultiConf_Init(void)
{
  /* LPBAM LPTIM1 initialization */
  MX_LPTIM1_Init();

  /* LPBAM SPI3 initialization */
  MX_SPI3_Init();

  /* Autonomous Mode initialization */
  MX_AutonomousMode_Init();

  /* USER CODE BEGIN TxRx_MultiConf_Init */

  /* USER CODE END TxRx_MultiConf_Init */
}

/**
  * @brief TxRx application MultiConf scenario de-initialization
  * @param None
  * @retval None
  */
void MX_TxRx_MultiConf_DeInit(void)
{
  /* LPBAM LPTIM1 De-initialization */
  MX_LPTIM1_DeInit();

  /* LPBAM SPI3 De-initialization */
  MX_SPI3_DeInit();

  /* Autonomous mode de-initialization */
  MX_AutonomousMode_DeInit();

  /* USER CODE BEGIN TxRx_MultiConf_DeInit */

  /* USER CODE END TxRx_MultiConf_DeInit */
}

/**
  * @brief TxRx application MultiConf scenario link
  * @param None
  * @retval None
  */
void MX_TxRx_MultiConf_Link(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN TxRx_MultiConf_Link 0 */

  /* USER CODE END TxRx_MultiConf_Link 0 */

  /* Link Coupled_Transmit queue to DMA channel */
  MX_Coupled_Transmit_Q_Link(&hdma[COUPLED_TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN LINK COUPLED_TRANSMIT_Q_IDX */

  /* USER CODE END LINK COUPLED_TRANSMIT_Q_IDX */

  /* Link Transmit queue to DMA channel */
  MX_Transmit_Q_Link(&hdma[TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN LINK TRANSMIT_Q_IDX */

  /* USER CODE END LINK TRANSMIT_Q_IDX */

  /* USER CODE BEGIN TxRx_MultiConf_Link 1 */
  __HAL_LINKDMA(&hspi3, hdmarx, hdma[COUPLED_TRANSMIT_Q_IDX]);
  __HAL_LINKDMA(&hspi3, hdmatx, hdma[TRANSMIT_Q_IDX]);
  /* USER CODE END TxRx_MultiConf_Link 1 */
}

/**
  * @brief TxRx application MultiConf scenario unlink
  * @param hdma :Pointer to a DMA_HandleTypeDef structure that contains the configuration information for the specified
  *              DMA Channel
  * @retval None
  */
void MX_TxRx_MultiConf_UnLink(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN TxRx_MultiConf_UnLink 0 */

  /* USER CODE END TxRx_MultiConf_UnLink 0 */

  /* LPBAM unLink Coupled_Transmit queue to DMA channel */
  MX_Coupled_Transmit_Q_UnLink(&hdma[COUPLED_TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN UNLINK COUPLED_TRANSMIT_Q_IDX */

  /* USER CODE END UNLINK COUPLED_TRANSMIT_Q_IDX */

  /* LPBAM unLink Transmit queue to DMA channel */
  MX_Transmit_Q_UnLink(&hdma[TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN UNLINK TRANSMIT_Q_IDX */

  /* USER CODE END UNLINK TRANSMIT_Q_IDX */

  /* USER CODE BEGIN TxRx_MultiConf_UnLink 1 */

  /* USER CODE END TxRx_MultiConf_UnLink 1 */
}

/**
  * @brief TxRx application MultiConf scenario start
  * @retval None
  */
void MX_TxRx_MultiConf_Start(DMA_HandleTypeDef *hdma)
{
  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[COUPLED_TRANSMIT_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[TRANSMIT_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TxRx_MultiConf_Start */

  /* LPBAM LPTIM1 start PWM generation */
  if (HAL_LPTIM_PWM_Start(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END TxRx_MultiConf_Start */
}

/**
  * @brief TxRx application MultiConf scenario stop
  * @retval None
  */
void MX_TxRx_MultiConf_Stop(DMA_HandleTypeDef *hdma)
{
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[COUPLED_TRANSMIT_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[COUPLED_TRANSMIT_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[COUPLED_TRANSMIT_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[COUPLED_TRANSMIT_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[COUPLED_TRANSMIT_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[COUPLED_TRANSMIT_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[TRANSMIT_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[TRANSMIT_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[TRANSMIT_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[TRANSMIT_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[TRANSMIT_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[TRANSMIT_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* USER CODE BEGIN TxRx_MultiConf_Stop */

  /* LPBAM LPTIM1 stop PWM generation */
  if (HAL_LPTIM_PWM_Stop(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* USER CODE END TxRx_MultiConf_Stop */
}

/**
  * @brief TxRx application MultiConf autonomous mode init
  * @param None
  * @retval None
  */
static void MX_AutonomousMode_Init(void)
{
  /* Enable LPDMA1 Sleep Clock */
  __HAL_RCC_LPDMA1_CLK_SLEEP_ENABLE();
  /* Enable LPDMA1 Autonomous Mode */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 Sleep Clock */
  __HAL_RCC_SRAM4_CLK_SLEEP_ENABLE();
  /* Enable SRAM4 Autonomous Mode */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();

  /* Enable LPTIM1 Sleep Clock */
  __HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE();
  /* Enable LPTIM1 Autonomous Mode */
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();

  /* Enable SPI3 Sleep Clock */
  __HAL_RCC_SPI3_CLK_SLEEP_ENABLE();
  /* Enable SPI3 Autonomous Mode */
  __HAL_RCC_SPI3_CLKAM_ENABLE();

  /* USER CODE BEGIN AutonomousMode_Init */

  /* USER CODE END AutonomousMode_Init */
}

/**
  * @brief  TxRx application MultiConf autonomous mode deinit
  * @param  None
  * @retval None
  */
static void MX_AutonomousMode_DeInit(void)
{
  /* Disable LPDMA1 Sleep Clock */
  __HAL_RCC_LPDMA1_CLK_SLEEP_DISABLE();
  /* Disable LPDMA1 Autonomous Mode */
  __HAL_RCC_LPDMA1_CLKAM_DISABLE();

  /* Disable SRAM4 Sleep Clock */
  __HAL_RCC_SRAM4_CLK_SLEEP_DISABLE();
  /* Disable SRAM4 Autonomous Mode */
  __HAL_RCC_SRAM4_CLKAM_DISABLE();

  /* Disable LPTIM1 Sleep Clock */
  __HAL_RCC_LPTIM1_CLK_SLEEP_DISABLE();
  /* Disable LPTIM1 Autonomous Mode */
  __HAL_RCC_LPTIM1_CLKAM_DISABLE();

  /* Disable SPI3 Sleep Clock */
  __HAL_RCC_SPI3_CLK_SLEEP_DISABLE();
  /* Disable SPI3 Autonomous Mode */
  __HAL_RCC_SPI3_CLKAM_DISABLE();

  /* USER CODE BEGIN AutonomousMode_DeInit */

  /* USER CODE END AutonomousMode_DeInit */
}

/**
  * @brief LPTIM1 initialization.
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{
  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  LPTIM_OC_ConfigTypeDef sConfig1 = {0};

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */

  /* Set LPTIM1 instance */
  hlptim1.Instance = LPTIM1;

  /* Register LPTIM msp callbacks */
  if (HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_MSPINIT_CB_ID, MX_LPTIM1_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register LPTIM msp callbacks */
  if (HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_MSPDEINIT_CB_ID, MX_LPTIM1_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.Period = LPTIM1_PWM_PERIOD;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = LPTIM1_PWM_REPETITION;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig1.Pulse = LPTIM1_PWM_PULSE;
  sConfig1.OCPolarity = LPTIM_OCPOLARITY_HIGH;
  if (HAL_LPTIM_OC_ConfigChannel(&hlptim1, &sConfig1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief LPTIM1 de-initialization.
  * @param None
  * @retval None
  */
static void MX_LPTIM1_DeInit(void)
{
  /* USER CODE BEGIN LPTIM1_DeInit 0 */

  /* USER CODE END LPTIM1_DeInit 0 */

  /* Set LPTIM1 instance */
  hlptim1.Instance = LPTIM1;

  /* UnRegister LPTIM msp callbacks */
  if (HAL_LPTIM_UnRegisterCallback(&hlptim1, HAL_LPTIM_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister LPTIM msp callbacks */
  if (HAL_LPTIM_UnRegisterCallback(&hlptim1, HAL_LPTIM_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init LPTIM1 peripheral */
  if (HAL_LPTIM_DeInit(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN LPTIM1_DeInit 1 */

  /* USER CODE END LPTIM1_DeInit 1 */
}

/**
  * @brief LPTIM1 MSP initialization.
  * @retval None
  */
static void MX_LPTIM1_MspInit(LPTIM_HandleTypeDef* lptimHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(lptimHandle->Instance==LPTIM1)
  {
  /* USER CODE BEGIN LPTIM1_MspInit 0 */

  /* USER CODE END LPTIM1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* LPTIM1 clock enable */
    __HAL_RCC_LPTIM1_CLK_ENABLE();
  /* USER CODE BEGIN LPTIM1_MspInit 1 */

  /* USER CODE END LPTIM1_MspInit 1 */
  }
}

/**
  * @brief LPTIM1 MSP de-initialization.
  * @retval None
  */
static void MX_LPTIM1_MspDeInit(LPTIM_HandleTypeDef* lptimHandle)
{

  if (lptimHandle->Instance==LPTIM1)
  {
  /* USER CODE BEGIN LPTIM1_MspDeInit 0 */

  /* USER CODE END LPTIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM1_CLK_DISABLE();
  /* USER CODE BEGIN LPTIM1_MspDeInit 1 */

  /* USER CODE END LPTIM1_MspDeInit 1 */
  }
}

/**
  * @brief SPI3 initialization.
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{
  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */

  /* Set SPI3 instance */
  hspi3.Instance = SPI3;

  /* Register SPI msp callbacks */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPINIT_CB_ID, MX_SPI3_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register SPI msp callbacks */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPDEINIT_CB_ID, MX_SPI3_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 0x7;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_08DATA;
  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  hspi3.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
  hspi3.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_ENABLE;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP2_LPTIM1_CH1_TRG;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
  if (HAL_SPIEx_SetConfigAutonomousMode(&hspi3, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configuration in LPBAM context
  */
  __HAL_SPI_ENABLE_IT(&hspi3, SPI_IT_UDR|SPI_IT_OVR);
  if (ADV_LPBAM_SPI_EnableDMARequests(SPI3) != LPBAM_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* Register spi3 error callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_ERROR_CB_ID, SPI3_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register spi3 tx complete callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_TX_COMPLETE_CB_ID, SPI3_TxCplt_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief SPI3 de-initialization.
  * @param None
  * @retval None
  */
static void MX_SPI3_DeInit(void)
{
  /* USER CODE BEGIN SPI3_DeInit 0 */

  /* USER CODE END SPI3_DeInit 0 */

  /* Set SPI3 instance */
  hspi3.Instance = SPI3;

  /* UnRegister SPI msp callbacks */
  if (HAL_SPI_UnRegisterCallback(&hspi3, HAL_SPI_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister SPI msp callbacks */
  if (HAL_SPI_UnRegisterCallback(&hspi3, HAL_SPI_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init SPI3 peripheral */
  if (HAL_SPI_DeInit(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN SPI3_DeInit 1 */

  /* USER CODE END SPI3_DeInit 1 */
}

/**
  * @brief SPI3 MSP initialization.
  * @retval None
  */
static void MX_SPI3_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(spiHandle->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspInit 0 */

  /* USER CODE END SPI3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI3;
    PeriphClkInit.Spi3ClockSelection = RCC_SPI3CLKSOURCE_MSIK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* SPI3 clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* SPI3 interrupt Init */
    HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI3_IRQn);
  /* USER CODE BEGIN SPI3_MspInit 1 */

  /* USER CODE END SPI3_MspInit 1 */
  }
}

/**
  * @brief SPI3 MSP de-initialization.
  * @retval None
  */
static void MX_SPI3_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if (spiHandle->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspDeInit 0 */

  /* USER CODE END SPI3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);

    /* SPI3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
  /* USER CODE BEGIN SPI3_MspDeInit 1 */

  /* USER CODE END SPI3_MspDeInit 1 */
  }
}

/**
  * @brief  Transmit queue link
  * @retval None
  */
static void MX_Transmit_Q_Link(DMA_HandleTypeDef *hdma)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  hdma->InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  hdma->InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  hdma->InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  hdma->InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(hdma) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMAEx_List_LinkQ(hdma, &Transmit_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_Transmit_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  Transmit queue unlink
  * @retval None
  */
static void MX_Transmit_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink Transmit queue to DMA channel */
  if (HAL_DMAEx_List_UnLinkQ(hdma) != HAL_OK)
  {
    Error_Handler();
  }

  /* UnRegister DMA channel error callbacks */
  if (HAL_DMA_UnRegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }

  /* DMA linked list de-init */
  if (HAL_DMAEx_List_DeInit(hdma) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Transmit queue dma error callback
  * @retval None
  */
static void MX_Transmit_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Transmit_DMA_Error_Callback */

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE END Transmit_DMA_Error_Callback */
}

/**
  * @brief  Coupled_Transmit queue link
  * @retval None
  */
static void MX_Coupled_Transmit_Q_Link(DMA_HandleTypeDef *hdma)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  hdma->InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  hdma->InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  hdma->InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  hdma->InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(hdma) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMAEx_List_LinkQ(hdma, &Coupled_Transmit_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_Coupled_Transmit_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  Coupled_Transmit queue unlink
  * @retval None
  */
static void MX_Coupled_Transmit_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink Coupled_Transmit queue to DMA channel */
  if (HAL_DMAEx_List_UnLinkQ(hdma) != HAL_OK)
  {
    Error_Handler();
  }

  /* UnRegister DMA channel error callbacks */
  if (HAL_DMA_UnRegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }

  /* DMA linked list de-init */
  if (HAL_DMAEx_List_DeInit(hdma) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Coupled_Transmit queue dma error callback
  * @retval None
  */
static void MX_Coupled_Transmit_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Coupled_Transmit_DMA_Error_Callback */

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE END Coupled_Transmit_DMA_Error_Callback */
}

/* USER CODE BEGIN TxRx_MultiConf_Config */
/**
  * @brief  spi3 error callback
  * @param  None
  * @retval None
  */
static void SPI3_Error_Callback(SPI_HandleTypeDef *hspi3)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
}

/**
  * @brief  spi3 tx complete callback
  * @param  None
  * @retval None
  */
static void SPI3_TxCplt_Callback(SPI_HandleTypeDef *hspi3)
{
}
/* USER CODE END TxRx_MultiConf_Config */

/**
  * @brief DMA channel NVIC configuration
  * @retval None
  */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority)
{
  IRQn_Type irq = LPDMA1_Channel0_IRQn;

  /* Check DMA channel instance */
  switch ((uint32_t)hdma->Instance)
  {
    case (uint32_t)LPDMA1_Channel0: /* DMA channel_0 */
    {
      irq = LPDMA1_Channel0_IRQn;
      break;
    }

    case (uint32_t)LPDMA1_Channel1: /* DMA channel_1 */
    {
      irq = LPDMA1_Channel1_IRQn;
      break;
    }

    case (uint32_t)LPDMA1_Channel2: /* DMA channel_2 */
    {
      irq = LPDMA1_Channel2_IRQn;
      break;
    }

    case (uint32_t)LPDMA1_Channel3: /* DMA channel_3 */
    {
      irq = LPDMA1_Channel3_IRQn;
      break;
    }
  }

  /* Enable NVIC for DMA channel */
  HAL_NVIC_SetPriority(irq, PreemptPriority, SubPriority);
  HAL_NVIC_EnableIRQ(irq);
}
