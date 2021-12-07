/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_COMP_InputSwitch/Src/main.c
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

/** @addtogroup LPBAM_COMP_InputSwitch
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* COMP handle declaration  */
COMP_HandleTypeDef CompHandle;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef LptimHandle;
/* DMA handlers declaration */
DMA_HandleTypeDef DMAHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t              DMAListInfo;
LPBAM_COMMON_TrigAdvConf_t       TrigConfig;
LPBAM_COMP_StartFullAdvConf_t    StartAdvConf;
LPBAM_COMP_OutLevelFullAdvConf_t OutLevelAdvConf;
LPBAM_COMP_StartFullDesc_t       StartDesc[4U];
LPBAM_COMP_OutLevelFullDesc_t    OutLevelDesc[4U];

/* DMA variables declaration */
DMA_QListTypeDef Queue;

/* Buffer declaration */
uint32_t CompOutput_Buffer[OUTPUT_BUFFERSIZE] = {0U};

/* Expected read values */
uint32_t Expected_CompOutput_Buffer[OUTPUT_BUFFERSIZE] = {INPUTMINUS_ABOVE_INPUTPLUS, INPUTMINUS_BELOW_INPUTPLUS,
                                                          INPUTMINUS_ABOVE_INPUTPLUS, INPUTMINUS_BELOW_INPUTPLUS};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config        (void);
