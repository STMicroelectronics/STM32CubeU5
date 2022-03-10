/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_AnalogWatchdog_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a ADC peripheral
  *          with ADC analog watchdog to monitor a channel and detect
  *          when the corresponding conversion data is out of window thresholds.
  *          This example is based on the STM32U5xx ADC LL API;
  *          Peripheral initialization done using LL unitary services functions.
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

/* Definitions of ADC hardware constraints delays */
/* Note: Only ADC IP HW delays are defined in ADC LL driver driver,           */
/*       not timeout values:                                                  */
/*       Timeout values for ADC operations are dependent to device clock      */
/*       configuration (system clock versus ADC clock),                       */
/*       and therefore must be defined in user application.                   */
/*       Refer to @ref ADC_LL_EC_HW_DELAYS for description of ADC timeout     */
/*       values definition.                                                   */

  /* Timeout values for ADC operations. */
  /* (calibration, enable settling time, disable settling time, ...)          */
  /* Values defined to be higher than worst cases: low clock frequency,       */
  /* maximum prescalers.                                                      */
  /* Note: ADC channel configuration ready (ADC_CHANNEL_CONF_RDY_TIMEOUT_MS)  */
  /*       is added in CubeMx code section.                                   */
  /* Unit: ms                                                                 */
  #define ADC_CALIBRATION_TIMEOUT_MS       (   1UL)
  #define ADC_ENABLE_TIMEOUT_MS            (   1UL)
  #define ADC_DISABLE_TIMEOUT_MS           (   1UL)
  #define ADC_STOP_CONVERSION_TIMEOUT_MS   (   1UL)
  #define ADC_CONVERSION_TIMEOUT_MS        (4000UL)

  /* Delay between ADC end of calibration and ADC enable.                     */
  /* Delay estimation in CPU cycles: Case of ADC enable done                  */
  /* immediately after ADC calibration, ADC clock setting slow                */
  /* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
  /* (CPU clock / ADC clock) is above 32.                                     */
  #define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3300UL)

/* Definitions of data related to this example */
  /* Definition of ADCx analog watchdog window thresholds */
  #define ADC_AWD_THRESHOLD_HIGH           (__LL_ADC_DIGITAL_SCALE(ADC1, LL_ADC_RESOLUTION_12B) / 2) /* Value of ADC analog watchdog threshold high */
  #define ADC_AWD_THRESHOLD_LOW            (   0UL) /* Value of ADC analog watchdog threshold low */

  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(ADC1, LL_ADC_RESOLUTION_12B) + 1)

  #define ADC_AWD_REARM_DELAY_MS           (5000UL) /* ADC analog watchdog rearm delay (unit: ms) */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

/* Variable to report status of ADC analog watchdog 1:                        */
/*  0: ADC conversion data into AWD window                                    */
/*  1: ADC conversion data out of AWD window                                  */
__IO uint8_t ubAnalogWatchdog1Status = 0U; /* Variable set into analog watchdog 1 interruption callback */

uint32_t led_toggle_period_ms = LED_BLINK_SLOW; /* LED toggle period (unit: ms) */
uint32_t adc_awd_rearm_delay_iterations_count; /* ADC analog watchdog rearm delay iterations count */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_ICACHE_Init(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

void ADC_Activate(void);
void ADC_AnalogWatchdog_Rearm(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
   NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Enable PWR clock interface */

  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */
  /* Enable the independent analog and I/Os supply */
   LL_PWR_EnableVDDA();
   LL_PWR_EnableVDDIO2();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_GPIO_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* Activate ADC */
  /* Perform ADC activation procedure to make it ready to convert. */
  ADC_Activate();

  /* Start ADC group regular conversion */
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of this feature is conditioned to    */
  /*       ADC state:                                                         */
  /*       ADC must be enabled without conversion on going on group regular,  */
  /*       without ADC disable command on going.                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if ((LL_ADC_IsEnabled(ADC1) == 1)               &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0)        &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_REG_StartConversion(ADC1);
  }
  else
  {
    /* Error: ADC conversion start could not be performed */
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Process analog watchdog out of window event */
    if(ubAnalogWatchdog1Status == 1)
    {
      /* Wait for ADC analog watchdog rearm */
      adc_awd_rearm_delay_iterations_count--;
      if(adc_awd_rearm_delay_iterations_count == 0)
      {
        led_toggle_period_ms = LED_BLINK_SLOW;

        /* Reset status variable of ADC analog watchdog 1 */
        ubAnalogWatchdog1Status = 0;

        /* Rearm ADC analog watchdog to make it ready for another trig */
        ADC_AnalogWatchdog_Rearm();
      }
    }

    LED_Toggle();
    LL_mDelay(led_toggle_period_ms);

    /* Note: LED state depending on ADC analog watchdog 1 status              */
    /*       variable "ubAnalogWatchdog1Status"                               */
    /*       is set into ADC analog watchdog 1 IRQ handler,                   */
    /*       refer to function "AdcAnalogWatchdog1_Callback()".               */

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_MSIS_Enable();

   /* Wait till MSIS is ready */
  while(LL_RCC_MSIS_IsReady() != 1)
  {
  }

  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSIS_SetRange(LL_RCC_MSISRANGE_4);
  LL_RCC_MSI_SetCalibTrimming(16, LL_RCC_MSI_OSCILLATOR_1);
  LL_RCC_PLL1_ConfigDomain_SYS(LL_RCC_PLL1SOURCE_MSIS, 1, 80, 2);
  LL_RCC_PLL1_EnableDomain_SYS();
  LL_RCC_SetPll1EPodPrescaler(LL_RCC_PLL1MBOOST_DIV_1);
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Insure 1µs transition state at intermediate medium speed clock based on DWT*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

  LL_Init1msTick(160000000);

  LL_SetSystemCoreClock(160000000);
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  LL_PWR_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  LL_PWR_SetRegulatorSupply(LL_PWR_SMPS_SUPPLY);

  while(LL_PWR_IsActiveFlag_REGULATOR()!=1)
  {
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetADCDACClockSource(LL_RCC_ADCDAC_CLKSOURCE_HSI);

  /* Peripheral clock enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA4   ------> ADC1_IN9
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC1 interrupt Init */
  NVIC_SetPriority(ADC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(ADC1_IRQn);

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  LL_ADC_SetTriggerFrequencyMode(ADC1, LL_ADC_TRIGGER_FREQ_HIGH);
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV4;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  ADC_InitStruct.LeftBitShift = LL_ADC_LEFT_BIT_SHIFT_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  LL_ADC_SetGainCompensation(ADC1, 0);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DR_TRANSFER;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);

  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(ADC1);
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(ADC1);
  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us. */
  /* Note: Variable divided by 2 to compensate partially */
  /* CPU processing cycles (depends on compilation optimization). */
  /* Note: If system core clock frequency is below 200kHz, wait time */
  /* is only a few CPU processing cycles. */
  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }

  /** Configure Analog WatchDog 1
  */
  LL_ADC_SetAnalogWDMonitChannels(ADC1, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG);
  LL_ADC_ConfigAnalogWDThresholds(ADC1, LL_ADC_AWD1, ADC_AWD_THRESHOLD_HIGH, ADC_AWD_THRESHOLD_LOW);
  LL_ADC_SetAWDFilteringConfiguration(ADC1, LL_ADC_AWD1, LL_ADC_AWD_FILTERING_NONE);
  LL_ADC_EnableIT_AWD1(ADC1);

  /** Configure Regular Channel
  */
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SINGLE_ENDED);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_391CYCLES_5);
  LL_ADC_SetChannelPreselection(ADC1, LL_ADC_CHANNEL_9);
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /**/
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @param  None
  * @retval None
  */
