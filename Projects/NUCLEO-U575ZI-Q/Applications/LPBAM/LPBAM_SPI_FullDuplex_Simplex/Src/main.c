/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_SPI_FullDuplex_Simplex/Src/main.c
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

/** @addtogroup LPBAM_SPI_FullDuplex_Simplex
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handlers declaration  */
DMA_HandleTypeDef DMAHandle_TX;
DMA_HandleTypeDef DMAHandle_RX;
/* SPI handler declaration   */
SPI_HandleTypeDef SPIHandle;
/* LPTIM handler declaration */
LPTIM_HandleTypeDef LPTIMHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t         DMAListInfo;
LPBAM_COMMON_TrigAdvConf_t  TrigConfig;
LPBAM_SPI_FullAdvConf_t     FullAdvConf;
LPBAM_SPI_DataAdvConf_t     DataAdvConf;
LPBAM_SPI_TxRx_TxFullDesc_t TxRx_TxFullDesc;
LPBAM_SPI_TxRx_RxFullDesc_t TxRx_RxFullDesc;
LPBAM_SPI_TxRx_TxDataDesc_t TxRx_TxDataDesc;
LPBAM_SPI_TxRx_RxDataDesc_t TxRx_RxDataDesc;
LPBAM_SPI_TxFullDesc_t      TxFullDesc;

/* DMA variables declaration */
DMA_QListTypeDef TxQueue;
DMA_QListTypeDef RxQueue;

/* Buffers used for transmission */
uint8_t  aTxBuffer1[]   = " ############### LPBAM SPI Application ############### ";
uint8_t  aDummyBuffer[] = " ################ SPI Full Duplex then Simplex Transfer ############### ";
uint16_t aTxBuffer2[TXBUFFERSIZE2];

/* Buffer used for reception */
uint8_t  aRxBuffer1[RXBUFFERSIZE1];
uint16_t aRxBuffer2[RXBUFFERSIZE2];

/* UserButton flags declaration */
__IO uint32_t UserButtonStatus = 0U;

/* Transfer state */
__IO uint32_t wTransferState = TRANSFER_WAIT;


/* Private function prototypes -----------------------------------------------*/
/* System configuration APIs */
static void SystemClock_Config(void);
static void CACHE_Enable(void);

