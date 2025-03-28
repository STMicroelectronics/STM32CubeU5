/**
  ******************************************************************************
  * @file    ymodem.c
  * @author  MCD Application Team
  * @brief   Ymodem module.
  *          This file provides set of firmware functions to manage Ymodem
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

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

#include "common.h"
#include "ymodem.h"
#include "string.h"
#include "main.h"

/* Private const -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* @note ATTENTION - please keep this variable 32bit aligned */
static uint8_t m_aPacketData[PACKET_1K_SIZE + PACKET_DATA_INDEX +
                             PACKET_TRAILER_SIZE]; /*!<Array used to store Packet Data*/
uint8_t m_aFileName[FILE_NAME_LENGTH + 1U]; /*!< Array used to store File Name data */
static CRC_HandleTypeDef CrcHandle; /*!<CRC handle*/

/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef ReceivePacket(uint8_t *pData, uint32_t *puLength, uint32_t uTimeout);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Receive a packet from sender
  * @param  pData
  * @param  puLength
  *     0: end of transmission
  *     2: abort by sender
  *    >0: packet length
  * @param  uTimeout
  * @retval HAL_OK: normally return
  *         HAL_BUSY: abort by user
  */
static HAL_StatusTypeDef ReceivePacket(uint8_t *pData, uint32_t *puLength, uint32_t uTimeout)
{
  uint32_t crc;
  uint32_t packet_size = 0U;
  HAL_StatusTypeDef status;
  uint8_t char1;
  uint8_t char2;
  uint8_t char3;

  *puLength = 0U;

  /* If the SecureBoot configured the IWDG, UserApp must reload IWDG counter with
   *value defined in the reload register */
  status = (HAL_StatusTypeDef)COM_Receive_Y(&char1, 1, uTimeout);

  if (status == HAL_OK)
  {
    switch (char1)
    {
      case SOH:
        packet_size = PACKET_SIZE;
        break;
      case STX:
        packet_size = PACKET_1K_SIZE;
        break;
      case EOT:
        packet_size = 0;
        break;
      case CA:
        if ((COM_Receive_Y(&char1, 1U, uTimeout) == HAL_OK) && (char1 == CA))
        {
          packet_size = 2U;
        }
        else
        {
          status = HAL_ERROR;
        }
        break;
      case ABORT1:
      case ABORT2:
        status = HAL_BUSY;
        break;
      case RB:
        if ((COM_Receive_Y(&char2, 1U, uTimeout) == HAL_OK) &&                /* Ymodem startup sequence : rb ==> 0x72 + 0x62 + 0x0D */
            (COM_Receive_Y(&char3, 1U, uTimeout) == HAL_OK) &&
            (char2 == 0x62) &&
            (char3 == 0xd))
            {
                packet_size = 3U;
                break;
            }
      default:
        status = HAL_ERROR;
        break;
    }
    *pData = char1;

    if (packet_size >= PACKET_SIZE)
    {
      status = COM_Receive_Y(&pData[PACKET_NUMBER_INDEX], packet_size + PACKET_OVERHEAD_SIZE, uTimeout);

      /* Simple packet sanity check */
      if (status == HAL_OK)
      {
        if (pData[PACKET_NUMBER_INDEX] != ((pData[PACKET_CNUMBER_INDEX]) ^ NEGATIVE_BYTE))
        {
          packet_size = 0U;
          status = HAL_ERROR;
        }
        else
        {
          /* Check packet CRC */
          crc = pData[ packet_size + PACKET_DATA_INDEX ] << 8U;
          crc += pData[ packet_size + PACKET_DATA_INDEX + 1U ];
          if (HAL_CRC_Calculate(&CrcHandle, (uint32_t *)&pData[PACKET_DATA_INDEX], packet_size) != crc)
          {
            packet_size = 0U;
            status = HAL_ERROR;
          }
        }
      }
      else
      {
        packet_size = 0U;
      }
    }
  }
  *puLength = packet_size;
  return status;
}


/**
  * @brief  Init of Ymodem module.
  * @param None.
  * @retval None.
  */
