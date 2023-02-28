/**
  ******************************************************************************
  * @file    ranging_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the ranging sensor feature in the
  *          b_u585i_iot02_ranging_sensors  driver
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
/* Private define ------------------------------------------------------------*/
#define POLLING_PERIOD (100U) /* milliseconds */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;
static RANGING_SENSOR_ITConfig_t Profile_IT;

/* Private function prototypes -----------------------------------------------*/
static void ToF_SetHint(void);

/**
  * @brief  Ranging sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Rs_demo(void)
{
  int32_t                   result  = 0;
  uint32_t                  chipId;
  RANGING_SENSOR_Result_t   distance;
  int32_t object_detected = 0;
  char str[10];
         
  ToF_SetHint();
  
  /* Initialize user button */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE) result--;

  if (BSP_RANGING_SENSOR_Init(VL53L5A1_DEV_CENTER) != BSP_ERROR_NONE) result--;
  
  /* set the profile if different from default one */
  if (BSP_RANGING_SENSOR_ConfigProfile(VL53L5A1_DEV_CENTER, &Profile) != BSP_ERROR_NONE) result--;  
  if (BSP_RANGING_SENSOR_ReadID(VL53L5A1_DEV_CENTER, &chipId) != BSP_ERROR_NONE) result--;
  if (BSP_RANGING_SENSOR_GetCapabilities(VL53L5A1_DEV_CENTER, &Cap) != BSP_ERROR_NONE) result--;

  Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
  Profile.TimingBudget = 30; /* 5 ms < TimingBudget < 100 ms */
  Profile.Frequency = 5; /* Hz */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  Profile_IT.Criteria = RS_IT_DEFAULT;
  Profile_IT.HighThreshold = 10;
  Profile_IT.LowThreshold =  200;
        
  /* set the profile if different from default one */
  if (BSP_RANGING_SENSOR_ConfigProfile(VL53L5A1_DEV_CENTER, &Profile) != BSP_ERROR_NONE) result--;    
  if (BSP_RANGING_SENSOR_ConfigIT(VL53L5A1_DEV_CENTER, &Profile_IT) != BSP_ERROR_NONE) result--; 
  if (BSP_RANGING_SENSOR_Start(VL53L5A1_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS) != BSP_ERROR_NONE)  result--;

  /* GetDistance function */
  while (1)
  {
    if (BSP_RANGING_SENSOR_GetDistance(VL53L5A1_DEV_CENTER, &distance) != BSP_ERROR_NONE) result--;
    
    if(distance.ZoneResult[0].Distance[0] < 150)
    {
      object_detected++;
    }
    sprintf(str, "%d", object_detected);
    UTIL_LCD_DisplayStringAt(0, 300, (uint8_t *)"detected objects", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 320, (uint8_t *)str, CENTER_MODE);
    
    HAL_Delay(POLLING_PERIOD);
    if ((x > 100) & (x < 300) & (y > 100) & (y < 300))
    {
      x = 0;
      y = 0;         
    }
    else if ((x > 190) & (x < 300) & (y < 55))
    {
      DrawBackround = 0;
      return 0;
    } 
  }
}


/**
  * @brief  Display ToF Demo Hint
  * @param  None
  * @retval None
  */
static void ToF_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);
  
  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 215, (uint8_t *)"Test of ToF", CENTER_MODE); 
  
  UTIL_LCD_DisplayStringAt(0, 235, (uint8_t *)"Ranging Sensor Init ...", CENTER_MODE); 
  
  HAL_Delay(500);

}

/**
  * @}
  */

/**
  * @}
  */

