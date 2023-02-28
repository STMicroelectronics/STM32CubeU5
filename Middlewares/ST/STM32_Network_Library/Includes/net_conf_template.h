/**
  ******************************************************************************
  * @file    net_conf_template.h
  * @author  MCD Application Team
  * @brief   Configures the network socket APIs.
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
#ifndef NET_CONF_TEMPLATE_H
#define NET_CONF_TEMPLATE_H

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Disable MISRA rule to enable doxygen comment, A section of code appear to have been commented out */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "net_types.h"

/* Please uncomment if you want socket address definition from LWIP include file rather than local one */
/* This is recommended if network interface uses LWIP to save some code size. This is required if      */
/* project uses IPv6 */

/* #define NET_USE_LWIP_DEFINITIONS  */

/* Experimental: Please uncomment if you want to use only control part of network library              */
/* net_socket APIs are directly redefined to LWIP, NET_MBEDTLS_HOST_SUPPORT is not supported with      */
/* this mode, dedicated to save memory (4K code)                                                       */
/* #define NET_BYPASS_NET_SOCKET */


/* Please uncomment if secure sockets have to be supported and is implemented thanks to MBEDTLS running on MCU */
/* #define NET_MBEDTLS_HOST_SUPPORT */

/* Please uncomment if device supports internally Secure TCP connection */
/* #define NET_MBEDTLS_DEVICE_SUPPORT */

#ifdef NET_USE_RTOS
#include "cmsis_os.h"
#endif /* NET_USE_RTOS */

/* Please uncomment if DHCP server is required and not natively supported by network interface */
/* #define NET_DHCP_SERVER_HOST_SUPPORT*/

/* when using LWIP, size of hostname */
#define NET_IP_HOSTNAME_MAX_LEN        32

#ifndef NET_USE_IPV6
#define NET_USE_IPV6    0
#endif /* NET_USE_IPV6 */

#if NET_USE_IPV6 && !defined(NET_USE_LWIP_DEFINITIONS)
#error "NET IPV6 required to define NET_USE_LWIP_DEFINITIONS"
#endif /* NET_USE_IPV6 */


/* MbedTLS configuration */
#ifdef NET_MBEDTLS_HOST_SUPPORT

#if !defined NET_MBEDTLS_DEBUG_LEVEL
#define NET_MBEDTLS_DEBUG_LEVEL 1
#endif /* NET_MBEDTLS_DEBUG_LEVEL */

#if !defined NET_MBEDTLS_CONNECT_TIMEOUT
#define NET_MBEDTLS_CONNECT_TIMEOUT     10000U
#endif /* NET_MBEDTLS_CONNECT_TIMEOUT */

#if !defined(MBEDTLS_CONFIG_FILE)
#define MBEDTLS_CONFIG_FILE "mbedtls/config.h"
#endif /* MBEDTLS_CONFIG_FILE */

#endif /* NET_MBEDTLS_HOST_SUPPORT */

#if !defined(NET_MAX_SOCKETS_NBR)
#define NET_MAX_SOCKETS_NBR            5
#endif /* NET_MAX_SOCKETS_NBR */

#define NET_IF_NAME_LEN                128
#define NET_DEVICE_NAME_LEN            64
#define NET_DEVICE_ID_LEN              64
#define NET_DEVICE_VER_LEN             64


#define NET_SOCK_DEFAULT_RECEIVE_TO    60000
#define NET_SOCK_DEFAULT_SEND_TO       60000
#define NET_UDP_MAX_SEND_BLOCK_TO      1024

#if !defined(NET_USE_DEFAULT_INTERFACE)
#define NET_USE_DEFAULT_INTERFACE      1
#endif /* NET_USE_DEFAULT_INTERFACE */

#ifdef NET_USE_RTOS

#if (osCMSIS < 0x20000U)
#define NET_RTOS_SUSPEND  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { (void) vTaskSuspendAll(); }
#define NET_RTOS_RESUME   if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { (void) xTaskResumeAll(); }
#else
#define NET_RTOS_SUSPEND    (void) osKernelLock()
#define NET_RTOS_RESUME     (void) osKernelUnlock()
#endif /* osCMSIS */

#else
#define NET_RTOS_SUSPEND
#define NET_RTOS_RESUME
#endif /* NET_USE_RTOS */

#if !defined(NET_DBG_INFO)
#define NET_DBG_INFO(...)
/*
#define NET_DBG_INFO(...)  do { \
                                (void) printf(__VA_ARGS__); \
                              } while (0)
*/
#endif /* NET_DBG_INFO */

#if !defined(NET_DBG_ERROR)
#define NET_DBG_ERROR(...)  do { \
                                 (void)printf("\nERROR: %s:%d ",__FILE__,__LINE__) ;\
                                 (void)printf(__VA_ARGS__);\
                                 (void)printf("\n"); \
                               } while (false)
#endif /* NET_DBG_ERROR */
#if !defined(NET_DBG_PRINT)
#define NET_DBG_PRINT(...)  do { \
                                 (void)printf("%s:%d ",__FILE__,__LINE__) ;\
                                 (void)printf(__VA_ARGS__);\
                                 (void)printf("\n"); \
                               } while (false)
#endif /* NET_DBG_PRINT */

#if !defined(NET_ASSERT)
#define NET_ASSERT(test,...)  do { if (!(test)) {\
                                   (void) printf("Assert Failed %s %d :",__FILE__,__LINE__);\
                                   (void) printf(__VA_ARGS__);\
                                   while(true) {}; }\
                                 } while (false)
#endif /* NET_ASSERT */

#if !defined(NET_PRINT)
#define NET_PRINT(...)      do { \
                                 (void) printf(__VA_ARGS__);\
                                 (void) printf("\n"); \
                               } while (false)
#endif /* NET_PRINT */

#if !defined(NET_PRINT_WO_CR)
#define NET_PRINT_WO_CR(...)   do { \
                                    (void) printf(__VA_ARGS__);\
                                  } while (false)
#endif /* NET_PRINT_WO_CR */

#if !defined(NET_WARNING)
#define NET_WARNING(...)    do { \
                                 (void) printf("Warning %s:%d ",__FILE__,__LINE__) ;\
                                 (void) printf(__VA_ARGS__);\
                                 (void) printf("\n"); \
                               } while (false)
#endif /* NET_WARNING */



#ifndef NET_PERF_MAXTHREAD
#define NET_PERF_MAXTHREAD      7U
#endif /* NET_PERF_MAXTHREAD  */

#ifndef NET_TASK_HISTORY_SIZE
#define NET_TASK_HISTORY_SIZE   0U
#endif /* NET_PERF_MAXTHREAD  */


/* The random provider to implement on the application side. */
int mbedtls_rng_raw(void *data, uchar_t *output, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_CONF_TEMPLATE_H */
