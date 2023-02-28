/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_txrx_sendbypack_config.c
  * @author  MCD Application Team
  * @brief   Provide LPBAM TxRx application SendByPack configuration services.
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
#define RECEIVE_Q_IDX (0U)
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
extern UART_HandleTypeDef hlpuart1;

/* LPBAM queues declaration */
extern DMA_QListTypeDef Receive_Q;
extern DMA_QListTypeDef Transmit_Q;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/

/* LPBAM LPTIM1 management APIs */
static void MX_LPTIM1_Init(void);
static void MX_LPTIM1_MspInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_MspDeInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_DeInit(void);

/* LPBAM LPUART1 management APIs */
static void MX_LPUART1_Init(void);
static void MX_LPUART1_MspInit(UART_HandleTypeDef *hlpuart1);
static void MX_LPUART1_MspDeInit(UART_HandleTypeDef *hlpuart1);
static void MX_LPUART1_DeInit(void);

/* LPBAM autonomous mode management APIs */
static void MX_AutonomousMode_Init(void);
static void MX_AutonomousMode_DeInit(void);

/* LPBAM queue linking/unlinking APIs */
static void MX_Receive_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_Receive_Q_UnLink(DMA_HandleTypeDef *hdma);
static void MX_Transmit_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_Transmit_Q_UnLink(DMA_HandleTypeDef *hdma);

/* LPBAM DMA user callback APIs */
static void MX_Receive_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
static void MX_Receive_Q_DMA_TC_Callback(DMA_HandleTypeDef *hdma);
static void MX_Transmit_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
/* LPBAM DMA NVIC API */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority);

/* USER CODE BEGIN PFP */
static void MX_LPUART1_Error_Callback(UART_HandleTypeDef *hlpuart1);
/* USER CODE END PFP */

/* Exported functions ----------------------------------------------------------------------------------------------*/
/**
  * @brief  TxRx application SendByPack scenario initialization
  * @param  None
  * @retval None
  */
void MX_TxRx_SendByPack_Init(void)
{
  /* LPBAM LPTIM1 initialization */
  MX_LPTIM1_Init();

  /* LPBAM LPUART1 initialization */
  MX_LPUART1_Init();

  /* Autonomous Mode initialization */
  MX_AutonomousMode_Init();

  /* USER CODE BEGIN TxRx_SendByPack_Init */

  /* USER CODE END TxRx_SendByPack_Init */
}

/**
  * @brief TxRx application SendByPack scenario de-initialization
  * @param None
  * @retval None
  */
void MX_TxRx_SendByPack_DeInit(void)
{
  /* LPBAM LPTIM1 De-initialization */
  MX_LPTIM1_DeInit();

  /* LPBAM LPUART1 De-initialization */
  MX_LPUART1_DeInit();

  /* Autonomous mode de-initialization */
  MX_AutonomousMode_DeInit();

  /* USER CODE BEGIN TxRx_SendByPack_DeInit */

  /* USER CODE END TxRx_SendByPack_DeInit */
}

/**
  * @brief TxRx application SendByPack scenario link
  * @param None
  * @retval None
  */
