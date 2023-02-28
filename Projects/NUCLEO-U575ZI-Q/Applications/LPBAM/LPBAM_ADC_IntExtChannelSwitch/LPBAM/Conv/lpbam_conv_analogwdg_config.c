/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_conv_analogwdg_config.c
  * @author  MCD Application Team
  * @brief   Provide LPBAM Conv application AnalogWDG configuration services.
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
#include "lpbam_conv.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define --------------------------------------------------------------------------------------------------*/
#define CONVERSION_Q_IDX (0U)
#define DMA_TIMEOUT_DURATION (0x1000U)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* External variables ------------------------------------------------------------------------------------------------*/
/* IP handler declaration */
extern ADC_HandleTypeDef hadc4;
extern LPTIM_HandleTypeDef hlptim1;

/* LPBAM queues declaration */
extern DMA_QListTypeDef Conversion_Q;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/

/* LPBAM ADC4 management APIs */
static void MX_ADC4_Init(void);
static void MX_ADC4_MspInit(ADC_HandleTypeDef *hadc4);
static void MX_ADC4_MspDeInit(ADC_HandleTypeDef *hadc4);
static void MX_ADC4_DeInit(void);

/* LPBAM LPTIM1 management APIs */
static void MX_LPTIM1_Init(void);
static void MX_LPTIM1_MspInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_MspDeInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_DeInit(void);

/* LPBAM autonomous mode management APIs */
static void MX_AutonomousMode_Init(void);
static void MX_AutonomousMode_DeInit(void);

/* LPBAM queue linking/unlinking APIs */
static void MX_Conversion_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_Conversion_Q_UnLink(DMA_HandleTypeDef *hdma);

/* LPBAM DMA user callback APIs */
static void MX_Conversion_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
/* LPBAM DMA NVIC API */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority);

/* USER CODE BEGIN PFP */
static void ADC4_Error_Callback(ADC_HandleTypeDef *hadc4);
static void ADC4_AnalogWDG1_Callback(ADC_HandleTypeDef *hadc4);
/* USER CODE END PFP */

/* Exported functions ----------------------------------------------------------------------------------------------*/
/**
  * @brief  Conv application AnalogWDG scenario initialization
  * @param  None
  * @retval None
  */
void MX_Conv_AnalogWDG_Init(void)
{
  /* LPBAM ADC4 initialization */
  MX_ADC4_Init();

  /* LPBAM LPTIM1 initialization */
  MX_LPTIM1_Init();

  /* Autonomous Mode initialization */
  MX_AutonomousMode_Init();

  /* USER CODE BEGIN Conv_AnalogWDG_Init */

  /* USER CODE END Conv_AnalogWDG_Init */
}

/**
  * @brief Conv application AnalogWDG scenario de-initialization
  * @param None
  * @retval None
  */
void MX_Conv_AnalogWDG_DeInit(void)
{
  /* LPBAM ADC4 De-initialization */
  MX_ADC4_DeInit();

  /* LPBAM LPTIM1 De-initialization */
  MX_LPTIM1_DeInit();

  /* Autonomous mode de-initialization */
  MX_AutonomousMode_DeInit();

  /* USER CODE BEGIN Conv_AnalogWDG_DeInit */

  /* USER CODE END Conv_AnalogWDG_DeInit */
}

/**
  * @brief Conv application AnalogWDG scenario link
  * @param None
  * @retval None
  */
void MX_Conv_AnalogWDG_Link(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Conv_AnalogWDG_Link 0 */

  /* USER CODE END Conv_AnalogWDG_Link 0 */

  /* Link Conversion queue to DMA channel */
  MX_Conversion_Q_Link(&hdma[CONVERSION_Q_IDX]);

  /* USER CODE BEGIN LINK CONVERSION_Q_IDX */

  /* USER CODE END LINK CONVERSION_Q_IDX */

  /* USER CODE BEGIN Conv_AnalogWDG_Link 1 */

  /* USER CODE END Conv_AnalogWDG_Link 1 */
}

/**
  * @brief Conv application AnalogWDG scenario unlink
  * @param hdma :Pointer to a DMA_HandleTypeDef structure that contains the configuration information for the specified
  *              DMA Channel
  * @retval None
  */
