/**
  ******************************************************************************
  * @file    Examples/BSP/Src/lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the LCD feature in the
  *          STM32U599J_DK discovery driver
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
#include "stlogo.h"
#include "rect_buff.h"
#include "stm32_lcd.h"
/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM     6
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Lcd_SetHint(uint32_t xpos, uint32_t ypos);
static void ReadPixelTest(void);
static void LCD_Show_Feature(void);
/**
  * @brief  LCD demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Lcd_demo(void)
{
  /* Display Demo description */
  Lcd_SetHint(40,60);

  LCD_Show_Feature ();

  /* Display Demo description */
  Lcd_SetHint(200,220);

  BSP_LCD_DrawBitmap(0, 380, 0, (uint8_t *)back);

  while ((x < 380));
  while ((x > 440));
  while ((y > 57));
  DrawBackround = 0;

  return 0;
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(void)
{
  Point Points2[3];
  uint32_t x_size, y_size;
  uint32_t i;
  uint32_t j;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  Points2[0].X = x_size - 400;
  Points2[0].Y = 200;
  Points2[1].X = x_size - 300;
  Points2[1].Y = 200;
  Points2[2].X = x_size - 300;
  Points2[2].Y = 300;

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(12, 110, x_size - 24, y_size- 104, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);

  /* Text Feature */
  UTIL_LCD_DisplayStringAt(14, 120, (uint8_t *)"Left aligned Text", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(0, 125, (uint8_t *)"Center aligned Text", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(14, 140, (uint8_t*)"Right aligned Text", RIGHT_MODE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_DisplayStringAt(x_size/2 -20, 180, (uint8_t *)"Font20", LEFT_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(x_size - 80, 184, (uint8_t *)"Font16", LEFT_MODE);

  HAL_Delay(2000);
  UTIL_LCD_FillRect(12, 110, x_size - 24, y_size- 104, UTIL_LCD_COLOR_WHITE);

  /* Draw misc. Shapes */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DrawRect(20, 120, 60 , 40, UTIL_LCD_COLOR_BLACK);
  BSP_LCD_FillRect(0, 100, 120, 60 , 40, UTIL_LCD_COLOR_BLACK);

  UTIL_LCD_DrawCircle(x_size - 120, 140, 20, UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_FillCircle(x_size - 40, 140, 20, UTIL_LCD_COLOR_GRAY);

  //UTIL_LCD_FillPolygon(Points, 8, UTIL_LCD_COLOR_GREEN);

  UTIL_LCD_DrawEllipse(130, 190, 30, 20, UTIL_LCD_COLOR_RED);
  UTIL_LCD_FillEllipse(200, 290, 50, 20, UTIL_LCD_COLOR_RED);

  UTIL_LCD_DrawLine (100, y_size - 20, 230, y_size- 50, UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DrawLine (100, y_size- 50, 230, y_size- 20, UTIL_LCD_COLOR_BLACK);

  UTIL_LCD_DrawPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);
  HAL_Delay(2000);
  UTIL_LCD_FillRect(12, 110, x_size - 24, y_size- 104, UTIL_LCD_COLOR_WHITE);


  /* Display a brown rectangle pixel by pixel */
  for (i = 200; i < 340; i++)
  {
    for (j = 200; j < 260; j++)
    {
      BSP_LCD_WritePixel(0, i, j, LCD_COLOR_GREEN);
    }
  }

  /* Display a red rectangle */
  BSP_LCD_FillRect(0, 210, 210, 120, 40, LCD_COLOR_RED);

  /* Display a blue rectangle pixel by pixel */
  for (i = 220; i < 220; i++)
  {
    for (j = 220; j < 140; j++)
    {
      BSP_LCD_WritePixel(0, i, j, LCD_COLOR_BLUE);
    }
  }

  BSP_LCD_DrawHLine(0, 500, 200, 140, LCD_COLOR_ORANGE);
  BSP_LCD_DrawVLine(0, 500, 200, 140, LCD_COLOR_ORANGE);
  BSP_LCD_DrawHLine(0, 500, 340, 140, LCD_COLOR_ORANGE);
  BSP_LCD_DrawVLine(0, 640, 200, 140, LCD_COLOR_ORANGE);
  BSP_LCD_DrawBitmap(0, 520, 235, (uint8_t *)stlogo);

  /* Display a rectancle from RGB buffer */
  BSP_LCD_FillRGBRect(0, 450, 250, (uint8_t *)rect_buff, 16, 8);
  HAL_Delay(500);

  /* LCD Read Pixel */
  ReadPixelTest();
}

/**
  * @brief  Display lcd demo hint
  * @param  None
  * @retval None
  */
static void Lcd_SetHint(uint32_t xpos, uint32_t ypos)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);

  /* Display text */
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, xpos, (uint8_t *)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, ypos, (uint8_t *)"LCD Features", CENTER_MODE);
}


/**
  * @brief  LCD Read Pixel test
  * @param  None
  * @retval None
  */
static void ReadPixelTest(void)
{
  uint16_t i, j, k;
  uint32_t x_size, y_size, read_pixel;
  uint32_t colors[18] = {UTIL_LCD_COLOR_CYAN, UTIL_LCD_COLOR_MAGENTA, UTIL_LCD_COLOR_YELLOW,
  UTIL_LCD_COLOR_LIGHTBLUE, UTIL_LCD_COLOR_LIGHTGREEN, UTIL_LCD_COLOR_LIGHTRED, UTIL_LCD_COLOR_LIGHTMAGENTA,
  UTIL_LCD_COLOR_LIGHTYELLOW, UTIL_LCD_COLOR_DARKBLUE, UTIL_LCD_COLOR_DARKGREEN, UTIL_LCD_COLOR_DARKRED, UTIL_LCD_COLOR_DARKCYAN,
  UTIL_LCD_COLOR_DARKMAGENTA, UTIL_LCD_COLOR_DARKYELLOW, UTIL_LCD_COLOR_LIGHTGRAY, UTIL_LCD_COLOR_DARKGRAY,
  UTIL_LCD_COLOR_BROWN, UTIL_LCD_COLOR_ORANGE };


  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  for(k = 0; k < 18; k++)
  {
    BSP_LCD_FillRect(0, 12, 110,  x_size - 24, y_size-104, colors[k]);
    for(j = 110; j < (y_size-104); j++)
    {
      for(i = 12; i < (x_size - 24); i++)
      {
        UTIL_LCD_GetPixel(i,j, &read_pixel);
        if(read_pixel != colors[k])
        {
          BSP_LED_On(LED_RED);
        }
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */



