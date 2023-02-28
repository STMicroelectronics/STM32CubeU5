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
extern __IO TS_State_t TS_State;
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
    
  printf("\r\n -------- LED3 and LED4 Initialized --------");
  
  /* Init Leds */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  HAL_Delay(500);
  if (BSP_LED_On(LED3) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(500);
  if (BSP_LED_On(LED4) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  printf("\r\n -------- LED3 and LED4 Enabled --------");  
  
  /* Keep the LEDs on for two seconds */
  HAL_Delay(2000);
  
  /* Turn off the LEDs */ 
  HAL_Delay(500);
  if (BSP_LED_Off(LED3) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  if (BSP_LED_Off(LED4) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  printf("\r\n -------- LED3 and LED4 Disabled --------");
  
  HAL_Delay(2000);
  
  if (BSP_LED_On(LED3) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
  
  if (BSP_LED_On(LED4) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  HAL_Delay(500);
    
  /* For about five seconds, toggle all the LEDs */
  while (index < 25)
  {
    HAL_Delay(200);
    index++;
    if (BSP_LED_Toggle(LED3) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Toggle(LED4) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }
  printf("\r\n -------- LED3 and LED4 Toggled --------");
  
  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);  
  
  while ((TS_State.TouchX < 190));
  while ((TS_State.TouchX > 300));
  while ((TS_State.TouchY > 55));
  DrawBackround = 0;
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
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 170, (uint8_t *)"Test of LED", CENTER_MODE); 
  
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"Please check the result", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 250, (uint8_t *)"on IO Terminal", CENTER_MODE);
}
/**
  * @}
  */ 

/**
  * @}
  */ 
  

