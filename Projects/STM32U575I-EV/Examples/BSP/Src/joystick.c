/**
  ******************************************************************************
  * @file    Examples/BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *          STM32U575I-EV driver
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick demo
  * @param  None
  * @retval None
  */
void Joystick_demo(void)
{ 
  static uint16_t xPtr = 156;
  static uint16_t yPtr = 154;
  static uint16_t old_xPtr = 156;
  static uint16_t old_yPtr = 154;  

  Joystick_SetHint();
  
  /* Initial cursor display in the center */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
    
  UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');   
  
  while (1)
  {
    /* Initial Joystick state set following MFX interrupt */
    if(JoyPinPressed != JOY_NONE)
    {
      /* Wait 1 ms to manage debounce */
      HAL_Delay(1);

      switch(JoyPinPressed)
      {
      case JOY_UP:
        if(yPtr > 92)
        {
          yPtr--;
        }
        break;     
      case JOY_DOWN:
        if(yPtr < 217)
        {
          yPtr++;
        }
        break;          
      case JOY_LEFT:
        if(xPtr > 12)
        {
          xPtr--;
        }
        break;         
      case JOY_RIGHT:
        if(xPtr < 301)
        {
          xPtr++;
        }
        break;                 
      default:
        break;           
      }
    
      if(JoyPinPressed == JOY_SEL)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');
      }
      else
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DisplayChar(old_xPtr, old_yPtr, 'X');
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayChar(xPtr, yPtr, 'X');

        old_xPtr = xPtr;
        old_yPtr = yPtr;
      }
      
      JoyPinPressed = JOY_NONE;
    }
    
    if(UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;

      return;
    }
  }
}


/**
  * @brief  Display joystick demo hint
  * @param  None
  * @retval None
  */
static void Joystick_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK); 
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Joystick", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the joystick", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"to move and color the X mark", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press User button to switch to next menu", CENTER_MODE);
  
  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);
}
/**
  * @}
  */ 

/**
  * @}
  */ 
  
