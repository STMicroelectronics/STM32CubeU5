/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/main.c
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

/** @addtogroup LPBAM_DAC_OPAMP_ContinuousConversion
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DAC handler declaration         */
DAC_HandleTypeDef   DACHandle;
/* OPAMP handler declaration       */
OPAMP_HandleTypeDef OPAMPHandle;
/* LPTIM DAC handle declaration    */
LPTIM_HandleTypeDef LPTIMHandle;
/* DAC DMA handler declaration     */
DMA_HandleTypeDef   DAC_DMAHandle;
/* OPAMP DMA handler declaration   */
DMA_HandleTypeDef   OPAMP_DMAHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t             DMAListInfo;
LPBAM_OPAMP_StartFullAdvConf_t  OPAMP_StartAdvConf;
LPBAM_OPAMP_StartFullDesc_t     OPAMP_StartDesc[4U];
LPBAM_DAC_DataAdvConf_t         DAC_DataAdvConf;
LPBAM_DAC_ConvDataDesc_t        DAC_DataDesc;
LPBAM_COMMON_TrigAdvConf_t      TrigConfig;
LPBAM_COMMON_DataAdvConf_t      DataConfig;

/* DMA queues declaration */
DMA_QListTypeDef DAC_StartQueue;
DMA_QListTypeDef OPAMP_ConfigQueue;


/* Private function prototypes -----------------------------------------------*/
/* System static APIs */
static void SystemClock_Config        (void);
static void Cache_Enable              (void);

/* LPTIM static API */
static void LPTIM_Config              (void);

/* DAC static APIs */
static void DAC_Config                (void);
static void DMA_DAC_LinkedListConfig  (void);

