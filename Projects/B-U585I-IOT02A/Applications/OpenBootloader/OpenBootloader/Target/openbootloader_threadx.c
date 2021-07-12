/**
  ******************************************************************************
  * @file    openbootloader_threadx.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "openbl_core.h"
#include "ux_device_dfu_media.h"
#include "usb_interface.h"
#include "interfaces_conf.h"
#include "openbootloader_threadx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_THREAD_MEMORY_SIZE         (2UL * 1024UL)
#define UX_DETECT_INTERFACE               0x01
#define OPENBL_USBX_ENTRY_INPUT           0
#define OPENBL_USBX_PRIORITY              20
#define OPENBL_USBX_PREEMT_THRESHOLD      20

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TX_EVENT_FLAGS_GROUP EventDetectInterface;
static TX_THREAD ux_detect_interface_thread;
static TX_THREAD ux_command_process_thread;
static CHAR *OpenBootloaderPointer;

/* External variables --------------------------------------------------------*/
extern uint8_t JumpUsb;
extern TX_THREAD ux_app_thread;
extern TX_THREAD usbx_dfu_download_thread;

/* Private function prototypes -----------------------------------------------*/
void OpenBootloader_DetectInterfaceThread(ULONG arg);
void OpenBootloader_ProcessCommandThread(ULONG arg);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize and create the open Bootloader threads.
  * @param  memory_ptr : memory pointer.
  * @retval returns UX_SUCCESS if there's no error else returns TX_GROUP_ERROR.
  */
uint16_t OpenBootloader_ThreadxInit(void *memory_ptr)
{
  UINT status = UX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL *)memory_ptr;

  /* Allocate the stack for OpenBootloader_DetectInterfaceThread */
  status = tx_byte_allocate(byte_pool, (VOID **) &OpenBootloaderPointer,
                            OPENBL_THREAD_MEMORY_SIZE, TX_NO_WAIT);

  /* Check the allocation operation status */
  if (UX_SUCCESS != status)
  {
    Error_Handler();
  }

  /* Create the OpenBootloader_DetectInterfaceThread thread. */
  status = tx_thread_create(&ux_detect_interface_thread, "OpenBootloader_DetectInterfaceThread",
                            OpenBootloader_DetectInterfaceThread, OPENBL_USBX_ENTRY_INPUT,
                            OpenBootloaderPointer, OPENBL_THREAD_MEMORY_SIZE, OPENBL_USBX_PRIORITY,
                            OPENBL_USBX_PREEMT_THRESHOLD, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Check the thread creation status */
  if (UX_SUCCESS != status)
  {
    Error_Handler();
  }

  /* Allocate the stack for OpenBootloader_ProcessCommandThread */
  status = tx_byte_allocate(byte_pool, (VOID **) &OpenBootloaderPointer,
                            OPENBL_THREAD_MEMORY_SIZE, TX_NO_WAIT);

  /* Check the allocation operation status */
  if (UX_SUCCESS != status)
  {
    Error_Handler();
  }

  /* Create the OpenBootloader_ProcessCommandThread thread */
  status = tx_thread_create(&ux_command_process_thread, "OpenBootloader_ProcessCommandThread",
                            OpenBootloader_ProcessCommandThread, OPENBL_USBX_ENTRY_INPUT,
                            OpenBootloaderPointer, OPENBL_THREAD_MEMORY_SIZE, OPENBL_USBX_PRIORITY,
                            OPENBL_USBX_PREEMT_THRESHOLD, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Check the thread creation status */
  if (UX_SUCCESS != status)
  {
    Error_Handler();
  }

  /* Create the event detect interface */
  if (tx_event_flags_create(&EventDetectInterface, "Event Detect Interface") != TX_SUCCESS)
  {
    status = TX_GROUP_ERROR;
  }

  return status;
}

/**
  * @brief  Sends an event to the OpenBootloader_ProcessCommandThread thread as soon as it detects an interface.
  * @param  arg: Not used
  * @retval None
  */
void OpenBootloader_DetectInterfaceThread(ULONG arg)
{
  UNUSED(arg);

  /*
     When we detect an interface other than USB, we disable the USB and the SysTick interrupt,
     and we send an event to the command process thread.
     If we detect an USB interface and a GO command is executed, we jump to the user application
     using the DFU_Jump() function.
  */
  while (1)
  {
    if (OPENBL_InterfaceDetection() == 1)
    {
      /* Disable the SysTick timer */
      NVIC_DisableIRQ(SysTick_IRQn);
      NVIC_ClearPendingIRQ(SysTick_IRQn);

      /* Disable the USB */
      OPENBL_USB_DeInit();

      /* Notify the command process thread that an interface is detected */
      if (tx_event_flags_set(&EventDetectInterface, UX_DETECT_INTERFACE, TX_OR) != TX_SUCCESS)
      {
        Error_Handler();
      }

      break;
    }

    if (JumpUsb == 1)
    {
      DFU_Jump();
    }

    /* Sleep for 1 ms so we don't stay blocked in this thread */
    tx_thread_sleep(1);
  }
}

/**
  * @brief  Wait for a detect interface event and process the command coming from the detected interface.
  * @param  arg: Not used
  * @retval None
  */
void OpenBootloader_ProcessCommandThread(ULONG arg)
{
  ULONG download_flags = 0;
  UINT status          = 0U;

  UNUSED(arg);

  /* Infinite loop */
  while (1)
  {
    /* Wait until the requested flag UX_DETECT_INTERFACE is received */
    status = tx_event_flags_get(&EventDetectInterface, UX_DETECT_INTERFACE,
                                TX_OR_CLEAR, &download_flags, TX_WAIT_FOREVER);

    if (status == UX_SUCCESS)
    {
      /* Delete other threads */
      _tx_thread_terminate(&ux_detect_interface_thread);
      _tx_thread_terminate(&usbx_dfu_download_thread);
      _tx_thread_terminate(&ux_app_thread);

      while (1)
      {
        /* Process the commands from the detected interface */
        OPENBL_CommandProcess();
      }
    }
  }
}
