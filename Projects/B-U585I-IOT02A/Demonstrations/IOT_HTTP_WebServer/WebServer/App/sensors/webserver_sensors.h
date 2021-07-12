/**
  **********************************************************************************************************************
  * @file    webserver_sensors.h
  * @author  MCD Application Team
  * @brief   Header for webserver_sensors.c module
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef WEBSERVER_SENSORS_H
#define WEBSERVER_SENSORS_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
int webserver_temp_sensor_start(void);
int webserver_temp_sensor_stop(void);
int webserver_temp_sensor_read(float *value);
int webserver_press_sensor_start(void);
int webserver_press_sensor_stop(void);
int webserver_press_sensor_read(float *value);
int webserver_humid_sensor_start(void);
int webserver_humid_sensor_stop(void);
int webserver_humid_sensor_read(float *value);
int webserver_sensors_start(void);

/* Private defines ---------------------------------------------------------------------------------------------------*/

#endif /* WEBSERVER_SENSORS_H */
