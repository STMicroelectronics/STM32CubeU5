/**
  ******************************************************************************
  * @file    button.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the button feature in the 
  *          STM32U599J_DK driver
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
static void Button_SetHint(void);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Button example
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Button_demo(void)
{
  
  /* Demo Hint */
  Button_SetHint();
  
  UTIL_LCD_SetFont(&Font20);

  /* ------------------- */
  /* Button in GPIO mode */
  /* ------------------- */
  /* Initialize buttons */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Check buttons state */
  if (BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    Error_Handler();
  }
  
  /* Ask user to press buttons and check results */
  UTIL_LCD_DisplayStringAt(0, 190, (uint8_t *)"Please press User button (GPIO mode)", CENTER_MODE);
  while (BSP_PB_GetState(BUTTON_USER) != 1);
  
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"        User button pressed         ", CENTER_MODE);
  
  /* De-initialize buttons */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Wait 2s before next test */
  HAL_Delay(2000);
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"                                    ", CENTER_MODE);
  
  /* ------------------- */
  /* Button in EXTI mode */
  /* ------------------- */
  /* Initialize buttons */
  UserButtonPressed = RESET;
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Check buttons state */
  if (BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    Error_Handler();
  }
  
  /* Ask user to press buttons and check results */
  UTIL_LCD_DisplayStringAt(0, 190, (uint8_t *)"Please press User button (EXTI mode)", CENTER_MODE);
  while (UserButtonPressed == RESET);
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"        User button pressed         ", CENTER_MODE);
  
  /* Wait 2s before next test */
  HAL_Delay(2000);
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"                                    ", CENTER_MODE);
  UserButtonPressed = RESET;

  /* Re-ask user to press buttons and check results */
  UTIL_LCD_DisplayStringAt(0, 190, (uint8_t *)"Please press one more time User button", CENTER_MODE);
  while (UserButtonPressed == RESET);
  UTIL_LCD_DisplayStringAt(0, 210, (uint8_t *)"        User button pressed         ", CENTER_MODE);

  /* Wait 2s before next test */
  HAL_Delay(2000);
  UserButtonPressed = RESET;

  /* De-initialize buttons */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  BSP_LCD_DrawBitmap(0, 360, 0, (uint8_t *)back);

  while ((x < 380));
  while ((x > 440));
  while ((y > 57));
  DrawBackround = 0;

  return 0;
}

/**
  * @brief  Display led demo hint
  * @param  None
  * @retval None
  */
static void Button_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)"Test of BUTTON", CENTER_MODE);
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
