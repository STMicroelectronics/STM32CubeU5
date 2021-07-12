/**
  ******************************************************************************
  * @file    app_cameraviwer.c
  * @author  MCD Application Team
  * @brief   Camera viewer implementation
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
#include <stdio.h>
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_lcd.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_camera.h"
#include "app_cameraviewer.h"
#include "k_config.h"
#include "k_menu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CAMERA_FRAME_BUFFER     CameraBuf

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CameraBrowserdemo(void);
KMODULE_RETURN _CameraBrowserDemoExec(void);
void CameraBrowserMenuUserAction(uint8_t sel);
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma location=0x20030000
#elif defined(__ARMCC_VERSION) ||defined(__GNUC__)
__attribute__((section("CameraBufsection")))
#endif
uint32_t CameraBuf[320*240/2]; 

CAMERA_Capabilities_t Cap;

const tMenuItem CameraBrowserMenuMenuItems[] =
{
    {NULL, 14, 30, TYPE_EXEC, MODULE_NONE, CameraBrowserdemo, CameraBrowserMenuUserAction, NULL, NULL, NULL  },
};

const tMenu CameraBrowserMenu = {
  NULL, CameraBrowserMenuMenuItems, countof(CameraBrowserMenuMenuItems), TYPE_EXEC, 1, 1 };

const K_ModuleItem_Typedef ModuleCamera =
{
  MODULE_CAMERA,
  NULL,
  _CameraBrowserDemoExec,
  NULL,
  NULL
};

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Run the 8 uart application
  * @param  None.
  * @note   run and display information about the uart transaction.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _CameraBrowserDemoExec(void)
{
  /* Prepare the main MMI */
  kMenu_Execute(CameraBrowserMenu);

  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}

/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void CameraBrowserdemo(void)
{
  uint32_t y_size;

  /* Configure TS interrupt */
  if (BSP_TS_EnableIT(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  BSP_LCD_GetYSize(0, &y_size);

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
    /* Start the Camera Capture */
    BSP_CAMERA_Start(0, (uint8_t *)CameraBuf, CAMERA_MODE_CONTINUOUS);
    HAL_Delay(500);

    user_action = 0;
    while (user_action == 0)
    {
      UTIL_LCD_FillRGBRect(0, 0, (uint8_t *)CameraBuf, 320, 240);
    }
  }

  user_action = 0;
  BSP_CAMERA_Stop(0);
  BSP_CAMERA_DeInit(0);

  /* Disable TS interrupt */
  if (BSP_TS_DisableIT(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  return;
}

/**
  * @brief  Get User action
  * @param  sel   User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.
  * @retval None
  */
void CameraBrowserMenuUserAction(uint8_t sel)
{
  if (user_action == 0)
  {
    user_action = 1;
    user_event = sel;
  }
}
