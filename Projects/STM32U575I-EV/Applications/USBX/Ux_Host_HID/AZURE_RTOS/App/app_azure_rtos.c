/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_azure_rtos.c
  * @author  MCD Application Team
  * @brief   app_azure_rtos application implementation file
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
#include "app_azure_rtos.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

#if (USE_MEMORY_POOL_ALLOCATION == 1)
/* USER CODE BEGIN TX_Pool_Buffer */
/* USER CODE END TX_Pool_Buffer */
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

/* USER CODE BEGIN UX_HOST_Pool_Buffer */
/* USER CODE END UX_HOST_Pool_Buffer */
static UCHAR  ux_host_byte_pool_buffer[UX_HOST_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL ux_host_app_byte_pool;

/* USER CODE BEGIN USBPD_Pool_Buffer */
/* USER CODE END USBPD_Pool_Buffer */
static UCHAR  usbpd_byte_pool_buffer[USBPD_DEVICE_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL usbpd_app_byte_pool;

#endif

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

  /**
  * @brief  Define the initial system.
  * @param  first_unused_memory : Pointer to the first unused memory
  * @retval None
  */
VOID tx_application_define(VOID *first_unused_memory)
{
  /* USER CODE BEGIN  tx_application_define_1*/

  /* USER CODE END  tx_application_define_1 */
#if (USE_MEMORY_POOL_ALLOCATION == 1)
  UINT status = TX_SUCCESS;
  VOID *memory_ptr;

  if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS)
  {
    /* USER CODE BEGIN TX_Byte_Pool_Error */

    /* USER CODE END TX_Byte_Pool_Error */
  }
  else
  {
    /* USER CODE BEGIN TX_Byte_Pool_Success */

    /* USER CODE END TX_Byte_Pool_Success */

    memory_ptr = (VOID *)&tx_app_byte_pool;
    status = App_ThreadX_Init(memory_ptr);
    if (status != TX_SUCCESS)
    {
      /* USER CODE BEGIN  App_ThreadX_Init_Error */
      while(1)
      {
      }
      /* USER CODE END  App_ThreadX_Init_Error */
    }
    /* USER CODE BEGIN  App_ThreadX_Init_Success */

    /* USER CODE END  App_ThreadX_Init_Success */

  }

  if (tx_byte_pool_create(&ux_host_app_byte_pool, "Ux App memory pool", ux_host_byte_pool_buffer, UX_HOST_APP_MEM_POOL_SIZE) != TX_SUCCESS)
  {
    /* USER CODE BEGIN TX_Byte_Pool_Error */

    /* USER CODE END TX_Byte_Pool_Error */
  }
  else
  {
    /* USER CODE BEGIN UX_HOST_Byte_Pool_Success */

    /* USER CODE END UX_HOST_Byte_Pool_Success */

    memory_ptr = (VOID *)&ux_host_app_byte_pool;
    status = MX_USBX_Host_Init(memory_ptr);
    if (status != UX_SUCCESS)
    {
      /* USER CODE BEGIN  MX_USBX_Host_Init_Error */
      while(1)
      {
      }
      /* USER CODE END  MX_USBX_Host_Init_Error */
    }
    /* USER CODE BEGIN  MX_USBX_Host_Init_Success */

    /* USER CODE END  MX_USBX_Host_Init_Success */
  }
  if (tx_byte_pool_create(&usbpd_app_byte_pool, "USBPD App memory pool", usbpd_byte_pool_buffer, USBPD_DEVICE_APP_MEM_POOL_SIZE) != TX_SUCCESS)
  {
    /* USER CODE BEGIN USBPD_Byte_Pool_Error */

    /* USER CODE END USBPD_Byte_Pool_Error */
  }
  else
  {
    /* USER CODE BEGIN USBPD_Byte_Pool_Success */

    /* USER CODE END USBPD_Byte_Pool_Success */

    memory_ptr = (VOID *)&usbpd_app_byte_pool;
    status = MX_USBPD_Init(memory_ptr);
    if (status != USBPD_OK)
    {
      /* USER CODE BEGIN  MX_USBPD_Init_Error */
      while(1)
      {
      }
      /* USER CODE END  MX_USBPD_Init_Error */
    }
    /* USER CODE BEGIN  MX_USBPD_Init */

    /* USER CODE END  MX_USBPD_Init */
  }
#else
/*
 * Using dynamic memory allocation requires to apply some changes to the linker file.
 * ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
 * using the "first_unused_memory" argument.
 * This require changes in the linker files to expose this memory location.
 * For EWARM add the following section into the .icf file:
     place in RAM_region    { last section FREE_MEM };
 * For MDK-ARM
     - either define the RW_IRAM1 region in the ".sct" file
     - or modify the line below in "tx_low_level_initilize.s to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|

 * For STM32CubeIDE add the following section into the .ld file:
     ._threadx_heap :
       {
          . = ALIGN(8);
          __RAM_segment_used_end__ = .;
          . = . + 64K;
          . = ALIGN(8);
        } >RAM_D1 AT> RAM_D1
    * The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
    * In the example above the ThreadX heap size is set to 64KBytes.
    * The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
    * Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
    * Read more in STM32CubeIDE User Guide, chapter: "Linker script".

 * The "tx_initialize_low_level.s" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.
 */

  /* USER CODE BEGIN DYNAMIC_MEM_ALLOC */
  (void)first_unused_memory;
  /* USER CODE END DYNAMIC_MEM_ALLOC */
#endif

}
