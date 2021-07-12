/**
  ******************************************************************************
  * @file    BSP/Src/camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera features of the
  *          stm32u575i_eval_camera driver
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
static void Camera_SetHint(void);
uint32_t LightMode=0;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo(void)
{
  uint32_t y_size;

  BSP_LCD_GetYSize(0, &y_size);

  Camera_SetHint();
  CameraTest = 1;

  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetFont(&Font24);

  /* Initialize the Camera */
  if(BSP_CAMERA_Init(0, CAMERA_R320x240, CAMERA_PF_RGB565) != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(20, (y_size - 44), (uint8_t *)"VERIFY THAT CAMERA", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, (y_size - 24), (uint8_t *)"IS CONNECTED", LEFT_MODE);
  } 
  else
  {
    UTIL_LCD_DisplayStringAt(20, (y_size - 24), (uint8_t *)"CAMERA CONTINUOUS MODE", CENTER_MODE);
    /* Wait for the camera initialization after HW reset*/
    HAL_Delay(100);

    BSP_CAMERA_GetCapabilities(0, &Cap);

    /* Start the Camera Capture */
    BSP_CAMERA_Start(0, (uint8_t *)CameraBuf, CAMERA_MODE_CONTINUOUS);
    HAL_Delay(500);

    UserButtonPressed = RESET;
    while (1)
    {
      UTIL_LCD_FillRGBRect(0, 0, (uint8_t *)CameraBuf, 320, 240);
      BSP_CAMERA_EnableNightMode(0);
      if(UserButtonPressed == SET)
      {
        UserButtonPressed = RESET;
        CameraTest = 0;
        BSP_CAMERA_DeInit(0);
        return;
      }
    }
  }

  while(UserButtonPressed == RESET);

  UserButtonPressed = RESET;
  CameraTest = 0;
  BSP_CAMERA_DeInit(0);
  return;
}

/**
  * @brief  Display Camera demo hint
  * @param  None
  * @retval None
  */
static void Camera_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Camera Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press User button for next menu     ", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 130, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_ST_BLUE_DARK);
}

/**
  * @}
  */

/**
  * @}
  */
