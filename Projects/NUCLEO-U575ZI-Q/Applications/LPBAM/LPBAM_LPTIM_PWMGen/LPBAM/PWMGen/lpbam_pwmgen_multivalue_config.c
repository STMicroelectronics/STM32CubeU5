/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_pwmgen_multivalue_config.c
  * @author  MCD Application Team
  * @brief   Provide LPBAM PWMGen application MultiValue configuration services.
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
#include "lpbam_pwmgen.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define --------------------------------------------------------------------------------------------------*/
#define PWMUPDATE_Q_IDX (0U)
#define DMA_TIMEOUT_DURATION (0x1000U)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* External variables ------------------------------------------------------------------------------------------------*/
/* IP handler declaration */
extern LPTIM_HandleTypeDef hlptim1;

/* LPBAM queues declaration */
extern DMA_QListTypeDef PWMUpdate_Q;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/

/* LPBAM LPTIM1 management APIs */
static void MX_LPTIM1_Init(void);
static void MX_LPTIM1_MspInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_MspDeInit(LPTIM_HandleTypeDef *hlptim1);
static void MX_LPTIM1_DeInit(void);

/* LPBAM autonomous mode management APIs */
static void MX_AutonomousMode_Init(void);
static void MX_AutonomousMode_DeInit(void);

/* LPBAM queue linking/unlinking APIs */
static void MX_PWMUpdate_Q_Link(DMA_HandleTypeDef *hdma);
static void MX_PWMUpdate_Q_UnLink(DMA_HandleTypeDef *hdma);

/* LPBAM DMA user callback APIs */
static void MX_PWMUpdate_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma);
/* LPBAM DMA NVIC API */
static void MX_DMA_NVIC_Config(DMA_HandleTypeDef *hdma, uint32_t PreemptPriority, uint32_t SubPriority);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ----------------------------------------------------------------------------------------------*/
/**
  * @brief  PWMGen application MultiValue scenario initialization
  * @param  None
  * @retval None
  */
void MX_PWMGen_MultiValue_Init(void)
{
  /* LPBAM LPTIM1 initialization */
  MX_LPTIM1_Init();

  /* Autonomous Mode initialization */
  MX_AutonomousMode_Init();

  /* USER CODE BEGIN PWMGen_MultiValue_Init */

  /* USER CODE END PWMGen_MultiValue_Init */
}

/**
  * @brief PWMGen application MultiValue scenario de-initialization
  * @param None
  * @retval None
  */
void MX_PWMGen_MultiValue_DeInit(void)
{
  /* LPBAM LPTIM1 De-initialization */
  MX_LPTIM1_DeInit();

  /* Autonomous mode de-initialization */
  MX_AutonomousMode_DeInit();

  /* USER CODE BEGIN PWMGen_MultiValue_DeInit */

  /* USER CODE END PWMGen_MultiValue_DeInit */
}

/**
  * @brief PWMGen application MultiValue scenario link
  * @param None
  * @retval None
  */
void MX_PWMGen_MultiValue_Link(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN PWMGen_MultiValue_Link 0 */

  /* USER CODE END PWMGen_MultiValue_Link 0 */

  /* Link PWMUpdate queue to DMA channel */
  MX_PWMUpdate_Q_Link(&hdma[PWMUPDATE_Q_IDX]);

  /* USER CODE BEGIN LINK PWMUPDATE_Q_IDX */

  /* USER CODE END LINK PWMUPDATE_Q_IDX */

  /* USER CODE BEGIN PWMGen_MultiValue_Link 1 */

  /* USER CODE END PWMGen_MultiValue_Link 1 */
}

/**
  * @brief PWMGen application MultiValue scenario unlink
  * @param hdma :Pointer to a DMA_HandleTypeDef structure that contains the configuration information for the specified
  *              DMA Channel
  * @retval None
  */
void MX_PWMGen_MultiValue_UnLink(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN PWMGen_MultiValue_UnLink 0 */

  /* USER CODE END PWMGen_MultiValue_UnLink 0 */

  /* LPBAM unLink PWMUpdate queue to DMA channel */
  MX_PWMUpdate_Q_UnLink(&hdma[PWMUPDATE_Q_IDX]);

  /* USER CODE BEGIN UNLINK PWMUPDATE_Q_IDX */

  /* USER CODE END UNLINK PWMUPDATE_Q_IDX */

  /* USER CODE BEGIN PWMGen_MultiValue_UnLink 1 */

  /* USER CODE END PWMGen_MultiValue_UnLink 1 */
}

/**
  * @brief PWMGen application MultiValue scenario start
  * @retval None
  */
