/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_module.c
  * @author  MCD Application Team
  * @brief   ThreadX Module applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
/* Specify that this is a module! */
#define TXM_MODULE

/* Include the ThreadX module header. */
#include "txm_module.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
PROCESSING_NOT_STARTED    = 99,
WRITING_TO_READWRITE      = 88,
WRITING_TO_READONLY       = 77,
READING_FROM_READWRITE    = 66,
READING_FROM_READONLY     = 55,
PROCESSING_FINISHED       = 44
} ProgressState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_STACK_SIZE         3*1024
#define DEFAULT_BYTE_POOL_SIZE     9120
#define DEFAULT_BLOCK_POOL_SIZE    1024

#define READONLY_REGION            0x20010000
#define READWRITE_REGION           0x20010100

#define MAIN_THREAD_PRIO                         2
#define MAIN_THREAD_PREEMPTION_THRESHOLD         MAIN_THREAD_PRIO
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define UNUSED(x) (void)(x)

/* Define the pool space in the bss section of the module. ULONG is used to
   get word alignment. */
#if defined(__GNUC__) || defined(__CC_ARM) || defined(__ARMCC_VERSION)
ULONG  default_module_pool_space[DEFAULT_BYTE_POOL_SIZE / 4] __attribute__ ((aligned(32)));
#else /* __ICCARM__ */
_Pragma("data_alignment=32") ULONG  default_module_pool_space[DEFAULT_BYTE_POOL_SIZE / 4];
#endif
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD               *MainThread;
TX_BYTE_POOL            *ModuleBytePool;
TX_BLOCK_POOL           *ModuleBlockPool;
TX_QUEUE                *ResidentQueue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void default_module_start(ULONG id);
void MainThread_Entry(ULONG thread_input);
void Error_Handler(void);
/* USER CODE END PFP */

/**
  * @brief  Module entry function.
  * @param  id : Module ID
  * @retval None
  */
void default_module_start(ULONG id)
{
  CHAR    *pointer;

  /* Allocate all the objects. In MPU mode, modules cannot allocate control blocks within
  their own memory area so they cannot corrupt the resident portion of ThreadX by overwriting
  the control block(s).  */
  txm_module_object_allocate((void*)&MainThread, sizeof(TX_THREAD));
  txm_module_object_allocate((void*)&ModuleBytePool, sizeof(TX_BYTE_POOL));
  txm_module_object_allocate((void*)&ModuleBlockPool, sizeof(TX_BLOCK_POOL));

  /* Create a byte memory pool from which to allocate the thread stacks.  */
  tx_byte_pool_create(ModuleBytePool, "Module Byte Pool", (UCHAR*)default_module_pool_space, DEFAULT_BYTE_POOL_SIZE);

  /* Allocate the stack for thread 0.  */
  tx_byte_allocate(ModuleBytePool, (VOID **) &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);

  /* Create the main thread.  */
  tx_thread_create(MainThread, "Module Main Thread", MainThread_Entry, 0,
                   pointer, DEFAULT_STACK_SIZE,
                   MAIN_THREAD_PRIO, MAIN_THREAD_PREEMPTION_THRESHOLD, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Allocate the memory for a small block pool. */
  tx_byte_allocate(ModuleBytePool, (VOID **) &pointer,
                   DEFAULT_BLOCK_POOL_SIZE, TX_NO_WAIT);

  /* Create a block memory pool. */
  tx_block_pool_create(ModuleBlockPool, "Module Block Pool",
                       sizeof(ULONG), pointer, DEFAULT_BLOCK_POOL_SIZE);

  /* Allocate a block. */
  tx_block_allocate(ModuleBlockPool, (VOID **) &pointer,
                    TX_NO_WAIT);

  /* Release the block back to the pool. */
  tx_block_release(pointer);

}

/**
  * @brief  Module main thread.
  * @param  thread_input: thread id
  * @retval none
  */
void MainThread_Entry(ULONG thread_input)
{
  UINT status;
  ULONG s_msg;
  ULONG readbuffer;

  /* Request access to the queue from the module manager */
  status = txm_module_object_pointer_get(TXM_QUEUE_OBJECT, "Resident Queue", (VOID **)&ResidentQueue);

  if(status)
  {
    Error_Handler();
  }

  /* Writing to write and read region */
  s_msg = WRITING_TO_READWRITE;
  tx_queue_send(ResidentQueue, &s_msg, TX_NO_WAIT);
  *(ULONG *)READWRITE_REGION = 0xABABABAB;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* WReading from write and read region */
  s_msg = READING_FROM_READWRITE;
  tx_queue_send(ResidentQueue, &s_msg, TX_NO_WAIT);
  readbuffer = *(ULONG*)READWRITE_REGION;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Reading from read only region */
  s_msg = READING_FROM_READONLY;
  tx_queue_send(ResidentQueue, &s_msg, TX_NO_WAIT);
  readbuffer = *(ULONG*)READONLY_REGION;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Writing to read only region */
  s_msg = WRITING_TO_READONLY;
  tx_queue_send(ResidentQueue, &s_msg, TX_NO_WAIT);
  *(ULONG *)READONLY_REGION = 0xABABABAB;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Notify module manager about job finish */
  s_msg = PROCESSING_FINISHED;
  tx_queue_send(ResidentQueue, &s_msg, TX_NO_WAIT);

  /* Suppress unused variable warning */
  UNUSED(readbuffer);

  /* Stay here, waiting for the module manager to stop and loading the module*/
  while(1)
  {
    tx_thread_sleep(10);
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Nothing to do, block here */
  tx_thread_sleep(TX_WAIT_FOREVER);
}
