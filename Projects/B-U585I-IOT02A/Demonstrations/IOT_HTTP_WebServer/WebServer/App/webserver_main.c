/**
  **********************************************************************************************************************
  * @file    app_main.c
  * @author  MCD Application Team
  * @brief   App main program body
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
#include "webserver_main.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/

/**
  * @brief  App main program
  * @param  None
  * @retval None
  */
void app_entry(void)
{
  /* Initialize web server system */
  if (system_init() != WEBSERVER_OK)
  {
    webserver_process_error();
  }

  /* Initialize wifi connection */
  if (webserver_wifi_init() != WEBSERVER_OK)
  {
    webserver_process_error();
  }

  /* Print web server header in hyperterminal */
  webserver_console_print_header();

  /* Connect to available wifi */
  if (webserver_wifi_connect() != WEBSERVER_OK)
  {
    webserver_process_error();
  }

  /* Start sensors */
  if (webserver_sensors_start() != BSP_ERROR_NONE)
  {
    webserver_process_error();
  }

  /* Start web server */
  if (webserver_http_start() != WEBSERVER_OK)
  {
    webserver_process_error();
  }
}
