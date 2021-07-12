/**
  **********************************************************************************************************************
  * @file    webserver_http_cmd.h
  * @author  MCD Application Team
  * @brief   header file for webserver_http_cmd.c
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
#ifndef WEBSERVER_HTTP_CMD_H
#define WEBSERVER_HTTP_CMD_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/* HTTP commands */
extern const char     http_get_cmd[];
extern const uint32_t http_get_cmd_size;

/* Get html command */
extern const char     http_html_cmd[];
extern const uint32_t http_html_cmd_size;

/* Get css commands */
extern const char     http_css_chunk_cmd[];
extern const uint32_t http_css_chunk_cmd_size;
extern const char     http_css_main_cmd[];
extern const uint32_t http_css_main_cmd_size;

/* Get js commands */
extern const char     http_js_chunk_cmd[];
extern const uint32_t http_js_chunk_cmd_size;
extern const char     http_js_main_cmd[];
extern const uint32_t http_js_main_cmd_size;

/* Get font command */
extern const char     http_font_cmd[];
extern const uint32_t http_font_cmd_size;

/* Get json command */
extern const char     http_json_cmd[];
extern const uint32_t http_json_cmd_size;

/* Get favicon command */
extern const char     http_favicon_cmd[];
extern const uint32_t http_favicon_cmd_size;

/* Get image command */
extern const char     http_image_cmd[];
extern const uint32_t http_image_cmd_size;

/* Get css command */
extern const char     http_css_cmd[];
extern const uint32_t http_css_cmd_size;

/* Read temperature command */
extern const char     http_read_temperature_cmd[];
extern const uint32_t http_read_temperature_cmd_size;
/* Read pressure command */
extern const char     http_read_pressure_cmd[];
extern const uint32_t http_read_pressure_cmd_size;
/* Read humidity command */
extern const char     http_read_humidity_cmd[];
extern const uint32_t http_read_humidity_cmd_size;

/* HTTP response headers */
extern const char     *http_headers[];
extern const uint32_t http_headers_size;

/* HTTP response content types */
extern const char     *http_content_types[];
extern const uint32_t http_content_types_size;

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* HTTP header response defines */
#define HTTP_HEADER_ACCEPT           (0U)
#define HTTP_HEADER_FILE_NOTFOUND    (1U)
#define HTTP_HEADER_CONNECTION_CLOSE (2U)
#define HTTP_HEADER_CONTENT_TYPE     (3U)
#define HTTP_HEADER_CONTENT_RANGE    (4U)
#define HTTP_HEADER_CONTENT_LENGTH   (5U)
#define HTTP_HEADER_HEADERS_END      (6U)
#define HTTP_HEADER_SERVER           (7U)
#define HTTP_HEADER_CONTROL_ORIGIN   (8U)
#define HTTP_HEADER_CONTROL_METHODS  (9U)
#define HTTP_HEADER_CONTROL_HEADERS  (10U)
#define HTTP_HEADER_CACHE_CONTROL    (11U)

/* HTTP response content defines */
#define HTTP_HEADER_CONTENT_HTML     (0U)
#define HTTP_HEADER_CONTENT_CSS      (1U)
#define HTTP_HEADER_CONTENT_JS       (2U)
#define HTTP_HEADER_CONTENT_FAVICON  (3U)
#define HTTP_HEADER_CONTENT_FONT     (4U)
#define HTTP_HEADER_CONTENT_JSON     (5U)
#define HTTP_HEADER_CONTENT_IMAGE    (6U)

/* HTTP response content IDs */
#define HTTP_HEADER_HTML_ID          (0U)
#define HTTP_HEADER_JS_ID            (1U)
#define HTTP_HEADER_CSS_ID           (2U)
#define HTTP_HEADER_FAVICON_ID       (3U)
#define HTTP_HEADER_SENSOR_ID        (4U)
#define HTTP_HEADER_TEMP_SENSOR_ID   (5U)
#define HTTP_HEADER_PRESS_SENSOR_ID  (6U)
#define HTTP_HEADER_HUM_SENSOR_ID    (7U)
#define HTTP_HEADER_FONT_ID          (8U)
#define HTTP_HEADER_JSON_ID          (9U)
#define HTTP_HEADER_IMAGE_ID         (10U)

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */

#endif /* WEBSERVER_HTTP_CMD_H */
