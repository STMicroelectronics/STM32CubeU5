/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_USART.h"

#include "cmsis.h"
#include "stm32l5xx_hal.h"
/* board configuration  */
#include "board.h"
#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  (void)arg
#endif /* ARG_UNUSED */
#define USART_DRV_VERSION  ARM_DRIVER_VERSION_MAJOR_MINOR(2, 2)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion =
{
  ARM_USART_API_VERSION,
  USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities =
{
  1, /* supports UART (Asynchronous) mode */
  0, /* supports Synchronous Master mode */
  0, /* supports Synchronous Slave mode */
  0, /* supports UART Single-wire mode */
  0, /* supports UART IrDA mode */
  0, /* supports UART Smart Card mode */
  0, /* Smart Card Clock generator available */
  0, /* RTS Flow Control available */
  0, /* CTS Flow Control available */
  0, /* Transmit completed event: \ref USARTx_EVENT_TX_COMPLETE */
  0, /* Signal receive character timeout event: \ref USARTx_EVENT_RX_TIMEOUT */
  0, /* RTS Line: 0=not available, 1=available */
  0, /* CTS Line: 0=not available, 1=available */
  0, /* DTR Line: 0=not available, 1=available */
  0, /* DSR Line: 0=not available, 1=available */
  0, /* DCD Line: 0=not available, 1=available */
  0, /* RI Line: 0=not available, 1=available */
  0, /* Signal CTS change event: \ref USARTx_EVENT_CTS */
  0, /* Signal DSR change event: \ref USARTx_EVENT_DSR */
  0, /* Signal DCD change event: \ref USARTx_EVENT_DCD */
  0, /* Signal RI change event: \ref USARTx_EVENT_RI */
  0  /* Reserved */
};

static ARM_DRIVER_VERSION USART_GetVersion(void)
{
  return DriverVersion;
}

static ARM_USART_CAPABILITIES USART_GetCapabilities(void)
{
  return DriverCapabilities;
}

static UART_HandleTypeDef  uart_device;
static int32_t USART0_Initialize(ARM_USART_SignalEvent_t cb_event)
{

#if !defined(__DOMAIN_NS)
  GPIO_InitTypeDef GPIO_Init;
  /* Configure COM Tx as alternate function */
  COM_TX_GPIO_CLK_ENABLE();
  COM_RX_GPIO_CLK_ENABLE();
  COM_CLK_ENABLE();
  GPIO_Init.Pin       = COM_TX_PIN;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Alternate = COM_TX_AF;
  HAL_GPIO_Init(COM_TX_GPIO_PORT, &GPIO_Init);

  /* Configure COM Rx as alternate function */
  GPIO_Init.Pin       = COM_RX_PIN;
  GPIO_Init.Alternate = COM_RX_AF;
  HAL_GPIO_Init(COM_RX_GPIO_PORT, &GPIO_Init);
#endif /* __DOMAIN_NS */
  uart_device.Instance = COM_INSTANCE;
  uart_device.Init.BaudRate       = 115200;
  uart_device.Init.WordLength     = UART_WORDLENGTH_8B;
  uart_device.Init.StopBits       = UART_STOPBITS_1;
  uart_device.Init.Parity         = UART_PARITY_NONE;
  uart_device.Init.Mode           = UART_MODE_TX_RX;
  uart_device.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  uart_device.Init.OverSampling   = UART_OVERSAMPLING_8;
  uart_device.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  uart_device.Init.ClockPrescaler = UART_PRESCALER_DIV1;

  /* Initialize COM */
  if (HAL_UART_Init(&uart_device) != HAL_OK)
  {
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

static int32_t USART0_Uninitialize(void)
{
  /* Nothing to be done */
  return ARM_DRIVER_OK;
}

static int32_t USART0_PowerControl(ARM_POWER_STATE state)
{
  return ARM_DRIVER_OK;
}

static int32_t USART0_Send(const void *data, uint32_t num)
{
  if ((data == NULL) || (num == 0U))
  {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  HAL_UART_Transmit(&uart_device, (uint8_t *) data, num,  1000);

  return ARM_DRIVER_OK;
}

static int32_t USART0_Receive(void *data, uint32_t num)
{
  if ((data == NULL) || (num == 0U))
  {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  HAL_UART_Receive_IT(&uart_device, data, num);

  return num;
}

static int32_t USART0_Transfer(const void *data_out, void *data_in,
                               uint32_t num)
{
  ARG_UNUSED(data_out);
  ARG_UNUSED(data_in);
  ARG_UNUSED(num);

  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static uint32_t USART0_GetTxCount(void)
{
  return 0;
}

static uint32_t USART0_GetRxCount(void)
{
  return 0;
}
static int32_t USART0_Control(uint32_t control, uint32_t arg)
{
  return ARM_DRIVER_OK;
}

static ARM_USART_STATUS USART0_GetStatus(void)
{
  ARM_USART_STATUS status = {0, 0, 0, 0, 0, 0, 0, 0};
  return status;
}

static int32_t USART0_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
  ARG_UNUSED(control);
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_USART_MODEM_STATUS USART0_GetModemStatus(void)
{
  ARM_USART_MODEM_STATUS modem_status = {0, 0, 0, 0, 0};
  return modem_status;
}

ARM_DRIVER_USART Driver_USART0 =
{
  USART_GetVersion,
  USART_GetCapabilities,
  USART0_Initialize,
  USART0_Uninitialize,
  USART0_PowerControl,
  USART0_Send,
  USART0_Receive,
  USART0_Transfer,
  USART0_GetTxCount,
  USART0_GetRxCount,
  USART0_Control,
  USART0_GetStatus,
  USART0_SetModemControl,
  USART0_GetModemStatus
};
