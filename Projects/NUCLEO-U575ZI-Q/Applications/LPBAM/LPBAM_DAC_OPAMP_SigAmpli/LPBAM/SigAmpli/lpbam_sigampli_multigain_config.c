/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_sigampli_multigain_config.c
  * @author MCD Application Team
  * @brief  Provide LPBAM SigAmpli application MultiGain scenario configuration services.
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
#include "lpbam_sigampli.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define --------------------------------------------------------------------------------------------------*/
#define DAC_CONVERSION_Q_IDX (0U)
#define OPAMP_CONFIG_Q_IDX (1U)
#define DMA_TIMEOUT_DURATION (0x1000U)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* External variables ------------------------------------------------------------------------------------------------*/
/* IP handler declaration */
extern DAC_HandleTypeDef hdac1;
extern LPTIM_HandleTypeDef hlptim3;
extern OPAMP_HandleTypeDef hopamp1;

/* LPBAM queues declaration */
extern DMA_QListTypeDef DAC_Conversion_Q;
extern DMA_QListTypeDef OPAMP_Config_Q;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/

/* LPBAM DAC1 management APIs */
static void MX_DAC1_Init(void);
static void MX_DAC1_MspInit(DAC_HandleTypeDef *hdac1);
static void MX_DAC1_MspDeInit(DAC_HandleTypeDef *hdac1);
static void MX_DAC1_DeInit(void);

/* LPBAM LPTIM3 management APIs */
static void MX_LPTIM3_Init(void);
static void MX_LPTIM3_MspInit(LPTIM_HandleTypeDef *hlptim3);
static void MX_LPTIM3_MspDeInit(LPTIM_HandleTypeDef *hlptim3);
static void MX_LPTIM3_DeInit(void);

/* LPBAM OPAMP1 management APIs */
static void MX_OPAMP1_Init(void);
static void MX_OPAMP1_MspInit(OPAMP_HandleTypeDef *hopamp1);
static void MX_OPAMP1_MspDeInit(OPAMP_HandleTypeDef *hopamp1);
static void MX_OPAMP1_DeInit(void);

/* LPBAM autonomous mode management APIs */
static void MX_AutonomousMode_Init(void);
static void MX_AutonomousMode_DeInit(void);

/* LPBAM queue linking/unlinking APIs */
static void MX_DAC_Conversion_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_DAC_Conversion_Q_UnLink(DMA_HandleTypeDef *hdma);
static void MX_OPAMP_Config_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_OPAMP_Config_Q_UnLink(DMA_HandleTypeDef *hdma);

/* LPBAM DMA user callback APIs */
static void MX_DAC_Conversion_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
static void MX_OPAMP_Config_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
/* LPBAM DMA NVIC API */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority);

/* USER CODE BEGIN PFP */
static void DAC1_CH1_DMAUnderrunError_Callback(DAC_HandleTypeDef *hdac1);
/* USER CODE END PFP */

/* Exported functions ----------------------------------------------------------------------------------------------*/
/**
  * @brief  SigAmpli application MultiGain scenario initialization
  * @param  None
  * @retval None
  */
void MX_SigAmpli_MultiGain_Init(void)
{
  /* LPBAM DAC1 initialization */
  MX_DAC1_Init();

  /* LPBAM LPTIM3 initialization */
  MX_LPTIM3_Init();

  /* LPBAM OPAMP1 initialization */
  MX_OPAMP1_Init();

  /* Autonomous Mode initialization */
  MX_AutonomousMode_Init();

  /* USER CODE BEGIN SigAmpli_MultiGain_Init */

  /* USER CODE END SigAmpli_MultiGain_Init */
}

/**
  * @brief SigAmpli application MultiGain scenario de-initialization
  * @param None
  * @retval None
  */
