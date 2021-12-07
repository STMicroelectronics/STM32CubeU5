/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPTIM_PWMGeneration/Src/main.c
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

/** @addtogroup LPBAM_LPTIM_PWMGeneration
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handle declaration   */
DMA_HandleTypeDef   DMAHandle;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef LptimHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t            DMAListInfo;
LPBAM_LPTIM_StartFullAdvConf_t StartAdvConf;
LPBAM_LPTIM_StartFullDesc_t    StartDesc;
LPBAM_LPTIM_PWMFullAdvConf_t   PWMAdvConf;
LPBAM_LPTIM_PWMFullDesc_t      PWMDesc[PWM_FORM_NUM];

/* DMA variables declaration */
DMA_QListTypeDef PWMUpdate_Queue;

/* LPTIM PWM buffers */
uint16_t PeriodValue_Buffer[PWM_FORM_NUM]     = {327, 654, 982, 1310, 1637};
uint16_t PulseValue_Buffer[PWM_FORM_NUM]      = {163, 327, 491,  654,  818};
uint8_t  RepetitionValue_Buffer[PWM_FORM_NUM] = {  0,   1,   2,    3,    4};

/* Private function prototypes -----------------------------------------------*/
/* System static APIs*/
static void Cache_Enable         (void);
static void SystemClock_Config   (void);
#if !defined (DEBUG_CONFIGURATION)
static void SystemPower_Config(void);
#endif /* !defined (DEBUG_CONFIGURATION) */

/* LPTIM static APIs */
static void LPTIM_Config         (void);
static void DMA_LinkedListConfig (void);

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

  /* LPTIM configuration */
  LPTIM_Config();

  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Start the LPTIM in continuous mode****************************************/
  /* Set PWM parameter to be changed */
  StartAdvConf.StartMode = LPBAM_LPTIM_START_CONTINUOUS ;
  if (ADV_LPBAM_LPTIM_Start_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &StartAdvConf, &StartDesc, &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler (); /* KO */
  }

  /* Set PWM parameter to be changed */
  PWMAdvConf.UpdatePeriod     = ENABLE;
  PWMAdvConf.UpdatePulse      = ENABLE;
  PWMAdvConf.UpdateRepetition = ENABLE;

  /* Set PWM configuration 1 ***************************************************
  *  Period     : 10 ms.
  *  Pulse      : 5 ms.
  *  Repetition : 1 time.
  */
  /* Set PWM parameters values */
  PWMAdvConf.PeriodValue     = PeriodValue_Buffer[PWM_CONFIG_1];
  PWMAdvConf.PulseValue      = PulseValue_Buffer[PWM_CONFIG_1];
  PWMAdvConf.RepetitionValue = RepetitionValue_Buffer[PWM_CONFIG_1];

  /* Advanced lpbam update PWM */
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &PWMAdvConf, &PWMDesc[PWM_CONFIG_1], &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }


  /* Set PWM configuration 2 ***************************************************
  *  Period     : 20 ms.
  *  Pulse      : 10 ms.
  *  Repetition : 2 times.
  */
  /* Set PWM parameters values */
  PWMAdvConf.PeriodValue     = PeriodValue_Buffer[PWM_CONFIG_2];
  PWMAdvConf.PulseValue      = PulseValue_Buffer[PWM_CONFIG_2];
  PWMAdvConf.RepetitionValue = RepetitionValue_Buffer[PWM_CONFIG_2];

  /* Advanced lpbam update PWM */
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &PWMAdvConf, &PWMDesc[PWM_CONFIG_2], &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }


  /* Set PWM configuration 3 ***************************************************
  *  Period     : 30 ms.
  *  Pulse      : 15 ms.
  *  Repetition : 3 times.
  */
  /* Set PWM parameters values */
  PWMAdvConf.PeriodValue     = PeriodValue_Buffer[PWM_CONFIG_3];
  PWMAdvConf.PulseValue      = PulseValue_Buffer[PWM_CONFIG_3];
  PWMAdvConf.RepetitionValue = RepetitionValue_Buffer[PWM_CONFIG_3];

  /* Advanced lpbam update PWM */
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &PWMAdvConf, &PWMDesc[PWM_CONFIG_3], &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }


  /* Set PWM configuration 4 ***************************************************
  *  Period     : 40 ms.
  *  Pulse      : 20 ms.
  *  Repetition : 4 times.
  */
  /* Set PWM parameters values */
  PWMAdvConf.PeriodValue     = PeriodValue_Buffer[PWM_CONFIG_4];
  PWMAdvConf.PulseValue      = PulseValue_Buffer[PWM_CONFIG_4];
  PWMAdvConf.RepetitionValue = RepetitionValue_Buffer[PWM_CONFIG_4];

  /* Advanced lpbam update PWM */
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &PWMAdvConf, &PWMDesc[PWM_CONFIG_4], &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }


  /* Set PWM configuration 5 ***************************************************
  *  Period     : 50 ms.
  *  Pulse      : 25 ms.
  *  Repetition : 5 times.
  */
  /* Set PWM parameters values */
  PWMAdvConf.PeriodValue     = PeriodValue_Buffer[PWM_CONFIG_5];
  PWMAdvConf.PulseValue      = PulseValue_Buffer[PWM_CONFIG_5];
  PWMAdvConf.RepetitionValue = RepetitionValue_Buffer[PWM_CONFIG_5];

  /* Advanced lpbam update PWM */
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &DMAListInfo, &PWMAdvConf, &PWMDesc[PWM_CONFIG_5], &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set circular mode : repeat in infinite loop PWM signal updating */
  if (ADV_LPBAM_Q_SetCircularMode(&PWMDesc[PWM_CONFIG_1], LPBAM_LPTIM_PWM_FULLQ_CONFIG_NODE, &PWMUpdate_Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Link LPTIM PWM update queue and start DMA channel */
  DMA_LinkedListConfig();

  /* Start the LPTIM PWM generation */
  if (HAL_LPTIM_PWM_Start(&LptimHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Enter the whole system to Stop 2 mode */
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
  * @brief  LPTIM configuration.
  * @param  None
  * @retval None
  */
static void LPTIM_Config(void)
{
  LPTIM_OC_ConfigTypeDef sConfig;
  /* Set PWM period value */
  uint32_t Period = 32000U / 100U;
  /* Set PWM pulse value  */
  uint32_t Pulse  = Period / 2U;

  /* Set lptim instance */
  LptimHandle.Instance = LPTIM1;

  /* Set parameters to be configured */
  LptimHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  LptimHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LptimHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LptimHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LptimHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LptimHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LptimHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LptimHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  LptimHandle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  LptimHandle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  LptimHandle.Init.Period                        = Period;
  LptimHandle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LptimHandle) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LptimHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Enable DMA requests */
  if (ADV_LPBAM_LPTIM_EnableDMARequests(LptimHandle.Instance, LPBAM_LPTIM_CHANNEL_1) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DMA linked list configuration.
  * @param  None
  * @retval None
  */
static void DMA_LinkedListConfig(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set DMA instance */
  DMAHandle.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link UART queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &PWMUpdate_Queue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start DMA linked list */
  if (HAL_DMAEx_List_Start(&DMAHandle) != HAL_OK)
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
