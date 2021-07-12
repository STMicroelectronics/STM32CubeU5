/**
  ******************************************************************************
  * @file    Examples/BSP/Src/led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the 
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
static void Led_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Led demo
  * @param  None
  * @retval None
  */
void Led_demo(void)
{
  uint32_t index = 0;
  
  Led_SetHint();
  
  /* Init and turn on LED 5 to 8  */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"Initialize then turn on", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"each led after 500 ms", CENTER_MODE);

  HAL_Delay(500);
  if (BSP_LED_On(LED5) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(500);
  if (BSP_LED_On(LED6) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(500);
  if (BSP_LED_On(LED7) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(500);
  if (BSP_LED_On(LED8) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Keep the LEDs on for two seconds */
  HAL_Delay(2000);
  
  /* Turn off the LEDs */ 
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"   Turn off each led   ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"     after 500 ms     ", CENTER_MODE);
  HAL_Delay(500);
  if (BSP_LED_Off(LED5) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  if (BSP_LED_Off(LED6) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  if (BSP_LED_Off(LED8) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  if (BSP_LED_Off(LED7) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(2000);
  
  /* After two seconds, turn on LED5, LED7, LED6 and LED8 */
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"     Turn on led 5     ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"                       ", CENTER_MODE);
  if (BSP_LED_On(LED5) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"     Turn on led 7     ", CENTER_MODE);
  if (BSP_LED_On(LED7) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"     Turn on led 6     ", CENTER_MODE);
  if (BSP_LED_On(LED6) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"     Turn on led 8     ", CENTER_MODE);
  if (BSP_LED_On(LED8) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  
  /* For about five seconds, toggle all the LEDs */
  UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"     Toggle all leds     ", CENTER_MODE);
  while (index < 25)
  {
    HAL_Delay(200);
    index++;
    if (BSP_LED_Toggle(LED5) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Toggle(LED6) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Toggle(LED8) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Toggle(LED7) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Display Led demo hint
  * @param  None
  * @retval None
  */
static void Led_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Led Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Led", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"initialize and turn on, turn off", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"and toggle the leds", CENTER_MODE);
  
  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);
}
/**
  * @}
  */ 

/**
  * @}
  */ 
  
