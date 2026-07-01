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
#define USB_PLUGGED   1u
#define USB_UNPLUGGED 0u
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

static ULONG cdc_acm_interface_number;
static ULONG cdc_acm_configuration_number;
static UX_SLAVE_CLASS_CDC_ACM_PARAMETER cdc_acm_parameter;
static TX_THREAD ux_device_app_thread;
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USER CODE BEGIN PV */
static TX_THREAD ux_cdc_read_thread;
static TX_THREAD ux_cdc_write_thread;
TX_EVENT_FLAGS_GROUP EventFlag;
TX_QUEUE ux_app_MsgQueue;

/* State machine for VBUS monitoring */
__IO Device_State device_state = Device_VBUS_SENSING;
uint32_t vbus_on = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID app_ux_device_thread_entry(ULONG thread_input);
/* USER CODE BEGIN PFP */
static VOID USBC_Detect_Process(uint32_t raw);
/* USER CODE END PFP */

/**
  * @brief  Application USBX Device Initialization.
  * @param  memory_ptr: memory pointer
  * @retval status
  */

UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  UCHAR *pointer;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN MX_USBX_Device_Init 0 */

  /* USER CODE END MX_USBX_Device_Init 0 */

  /* USER CODE BEGIN MX_USBX_Device_Init 1 */

  /* USER CODE END MX_USBX_Device_Init 1 */

  /* Allocate the stack for device application main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_DEVICE_APP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_ALLOCATE_STACK_ERROR */
    return TX_POOL_ERROR;
    /* USER CODE END MAIN_THREAD_ALLOCATE_STACK_ERROR */
  }

  /* Create the device application main thread */
  if (tx_thread_create(&ux_device_app_thread, UX_DEVICE_APP_THREAD_NAME, app_ux_device_thread_entry,
                       0, pointer, UX_DEVICE_APP_THREAD_STACK_SIZE, UX_DEVICE_APP_THREAD_PRIO,
                       UX_DEVICE_APP_THREAD_PREEMPTION_THRESHOLD, UX_DEVICE_APP_THREAD_TIME_SLICE,
                       UX_DEVICE_APP_THREAD_START_OPTION) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_CREATE_ERROR */
    return TX_THREAD_ERROR;
    /* USER CODE END MAIN_THREAD_CREATE_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Device_Init 2 */
  /* Allocate the stack for usbx cdc acm read thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 1024, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the usbx cdc acm read thread */
  if (tx_thread_create(&ux_cdc_read_thread, "cdc_acm_read_usbx_app_thread_entry",
                       usbx_cdc_acm_read_thread_entry, 1, pointer,
                       1024, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for usbx cdc acm write thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 1024, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the usbx_cdc_acm_write_thread_entry thread */
  if (tx_thread_create(&ux_cdc_write_thread, "cdc_acm_write_usbx_app_thread_entry",
                       usbx_cdc_acm_write_thread_entry, 1, pointer,
                       1024, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the event flags group */
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
  /* USER CODE END MX_USBX_Device_Init 2 */

  return ret;
}

/**
  * @brief  MX_USBX_Device_Stack_Init
  *         Intialization of USB Device.
  *         Initialize the device stack, register of device class stack
  *         Register of the usb device controller
  * @param  None
  * @retval ret
  */
UINT MX_USBX_Device_Stack_Init(void)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG language_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;

  /* USER CODE BEGIN MX_USBX_Device_Stack_Init 0 */

  /* USER CODE END MX_USBX_Device_Stack_Init 0 */
  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                                                &device_framework_hs_length);

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(device_framework_high_speed,
                                 device_framework_hs_length,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 language_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_INITIALIZE_ERROR */
  }

  /* Initialize the cdc acm class parameters for the device */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate   = USBD_CDC_ACM_Activate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = USBD_CDC_ACM_Deactivate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change    = USBD_CDC_ACM_ParameterChange;

  /* USER CODE BEGIN CDC_ACM_PARAMETER */

  /* USER CODE END CDC_ACM_PARAMETER */

  /* Get cdc acm configuration number */
  cdc_acm_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_CDC_ACM, 0);

  /* Find cdc acm interface number */
  cdc_acm_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_CDC_ACM, 0);

  /* Initialize the device cdc acm class */
  if (ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                     ux_device_class_cdc_acm_entry,
                                     cdc_acm_configuration_number,
                                     cdc_acm_interface_number,
                                     &cdc_acm_parameter) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_CDC_ACM_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_CDC_ACM_REGISTER_ERROR */
  }

  /* Initialize and link controller HAL driver */
  ux_dcd_stm32_initialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_HS);
  /* USER CODE BEGIN MX_USBX_Device_Stack_Init_PostTreatment */
  /* USER CODE END MX_USBX_Device_Stack_Init_PostTreatment */

  /* USER CODE BEGIN MX_USBX_Device_Stack_Init 1 */

  /* USER CODE END MX_USBX_Device_Stack_Init 1 */

  return ret;
}

