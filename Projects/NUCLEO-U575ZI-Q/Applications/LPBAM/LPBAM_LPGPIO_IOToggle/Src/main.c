/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPGPIO_IOToggle/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use LPGPIOs through
  *          the STM32U5xx HAL API and to the LPDMA.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* DMA handle declaration */
DMA_HandleTypeDef   DMAHandle;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef LPTIMHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t              DMAListInfo;
LPBAM_GPIO_PinSeqFullAdvConf_t   WritePinSeqAdvConf;
LPBAM_GPIO_WritePinSeqFullDesc_t WritePinSeqDesc;
LPBAM_COMMON_TrigAdvConf_t       TrigConfig;

/* LPBAM variables declaration */
DMA_QListTypeDef Queue;

/* DMA variables declaration */
DMA_TriggerConfTypeDef TriggerConfig;

/* LPGPIO state buffer */
uint32_t LPGPIOBufferState[GPIO_STATE_BUFFER_SIZE] =
{
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_0, LPBAM_GPIO_PIN_SET),
  __LPBAM_GPIO_STATE(LPBAM_GPIO_PIN_0, LPBAM_GPIO_PIN_RESET)
};

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PM */
static void SystemClock_Config(void);
static void CACHE_Enable(void);
void HAL_TransferError(DMA_HandleTypeDef *DMAHandle);

static void MX_LPTIM_Config(void);
static void MX_LPGPIO_Config(void);
static void MX_DMA_LinkedListConfig(void);
static HAL_StatusTypeDef LSI_ClockEnable(void);
/* USER CODE END PM */

/* Private user code -------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
* @brief  The application entry point.
* @retval int
*/
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* STM32U5xx HAL library initialization:
  - Configure the Flash prefetch
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 3
  - Low Level Initialization
  */
  /* USER CODE END 1 */

  /* USER CODE BEGIN SysInit */
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Enable the Instruction Cache */
  CACHE_Enable();

  /* Configure the system clock */
  SystemClock_Config();

  /* Enable PWR CLK */
  __HAL_RCC_PWR_CLK_ENABLE();
  /* USER CODE END SysInit */

  /* -1- Initialize LEDs mounted on NUCLEO-U575ZI-Q (MB1549) board */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* -2- Initialize all configured peripherals */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* -3- Initialize all configured peripherals */
  MX_LPTIM_Config();
  MX_LPGPIO_Config();

  /* Set pin node *****************************************************************************************************/
  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set parameter to be configured */
  WritePinSeqAdvConf.Pin   = LPBAM_GPIO_PIN_0;
  WritePinSeqAdvConf.Size  = GPIO_STATE_BUFFER_SIZE;
  WritePinSeqAdvConf.pData = LPGPIOBufferState;

  /* GPIO write pin */
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &DMAListInfo, &WritePinSeqAdvConf, &WritePinSeqDesc, &Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSeqDesc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Enable circular mode */
  if (ADV_LPBAM_Q_SetCircularMode(&WritePinSeqDesc, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* DMA linked-list configuration */
  MX_DMA_LinkedListConfig();

  /* Enter the system to STOP 2 mode */
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
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  *            APB3 Prescaler                 = 1
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
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
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
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
  * @brief  Enable External Low Speed Clock (LSI)
  * @param  None
  * @retval Status
  */
static HAL_StatusTypeDef LSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable LSI clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
  RCC_OscInitStruct.LSIDiv         = RCC_LSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  return (HAL_RCC_OscConfig(&RCC_OscInitStruct));
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPGPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable LPGPIO clock */
  __HAL_RCC_LPGPIO1_CLK_ENABLE();
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure the LPGPIO PIN as output*/
  GPIO_InitStruct.Pin       = GPIO_PIN_0;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LPGPIO1, &GPIO_InitStruct);
}

/**
  * @brief DMA Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA_LinkedListConfig(void)
{
  /* Enable DMA clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Set DMA configuration */
  DMAHandle.Instance                         = LPDMA1_Channel0;
  DMAHandle.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA channel */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &Queue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Register callbacks */
  HAL_DMA_RegisterCallback(&DMAHandle, HAL_DMA_XFER_ERROR_CB_ID, HAL_TransferError);

  /* Configure the NVIC for DMA instance */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0x0, 0x0);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);

  /* Start linked list */
  if (HAL_DMAEx_List_Start_IT(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Init LPTIM peripheral.
  * @retval None.
  */
void MX_LPTIM_Config(void)
{
  /* Clocks structure declaration */
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  /* Channel_config structure declaration */
  LPTIM_OC_ConfigTypeDef sConfig = {0};

  /* Enable the LSE Clock */
  if (LSI_ClockEnable() != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 1 - Re-target the LSI to Clock the LPTIM Counter ################# */
  /* Select the LSE clock as LPTIM peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);


  /* ### - 2 - Initialize the LPTIM peripheral ############################## */
  /*
   *  Instance        = LPTIM1
   *  Clock Source    = APB or LowPowerOSCillator (in this example LSI is
   *                    already selected from the RCC stage)
   *  Counter source  = External event.
   *  Clock prescaler = 1 (No division)
   *  Counter Trigger = Software trigger
   *  Output Polarity = High
   *  Update mode     = Immediate (Registers are immediately updated after any
   *                    write access)
   */

  LPTIMHandle.Instance = LPTIM1;

  LPTIMHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIMHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIMHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIMHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIMHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIMHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIMHandle.Init.Period                        = LPTIM_PWM_PERIOD;
  LPTIMHandle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LPTIMHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 3 - Start counting in interrupt mode ############################# */
  /*
   *  Period = 99
   *  Pulse  = 49
   *  According to this configuration, the duty cycle will be equal to 50%
   */
  sConfig.Pulse      = LPTIM_PWM_PULSE;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_LOW;
  if (HAL_LPTIM_OC_ConfigChannel(&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start PWM generation */
  if (HAL_LPTIM_PWM_Start(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  /* USER CODE BEGIN Error_Handler_Debug */
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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

/**
  * @brief  BSP push button callback
  * @par    None
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
}

/**
* @brief DMA error callback
* @par None
* @retval None
*/
void HAL_TransferError(DMA_HandleTypeDef *DMAHandle)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while(1)
  {
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
