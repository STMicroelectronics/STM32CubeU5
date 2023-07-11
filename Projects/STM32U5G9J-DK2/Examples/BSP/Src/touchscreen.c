/**
  ******************************************************************************
  * @file    Examples/BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen BSP driver.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"

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
static void Touchscreen_SetHint(void);
uint16_t xpos =0 ;
uint16_t ypos =0 ;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
int32_t Touchscreen_demo(void)
{
  /* Demo Hint */
  Touchscreen_SetHint();

  while (1)
  {
    xpos = x;
    ypos = y;
    
    if ((xpos > 200) & (xpos < 585) & (ypos > 150) & (ypos < 435))
    {
      UTIL_LCD_FillCircle(xpos, ypos, 10, UTIL_LCD_COLOR_RED);
      xpos = 0;
      ypos = 0;
    }
    else if ((xpos > 380) & (xpos < 440) & (ypos < 57))
    {
      DrawBackround = 0;
      return 0;
    }
  }
}

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_DrawBitmap(0, 380, 0, (uint8_t *)back);

  BSP_LCD_FillRect(0, 200, 150, 400, 300, LCD_COLOR_WHITE);

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 100, (uint8_t *)"Test of", CENTER_MODE);

  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"TouchScreen", CENTER_MODE);

  x = 0;
  y = 0;

  HAL_Delay(500);
}

/**
  * @}
  */

/**
  * @}
  */


