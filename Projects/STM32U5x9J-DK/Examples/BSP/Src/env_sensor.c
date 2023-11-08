/**
  ******************************************************************************
  * @file    env_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the environmental sensors feature
  *          in the stm32u5x9j_discovery_env_sensors driver
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
#define POLLING_PERIOD (500U) /* milliseconds */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void ES_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Env sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Es_demo(void)
{
  float                     value;
  uint16_t                  xpos;
  uint16_t                  ypos;
  uint8_t                   chipId;
  char                      str[10] = {0};

  ES_SetHint();

  /***********************/
  /* Test of Temperature */
  /***********************/
  /* Initialize function */
  if (BSP_ENV_SENSOR_Init(0, ENV_TEMPERATURE) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Read id function */
  if (BSP_ENV_SENSOR_ReadID(0, &chipId) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Enable function */
  if (BSP_ENV_SENSOR_Enable(0, ENV_TEMPERATURE) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  while (1)
  {
    xpos = x;
    ypos = y;

    /* GetValue function */
    if (BSP_ENV_SENSOR_GetValue(0, ENV_TEMPERATURE, &value) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    /* Display the Temperature value on the screen */
    (void) sprintf(str, "%.1f C", value);
    UTIL_LCD_DisplayStringAt(300, 235, (uint8_t *)str, LEFT_MODE);

    HAL_Delay(POLLING_PERIOD);

    if ((xpos > 190) & (xpos < 300) & (ypos < 55))
    {
      DrawBackround = 0;
      return 0;
    }
  }
  /******************/
  /* End of example */
  /******************/
}


/**
  * @brief  Display ES Demo Hint
  * @param  None
  * @retval None
  */
static void ES_SetHint(void)
{
  /* Clear the LCD */
  (void) BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);

  (void) BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 215, (uint8_t *)"Test of Environment Sensors", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(60, 235, (uint8_t *)"Temperature ", LEFT_MODE);
}

/**
  * @}
  */

/**
  * @}
  */

