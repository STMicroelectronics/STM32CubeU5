/**
  ******************************************************************************
  * @file    app_about.c
  * @author  MCD Application Team
  * @brief   Demonstration information slide
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
KMODULE_RETURN _UsbPdDemoExec(void);

void UsbPdDemo(void);
void UsbPdUserInformation(int16_t tempvalue);
void UsbPdUserHeader(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem UsbPdMenuItems[] =
{
    {NULL, 14, 30, TYPE_EXEC, MODULE_NONE, UsbPdDemo, NULL, NULL, NULL, NULL  },
};

const tMenu UsbPdMenu = {
  NULL, UsbPdMenuItems, countof(UsbPdMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleUsbPd =
{
  MODULE_USBPD,
  NULL,
  _UsbPdDemoExec,
  NULL,
  NULL
};

/**
  * @brief  Run the 8 uart application
  * @param  None.
  * @note   run and display information about the uart transaction.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _UsbPdDemoExec(void)
{
  /* Prepare the main MMI */
  kMenu_Execute(UsbPdMenu);

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
void UsbPdDemo(void)
{
  uint32_t joyState = JOY_NONE;

  user_action = 0;
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);

  UTIL_LCD_DisplayStringAt(0,20, (uint8_t *) "USB-PD Module", CENTER_MODE);
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0,80, (uint8_t *) "Not included in this version", CENTER_MODE);

                           
  UTIL_LCD_DisplayStringAt(0,200, (uint8_t *) "<< RETURN >>", CENTER_MODE);

    /* Polling on joystick event */
  while (joyState == JOY_NONE)
  {
    joyState = BSP_JOY_GetState(JOY1);
  }
  
  HAL_Delay(200);
}


