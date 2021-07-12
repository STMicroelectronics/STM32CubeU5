/**
  **********************************************************************************************************************
  * @file    net_conf.h
  * @author  MCD Application Team
  * @brief   Configures the network socket APIs.
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
#ifndef NET_CONF_H
#define NET_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* when using LWIP , size of hostname */
#define NET_IP_HOSTNAME_MAX_LEN        (32U)

#define NET_USE_IPV6                   (0U)

/* MbedTLS configuration */
#ifdef NET_MBEDTLS_HOST_SUPPORT

#if !defined NET_MBEDTLS_DEBUG_LEVEL
#define NET_MBEDTLS_DEBUG_LEVEL        (1U)
#endif /*   NET_MBEDTLS_DEBUG_LEVEL */

#if !defined NET_MBEDTLS_CONNECT_TIMEOUT
#define NET_MBEDTLS_CONNECT_TIMEOUT    (10000U)
#endif /* NET_MBEDTLS_CONNECT_TIMEOUT */

#if !defined(MBEDTLS_CONFIG_FILE)
#define MBEDTLS_CONFIG_FILE "mbedtls/config.h"
#endif /* MBEDTLS_CONFIG_FILE */
#endif /* NET_MBEDTLS_HOST_SUPPORT */

#if !defined(NET_MAX_SOCKETS_NBR)
#define NET_MAX_SOCKETS_NBR            (5U)
#endif /* NET_MAX_SOCKETS_NBR */

#define NET_IF_NAME_LEN                (128U)
#define NET_DEVICE_NAME_LEN            (64U)
#define NET_DEVICE_ID_LEN              (64U)
#define NET_DEVICE_VER_LEN             (64U)
#define NET_SOCK_DEFAULT_RECEIVE_TO    (60000U)
#define NET_SOCK_DEFAULT_SEND_TO       (60000U)
#define NET_UDP_MAX_SEND_BLOCK_TO      (1024U)
#define NET_USE_DEFAULT_INTERFACE      (1U)
#define NET_RTOS_SUSPEND
#define NET_RTOS_RESUME
#define NET_DBG_INFO(...)
#define NET_DBG_ERROR(...)
#define NET_DBG_PRINT(...)
#define NET_ASSERT(test,...)
#define NET_PRINT(...)
#define NET_PRINT_WO_CR(...)
#define NET_WARNING(...)

#ifndef NET_PERF_MAXTHREAD
#define NET_PERF_MAXTHREAD (10U)
#endif /* NET_PERF_MAXTHREAD  */



#ifdef __cplusplus
}
#endif

#endif /* NET_CONF */