void MX_SigAmpli_MultiGain_DeInit(void)
{
  /* LPBAM DAC1 De-initialization */
  MX_DAC1_DeInit();

  /* LPBAM LPTIM3 De-initialization */
  MX_LPTIM3_DeInit();

  /* LPBAM OPAMP1 De-initialization */
  MX_OPAMP1_DeInit();

  /* Autonomous mode de-initialization */
  MX_AutonomousMode_DeInit();

  /* USER CODE BEGIN SigAmpli_MultiGain_DeInit */

  /* USER CODE END SigAmpli_MultiGain_DeInit */
}

/**
  * @brief SigAmpli application MultiGain scenario link
  * @param None
  * @retval None
  */
void MX_SigAmpli_MultiGain_Link(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN SigAmpli_MultiGain_Link 0 */

  /* USER CODE END SigAmpli_MultiGain_Link 0 */

  /* Link DAC_Conversion queue to DMA channel */
  MX_DAC_Conversion_Q_Link(&hdma[DAC_CONVERSION_Q_IDX]);

  /* USER CODE BEGIN LINK DAC_CONVERSION_Q_IDX */

  /* USER CODE END LINK DAC_CONVERSION_Q_IDX */

  /* Link OPAMP_Config queue to DMA channel */
  MX_OPAMP_Config_Q_Link(&hdma[OPAMP_CONFIG_Q_IDX]);

  /* USER CODE BEGIN LINK OPAMP_CONFIG_Q_IDX */

  /* USER CODE END LINK OPAMP_CONFIG_Q_IDX */

  /* USER CODE BEGIN SigAmpli_MultiGain_Link 1 */

  /* USER CODE END SigAmpli_MultiGain_Link 1 */
}

/**
  * @brief SigAmpli application MultiGain scenario unlink
  * @param hdma :Pointer to a DMA_HandleTypeDef structure that contains the configuration information for the specified
  *              DMA Channel
  * @retval None
  */
void MX_SigAmpli_MultiGain_UnLink(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN SigAmpli_MultiGain_UnLink 0 */

  /* USER CODE END SigAmpli_MultiGain_UnLink 0 */

  /* LPBAM unLink DAC_Conversion queue to DMA channel */
  MX_DAC_Conversion_Q_UnLink(&hdma[DAC_CONVERSION_Q_IDX]);

  /* USER CODE BEGIN UNLINK DAC_CONVERSION_Q_IDX */

  /* USER CODE END UNLINK DAC_CONVERSION_Q_IDX */

  /* LPBAM unLink OPAMP_Config queue to DMA channel */
  MX_OPAMP_Config_Q_UnLink(&hdma[OPAMP_CONFIG_Q_IDX]);

  /* USER CODE BEGIN UNLINK OPAMP_CONFIG_Q_IDX */

  /* USER CODE END UNLINK OPAMP_CONFIG_Q_IDX */

  /* USER CODE BEGIN SigAmpli_MultiGain_UnLink 1 */

  /* USER CODE END SigAmpli_MultiGain_UnLink 1 */
}

/**
  * @brief SigAmpli application MultiGain scenario start
  * @retval None
  */
