/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPUART_TransmitReceive/Src/main.c
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

/** @addtogroup LPBAM_LPUART_TransmitReceive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* LPUART handler declaration */
UART_HandleTypeDef UartHandle;
/* LPTIM handle declaration   */
LPTIM_HandleTypeDef LptimHandle;
/* DMA handlers declaration   */
DMA_HandleTypeDef DMAHandle_TX;
DMA_HandleTypeDef DMAHandle_RX;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t        DMAListInfo;
LPBAM_COMMON_TrigAdvConf_t TrigConfig;
LPBAM_UART_FullAdvConf_t   FullAdvConf;
LPBAM_UART_DataAdvConf_t   DataAdvConf;
LPBAM_UART_TxFullDesc_t    TxFullDesc[2U];
LPBAM_UART_RxDataDesc_t    RxDataDesc;

/* UART variables declaration */
UART_AutonomousModeConfTypeDef AutonomousConfig;

/* DMA variables declaration */
DMA_QListTypeDef Tx_Queue;
DMA_QListTypeDef Rx_Queue;

/* Buffer used for transmission */
uint8_t Tx_Buffer[]  = "LPBAM_LPUART_Two_Boards_Communication_LowPower_Mode_Packet_Size_8";
uint8_t Tx_Buffer2[] = "LPBAM_LPUART_Two_Boards_Communication_LowPower_Mode_Packet_Size_4";

