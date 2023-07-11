/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_host.c
  * @author  MCD Application Team
  * @brief   USBX host applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_QUEUE_SIZE                               1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TX_THREAD ux_host_app_thread;
/* USER CODE BEGIN PV */
UX_HOST_CLASS_CDC_ACM        *cdc_acm;
TX_EVENT_FLAGS_GROUP         ux_app_EventFlag;
TX_THREAD                    cdc_acm_send_thread;
TX_THREAD                    cdc_acm_recieve_thread;
TX_QUEUE                     ux_app_MsgQueue_UCPD;

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN USB_MODE_STATE USB_Host_State_Msg   __ALIGN_END;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID app_ux_host_thread_entry(ULONG thread_input);
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* USER CODE BEGIN PFP */
extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
/* USER CODE END PFP */

/**
  * @brief  Application USBX Host Initialization.
  * @param  memory_ptr: memory pointer
  * @retval status
  */
UINT MX_USBX_Host_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  UCHAR *pointer;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_USBX_Host_Init0 */

  /* USER CODE END MX_USBX_Host_Init0 */

  /* Allocate the stack for USBX Memory */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_HOST_MEMORY_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_ALLOCATE_STACK_ERROR */
    return TX_POOL_ERROR;
    /* USER CODE END USBX_ALLOCATE_STACK_ERROR */
  }

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_HOST_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_SYSTEM_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_SYSTEM_INITIALIZE_ERROR */
  }

  /* Install the host portion of USBX */
  if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_HOST_INITIALIZE_ERROR */
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);

  /* Initialize the host cdc acm class */
  if ((ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name,
                                    ux_host_class_cdc_acm_entry)) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_CDC_ACM_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_HOST_CDC_ACM_REGISTER_ERROR */
  }

  /* Allocate the stack for host application main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_HOST_APP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_ALLOCATE_STACK_ERROR */
    return TX_POOL_ERROR;
    /* USER CODE END MAIN_THREAD_ALLOCATE_STACK_ERROR */
  }

  /* Create the host application main thread */
  if (tx_thread_create(&ux_host_app_thread, UX_HOST_APP_THREAD_NAME, app_ux_host_thread_entry,
                       0, pointer, UX_HOST_APP_THREAD_STACK_SIZE, UX_HOST_APP_THREAD_PRIO,
                       UX_HOST_APP_THREAD_PREEMPTION_THRESHOLD, UX_HOST_APP_THREAD_TIME_SLICE,
                       UX_HOST_APP_THREAD_START_OPTION) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_CREATE_ERROR */
    return TX_THREAD_ERROR;
    /* USER CODE END MAIN_THREAD_CREATE_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Host_Init1 */


  /* Allocate the stack for cdc_acm receive thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       UX_HOST_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the cdc_acm_recieve thread */
  if (tx_thread_create(&cdc_acm_recieve_thread, "CDC Receive App thread",
                       cdc_acm_recieve_app_thread_entry, 0, pointer,
                       UX_HOST_APP_THREAD_STACK_SIZE, 30, 30, 0,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for cdc_acm send thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       UX_HOST_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the cdc_acm_send thread */
  if (tx_thread_create(&cdc_acm_send_thread, "CDC Read App thread",
                       cdc_acm_send_app_thread_entry, 0, pointer,
                       UX_HOST_APP_THREAD_STACK_SIZE, 30, 30, 0,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

/* Allocate Memory for the ux_app_Queue_UCPD  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_QUEUE_SIZE * sizeof(ULONG), TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

/* Create the MsgQueue for ucpd_app_thread   */
  if (tx_queue_create(&ux_app_MsgQueue_UCPD, "Message Queue UCPD", TX_1_ULONG,
                      pointer, APP_QUEUE_SIZE * sizeof(ULONG)) != TX_SUCCESS)
  {
    return TX_QUEUE_ERROR;
  }

  /* Create the event flags group */
  if (tx_event_flags_create(&ux_app_EventFlag, "Event Flag") != TX_SUCCESS)
  {
    return TX_GROUP_ERROR;
  }

  /* USER CODE END MX_USBX_Host_Init1 */

  return ret;
}

/**
  * @brief  Function implementing app_ux_host_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
static VOID app_ux_host_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN app_ux_host_thread_entry */

  /* Initialization of USB host */
  USBX_APP_Host_Init();

  while (1)
  {
    /* wait for message queue from callback event */
    if(tx_queue_receive(&ux_app_MsgQueue_UCPD, &USB_Host_State_Msg, TX_WAIT_FOREVER)!= TX_SUCCESS)
    {
     Error_Handler();
    }
    /* Check if received message equal to START_USB_HOST */
    if (USB_Host_State_Msg == START_USB_HOST)
    {
      /* Start USB Host  */
      HAL_HCD_Start(&hhcd_USB_OTG_FS);
    }
    /* Check if received message equal to STOP_USB_HOST */
    else if (USB_Host_State_Msg == STOP_USB_HOST)
    {
      /* Stop USB Host */
      HAL_HCD_Stop(&hhcd_USB_OTG_FS);
    }
    /* Else Error */
    else
    {
      /*Error*/
      Error_Handler();
    }
   tx_thread_sleep(MS_TO_TICK(10));
  }

  /* USER CODE END app_ux_host_thread_entry */
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN ux_host_event_callback0 */

  /* USER CODE END ux_host_event_callback0 */

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* USER CODE BEGIN UX_DEVICE_INSERTION */

      /* Get current CDC Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_cdc_acm_entry)
      {
        if (cdc_acm == UX_NULL)
        {
          /* Get current CDC Instance */
          cdc_acm = (UX_HOST_CLASS_CDC_ACM *)current_instance;

          /* Check if this is CDC DATA instance */
          if (cdc_acm -> ux_host_class_cdc_acm_bulk_in_endpoint == UX_NULL)
          {
            cdc_acm = UX_NULL;
          }
          else
          {
            USBH_UsrLog("\nUSB CDC Device Found");
            USBH_UsrLog("PID: %#x ", (UINT)cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idProduct);
            USBH_UsrLog("VID: %#x ", (UINT)cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idVendor);
            USBH_UsrLog("Data Interface initialized");
          }
        }
      }

      /* USER CODE END UX_DEVICE_INSERTION */

      break;

    case UX_DEVICE_REMOVAL:

      /* USER CODE BEGIN UX_DEVICE_REMOVAL */

      if ((VOID*)cdc_acm == current_instance)
      {
        /* Clear cdc instance */
        cdc_acm = UX_NULL;
        USBH_UsrLog("\nUSB CDC ACM Device Removal");

        /* Set NEW_RECEIVED_DATA flag */
        if (tx_event_flags_set(&ux_app_EventFlag, REMOVED_CDC_INSTANCE, TX_OR) != TX_SUCCESS)
        {
          Error_Handler();
        }
      }

      /* USER CODE END UX_DEVICE_REMOVAL */

      break;

    case UX_DEVICE_CONNECTION:

      /* USER CODE BEGIN UX_DEVICE_CONNECTION */

      /* USER CODE END UX_DEVICE_CONNECTION */

      break;

    case UX_DEVICE_DISCONNECTION:

      /* USER CODE BEGIN UX_DEVICE_DISCONNECTION */

      /* USER CODE END UX_DEVICE_DISCONNECTION */

      break;

    default:

      /* USER CODE BEGIN EVENT_DEFAULT */

      /* USER CODE END EVENT_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_event_callback1 */

  /* USER CODE END ux_host_event_callback1 */

  return status;
}

/**
  * @brief ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval Status
  */
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  /* USER CODE BEGIN ux_host_error_callback0 */

  /* USER CODE END ux_host_error_callback0 */

  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:

      /* USER CODE BEGIN UX_DEVICE_ENUMERATION_FAILURE */

      USBH_UsrLog("USB Device Enumeration Failure");

      /* USER CODE END UX_DEVICE_ENUMERATION_FAILURE */

      break;

    case  UX_NO_DEVICE_CONNECTED:

      /* USER CODE BEGIN UX_NO_DEVICE_CONNECTED */

      USBH_UsrLog("USB Device disconnected");
      /* USER CODE END UX_NO_DEVICE_CONNECTED */

      break;

    default:

      /* USER CODE BEGIN ERROR_DEFAULT */

      /* USER CODE END ERROR_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_error_callback1 */

  /* USER CODE END ux_host_error_callback1 */
}

