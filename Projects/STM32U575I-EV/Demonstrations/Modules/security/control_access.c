/**
  ******************************************************************************
  * @file    control_accees.c
  * @author  MCD Application Team
  * @brief   Security Control Access implementation
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
#include "main.h"
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

/* Private function prototypes -----------------------------------------------*/
uint32_t Touch_Select(TS_State_t TS_State);
COM_InitTypeDef COM_Init;
/* Private Variable ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Control_Access_Demo(void)
{
  /* This a simulation of IPs control access feature  */
  /* You can refer to GTZC examples for the functional implementation*/
  uint8_t next = 0, sel = 0, selected = 0, exit = 0;
  uint8_t uart_sec = 0, led_sec = 0, uart_acces_sec =0, led_acces_sec =0;
  
  uint32_t joyState = JOY_NONE;

  BSP_LED_Init(LED5);
  

  COM_Init.BaudRate    = 115200;
  COM_Init.WordLength  = COM_WORDLENGTH_8B;
  COM_Init.StopBits    = COM_STOPBITS_1;
  COM_Init.Parity      = COM_PARITY_NONE;
  COM_Init.HwFlowCtl   = COM_HWCONTROL_NONE;
  BSP_COM_Init(COM1, &COM_Init);

  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Security Access", CENTER_MODE);
  
  
  /* Configuration Panel */
  UTIL_LCD_FillRect(2, 30, 316, 90,UTIL_LCD_COLOR_WHITE);  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PINK);
  UTIL_LCD_DisplayStringAt(4, 30, (uint8_t *)"Security Configuration", LEFT_MODE);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 60, (uint8_t *)"UART:   S          NS", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(4, 90, (uint8_t *)"LED :   S          NS", LEFT_MODE);  
  
  UTIL_LCD_DrawRect(140, 56, 120, 26,UTIL_LCD_COLOR_RED); 
  UTIL_LCD_FillRect(141, 57, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(200, 58, 58, 22,UTIL_LCD_COLOR_BLACK); 
  
  UTIL_LCD_DrawRect(140, 86, 120, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_FillRect(141, 87, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(200, 88, 58, 22,UTIL_LCD_COLOR_BLACK); 
  
  /* Execution Panel */ 
  UTIL_LCD_FillRect(2, 126, 316, 90,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PINK);
  UTIL_LCD_DisplayStringAt(4, 128, (uint8_t *)"Execution: Access Mode", LEFT_MODE);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 157, (uint8_t *)"UART:S        NS", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(4, 190, (uint8_t *)"LED :S        NS", LEFT_MODE);  
  
  UTIL_LCD_DrawRect(4, 150, 312, 30,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DrawRect(4, 184, 312, 30,UTIL_LCD_COLOR_BLACK);
  
  UTIL_LCD_DrawRect( 90, 152, 110, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_FillRect( 91, 153, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(144, 154,  53, 22,UTIL_LCD_COLOR_BLACK); 
  
  UTIL_LCD_DrawRect( 90, 186, 110, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_FillRect( 91, 187, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(144, 188,  53, 22,UTIL_LCD_COLOR_BLACK); 
  
  /* Buttons Print and Toggle */
  UTIL_LCD_DrawRect(240, 152, 70, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_FillRect(241, 153, 68, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(242, 154, 66, 22,UTIL_LCD_COLOR_BLACK); 
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(244, 159, (uint8_t *)"Print", LEFT_MODE);
  
  
  UTIL_LCD_DrawRect(240, 186, 70, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_FillRect(241, 187, 68, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(242, 188, 66, 22,UTIL_LCD_COLOR_BLACK);   
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(242, 192, (uint8_t *)"Toggle", LEFT_MODE);
  
  
  
  /* Log panel part */ 
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);   

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BROWN);
  UTIL_LCD_DisplayStringAt(60, 224, (uint8_t *)"Access Control Simulation ", LEFT_MODE);

            
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);
  
  
  do
  { 
    
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
    
    /* anti bounding assert */ 
    while( BSP_JOY_GetState(JOY1) != JOY_NONE);
    
    while(TS_State.TouchDetected != 0)
    {
      BSP_TS_GetState(0, &TS_State);
    }
    
    HAL_Delay(100);
    
    if (joyState == JOY_NONE)
    {
      
      selected = sel = Touch_Select(TS_State);
      
      if( sel != 10)
      {
        next = 1;
      }
      else
      {
        next = 0;
      }
      
      
    }
    else
    {
      
      next = 0;    
      
      switch(joyState)
      {
      case JOY_SEL :
        selected = sel;
        next = 1;
        
        break;
      case JOY_DOWN :
      case JOY_RIGHT :
        sel++;
        /* check if sel is on the latest line position */
        if(sel > 6)
        {
          sel = 0;
        }
        break;
      case JOY_UP :
      case JOY_LEFT:
        /** check if sel is on the first line */
        if (sel != 0)
        { 
          sel--;
        }
        else
        {
          sel = 6;
        }
        
        break;
      default:
        next = 0;
        break;
      }
    }
    
    UTIL_LCD_DrawRect(140, 56, 120, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect(140, 86, 120, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect( 90, 152, 110, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect( 90, 186, 110, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect(240, 152, 70, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect(240, 186, 70, 26,UTIL_LCD_COLOR_BLACK); 
    UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);  
    switch(sel)
    {
    case 0 :
      UTIL_LCD_DrawRect(140, 56, 120, 26,UTIL_LCD_COLOR_RED); 
      break;
    case 1 :
      UTIL_LCD_DrawRect(140, 86, 120, 26,UTIL_LCD_COLOR_RED); 
      break;
    case 2 :
      UTIL_LCD_DrawRect( 90, 152, 110, 26,UTIL_LCD_COLOR_RED); 
      break;
    case 3 :
      UTIL_LCD_DrawRect(240, 152, 70, 26,UTIL_LCD_COLOR_RED); 
      break;        
    case 4 :
      UTIL_LCD_DrawRect( 90, 186, 110, 26,UTIL_LCD_COLOR_RED); 
      break;
    case 5 :
      UTIL_LCD_DrawRect(240, 186, 70, 26,UTIL_LCD_COLOR_RED); 
      break;
    case 6 :
      UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_RED);   
      break;
    default:
      break;
    }
    
    
    if (next == 1)
    {
      switch(selected)
      {
      case 0 :
        {
          if (uart_sec == 1)
          {
            UTIL_LCD_FillRect(141, 57, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(200, 58, 58, 22,UTIL_LCD_COLOR_BLACK);
            uart_sec = 0;
          }
          else
          {

            UTIL_LCD_FillRect(141, 57, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(142, 58, 58, 22,UTIL_LCD_COLOR_BLACK);
            
            uart_sec = 1;
          }        
          
        }
        break;
      case 1 :
        {
          if (led_sec == 1)
          {
            UTIL_LCD_FillRect(141, 87, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(200, 88, 58, 22,UTIL_LCD_COLOR_BLACK);
            led_sec = 0;
          }
          else
          {
            UTIL_LCD_FillRect(141, 87, 118, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(142, 88, 58, 22,UTIL_LCD_COLOR_BLACK);
            led_sec = 1;
          }        
          
        }
        break;
      case 2 :
        {
          if (uart_acces_sec == 1)
          {
            UTIL_LCD_FillRect( 91, 153, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(144, 154,  53, 22,UTIL_LCD_COLOR_BLACK); 
            uart_acces_sec = 0;
          }
          else
          {
            UTIL_LCD_FillRect( 91, 153, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(92, 154,  53, 22,UTIL_LCD_COLOR_BLACK); 
            uart_acces_sec = 1;
          }        
          
        }
        break;
      case 3 :
        {
          UTIL_LCD_SetFont(&Font16);
          UTIL_LCD_FillRect(241, 153, 68, 24,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_FillRect(242, 154, 66, 22,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_SetFont(&Font16);
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_DisplayStringAt(244, 159, (uint8_t *)"Print", LEFT_MODE);
          HAL_Delay(100);
          
          UTIL_LCD_FillRect(241, 153, 68, 24,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_FillRect(242, 154, 66, 22,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_SetFont(&Font16);
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_DisplayStringAt(244, 159, (uint8_t *)"Print", LEFT_MODE);

          if (uart_acces_sec == 1)
          {

            if (uart_sec == 1)
            {
              printf("Autorized access from Secure Access to the UART device in Secure Configuration"); 
            }
            else
            {
              printf("Autorized access from Secure Access to the UART device in Non-Secure Configuration"); 
            }              
            
            UTIL_LCD_SetFont(&Font12);
            UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_GREEN_DARK);
            UTIL_LCD_DisplayStringAt(5, 224, (uint8_t *)" Access Authorized UART(115200/8/n/1/n)", LEFT_MODE);
          }
          else 
          {
            if (uart_sec == 0)
            {
              printf("Autorized access from Non-Secure Access to the UART device in Non-Secure Configuration\n"); 
              
              UTIL_LCD_SetFont(&Font12);
              UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
              UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_GREEN_DARK);
              UTIL_LCD_DisplayStringAt(5, 224, (uint8_t *)" Access Authorized UART(115200/8/n/1/n)", LEFT_MODE);
            }
            else
            {
              UTIL_LCD_SetFont(&Font12);
              UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
              UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
              UTIL_LCD_DisplayStringAt(5, 224, (uint8_t *)"    Access NOT Autorized UART(VCP)     ", LEFT_MODE);
 
            }              
          }

          
        }
        break;      
      case 4 :
        {
          if (led_acces_sec == 1)
          {
            UTIL_LCD_FillRect( 91, 187, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(144, 188,  53, 22,UTIL_LCD_COLOR_BLACK); 
            led_acces_sec = 0;
          }
          else
          {
            UTIL_LCD_FillRect( 91, 187, 108, 24,UTIL_LCD_COLOR_ST_YELLOW); 
            UTIL_LCD_FillRect(92, 188,  53, 22,UTIL_LCD_COLOR_BLACK); 
            led_acces_sec = 1;
          }        
          
        }
        break; 
      case 5 :
        {
          UTIL_LCD_SetFont(&Font16);
          UTIL_LCD_FillRect(241, 187, 68, 24,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_FillRect(242, 188, 66, 22,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_DisplayStringAt(242, 192, (uint8_t *)"Toggle", LEFT_MODE);
          HAL_Delay(100);
          
          UTIL_LCD_FillRect(241, 187, 68, 24,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_FillRect(242, 188, 66, 22,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_DisplayStringAt(242, 192, (uint8_t *)"Toggle", LEFT_MODE);
          if ((led_acces_sec == 1) ||(led_sec == 0))
          {
            BSP_LED_Toggle(LED5);
            UTIL_LCD_SetFont(&Font12);
            UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_GREEN_DARK);
            UTIL_LCD_DisplayStringAt(5, 224, (uint8_t *)"Access Authorized GREEN_LED/GPIO_PB7   ", LEFT_MODE);
          }
          else
          {
            UTIL_LCD_SetFont(&Font12);
            UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
            UTIL_LCD_DisplayStringAt(5, 224, (uint8_t *)"Access NOT Authorized GREEN_LED/PB7    ", LEFT_MODE);
            
          }
          
        }
        break; 
      case 6 :
        {
          UTIL_LCD_SetFont(&Font12);
          UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);
          HAL_Delay(100);
          
          UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
          UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK); 
          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);
          HAL_Delay(100);
          exit = 1;   
        }
        break; 
      default:
        break;
      }
      
    }
    
    
  } while (exit == 0); 
  
  
}

uint32_t Touch_Select(TS_State_t TS_State)
{
  
  if (((TS_State.TouchX > 140) && (TS_State.TouchX < 260)) && ((TS_State.TouchY > 56) && (TS_State.TouchY < 82)))
  {
    return (0);
  }
  
  if (((TS_State.TouchX > 140) && (TS_State.TouchX < 260)) && ((TS_State.TouchY > 86) && (TS_State.TouchY < 112)))
  {
    return (1);
  }
  
  if (((TS_State.TouchX > 90) && (TS_State.TouchX < 200)) && ((TS_State.TouchY > 152) && (TS_State.TouchY < 178)))
  {
    return (2);
  } 
  
  if (((TS_State.TouchX > 240) && (TS_State.TouchX < 310)) && ((TS_State.TouchY > 152) && (TS_State.TouchY < 178)))
  {
    return (3);
  }
  
  if (((TS_State.TouchX > 90) && (TS_State.TouchX < 200)) && ((TS_State.TouchY > 186) && (TS_State.TouchY < 212)))
  {
    return (4);
  }
  if (((TS_State.TouchX > 240) && (TS_State.TouchX < 310)) && ((TS_State.TouchY > 186) && (TS_State.TouchY < 200)))
  {
    return (5);
  }
  
  if (((TS_State.TouchX > 260) && (TS_State.TouchX < 320)) && ((TS_State.TouchY > 200) && (TS_State.TouchY < 240)))
  {
    return (6);
  }  
  
  
  return (10);
}