#if !defined (DEBUG_CONFIGURATION)
static void SystemPower_Config        (void);
#endif /* !defined (DEBUG_CONFIGURATION) */
static void Error_Handler             (void);
static void Cache_Enable              (void);
static void LPTIM_Config              (void);
static void COMP_Config               (void);
static void DMA_LinkedListConfig      (void);
static uint16_t Buffercmp             (uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
static void CompOutput_Data_Processing(uint32_t* pBuffer, uint16_t BufferLength);

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

  /* COMP Initialization sequence */
  COMP_Config();

  /* LPTIM Initialization sequence */
  LPTIM_Config ();

  /************************************************** Configuration 1 *************************************************/
  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set COMP configuration */
  StartAdvConf.InputPlus  = LPBAM_COMP_INPUT_PLUS_IO2;
  StartAdvConf.InputMinus = LPBAM_COMP_INPUT_MINUS_VREFINT;
  StartAdvConf.OutputPol  = LPBAM_COMP_OUTPUTPOL_NONINVERTED;

  /* Advanced lpbam COMP Configuration */
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &DMAListInfo, &StartAdvConf, &StartDesc[0U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &StartDesc[0U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /**************************************************** Read value 1 **************************************************/
  /* Set Node configuration */
  OutLevelAdvConf.Size  = 1U ;
  OutLevelAdvConf.pData = &CompOutput_Buffer[0U];

  /* Advanced lpbam COMP Read output value */
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &DMAListInfo, &OutLevelAdvConf, &OutLevelDesc[0U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutLevelDesc[0U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /************************************************** Configuration 2 *************************************************/
  /* Set COMP configuration */
  StartAdvConf.InputPlus = LPBAM_COMP_INPUT_PLUS_IO3;

  /* Advanced lpbam COMP Configuration */
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &DMAListInfo, &StartAdvConf, &StartDesc[1U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &StartDesc[1U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /**************************************************** Read value 2 **************************************************/
  /* Set Node configuration */
  OutLevelAdvConf.pData = &CompOutput_Buffer[1U];

  /* Advanced lpbam COMP Read output value */
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &DMAListInfo, &OutLevelAdvConf, &OutLevelDesc[1U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutLevelDesc[1U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /************************************************** Configuration 3 *************************************************/
  /* Set COMP configuration */
  StartAdvConf.InputPlus = LPBAM_COMP_INPUT_PLUS_IO2;

  /* Advanced lpbam COMP Configuration */
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &DMAListInfo, &StartAdvConf, &StartDesc[2U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &StartDesc[2U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /**************************************************** Read value 3 **************************************************/
  /* Set Node configuration */
  OutLevelAdvConf.pData = &CompOutput_Buffer[2U];

  /* Advanced lpbam COMP Read output value */
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &DMAListInfo, &OutLevelAdvConf, &OutLevelDesc[2U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutLevelDesc[2U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /************************************************** Configuration 4 *************************************************/
  /* Set COMP configuration */
  StartAdvConf.InputPlus = LPBAM_COMP_INPUT_PLUS_IO3;

  /* Advanced lpbam COMP Configuration */
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &DMAListInfo, &StartAdvConf, &StartDesc[3U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &StartDesc[3U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /**************************************************** Read value 4 **************************************************/
  /* Set Node configuration */
  OutLevelAdvConf.pData = &CompOutput_Buffer[3U];

  /* Advanced lpbam COMP Read output value */
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &DMAListInfo, &OutLevelAdvConf, &OutLevelDesc[3U], &Queue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Set DMA trigger */
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutLevelDesc[3U]) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* DMA configuration */
  DMA_LinkedListConfig();

  /* Generate PWM signal to trig the DMA */
  if (HAL_LPTIM_PWM_Start(&LptimHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler ();
  }

  /* Enter the system to STOP2 mode */
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

  /* Apply mask to read output value */
  CompOutput_Data_Processing(CompOutput_Buffer, OUTPUT_BUFFERSIZE);

  /* Compare read values to expected read values */
  if(Buffercmp(CompOutput_Buffer, Expected_CompOutput_Buffer, OUTPUT_BUFFERSIZE))
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

  /* Switch to SMPS regulator instead of LDO */
  HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY);

  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
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
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and confisgure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
                                      RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
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
static void LPTIM_Config (void)
{
  LPTIM_OC_ConfigTypeDef sConfig;
  uint32_t Period = LPTIM_PWM_PERIOD;
  uint32_t Pulse  = Period / 2U;

  /* Set lptim instance */
  LptimHandle.Instance = LPTIM1;

  /* Set parameters to be configured */
  LptimHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.UpdateMode                    = LPTIM_UPDATE_IMMEDIATE;
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
  LptimHandle.Init.RepetitionCounter             = 0U;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LptimHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LptimHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  COMP configuration
* @param  None
* @retval None
*/
void COMP_Config(void)
{
  /* Set COMP instance */
  CompHandle.Instance          = COMP1;
  /* Set COMP configuration */
  CompHandle.Init.InputMinus   = COMP_INPUT_MINUS_VREFINT;
  CompHandle.Init.InputPlus    = COMP_INPUT_PLUS_IO1;
  CompHandle.Init.OutputPol    = COMP_OUTPUTPOL_NONINVERTED;
  CompHandle.Init.Mode         = COMP_POWERMODE_ULTRALOWPOWER;
  CompHandle.Init.Hysteresis   = COMP_HYSTERESIS_NONE;
  CompHandle.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
  CompHandle.Init.WindowMode   = COMP_WINDOWMODE_DISABLE;

  /* Initialize COMP */
  if(HAL_COMP_Init(&CompHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  DMA configuration.
* @param  None
* @retval None
*/
static void DMA_LinkedListConfig (void)
{
  /* Set DMA instance */
  DMAHandle.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link Queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &Queue) != HAL_OK)
  {
    Error_Handler();
  }
  /* Enable end of transfer interrupt to wake up from stop2*/
  __HAL_DMA_ENABLE_IT(&DMAHandle, DMA_IT_TC);

  /* Enable DMA interrupt */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start (&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval 0  : pBuffer1 identical to pBuffer2
*         >0 : pBuffer1 differs from pBuffer2
*/
static uint16_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
* @brief  modify buffers.
* @param  pBuffer: buffers to be modified.
* @param  BufferLength: buffer's length
* @retval None
*/
static void CompOutput_Data_Processing(uint32_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    *pBuffer = __LPBAM_COMP_GET_OUTPUTLEVEL(*pBuffer);
    pBuffer++;
  }
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
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