/**
  * @brief  Function implementing app_ux_device_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
static VOID app_ux_device_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN app_ux_device_thread_entry */
  GPIO_PinState level;
  /* USB_OTG_HS init function */
  MX_USB_OTG_HS_PCD_Init();

  /* Initialize the Stack USB Device*/
  if (MX_USBX_Device_Stack_Init() != UX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_INITIALIZE_STACK_ERROR */
    Error_Handler();
    /* USER CODE END MAIN_INITIALIZE_STACK_ERROR */
  }
  /* USB device start deferred until PA1 (USBC_DETECT) indicates attach */
  level = HAL_GPIO_ReadPin(USBC_DETECT_GPIO_Port, USBC_DETECT_Pin);

  if (level == GPIO_PIN_SET)
  {
    USBC_Detect_Process(USB_PLUGGED);
  }
  else
  {
    USBC_Detect_Process(USB_UNPLUGGED);
  }

  if (device_state == Device_Connection || device_state == Device_Disconnection)
  {
    if (level == GPIO_PIN_SET)
    {
      USBC_Detect_Process(USB_PLUGGED);
    }
    else
    {
      USBC_Detect_Process(USB_UNPLUGGED);
    }
  }

  /* USER CODE END app_ux_device_thread_entry */
}

/**
  * @brief  MX_USBX_Device_Stack_DeInit
  *         Unitialization of USB Device.
  *         uninitialize the device stack, unregister of device class stack
  *         unregister of the usb device controller
  * @retval ret
  */
UINT MX_USBX_Device_Stack_DeInit(void)
{
  UINT ret = UX_SUCCESS;

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PreTreatment_0 */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PreTreatment_0 */

  /* Unregister USB device controller. */

  if (ux_dcd_stm32_uninitialize((ULONG)USB_OTG_HS, (ULONG)&hpcd_USB_OTG_HS) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Unregister CDC ACM class. */
  if (ux_device_stack_class_unregister(_ux_system_slave_class_cdc_acm_name,
                                     ux_device_class_cdc_acm_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* The code below is required for uninstalling the device portion of USBX.  */
  if (ux_device_stack_uninitialize() != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PreTreatment_1 */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Device_Stack_DeInit_PostTreatment */
  /* USER CODE END MX_USBX_Device_Stack_DeInit_PostTreatment */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief  USBX_APP_UART_Init
  *         Initialization of UART.
  * @param  huart: Pointer to UART handler
  * @retval None
  */
VOID USBX_APP_UART_Init(UART_HandleTypeDef **huart)
{
  /* USER CODE BEGIN USBX_APP_UART_Init */
  MX_USART1_UART_Init();
  *huart = &huart1;
  /* USER CODE END USBX_APP_UART_Init */
}

/**
  * @brief  USBC_Detect_Process
  *         Handles USB device attach/detach state transitions based on PA1 (USBC_DETECT) digital level.
  *         Starts or stops the USB device.
  * @param  raw: VBUS sense value (1 = attach, 0 = detach) sampled from USBC_DETECT pin.
  * @retval none
  */
static VOID USBC_Detect_Process(uint32_t raw)
{
  switch (device_state)
  {
  case Device_VBUS_SENSING:
    if (raw)
    {
      device_state = Device_Connection;
    }
    else
    {
      device_state = Device_Disconnection;
    }
    break;

  case Device_Connection:
    if (vbus_on == 1)
    {
      if (HAL_PCD_Start(&hpcd_USB_OTG_HS) != HAL_OK)
      {
        Error_Handler();
      }
      vbus_on = 0;
    }
    device_state = Device_VBUS_SENSING;
    break;

  case Device_Disconnection:
    if (vbus_on == 0)
    {
      ux_device_stack_disconnect();
      if (HAL_PCD_Stop(&hpcd_USB_OTG_HS) != HAL_OK)
      {
        Error_Handler();
      }
      vbus_on = 1;
    }
    device_state = Device_VBUS_SENSING;
    break;

  default:
    device_state = Device_VBUS_SENSING;
    break;
  }
}

/**
  * @brief  GPIO EXTI Callback function
  *         Handles USB attach events triggered by USBC_DETECT pin rising edge.
  *         Invokes USB state machine to process device plug-in (attach).
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USBC_DETECT_Pin)
  {
      USBC_Detect_Process(USB_PLUGGED);
      if (device_state == Device_Connection || device_state == Device_Disconnection)
      {
          USBC_Detect_Process(USB_PLUGGED);
      }
  }
}

/**
  * @brief  GPIO EXTI Callback function
  *         Handles USB attach/detach events triggered by USBC_DETECT pin falling edge.
  *         Invokes USB state machine to process device unplug (detach).
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USBC_DETECT_Pin)
  {
      USBC_Detect_Process(USB_UNPLUGGED);
      if (device_state == Device_Connection || device_state == Device_Disconnection)
      {
          USBC_Detect_Process(USB_UNPLUGGED);
      }
  }
}
/* USER CODE END 1 */
