/**
  ******************************************************************************
  * @file    net_mem.h
  * @author  MCD Application Team
  * @brief   Memory allocator functions
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
#ifndef NET_MEM_H
#define NET_MEM_H

/* Includes ------------------------------------------------------------------*/
#include "net_conf.h"
#include "net_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NET_ALLOC_DEBUG

#define NET_ALLOC_OVERWRITE_CHECK_OFFSET                (4)

#if !defined NET_ALLOC_MAX_NODE
#define NET_ALLOC_MAX_NODE                              50U
#endif /* NET_ALLOC_MAX_NODE */

#if !defined NET_DISPLAY_TAB
#define NET_DISPLAY_TAB                                 4U
#endif /* NET_DISPLAY_TAB */

#if !defined NET_DISPLAY_WIDTH
#define NET_DISPLAY_WIDTH                               120U
#endif /* NET_DISPLAY_WIDTH */

#if !defined NET_DISPLAY_RESULT_WIDTH
#define NET_DISPLAY_RESULT_WIDTH                        8U
#endif /* NET_DISPLAY_RESULT_WIDTH */

#if !defined NET_DISPLAY_DIRNAME_LEN
#define NET_DISPLAY_DIRNAME_LEN                         30U
#endif /* NET_DISPLAY_DIRNAME_LEN */

#if !defined NET_ALLOC_VERBOSE
#define NET_ALLOC_VERBOSE 0
#endif /*   NET_ALLOC_VERBOSE */

#ifndef NET_LEAKAGE_ARRAY
#define NET_LEAKAGE_ARRAY 300U
#endif /* NET_LEAKAGE_ARRAY */

#ifndef NET_ALLOC_BREAK
#define NET_ALLOC_BREAK 0xFFFFFFFFU
#endif /* NET_ALLOC_BREAK */

/* From 32 to 2 exp(5 + NET_MALLOC_MAX_HISTOGRAM)= 65536 */
#ifndef NET_MALLOC_MAX_HISTOGRAM
#define NET_MALLOC_MAX_HISTOGRAM 11
#endif /* NET_ALLOC_BREAK */


#define NET_CALLOC(NUM, SIZE)  net_calloc_debug(NUM, SIZE, __FILE__, __LINE__)
#define NET_REALLOC(P, SIZE)   net_realloc_debug(P, SIZE, __FILE__, __LINE__)
#define NET_MALLOC(SIZE)       net_malloc_debug(SIZE, __FILE__, __LINE__)
#define NET_FREE(P)            net_free_debug(P)


void *net_calloc_debug(size_t num, size_t size, const char *filename, uint32_t line);
void *net_malloc_debug(size_t size, const char *filename, uint32_t line);
void *net_realloc_debug(void *p, size_t size, const char_t *filename, uint32_t line);
void  net_free_debug(void *p);

void net_alloc_report(void);


#else /* !NET_ALLOC_DEBUG */

#ifdef NET_USE_RTOS
#define NET_CALLOC(NUM, SIZE)  net_calloc(NUM, SIZE)
#define NET_REALLOC(P, SIZE)   net_realloc(P, SIZE)
#define NET_MALLOC(SIZE)       pvPortMalloc(SIZE)
#define NET_FREE(P)            vPortFree(P)

void *net_calloc(size_t num, size_t size);
void *net_realloc(void *p, size_t size);

#else
#define NET_CALLOC(NUM, SIZE)  calloc((NUM), (SIZE))
#define NET_REALLOC(P, SIZE)   realloc((P), (SIZE))
#define NET_MALLOC(SIZE)       malloc((SIZE))
#define NET_FREE(P)            free((P))
#endif /* NET_USE_RTOS */

#endif /* NET_ALLOC_DEBUG */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_MEM_H */
