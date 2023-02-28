/**
  ******************************************************************************
  * @file    fdcan_interface.c
  * @author  MCD Application Team
  * @brief   Contains FDCAN HW configuration
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
#include "openbl_fdcan_cmd.h"

#include "app_openbootloader.h"
#include "fdcan_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FDCAN_HandleTypeDef hfdcan;
static FDCAN_FilterTypeDef sFilterConfig;
static FDCAN_TxHeaderTypeDef TxHeader;
static FDCAN_RxHeaderTypeDef RxHeader;
static uint8_t FdcanDetected = 0U;

/* Exported variables --------------------------------------------------------*/
uint8_t TxData[FDCAN_RAM_BUFFER_SIZE];
uint8_t RxData[FDCAN_RAM_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FDCAN_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used FDCAN instance.
  * @retval None.
  */
static void OPENBL_FDCAN_Init(void)
{
  /*                Bit time configuration:
    Bit time parameter         | Nominal      |  Data
    ---------------------------|--------------|----------------
    fdcan_ker_ck               | 20 MHz       | 20 MHz
    Time_quantum (tq)          | 50 ns        | 50 ns
    Synchronization_segment    | 1 tq         | 1 tq
    Propagation_segment        | 23 tq        | 1 tq
    Phase_segment_1            | 8 tq         | 4 tq
    Phase_segment_2            | 8 tq         | 4 tq
    Synchronization_Jump_width | 8 tq         | 4 tq
    Bit_length                 | 40 tq = 2 us | 10 tq = 0.5 us
    Bit_rate                   | 0.5 MBit/s   | 2 MBit/s
  */

  hfdcan.Instance                  = FDCANx;
  hfdcan.Init.FrameFormat          = FDCAN_FRAME_FD_BRS;
  hfdcan.Init.Mode                 = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission   = ENABLE;
  hfdcan.Init.TransmitPause        = DISABLE;
  hfdcan.Init.ProtocolException    = ENABLE;
  hfdcan.Init.NominalPrescaler     = 0x01U;
  hfdcan.Init.NominalSyncJumpWidth = 0x10U;
  hfdcan.Init.NominalTimeSeg1      = 0x3FU;
  hfdcan.Init.NominalTimeSeg2      = 0x10U;
  hfdcan.Init.DataPrescaler        = 0x01U;
  hfdcan.Init.DataSyncJumpWidth    = 0x04U;
  hfdcan.Init.DataTimeSeg1         = 0x0FU;
  hfdcan.Init.DataTimeSeg2         = 0x04U;
  hfdcan.Init.StdFiltersNbr        = 0x01U;
  hfdcan.Init.ExtFiltersNbr        = 0x00U;
  hfdcan.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;

  if (HAL_FDCAN_Init(&hfdcan) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure Rx filter */
  sFilterConfig.IdType       = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex  = 0U;
  sFilterConfig.FilterType   = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1    = 0x111U;
  sFilterConfig.FilterID2    = 0x7FFU;

  if (HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Prepare Tx Header */
  TxHeader.Identifier          = 0x111U;
  TxHeader.IdType              = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType         = FDCAN_DATA_FRAME;
  TxHeader.DataLength          = FDCAN_DLC_BYTES_64;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch       = FDCAN_BRS_ON;
  TxHeader.FDFormat            = FDCAN_FD_CAN;
  TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker       = 0U;

  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan) != HAL_OK)
  {
    Error_Handler();
  }
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure FDCAN pins and then initialize the used FDCAN instance.
  * @retval None.
  */
void OPENBL_FDCAN_Configuration(void)
{
  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  FDCANx_GPIO_CLK_RX_ENABLE();
  FDCANx_GPIO_CLK_TX_ENABLE();

  OPENBL_FDCAN_Init();
}

/**
  * @brief  This function is used to De-initialize the FDCAN pins and instance.
  * @retval None.
  */
void OPENBL_FDCAN_DeInit(void)
{
  /* Only de-initialize the FDCAN if it is not the current detected interface */
  if (FdcanDetected == 0U)
  {
    FDCANx_FORCE_RESET();
    FDCANx_RELEASE_RESET();
    HAL_GPIO_DeInit(FDCANx_TX_GPIO_PORT, FDCANx_TX_PIN);
    HAL_GPIO_DeInit(FDCANx_RX_GPIO_PORT, FDCANx_RX_PIN);

    FDCANx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on FDCAN protocol.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_FDCAN_ProtocolDetection(void)
{
  /* check if FIFO 0 receive at least one message */
  if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) > 0U)
  {
    FdcanDetected = 1U;
  }
  else
  {
    FdcanDetected = 0U;
  }

  return FdcanDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command operation code.
  */
uint8_t OPENBL_FDCAN_GetCommandOpcode(void)
{
  uint8_t command_opc;
  HAL_StatusTypeDef status;

  /* check if FIFO 0 receive at least one message */
  while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) < 1U)
  {}

  /* Retrieve Rx messages from Rx FIFO0 */
  status = HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);

  /* Check for errors */
  if (status == HAL_ERROR)
  {
    command_opc = ERROR_COMMAND;
  }
  else
  {
    command_opc         = (uint8_t) RxHeader.Identifier;
    TxHeader.Identifier = RxHeader.Identifier;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from FDCAN pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_FDCAN_ReadByte(void)
{
  uint8_t byte;

  /* check if FIFO 0 receive at least one message */
  while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) < 1U)
  {
    OPENBL_IWDG_Refresh();
  }

  /* Retrieve Rx messages from Rx FIFO0 */
  if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &byte) != HAL_OK)
  {
    Error_Handler();
  }

  return byte;
}

