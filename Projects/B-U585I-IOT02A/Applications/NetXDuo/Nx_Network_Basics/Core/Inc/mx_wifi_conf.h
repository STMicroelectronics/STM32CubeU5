/**
  ******************************************************************************
  * @file    mx_wifi_conf.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi module
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
#ifndef MX_WIFI_CONF_H
#define MX_WIFI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include "main.h"  /* Inherit some declarations from the current platform. */
#include "mx_wifi_azure_rtos_conf.h"
#include "nx_driver_emw3080.h"

/* Exported macros -----------------------------------------------------------*/

#define MX_WIFI_USE_SPI                             (1)

#define DMA_ON_USE                                  (1)

#define MX_WIFI_NETWORK_BYPASS_MODE                 (1)

#define MX_WIFI_BYPASS_DEBUG                        (0)

#define MX_WIFI_TX_BUFFER_NO_COPY                   (1)

#define MX_WIFI_USE_CMSIS_OS                        (0)

/* Edit the defines below to suit the WiFi connection. */
/* Access point connection parameters */
#define WIFI_SSID                                   "YOUR_SSID"
#define WIFI_PASSWORD                               "YOUR_PASSWORD"

/* DEBUG LOG */
/* #define MX_WIFI_API_DEBUG  */
/* #define MX_WIFI_IPC_DEBUG  */
/* #define MX_WIFI_HCI_DEBUG  */
/* #define MX_WIFI_SLIP_DEBUG */
/* #define MX_WIFI_IO_DEBUG   */

#define MX_WIFI_PRODUCT_NAME                        ("MXCHIP-WIFI")

#define MX_WIFI_PRODUCT_ID                          ("EMW3080B")

#define MX_WIFI_BUFFER_SIZE                         (NX_DRIVER_PACKET_SIZE - 2)

#define MX_WIFI_IPC_PAYLOAD_SIZE                    (MX_WIFI_BUFFER_SIZE - 6)  /* MX_WIFI_BUFFER_SIZE - MIPC_HEADER_SIZE */

#define MX_WIFI_CMD_TIMEOUT                         (10000)

#define MX_WIFI_MAX_DETECTED_AP                     (10)

#define MX_WIFI_MAX_SSID_NAME_SIZE                  (32)

#define MX_WIFI_MAX_PSWD_NAME_SIZE                  (64)

#define MX_WIFI_PRODUCT_NAME_SIZE                   (32)

#define MX_WIFI_PRODUCT_ID_SIZE                     (32)

#define MX_WIFI_FW_REV_SIZE                         (24)

#define MX_WIFI_SPI_THREAD_PRIORITY                 (OSPRIORITYREALTIME)

#define MX_WIFI_SPI_THREAD_STACK_SIZE               (256 * 4)

#define MX_WIFI_RECEIVED_THREAD_PRIORITY            (OSPRIORITYABOVENORMAL)

#define MX_WIFI_RECEIVED_THREAD_STACK_SIZE          (384 * 4)

#define MX_WIFI_TRANSMIT_THREAD_PRIORITY            (OSPRIORITYABOVENORMAL)

#define MX_WIFI_TRANSMIT_THREAD_STACK_SIZE          (256 * 4)

/* Maximum number of RX buffer that can be queued by Hardware interface (SPI/UART)                         */
/* This is used to size internal queue, and avoid to block the IP thread if it can still push some buffers */
/* Impact on memory foot print is one single void* per place in the queue, but it may lead to              */
/* over allocation TCP/IP stack.                                                                           */
#define MX_WIFI_MAX_RX_BUFFER_COUNT                 (16)

/* Maximum number of TX buffer that can be queued by IP stack without blocking the calling thread          */
/* This is used to size internal queue, and avoid to block the IP thread if it can still push some buffers */
/* Impact on memory foot print is one single void* per place in the queue, but it may lead to              */
/* over allocation TCP/IP stack.                                                                           */
#define MX_WIFI_MAX_TX_BUFFER_COUNT                 (8)

#define MX_STAT_ON                                  (0)

#if (MX_STAT_ON == 1)
typedef struct
{
  uint32_t alloc;
  uint32_t free;
  uint32_t cmd_get_answer;
  uint32_t callback;
  uint32_t in_fifo;
  uint32_t out_fifo;
} mx_stat_t;

extern mx_stat_t mx_stat;

#define MX_STAT_LOG()                                                                                                                  \
  (void) printf("\n Number of allocated buffer for Rx and command answer %" PRIu32 "\n", mx_stat.alloc);                               \
  (void) printf(" Number of freed buffer %" PRIu32 "\n", mx_stat.free);                                                                \
  (void) printf(" Number of command answer %" PRIu32 ", callback %" PRIu32 ", sum of both %" PRIu32 " (should match alloc && free)\n", \
                mx_stat.cmd_get_answer, mx_stat.callback, mx_stat.cmd_get_answer + mx_stat.callback);                                  \
  (void) printf(" Number of posted answer (callback + cmd answer) %" PRIu32 ", processed answer %" PRIu32 "\n\n",                      \
                mx_stat.in_fifo, mx_stat.out_fifo);

#define MX_STAT_INIT()        (void) memset((void*)&mx_stat, 0, sizeof(mx_stat))
#define MX_STAT(A)            mx_stat.A++
#define MX_STAT_DECLARE()     mx_stat_t mx_stat

#else /* MX_STAT_ON */
#define MX_STAT_INIT()
#define MX_STAT(A)
#define MX_STAT_LOG()
#define MX_STAT_DECLARE()
#endif /* MX_STAT_ON */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_CONF_H */