void MX_PWMGen_MultiValue_Start(DMA_HandleTypeDef *hdma)
{
  /* LPBAM start DMA channel in linked-list mode */
  if (HAL_DMAEx_List_Start(&hdma[PWMUPDATE_Q_IDX]) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN PWMGen_MultiValue_Start */

  /* USER CODE END PWMGen_MultiValue_Start */
}

/**
  * @brief PWMGen application MultiValue scenario stop
  * @retval None
  */
void MX_PWMGen_MultiValue_Stop(DMA_HandleTypeDef *hdma)
{
  /* LPBAM stop DMA channel in linked-list mode */
  if ((hdma[PWMUPDATE_Q_IDX].State == HAL_DMA_STATE_BUSY) && (hdma[PWMUPDATE_Q_IDX].LinkedListQueue->FirstCircularNode != 0U))
  {
    if (HAL_DMA_Abort(&hdma[PWMUPDATE_Q_IDX]) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* Check if DMA channel interrupt is enabled */
  if ((hdma[PWMUPDATE_Q_IDX].State == HAL_DMA_STATE_BUSY) && (__HAL_DMA_GET_IT_SOURCE(&hdma[PWMUPDATE_Q_IDX], DMA_IT_TC) == 0U))
  {
    if (HAL_DMA_PollForTransfer(&hdma[PWMUPDATE_Q_IDX], HAL_DMA_FULL_TRANSFER, DMA_TIMEOUT_DURATION) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* USER CODE BEGIN PWMGen_MultiValue_Stop */

  /* Start the LPTIM PWM generation */
  if (HAL_LPTIM_PWM_Stop(&hlptim1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* USER CODE END PWMGen_MultiValue_Stop */
}

/**
  * @brief PWMGen application MultiValue autonomous mode init
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

  /* USER CODE BEGIN AutonomousMode_Init */

  /* USER CODE END AutonomousMode_Init */
}

/**
  * @brief  PWMGen application MultiValue autonomous mode deinit
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
  hlptim1.Init.Period = LPTIM1_PWM_DEFAULT_PERIOD;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = LPTIM1_PWM_DEFAULT_REPETITION;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig1.Pulse = LPTIM1_PWM_DEFAULT_PULSE;
  sConfig1.OCPolarity = LPTIM_OCPOLARITY_HIGH;
  if (HAL_LPTIM_OC_ConfigChannel(&hlptim1, &sConfig1, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configuration in LPBAM context
  */
  if (ADV_LPBAM_LPTIM_EnableDMARequests(LPTIM1, LPBAM_LPTIM_CHANNEL_1) != LPBAM_OK)
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

  GPIO_InitTypeDef GPIO_InitStruct = {0};
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

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**LPTIM1 GPIO Configuration
    PC1     ------> LPTIM1_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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

    /**LPTIM1 GPIO Configuration
    PC1     ------> LPTIM1_CH1
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

  /* USER CODE BEGIN LPTIM1_MspDeInit 1 */

  /* USER CODE END LPTIM1_MspDeInit 1 */
  }
}

/**
  * @brief  PWMUpdate queue link
  * @retval None
  */
static void MX_PWMUpdate_Q_Link(DMA_HandleTypeDef *hdma)
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
  if (HAL_DMAEx_List_LinkQ(hdma, &PWMUpdate_Q) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_DMA_ENABLE_IT(hdma, DMA_IT_DTE|DMA_IT_ULE
                              |DMA_IT_USE|DMA_IT_TO);
  if (HAL_DMA_ConfigChannelAttributes(hdma, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register DMA channel error callbacks */
  if (HAL_DMA_RegisterCallback(hdma, HAL_DMA_XFER_ERROR_CB_ID, MX_PWMUpdate_Q_DMA_Error_Callback) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_NVIC_Config(hdma, 0, 0);
}

/**
  * @brief  PWMUpdate queue unlink
  * @retval None
  */
static void MX_PWMUpdate_Q_UnLink(DMA_HandleTypeDef *hdma)
{
  /* UnLink PWMUpdate queue to DMA channel */
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
  * @brief  PWMUpdate queue dma error callback
  * @retval None
  */
static void MX_PWMUpdate_Q_DMA_Error_Callback(DMA_HandleTypeDef *hdma)
{
  /* USER CODE BEGIN PWMUpdate_DMA_Error_Callback */

  /* Turn on LED3 */
  BSP_LED_On(LED3);

  /* USER CODE END PWMUpdate_DMA_Error_Callback */
}

/* USER CODE BEGIN PWMGen_MultiValue_Config */

/* USER CODE END PWMGen_MultiValue_Config */

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
