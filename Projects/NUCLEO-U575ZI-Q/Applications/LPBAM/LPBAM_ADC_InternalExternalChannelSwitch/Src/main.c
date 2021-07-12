/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_ADC_InternalExternalChannelSwitch/Src/main.c
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

/** @addtogroup LPBAM_ADC_InternalExternalChannelSwitch
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef   ADCHandle;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef LPTIM1Handle;
LPTIM_HandleTypeDef LPTIM3Handle;
/* DMA handle declaration */
DMA_HandleTypeDef   DMAHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t            DMAListInfo;
LPBAM_ADC_DataAdvConf_t        ConvDataAdvConf;
LPBAM_ADC_FullAdvConf_t        ConvFullAdvConf;
LPBAM_ADC_ConvDataDesc_t       ConvDataDesc;
LPBAM_ADC_ConvFullDesc_t       ConvFullDesc;
LPBAM_ADC_ChannelAdvConf_t     ChannelAdvConf;

LPBAM_LPTIM_CMFullAdvConf_t    CMAdvConf;
LPBAM_LPTIM_StartFullAdvConf_t CMStartFullAdvConf;
LPBAM_LPTIM_StopFullAdvConf_t  CMStopFullAdvConf;
LPBAM_LPTIM_CMFullDesc_t       CMFullDesc;
LPBAM_LPTIM_StartFullDesc_t    CMStartDesc;
LPBAM_LPTIM_StopFullDesc_t     CMStopDesc;

/* DMA variables declaration */
DMA_QListTypeDef ConvQueue;
DMA_QListTypeDef CMQueue;

/* Destination buffers declaration */
uint16_t aADCConvDataSeq1[ADC_CONVERTED_DATA_BUFFER_SIZE] = {0U};
uint16_t aADCConvDataSeq2[ADC_CONVERTED_DATA_BUFFER_SIZE] = {0U};

/* Private function prototypes -----------------------------------------------*/
/* System static APIs*/
static void CACHE_Enable      (void);
static void SystemClock_Config(void);