/* OPAMP static APIs */
static void OPAMP_Config              (void);
static void DMA_OPAMP_LinkedListConfig(void);

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

  /* Initialize LED1 and LED3 : GREEN and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);


  /*
   *   ######## LPBAM DAC configuration ########
   */

  /* DAC Initialization and configuration */
  DAC_Config();

  /* Set DMA information */
  DMAListInfo.pInstance       = LPDMA1;
  DMAListInfo.QueueType       = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set data information */
  DAC_DataAdvConf.Alignment   = LPBAM_DAC_ALIGN_12B_R;
  DAC_DataAdvConf.Size        = DAC_TRIANGULAR_SIGNAL_DATASIZE;
  DAC_DataAdvConf.pData       = (uint32_t *)TriangularSignal_Buffer;

  /* Advanced lpbam DAC start conversion */
  if (ADV_LPBAM_DAC_Conversion_SetDataQ(DAC1, LPBAM_DAC_CHANNEL_1, &DMAListInfo, &DAC_DataAdvConf, &DAC_DataDesc, &DAC_StartQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Update DAC DMA queue transfer event generation */
  DataConfig.TransferConfig.Transfer.TransferEventMode = LPBAM_DMA_TCEM_BLOCK_TRANSFER;
  DataConfig.UpdateSrcInc                              = DISABLE;
  DataConfig.UpdateDestInc                             = DISABLE;
  DataConfig.UpdateSrcDataWidth                        = DISABLE;
  DataConfig.UpdateDestDataWidth                       = DISABLE;
  DataConfig.UpdateTransferEventMode                   = ENABLE;
  if (ADV_LPBAM_Q_SetDataConfig(&DataConfig, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &DAC_DataDesc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set circular mode for linked-list queue */
  if (ADV_LPBAM_Q_SetCircularMode(&DAC_DataDesc, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &DAC_StartQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Link DAC queue and start DMA channel */
  DMA_DAC_LinkedListConfig();


  /*
   *   ######## LPBAM OPAMP configuration ########
   */

  /* OPAMP Initialization and configuration */
  OPAMP_Config();

  /* Build Opamp_Cfg1_Queue *******************************************************************************************/
  /* Set OPAMP1 configuration */
  OPAMP_StartAdvConf.Mode              = LPBAM_OPAMP_PGA_MODE;
  OPAMP_StartAdvConf.PgaGain           = LPBAM_OPAMP_PGA_GAIN_2;
  OPAMP_StartAdvConf.InvertingInput    = LPBAM_OPAMP_INVERTINGINPUT_CONNECT_NO;
  OPAMP_StartAdvConf.NonInvertingInput = LPBAM_OPAMP_NONINVERTINGINPUT_DAC_CH;

  /* Advanced lpbam OPAMP Configuration */
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &DMAListInfo, &OPAMP_StartAdvConf, &OPAMP_StartDesc[0U], &OPAMP_ConfigQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = LPBAM_DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_StartDesc[0U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Build Opamp_Cfg2_Queue *******************************************************************************************/
  /* Set OPAMP configuration */
  OPAMP_StartAdvConf.PgaGain = LPBAM_OPAMP_PGA_GAIN_4;

  /* Advanced lpbam OPAMP Configuration */
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &DMAListInfo, &OPAMP_StartAdvConf, &OPAMP_StartDesc[1U], &OPAMP_ConfigQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = LPBAM_DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_StartDesc[1U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Build Opamp_Cfg3_Queue *******************************************************************************************/
  /* Set OPAMP configuration */
  OPAMP_StartAdvConf.PgaGain = LPBAM_OPAMP_PGA_GAIN_8;

  /* Advanced lpbam OPAMP Configuration */
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &DMAListInfo, &OPAMP_StartAdvConf, &OPAMP_StartDesc[2U], &OPAMP_ConfigQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = LPBAM_DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_StartDesc[2U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Build Opamp_Cfg4_Queue *******************************************************************************************/
  /* Set OPAMP configuration */
  OPAMP_StartAdvConf.PgaGain = LPBAM_OPAMP_PGA_GAIN_16;

  /* Advanced lpbam OPAMP Configuration */
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &DMAListInfo, &OPAMP_StartAdvConf, &OPAMP_StartDesc[3U], &OPAMP_ConfigQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = LPBAM_DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_StartDesc[3U]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set circular mode for linked-list queue */
  if (ADV_LPBAM_Q_SetCircularMode(&OPAMP_StartDesc[0U], LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_ConfigQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Link OPAMP queue and start DMA channel */
  DMA_OPAMP_LinkedListConfig();

  /* LPTIM Initialization sequence */
  LPTIM_Config();

  /* Enter the system to STOP2 mode */
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

  /* Infinite loop */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  /* Set LSE configuration */
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  /* Set PLL configuration */
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
    Error_Handler();
  }

  /* Select PLL as system clock source and confisgure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
                                      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  OPAMP configuration.
  * @param  None
  * @retval None
  */
void OPAMP_Config(void)
{
  /* Set OPAMP instance */
  OPAMPHandle.Instance = OPAMP1;

  /* DeInit OPAMP peripheral */
  if (HAL_OPAMP_DeInit(&OPAMPHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set parameter to be configured */
  OPAMPHandle.Init.PowerMode    = OPAMP_POWERMODE_LOWPOWER_HIGHSPEED;
  OPAMPHandle.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;

  /* Init OPAMP peripheral */
  if(HAL_OPAMP_Init(&OPAMPHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DMA OPAMP linked list configuration.
  * @param  None
  * @retval None
  */
static void DMA_OPAMP_LinkedListConfig(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set DMA instance */
  OPAMP_DMAHandle.Instance                         = LPDMA1_Channel1;
  /* Set linked list configuration */
  OPAMP_DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  OPAMP_DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  OPAMP_DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  OPAMP_DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  OPAMP_DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* DMA linked list init */
  if(HAL_DMAEx_List_Init(&OPAMP_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link OPAMP queue to DMA channel */
  if(HAL_DMAEx_List_LinkQ(&OPAMP_DMAHandle, &OPAMP_ConfigQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Optimize node queue and start node execution */
  if (HAL_DMAEx_List_Start(&OPAMP_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  LPTIM configuration.
  * @param  None
  * @retval None
  */
static void LPTIM_Config(void)
{
  LPTIM_OC_ConfigTypeDef  sConfig;

  /* Set LPTIM instance */
  LPTIMHandle.Instance                           = LPTIM3;
  /* Set parameter to be configured */
  LPTIMHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIMHandle.Init.UpdateMode                    = LPTIM_UPDATE_IMMEDIATE;
  LPTIMHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIMHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIMHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIMHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIMHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIMHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  LPTIMHandle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  LPTIMHandle.Init.Period                        = 2U;

  /* Init LPTIM peripheral */
  if (HAL_LPTIM_Init(&LPTIMHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set parameter to be configured */
  sConfig.Pulse      = LPTIMHandle.Init.Period / 2U;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure LPTIM peripheral */
  if (HAL_LPTIM_OC_ConfigChannel(&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* LPTIM start PWM */
  if (HAL_LPTIM_PWM_Start(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DAC configuration.
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  DAC_ChannelConfTypeDef        dacChannelConfig;
  DAC_AutonomousModeConfTypeDef AutonomousModeConfig;

  /* Set DAC instance */
  DACHandle.Instance = DAC1;

  /* Init DAC peripheral */
  if(HAL_DAC_Init(&DACHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set parameter to be configured */
  dacChannelConfig.DAC_AutonomousMode                      = DAC_AUTONOMOUS_MODE_ENABLE;
  dacChannelConfig.DAC_HighFrequency                       = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  dacChannelConfig.DAC_DMADoubleDataMode                   = DISABLE;
  dacChannelConfig.DAC_SignedFormat                        = DISABLE;
  dacChannelConfig.DAC_Trigger                             = DAC_TRIGGER_LPTIM3_OUT;
  dacChannelConfig.DAC_OutputBuffer                        = DAC_OUTPUTBUFFER_DISABLE;
  dacChannelConfig.DAC_ConnectOnChipPeripheral             = DAC_CHIPCONNECT_BOTH;
  dacChannelConfig.DAC_UserTrimming                        = DAC_TRIMMING_USER;
  dacChannelConfig.DAC_TrimmingValue                       = 16U;
  /* Setup DAC sample and hold */
  dacChannelConfig.DAC_SampleAndHold                       = DAC_SAMPLEANDHOLD_ENABLE;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_SampleTime  = 10U;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_HoldTime    = 20U;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_RefreshTime = 5U;

  /* Configures the DAC channel1 */
  if (HAL_DAC_ConfigChannel(&DACHandle, &dacChannelConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set autonomous mode configuration */
  AutonomousModeConfig.AutonomousModeState = DAC_AUTONOMOUS_MODE_ENABLE;
  if (HAL_DACEx_SetConfigAutonomousMode(&DACHandle, &AutonomousModeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable DMA request */
  if (ADV_LPBAM_DAC_EnableDMARequests(DACHandle.Instance, LPBAM_DAC_CHANNEL_1) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DMA DAC linked list configuration.
  * @param  None
  * @retval None
  */
static void DMA_DAC_LinkedListConfig(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set DMA instance */
  DAC_DMAHandle.Instance                         = LPDMA1_Channel0;
  /* Set linked list configuration */
  DAC_DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DAC_DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DAC_DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DAC_DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DAC_DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* DMA linked list init */
  if(HAL_DMAEx_List_Init(&DAC_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link DAC queue to DMA channel */
  if(HAL_DMAEx_List_LinkQ(&DAC_DMAHandle, &DAC_StartQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable DMA error interrupts */
  __HAL_DMA_ENABLE_IT(&DAC_DMAHandle, DMA_IT_DTE | DMA_IT_ULE | DMA_IT_USE | DMA_IT_TO);

  /* Enable NVIC for DMA channel */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0U, 0U);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);

  /* Start DMA linked list */
  if (HAL_DMAEx_List_Start(&DAC_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

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