/* USER CODE BEGIN 1 */



/**
  * @brief USBX_APP_Host_Init
  *        Initialization of USB Host.
  * @param  none
  * @retval none
  */
VOID USBX_APP_Host_Init(VOID)
{
  /* USER CODE BEGIN USB_Host_Init_PreTreatment_0 */
  /* USER CODE END USB_Host_Init_PreTreatment_0 */

  /* Initialize the LL driver */
  MX_USB_OTG_FS_HCD_Init();

  /* Initialize the host controller driver */
  ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                             _ux_hcd_stm32_initialize, (ULONG)USB_OTG_FS,
                             (ULONG)&hhcd_USB_OTG_FS);

  /* USER CODE BEGIN USB_Host_Init_PreTreatment1 */
  /* Initialize user_button to handle sending data */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Start Application Message */
  USBH_UsrLog("**** USB OTG FS CDC Host ****\n");
  USBH_UsrLog("USB Host library started.\n");

  /* Wait for Device to be attached */
  USBH_UsrLog("Starting CDC Application");
  USBH_UsrLog("Connect your CDC Device");

  /* USER CODE END USB_Host_Init_PreTreatment1 */

}





/**
  * @brief  GPIO EXTI Callback function
  *         Handle Sending data through key button
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    /* Set NEW_DATA_TO_SEND flag */
    if (tx_event_flags_set(&ux_app_EventFlag, NEW_DATA_TO_SEND, TX_OR) != TX_SUCCESS)
    {
      Error_Handler();
    }
  }
}
/* USER CODE END 1 */
