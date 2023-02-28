/**
  ******************************************************************************
  * @file    usart_interface.c
  * @author  MCD Application Team
  * @brief   Contains USART HW configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

#include "platform.h"
#include "interfaces_conf.h"

#include "openbl_core.h"
#include "openbl_usart_cmd.h"

#include "app_openbootloader.h"
#include "usart_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEFAULT_USART_BAUDRATE   115200U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t UsartDetected = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used USARTx instance.
  * @retval None.
  */
static void OPENBL_USART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct;

  /* Configure the USARTx IP:
     - Data width : 9 bits.
     - Stop bit   : 1 bit.
     - Parity     : Even.
  */
  USART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate            = DEFAULT_USART_BAUDRATE;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_9B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_EVEN;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;

  /* Enable the the auto baud rate feature if the USARTx instance supports it */
  if (IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(USARTx))
  {
    LL_USART_EnableAutoBaudRate(USARTx);
    LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUD_DETECT_ON_7F_FRAME);
  }
  else
  {
    LL_USART_DisableAutoBaudRate(USARTx);
  }

  if (LL_USART_Init(USARTx, &USART_InitStruct) != SUCCESS)
  {
    Error_Handler();
  }

  LL_USART_Enable(USARTx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure USARTx pins and then initialize the used USARTx instance.
  * @retval None.
  */
void OPENBL_USART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0U};

  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  USARTx_GPIO_CLK_TX_ENABLE();
  USARTx_GPIO_CLK_RX_ENABLE();

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /* USARTx pins configuration -----------------------------------------------*/
  /*      +--------------+
          |     USART1   |
  +-------+--------------+
  |  Tx   |     PA9      |
  +-------+--------------+
  |  Rx   |     P10      |
  +-------+--------------+ */

  /* Common configuration for USARTx PINs:
      Mode        : Alternate function.
      Output type : Push-Pull (Pull-Up).
      Speed       : High speed.
  */
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_ALTERNATE;

  /* Configure the Tx PIN */
  GPIO_InitStruct.Pin = USARTx_TX_PIN;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* Configure the Rx PIN */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  OPENBL_USART_Init();
}

/**
  * @brief  This function is used to De-initialize the USARTx pins and instance.
  * @retval None.
  */
void OPENBL_USART_DeInit(void)
{
  /* Only de-initialize the USARTx if it is not the current detected interface */
  if (UsartDetected == 0U)
  {
    LL_USART_Disable(USARTx);

    USARTx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on USART protocol.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_USART_ProtocolDetection(void)
{
  uint32_t auto_baudrate_flag;
  uint32_t auto_baudrate_erro_flag;

  /* Check if the USARTx is addressed */
  auto_baudrate_flag      = LL_USART_IsActiveFlag_ABR(USARTx);
  auto_baudrate_erro_flag = LL_USART_IsActiveFlag_ABRE(USARTx);

  if ((auto_baudrate_flag != 0U) && (auto_baudrate_erro_flag == 0U))
  {
    /* Read byte in order to flush the 0x7F synchronization byte */
    (void)OPENBL_USART_ReadByte();

    /* Acknowledge the host */
    OPENBL_USART_SendByte(ACK_BYTE);

    UsartDetected = 1U;
  }
  else
  {
    UsartDetected = 0U;
  }

  return UsartDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_USART_GetCommandOpcode(void)
{
  uint8_t command_opc;

  /* Get the command opcode */
  command_opc = OPENBL_USART_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_USART_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USARTx pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_USART_ReadByte(void)
{
  /* Wait until the Read Data Register Not Empty Flag is set */
  while (LL_USART_IsActiveFlag_RXNE_RXFNE(USARTx) == 0U)
  {
    OPENBL_IWDG_Refresh();
  }

  return LL_USART_ReceiveData8(USARTx);
}

/**
  * @brief  This function is used to send one byte through USARTx pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_USART_SendByte(uint8_t Byte)
{
  LL_USART_TransmitData8(USARTx, (Byte & 0xFFU));

  /* Waint until the reset of the Transmission Complete Flag */
  while (LL_USART_IsActiveFlag_TC(USARTx) == 0U)
  {
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  pSpecialCmd Pointer to the OPENBL_SpecialCmdTypeDef structure.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
void OPENBL_USART_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *pSpecialCmd)
{
  switch (pSpecialCmd->OpCode)
  {
    case SPECIAL_CMD_DEFAULT:
      if (pSpecialCmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_USART_SendByte(0x00U);
        OPENBL_USART_SendByte(0x00U);

        /* Send NULL status size */
        OPENBL_USART_SendByte(0x00U);
        OPENBL_USART_SendByte(0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      else if (pSpecialCmd->CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL status size */
        OPENBL_USART_SendByte(0x00U);
        OPENBL_USART_SendByte(0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      else
      {
        /* Nothing to do */
      }

      break;

    default:
      break;
  }
}