void MX_TxRx_SendByPack_Link(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN TxRx_SendByPack_Link 0 */

  /* USER CODE END TxRx_SendByPack_Link 0 */

  /* Link Receive queue to DMA channel */
  MX_Receive_Q_Link(&hdma[RECEIVE_Q_IDX]);

  /* USER CODE BEGIN LINK RECEIVE_Q_IDX */

  /* USER CODE END LINK RECEIVE_Q_IDX */

  /* Link Transmit queue to DMA channel */
  MX_Transmit_Q_Link(&hdma[TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN LINK TRANSMIT_Q_IDX */

  /* USER CODE END LINK TRANSMIT_Q_IDX */

  /* USER CODE BEGIN TxRx_SendByPack_Link 1 */

  /* USER CODE END TxRx_SendByPack_Link 1 */
}

/**
  * @brief TxRx application SendByPack scenario unlink
  * @param hdma :Pointer to a DMA_HandleTypeDef structure that contains the configuration information for the specified
  *              DMA Channel
  * @retval None
  */
void MX_TxRx_SendByPack_UnLink(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN TxRx_SendByPack_UnLink 0 */

  /* USER CODE END TxRx_SendByPack_UnLink 0 */

  /* LPBAM unLink Receive queue to DMA channel */
  MX_Receive_Q_UnLink(&hdma[RECEIVE_Q_IDX]);

  /* USER CODE BEGIN UNLINK RECEIVE_Q_IDX */

  /* USER CODE END UNLINK RECEIVE_Q_IDX */

  /* LPBAM unLink Transmit queue to DMA channel */
  MX_Transmit_Q_UnLink(&hdma[TRANSMIT_Q_IDX]);

  /* USER CODE BEGIN UNLINK TRANSMIT_Q_IDX */

  /* USER CODE END UNLINK TRANSMIT_Q_IDX */

  /* USER CODE BEGIN TxRx_SendByPack_UnLink 1 */

  /* USER CODE END TxRx_SendByPack_UnLink 1 */
}

/**
  * @brief TxRx application SendByPack scenario start
  * @retval None
  */
void MX_TxRx_SendByPack_Start(DMA_HandleTypeDef *hdma)
{
  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[RECEIVE_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[TRANSMIT_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TxRx_SendByPack_Start */

  /* LPBAM LPTIM1 start PWM generation */
  if (HAL_LPTIM_PWM_Start(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END TxRx_SendByPack_Start */
}

/**
  * @brief TxRx application SendByPack scenario stop
  * @retval None
  */
void MX_TxRx_SendByPack_Stop(DMA_HandleTypeDef *hdma)
{
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[RECEIVE_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[RECEIVE_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[RECEIVE_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[RECEIVE_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[RECEIVE_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[RECEIVE_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
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

  /* USER CODE BEGIN TxRx_SendByPack_Stop */

  /* LPBAM LPTIM1 stop PWM generation */
  if (HAL_LPTIM_PWM_Stop(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* USER CODE END TxRx_SendByPack_Stop */
}

/**
  * @brief TxRx application SendByPack autonomous mode init
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

  /* Enable LPUART1 Sleep Clock */
  __HAL_RCC_LPUART1_CLK_SLEEP_ENABLE();
  /* Enable LPUART1 Autonomous Mode */
  __HAL_RCC_LPUART1_CLKAM_ENABLE();

  /* USER CODE BEGIN AutonomousMode_Init */

  /* USER CODE END AutonomousMode_Init */
}

/**
  * @brief  TxRx application SendByPack autonomous mode deinit
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

  /* Disable LPUART1 Sleep Clock */
  __HAL_RCC_LPUART1_CLK_SLEEP_DISABLE();
  /* Disable LPUART1 Autonomous Mode */
  __HAL_RCC_LPUART1_CLKAM_DISABLE();

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
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
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
  * @brief LPUART1 initialization.
  * @param None
  * @retval None
  */

static void MX_LPUART1_Init(void)
{
  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  UART_AutonomousModeConfTypeDef sConfigLpuart1 = {0};

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */

  /* Set LPUART1 instance */
  hlpuart1.Instance = LPUART1;

  /* Register UART msp callbacks */
  if (HAL_UART_RegisterCallback(&hlpuart1, HAL_UART_MSPINIT_CB_ID, MX_LPUART1_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register UART msp callbacks */
  if (HAL_UART_RegisterCallback(&hlpuart1, HAL_UART_MSPDEINIT_CB_ID, MX_LPUART1_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = LPUART1_BAUDRATE;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  hlpuart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  hlpuart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigLpuart1.AutonomousModeState = UART_AUTONOMOUS_MODE_ENABLE;
  sConfigLpuart1.TriggerSelection = LPUART_LPTIM1_OUT_TRG;
  sConfigLpuart1.TriggerPolarity = UART_TRIG_POLARITY_RISING;
  sConfigLpuart1.DataSize = LPUART1_DEFAULT_PAKETSIZE;
  sConfigLpuart1.IdleFrame = UART_IDLE_FRAME_ENABLE;
  if (HAL_UARTEx_SetConfigAutonomousMode(&hlpuart1, &sConfigLpuart1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configuration in LPBAM context
  */
  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_ERR|UART_IT_PE);
  if (ADV_LPBAM_UART_EnableDMARequests(LPUART1) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableStopMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* Register lpuart1 error callback */
  if (HAL_UART_RegisterCallback(&hlpuart1, HAL_UART_ERROR_CB_ID, MX_LPUART1_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief LPUART1 de-initialization.
  * @param None
  * @retval None
  */
static void MX_LPUART1_DeInit(void)
{
  /* USER CODE BEGIN LPUART1_DeInit 0 */

  /* USER CODE END LPUART1_DeInit 0 */

  /* Set LPUART1 instance */
  hlpuart1.Instance = LPUART1;

  /* UnRegister UART msp callbacks */
  if (HAL_UART_UnRegisterCallback(&hlpuart1, HAL_UART_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister UART msp callbacks */
  if (HAL_UART_UnRegisterCallback(&hlpuart1, HAL_UART_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init LPUART1 peripheral */
  if (HAL_UART_DeInit(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN LPUART1_DeInit 1 */

  /* USER CODE END LPUART1_DeInit 1 */
}

/**
  * @brief LPUART1 MSP initialization.
  * @retval None
  */
static void MX_LPUART1_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**LPUART1 GPIO Configuration
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }
}

/**
  * @brief LPUART1 MSP de-initialization.
  * @retval None
  */
static void MX_LPUART1_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if (uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();

    /**LPUART1 GPIO Configuration
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
}

/**
  * @brief  Receive queue link
  * @retval None
  */
static void MX_Receive_Q_Link(DMA_HandleTypeDef *hdma)
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
  if (HAL_DMAEx_List_LinkQ(hdma, &Receive_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TC
                              |DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_Receive_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_CPLT_CB_ID, MX_Receive_Q_DMA_TC_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  Receive queue unlink
  * @retval None
  */
static void MX_Receive_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink Receive queue to DMA channel */
  if (HAL_DMAEx_List_UnLinkQ(hdma) != HAL_OK)
  {
    Error_Handler();
  }

  /* UnRegister DMA channel error callbacks */
  if (HAL_DMA_UnRegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register DMA channel transfer complete callbacks */
  if (HAL_DMA_UnRegisterCallback(hdma, HAL_DMA_XFER_CPLT_CB_ID) != HAL_OK)
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
  * @brief  Receive queue dma error callback
  * @retval None
  */
static void MX_Receive_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Receive_DMA_Error_Callback */

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE END Receive_DMA_Error_Callback */
}

/**
  * @brief  Receive queue dma transfer complete callbacks
  * @retval None
  */
static void MX_Receive_Q_DMA_TC_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Receive_DMA_TC_Callback */

  /* USER CODE END Receive_DMA_TC_Callback */
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

/* USER CODE BEGIN TxRx_SendByPack_Config */
/**
  * @brief  lpuart1 error callback
  * @param  None
  * @retval None
  */
static void MX_LPUART1_Error_Callback(UART_HandleTypeDef *hlpuart1)
{
  /* USER CODE BEGIN LPUART1_Error_Callback */

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE END LPUART1_Error_Callback */
}
/* USER CODE END TxRx_SendByPack_Config */

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
