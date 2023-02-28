/**
  ******************************************************************************
  * @file    mx_wifi_azure_rtos_conf.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_azure_rtos module
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
#ifndef MX_WIFI_AZURE_RTOS_CONF_H
#define MX_WIFI_AZURE_RTOS_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#if (defined(MX_WIFI_USE_CMSIS_OS) && (MX_WIFI_USE_CMSIS_OS != 0))
#error The NetXDuo wifi driver does not support CMSIS OS
#endif /* (MX_WIFI_USE_CMSIS_OS != 0) */

#include <stdint.h>
#include <stddef.h>

#include "tx_api.h"
#include "nx_api.h"

UINT mx_wifi_alloc_init(void);
void *mx_wifi_malloc(size_t size);
void mx_wifi_free(void *p);

#define MX_WIFI_MALLOC(size) mx_wifi_malloc(size)
#define MX_WIFI_FREE(p) mx_wifi_free(p)


typedef NX_PACKET mx_buf_t;

NX_PACKET *mx_net_buffer_alloc(uint32_t n);
void mx_net_buffer_free(NX_PACKET *nx_packet);

#define MX_NET_BUFFER_ALLOC(len)                        mx_net_buffer_alloc((len))

#define MX_NET_BUFFER_FREE(nx_packet)                   mx_net_buffer_free((nx_packet))

#define MX_NET_BUFFER_PAYLOAD(packet_ptr)               (packet_ptr)->nx_packet_prepend_ptr

#define MX_NET_BUFFER_SET_PAYLOAD_SIZE(packet_ptr, n)   \
  do {                                                  \
    (packet_ptr)->nx_packet_length = (n);               \
    (packet_ptr)->nx_packet_append_ptr =                \
    (packet_ptr)->nx_packet_prepend_ptr + (n);          \
  } while (0)

#define MX_NET_BUFFER_GET_PAYLOAD_SIZE(packet_ptr)      (packet_ptr)->nx_packet_length

#define MX_NET_BUFFER_HIDE_HEADER(packet_ptr, n)        \
  do {                                                  \
    (packet_ptr)->nx_packet_prepend_ptr += (n);         \
    (packet_ptr)->nx_packet_length -= (n);              \
  } while (0)

#define DELAY_MS(n)                                             \
  do {                                                          \
    if ((n) == 1) {                                             \
      tx_thread_relinquish();                                   \
    } else {                                                    \
      tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * (n) / 1000);  \
    }                                                           \
  } while(0)

#define MX_ASSERT(A)                                    do {} while(!(A))

#define LOCK_DECLARE(A)                                 TX_MUTEX (A)
#define LOCK_INIT(A)                                    tx_mutex_create(&A, #A, TX_NO_INHERIT)
#define LOCK_DEINIT(A)                                  tx_mutex_delete(&A)
#define LOCK(A)                                         tx_mutex_get(&A, TX_WAIT_FOREVER)
#define UNLOCK(A)                                       tx_mutex_put(&A)

#define SEM_DECLARE(A)                                  TX_SEMAPHORE (A)
#define SEM_INIT(A, COUNT)                              tx_semaphore_create(&A, #A, 0)
#define SEM_DEINIT(A)                                   tx_semaphore_delete(&A)
#define SEM_SIGNAL(A)                                   tx_semaphore_put(&A)
#define SEM_WAIT(A, TIMEOUT, IDLE_FUNC)                 tx_semaphore_get(&A, (TIMEOUT))

UINT mx_wifi_thread_init(TX_THREAD *thread_ptr, CHAR *name_ptr,
                         VOID (*entry_function)(ULONG id), ULONG entry_input,
                         ULONG stack_size, UINT priority);
UINT mx_wifi_thread_deinit(TX_THREAD *thread_ptr);
void mx_wifi_thread_terminate(void);

#define THREAD_CONTEXT_TYPE                             ULONG

#define OSPRIORITYNORMAL                                11
#define OSPRIORITYABOVENORMAL                           9
#define OSPRIORITYREALTIME                              8

#define THREAD_DECLARE(A)                               TX_THREAD (A)

#define THREAD_INIT(A, THREAD_FUNC, THREAD_CONTEXT, STACKSIZE, PRIORITY)     \
  mx_wifi_thread_init(&A, #A,                                                \
                      (VOID (*)(ULONG)) THREAD_FUNC, (ULONG) THREAD_CONTEXT, \
                      STACKSIZE, PRIORITY)

#define THREAD_DEINIT(A)                                mx_wifi_thread_deinit(&A)
#define THREAD_TERMINATE()                              mx_wifi_thread_terminate()

UINT mx_wifi_fifo_init(TX_QUEUE *queue_ptr, char *name_ptr, ULONG size);
UINT mx_wifi_fifo_deinit(TX_QUEUE *queue_ptr);
UINT mx_wifi_fifo_push(TX_QUEUE *queue_ptr, void *source_ptr, ULONG wait_option);
void *mx_wifi_fifo_pop(TX_QUEUE *queue_ptr, ULONG wait_option);

#define FIFO_DECLARE(QUEUE)                             TX_QUEUE QUEUE
#define FIFO_INIT(QUEUE, QSIZE)                         mx_wifi_fifo_init(&QUEUE, #QUEUE, (QSIZE))
#define FIFO_DEINIT(QUEUE)                              mx_wifi_fifo_deinit(&QUEUE)
#define FIFO_PUSH(QUEUE, VALUE, TIMEOUT, IDLE_FUNC)     mx_wifi_fifo_push(&QUEUE, &VALUE, (TIMEOUT))
#define FIFO_POP(QUEUE, TIMEOUT, IDLE_FUNC)             mx_wifi_fifo_pop(&QUEUE, (TIMEOUT))

#define WAIT_FOREVER                                    TX_WAIT_FOREVER
#define SEM_OK                                          TX_SUCCESS
#define THREAD_OK                                       TX_SUCCESS
#define FIFO_OK                                         TX_SUCCESS

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_AZURE_RTOS_CONF_H */
