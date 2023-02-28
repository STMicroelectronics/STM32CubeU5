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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO TS_State_t TS_State;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Lcd_SetHint(void);

/**
  * @brief  LCD demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Lcd_demo(void)
{
  int32_t   result = 0;
  uint32_t  Brightness;
  uint32_t  Format;
  uint32_t  Xsize = 480, Ysize = 480;
  uint32_t  Pixel;
  
  Lcd_SetHint();
      
  /* Set the brightness */
  if (BSP_LCD_SetBrightness(0, 100) != BSP_ERROR_FEATURE_NOT_SUPPORTED) result--;
  
  /* Get the brightness */
  if (BSP_LCD_GetBrightness(0, &Brightness) != BSP_ERROR_FEATURE_NOT_SUPPORTED) result--;
  
  /* Get the format */
  if (BSP_LCD_GetFormat(0, &Format) != BSP_ERROR_NONE) result--;
  if (Format != LCD_PIXEL_FORMAT_ARGB8888) result--;
  
  /* Get the display Xsize */
  if (BSP_LCD_GetXSize(0, &Xsize) != BSP_ERROR_NONE) result--;
  if (Xsize != 480U) result--;
  
  /* Get the display Ysize */
  if (BSP_LCD_GetYSize(0, &Ysize) != BSP_ERROR_NONE) result--;
  if (Ysize != 480U) result--;
  
  /* Set active layer */
  if (BSP_LCD_SetActiveLayer(0, 0) != BSP_ERROR_NONE) result--;
  
  /* Clean display */
  uint32_t i;
  uint32_t j;
  
  for (i = 0; i < Xsize; i++)
  {
    for (j = 0; j < Ysize; j++)
    {
      if (BSP_LCD_WritePixel(0, i, j, LCD_COLOR_BLACK) != BSP_ERROR_NONE) result--;
    }
  }
  
  
  /* Set the display on */
  if (BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE) result--;
  
  /* Display a brown rectangle pixel by pixel */
  for (i = 100; i < 240; i++)
  {
    for (j = 100; j < 160; j++)
    {
      if (BSP_LCD_WritePixel(0, i, j, LCD_COLOR_GREEN) != BSP_ERROR_NONE) result--;
    }
  }
  
  
  /* Get pixel */
  if (BSP_LCD_ReadPixel(0, 120, 121, &Pixel) != BSP_ERROR_NONE) result--;
  if (Pixel != LCD_COLOR_GREEN) result--;
  
  /* Display a red rectangle */
  if (BSP_LCD_FillRect(0, 110, 110, 120, 40, LCD_COLOR_RED) != BSP_ERROR_NONE) result--;
  
  
  /* Display a blue rectangle pixel by pixel */
  for (i = 120; i < 220; i++)
  {
    for (j = 120; j < 140; j++)
    {
      if (BSP_LCD_WritePixel(0, i, j, LCD_COLOR_BLUE) != BSP_ERROR_NONE) result--;
    }
  }

  
  /* Display a orange horizontal line */
  if (BSP_LCD_DrawHLine(0, 100, 200, 140, LCD_COLOR_ORANGE) != BSP_ERROR_NONE) result--;
  
  
  /* Display a orange vertical line */
  if (BSP_LCD_DrawVLine(0, 100, 200, 140, LCD_COLOR_ORANGE) != BSP_ERROR_NONE) result--;
  

  
  /* Display a orange horizontal line */
  if (BSP_LCD_DrawHLine(0, 100, 340, 140, LCD_COLOR_ORANGE) != BSP_ERROR_NONE) result--;
  
  
  /* Display a orange vertical line */
  if (BSP_LCD_DrawVLine(0, 240, 200, 140, LCD_COLOR_ORANGE) != BSP_ERROR_NONE) result--;
  
  
  /* Display a bitmap */
  if (BSP_LCD_DrawBitmap(0, 120, 235, (uint8_t *)stlogo) != BSP_ERROR_NONE) result--;
  
  
  /* Display a rectancle from RGB buffer */
  if (BSP_LCD_FillRGBRect(0, 250, 250, (uint8_t *)rect_buff, 16, 8) != BSP_ERROR_NONE) result--;
  
  
  HAL_Delay(1000);
  
  /* Set the display off */
  if (BSP_LCD_DisplayOff(0) != BSP_ERROR_NONE) result--;
  
  HAL_Delay(1000);
  
  /* Set the display on */
  if (BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE) result--;
    
  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);  
  
  while ((TS_State.TouchX < 190));
  while ((TS_State.TouchX > 300));
  while ((TS_State.TouchY > 55));
  DrawBackround = 0;
  return 0;
}

/**
  * @brief  Display lcd demo hint
  * @param  None
  * @retval None
  */
static void Lcd_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 170, (uint8_t *)"Test of LCD", CENTER_MODE); 
}

/**
  * @}
  */ 

/**
  * @}
  */ 



