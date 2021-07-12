/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_I2C_SequentialTransfer/Src/main.c
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

/** @addtogroup LPBAM_I2C_SequentialTransfer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA handler declaration */
DMA_HandleTypeDef DMAHandle;
/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t            DMAListInfo;
LPBAM_I2C_DataAdvConf_t        I2C_DataAdvConf;
LPBAM_I2C_MasterTxDataDesc_t   MasterTxDataDesc;
LPBAM_I2C_MasterRxDataDesc_t   MasterRxDataDesc[2U];
LPBAM_I2C_SlaveRxDataDesc_t    SlaveRxDataDesc;
LPBAM_I2C_SlaveTxDataDesc_t    SlaveTxDataDesc[2U];
LPBAM_I2C_MasterStopGenDesc_t  MasterStopGenerationDesc;

/* LPBAM variables declaration */
DMA_QListTypeDef I2C_TransmitQueue;


/* Buffers used for transmission */
uint8_t aTxBuffer1[] = " ############### LPBAM I2C Application ############### ";

uint8_t aTxBuffer2[] = " ############### This application shows how to use LPBAM I2C advanced utility to perform sequential data transfer in master and slave mode ############### \
                         ############### This application shows how to use LPBAM I2C advanced utility to perform sequential data transfer in master and slave mode ############### ";

/* Buffer used for reception */
uint8_t aRxBuffer1[RXBUFFERSIZE1];
uint8_t aRxBuffer2[RXBUFFERSIZE2];

/* DMA flags declaration */
__IO uint32_t TransferCompleteDetected = 0U, TransferErrorDetected = 0U;
/* Application variables */
uint32_t TransferIdx = 0U, data_size;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CACHE_Enable(void);
static void TransferComplete(DMA_HandleTypeDef *hdma);
static void TransferError(DMA_HandleTypeDef *hdma);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

static void I2C_Config(void);
#ifdef MASTER_BOARD
static void RTC_AlarmConfig(void);
#endif /* MASTER_BOARD */
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
  CACHE_Enable();

  /* Configure the System clock to have a frequency of 160 MHz */
  SystemClock_Config();

  /* Initialize LED1, LED2 and LED3 : GREEN, BLUE and RED leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* I2C configuration */
  I2C_Config();

