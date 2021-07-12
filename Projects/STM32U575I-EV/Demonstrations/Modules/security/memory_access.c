/**
  ******************************************************************************
  * @file    memory_access.c
  * @author  MCD Application Team
  * @brief   Security Memory Acceess implementation
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
#include <string.h>
#include <stdio.h>
#include "stm32u5xx_hal.h"
#include <stdio.h>
#include "stm32u5xx.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "app_security.h"


/* Private typedef ----------------------------------------------------------*/

/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
static TS_State_t TS_State;

#if defined ( __ICCARM__ )
#pragma location=0x20000000
#elif defined(__ARMCC_VERSION) ||defined(__GNUC__)
__attribute__((section("DrawBuffersection")))
#endif
uint8_t DrawBuffer[320*240];

/* Private function prototypes -----------------------------------------------*/
/* Private Variable ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Memory_Access_Demo(void)
{
  /* This a simulation of IPs control access feature  */
  /* You can refer to GTZC examples for the functional implementation*/
  char tmp[30];
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Memory Access", CENTER_MODE);
  
  
  /* Drawing Panel */
  UTIL_LCD_FillRect(2, 30, 316, 186,UTIL_LCD_COLOR_WHITE);  
  
  /* Forbidden Panel */
  UTIL_LCD_FillRect(2, 100, 316, 60,UTIL_LCD_COLOR_RED); 
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DisplayStringAt(2, 120, (uint8_t *)"Forbidden Zone", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(2, 100, (uint8_t *)"0x20007D00", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(2, 148, (uint8_t *)"0x2000C800", LEFT_MODE);
  
  
  /* Log Panel */
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);   

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(50, 224, (uint8_t *) " Draw with your Finger ", LEFT_MODE);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);
  
  do {
    BSP_TS_GetState(0, &TS_State); 
    while(TS_State.TouchDetected == 0) 
    { 
      if( BSP_JOY_GetState(JOY1) != JOY_NONE)
      {
        while ( BSP_JOY_GetState(JOY1) != JOY_NONE);
        return; 
      }
         
      BSP_TS_GetState(0, &TS_State);
    }
    
    if ((TS_State.TouchY > 100) && (TS_State.TouchY < 160)) 
    {
      /* Illegal access */
     
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      sprintf(tmp,"Illegal Access:   0x%p ",&DrawBuffer[(320* TS_State.TouchY)+TS_State.TouchX]);
      UTIL_LCD_DisplayStringAt(40, 224, (uint8_t *)tmp, LEFT_MODE);
      
      DrawBuffer[(320* TS_State.TouchY)+TS_State.TouchX] = 0xA5; 
      
    }
    else if (((TS_State.TouchY > 30) && (TS_State.TouchY < 92)) || ((TS_State.TouchY > 168) && (TS_State.TouchY < 210)) )
    {
      /* Autorized access */

      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      sprintf(tmp,"Autorized Access: 0x%p ",&DrawBuffer[(320* TS_State.TouchY)+TS_State.TouchX]);
      UTIL_LCD_DisplayStringAt(40, 224, (uint8_t *)tmp, LEFT_MODE);

      DrawBuffer[(320* TS_State.TouchY)+TS_State.TouchX] = 0xA5; 
      UTIL_LCD_FillCircle(TS_State.TouchX,TS_State.TouchY,8,UTIL_LCD_COLOR_BLACK); 
      
    }
    
  } while ((( TS_State.TouchX < 270) || (TS_State.TouchY < 200)) && ( BSP_JOY_GetState(JOY1) == JOY_NONE));
  
                                                        
                                                        
}
                                                        
                                                        
                                                        
                                                        
                                                        