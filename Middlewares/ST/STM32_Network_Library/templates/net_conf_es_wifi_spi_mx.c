/**
  ******************************************************************************
  * @file    es_wifi_io.c
  * @author  MCD Application Team
  * @brief   This file implements the IO operations to deal with the es-wifi
  *          module. It mainly Inits and Deinits the SPI interface. Send and
  *          receive data over it.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "es_wifi.h"

/* Global variables  --------------------------------------------------------*/
ES_WIFIObject_t    EsWifiObj;

/* Function  definitions  --------------------------------------------------------*/
int32_t wifi_probe(void **ll_drv_context);
void    es_wifi_select_spi(SPI_HandleTypeDef       *pSelectedSpiHandle);
void    es_wifi_HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *SpiHandle);
void    es_wifi_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *SpiHandle);
void    es_wifi_SPI_WIFI_ISR(void);


#if  !defined(ESWIFI_USE_CUBEMX)

#define ISM43362_BOOT0_Pin GPIO_PIN_12
#define ISM43362_BOOT0_GPIO_Port GPIOB
#define ISM43362_RST_Pin GPIO_PIN_8
#define ISM43362_RST_GPIO_Port GPIOE
#define ISM43362_WAKEUP_Pin GPIO_PIN_13
#define ISM43362_WAKEUP_GPIO_Port GPIOB
#define ISM43362_SPI3_CSN_Pin GPIO_PIN_0
#define ISM43362_SPI3_CSN_GPIO_Port GPIOE
#define ISM43362_DRDY_EXTI1_Pin GPIO_PIN_1
#define ISM43362_DRDY_EXTI1_GPIO_Port GPIOE
#define ISM43362_DRDY_EXTI1_EXTI_IRQn EXTI1_IRQn

#define INTERNAL_SPI3_SCK_Pin GPIO_PIN_10
#define INTERNAL_SPI3_SCK_GPIO_Port GPIOC
#define INTERNAL_SPI3_MISO_Pin GPIO_PIN_11
#define INTERNAL_SPI3_MISO_GPIO_Port GPIOC
#define INTERNAL_SPI3_MOSI_Pin GPIO_PIN_12
#define INTERNAL_SPI3_MOSI_GPIO_Port GPIOC

#endif /*  ESWIFI_USE_CUBEMX */

/* Private define ------------------------------------------------------------*/
#define WIFI_RESET_MODULE() do{\
                                HAL_GPIO_WritePin(ISM43362_RST_GPIO_Port, ISM43362_RST_Pin, GPIO_PIN_RESET);\
                                HAL_Delay(10);\
                                HAL_GPIO_WritePin(ISM43362_RST_GPIO_Port, ISM43362_RST_Pin, GPIO_PIN_SET);\
                                HAL_Delay(500);\
                              }while(0);


#define WIFI_ENABLE_NSS() do{ \
                              HAL_GPIO_WritePin( ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_RESET );\
                            }while(0);

#define WIFI_DISABLE_NSS()   do{ \
                                 HAL_GPIO_WritePin( ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET );\
                               }while(0);

#define WIFI_IS_CMDDATA_READY()            (HAL_GPIO_ReadPin(ISM43362_DRDY_EXTI1_GPIO_Port, ISM43362_DRDY_EXTI1_Pin)\
                                            == GPIO_PIN_SET)


/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static  SPI_HandleTypeDef       *pSpiHandle = 0;
static  int32_t                 __IO spi_rx_event = 0;
static  int32_t                 __IO spi_tx_event = 0;
static  int32_t                 __IO cmddata_rdy_rising_event = 0;

#ifdef WIFI_USE_CMSIS_OS
osMutexId es_wifi_mutex;
osMutexDef(es_wifi_mutex);

static    osMutexId spi_mutex;
osMutexDef(spi_mutex);

static    osSemaphoreId spi_rx_sem;
osSemaphoreDef(spi_rx_sem);

static    osSemaphoreId spi_tx_sem;
osSemaphoreDef(spi_tx_sem);

static    osSemaphoreId cmddata_rdy_rising_sem;
osSemaphoreDef(cmddata_rdy_rising_sem);

#endif /* WIFI_USE_CMSIS_OS */

#ifdef WIFI_USE_FREERTOS
SemaphoreHandle_t es_wifi_mutex;
static    SemaphoreHandle_t spi_mutex;
static    SemaphoreHandle_t spi_rx_sem;
static    SemaphoreHandle_t spi_tx_sem;
static    SemaphoreHandle_t cmddata_rdy_rising_sem;

