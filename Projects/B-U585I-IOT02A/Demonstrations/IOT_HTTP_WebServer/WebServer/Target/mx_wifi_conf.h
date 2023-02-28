/**
  **********************************************************************************************************************
  * @file    mx_wifi_conf.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_conf module
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef MX_WIFI_CONF_H
#define MX_WIFI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include "main.h" /* Inherit some declarations from the current platform. */


#define MX_WIFI_USE_SPI                             (1)

#define DMA_ON_USE                                  (0)

#define MX_WIFI_NETWORK_BYPASS_MODE                 (0)

#define MX_WIFI_TX_BUFFER_NO_COPY                   (1)

#define MX_WIFI_USE_CMSIS_OS                        (0)

#define WIFI_SSID                                   "YOUR_SSID"
#define WIFI_PASSWORD                               "YOUR_PASSWORD"

/* DEBUG LOG */
/* #define MX_WIFI_API_DEBUG  */
/* #define MX_WIFI_IPC_DEBUG  */
/* #define MX_WIFI_HCI_DEBUG  */
/* #define MX_WIFI_IO_DEBUG   */

#include "mx_wifi_bare_os.h"

#define MX_WIFI_PRODUCT_NAME                        ("MXCHIP-WIFI")

#define MX_WIFI_PRODUCT_ID                          ("EMW3080B")

#ifndef MX_WIFI_UART_BAUDRATE
#define MX_WIFI_UART_BAUDRATE                       (115200*2)
#endif /* MX_WIFI_UART_BAUDRATE */

#ifndef MX_WIFI_MTU_SIZE
#define MX_WIFI_MTU_SIZE                            (1500)
#endif /* MX_WIFI_MTU_SIZE */

#define MX_WIFI_BUFFER_SIZE                         (2500)  /* Bigger buffer size (needed with some other use cases) */

#define MX_WIFI_IPC_PAYLOAD_SIZE                    (MX_WIFI_BUFFER_SIZE - 6)  /* MX_WIFI_BUFFER_SIZE - MIPC_HEADER_SIZE */

#define MX_WIFI_SOCKET_DATA_SIZE                    ((MX_WIFI_IPC_PAYLOAD_SIZE) - 12) /* MX_WIFI_IPC_PAYLOAD_SIZE - socket_api_params_header */

#ifndef MX_WIFI_CMD_TIMEOUT
#define MX_WIFI_CMD_TIMEOUT                         (0xFFFFFFFF)  /* default 10s timeout */
#endif /* MX_WIFI_CMD_TIMEOUT */

#define MX_WIFI_MAX_SOCKET_NBR                      (8)

#define MX_WIFI_MAX_DETECTED_AP                     (10)

#define MX_WIFI_MAX_SSID_NAME_SIZE                  (32)

#define MX_WIFI_MAX_PSWD_NAME_SIZE                  (64)

#define MX_WIFI_PRODUCT_NAME_SIZE                   (32)

#define MX_WIFI_PRODUCT_ID_SIZE                     (32)

#define MX_WIFI_FW_REV_SIZE                         (24)

/* Maximum number of RX buffer that can be queued by Hardware interface (SPI/UART)                         */
/* This is used to size internal queue, and avoid to block the IP thread if it can still push some buffers */
/* Impact on Memory foot print is weak, one single void* per place in the queue                            */
#ifndef MX_WIFI_MAX_RX_BUFFER_COUNT
#define MX_WIFI_MAX_RX_BUFFER_COUNT                 (2)
#endif /* MX_WIFI_MAX_RX_BUFFER_COUNT */


/* Maximum number of TX buffer that can be queued by IP stack (LwIP or Netx) without blocking the calling thread */
/* This is used to size internal queue, and avoid to block the IP thread if it can still push some buffers       */
/* Impact on Memory foot print is one single void* per place in the queue, but it may lead to over allocation    */
/* TCP/IP stack (LwIP  for instance )                                                                            */
#ifndef MX_WIFI_MAX_TX_BUFFER_COUNT
#define MX_WIFI_MAX_TX_BUFFER_COUNT                 (4)
#endif /* MX_WIFI_MAX_TX_BUFFER_COUNT */


/**
  * For the TX buffer, by default no-copy feature is enabled, meaning that
  * the IP buffer are used in the whole process and should come with
  * available room in front of payload to accommodate transport header buffer.
  * This is managed in interface between driver and IP stack.
  */

/**
  * For LwIP "PBUF_LINK_ENCAPSULATION_HLEN" must be defined as > MX_WIFI_MIN_TX_HEADER_SIZE,
  * see net_mx_wifi/c file for implementation.
  */
#define MX_WIFI_MIN_TX_HEADER_SIZE                  (28)


#define MX_STAT_ON                                  (0)

#define MX_STAT_INIT()
#define MX_STAT(A)
#define MX_STAT_LOG()
#define MX_STAT_DECLARE()

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_CONF_H */
