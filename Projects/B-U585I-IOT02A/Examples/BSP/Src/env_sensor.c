/**
  ******************************************************************************
  * @file    env_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the environmental sensors feature
  *          in the b_u585i_iot02_env_sensors driver
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
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Env sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Es_demo(void)
{
  int32_t                   result  = 0;
  ENV_SENSOR_Capabilities_t Capabilities;
  uint8_t                   chipId;
  float                     outputDataRate;
  float                     value;

  printf("\n******ENV SENSOR******\n");
  printf("Press user button for next step\n");

  /* Initialize user button */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE) result--;

  /* -------------- */
  /* TEST of HTS221 */
  /* -------------- */

  /***********************/
  /* Test of Temperature */
  /***********************/
  printf("TEST OF HTS221 TEMPERATURE\n");

  /* Initialize function */
  if (BSP_ENV_SENSOR_Init(0, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;

  if (BSP_ENV_SENSOR_GetCapabilities(0, &Capabilities) != BSP_ERROR_NONE) result--;

  /* Read id function */
  if (BSP_ENV_SENSOR_ReadID(0, &chipId) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_ENV_SENSOR_Enable(0, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;
  
  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_TEMPERATURE, 12.5f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_TEMPERATURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 12.5f) result--;
 
  if (BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_TEMPERATURE, 7.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_TEMPERATURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 7.0f) result--;

  /* GetValue function */
  printf("Temperature values:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_ENV_SENSOR_GetValue(0, ENV_TEMPERATURE, &value) != BSP_ERROR_NONE) result--;
    printf("Temperature = %.1f\n", value);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_ENV_SENSOR_Disable(0, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;
  
  /* De-initialize function */
  if (BSP_ENV_SENSOR_DeInit(0) != BSP_ERROR_NONE) result--;

  /********************/
  /* Test of humidity */
  /********************/
  printf("TEST OF HTS221 HUMIDITY\n");

  /* Initialize function */
  if (BSP_ENV_SENSOR_Init(0, ENV_HUMIDITY) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_ENV_SENSOR_Enable(0, ENV_HUMIDITY) != BSP_ERROR_NONE) result--;
  
  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_HUMIDITY, 12.5f) != BSP_ERROR_NONE) result--;

  if (BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_HUMIDITY, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 12.5f) result--;
  if (BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_HUMIDITY, 7.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_HUMIDITY, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 7.0f) result--;

  /* GetValue function */
  printf("Humidity values:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_ENV_SENSOR_GetValue(0, ENV_HUMIDITY, &value) != BSP_ERROR_NONE) result--;
    printf("Humidity = %.1f\n", value);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_ENV_SENSOR_Disable(0, ENV_HUMIDITY) != BSP_ERROR_NONE) result--;
  
  /* De-initialize function */
  if (BSP_ENV_SENSOR_DeInit(0) != BSP_ERROR_NONE) result--;
  
  /* --------------- */
  /* TEST of LPS22HH */
  /* --------------- */

  /***********************/
  /* Test of Temperature */
  /***********************/
  printf("TEST OF LPS22HH TEMPERATURE\n");

  /* Initialize function */
  if (BSP_ENV_SENSOR_Init(1, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;

  /* Get capabilities */
  if (BSP_ENV_SENSOR_GetCapabilities(1, &Capabilities) != BSP_ERROR_NONE) result--;

  /* Read id function */
  if (BSP_ENV_SENSOR_ReadID(1, &chipId) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_ENV_SENSOR_Enable(1, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;
  
  /* GetOutputDataRate and SetOutputDataRate functions */

  if (BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_TEMPERATURE, 75.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_TEMPERATURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 75.0f) result--;
  if (BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_TEMPERATURE, 50.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_TEMPERATURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 50.0f) result--;

  /* GetValue function */
  printf("Temperature values:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_ENV_SENSOR_GetValue(1, ENV_TEMPERATURE, &value) != BSP_ERROR_NONE) result--;
    printf("Temperature = %.1f\n", value);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_ENV_SENSOR_Disable(1, ENV_TEMPERATURE) != BSP_ERROR_NONE) result--;
  
  /* De-initialize function */
  if (BSP_ENV_SENSOR_DeInit(1) != BSP_ERROR_NONE) result--;
  
  /********************/
  /* Test of pressure */
  /********************/
  printf("TEST OF LPS22HH PRESSURE\n");

  /* Initialize function */
  if (BSP_ENV_SENSOR_Init(1, ENV_PRESSURE) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_ENV_SENSOR_Enable(1, ENV_PRESSURE) != BSP_ERROR_NONE) result--;
  
  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_PRESSURE, 75.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_PRESSURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 75.0f) result--;

  if (BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_PRESSURE, 50.0f) != BSP_ERROR_NONE) result--;
  if (BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_PRESSURE, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 50.0f) result--;

  /* GetValue function */
  printf("Pressure values:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_ENV_SENSOR_GetValue(1, ENV_PRESSURE, &value) != BSP_ERROR_NONE) result--;
    printf("Pressure = %.1f\n", value);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_ENV_SENSOR_Disable(1, ENV_PRESSURE) != BSP_ERROR_NONE) result--;
  
  /* De-initialize function */
  if (BSP_ENV_SENSOR_DeInit(1) != BSP_ERROR_NONE) result--;

  /***************/
  /* End of example */
  /***************/

  /* De-Initialize User Button */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE) result--;

  printf("Environmental Sensors Example Ended\n");
  return result;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