/* LPTIM configuration API */
static void SPI_Config(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void BufferProcessing(uint8_t* pBuffer1_8bits, uint16_t* pBuffer1_16bits, uint16_t BufferLength_8Bit);

#ifdef MASTER_BOARD
/* DMA linked-list configuration APIs */
static void DMA_Rx_LinkedListConfig(void);
static void DMA_Tx_LinkedListConfig(void);

/* LPTIM configuration API */
void LPTIM_Config(void);
#endif /* MASTER_BOARD */

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

  /* Initialize LED1, LED2 and LED3 : GREEN, BLUE and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

#ifdef MASTER_BOARD
  /* Set SPI Master instance */
  SPIHandle.Instance  = SPIx;
  SPIHandle.Init.Mode = SPI_MODE_MASTER;
#else
  /* Set SPI Slave instance */
  SPIHandle.Instance  = SPIy;
  SPIHandle.Init.Mode = SPI_MODE_SLAVE;
#endif /* MASTER_BOARD */

  /* SPI configuration */
  SPI_Config();

  /* aTxBuffer2 Data buffer processing */
  BufferProcessing((uint8_t*)aDummyBuffer, (uint16_t*)aTxBuffer2, (TXBUFFERSIZE2 * 2U));

#ifdef MASTER_BOARD
  /* Communication 1: Transmit & Receive data in FullDuplex mode ******************************************************/
  /* Advanced LPBAM SPI Receive configuration */
  /* Set DMA information */
  DMAListInfo.pInstance                 = LPDMA1;
  DMAListInfo.QueueType                 = LPBAM_LINEAR_ADDRESSING_Q;
  /* Set transfer information */
  DataAdvConf.DataSize                  = LPBAM_SPI_DATASIZE_8BIT;
  DataAdvConf.AutoModeConf.TriggerState = LPBAM_SPI_AUTO_MODE_DISABLE;
  DataAdvConf.pTxData                   = aTxBuffer1;
  DataAdvConf.pRxData                   = aRxBuffer1;
  DataAdvConf.Size                      = RXBUFFERSIZE1;

  /* Advanced LPBAM SPI Receive */
  if (ADV_LPBAM_SPI_TransmitReceive_SetDataQ(SPIx, &DMAListInfo, &DataAdvConf, &TxRx_TxDataDesc, &TxRx_RxDataDesc, &TxQueue, &RxQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* DMA linked-list configuration */
  DMA_Rx_LinkedListConfig();

  /* Setup LPTIM trigger for SPI transfers */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_SPI_TXRX_DATAQ_CONFIG_NODE, &TxRx_TxDataDesc) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Communication 2: Transmit Data in Simplex mode *******************************************************************/
  /* Advanced LPBAM SPI Transmit configuration */
  /* Set transfer information */
  FullAdvConf.BaudRatePrescaler = LPBAM_SPI_BAUDRATEPRESCALER_64;
  FullAdvConf.DataSize          = LPBAM_SPI_DATASIZE_16BIT;
  FullAdvConf.WakeupIT          = LPBAM_SPI_IT_EOT;
  FullAdvConf.pTxData           = (uint8_t *)aTxBuffer2;
  FullAdvConf.Size              = TXBUFFERSIZE2;

  /* Advanced LPBAM SPI Transmit */
  if (ADV_LPBAM_SPI_Transmit_SetFullQ(SPIx, &DMAListInfo, &FullAdvConf, &TxFullDesc, &TxQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Setup LPTIM trigger for SPI transfers */
  TrigConfig.TriggerConfig.TriggerMode      = DMA_TRIGM_BLOCK_TRANSFER;
  TrigConfig.TriggerConfig.TriggerPolarity  = DMA_TRIG_POLARITY_RISING;
  TrigConfig.TriggerConfig.TriggerSelection = LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&TrigConfig, LPBAM_SPI_TX_FULLQ_CONFIG_NODE, &TxFullDesc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* DMA linked-list configuration */
  DMA_Tx_LinkedListConfig();

  /* Wait for User push-button press before starting the Communication.
   * In the meantime, BLUE LED is blinking
   */
  while (UserButtonStatus == 0U)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
  }

  /* Turn LED2 off */
  BSP_LED_Off(LED2);

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
#else /* SLAVE_BOARD */
  /* Communication 1: Transmit & Receive data in FullDuplex mode ******************************************************/
  /* Put SPI Slave instance in the fullduplex process */
  if(HAL_SPI_TransmitReceive_IT(&SPIHandle, (uint8_t *)aTxBuffer1, (uint8_t *)aRxBuffer1, RXBUFFERSIZE1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait for the end of the transfer */
  while (wTransferState == TRANSFER_WAIT)
  {
  }

  /* Initialize wTransferState to performe the second communication */
  wTransferState = TRANSFER_WAIT;

  /* Communication 2: Receive Data in Simplex mode ********************************************************************/
  /* SPI reconfiguration */
  SPIHandle.Init.DataSize  = SPI_DATASIZE_16BIT;
  SPIHandle.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  if(HAL_SPI_Init(&SPIHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Put SPI Slave instance in the simplex process */
  if(HAL_SPI_Receive_IT(&SPIHandle, (uint8_t *)aRxBuffer2, RXBUFFERSIZE2) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait for the end of the transfer */
  while (wTransferState == TRANSFER_WAIT)
  {
  }

  /* Compare the sent and received buffers of the second communication */
  if((Buffercmp((uint8_t*)aTxBuffer2, (uint8_t*)aRxBuffer2, TXBUFFERSIZE2) != 0U))
  {
    Error_Handler();
  }
#endif /* MASTER_BOARD */

  /* Compare the sent and received buffers of the first communication */
  if((Buffercmp((uint8_t*)aTxBuffer1, (uint8_t*)aRxBuffer1, TXBUFFERSIZE1) != 0U))
  {
    Error_Handler();
  }

  /* Turn on LED1 "GREEN": Full Duplex communication are completed */
  BSP_LED_On(LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

#ifndef MASTER_BOARD
/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Turn LED1 on: Transfer in transmission process is complete */
  BSP_LED_On(LED1);

  wTransferState = TRANSFER_COMPLETE;
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Turn LED1 on: Transfer in transmission process is complete */
  BSP_LED_On(LED1);

  wTransferState = TRANSFER_COMPLETE;
}
#endif /* MASTER_BOARD */

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  wTransferState = TRANSFER_ERROR;
}


#ifdef MASTER_BOARD
/**
  * @brief  DMA Rx configuration.
  * @param  None
  * @retval None
  */
static void DMA_Rx_LinkedListConfig (void)
{
  /* Set DMA instance */
  DMAHandle_RX.Instance                         = LPDMA1_Channel1;

  /* Set parameters to be configured */
  DMAHandle_RX.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle_RX.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle_RX.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle_RX.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle_RX.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle_RX) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link UART queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle_RX, &RxQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Associate the initialized DMA handle to the the SPI handle */
  __HAL_LINKDMA(&SPIHandle, hdmarx, DMAHandle_RX);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle_RX) != HAL_OK)
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
  if (HAL_DMAEx_List_LinkQ(&DMAHandle_TX, &TxQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Associate the initialized DMA handle to the the SPI handle */
  __HAL_LINKDMA(&SPIHandle, hdmatx, DMAHandle_TX);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle_TX) != HAL_OK)
  {
    Error_Handler();
  }
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
#endif /* MASTER_BOARD */

/**
  * @brief  SPI configuration.
  * @param  None
  * @retval None
  */
static void SPI_Config (void)
{
  /* Configure the SPI peripheral */
  SPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SPIHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SPIHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SPIHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SPIHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SPIHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SPIHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SPIHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SPIHandle.Init.CRCPolynomial     = 7;
  SPIHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  SPIHandle.Init.NSS               = SPI_NSS_SOFT;
  SPIHandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
  SPIHandle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;

  /* Initializes the SPI according to the specified parameters */
  if(HAL_SPI_Init(&SPIHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable SPI error interrupts */
  __HAL_SPI_ENABLE_IT(&SPIHandle, (SPI_IT_ERR | SPI_IT_UDR | SPI_IT_OVR));

#ifdef MASTER_BOARD
  /* Enable SPI DMA requests */
  if(ADV_LPBAM_SPI_EnableDMARequests(SPIHandle.Instance) != LPBAM_OK)
  {
    Error_Handler();
  }
#endif /* MASTER_BOARD */
}

#ifdef MASTER_BOARD
/**
  * @brief  LPTIM configuration.
  * @param  None
  * @retval None
  */
void LPTIM_Config (void)
{
  LPTIM_OC_ConfigTypeDef sConfig;

  /*
     Set period value :
     LSE = 32KH
     (1/32) * (10^-3) * 16 = 2 mS.
  */
  uint32_t Period = 32000 / 100;
  /*
     Set pulse value :
     LSE = 32KH
     (1/32) * (10^-3) * 16 = 2 mS.
  */
  uint32_t Pulse  = Period / 2U;

  /* Set lptim instance */
  LPTIMHandle.Instance = LPTIM1;

  /* Set parameters to be configured */
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
  LPTIMHandle.Init.Period                        = Period;
  LPTIMHandle.Init.RepetitionCounter             = 0;

  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LPTIMHandle) != HAL_OK)
  {
    Error_Handler (); /* KO */
  }

  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;

  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler (); /* KO */
  }

  /* Generate one pulse : LPTIM_OUT is used to trig the DMA in order to start
  transactions                                                             */
  if (HAL_LPTIM_PWM_Start(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    while (1); /* KO */
  }
}
#endif /* MASTER_BOARD */

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
    /* Initialization Error */
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
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Exchange buffer data
  * @param  pBuffer1_8bits, pBuffer1_16bits: buffers to be compared.
  * @param  BufferLength: pBuffer1_8bits length
  */
static void BufferProcessing(uint8_t* pBuffer1_8bits, uint16_t* pBuffer1_16bits, uint16_t BufferLength_8Bit)
{
  uint16_t idx_8bit = 0U;
  uint16_t idx_16bit = 0U;

  while (idx_8bit < BufferLength_8Bit)
  {
    pBuffer1_16bits[idx_16bit] = (pBuffer1_8bits[idx_8bit] << 8U) | pBuffer1_8bits[idx_8bit + 1U];

    idx_8bit  += 2U;
    idx_16bit += 1U;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0   : pBuffer1 identical to pBuffer2
  *         > 0 : pBuffer1 differs from pBuffer2
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED1 off */
  BSP_LED_Off(LED1);

  /* Turn LED2 off */
  BSP_LED_Off(LED2);

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
