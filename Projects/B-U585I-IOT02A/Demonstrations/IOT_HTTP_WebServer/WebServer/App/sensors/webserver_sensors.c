/**
  **********************************************************************************************************************
  * @file    webserver_sensors.c
  * @author  MCD Application Team
  * @brief   This file implements the web server sensors services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver_sensors.h"
#include "b_u585i_iot02a_env_sensors.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  Start temperature, pressure and humidity sensors acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_sensors_start(void)
{
  int   status = BSP_ERROR_NONE;

  /* De-Initialize function */
  status |= BSP_ENV_SENSOR_DeInit(0);
  status |= BSP_ENV_SENSOR_DeInit(1);

  /* Start sensors */
  status |= webserver_press_sensor_start();
  status |= webserver_humid_sensor_start();
  status |= webserver_temp_sensor_start();

  return status;
}

/**
  * @brief  Start temperature sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_temp_sensor_start(void)
{
  float outputDataRate = 0;
  int   status         = BSP_ERROR_NONE;

  /* Initialize function */
  status |= BSP_ENV_SENSOR_Init(0, ENV_TEMPERATURE);

  /* Enable function */
  status |= BSP_ENV_SENSOR_Enable(0, ENV_TEMPERATURE);

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_TEMPERATURE, 12.5f);
  status |= BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_TEMPERATURE, &outputDataRate);
  if (outputDataRate != 12.5f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_TEMPERATURE, 7.0f);
  status |= BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_TEMPERATURE, &outputDataRate);
  if (outputDataRate != 7.0f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  return status;
}

/**
  * @brief  Stop temperature sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_temp_sensor_stop(void)
{
  int status = BSP_ERROR_NONE;

  /* De-Initialize function */
  status |= BSP_ENV_SENSOR_DeInit(0);

  return status;
}

/**
  * @brief  Read temperature sensor acquisition value.
  * @param  None
  * @retval BSP status
  */
int webserver_temp_sensor_read(float *value)
{
  int   status = BSP_ERROR_NONE;

  /* Get value function */
  status |= BSP_ENV_SENSOR_GetValue(0, ENV_TEMPERATURE, value);

  return status;
}

/**
  * @brief  Start pressure sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_press_sensor_start(void)
{
  float outputDataRate = 0;
  int   status         = BSP_ERROR_NONE;

  /* Initialize function */
  status |= BSP_ENV_SENSOR_Init(1, ENV_PRESSURE);

  /* Enable function */
  status |= BSP_ENV_SENSOR_Enable(1, ENV_PRESSURE);

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_PRESSURE, 75.0f);
  status |= BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_PRESSURE, &outputDataRate);
  if (outputDataRate != 75.0f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(1, ENV_PRESSURE, 50.0f);
  status |= BSP_ENV_SENSOR_GetOutputDataRate(1, ENV_PRESSURE, &outputDataRate);
  if (outputDataRate != 50.0f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  return status;
}

/**
  * @brief  Stop pressure sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_press_sensor_stop(void)
{
  int status = BSP_ERROR_NONE;

  /* De-Initialize function */
  status |= BSP_ENV_SENSOR_DeInit(1);

  return status;
}

/**
  * @brief  Read pressure sensor acquisition value.
  * @param  None
  * @retval BSP status
  */
int webserver_press_sensor_read(float *value)
{
  int   status = BSP_ERROR_NONE;

  /* Get value function */
  status |= BSP_ENV_SENSOR_GetValue(1, ENV_PRESSURE, value);

  return status;
}

/**
  * @brief  Start humidity sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_humid_sensor_start(void)
{
  float outputDataRate = 0;
  int   status         = BSP_ERROR_NONE;

  /* Initialize function */
  status |= BSP_ENV_SENSOR_Init(0, ENV_HUMIDITY);

  /* Enable function */
  status |= BSP_ENV_SENSOR_Enable(0, ENV_HUMIDITY);

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_HUMIDITY, 12.5f);

  status |= BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_HUMIDITY, &outputDataRate);
  if (outputDataRate != 12.5f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  /* GetOutputDataRate and SetOutputDataRate functions */
  status |= BSP_ENV_SENSOR_SetOutputDataRate(0, ENV_HUMIDITY, 7.0f);
  status |= BSP_ENV_SENSOR_GetOutputDataRate(0, ENV_HUMIDITY, &outputDataRate);
  if (outputDataRate != 7.0f)
  {
    status |= BSP_ERROR_UNKNOWN_FAILURE;
  }

  return status;
}

/**
  * @brief  Stop humidity sensor acquisition.
  * @param  None
  * @retval BSP status
  */
int webserver_humid_sensor_stop(void)
{
  int status = BSP_ERROR_NONE;

  /* De-Initialize function */
  status |= BSP_ENV_SENSOR_DeInit(0);

  return status;
}

/**
  * @brief  Read humidity sensor acquisition value.
  * @param  None
  * @retval BSP status
  */
int webserver_humid_sensor_read(float *value)
{
  int   status = BSP_ERROR_NONE;

  /* Get value function */
  status |= BSP_ENV_SENSOR_GetValue(0, ENV_HUMIDITY, value);

  return status;
}