#ifdef MASTER_BOARD
  /* Sequence 1 : Transmit in master mode N1 data < 255 bytes *********************************************************/
  /* Set queue type */
  DMAListInfo.QueueType                           = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set transfer information */
  I2C_DataAdvConf.DevAddress                      = I2C_ADDRESS;
  I2C_DataAdvConf.SequenceNumber                  = 2U;
  I2C_DataAdvConf.AddressingMode                  = LPBAM_I2C_ADDRESSINGMODE_7BIT;
  I2C_DataAdvConf.Size                            = RXBUFFERSIZE1;
  I2C_DataAdvConf.pData                           = aTxBuffer1;
  I2C_DataAdvConf.AutoModeConf.TriggerState       = LPBAM_I2C_AUTO_MODE_ENABLE;

  /* Advanced lpbam I2C master transmit set data */
  if (ADV_LPBAM_I2C_MasterTransmit_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &MasterTxDataDesc, &I2C_TransmitQueue) != LPBAM_OK)
  {
    Error_Handler();
  }


  /* Sequence 2 : Receive in master mode N2 data > 255 bytes **********************************************************/
  data_size             = RXBUFFERSIZE2;

  /* Set transfer information */
  I2C_DataAdvConf.Size  = data_size;
  I2C_DataAdvConf.pData = aRxBuffer2;

  while (I2C_DataAdvConf.Size != 0U)
  {
    /* Advanced lpbam I2C master receive set data*/
    if (ADV_LPBAM_I2C_MasterReceive_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &MasterRxDataDesc[TransferIdx], &I2C_TransmitQueue) != LPBAM_OK)
    {
      Error_Handler();
    }

    TransferIdx++;

    if (data_size > I2C_MAX_DATA_SIZE)
    {
      data_size -= I2C_MAX_DATA_SIZE;
    }
    else
    {
      data_size = 0U;
    }

    /* Set new data size */
    I2C_DataAdvConf.Size = data_size;
  }

  /* Advanced lpbam I2C master stop generation */
  if (ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ(I2C3,&DMAListInfo, &MasterStopGenerationDesc, &I2C_TransmitQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
  }

  /* Turn off LED2 */
  BSP_LED_Off(LED2);

  /* Delay to avoid that possible signal rebound is taken as button release */
  HAL_Delay(50);

  /* Wait for User push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* RTC alarm configuration */
  RTC_AlarmConfig();

#else
  /* Sequence 1 : Receive in slave mode N1 data < 255 bytes ***********************************************************/
  /* Set queue type */
  DMAListInfo.QueueType = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set transfer information */
  I2C_DataAdvConf.SequenceNumber = 2U;
  I2C_DataAdvConf.Size           = RXBUFFERSIZE1;
  I2C_DataAdvConf.pData          = aRxBuffer1;

  /* Advanced lpbam I2C slave receive set data */
  if (ADV_LPBAM_I2C_SlaveReceive_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &SlaveRxDataDesc, &I2C_TransmitQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Sequence 2 : Transmit in slave mode N2 data > 255 bytes **********************************************************/
  data_size             = RXBUFFERSIZE2;
  /* Set transfer information */
  I2C_DataAdvConf.Size  = data_size;
  I2C_DataAdvConf.pData = aTxBuffer2;

  while (I2C_DataAdvConf.Size != 0U)
  {
    /* Advanced lpbam I2C slave transmit set data*/
    if (ADV_LPBAM_I2C_SlaveTransmit_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &SlaveTxDataDesc[TransferIdx], &I2C_TransmitQueue) != LPBAM_OK)
    {
      Error_Handler();
    }

    TransferIdx++;

    if (data_size > I2C_MAX_DATA_SIZE)
    {
      data_size -= I2C_MAX_DATA_SIZE;
    }
    else
    {
      data_size = 0U;
    }

    /* Set new data size */
    I2C_DataAdvConf.Size = data_size;
  }
#endif /* MASTER_BOARD */

  /* DMA linked-list configuration */
  DMA_LinkedListConfig();

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

#ifdef MASTER_BOARD
  /* Compare aRxBuffer2 with expected content */
  if (Buffercmp(aTxBuffer2, aRxBuffer2, RXBUFFERSIZE2) != 0U)
  {
    Error_Handler();
  }

  /* DeInitialize the RTC peripheral */
  if (HAL_RTC_DeInit(&RTCHandle) != HAL_OK)
  {
    Error_Handler();
  }
#else
  /* Compare aRxBuffer1 with expected content */
  if (Buffercmp(aTxBuffer1, aRxBuffer1, RXBUFFERSIZE1) != 0U)
  {
    Error_Handler();
  }
#endif /* MASTER_BOARD */

  /* Check DMA flags */
  if ((TransferCompleteDetected == 0U) && (TransferErrorDetected != 0U))
  {
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(10U);
  }
}

/**
  * @brief  I2C configuration.
  * @param  None
  * @retval None
  */