void ADC_Activate(void)
{
  __IO uint32_t wait_loop_index = 0U;
  #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0U; /* Variable used for timeout management */
  #endif /* USE_TIMEOUT */

  /*## Operation on ADC hierarchical scope: ADC instance #####################*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features is conditioned to  */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1, LL_ADC_CALIB_OFFSET);

    /* Poll for ADC effectively calibrated */
    #if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
    {
    #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          /* Error: Time-out */
          Error_Handler();
        }
      }
    #endif /* USE_TIMEOUT */
    }

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Enable ADC */
    LL_ADC_Enable(ADC1);

    /* Poll for ADC ready to convert */
    #if (USE_TIMEOUT == 1)
    Timeout = ADC_ENABLE_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */

    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
    {
    #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          /* Error: Time-out */
          Error_Handler();
        }
      }
    #endif /* USE_TIMEOUT */
    }

    /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
    /*       status afterwards.                                               */
    /*       This flag should be cleared at ADC Deactivation, before a new    */
    /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
  }

  /*## Operation on ADC hierarchical scope: ADC group regular ################*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */

  /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
  /* Note: No operation on ADC group injected performed here.                 */
  /*       ADC group injected conversions to be performed after this function */
  /*       using function:                                                    */
  /*       "LL_ADC_INJ_StartConversion();"                                    */

}

/**
  * @brief  Rearm ADC analog watchdog to make it ready another trig
  *         (ADC instance: ADC1, analog watchdog instance: AWD1).
  * @param  None
  * @retval None
  */
void ADC_AnalogWatchdog_Rearm(void)
{
  /* Clear flag ADC analog watchdog 1 */
  LL_ADC_ClearFlag_AWD1(ADC1);

  /* Enable ADC analog watchdog 1 interruption */
  LL_ADC_EnableIT_AWD1(ADC1);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED on */
  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

/**
  * @brief  Toggle LED1.
  * @param  None
  * @retval None
  */
void LED_Toggle(void)
{
  /* Turn LED off */
  LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  ADC analog watchdog 1 interruption callback
  * @note   This function is executed when the ADC conversion data is
  *         out of analog watchdog 1 window thresholds.
  * @retval None
  */
void AdcAnalogWatchdog1_Callback()
{
  /* Disable ADC analog watchdog 1 interruption */
  /* Note: ADC analog watchdog 1 interruption rearmed afterwards after
           event processing completion */
  LL_ADC_DisableIT_AWD1(ADC1);

  /* Retrieve ADC conversion data */
  /* Note: data scale corresponds to ADC resolution */
  uhADCxConvertedData = LL_ADC_REG_ReadConversionData32(ADC1);

  /* Update status variable of ADC analog watchdog 1 */
  ubAnalogWatchdog1Status = 1;

  /* Change LED toggle frequency depending on voltage below or above
     analog watchdog window */
  if (uhADCxConvertedData > ADC_AWD_THRESHOLD_HIGH)
  {
    led_toggle_period_ms = LED_BLINK_VERY_FAST;
  }
  else if (ADC_AWD_THRESHOLD_LOW < uhADCxConvertedData)
  {
    led_toggle_period_ms = LED_BLINK_SLOW;
  }
  else /* (uhADCxConvertedData < ADC_AWD_THRESHOLD_LOW) */
  {
    led_toggle_period_ms = LED_BLINK_FAST;
  }

  /* Set ADC analog watchdog rearm delay iterations count */
  adc_awd_rearm_delay_iterations_count = (ADC_AWD_REARM_DELAY_MS / led_toggle_period_ms);
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

  /* Turn on LED and remain in infinite loop */
  while (1)
  {
    LED_On();
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
