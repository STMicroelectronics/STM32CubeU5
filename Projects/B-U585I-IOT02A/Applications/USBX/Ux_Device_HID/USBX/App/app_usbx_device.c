/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
#include "ux_device_mouse.h"
#include "app_azure_rtos_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Define constants.  */
/* USB stack size */
#define USBX_APP_STACK_SIZE                          1024
/* USB memory size */
#define USBX_MEMORY_SIZE                             (32 * 1024)


/* ux_app_thread struct */
TX_THREAD    ux_app_thread;
/* ux_hid_thread struct */
TX_THREAD    ux_hid_thread;
/* ux app msg queue */
TX_QUEUE     ux_app_MsgQueue;

/* HID Class Calling Parameter structure */
UX_SLAVE_CLASS_HID_PARAMETER hid_parameter;
UX_SLAVE_CLASS_HID_EVENT     mouse_hid_event;

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

USB_MODE_STATE USB_Device_State_Msg;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void  usbx_app_thread_entry(ULONG arg);
/* USER CODE END PFP */
/**
  * @brief  Application USBX Device Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_USBX_Device_MEM_POOL */

  /* USER CODE END App_USBX_Device_MEM_POOL */

  /* USER CODE BEGIN MX_USBX_Device_Init */
#if (USE_MEMORY_POOL_ALLOCATION == 1)

  UCHAR *pointer;
  /* Device framework FS length*/
  ULONG device_framework_fs_length;
  /* Device String framework length*/
  ULONG string_framework_length;
  /* Device language id framework length*/
  ULONG languge_id_framework_length;
  /* Device Framework Full Speed */
  UCHAR *device_framework_full_speed;
  /* String Framework*/
  UCHAR *string_framework;
  /* Language_Id_Framework*/
  UCHAR *language_id_framework;
  /* Status Tx */
  UINT tx_status = UX_SUCCESS;
  /* Allocate the stack for thread 0.  */
  tx_status = tx_byte_allocate(byte_pool, (VOID **) &pointer,
                               USBX_MEMORY_SIZE, TX_NO_WAIT);

  /* Check memory allocation */
  if (UX_SUCCESS != tx_status)
  {
    Error_Handler();
  }
  /* Initialize USBX Memory */
  ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0);

  /* Get_Device_Framework_Full_Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* The code below is required for installing the device portion of USBX.
  In this application */
  ret =  _ux_device_stack_initialize(NULL,
                                     0,
                                     device_framework_full_speed,
                                     device_framework_fs_length,
                                     string_framework,
                                     string_framework_length,
                                     language_id_framework,
                                     languge_id_framework_length, UX_NULL);

  /* Check the Stack initialize status */
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
  }

  /* Initialize the hid class parameters for the device. */
  hid_parameter.ux_device_class_hid_parameter_report_address = USBD_Get_Device_HID_MOUSE_ReportDesc();

  hid_parameter.ux_device_class_hid_parameter_report_length = USBD_HID_MOUSE_REPORT_DESC_SIZE;

  hid_parameter.ux_device_class_hid_parameter_report_id = UX_TRUE;

  hid_parameter.ux_device_class_hid_parameter_callback = app_usbx_device_thread_hid_callback;

  /* Initialize the device hid class. The class is connected with interface 0 */
  ret = ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                       ux_device_class_hid_entry, 1, 0,
                                       (VOID *)&hid_parameter);

  /* Check the device stack class status */
  if (ret != UX_SUCCESS)
  {
    Error_Handler();
  }

  /* Put system definition stuff in here, e.g. thread creates and other assorted
  create information.  */

  /* Allocate the stack for main_usbx_app_thread_entry.  */
  tx_status = tx_byte_allocate(byte_pool, (VOID **) &pointer, USBX_APP_STACK_SIZE, TX_NO_WAIT);

  /* Check memory allocation */
  if (UX_SUCCESS != tx_status)
  {
    Error_Handler();
  }
  /* Create the main thread.  */
 tx_status = tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry",
                              usbx_app_thread_entry, 0, pointer, USBX_APP_STACK_SIZE,
							  20, 20, 1, TX_AUTO_START);

  /* Check usbx_app_thread_entry creation */
  if (UX_SUCCESS != tx_status)
  {
    Error_Handler();
  }
  /* Allocate the stack for hid_usbx_app_thread_entry.  */
  tx_status = tx_byte_allocate(byte_pool, (VOID **) &pointer,
                               USBX_APP_STACK_SIZE, TX_NO_WAIT);

  /* Check memory allocation */
  if (UX_SUCCESS != tx_status)
  {
    Error_Handler();
  }
  /* Create threads 1 and 2. These threads pass information through a ThreadX
  message queue.  It is also interesting to note that these threads have a time
  slice.  */
  tx_status = tx_thread_create(&ux_hid_thread, "hid_usbx_app_thread_entry",
                   usbx_hid_thread_entry, 1,
				   pointer, USBX_APP_STACK_SIZE, 20, 20,
				    1, TX_AUTO_START);

  /* Check usbx_hid_thread_entry creation */
  if (UX_SUCCESS != tx_status)
  {
    Error_Handler();
  }

  /* Allocate Memory for the Queue */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       sizeof(APP_QUEUE_SIZE*sizeof(ULONG)),
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create the MsgQueue */
  if (tx_queue_create(&ux_app_MsgQueue, "Message Queue app",
                      TX_1_ULONG, pointer,
                      APP_QUEUE_SIZE*sizeof(ULONG)) != TX_SUCCESS)
  {
    Error_Handler();
  }

#endif
  /* USER CODE END MX_USBX_Device_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing usbx_app_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_app_thread_entry(ULONG arg)
{
  /* Initialization of USB device */
  MX_USB_Device_Init();

  /* Wait for message queue to start/stop the device */
  while(1)
  {
    /* Wait for a device to be connected */
    if (tx_queue_receive(&ux_app_MsgQueue, &USB_Device_State_Msg,
                         TX_WAIT_FOREVER)!= TX_SUCCESS)
    {
     /*Error*/
     Error_Handler();
    }
    /* Check if received message equal to USB_PCD_START */
    if (USB_Device_State_Msg == START_USB_DEVICE)
    {
      /* Start device USB */
      HAL_PCD_Start(&hpcd_USB_OTG_FS);
    }
    /* Check if received message equal to USB_PCD_STOP */
    else if (USB_Device_State_Msg == STOP_USB_DEVICE)
    {
      /* Stop device USB */
      HAL_PCD_Stop(&hpcd_USB_OTG_FS);
    }
    /* Else Error */
    else
    {
      /*Error*/
      Error_Handler();
    }
  }
}

/**
  * @brief MX_USB_Device_Init
  *        Initialization of USB device.
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */
  /* USER CODE END USB_Device_Init_PreTreatment_0 */

  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */
  /* Set Rx FIFO */
  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x80);
  /* Set Tx FIFO 0 */
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x40);
  /* Set Tx FIFO 1 */
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x40);
  /* USER CODE END USB_Device_Init_PreTreatment_1 */

  /* initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG)USB_OTG_FS, (ULONG)&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN USB_Device_Init_PostTreatment */
  /* USER CODE END USB_Device_Init_PostTreatment */
}

/* USER CODE END 1 */
