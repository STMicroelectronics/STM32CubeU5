/**
  ******************************************************************************
  * @file    app_about.c
  * @author  MCD Application Team
  * @brief   Shows the demonstration information
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
#define __APP_ABOUT_C

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32u5xx.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"

/* Private typedef ----------------------------------------------------------*/
/* Private constants ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _SystemInfoDemoExec(void);

void SystemInfoDemo(void);
void SystemInfoUserInformation(int16_t tempvalue);
void SystemInfoUserHeader(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem SystemInfoMenuItems[] =
{
    {NULL, 14, 30, TYPE_EXEC, MODULE_NONE, SystemInfoDemo, NULL, NULL, NULL, NULL  },
};

const tMenu SystemInfoMenu = {
  NULL, SystemInfoMenuItems, countof(SystemInfoMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleSystemInfo =
{
  MODULE_SYSINFO,
  NULL,
  _SystemInfoDemoExec,
  NULL,
  NULL
};

/**
  * @brief  Run the 8 uart application
  * @param  None.
  * @note   run and display information about the uart transaction.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _SystemInfoDemoExec(void)
{
  /* Prepare the main MMI */
  kMenu_Execute(SystemInfoMenu);

  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


/**
  * @brief  Run the image browser
  * @param  None.
  * @note   run and display image accordng the user action.
  * @retval None.
  */
void SystemInfoDemo(void)
{
  static  uint32_t joyState = JOY_NONE;
  static TS_State_t TS_State;
  user_action = 0;
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);

  UTIL_LCD_DisplayStringAt(0,1*Font24.Height, (uint8_t *)EVAL_BOARD, CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0,2*Font24.Height, (uint8_t *)DEMO_VERSION, CENTER_MODE);

  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);  
  UTIL_LCD_DisplayStringAt(0,5*Font20.Height, (uint8_t *)"Core: CM33", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0,6*Font20.Height, (uint8_t *)"Mem: 2MB/786KB", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0,7*Font20.Height, (uint8_t *)"Freq: 160Mhz", CENTER_MODE);

  UTIL_LCD_SetFont(&Font16);                         
  UTIL_LCD_DisplayStringAt(0,11*Font16.Height, (uint8_t *)DEMO_INFO1, CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0,12*Font16.Height, (uint8_t *)DEMO_INFO2, CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0,13*Font16.Height, (uint8_t *)DEMO_INFO3, CENTER_MODE);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(0,19*Font12.Height, (uint8_t *) "<< RETURN >>", CENTER_MODE);
  
  HAL_Delay(500);
  joyState = JOY_NONE;
  TS_State.TouchDetected=0;
  TS_State.TouchX=0;
  TS_State.TouchY=0;
  
  BSP_TS_GetState(0, &TS_State);
  BSP_TS_GetState(0, &TS_State);
  /* Polling on joystick event */
  while(( joyState == JOY_NONE) && (TS_State.TouchDetected == 0))
  {
    joyState = BSP_JOY_GetState(JOY1);
    BSP_TS_GetState(0, &TS_State);
  }
 
    HAL_Delay(200);
}