/* LPBAM Scenario APIs*/
static void DMA_LinkedListConfig(void);
static void ADC_Config          (void);
static void LPTIM1_Config       (void);
static void LPTIM3_Config       (void);

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
  CACHE_Enable();

  /* Configure the System clock to have a frequency of 160 MHz */
  SystemClock_Config();

  /* Initialize LED1 and LED3 : GREEN and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Configure User push-button in Interrupt mode for wakeup */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* ADC configuration */
  ADC_Config();

  /* LPTIM 3 configuration */
  LPTIM3_Config();

  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Wait for ADC conversion start time **********************************************************************************/
  /* Set the LPTIM 3 configuration */
  CMStartFullAdvConf.WakeupIT  = LPBAM_LPTIM_IT_NONE;
  CMStartFullAdvConf.StartMode = LPBAM_LPTIM_START_SINGLE;
  if (ADV_LPBAM_LPTIM_Start_SetFullQ(LPTIM3, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &CMStartFullAdvConf, &CMStartDesc, &CMQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set compare match configuration (wait for 4 ms) */
  CMAdvConf.Period     = 32U; /* 1 ms is the lptim period  */
  CMAdvConf.Repetition = 3U;  /* repeat the period 3 times */
  if (ADV_LPBAM_LPTIM_CM_SetFullQ(LPTIM3, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &CMAdvConf, &CMFullDesc, &CMQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Stop the LPTIM 3 */
  CMStopFullAdvConf.Mode = LPBAM_LPTIM_CM_MODE;
  if (ADV_LPBAM_LPTIM_Stop_SetFullQ(LPTIM3, &DMAListInfo, &CMStopFullAdvConf, &CMStopDesc, &CMQueue) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Set ADC process **************************************************************************************************/
  /*
   *      Set ADC configuration 1
   *      The configuration is inherited from HAL with below setup:
   *            - Channel               : Internal (VREFINT).
   *            - ScanConvMode          : Scan Direction is Forward.
   *            - Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
   *            - Trigger low frequency : Disabled.
   *            - ContinuousConvMode    : Enabled.
   *            - DiscontinuousConvMode : Disabled.
   *            - Analog watchdog       : Disabled.
   *            - DMAContinuousRequests : Disabled.
   *      Note:  Only conversion start and data storage is handled by LPBAM.
   */
  /* Set ADC data buffer */
  ConvDataAdvConf.Size  = ADC_CONVERTED_DATA_BUFFER_SIZE;
  ConvDataAdvConf.pData = (uint32_t*)&aADCConvDataSeq1;

  /* Advanced lpbam ADC conversion */
  if (ADV_LPBAM_ADC_Conversion_SetDataQ(ADC4, &DMAListInfo, &ConvDataAdvConf, &ConvDataDesc, &ConvQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Advanced lpbam ADC conversion */
  if (HAL_DMAEx_List_InsertQ(&CMQueue, &ConvDataDesc.pNodes[0U], &ConvQueue) != HAL_OK)
  {
    Error_Handler();
  }


  /*
   *      Set ADC configuration 2
   *      This configuration is performed via LPBAM with below setup:
   *            - Channel               : External (Channel 4).
   *            - ScanConvMode          : Scan Direction is Forward.
   *            - Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
   *            - Trigger low frequency : Disabled.
   *            - ContinuousConvMode    : Enabled.
   *            - DiscontinuousConvMode : Disabled.
   *            - Analog watchdog       : Disabled.
   *            - DMAContinuousRequests : Enabled (Circular Mode).
   */
  /* Set ADC configuration */
  ConvFullAdvConf.ScanConvMode          = LPBAM_ADC_SCAN_DIRECTION_FORWARD;
  ConvFullAdvConf.ContinuousConvMode    = ENABLE;
  ConvFullAdvConf.DiscontinuousConvMode = DISABLE;
  ConvFullAdvConf.ExternalTrigConv      = LPBAM_ADC_EXTERNALTRIG_LPTIM1_CH1;
  ConvFullAdvConf.ExternalTrigConvEdge  = LPBAM_ADC_EXTERNALTRIGCONVEDGE_RISING;
  ConvFullAdvConf.TriggerFrequencyMode  = LPBAM_ADC_TRIGGER_FREQ_HIGH;
  ConvFullAdvConf.LowPowerAutoWait      = ENABLE;
  ConvFullAdvConf.DMAContinuousRequests = ENABLE;

  /* Set Analog watchdog configuration */
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_1].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_1;
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_1].WatchdogMode   = LPBAM_ADC_ANALOGWATCHDOG_NONE;
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_2].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_2;
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_2].WatchdogMode   = LPBAM_ADC_ANALOGWATCHDOG_NONE;
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_3].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_3;
  ConvFullAdvConf.AnalogWDGConfig[ANALOGWATCHDOG_3].WatchdogMode   = LPBAM_ADC_ANALOGWATCHDOG_NONE;

  /* Set ADC data buffer */
  ConvFullAdvConf.Size  = ADC_CONVERTED_DATA_BUFFER_SIZE;
  ConvFullAdvConf.pData = (uint32_t*)&aADCConvDataSeq2;

  /* Advanced lpbam ADC conversion */
  if (ADV_LPBAM_ADC_Conversion_SetFullQ(ADC4, &DMAListInfo, &ConvFullAdvConf, &ConvFullDesc, &ConvQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Advanced lpbam ADC channel 4 config */
  ChannelAdvConf.ChannelConfig.Channel = LPBAM_ADC_CHANNEL_4;
  ChannelAdvConf.ChannelConfig.Rank    = LPBAM_ADC_REGULAR_RANK_1;
  if (ADV_LPBAM_ADC_ChannelConfig(&ChannelAdvConf, &ConvFullDesc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set circular mode for linked-list queue */
  if (ADV_LPBAM_Q_SetCircularMode(&ConvFullDesc, LPBAM_ADC_CONVERSION_FULLQ_DATA_NODE, &ConvQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /*
   *
   *      Configure and start DMA channel
   *
   */

  /* DMA linked-list configuration */
  DMA_LinkedListConfig();

  /* Configure and start LPTIM1 */
  LPTIM1_Config();

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

  /* Turn on LED1 */
  BSP_LED_On(LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  ADC configuration.
  * @param  None
  * @retval None
  */
static void DMA_LinkedListConfig (void)
{
  /* Enable DMA clock */
  DMA_CLK_ENABLE();

  /* Set DMA instance */
  DMAHandle.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &ConvQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Configure ADC peripheral instance.
  * @param  None
  * @retval None
  */
 static void ADC_Config (void)
{
  /* ADC channel configuration structure declaration */
  ADC_ChannelConfTypeDef sConfig;

   /* ### - 1 - Initialize ADC peripheral #################################### */
  ADCHandle.Instance = ADC_Instance;

  ADCHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;            /* Asynchronous clock mode, input ADC clock                                                     */
  ADCHandle.Init.Resolution               = ADC_RESOLUTION_12B;              /* 12-bit resolution for converted data                                                         */
  ADCHandle.Init.ScanConvMode             = ADC_SCAN_DIRECTION_FORWARD;      /* Sequencer set to not fully configurable (following settings below) : Scan direction forward  */
  ADCHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;             /* EOC flag picked-up to indicate conversion end                                                */
  ADCHandle.Init.LowPowerAutoWait         = DISABLE;                         /* Auto-delayed conversion feature disabled                                                     */
  ADCHandle.Init.ContinuousConvMode       = DISABLE;                         /* Continuous mode disabled to have only 1 conversion at each conversion trig                   */
  ADCHandle.Init.DiscontinuousConvMode    = DISABLE;                         /* Discontinuous mode disabaled to convert the sequence in one shot                             */
  ADCHandle.Init.ExternalTrigConv         = ADC4_EXTERNALTRIG_LPTIM1_CH1;    /* Trig of conversion start done by external event                                              */
  ADCHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
  ADCHandle.Init.DMAContinuousRequests    = ENABLE;                          /* ADC-DMA continuous requests to match with DMA configured in circular mode                    */
  ADCHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;        /* DR register is overwritten with the last conversion result in case of overrun                */
  ADCHandle.Init.OversamplingMode         = DISABLE;                         /* No oversampling                                                                              */
  ADCHandle.Init.SamplingTimeCommon1      = ADC4_SAMPLETIME_160CYCLES_5;     /* Sampling time (number of clock cycles unit)                                                  */
  ADCHandle.Init.LowPowerAutoPowerOff     = ADC_LOW_POWER_AUTOFF_DPD;        /* Auto-off and DPD conversion feature enabled                                                  */

  /* De-Initialize ADC peripheral */
  if (HAL_ADC_DeInit(&ADCHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&ADCHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&ADCHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL;                /* Sampled channel number                            */
  sConfig.Rank         = ADC4_RANK_CHANNEL_NUMBER;   /* Rank of each channel is defined by channel number */
  sConfig.SamplingTime = ADC4_SAMPLINGTIME_COMMON_1; /* ADC channel sampling time                         */

  /* Configure ADC channel */
  if (HAL_ADC_ConfigChannel(&ADCHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable ADC DMA requests */
  if (ADV_LPBAM_ADC_EnableDMARequests(ADCHandle.Instance) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  LPTIM 3 configuration.
  * @param  None
  * @retval None
  */
static void LPTIM3_Config(void)
{
  LPTIM_OC_ConfigTypeDef sConfig;

  /*
     Set period value :
     LSI = 32KH
     (1/32) * (10^-3) * 32 = 1 mS.
  */
  uint32_t Period = 32000 / 1000;
  /*
     Set pulse value :
     LSI = 32KH
     (1/32) * (10^-3) * 16 = 0.5 mS.
  */
  uint32_t Pulse  = Period / 2U;

  /* Set lptim instance */
  LPTIM3Handle.Instance = LPTIM3;

  /* Set parameters to be configured */
  LPTIM3Handle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIM3Handle.Init.UpdateMode                    = LPTIM_UPDATE_IMMEDIATE;
  LPTIM3Handle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIM3Handle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIM3Handle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIM3Handle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIM3Handle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIM3Handle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIM3Handle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  LPTIM3Handle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  LPTIM3Handle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  LPTIM3Handle.Init.Period                        = Period;
  LPTIM3Handle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LPTIM3Handle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_LOW;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LPTIM3Handle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable DMA requests */
  if (ADV_LPBAM_LPTIM_EnableDMARequests(LPTIM3Handle.Instance, LPBAM_LPTIM_CHANNEL_1) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  LPTIM 1 configuration.
  * @param  None
  * @retval None
  */
static void LPTIM1_Config (void)
{
  LPTIM_OC_ConfigTypeDef sConfig;

  /*
     Set period value :
     LSI = 32KH
     (1/32) * (10^-3) * 64 = 2 mS.
  */
  uint32_t Period = 32000 / 1000;
  /*
     Set pulse value :
     LSI = 32KH
     (1/32) * (10^-3) * 32 = 1 mS.
  */
  uint32_t Pulse  = Period / 2U;

  /* Set lptim instance */
  LPTIM1Handle.Instance = LPTIM1;

  /* Set parameters to be configured */
  LPTIM1Handle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIM1Handle.Init.UpdateMode                    = LPTIM_UPDATE_IMMEDIATE;
  LPTIM1Handle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIM1Handle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIM1Handle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIM1Handle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIM1Handle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIM1Handle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIM1Handle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  LPTIM1Handle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  LPTIM1Handle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  LPTIM1Handle.Init.Period                        = Period;
  LPTIM1Handle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LPTIM1Handle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LPTIM1Handle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Generate one pulse : LPTIM1_CH1 is used to trig the ADC in order to start
     transactions                                                             */
  if (HAL_LPTIM_PWM_Start (&LPTIM1Handle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
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

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLFRACN= 0;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
static void CACHE_Enable(void)
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
void assert_failed(uint8_t *file, uint32_t line)
{
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
