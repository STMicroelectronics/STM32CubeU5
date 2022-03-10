/**
  ******************************************************************************
  * @file    camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera features of the
  *          b_u585i_iot02_camera driver
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

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CAMERA_FRAME_BUFFER     CameraBuf

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t CameraBuf[320*240/2];  
CAMERA_Capabilities_t Cap;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
int32_t Camera_demo(void)
{
  int32_t   result = 0;
  
  printf("\n******CAMERA EXAMPLE******\n");

  /* Initialize the Camera */
  if(BSP_CAMERA_Init(0, CAMERA_R320x240, CAMERA_PF_RGB565) != BSP_ERROR_NONE)
  {
    printf("VERIFY THAT CAMERA IS CONNECTED!\n");    
    printf("TEST ABORTED\n");

    return 0;
  }
  
  else
  {
    printf("CAMERA CONTINUOUS MODE\n");
    /* Wait for the camera initialization after HW reset*/
    HAL_Delay(100);

    if(BSP_CAMERA_GetCapabilities(0, &Cap) != BSP_ERROR_NONE) result--;

    /* Start the Camera Capture */
    if(BSP_CAMERA_Start(0, (uint8_t *)CameraBuf, CAMERA_MODE_CONTINUOUS)) result--;
    HAL_Delay(500);
  }

  printf("Camera is capturing and buffer is stored in CameraBuf\n");
  HAL_Delay(2000);
  
  BSP_CAMERA_Stop(0);
  BSP_CAMERA_DeInit(0);

 if (CheckResult() != 0U) result--;

  printf("Camera Example Ended\n");
  return result;
}

/**
  * @}
  */

/**
  * @}
  */
