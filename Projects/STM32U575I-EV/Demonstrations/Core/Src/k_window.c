/**
  ******************************************************************************
  * @file    k_window.c
  * @author  MCD Application Team
  * @brief   This file contains the Hex dumps of the images available
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
#include "stm32u575i_eval_lcd.h"
#include "stm32_lcd.h"
#include "k_window.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Handle a window display.
  * @param  title to display a message, \n is used for multiple line.
  * @param  title_tc : title text color.
  * @param  title_bc : title background color.
  * @param  Msg to display a message, \n is used for multiple line.
  * @param  msg_tc : title text color.
  * @param  msg_bc : title background color.
  * @retval None
  */
void kWindow_Popup(char *title, uint32_t title_tc, uint32_t title_bc ,char *Msg, uint32_t msg_tc, uint32_t msg_bc )
{
  uint32_t pXSize, pYSize;
  uint8_t substring[25];
  uint8_t lineindex;
  uint16_t index,subindex;

  /* Clear the LCD Screen */
  UTIL_LCD_Clear(title_bc);
  UTIL_LCD_SetBackColor(title_bc);
  UTIL_LCD_SetTextColor(title_tc);

  /* Set the Back Color */
  UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
  UTIL_LCD_SetTextColor(title_tc);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)title, CENTER_MODE);

  /* Prepare the msg area */
  UTIL_LCD_SetBackColor(msg_bc);
  UTIL_LCD_SetTextColor(msg_bc);
  BSP_LCD_GetXSize(0, &pXSize);
  BSP_LCD_GetYSize(0, &pYSize);
  UTIL_LCD_FillRect(0, UTIL_LCD_DEFAULT_FONT.Height, pXSize, pYSize - UTIL_LCD_DEFAULT_FONT.Height, msg_bc);
  UTIL_LCD_SetTextColor(msg_tc);

  lineindex = subindex = index = 0;
  do
  {
    substring[subindex]=Msg[index];
    if((Msg[index] == '\n') || (Msg[subindex] == '\0'))
    {
      substring[subindex] = '\0';
      UTIL_LCD_DisplayStringAt(0, (2+lineindex) * UTIL_LCD_DEFAULT_FONT.Height, substring, CENTER_MODE);
      lineindex++;
      subindex = 0;
    }
    else
    {
      subindex++;
    }

    if(Msg[index] != '\0')
    {
      index++;
    }
  }
  while(Msg[index] != '\0');

}

/**
  * @brief  Handle a window display.
  * @param  msg   pointer to the message to display, \n is used for multiple line.
  * @retval None
  */
void kWindow_Error(char *msg)
{
  kWindow_Popup("Error popup", UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_RED,msg, UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_RED);
}

