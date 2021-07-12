/**
  ******************************************************************************
  * @file    com.c
  * @author  MCD Application Team
  * @brief   COM module.
  *          This file provides set of firmware functions to manage Com
  *          functionalities.
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
#include "com.h"
#include "stm32u5xx_hal.h"



/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup USER_APP_COMMON Common
  * @{
  */

/** @defgroup  COM_Private_Defines Private Defines
  * @{
  */
const char YMODEM_IT_MSG[]="\r\nYmodem stop by printf\r\n";
/**
  * @}
  */

/** @defgroup  COM_Private_Variables Exported Variables
  * @{
  */

static UART_HandleTypeDef   UartHandle; /*!< Uart Handler*/

static volatile int Ymodem = 0;              /*!< set to 1 when Ymodem uses UART> */
static uint8_t Abort;
/**
  * @}
  */

/** @defgroup  COM_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup  COM_Initialization_Functions Initialization Functions
  * @{
  */

/**
  * @brief  Initialize COM module.
  * @param  None.
  * @retval HAL Status.
  */
HAL_StatusTypeDef  COM_Init(void)
{
#if defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0);
#endif /* __GNUC__ */

  /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
  /* USART configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  UartHandle.Instance = COM_UART;
  UartHandle.Init.BaudRate = 115200U;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_RX | UART_MODE_TX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  UartHandle.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  UartHandle.FifoMode = UART_FIFOMODE_ENABLE;
  return HAL_UART_Init(&UartHandle);
}

/**
  * @brief  DeInitialize COM module.
  * @retval None.
  * @retval HAL Status.
  */
HAL_StatusTypeDef  COM_DeInit(void)
{
  /*
  * ADD SRC CODE HERE
  * ...
  */
  return HAL_OK;
}

/**
  * @brief  UART MSP Init.
  * @param  huart: UART handler pointer.
  * @retval None.
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if (huart->Instance == COM_UART)
  {
    /* Peripheral Clock Enable */
    COM_UART_CLK_ENABLE();

    /* GPIO Ports Clock Enable */
    COM_UART_TX_GPIO_CLK_ENABLE();
    COM_UART_RX_GPIO_CLK_ENABLE();

    /*Configure GPIO pins : COM_UART_TX_Pin  */
    GPIO_InitStruct.Pin = COM_UART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = COM_UART_TX_AF;
    HAL_GPIO_Init(COM_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : COM_UART_RX_Pin  */
    GPIO_InitStruct.Pin = COM_UART_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = COM_UART_RX_AF;
    HAL_GPIO_Init(COM_UART_RX_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * @brief  UART MSP DeInit.
  * @param  huart: UART handler pointer.
  * @retval None.
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == COM_UART)
  {
    /* Peripheral clock disable */
    COM_UART_CLK_DISABLE();

    HAL_GPIO_DeInit(COM_UART_TX_GPIO_PORT, COM_UART_TX_PIN);
    HAL_GPIO_DeInit(COM_UART_RX_GPIO_PORT, COM_UART_RX_PIN);
  }
}


/**
  * @}
  */

/** @defgroup  COM_Control_Functions Control Functions
  * @{
  */

/**
  * @brief Transmit Data.
  * @param uDataLength: Data pointer to the Data to transmit.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Transmit operation.
  */
HAL_StatusTypeDef COM_Transmit(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  HAL_StatusTypeDef status;
  uint32_t i;
  if (Ymodem)
  {
    /* send abort Y modem transfer sequence */
    for(i=0;i<5;i++)
    {
        status = HAL_UART_Transmit(&UartHandle, (uint8_t *)&Abort, 1, uTimeout);
        if (status != HAL_OK)
            return status;
    }
    status = HAL_UART_Transmit(&UartHandle,(uint8_t *)YMODEM_IT_MSG, sizeof(YMODEM_IT_MSG), uTimeout);
    if (status != HAL_OK)
            return status;
    Ymodem=0;
  }
  return HAL_UART_Transmit(&UartHandle, (uint8_t *)Data, uDataLength, uTimeout);
}

/**
  * @brief Transmit Data function for Y modem.
  * @param uDataLength: Data pointer to the Data to transmit.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Transmit operation.
  */
HAL_StatusTypeDef COM_Transmit_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (Ymodem)
    return HAL_UART_Transmit(&UartHandle, (uint8_t *)Data, uDataLength, uTimeout);
  else
    return HAL_ERROR;
}


/**
  * @brief Receive Data.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
HAL_StatusTypeDef COM_Receive(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (!Ymodem)
    return HAL_UART_Receive(&UartHandle, (uint8_t *)Data, uDataLength, uTimeout);
  else
    return HAL_ERROR;
}
/**
  * @brief Receive Data function Y Modem.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
HAL_StatusTypeDef COM_Receive_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (Ymodem)
    return HAL_UART_Receive(&UartHandle, (uint8_t *)Data, uDataLength, uTimeout);
  else
    return HAL_BUSY;
}

/**
  * @brief  Flush COM Input.
  * @param None.
  * @retval HAL_Status.
  */
HAL_StatusTypeDef COM_Flush(void)
{
  /* Clean the input path */
  __HAL_UART_FLUSH_DRREGISTER(&UartHandle);
  return HAL_OK;
}


HAL_StatusTypeDef  COM_Y_On(uint8_t AbortChar)
{
    HAL_StatusTypeDef status=HAL_ERROR;
    if (!Ymodem)
    {
        Ymodem = 1;
        Abort=AbortChar;
        status=HAL_OK;
    }
    return status;
}
HAL_StatusTypeDef  COM_Y_Off(void)
{
    HAL_StatusTypeDef status=HAL_ERROR;
    if (Ymodem)
    {
        Ymodem = 0;
        status=HAL_OK;
    }
    return status;
}
/**
  * @}
  */

/** @addtogroup  COM_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup COM_Callback_Functions Callback Functions
  * @{
  */

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  FileSize Dimension of the file that will be received.
  * @retval None
  */
__weak HAL_StatusTypeDef Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{

  /* NOTE : This function should not be modified, when the callback is needed,
            the Ymodem_HeaderPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
}

/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  Size Packet dimension.
  * @retval None
  */
__weak HAL_StatusTypeDef Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{

  /* NOTE : This function should not be modified, when the callback is needed,
            the Ymodem_DataPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
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