void MX_Conv_AnalogWDG_UnLink(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Conv_AnalogWDG_UnLink 0 */

  /* USER CODE END Conv_AnalogWDG_UnLink 0 */

  /* LPBAM unLink Conversion queue to DMA channel */
  MX_Conversion_Q_UnLink(&hdma[CONVERSION_Q_IDX]);

  /* USER CODE BEGIN UNLINK CONVERSION_Q_IDX */

  /* USER CODE END UNLINK CONVERSION_Q_IDX */

  /* USER CODE BEGIN Conv_AnalogWDG_UnLink 1 */

  /* USER CODE END Conv_AnalogWDG_UnLink 1 */
}

/**
  * @brief Conv application AnalogWDG scenario start
  * @retval None
  */
void MX_Conv_AnalogWDG_Start(DMA_HandleTypeDef *hdma)
{
  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[CONVERSION_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Conv_AnalogWDG_Start */

  /* LPBAM LPTIM1 start PWM generation */
  if (HAL_LPTIM_PWM_Start(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END Conv_AnalogWDG_Start */
}

/**
  * @brief Conv application AnalogWDG scenario stop
  * @retval None
  */
void MX_Conv_AnalogWDG_Stop(DMA_HandleTypeDef *hdma)
{
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[CONVERSION_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[CONVERSION_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[CONVERSION_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[CONVERSION_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[CONVERSION_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[CONVERSION_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* USER CODE BEGIN Conv_AnalogWDG_Stop */

  /* LPBAM LPTIM1 stop PWM generation */
  if (HAL_LPTIM_PWM_Stop(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* USER CODE END Conv_AnalogWDG_Stop */
}

/**
  * @brief Conv application AnalogWDG autonomous mode init
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

  /* Enable ADC4 Sleep Clock */
  __HAL_RCC_ADC4_CLK_SLEEP_ENABLE();
  /* Enable ADC4 Autonomous Mode */
  __HAL_RCC_ADC4_CLKAM_ENABLE();

  /* Enable LPTIM1 Sleep Clock */
  __HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE();
  /* Enable LPTIM1 Autonomous Mode */
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();

  /* USER CODE BEGIN AutonomousMode_Init */

  /* USER CODE END AutonomousMode_Init */
}

/**
  * @brief  Conv application AnalogWDG autonomous mode deinit
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

  /* Disable ADC4 Sleep Clock */
  __HAL_RCC_ADC4_CLK_SLEEP_DISABLE();
  /* Disable ADC4 Autonomous Mode */
  __HAL_RCC_ADC4_CLKAM_DISABLE();

  /* Disable LPTIM1 Sleep Clock */
  __HAL_RCC_LPTIM1_CLK_SLEEP_DISABLE();
  /* Disable LPTIM1 Autonomous Mode */
  __HAL_RCC_LPTIM1_CLKAM_DISABLE();

  /* USER CODE BEGIN AutonomousMode_DeInit */

  /* USER CODE END AutonomousMode_DeInit */
}

/**
  * @brief ADC4 initialization.
  * @param None
  * @retval None
  */
static void MX_ADC4_Init(void)
{
  /* USER CODE BEGIN ADC4_Init 0 */

  /* USER CODE END ADC4_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC4_Init 1 */

  /* USER CODE END ADC4_Init 1 */

  /* Set ADC4 instance */
  hadc4.Instance = ADC4;

  /* Register ADC msp callbacks */
  if (HAL_ADC_RegisterCallback(&hadc4, HAL_ADC_MSPINIT_CB_ID, MX_ADC4_MspInit) != HAL_OK)
  {
    Error_Handler();
  }
  /* Register ADC msp callbacks */
  if (HAL_ADC_RegisterCallback(&hadc4, HAL_ADC_MSPDEINIT_CB_ID, MX_ADC4_MspDeInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Common config
  */
  hadc4.Instance = ADC4;
  hadc4.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc4.Init.Resolution = ADC_RESOLUTION_12B;
  hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc4.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc4.Init.LowPowerAutoPowerOff = ADC_LOW_POWER_AUTOFF_DPD;
  hadc4.Init.LowPowerAutoWait = DISABLE;
  hadc4.Init.ContinuousConvMode = DISABLE;
  hadc4.Init.NbrOfConversion = 1;
  hadc4.Init.DiscontinuousConvMode = DISABLE;
  hadc4.Init.ExternalTrigConv = ADC4_EXTERNALTRIG_LPTIM1_CH1;
  hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc4.Init.DMAContinuousRequests = ENABLE;
  hadc4.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_LOW;
  hadc4.Init.VrefProtection = ADC_VREF_PPROT_NONE;
  hadc4.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc4.Init.SamplingTimeCommon1 = ADC4_SAMPLETIME_814CYCLES_5;
  hadc4.Init.SamplingTimeCommon2 = ADC4_SAMPLETIME_1CYCLE_5;
  hadc4.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC4_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC4_SAMPLINGTIME_COMMON_1;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configuration in LPBAM context
  */
  /* USER CODE BEGIN ADC4_Init Calibration */

  /*
   * ADC4 Calibration
   */
  if (HAL_ADCEx_Calibration_Start(&hadc4, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END ADC4_Init Calibration */
  __HAL_ADC_ENABLE_IT(&hadc4, ADC_IT_OVR);
  if (ADV_LPBAM_ADC_EnableDMARequests(ADC4) != LPBAM_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC4_Init 2 */

  /* Register ADC4 error callback */
  if (HAL_ADC_RegisterCallback(&hadc4, HAL_ADC_ERROR_CB_ID, ADC4_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register ADC4 analog watchdog callback */
  if (HAL_ADC_RegisterCallback(&hadc4, HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID, ADC4_AnalogWDG1_Callback) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END ADC4_Init 2 */

}

/**
  * @brief ADC4 de-initialization.
  * @param None
  * @retval None
  */
static void MX_ADC4_DeInit(void)
{
  /* USER CODE BEGIN ADC4_DeInit 0 */

  /* USER CODE END ADC4_DeInit 0 */

  /* Set ADC4 instance */
  hadc4.Instance = ADC4;

  /* UnRegister ADC msp callbacks */
  if (HAL_ADC_UnRegisterCallback(&hadc4, HAL_ADC_MSPINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* UnRegister ADC msp callbacks */
  if (HAL_ADC_UnRegisterCallback(&hadc4, HAL_ADC_MSPDEINIT_CB_ID) != HAL_OK)
  {
    Error_Handler();
  }
  /* Init ADC4 peripheral */
  if (HAL_ADC_DeInit(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN ADC4_DeInit 1 */

  /* USER CODE END ADC4_DeInit 1 */
}

/**
  * @brief ADC4 MSP initialization.
  * @retval None
  */
static void MX_ADC4_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC4)
  {
  /* USER CODE BEGIN ADC4_MspInit 0 */
    /* Enable VDDA supply for ADC */
    HAL_PWREx_EnableVddA();
  /* USER CODE END ADC4_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
    PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_MSIK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC4 clock enable */
    __HAL_RCC_ADC4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC4 GPIO Configuration
    PC3     ------> ADC4_IN4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC4 interrupt Init */
    HAL_NVIC_SetPriority(ADC4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC4_IRQn);
  /* USER CODE BEGIN ADC4_MspInit 1 */

  /* USER CODE END ADC4_MspInit 1 */
  }
}

/**
  * @brief ADC4 MSP de-initialization.
  * @retval None
  */
static void MX_ADC4_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if (adcHandle->Instance==ADC4)
  {
  /* USER CODE BEGIN ADC4_MspDeInit 0 */

  /* USER CODE END ADC4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC4_CLK_DISABLE();

    /**ADC4 GPIO Configuration
    PC3     ------> ADC4_IN4
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);

    /* ADC4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC4_IRQn);
  /* USER CODE BEGIN ADC4_MspDeInit 1 */

  /* USER CODE END ADC4_MspDeInit 1 */
  }
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
  * @brief  Conversion queue link
  * @retval None
  */
static void MX_Conversion_Q_Link(DMA_HandleTypeDef *hdma)
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
  if (HAL_DMAEx_List_LinkQ(hdma, &Conversion_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_Conversion_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  Conversion queue unlink
  * @retval None
  */
static void MX_Conversion_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink Conversion queue to DMA channel */
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
  * @brief  Conversion queue dma error callback
  * @retval None
  */
static void MX_Conversion_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN Conversion_DMA_Error_Callback */

  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE END Conversion_DMA_Error_Callback */
}

/* USER CODE BEGIN Conv_AnalogWDG_Config */
/**
  * @brief  Conversion queue adc4 error callback
  * @param  None
  * @retval None
  */
static void ADC4_Error_Callback(ADC_HandleTypeDef *hadc4)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
}

/**
  * @brief  Conversion queue adc4 watchdog callback
  * @param  None
  * @retval None
  */
static void ADC4_AnalogWDG1_Callback(ADC_HandleTypeDef *hadc4)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
}
/* USER CODE END Conv_AnalogWDG_Config */

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