void MX_SigAmpli_MultiGain_Start(DMA_HandleTypeDef *hdma)
{
  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[DAC_CONVERSION_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[OPAMP_CONFIG_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN SigAmpli_MultiGain_Start */

  /* LPTIM3 PWM start */
  if (HAL_LPTIM_PWM_Start(&hlptim3, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END SigAmpli_MultiGain_Start */
}

/**
  * @brief SigAmpli application MultiGain scenario stop
  * @retval None
  */
void MX_SigAmpli_MultiGain_Stop(DMA_HandleTypeDef *hdma)
{
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[DAC_CONVERSION_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[DAC_CONVERSION_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[DAC_CONVERSION_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[DAC_CONVERSION_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[DAC_CONVERSION_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[DAC_CONVERSION_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[OPAMP_CONFIG_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[OPAMP_CONFIG_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[OPAMP_CONFIG_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[OPAMP_CONFIG_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[OPAMP_CONFIG_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[OPAMP_CONFIG_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* USER CODE BEGIN SigAmpli_MultiGain_Stop */

  /* Stop DAC conversion */
  if (HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Stop PWM signal */
  if (HAL_LPTIM_PWM_Stop(&hlptim3, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* USER CODE END SigAmpli_MultiGain_Stop */
}

/**
  * @brief SigAmpli application MultiGain autonomous mode init
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

  /* Enable DAC1 Sleep Clock */
  __HAL_RCC_DAC1_CLK_SLEEP_ENABLE();
  /* Enable DAC1 Autonomous Mode */
  __HAL_RCC_DAC1_CLKAM_ENABLE();

  /* Enable LPTIM3 Sleep Clock */
  __HAL_RCC_LPTIM3_CLK_SLEEP_ENABLE();
  /* Enable LPTIM3 Autonomous Mode */
  __HAL_RCC_LPTIM3_CLKAM_ENABLE();

  /* Enable OPAMP1 Sleep Clock */
  __HAL_RCC_OPAMP_CLK_SLEEP_ENABLE();
  /* Enable OPAMP1 Autonomous Mode */
  __HAL_RCC_OPAMP_CLKAM_ENABLE();

  /* USER CODE BEGIN AutonomousMode_Init */

  /* USER CODE END AutonomousMode_Init */
}

/**
  * @brief  SigAmpli application MultiGain autonomous mode deinit
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

  /* Disable DAC1 Sleep Clock */
  __HAL_RCC_DAC1_CLK_SLEEP_DISABLE();
  /* Disable DAC1 Autonomous Mode */
  __HAL_RCC_DAC1_CLKAM_DISABLE();

  /* Disable LPTIM3 Sleep Clock */
  __HAL_RCC_LPTIM3_CLK_SLEEP_DISABLE();
  /* Disable LPTIM3 Autonomous Mode */
  __HAL_RCC_LPTIM3_CLKAM_DISABLE();

  /* Disable OPAMP1 Sleep Clock */
  __HAL_RCC_OPAMP_CLK_SLEEP_DISABLE();
  /* Disable OPAMP1 Autonomous Mode */
  __HAL_RCC_OPAMP_CLKAM_DISABLE();

  /* USER CODE BEGIN AutonomousMode_DeInit */

  /* USER CODE END AutonomousMode_DeInit */
}

/**
  * @brief DAC1 initialization.
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{
  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};
  DAC_AutonomousModeConfTypeDef sAutonomousMode = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /* Set DAC1 instance */
  hdac1.Instance = DAC1;

  /* Register DAC msp callbacks */
  if (HAL_DAC_RegisterCallback(&hdac1, HAL_DAC_MSPINIT_CB_ID, MX_DAC1_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register DAC msp callbacks */
  if (HAL_DAC_RegisterCallback(&hdac1, HAL_DAC_MSPDEINIT_CB_ID, MX_DAC1_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DAC_DeInit(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_ENABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_LPTIM3_OUT;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_USER;
  sConfig.DAC_TrimmingValue = DAC_TRIMMING_VALUE;
  sConfig.DAC_SampleAndHoldConfig.DAC_SampleTime = DAC_SAMPLE_TIME;
  sConfig.DAC_SampleAndHoldConfig.DAC_HoldTime = DAC_HOLD_TIME;
  sConfig.DAC_SampleAndHoldConfig.DAC_RefreshTime = DAC_REFRECH_TIME;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Autonomous Mode
  */
  sAutonomousMode.AutonomousModeState = DAC_AUTONOMOUS_MODE_ENABLE;
  if (HAL_DACEx_SetConfigAutonomousMode(&hdac1, &sAutonomousMode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configuration in LPBAM context
  */
  __HAL_DAC_ENABLE_IT(&hdac1, DAC_IT_DMAUDR1);
  if (ADV_LPBAM_DAC_EnableDMARequests(DAC1, LPBAM_DAC_CHANNEL_1) != LPBAM_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* Register DAC channel1 dma underrun error callback */
  if (HAL_DAC_RegisterCallback(&hdac1, HAL_DAC_CH1_UNDERRUN_CB_ID, DAC1_CH1_DMAUnderrunError_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DAC1 de-initialization.
  * @param None
  * @retval None
  */
static void MX_DAC1_DeInit(void)
{
  /* USER CODE BEGIN DAC1_DeInit 0 */

  /* USER CODE END DAC1_DeInit 0 */

  /* Set DAC1 instance */
  hdac1.Instance = DAC1;

  /* UnRegister DAC msp callbacks */
  if (HAL_DAC_UnRegisterCallback(&hdac1, HAL_DAC_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister DAC msp callbacks */
  if (HAL_DAC_UnRegisterCallback(&hdac1, HAL_DAC_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init DAC1 peripheral */
  if (HAL_DAC_DeInit(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN DAC1_DeInit 1 */

  /* USER CODE END DAC1_DeInit 1 */
}

/**
  * @brief DAC1 MSP initialization.
  * @retval None
  */
static void MX_DAC1_MspInit(DAC_HandleTypeDef* dacHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(dacHandle->Instance==DAC1)
  {
  /* USER CODE BEGIN DAC1_MspInit 0 */

  /* USER CODE END DAC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC|RCC_PERIPHCLK_DAC1;
    PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_MSIK;
    PeriphClkInit.Dac1ClockSelection = RCC_DAC1CLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* DAC1 clock enable */
    __HAL_RCC_DAC1_CLK_ENABLE();

    /* DAC1 interrupt Init */
    HAL_NVIC_SetPriority(DAC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DAC1_IRQn);
  /* USER CODE BEGIN DAC1_MspInit 1 */

  /* USER CODE END DAC1_MspInit 1 */
  }
}

/**
  * @brief DAC1 MSP de-initialization.
  * @retval None
  */
static void MX_DAC1_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if (dacHandle->Instance==DAC1)
  {
  /* USER CODE BEGIN DAC1_MspDeInit 0 */

  /* USER CODE END DAC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC1_CLK_DISABLE();

    /* DAC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(DAC1_IRQn);
  /* USER CODE BEGIN DAC1_MspDeInit 1 */

  /* USER CODE END DAC1_MspDeInit 1 */
  }
}

/**
  * @brief LPTIM3 initialization.
  * @param None
  * @retval None
  */
static void MX_LPTIM3_Init(void)
{
  /* USER CODE BEGIN LPTIM3_Init 0 */

  /* USER CODE END LPTIM3_Init 0 */

  LPTIM_OC_ConfigTypeDef sConfig1 = {0};

  /* USER CODE BEGIN LPTIM3_Init 1 */

  /* USER CODE END LPTIM3_Init 1 */

  /* Set LPTIM3 instance */
  hlptim3.Instance = LPTIM3;

  /* Register LPTIM msp callbacks */
  if (HAL_LPTIM_RegisterCallback(&hlptim3, HAL_LPTIM_MSPINIT_CB_ID, MX_LPTIM3_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register LPTIM msp callbacks */
  if (HAL_LPTIM_RegisterCallback(&hlptim3, HAL_LPTIM_MSPDEINIT_CB_ID, MX_LPTIM3_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  hlptim3.Instance = LPTIM3;
  hlptim3.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim3.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim3.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim3.Init.Period = LPTIM3_PWM_PERIOD;
  hlptim3.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim3.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim3.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim3.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim3.Init.RepetitionCounter = LPTIM3_PWM_REPETITION;
  if (HAL_LPTIM_Init(&hlptim3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig1.Pulse = LPTIM3_PWM_PULSE;
  sConfig1.OCPolarity = LPTIM_OCPOLARITY_HIGH;
  if (HAL_LPTIM_OC_ConfigChannel(&hlptim3, &sConfig1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM3_Init 2 */

  /* USER CODE END LPTIM3_Init 2 */

}

/**
  * @brief LPTIM3 de-initialization.
  * @param None
  * @retval None
  */
static void MX_LPTIM3_DeInit(void)
{
  /* USER CODE BEGIN LPTIM3_DeInit 0 */

  /* USER CODE END LPTIM3_DeInit 0 */

  /* Set LPTIM3 instance */
  hlptim3.Instance = LPTIM3;

  /* UnRegister LPTIM msp callbacks */
  if (HAL_LPTIM_UnRegisterCallback(&hlptim3, HAL_LPTIM_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister LPTIM msp callbacks */
  if (HAL_LPTIM_UnRegisterCallback(&hlptim3, HAL_LPTIM_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init LPTIM3 peripheral */
  if (HAL_LPTIM_DeInit(&hlptim3) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN LPTIM3_DeInit 1 */

  /* USER CODE END LPTIM3_DeInit 1 */
}

/**
  * @brief LPTIM3 MSP initialization.
  * @retval None
  */
static void MX_LPTIM3_MspInit(LPTIM_HandleTypeDef* lptimHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(lptimHandle->Instance==LPTIM3)
  {
  /* USER CODE BEGIN LPTIM3_MspInit 0 */

  /* USER CODE END LPTIM3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM34;
    PeriphClkInit.Lptim34ClockSelection = RCC_LPTIM34CLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* LPTIM3 clock enable */
    __HAL_RCC_LPTIM3_CLK_ENABLE();
  /* USER CODE BEGIN LPTIM3_MspInit 1 */

  /* USER CODE END LPTIM3_MspInit 1 */
  }
}

/**
  * @brief LPTIM3 MSP de-initialization.
  * @retval None
  */
static void MX_LPTIM3_MspDeInit(LPTIM_HandleTypeDef* lptimHandle)
{

  if (lptimHandle->Instance==LPTIM3)
  {
  /* USER CODE BEGIN LPTIM3_MspDeInit 0 */

  /* USER CODE END LPTIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM3_CLK_DISABLE();
  /* USER CODE BEGIN LPTIM3_MspDeInit 1 */

  /* USER CODE END LPTIM3_MspDeInit 1 */
  }
}

/**
  * @brief OPAMP1 initialization.
  * @param None
  * @retval None
  */
static void MX_OPAMP1_Init(void)
{
  /* USER CODE BEGIN OPAMP1_Init 0 */

  /* USER CODE END OPAMP1_Init 0 */

  /* USER CODE BEGIN OPAMP1_Init 1 */

  /* USER CODE END OPAMP1_Init 1 */

  /* Set OPAMP1 instance */
  hopamp1.Instance = OPAMP1;

  /* Register OPAMP msp callbacks */
  if (HAL_OPAMP_RegisterCallback(&hopamp1, HAL_OPAMP_MSP_INIT_CB_ID, MX_OPAMP1_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register OPAMP msp callbacks */
  if (HAL_OPAMP_RegisterCallback(&hopamp1, HAL_OPAMP_MSP_DEINIT_CB_ID, MX_OPAMP1_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }
  hopamp1.Instance = OPAMP1;
  hopamp1.Init.PowerSupplyRange = OPAMP_POWERSUPPLY_HIGH;
  hopamp1.Init.Mode = OPAMP_PGA_MODE;
  hopamp1.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_DAC_CH;
  hopamp1.Init.InvertingInput = OPAMP_INVERTINGINPUT_CONNECT_NO;
  hopamp1.Init.PowerMode = OPAMP_POWERMODE_LOWPOWER_NORMALSPEED;
  hopamp1.Init.PgaGain = OPAMP_PGA_GAIN_2;
  hopamp1.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
  if (HAL_OPAMP_Init(&hopamp1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OPAMP1_Init 2 */

  /* USER CODE END OPAMP1_Init 2 */

}

/**
  * @brief OPAMP1 de-initialization.
  * @param None
  * @retval None
  */
static void MX_OPAMP1_DeInit(void)
{
  /* USER CODE BEGIN OPAMP1_DeInit 0 */

  /* USER CODE END OPAMP1_DeInit 0 */

  /* Set OPAMP1 instance */
  hopamp1.Instance = OPAMP1;

  /* UnRegister OPAMP msp callbacks */
  if (HAL_OPAMP_UnRegisterCallback(&hopamp1, HAL_OPAMP_MSP_INIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister OPAMP msp callbacks */
  if (HAL_OPAMP_UnRegisterCallback(&hopamp1, HAL_OPAMP_MSP_DEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init OPAMP1 peripheral */
  if (HAL_OPAMP_DeInit(&hopamp1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN OPAMP1_DeInit 1 */

  /* USER CODE END OPAMP1_DeInit 1 */
}

/**
  * @brief OPAMP1 MSP initialization.
  * @retval None
  */
static void MX_OPAMP1_MspInit(OPAMP_HandleTypeDef* opampHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(opampHandle->Instance==OPAMP1)
  {
  /* USER CODE BEGIN OPAMP1_MspInit 0 */

  /* USER CODE END OPAMP1_MspInit 0 */
    /* OPAMP1 clock enable */
    __HAL_RCC_OPAMP_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**OPAMP1 GPIO Configuration
    PA3     ------> OPAMP1_VOUT
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN OPAMP1_MspInit 1 */

  /* USER CODE END OPAMP1_MspInit 1 */
  }
}

/**
  * @brief OPAMP1 MSP de-initialization.
  * @retval None
  */
static void MX_OPAMP1_MspDeInit(OPAMP_HandleTypeDef* opampHandle)
{

  if (opampHandle->Instance==OPAMP1)
  {
  /* USER CODE BEGIN OPAMP1_MspDeInit 0 */

  /* USER CODE END OPAMP1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_OPAMP_CLK_DISABLE();

    /**OPAMP1 GPIO Configuration
    PA3     ------> OPAMP1_VOUT
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

  /* USER CODE BEGIN OPAMP1_MspDeInit 1 */

  /* USER CODE END OPAMP1_MspDeInit 1 */
  }
}

/**
  * @brief  DAC_Conversion queue link
  * @retval None
  */
static void MX_DAC_Conversion_Q_Link(DMA_HandleTypeDef *hdma)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  hdma->InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  hdma->InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  hdma->InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  hdma->InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(hdma) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMAEx_List_LinkQ(hdma, &DAC_Conversion_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_DAC_Conversion_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  DAC_Conversion queue unlink
  * @retval None
  */
static void MX_DAC_Conversion_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink DAC_Conversion queue to DMA channel */
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
  * @brief  DAC_Conversion queue dma error callback
  * @retval None
  */
static void MX_DAC_Conversion_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN DAC_Conversion_DMA_Error_Callback */

  /* Turn on LED3 */
  BSP_LED_On(LED3);

  /* USER CODE END DAC_Conversion_DMA_Error_Callback */
}

/**
  * @brief  OPAMP_Config queue link
  * @retval None
  */
static void MX_OPAMP_Config_Q_Link(DMA_HandleTypeDef *hdma)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  hdma->InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  hdma->InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  hdma->InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  hdma->InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(hdma) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMAEx_List_LinkQ(hdma, &OPAMP_Config_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_OPAMP_Config_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  OPAMP_Config queue unlink
  * @retval None
  */
static void MX_OPAMP_Config_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink OPAMP_Config queue to DMA channel */
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
  * @brief  OPAMP_Config queue dma error callback
  * @retval None
  */
static void MX_OPAMP_Config_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN OPAMP_Config_DMA_Error_Callback */

  /* Turn on LED3 */
  BSP_LED_On(LED3);

  /* USER CODE END OPAMP_Config_DMA_Error_Callback */
}

/* USER CODE BEGIN SigAmpli_MultiGain_Config */
/**
  * @brief  dac1 channel1 dma underrun error callback
  * @param  None
  * @retval None
  */
static void DAC1_CH1_DMAUnderrunError_Callback(DAC_HandleTypeDef *hdac1)
{
  /* Turn on LED3 */
  BSP_LED_On(LED3);
}
/* USER CODE END SigAmpli_MultiGain_Config */

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