#endif /* WIFI_USE_FREERTOS */


/* Private function prototypes -----------------------------------------------*/
static  int32_t wait_cmddata_rdy_high(int32_t timeout);
static  int32_t wait_cmddata_rdy_rising_event(int32_t timeout);
static  int32_t wait_spi_tx_event(int32_t timeout);
static  int32_t wait_spi_rx_event(int32_t timeout);
static  void SPI_WIFI_DelayUs(uint32_t);
static int8_t  SPI_WIFI_DeInit(void);
static int8_t  SPI_WIFI_Init(uint16_t mode);
static int8_t  SPI_WIFI_ResetModule(void);
static int16_t SPI_WIFI_ReceiveData(uint8_t *pData, uint16_t len, uint32_t timeout);
static int16_t SPI_WIFI_SendData(uint8_t *pData, uint16_t len, uint32_t timeout);
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
                       COM Driver Interface (SPI)
  *******************************************************************************/

#if  !defined(ESWIFI_USE_CUBEMX)
/**
  * @brief  Initialize SPI MSP
  * @param  hspi_es_wifi: SPI handle
  * @retval None
  */
static void localSPI_WIFI_MspInit(SPI_HandleTypeDef *hspi_es_wifi)
{

  GPIO_InitTypeDef GPIO_Init;


  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();


  /* configure Wake up pin */
  HAL_GPIO_WritePin(ISM43362_WAKEUP_GPIO_Port, ISM43362_WAKEUP_Pin, GPIO_PIN_RESET);
  GPIO_Init.Pin       = ISM43362_WAKEUP_Pin;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ISM43362_WAKEUP_GPIO_Port, &GPIO_Init);

  /* configure Data ready pin */
  GPIO_Init.Pin       = ISM43362_DRDY_EXTI1_Pin;
  GPIO_Init.Mode      = GPIO_MODE_IT_RISING;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ISM43362_DRDY_EXTI1_GPIO_Port, &GPIO_Init);

  /* configure Reset pin */
  GPIO_Init.Pin       = ISM43362_RST_Pin;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_Init.Alternate = 0;
  HAL_GPIO_Init(ISM43362_RST_GPIO_Port, &GPIO_Init);

  /* configure SPI NSS pin pin */
  HAL_GPIO_WritePin(GPIOE, ISM43362_SPI3_CSN_Pin, GPIO_PIN_SET);
  GPIO_Init.Pin       =  ISM43362_SPI3_CSN_Pin;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(ISM43362_SPI3_CSN_GPIO_Port, &GPIO_Init);

  __HAL_RCC_SPI3_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  /**SPI3 GPIO Configuration
  PC10     ------> SPI3_SCK
  PC11     ------> SPI3_MISO
  PC12     ------> SPI3_MOSI
  */

  GPIO_Init.Pin = INTERNAL_SPI3_SCK_Pin | INTERNAL_SPI3_MISO_Pin | INTERNAL_SPI3_MOSI_Pin;
  GPIO_Init.Mode = GPIO_MODE_AF_PP;
  GPIO_Init.Pull = GPIO_NOPULL;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(INTERNAL_SPI3_MOSI_GPIO_Port, &GPIO_Init);


  /* Enable Interrupt for SPI tx and rx */
  HAL_NVIC_SetPriority((IRQn_Type)SPI3_IRQn, SPI_INTERFACE_PRIO, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)SPI3_IRQn);

}


/**
  * @brief SPI Initialization Function
  * @param None
  * @retval None
  */
static void localMX_SPI_Init(void)
{

  pSpiHandle->Instance = SPI3;
  pSpiHandle->Init.Mode = SPI_MODE_MASTER;
  pSpiHandle->Init.Direction = SPI_DIRECTION_2LINES;
  pSpiHandle->Init.DataSize = SPI_DATASIZE_16BIT;
  pSpiHandle->Init.CLKPolarity = SPI_POLARITY_LOW;
  pSpiHandle->Init.CLKPhase = SPI_PHASE_1EDGE;
  pSpiHandle->Init.NSS = SPI_NSS_SOFT;
  pSpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;/* 8 */
  pSpiHandle->Init.FirstBit = SPI_FIRSTBIT_MSB;
  pSpiHandle->Init.TIMode = SPI_TIMODE_DISABLE;
  pSpiHandle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pSpiHandle->Init.CRCPolynomial = 7; /* 0*/
  pSpiHandle->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  pSpiHandle->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(pSpiHandle) != HAL_OK)
  {
    while (1);
  }
  localSPI_WIFI_MspInit(pSpiHandle);
}


