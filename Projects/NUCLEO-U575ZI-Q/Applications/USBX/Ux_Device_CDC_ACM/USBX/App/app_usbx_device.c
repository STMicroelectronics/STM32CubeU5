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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USBX_APP_STACK_SIZE         1024
#define USBX_MEMORY_SIZE            (4 * 1024)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD                           ux_app_thread;
TX_THREAD                           ux_cdc_read_thread;
TX_THREAD                           ux_cdc_write_thread;
TX_QUEUE                            ux_app_MsgQueue;
TX_EVENT_FLAGS_GROUP                EventFlag;
UX_SLAVE_CLASS_CDC_ACM_PARAMETER    cdc_acm_parameter;
USB_MODE_STATE                      USB_Device_State_Msg;
extern PCD_HandleTypeDef            hpcd_USB_OTG_FS;
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
#if (USE_STATIC_ALLOCATION == 1)

  UCHAR *pointer;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG languge_id_framework_length;
  UCHAR *device_framework_full_speed;
  UCHAR *string_framework;
  UCHAR *language_id_framework;

  /* Allocate USBX_MEMORY_SIZE. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get_Device_Framework_Full_Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* The code below is required for installing the device portion of USBX.
     In this application */
  if (ux_device_stack_initialize(NULL,
                                 0U,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 languge_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the cdc class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate = CDC_Init_FS;

  /* Deinitialize the cdc class parameters for the device. */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = CDC_DeInit_FS;

  /* Manage the CDC class requests */
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change = ux_app_parameters_change;

  /* Registers a slave class to the slave stack. The class is connected with
     interface 0 */
  if (ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                     ux_device_class_cdc_acm_entry, 1, 0,
                                     (VOID *)&cdc_acm_parameter) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Allocate the stack for main_usbx_app_thread_entry. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the usbx_app_thread_entry main thread. */
  if (tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry",
                       usbx_app_thread_entry, 0, pointer, USBX_APP_STACK_SIZE,
                       20, 20, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for usbx_cdc_acm_read_thread_entry. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the usbx_cdc_acm_read_thread_entry thread. */
  if (tx_thread_create(&ux_cdc_read_thread, "cdc_acm_read_usbx_app_thread_entry",
                       usbx_cdc_acm_read_thread_entry, 1, pointer,
                       USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for usbx_cdc_acm_write_thread_entry. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the usbx_cdc_acm_write_thread_entry thread. */
  if (tx_thread_create(&ux_cdc_write_thread, "cdc_acm_write_usbx_app_thread_entry",
                       usbx_cdc_acm_write_thread_entry, 1, pointer,
                       USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the event flags group. */
  if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
  {
    return TX_GROUP_ERROR;
  }

  /* Allocate Memory for the Queue */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, APP_QUEUE_SIZE*sizeof(ULONG),
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the MsgQueue */
  if (tx_queue_create(&ux_app_MsgQueue, "Message Queue app", TX_1_ULONG,
                      pointer, APP_QUEUE_SIZE*sizeof(ULONG)) != TX_SUCCESS)
  {
    return TX_QUEUE_ERROR;
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

  /* USB_OTG_HS init function */
  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */
  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 0x100);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 0x10);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 0x20);
  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 2, 0x10);
  /* USER CODE END USB_Device_Init_PreTreatment_1 */

  /* initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG)USB_OTG_FS, (ULONG)&hpcd_USB_OTG_FS);

  /* USER CODE BEGIN USB_Device_Init_PostTreatment */
  /* USER CODE END USB_Device_Init_PostTreatment */
}

/* USER CODE END 1 */
