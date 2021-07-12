/**
  **********************************************************************************************************************
  * @file    webserver_status.h
  * @author  MCD Application Team
  * @brief   Header for webserver_status.c module
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
#ifndef WEBSERVER_STATUS_H
#define WEBSERVER_STATUS_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "b_u585i_iot02a.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  WEBSERVER_OK  = 0,
  UNKOWN_ERROR  = -1,
  SYSTEM_ERROR  = -2,
  CONSOLE_ERROR = -3,
  SENSOR_ERROR  = -4,
  PERIPH_ERROR  = -5,
  WIFI_ERROR    = -6,
  HTTP_ERROR    = -7,
  SOCKET_ERROR  = -8,

} WebServer_StatusTypeDef;

/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
void webserver_process_error(void);

/* Private defines ---------------------------------------------------------------------------------------------------*/


#endif /* WEBSERVER_STATUS_H */
