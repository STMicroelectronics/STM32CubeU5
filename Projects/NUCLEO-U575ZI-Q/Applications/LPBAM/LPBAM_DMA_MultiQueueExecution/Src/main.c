/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_DMA_MultiQueueExecution/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/** @addtogroup STM32U5xx_Application
  * @{
  */

/** @addtogroup LPBAM_DMA_MultiQueueExecution
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handle declaration   */
DMA_HandleTypeDef   DMAMaster_Handle;
DMA_HandleTypeDef   DMASlave_Handle;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef Lptim1Handle;
LPTIM_HandleTypeDef Lptim3Handle;

/* LPBAM variables declaration */
LPBAM_DMA_StartFullAdvConf_t     DMA_StartAdvConf;
LPBAM_DMA_StartFullDesc_t        QueueExecution[DMA_TOGGLEIO_QUEUES_NUM];
LPBAM_GPIO_PinSeqFullAdvConf_t   GPIO_PinSeqAdvConf;
LPBAM_GPIO_WritePinSeqFullDesc_t WritePinSeqDesc[DMA_TOGGLEIO_QUEUES_NUM];

/* DMA variables declaration */
DMA_QListTypeDef           DMA_Master_SwitchIO_Queue;
DMA_QListTypeDef           DMA_Slave_ToggleIO_0_Queue;
DMA_QListTypeDef           DMA_Slave_ToggleIO_1_Queue;
LPBAM_COMMON_TrigAdvConf_t TrigConfig;

/* Source buffers declaration */
uint32_t LPGPIO_ToggleIO0[LPGPIO_TOGGLE_BUFFER_SIZE] =
{
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_0, LPBAM_GPIO_PIN_SET),
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_0, LPBAM_GPIO_PIN_RESET)
};
uint32_t LPGPIO_ToggleIO1[LPGPIO_TOGGLE_BUFFER_SIZE] =
{
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_1, LPBAM_GPIO_PIN_SET),
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_1, LPBAM_GPIO_PIN_RESET)
};

/* Private function prototypes -----------------------------------------------*/
/* System static APIs*/
static void Cache_Enable       (void);
static void SystemClock_Config (void);
#if !defined (DEBUG_CONFIGURATION)
static void SystemPower_Config (void);
#endif /* !defined (DEBUG_CONFIGURATION) */

/* LPTIM static APIs */
static void LPGPIO_Config              (void);
static void SlaveDMA_LinkedList_Config (void);
static void Create_ToggleIO_0_Queue    (void);
static void Create_ToggleIO_1_Queue    (void);
static void Create_SlaveMonitor_Queue  (void);
static void MasterDMA_LinkedList_Config(void);
static void LPTIM1_Config              (void);
static void LPTIM3_Config              (void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32U5xx HAL library initialization:
     - Configure the Flash prefetch
     - Configure the Systick to generate an interrupt each 1 msec
     - Set NVIC Group Priority to 3
     - Low Level Initialization
  */
  HAL_Init();

  /* Enable the Instruction Cache */
  Cache_Enable();

  /* Configure the System clock to have a frequency of 160 MHz */
  SystemClock_Config();

#if !defined (DEBUG_CONFIGURATION)
  /* Configure the system power */
  SystemPower_Config();
#endif /* defined (DEBUG_CONFIGURATION) */

  /* Initialize LED1 and LED3 : GREEN and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* User push-button will be used to wakeup the system from Stop2 mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure LPGPIO1 peripheral */
  LPGPIO_Config();

  /*
   *      Configure slave DMA in linked list mode
   */
  SlaveDMA_LinkedList_Config();

  /*
   *      Create 2 circular queues to toggle LPGPIO IO 0 and IO 1 in infinite loop
   *      (To be executed by slave DMA channel)
   */
  Create_ToggleIO_0_Queue();
  Create_ToggleIO_1_Queue();

  /*
   *      Create circular queue to monitor DMA slave channe execution
   *      (To be executed by master DMA channel)
   */
  Create_SlaveMonitor_Queue();

  /*
   *      Configure and start master DMA queue execution
   */
  MasterDMA_LinkedList_Config();

  /*
   *      Configure and start lptim PWM generation
   *      (LPTIM1 to trig Master DMA transfers)
   *      (LPTIM3 to trig Slave DMA transfers)
   */
  LPTIM1_Config();
  LPTIM3_Config();

  /* Enter stop 2 mode */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

  /* Check that the system was resumed from stop 2 */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_STOPF) == 0U)
  {
    Error_Handler();
  }

  /* Clear stop flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOPF);

  /* Check that stop flag is cleared */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_STOPF) != 0U)
  {
    Error_Handler();
  }

  /* Turn on LED1 */
  BSP_LED_On(LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure LPGPIO.
  * @param  None
  * @retval None
  */
static void LPGPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable LPGPIO clock */
  __HAL_RCC_LPGPIO1_CLK_ENABLE();
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure the LPGPIO PIN as output */
  GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LPGPIO1, &GPIO_InitStruct);
}

