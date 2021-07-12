/**
  **********************************************************************************************************************
  * @file    webserver_http_encoder.h
  * @author  MCD Application Team
  * @brief   header file for webserver_http_encoder.c
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
#ifndef WEBSERVER_HTTP_ENCODER_H
#define WEBSERVER_HTTP_ENCODER_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"
#include "webserver_http_cmd.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
WebServer_StatusTypeDef webserver_http_encode_html_response(char *html_response,
                                                            int html_size);
WebServer_StatusTypeDef webserver_http_encode_css_response(char *css_response,
                                                           int css_size);
WebServer_StatusTypeDef webserver_http_encode_js_response(char *js_response,
                                                          int js_size);
WebServer_StatusTypeDef webserver_http_encode_favicon_response(char *favicon_response,
                                                               int favicon_size);
WebServer_StatusTypeDef webserver_http_encode_sensor_response(char *sensor_response,
                                                              int sensor_size);
WebServer_StatusTypeDef webserver_http_encode_woff2_response(char *woff2_response,
                                                             int woff2_size);
WebServer_StatusTypeDef webserver_http_encode_json_response(char *json_response,
                                                            int json_size);
WebServer_StatusTypeDef webserver_http_encode_image_response(char *image_response,
                                                             int image_size);

#endif /* WEBSERVER_HTTP_ENCODER_H */
