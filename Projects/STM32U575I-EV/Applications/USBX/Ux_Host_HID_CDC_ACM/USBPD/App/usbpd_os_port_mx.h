/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_os_port_mx.h
  * @author  MCD Application Team
  * @brief   This file contains the core os portability macro definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef USBPD_CORE_OSPORT_H_
#define USBPD_CORE_OSPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tx_api.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CORE_OS
  * @{
  */

/** @addtogroup USBPD_CORE_OS_Macro
  * @{
  */
/* Exported define -----------------------------------------------------------*/
/**
  * @brief macro definition used to define the task function
  */
#define DEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(ULONG argument)

/**
  * @brief macro definition used to initialize the OS environment
  */
#define OS_INIT()                                        \
    TX_BYTE_POOL *usbpd_pool = (TX_BYTE_POOL*)MemoryPtr; \
    char *ptr;                                           \
    uint32_t _retr = TX_SUCCESS;

/**
  * @brief macro definition the define a queue type
  */
#define OS_QUEUE_ID TX_QUEUE

/**
  * @brief macro definition the define a queue type
  */
#define OS_ELEMENT_SIZE TX_1_ULONG

/**
  * @brief macro definition used to define a queue
  */
#define OS_CREATE_QUEUE(_ID_,_NAME_, _ELT_,_ELTSIZE_)                                                  \
  do{                                                                                                  \
    _retr = tx_byte_allocate(usbpd_pool, (void **) &ptr,(_ELT_)*sizeof(ULONG)*(_ELTSIZE_),TX_NO_WAIT); \
    if(_retr != TX_SUCCESS)                                                                            \
    {                                                                                                  \
      goto error;                                                                                      \
    }                                                                                                  \
    _retr = tx_queue_create(&(_ID_),(_NAME_), (_ELTSIZE_), ptr ,(_ELT_)*sizeof(ULONG)*(_ELTSIZE_));    \
    if(_retr != TX_SUCCESS)                                                                            \
    {                                                                                                  \
      goto error;                                                                                      \
    }                                                                                                  \
  } while(0);

/**
  * @brief macro definition used to read a queue message
  */
#define OS_GETMESSAGE_QUEUE(_ID_, _TIME_)                                      \
  do {                                                                         \
    ULONG value;                                                               \
    tx_queue_receive(&(_ID_), (void*)&value, (_TIME_));                        \
  } while(0)

/**
  * @brief macro definition used to define put a message inside the queue
  */
#define OS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_)                               \
  do{                                                                            \
    ULONG _msg = _MSG_;                                                          \
    (void)tx_queue_send(&(_ID_), &_msg,(_TIMEOUT_));                             \
  }while(0)

/**
  * @brief macro definition used to define a task
  */
#define OS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)

/**
  * @brief macro definition of the TASK id
  */
#define OS_TASK_ID   TX_THREAD

/**
  * @brief macro definition used to create a task
  */
#define OS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)           \
  do {                                                                                \
    _retr = tx_byte_allocate(usbpd_pool, (void **)&ptr,(_STACK_SIZE_),TX_NO_WAIT);    \
    if(_retr != TX_SUCCESS)                                                           \
    {                                                                                 \
      goto error;                                                                     \
    }                                                                                 \
    _retr = tx_thread_create(&(_ID_),#_NAME_,(_FUNC_), _PARAM_,                       \
                         ptr,(_STACK_SIZE_),                                          \
                         _PRIORITY_, 1, TX_NO_TIME_SLICE,                             \
                         TX_AUTO_START);                                              \
    if(_retr != TX_SUCCESS)                                                           \
    {                                                                                 \
      goto error;                                                                     \
    }                                                                                 \
  } while(0);

/**
  * @brief macro definition used to check is task is suspended
  */
#define OS_TASK_IS_SUPENDED(_ID_) (TX_SUSPENDED == (_ID_).tx_thread_state)

/**
  * @brief macro definition used to get the task ID
  */
#define OS_TASK_GETID()          tx_thread_identify()

/**
  * @brief macro definition used to suspend a task
  */
#define OS_TASK_SUSPEND(_ID_)    tx_thread_suspend(_ID_)

/**
  * @brief macro definition used to resume a task
  */
#define OS_TASK_RESUME(_ID_)     tx_thread_resume(&_ID_)

/**
  * @brief macro definition used to manage the delay
  */
#define OS_DELAY(_TIME_)   tx_thread_sleep(_TIME_)

/**
  * @brief macro definition used to start the task scheduling
  */
#define OS_KERNEL_START() /* This function is not managed at usbpd level in the case of threadX */

/* Exported types ------------------------------------------------------------*/
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* USBPD_CORE_OSPORT_H_ */