void Ymodem_Init(void)
{
  __HAL_RCC_CRC_CLK_ENABLE();

  /*-1- Configure the CRC peripheral */
  CrcHandle.Instance = CRC;

  /* The CRC-16-CCIT polynomial is used */
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
  CrcHandle.Init.GeneratingPolynomial    = 0x1021U;
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B;

  /* The zero init value is used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
  CrcHandle.Init.InitValue               = 0U;

  /* The input data are not inverted */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;

  /* The output data are not inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;

  /* The input data are 32-bit long words */
  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }
  if (COM_Y_On(CA) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @brief  Receive a file using the ymodem protocol with CRC16.
  * @param  puSize The uSize of the file.
  * @param  uFlashDestination where the file has to be downloaded.
  * @retval COM_StatusTypeDef result of reception/programming
  */
COM_StatusTypeDef Ymodem_Receive(uint32_t *puSize, uint32_t uFlashDestination)
{
  uint32_t i, packet_length, session_done = 0U, file_done, errors = 0U, session_begin = 0U;
  uint32_t eot = 0U;
  uint32_t ramsource, filesize;
  uint8_t *file_ptr;
  uint8_t file_size[FILE_SIZE_LENGTH + 1U], tmp;
  uint32_t packets_received;
  COM_StatusTypeDef e_result = COM_OK;
  uint32_t cause = 0;
  while ((session_done == 0U) && (e_result == COM_OK))
  {
    packets_received = 0U;
    file_done = 0U;
    while ((file_done == 0U) && (e_result == COM_OK))
    {
      switch (ReceivePacket(m_aPacketData, &packet_length, DOWNLOAD_TIMEOUT))
      {
        case HAL_OK:
          errors = 0U;
          switch (packet_length)
          {
            case 3U:
              /* Startup sequence */
              break;
            case 2U:
              /* Abort by sender */
              Serial_PutByte(ACK);
              e_result = COM_ABORT;
              break;
            case 0U:
              /* End of transmission */
              if (!eot)
              {
                    Serial_PutByte(NAK);
                    eot = 1;
              }
              else
              {
                    Serial_PutByte(ACK);
                    *puSize = filesize;
                    file_done = 1U;
              }
              break;
            default:
              /* Normal packet */
              if (m_aPacketData[PACKET_NUMBER_INDEX] != (packets_received & 0xff))
              {
                /*             Serial_PutByte(NAK);*/
              }
              else
              {
                if (packets_received == 0U)
                {
                  /* File name packet */
                  if (m_aPacketData[PACKET_DATA_INDEX] != 0U)
                  {
                    /* File name extraction */
                    i = 0U;
                    file_ptr = m_aPacketData + PACKET_DATA_INDEX;
                    while ((*file_ptr != 0U) && (i < FILE_NAME_LENGTH))
                    {
                      m_aFileName[i++] = *file_ptr++;
                    }

                    /* File size extraction */
                    m_aFileName[i++] = '\0';
                    i = 0U;
                    file_ptr ++;
                    while ((*file_ptr != ' ') && (i < FILE_SIZE_LENGTH))
                    {
                      file_size[i++] = *file_ptr++;
                    }
                    file_size[i++] = '\0';
                    Str2Int(file_size, &filesize);
                    if ((uint32_t)filesize > *puSize)
                    {
                      *puSize = 0;
                      tmp = CA;
                      e_result = COM_ABORT;
                      cause = 1;
                    }

                    /* Header packet received callback call*/
                    if ((*puSize) && (Ymodem_HeaderPktRxCpltCallback(uFlashDestination, (uint32_t) filesize) == HAL_OK))
                    {

                      Serial_PutByte(ACK);
                      COM_Flush();
                      Serial_PutByte(CRC16);
                    }
                    else
                    {
                      /* End session */
                      COM_Transmit_Y(&tmp, 1U, NAK_TIMEOUT);
                      COM_Transmit_Y(&tmp, 1U, NAK_TIMEOUT);
                      cause = 2;
                      break;
                    }

                  }
                  /* File header packet is empty, end session */
                  else
                  {
                    Serial_PutByte(ACK);
                    file_done = 1;
                    session_done = 1;
                    cause = 3;
                    break;
                  }
                }
                else /* Data packet */
                {
                  ramsource = (uint32_t) & m_aPacketData[PACKET_DATA_INDEX];

                  /* Data packet received callback call*/
                  if ((*puSize) && (Ymodem_DataPktRxCpltCallback((uint8_t *)
                                                                 ramsource, uFlashDestination, (uint32_t) packet_length) == HAL_OK))
                  {
                    uFlashDestination += (packet_length);
                    Serial_PutByte(ACK);
                  }
                  else /* An error occurred while writing to Flash memory */
                  {
                    /* End session */
                    COM_Transmit_Y(&tmp, 1U, NAK_TIMEOUT);
                    COM_Transmit_Y(&tmp, 1U, NAK_TIMEOUT);
                    cause = 4;
                  }
                }
                packets_received ++;
                session_begin = 1U;
              }
              break;
          }
          break;
        case HAL_BUSY: /* Abort actually */
          Serial_PutByte(CA);
          Serial_PutByte(CA);
          e_result = COM_ABORT;
          cause  = 5;
          break;
        default:
          if (session_begin > 0U)
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)
          {
            /* Abort communication */
            Serial_PutByte(CA);
            Serial_PutByte(CA);
          }
          else
          {
            Serial_PutByte(CRC16); /* Ask for a packet */
          }
          break;
      }
    }
  }
  COM_Y_Off();
#if defined(__ARMCC_VERSION)
  printf("e_result = %x , %u\n", e_result, cause);
#else
  printf("e_result = %x , %lu\n", e_result, cause);
#endif /* __ARMCC_VERSION */
  return e_result;
}

/**
  * @}
  */

/**
  * @}
  */
