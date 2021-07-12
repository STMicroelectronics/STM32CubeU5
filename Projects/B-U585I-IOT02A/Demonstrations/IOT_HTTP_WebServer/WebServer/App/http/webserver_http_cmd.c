/**
  **********************************************************************************************************************
  * @file    webserver_http_cmd.c
  * @author  MCD Application Team
  * @brief   This file implements the http command resources
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
#include "webserver_http_cmd.h"

/* HTTP commands */
const char     http_get_cmd[]    = "GET /";
const uint32_t http_get_cmd_size = sizeof(http_get_cmd) - 1U;

/* Get html command */
const char     http_html_cmd[]    = " ";
const uint32_t http_html_cmd_size = sizeof(http_html_cmd) - 1U;

/* Get css commands */
const char     http_css_chunk_cmd[]    = "static/css/chunk.css ";
const uint32_t http_css_chunk_cmd_size = sizeof(http_css_chunk_cmd) - 1U;
const char     http_css_main_cmd[]     = "static/css/main.css ";
const uint32_t http_css_main_cmd_size  = sizeof(http_css_main_cmd) - 1U;

/* Get js commands */
const char     http_js_chunk_cmd[]    = "static/js/chunk.js ";
const uint32_t http_js_chunk_cmd_size = sizeof(http_js_chunk_cmd) - 1U;
const char     http_js_main_cmd[]     = "static/js/main.js ";
const uint32_t http_js_main_cmd_size  = sizeof(http_js_main_cmd) - 1U;

/* Get font command */
const char     http_font_cmd[]    = "static/media/fa-solid-900.woff2 ";
const uint32_t http_font_cmd_size = sizeof(http_font_cmd) - 1U;

/* Get json command */
const char     http_json_cmd[]    = "manifest.json ";
const uint32_t http_json_cmd_size = sizeof(http_json_cmd) - 1U;

/* Get favicon command */
const char     http_favicon_cmd[]    = "favicon.png ";
const uint32_t http_favicon_cmd_size = sizeof(http_favicon_cmd) - 1U;

/* Get image command */
const char     http_image_cmd[]    = "static/media/FLSTM32U5.jpg ";
const uint32_t http_image_cmd_size = sizeof(http_image_cmd) - 1U;

/* Get css command */
const char     http_css_cmd[]    = "styles.css ";
const uint32_t http_css_cmd_size = sizeof(http_css_cmd) - 1U;

/* Read temperature command */
const char     http_read_temperature_cmd[]    = "Read_Temperature ";
const uint32_t http_read_temperature_cmd_size = sizeof(http_read_temperature_cmd) - 1U;
/* Read pressure command */
const char     http_read_pressure_cmd[]    = "Read_Pressure ";
const uint32_t http_read_pressure_cmd_size = sizeof(http_read_pressure_cmd) - 1U;
/* Read humidity command */
const char     http_read_humidity_cmd[]    = "Read_Humidity ";
const uint32_t http_read_humidity_cmd_size = sizeof(http_read_humidity_cmd) - 1U;

/* HTTP response headers */
const char *http_headers[] =
{
  "HTTP/1.1 200 OK\r\n",
  "HTTP/1.1 404 File not found\r\n",
  "Connection: Close\r\n",
  "Content-Type: ",
  "Accept-Ranges: bytes\r\n",
  "Content-Length: ",
  "\r\n",
  "Server: STM32U585-DK\r\n",
  "Access-Control-Allow-Origin: * \r\n",
  "Access-Control-Allow-Methods: GET\r\n",
  "Access-Control-Allow-Headers: cache-control, last-event-id, X-Requested-With\r\n",
  "Cache-Control: no-cache\r\n",
};

/* HTTP response content types */
const char *http_content_types[] =
{
  "text/html; charset=utf-8\r\n",
  "text/css\r\n",
  "text/javascript\r\n",
  "image/png\r\n",
  "font/woff2\r\n",
  "application/json\r\n",
  "image/jpg\r\n",
};
