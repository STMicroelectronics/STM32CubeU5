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
    if ((x > 100) & (x < 335) & (y > 100) & (y < 285))
    { 
      BSP_LCD_FillRect(0, x, y, 15, 15, LCD_COLOR_DARKRED);
      x = 0;
      y = 0;         
    }
    else if ((x > 190) & (x < 300) & (y < 55))
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
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);
  
  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);

  BSP_LCD_FillRect(0, 100, 100, 254, 200, LCD_COLOR_WHITE);
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 315, (uint8_t *)"Test of", CENTER_MODE); 
  
  UTIL_LCD_DisplayStringAt(0, 335, (uint8_t *)"TouchScreen", CENTER_MODE); 
  
  HAL_Delay(500);

}

/**
  * @}
  */

/**
  * @}
  */