/* Buffer used for reception */
uint8_t Rx_Buffer[RX_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config      (void);
#if !defined (DEBUG_CONFIGURATION)
static void SystemPower_Config      (void);
#endif /* !defined (DEBUG_CONFIGURATION) */
static void Error_Handler           (void);
static void Cache_Enable            (void);
#ifdef TRANSMITTER_BOARD
static void LPTIM_Config            (void);
#endif /* TRANSMITTER_BOARD */
static void LPUART_Config           (void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#ifdef TRANSMITTER_BOARD
static void DMA_Rx_LinkedListConfig (void);
static void DMA_Tx_LinkedListConfig (void);
#endif /* TRANSMITTER_BOARD */

/* UserButton flags declaration */
__IO uint32_t UserButtonStatus = 0U;

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

  /* Initialize LED1, LED2 and LED3 : GREEN, BLUE and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* LPUART Initialization sequence */
  LPUART_Config();

#ifdef TRANSMITTER_BOARD
  /* Sequence 1 : advanced lpbam usart receive configuration **********************************************************/
  /* Set DMA information */
  DMAListInfo.pInstance = LPDMA1;
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set transfer information */
  DataAdvConf.Size      = RX_BUFFER_SIZE;
  DataAdvConf.pData     = Rx_Buffer;

  /* Advanced lpbam USART receive */
  if (ADV_LPBAM_UART_Receive_SetDataQ(LPUART1, &DMAListInfo, &DataAdvConf, &RxDataDesc, &Rx_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* DMA linked-list configuration */
  DMA_Rx_LinkedListConfig();


  /* Sequence 2 : advanced lpbam usart transmit configuration *********************************************************/
  /* Set transfer information */
  FullAdvConf.AutoModeConf.AutonomousModeState = LPBAM_UART_AUTONOMOUS_MODE_ENABLE;
  FullAdvConf.AutoModeConf.TriggerPolarity     = LPBAM_UART_TRIG_POLARITY_RISING;
  FullAdvConf.AutoModeConf.TriggerSelection    = LPBAM_LPUART_LPTIM1_OUT_TRG;
  FullAdvConf.AutoModeConf.DataSize            = 8U;
  FullAdvConf.Size                             = TX_BUFFER_SIZE;
  FullAdvConf.pData                            = Tx_Buffer;

  /* Advanced lpbam USART transmit */
  if (ADV_LPBAM_UART_Transmit_SetFullQ(LPUART1, &DMAListInfo, &FullAdvConf, &TxFullDesc[0U], &Tx_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_UART_TX_FULLQ_CONFIG_NODE, &TxFullDesc[0U]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set transfer information */
  FullAdvConf.AutoModeConf.DataSize = 4U;
  FullAdvConf.Size                  = TX_BUFFER_SIZE;
  FullAdvConf.pData                 = Tx_Buffer2;

  /* Advanced lpbam USART transmit */
  if (ADV_LPBAM_UART_Transmit_SetFullQ(LPUART1, &DMAListInfo, &FullAdvConf, &TxFullDesc[1U], &Tx_Queue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set DMA trigger */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_UART_TX_FULLQ_CONFIG_NODE, &TxFullDesc[1U]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* DMA linked-list configuration */
  DMA_Tx_LinkedListConfig();

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Wait for User push-button press before starting the Communication.
  In the meantime, LED2 is blinking */
  while (UserButtonStatus == 0U)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
  }

  /* Turn LED2 off */
  BSP_LED_Off(LED2);

  /* Configure and start LPTIM PWM generation */
  LPTIM_Config ();

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

#else

  /* The board receives the message and sends it back */
  /* Put UART peripheral in reception process */
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)Rx_Buffer, RX_BUFFER_SIZE, 0x1FFFFFF) != HAL_OK)
  {
    Error_Handler();
  }

  /* Turn LED2 on: Transfer in reception process is correct */
  BSP_LED_On(LED2);

  /* Start the transmission process */
  /* While the UART in reception process, user can transmit data through "aTxBuffer" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)Rx_Buffer, RX_BUFFER_SIZE, 5000)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Turn LED2 off: Transfer in transmission process is correct */
  BSP_LED_Off(LED2);

#endif /* TRANSMITTER_BOARD */

  /* Compare the sent and received buffer */
  if((Buffercmp((uint8_t*)Tx_Buffer, (uint8_t*)Rx_Buffer, TX_BUFFER_SIZE) != 0))
  {
    Error_Handler();
  }

  /* Compare the sent and received buffer */
  if((Buffercmp((uint8_t*)Tx_Buffer2, &Rx_Buffer[TX_BUFFER_SIZE], TX_BUFFER_SIZE) != 0))
  {
    Error_Handler();
  }

  /* Turn on LED1 if test passes then enter infinite loop */
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
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
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

  /* Select PLL as system clock source and configure bus clocks dividers */
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

#ifdef TRANSMITTER_BOARD
/**
  * @brief  LPTIM configuration.
  * @param  None
  * @retval None
  */
static void LPTIM_Config (void)
{
  LPTIM_OC_ConfigTypeDef sConfig;
  uint32_t Period = 0x280;
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
  LptimHandle.Init.RepetitionCounter             = 0;

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

  /* Generate PWM signal to trig the UART in order to start transfer */
  if (HAL_LPTIM_PWM_Start (&LptimHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler (); /* KO */
  }
}
#endif /* TRANSMITTER_BOARD */

/**
  * @brief  LPUART configuration.
  * @param  None
  * @retval None
  */
static void LPUART_Config (void)
{
  /* UART configuration */
  UartHandle.Instance                    = LPUART1;
  UartHandle.Init.BaudRate               = 9600;
  UartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits               = UART_STOPBITS_1;
  UartHandle.Init.Parity                 = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode                   = UART_MODE_TX_RX;
  UartHandle.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  /* UART initialisation */
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

#ifdef TRANSMITTER_BOARD
  /* Config Autonomous mode parameters */
  AutonomousConfig.AutonomousModeState = UART_AUTONOMOUS_MODE_ENABLE;
  AutonomousConfig.IdleFrame           = UART_IDLE_FRAME_ENABLE;
  AutonomousConfig.TriggerPolarity     = UART_TRIG_POLARITY_RISING;
  AutonomousConfig.TriggerSelection    = LPUART_LPTIM1_OUT_TRG;
  AutonomousConfig.DataSize            = 5U;

  /* Set Autonomous mode parameters */
  if (HAL_UARTEx_SetConfigAutonomousMode(&UartHandle, &AutonomousConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable stop mode parameters */
  if (HAL_UARTEx_EnableStopMode(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable UART DMA requests */
  if (ADV_LPBAM_UART_EnableDMARequests(UartHandle.Instance) != LPBAM_OK)
  {
    Error_Handler();
  }
#endif /* TRANSMITTER_BOARD */
}

#ifdef TRANSMITTER_BOARD
/**
  * @brief  DMA Rx configuration.
  * @param  None
  * @retval None
  */
static void DMA_Rx_LinkedListConfig (void)
{
    /* Set DMA instance */
  DMAHandle_RX.Instance                               = LPDMA1_Channel1;

  /* Set parameters to be configured */
  DMAHandle_RX.InitLinkedList.Priority                = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle_RX.InitLinkedList.LinkStepMode            = DMA_LSM_FULL_EXECUTION;
  DMAHandle_RX.InitLinkedList.LinkAllocatedPort       = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle_RX.InitLinkedList.TransferEventMode       = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle_RX.InitLinkedList.LinkedListMode          = DMA_LINKEDLIST_NORMAL;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle_RX) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link UART queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle_RX, &Rx_Queue) != HAL_OK)
  {
    Error_Handler();
  }
  /* Enable end of transfer interrupt to wake up from stop2*/
  __HAL_DMA_ENABLE_IT(&DMAHandle_RX, DMA_IT_TC);

  /* Enable DMA interrupt */
  HAL_NVIC_SetPriority(LPDMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel1_IRQn);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start (&DMAHandle_RX) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  DMA Tx configuration.
  * @param  None
  * @retval None
  */
static void DMA_Tx_LinkedListConfig (void)
{
  /* Set DMA instance */
  DMAHandle_TX.Instance                               = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle_TX.InitLinkedList.Priority                = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle_TX.InitLinkedList.LinkStepMode            = DMA_LSM_FULL_EXECUTION;
  DMAHandle_TX.InitLinkedList.LinkAllocatedPort       = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle_TX.InitLinkedList.TransferEventMode       = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle_TX.InitLinkedList.LinkedListMode          = DMA_LINKEDLIST_NORMAL;


  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle_TX) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link UART queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle_TX, &Tx_Queue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start (&DMAHandle_TX) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif /* TRANSMITTER_BOARD */

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if(Button == BUTTON_USER)
  {
    UserButtonStatus = 1;
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