/**
  * @brief  This function is used to read bytes from FDCAN pipe.
  * @param  pBuffer The data buffer where received data will be stored.
  * @param  BufferSize The number of bytes to be read and stored in the receive buffer.
  * @retval None.
  */
void OPENBL_FDCAN_ReadBytes(uint8_t *pBuffer, uint32_t BufferSize)
{
  UNUSED(BufferSize);

  /* check if FIFO 0 receive at least one message */
  while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) < 1U)
  {
    OPENBL_IWDG_Refresh();
  }

  /* Retrieve Rx messages from RX FIFO0 */
  if (HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, pBuffer) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is used to send one byte through FDCAN pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_FDCAN_SendByte(uint8_t Byte)
{
  TxHeader.DataLength = FDCAN_DLC_BYTES_1;

  while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) == 0U)
  {}

  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, &Byte) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait that the data is completely sent (sent FIFO empty) */
  while (__HAL_FDCAN_GET_FLAG(&hfdcan, FDCAN_FLAG_TX_FIFO_EMPTY) != FDCAN_FLAG_TX_FIFO_EMPTY)
  {}

  /* Clear the complete flag */
  __HAL_FDCAN_CLEAR_FLAG(&hfdcan, FDCAN_FLAG_TX_FIFO_EMPTY);
}

/**
  * @brief  This function is used to send a buffer using FDCAN.
  * @param  pBuffer The data buffer to be sent.
  * @param  BufferSize The size of the data buffer to be sent.
  * @retval None.
  */
void OPENBL_FDCAN_SendBytes(uint8_t *pBuffer, uint32_t BufferSize)
{
  TxHeader.DataLength = BufferSize;

  while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan) == 0U)
  {}

  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, pBuffer) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait that the data is completely sent (sent FIFO empty) */
  while (__HAL_FDCAN_GET_FLAG(&hfdcan, FDCAN_FLAG_TX_FIFO_EMPTY) != FDCAN_FLAG_TX_FIFO_EMPTY)
  {}

  /* Clear the complete flag */
  __HAL_FDCAN_CLEAR_FLAG(&hfdcan, FDCAN_FLAG_TX_FIFO_EMPTY);
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  pSpecialCmd Pointer to special command structure.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
void OPENBL_FDCAN_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *pSpecialCmd)
{
  switch (pSpecialCmd->OpCode)
  {
    case SPECIAL_CMD_DEFAULT:
      if (pSpecialCmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        TxData[0] = 0x0U;
        TxData[1] = 0x0U;

        /* Send NULL status size */
        TxData[2] = 0x0U;
        TxData[3] = 0x0U;

        OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_4);
        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      else if (pSpecialCmd->CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL status size */
        TxData[0] = 0x0U;
        TxData[1] = 0x0U;

        OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_2);
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
