/**
  ******************************************************************************
  * @file    Examples/BSP/Src/led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the
  *          STM32U599J_DK driver
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
int32_t Led_demo(void)
{
  uint32_t index = 0;

  /* Demo Hint */
  Led_SetHint();

  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"Initialize then turn on", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"each led after 500 ms", CENTER_MODE);

  /* Init Leds */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  HAL_Delay(500);
  if (BSP_LED_On(LED_GREEN) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  HAL_Delay(500);
  if (BSP_LED_On(LED_RED) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Keep the LEDs on for 2 seconds */
  HAL_Delay(2000);
  
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"   Turn off each led   ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"     after 500 ms     ", CENTER_MODE);

  /* Turn off the LEDs */
  HAL_Delay(500);
  if (BSP_LED_Off(LED_GREEN) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  if (BSP_LED_Off(LED_RED) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(2000);
  
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"   Turn on green led   ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"     after 500 ms     ", CENTER_MODE);
  
  HAL_Delay(500);

  if (BSP_LED_On(LED_GREEN) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Keep the LEDs on for 2 seconds */
  HAL_Delay(2000);
  
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"   Turn on red led    ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"     after 500 ms     ", CENTER_MODE);
  
  BSP_LED_Off(LED_GREEN);
  
  HAL_Delay(500);

  if (BSP_LED_On(LED_RED) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Keep the LEDs on for 2 seconds */
  HAL_Delay(2000);

  /* For about five seconds, toggle all the LEDs */
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"     Toggle all leds     ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"                         ", CENTER_MODE);

  while (index < 30)
  {
    HAL_Delay(200);
    index++;
    if (BSP_LED_Toggle(LED_GREEN) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Toggle(LED_RED) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }
  
  /* Turn off red led */
  BSP_LED_Off(LED_RED);

  return 0;

}

/**
  * @brief  Display led demo hint
  * @param  None
  * @retval None
  */
static void Led_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)"Test of LED", CENTER_MODE);
}
/**
  * @}
  */

/**
  * @}
  */