#endif /* !ESWIFI_USE_CUBEMX*/

/**
  * @brief  Initialize the SPI
  * @param  None
  * @retval None
  */

static int8_t SPI_WIFI_Init(uint16_t mode)
{
  int8_t  rc = 0;

  if (mode == ES_WIFI_INIT)
  {
#if  !defined(ESWIFI_USE_CUBEMX)
    localMX_SPI_Init();

    /* Enable Interrupt for Data Ready pin , GPIO_PIN1 */
    HAL_NVIC_SetPriority((IRQn_Type)ISM43362_DRDY_EXTI1_EXTI_IRQn, SPI_INTERFACE_PRIO, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)ISM43362_DRDY_EXTI1_EXTI_IRQn);
#endif /* !ESWIFI_USE_CUBEMX */

    cmddata_rdy_rising_event = 0;

#ifdef WIFI_USE_CMSIS_OS
#if (osCMSIS < 0x20000U)
    es_wifi_mutex = osMutexCreate(osMutex(es_wifi_mutex));
    spi_mutex = osMutexCreate(osMutex(spi_mutex));
    spi_rx_sem = osSemaphoreCreate(osSemaphore(spi_rx_sem), 1);
    spi_tx_sem = osSemaphoreCreate(osSemaphore(spi_tx_sem), 1);
    cmddata_rdy_rising_sem = osSemaphoreCreate(osSemaphore(cmddata_rdy_rising_sem), 1);
#else
    es_wifi_mutex = osMutexNew(osMutex(es_wifi_mutex));
    spi_mutex = osMutexNew(osMutex(spi_mutex));
    spi_rx_sem = osSemaphoreNew(1, 1, NULL);
    spi_tx_sem = osSemaphoreNew(1, 1, NULL);
    cmddata_rdy_rising_sem = osSemaphoreNew(1, 1, NULL);
#endif /* osCMSIS */

    /* take semaphore */
    if (osOK != SEM_WAIT(cmddata_rdy_rising_sem, 1))
    {
      return -1;
    }
    if (osOK != SEM_WAIT(spi_rx_sem, 1))
    {
      return -1;
    }
    if (osOK != SEM_WAIT(spi_tx_sem, 1))
    {
      return -1;
    }
#endif /* WIFI_USE_CMSIS_OS */

#ifdef WIFI_USE_FREERTOS
    es_wifi_mutex = xSemaphoreCreateBinary();
    spi_mutex = xSemaphoreCreateBinary();
    xSemaphoreGive(es_wifi_mutex);
    xSemaphoreGive(spi_mutex);

    spi_rx_sem = xSemaphoreCreateBinary();
    spi_tx_sem = xSemaphoreCreateBinary();
    cmddata_rdy_rising_sem = xSemaphoreCreateBinary();



#endif /* WIFI_USE_CMSIS_OS */


    /* first call used for calibration */
    SPI_WIFI_DelayUs(10);
  }

  rc = SPI_WIFI_ResetModule();

  return rc;
}


static int8_t SPI_WIFI_ResetModule(void)
{
  uint32_t tickstart = HAL_GetTick();
  uint8_t Prompt[6];
  uint8_t count = 0;
  HAL_StatusTypeDef  Status;

  WIFI_RESET_MODULE();
  WIFI_ENABLE_NSS();
  SPI_WIFI_DelayUs(15);

  while (WIFI_IS_CMDDATA_READY())
  {
    Status = HAL_SPI_Receive(pSpiHandle, &Prompt[count], 1, 0xFFFF);
    count += 2;
    if (((HAL_GetTick() - tickstart) > 0xFFFF) || (Status != HAL_OK))
    {
      WIFI_DISABLE_NSS();
      return -1;
    }
  }

  WIFI_DISABLE_NSS();
  if ((Prompt[0] != 0x15) || (Prompt[1] != 0x15) || (Prompt[2] != '\r') ||
      (Prompt[3] != '\n') || (Prompt[4] != '>') || (Prompt[5] != ' '))
  {
    return -1;
  }
  return 0;
}

/**
  * @brief  DeInitialize the SPI
  * @param  None
  * @retval None
  */
