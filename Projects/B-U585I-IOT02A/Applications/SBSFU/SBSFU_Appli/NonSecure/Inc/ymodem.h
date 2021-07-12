/**
  ******************************************************************************
  * @file    ymodem.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Ymodem functionalities.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef YMODEM_H_
#define YMODEM_H_

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef enum
{
  COM_OK       = 0x00U,  /*!< OK */
  COM_ERROR    = 0x01U,  /*!< Error */
  COM_ABORT    = 0x02U,  /*!< Abort */
  COM_TIMEOUT  = 0x03U,  /*!< Timeout */
  COM_DATA     = 0x04U,  /*!< Data */
  COM_LIMIT    = 0x05U   /*!< Limit*/
} COM_StatusTypeDef;     /*!< Comm status structures definition */


/* Exported constants --------------------------------------------------------*/
/* Packet structure defines */
#define PACKET_HEADER_SIZE      ((uint32_t)3U)    /*!<Header Size*/
#define PACKET_DATA_INDEX       ((uint32_t)4U)    /*!<Data Index*/
#define PACKET_START_INDEX      ((uint32_t)1U)    /*!<Start Index*/
#define PACKET_NUMBER_INDEX     ((uint32_t)2U)    /*!<Packet Number Index*/
#define PACKET_CNUMBER_INDEX    ((uint32_t)3U)    /*!<Cnumber Index*/
#define PACKET_TRAILER_SIZE     ((uint32_t)2U)    /*!<Trailer Size*/
#define PACKET_OVERHEAD_SIZE    (PACKET_HEADER_SIZE + PACKET_TRAILER_SIZE - 1U) /*!<Overhead Size*/
#define PACKET_SIZE             ((uint32_t)128U)  /*!<Packet Size*/
#define PACKET_1K_SIZE          ((uint32_t)1024U) /*!<Packet 1K Size*/

/* /-------- Packet in IAP memory ------------------------------------------\
 * | 0      |  1    |  2     |  3   |  4      | ... | n+4     | n+5  | n+6  |
 * |------------------------------------------------------------------------|
 * | unused | start | number | !num | data[0] | ... | data[n] | crc0 | crc1 |
 * \------------------------------------------------------------------------/
 * the first byte is left unused for memory alignment reasons                 */

#define FILE_NAME_LENGTH        ((uint32_t)64U)  /*!< File name length*/
#define FILE_SIZE_LENGTH        ((uint32_t)16U)  /*!< File size length*/

#define SOH                     ((uint8_t)0x01U)  /*!< start of 128-byte data packet */
#define STX                     ((uint8_t)0x02U)  /*!< start of 1024-byte data packet */
#define EOT                     ((uint8_t)0x04U)  /*!< end of transmission */
#define ACK                     ((uint8_t)0x06U)  /*!< acknowledge */
#define NAK                     ((uint8_t)0x15U)  /*!< negative acknowledge */
#define CA                      ((uint8_t)0x18U)  /*!< two of these in succession aborts transfer */
#define CRC16                   ((uint8_t)0x43U)  /*!< 'C' == 0x43, request 16-bit CRC */
#define RB                      ((uint8_t)0x72U)  /*!< Startup sequence */
#define NEGATIVE_BYTE           ((uint8_t)0xFFU)  /*!< Negative Byte*/

#define ABORT1                  ((uint8_t)0x41U)  /* 'A' == 0x41, abort by user */
#define ABORT2                  ((uint8_t)0x61U)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             ((uint32_t)0x100000U)  /*!< NAK Timeout*/
#define DOWNLOAD_TIMEOUT        ((uint32_t)2000U) /* One second retry delay */
#define MAX_ERRORS              ((uint32_t)5U)    /*!< Maximum number of retry*/

/* Exported functions ------------------------------------------------------- */
void Ymodem_Init(void);
COM_StatusTypeDef Ymodem_Receive(uint32_t *puSize, uint32_t uFlashDestination);

/**
  * @}
  */

/**
  * @}
  */

#endif  /* YMODEM_H_ */