static void DMA_LinkedListConfig (void)
{
  /* Set DMA instance */
  DMAHandle.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &I2C_TransmitQueue) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the NVIC for LPDMA1 Channel 0 */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);

  /* Select Callbacks functions called after Transfer Complete and Transfer Error */
  HAL_DMA_RegisterCallback(&DMAHandle, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&DMAHandle, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start_IT(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  I2C configuration.
  * @param  None
  * @retval None
  */
static void I2C_Config (void)
{
  /* Set I2C instance */
  I2cHandle.Instance              = I2C3;

  /* Configure the I2C peripheral */
  I2cHandle.Init.Timing           = I2C_GetTiming(HAL_RCC_GetPCLK1Freq(), BUS_I2Cx_FREQUENCY);
  I2cHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  I2cHandle.Init.OwnAddress1      = I2C_ADDRESS;
  I2cHandle.Init.OwnAddress2      = 0xFF;

  /* Initializes the I2C according to the specified parameters */
  if (HAL_I2C_Init (&I2cHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  if (HAL_I2CEx_ConfigAnalogFilter (&I2cHandle, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

#ifdef MASTER_BOARD
  I2C_AutonomousModeConfTypeDef AutonomousModeConf;

  /* Set Autonomous Mode configuration */
  AutonomousModeConf.TriggerState     = I2C_AUTO_MODE_ENABLE;
  AutonomousModeConf.TriggerPolarity  = I2C_TRIG_POLARITY_RISING;
  AutonomousModeConf.TriggerSelection = I2C_GRP2_RTC_ALRA_TRG;
  if (HAL_I2CEx_SetConfigAutonomousMode(&I2cHandle, &AutonomousModeConf) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable Transfer complete interrupt */
  __HAL_I2C_ENABLE_IT(&I2cHandle, I2C_IT_TCI);
#else
  /* Enable STOP detection interrupt */
  __HAL_I2C_ENABLE_IT(&I2cHandle, I2C_IT_STOPI);
#endif /* MASTER_BOARD */

  /* Enable I2C to wake up CPU from stop mode */
  if (HAL_I2CEx_EnableWakeUp(&I2cHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable errors interrupt */
  __HAL_I2C_ENABLE_IT(&I2cHandle, I2C_IT_ERRI);

  /* Enable DMA request */
  if (ADV_LPBAM_I2C_EnableDMARequests(I2cHandle.Instance) != LPBAM_OK)
  {
    Error_Handler();
  }
}

#ifdef MASTER_BOARD
/**
  * @brief  Configure the RTC alarm.
  * @param  None
  * @retval None
  */
static void RTC_AlarmConfig(void)
{
  RTC_PrivilegeStateTypeDef privilegeState = {0};
  RTC_TimeTypeDef           sTime          = {0};
  RTC_DateTypeDef           sDate          = {0};
  RTC_AlarmTypeDef          sAlarm         = {0};

  /* RTC Init */
  RTCHandle.Instance = RTC;
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 12
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Push pull */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RTCHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_PUSHPULL;
  RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    Error_Handler();
  }
  privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
  privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
  privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
  privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
  if (HAL_RTCEx_PrivilegeModeSet(&RTCHandle, &privilegeState) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize RTC and set the Time and Date*/
  /* Set Time: 02:20:00 */
  sTime.Hours = 0x2;
  sTime.Minutes = 0x20;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&RTCHandle, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* Set Date */
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_FEBRUARY;
  sDate.Date = 0x18;
  sDate.Year = 0x14;
  if (HAL_RTC_SetDate(&RTCHandle, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* Set Alarm at 02:20:30*/
  sAlarm.AlarmTime.Hours = 0x2;
  sAlarm.AlarmTime.Minutes = 0x20;
  sAlarm.AlarmTime.Seconds = 0x05;
  sAlarm.AlarmTime.SubSeconds = 0x56;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&RTCHandle, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
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
  __HAL_RCC_PWR_CLK_DISABLE();

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
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(DMA_HandleTypeDef *hdma)
{
  TransferCompleteDetected = 1U;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void TransferError(DMA_HandleTypeDef *hdma)
{
  TransferErrorDetected = 1U;
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while(1)
  {
  }
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void Custom_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)
{

  /* Check whether NACK interrupt is enabled or not */
  if (__HAL_I2C_GET_IT_SOURCE(hi2c, I2C_IT_NACKI) != 0U)
  {
    /* Check if NACK received flag is active */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) != 0U)
    {
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

      Error_Handler();
    }
  }

    /* Check if TC flag is active */
  if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TC) != 0U)
  {
    /* generate stop condition to clear TC flag */
    hi2c->Instance->CR2 |= I2C_CR2_STOP;
  }

  /* Check if stop condition received */
  if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) != 0U)
  {
    /* Clear STOP Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
  }
}

/**
  * @brief  This function handles I2C interrupt errors.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void Custom_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    /* Check if arbitration lost flag is active */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ARLO) != 0U)
    {
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ARLO);
    }
    /* Check if bus error flag is active */
    else if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BERR) != 0U)
    {
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_BERR);
    }
    /* Check if overrun flag is active */
    else if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_OVR) != 0U)
    {
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_OVR);
    }

    Error_Handler();
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
