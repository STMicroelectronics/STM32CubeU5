/**
  ******************************************************************************
  * @file    net_perf.h
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
#ifndef NET_PERF_H
#define NET_PERF_H

/* Includes ------------------------------------------------------------------*/
/* disable Misra rule to enable doxigen comment , A sectio of code appear to have been commented out */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "net_conf.h"
#include "net_types.h"


#ifdef NET_USE_RTOS

#if (osCMSIS < 0x20000U)
#define NET_TICK   osKernelSysTick
#else
#define NET_TICK   osKernelGetTickCount
#endif /* osCMSIS */

#else
#define NET_TICK        HAL_GetTick

#endif /* NET_USE_RTOS */


#ifndef __IO
#define __IO volatile
#endif /* __IO */

#define NET_DWT_CONTROL             (*((__IO uint32_t*)0xE0001000U))
#define NET_DWT_CYCCNTENA_BIT       (1UL<<0U)
/*!< DWT Cycle Counter register */
#define NET_DWT_CYCCNT              (*((__IO uint32_t*)0xE0001004U))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define NET_DEMCR                   (*((__IO uint32_t*)0xE000EDFCU))
/*!< Trace enable bit in DEMCR register */
#define NET_TRCENA_BIT              (1UL<<24U)


static inline uint32_t net_get_cycle(void)
{
  return NET_DWT_CYCCNT;
}
extern uint32_t SystemCoreClock;

static inline uint32_t net_get_us(void)
{
  return NET_DWT_CYCCNT / (SystemCoreClock / 1000000U);
}


static inline void net_stop_cycle(void)
{
  NET_DWT_CONTROL &= ~NET_DWT_CYCCNTENA_BIT ;
}

static inline void net_start_cycle(void)
{
  NET_DWT_CONTROL |= NET_DWT_CYCCNTENA_BIT ;
}

void net_perf_start(void);
void net_perf_report(void);

#ifdef NET_USE_RTOS

#if defined(NET_PERF_TASK) && !defined(NET_FREERTOS_PERF)
#warning "To use NET_PERF_TASK please add following lines to FreeRTOSConfig.h"
#warning  "           void net_perf_task_in(void);"
#warning  "           void net_perf_task_out(void);"
#warning  "           #define NET_FREERTOS_PERF"
#warning  "           #define traceTASK_SWITCHED_IN net_perf_task_in"
#warning  "           #define traceTASK_SWITCHED_OUT net_perf_task_out"
#endif /* NET_PERF_TASK */

#if defined(NET_PERF_TASK)
void net_perf_task_tag(uint32_t msg_id);

#ifndef NET_PERF_TASK_TAG
#define NET_PERF_TASK_TAG       net_perf_task_tag
#endif /* NET_PERF_TASK_TAG */

#endif /* NET_PERF_TASK*/

#else /* NET_USE_RTOS */
uint32_t HAL_GetTick(void);

#endif /* NET_USE_RTOS */


#ifndef NET_PERF_TASK_TAG
#define NET_PERF_TASK_TAG(...)
#endif /* NET_PERF_TASK_TAG */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_PERF_H */