static int8_t SPI_WIFI_DeInit(void)
{
  HAL_SPI_DeInit(pSpiHandle);
#ifdef  WIFI_USE_CMSIS_OS
  osMutexDelete(spi_mutex);
  osMutexDelete(es_wifi_mutex);
  osSemaphoreDelete(spi_tx_sem);
  osSemaphoreDelete(spi_rx_sem);
  osSemaphoreDelete(cmddata_rdy_rising_sem);
#endif /* WIFI_USE_CMSIS_OS */
  return 0;
}



/**
  * @brief  Receive wifi Data from SPI
  * @param  pdata : pointer to data
  * @param  len : Data length
  * @param  timeout : send timeout in mS
  * @retval Length of received data (payload)
  */

int32_t wait_cmddata_rdy_high(int32_t timeout)
{
  int32_t tickstart = HAL_GetTick();
  while (WIFI_IS_CMDDATA_READY() == 0)
  {
    if ((HAL_GetTick() - tickstart) > timeout)
    {
      return -1;
    }
  }
  return 0;
}



int32_t wait_cmddata_rdy_rising_event(int32_t timeout)
{
#ifdef SEM_WAIT
  RETURN_ON_ERROR(SEM_WAIT(cmddata_rdy_rising_sem, timeout), -1);
#else
  int32_t tickstart = HAL_GetTick();
  while (cmddata_rdy_rising_event == 1)
  {
    if ((HAL_GetTick() - tickstart) > timeout)
    {
      return -1;
    }
  }
#endif /* SEM_WAIT */
  return 0;
}

int32_t wait_spi_rx_event(int32_t timeout)
{
#ifdef SEM_WAIT
  RETURN_ON_ERROR(SEM_WAIT(spi_rx_sem, timeout), -1);
#else
  int32_t tickstart = HAL_GetTick();
  while (spi_rx_event == 1)
  {
    if ((HAL_GetTick() - tickstart) > timeout)
    {
      return -1;
    }
  }
#endif /* SEM_WAIT */
  return 0;
}

int32_t wait_spi_tx_event(int32_t timeout)
{
#ifdef SEM_WAIT
  RETURN_ON_ERROR(SEM_WAIT(spi_tx_sem, timeout), -1);
#else
  int32_t tickstart = HAL_GetTick();
  while (spi_tx_event == 1)
  {
    if ((HAL_GetTick() - tickstart) > timeout)
    {
      return -1;
    }
  }
#endif /* SEM_WAIT */
  return 0;
}



int16_t SPI_WIFI_ReceiveData(uint8_t *pData, uint16_t len, uint32_t timeout)
{
  int16_t length = 0;
  uint8_t tmp[2];

  WIFI_DISABLE_NSS();
  SPI_WIFI_DelayUs(3);


  if (wait_cmddata_rdy_rising_event(timeout) != 0)
  {
    return ES_WIFI_ERROR_WAITING_DRDY_FALLING;
  }

  LOCK_SPI();
  WIFI_ENABLE_NSS();
  SPI_WIFI_DelayUs(15);
  while (WIFI_IS_CMDDATA_READY())
  {
    if ((length < len) || (!len))
    {
      spi_rx_event = 1;
      if (HAL_SPI_Receive_IT(pSpiHandle, tmp, 1) != HAL_OK)
      {
        WIFI_DISABLE_NSS();
        UNLOCK_SPI();
        return ES_WIFI_ERROR_SPI_FAILED;
      }

      if (wait_spi_rx_event(timeout) != 0)
      {
        WIFI_DISABLE_NSS();
        UNLOCK_SPI();
        return ES_WIFI_MUTEX_ERROR;
      }

      pData[0] = tmp[0];
      pData[1] = tmp[1];
      length += 2;
      pData  += 2;

      if (length >= ES_WIFI_DATA_SIZE)
      {
        WIFI_DISABLE_NSS();
        SPI_WIFI_ResetModule();
        UNLOCK_SPI();
        return ES_WIFI_ERROR_STUFFING_FOREVER;
      }
    }
    else
    {
      break;
    }
  }
  WIFI_DISABLE_NSS();
  UNLOCK_SPI();
  return length;
}
/**
  * @brief  Send wifi Data through SPI
  * @param  pdata : pointer to data
  * @param  len : Data length
  * @param  timeout : send timeout in mS
  * @retval Length of sent data
  */