/**
  * @brief  Create toggle IO 0 queue.
  * @param  None
  * @retval None
  */
static void Create_ToggleIO_0_Queue(void)
{
  LPBAM_DMAListInfo_t DMAListInfo;

  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set parameter to be configured */
  GPIO_PinSeqAdvConf.Pin   = LPBAM_GPIO_PIN_0;
  GPIO_PinSeqAdvConf.Size  = LPGPIO_TOGGLE_BUFFER_SIZE;
  GPIO_PinSeqAdvConf.pData = LPGPIO_ToggleIO0;

  /* GPIO write pin sequence */
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &DMAListInfo, &GPIO_PinSeqAdvConf, &WritePinSeqDesc[TOGGLEIO_0_IDX], &DMA_Slave_ToggleIO_0_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM3_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSeqDesc[TOGGLEIO_0_IDX]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Enable circular mode */
  if (HAL_DMAEx_List_SetCircularMode(&DMA_Slave_ToggleIO_0_Queue) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Create toggle IO 1 queue.
  * @param  None
  * @retval None
  */
static void Create_ToggleIO_1_Queue(void)
{
  LPBAM_DMAListInfo_t DMAListInfo;

  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set parameter to be configured */
  GPIO_PinSeqAdvConf.Pin   = LPBAM_GPIO_PIN_1;
  GPIO_PinSeqAdvConf.Size  = LPGPIO_TOGGLE_BUFFER_SIZE;
  GPIO_PinSeqAdvConf.pData = LPGPIO_ToggleIO1;

  /* GPIO write pin sequence */
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &DMAListInfo, &GPIO_PinSeqAdvConf, &WritePinSeqDesc[TOGGLEIO_1_IDX], &DMA_Slave_ToggleIO_1_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM3_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSeqDesc[TOGGLEIO_1_IDX]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Enable circular mode */
  if (HAL_DMAEx_List_SetCircularMode(&DMA_Slave_ToggleIO_1_Queue) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Create slave monitor queue.
  * @param  None
  * @retval None
  */
static void Create_SlaveMonitor_Queue(void)
{
  LPBAM_DMAListInfo_t DMAListInfo;

  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Configure DMA slave to execute toggle IO 0 queue ********************/
  DMA_StartAdvConf.HeadQAddress = (uint32_t)DMA_Slave_ToggleIO_0_Queue.Head;
  DMA_StartAdvConf.WakeupIT     = LPBAM_DMA_IT_NONE;

  /* Setup Queue to be executed */
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel3, &DMAListInfo, &DMA_StartAdvConf, &QueueExecution[TOGGLEIO_0_IDX], &DMA_Master_SwitchIO_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_DMA_START_FULLQ_CONFIG_NODE, &QueueExecution[TOGGLEIO_0_IDX]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Configure DMA slave to execute toggle IO 1 queue ********************/
  DMA_StartAdvConf.HeadQAddress = (uint32_t)DMA_Slave_ToggleIO_1_Queue.Head;
  DMA_StartAdvConf.WakeupIT     = LPBAM_DMA_IT_NONE;

  /* Setup Queue to be executed */
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel3, &DMAListInfo, &DMA_StartAdvConf, &QueueExecution[TOGGLEIO_1_IDX], &DMA_Master_SwitchIO_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_DMA_START_FULLQ_CONFIG_NODE, &QueueExecution[TOGGLEIO_1_IDX]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set circular mode */
  if (HAL_DMAEx_List_SetCircularMode(&DMA_Master_SwitchIO_Queue) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Slave DMA linked list configuration.
  * @param  None
  * @retval None
  */
static void SlaveDMA_LinkedList_Config(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set DMA instance */
  DMASlave_Handle.Instance                         = LPDMA1_Channel3;

  /* Set parameters to be configured */
  DMASlave_Handle.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMASlave_Handle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMASlave_Handle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMASlave_Handle.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DMASlave_Handle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMASlave_Handle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Master DMA linked list configuration.
  * @param  None
  * @retval None
  */
static void MasterDMA_LinkedList_Config(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set DMA instance */
  DMAMaster_Handle.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAMaster_Handle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAMaster_Handle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAMaster_Handle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAMaster_Handle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAMaster_Handle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAMaster_Handle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set channel attributes */
  if (HAL_DMA_ConfigChannelAttributes(&DMAMaster_Handle, DMA_CHANNEL_PRIV) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAMaster_Handle, &DMA_Master_SwitchIO_Queue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start DMA linked list */
  if (HAL_DMAEx_List_Start(&DMAMaster_Handle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  LPTIM1 configuration.
  * @param  None
  * @retval None
  */
static void LPTIM1_Config(void)
{
  LPTIM_OC_ConfigTypeDef sConfig;

  /* Set lptim instance */
  Lptim1Handle.Instance = LPTIM1;

  /* Set parameters to be configured */
  Lptim1Handle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  Lptim1Handle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  Lptim1Handle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  Lptim1Handle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  Lptim1Handle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  Lptim1Handle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  Lptim1Handle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  Lptim1Handle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  Lptim1Handle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  Lptim1Handle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  Lptim1Handle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  Lptim1Handle.Init.Period                        = LPTIM1_PWM_PERIOD;
  Lptim1Handle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&Lptim1Handle) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = LPTIM1_PWM_PULSE;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel(&Lptim1Handle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Start the LPTIM PWM generation */
  if (HAL_LPTIM_PWM_Start(&Lptim1Handle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }
}

/**
  * @brief  LPTIM3 configuration.
  * @param  None
  * @retval None
  */
static void LPTIM3_Config(void)
{
  LPTIM_OC_ConfigTypeDef sConfig;

  /* Set lptim instance */
  Lptim3Handle.Instance = LPTIM3;

  /* Set parameters to be configured */
  Lptim3Handle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  Lptim3Handle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  Lptim3Handle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  Lptim3Handle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  Lptim3Handle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  Lptim3Handle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  Lptim3Handle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  Lptim3Handle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  Lptim3Handle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  Lptim3Handle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  Lptim3Handle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  Lptim3Handle.Init.Period                        = LPTIM3_PWM_PERIOD;
  Lptim3Handle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&Lptim3Handle) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = LPTIM3_PWM_PULSE;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel(&Lptim3Handle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Start the LPTIM PWM generation */
  if (HAL_LPTIM_PWM_Start(&Lptim3Handle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 160000000
  *            HCLK(Hz)                       = 160000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_MBOOST                     = 1
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 1 for frequency above 100 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST       = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM            = 1;
  RCC_OscInitStruct.PLL.PLLN            = 80;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  RCC_OscInitStruct.PLL.PLLP            = 2;
  RCC_OscInitStruct.PLL.PLLQ            = 2;
  RCC_OscInitStruct.PLL.PLLFRACN        = 0;
  RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLLVCIRANGE_0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
                                     RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler ();
  }
}

#if !defined (DEBUG_CONFIGURATION)
/**
  * @brief  System Power Configuration for LPBAM
  * @param  None
  * @retval None
  */
static void SystemPower_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable PWR CLK */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Switch to SMPS regulator */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Keep ICache and SRAM4 retention */
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);

  /* Enable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Set parameters to be configured */
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = GPIO_PIN_ALL;

  /* Initialize all GPIO pins */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* Disable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
}
#endif /* !defined (DEBUG_CONFIGURATION) */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while(1)
  {
  }
}

/**
  * @brief  Enable ICACHE with 1-way set-associative configuration.
  * @param  None
  * @retval None
  */
static void Cache_Enable(void)
{
  /* Configure ICACHE associativity mode */
  HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);

  /* Enable ICACHE */
  HAL_ICACHE_Enable();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
