/**
  ******************************************************************************
  * @file    Examples/BSP/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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
#define LCD_FEATURES_NUM        3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature;

/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo(void)
{
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature);

  while (1)
  {
    if (UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;

      if (++LCD_Feature < LCD_FEATURES_NUM)
      {
        LCD_Show_Feature(LCD_Feature);
      }
      else
      {
        return;
      }
    }
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use User push-button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"to display next page", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[] = {{20, 150}, {80, 150}, {80, 200}};
  Point Points2[] = {{100, 150}, {160, 150}, {160, 200}};

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(12, 92, 296, 136, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);

  switch (feature)
  {
    case 0:
      /* Text Feature */

      UTIL_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(14, 130, (uint8_t *)"Right aligned Text", RIGHT_MODE);
      UTIL_LCD_SetFont(&Font24);
      UTIL_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
      UTIL_LCD_SetFont(&Font20);
      UTIL_LCD_DisplayStringAt(140, 180, (uint8_t *)"Font20", LEFT_MODE);
      UTIL_LCD_SetFont(&Font16);
      UTIL_LCD_DisplayStringAt(240, 184, (uint8_t *)"Font16", LEFT_MODE);
      break;

    case 1:

      /* Draw misc. Shapes */
      UTIL_LCD_DrawRect(20, 100, 60 , 40, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_FillRect(100, 100, 60 , 40, UTIL_LCD_COLOR_BLACK);

      UTIL_LCD_DrawCircle(200, 120, 20, UTIL_LCD_COLOR_GRAY);
      UTIL_LCD_FillCircle(280, 120, 20, UTIL_LCD_COLOR_GRAY);

      UTIL_LCD_DrawPolygon(Points, 3, UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);

      UTIL_LCD_DrawEllipse(200, 170, 30, 20, UTIL_LCD_COLOR_RED);
      UTIL_LCD_FillEllipse(270, 170, 30, 20, UTIL_LCD_COLOR_RED);

      UTIL_LCD_DrawHLine(20, 210, 64, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawLine (170, 220, 300, 190, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawLine (170, 190, 300, 220, UTIL_LCD_COLOR_BLACK);
      break;

    case 2:
      /* Draw Bitmap */
      UTIL_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(120, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(220, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(20, 160, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(120, 160, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(220, 160, (uint8_t *)stlogo);
      HAL_Delay(100);
      break;
    default :
      break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