int16_t SPI_WIFI_SendData(uint8_t *pdata,  uint16_t len, uint32_t timeout)
{
  uint8_t Padding[2];

  if (wait_cmddata_rdy_high(timeout) != 0)
  {
    return ES_WIFI_ERROR_SPI_FAILED;
  }

  /* arm to detect rising event */
  cmddata_rdy_rising_event = 1;
  LOCK_SPI();
  WIFI_ENABLE_NSS();
  SPI_WIFI_DelayUs(15);
  if (len > 1)
  {
    spi_tx_event = 1;
    if (HAL_SPI_Transmit_IT(pSpiHandle, (uint8_t *)pdata, len / 2) != HAL_OK)
    {
      WIFI_DISABLE_NSS();
      UNLOCK_SPI();
      return ES_WIFI_ERROR_SPI_FAILED;
    }
    if (wait_spi_tx_event(timeout) != 0)
    {
      WIFI_DISABLE_NSS();
      UNLOCK_SPI();
      return ES_WIFI_ERROR_SPI_FAILED;
    }
  }

  if (len & 1)
  {
    Padding[0] = pdata[len - 1];
    Padding[1] = '\n';

    spi_tx_event = 1;
    if (HAL_SPI_Transmit_IT(pSpiHandle, Padding, 1) != HAL_OK)
    {
      WIFI_DISABLE_NSS();
      UNLOCK_SPI();
      return ES_WIFI_ERROR_SPI_FAILED;
    }
    if (wait_spi_tx_event(timeout) != 0)
    {
      WIFI_DISABLE_NSS();
      UNLOCK_SPI();
      return ES_WIFI_ERROR_SPI_FAILED;
    }

  }
  UNLOCK_SPI();
  return len;
}

/**
   * @brief  Delay
  * @param  Delay in us
  * @retval None
  */
void SPI_WIFI_DelayUs(uint32_t n)
{
  __IO        uint32_t ct = 0;
  uint32_t        loop_per_us = 0;
  static uint32_t cycle_per_loop = 0;

  /* calibration happen on first call for a duration of 1 ms * nbcycle per loop */
  /* 10 cycle for STM32L4 */
  if (cycle_per_loop == 0)
  {
    uint32_t cycle_per_ms = (SystemCoreClock / 1000UL);
    uint32_t tick = 0;
    ct = cycle_per_ms;
    tick = HAL_GetTick();
    while (ct)
    {
      ct--;
    }
    cycle_per_loop = HAL_GetTick() - tick;
    if (cycle_per_loop == 0)
    {
      cycle_per_loop = 1;
    }
  }

  loop_per_us = SystemCoreClock / 1000000UL / cycle_per_loop;
  ct = n * loop_per_us;
  while (ct)
  {
    ct--;
  }
  return;
}

/**
  * @brief Rx Transfer completed callback.
  * @param  hspi_es_wifi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */

void es_wifi_HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi_es_wifi)
{
  if (spi_rx_event)
  {
#ifdef SEM_WAIT
    SEM_SIGNAL(spi_rx_sem);
#endif /*  SEM_WAIT */
    spi_rx_event = 0;
  }
}

/**
  * @brief Tx Transfer completed callback.
  * @param  hspi_es_wifi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void es_wifi_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi_es_wifi)
{
  if (spi_tx_event)
  {
#ifdef SEM_WAIT
    SEM_SIGNAL(spi_tx_sem);
#endif /*  SEM_WAIT */
    spi_tx_event = 0;
  }
}


/**
  * @brief  Interrupt handler for  Data RDY signal
  * @param  None
  * @retval None
  */
void    es_wifi_SPI_WIFI_ISR(void)
{
  if (cmddata_rdy_rising_event == 1)
  {
#ifdef SEM_WAIT
    SEM_SIGNAL(cmddata_rdy_rising_sem);
#endif /*  SEM_WAIT */
    cmddata_rdy_rising_event = 0;
  }
}


void es_wifi_select_spi(SPI_HandleTypeDef       *pSelectedSpiHandle)
{
  pSpiHandle = pSelectedSpiHandle;
}


/**
  * @brief  probe function to register wifi to Network library framework
  * @param  None
  * @retval None
  */
int32_t wifi_probe(void **ll_drv_context)
{
  if (ES_WIFI_RegisterBusIO(&EsWifiObj,
                            SPI_WIFI_Init,
                            SPI_WIFI_DeInit,
                            HAL_Delay,
                            SPI_WIFI_SendData,
                            SPI_WIFI_ReceiveData) == 0)
  {
    *ll_drv_context = &EsWifiObj;
    return 0;
  }
  return -1;
}



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
