/**
  **********************************************************************************************************************
  * @file    webserver_http_encoder.c
  * @author  MCD Application Team
  * @brief   This file implements the web server http encoding services
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
#include "webserver_http_encoder.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Private functions -------------------------------------------------------------------------------------------------*/
/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  Encode html response.
  * @param  html_response : Pointer to html response buffer.
  * @param  html_size     : html body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_html_response(char *html_response,
                                                            int html_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous data */
  memset(html_response, 0, strlen((char *)html_response));

  /* Add http accept header */
  strcpy(html_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http server header */
  strcat(html_response, http_headers[HTTP_HEADER_SERVER]);

  /* Add http control headers */
  strcat(html_response, http_headers[HTTP_HEADER_CONTROL_ORIGIN]);
  strcat(html_response, http_headers[HTTP_HEADER_CONTROL_METHODS]);
  strcat(html_response, http_headers[HTTP_HEADER_CONTROL_HEADERS]);

  /* Add http content type header */
  strcat(html_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(html_response, http_content_types[HTTP_HEADER_CONTENT_HTML]);

  /* Add http content range header */
  strcat(html_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(html_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", html_size);
  strcat(html_response, body_length);
  strcat(html_response, "\r\n");

  /* Add http close connection header */
  strcat(html_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(html_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode css response.
  * @param  css_response : Pointer to css response buffer.
  * @param  css_size     : css body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_css_response(char *css_response,
                                                           int css_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous data */
  memset(css_response, 0, strlen((char *)css_response));

  /* Add http accept header */
  strcpy(css_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http server header */
  strcat(css_response, http_headers[HTTP_HEADER_SERVER]);

  /* Add http content type header */
  strcat(css_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(css_response, http_content_types[HTTP_HEADER_CONTENT_CSS]);

  /* Add http content range header */
  strcat(css_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(css_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", css_size);
  strcat(css_response, body_length);
  strcat(css_response, "\r\n");

  /* Add http close connection header */
  strcat(css_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(css_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode js response.
  * @param  js_response : Pointer to js response buffer.
  * @param  js_size     : js body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_js_response(char *js_response,
                                                          int js_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous data */
  memset(js_response, 0, strlen((char *)js_response));

  /* Add http accept header */
  strcpy(js_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http server header */
  strcat(js_response, http_headers[HTTP_HEADER_SERVER]);

  /* Add http content type header */
  strcat(js_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(js_response, http_content_types[HTTP_HEADER_CONTENT_JS]);

  /* Add http content range header */
  strcat(js_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(js_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", js_size);
  strcat(js_response, body_length);
  strcat(js_response, "\r\n");

  /* Add http close connection header */
  strcat(js_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(js_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode favicon response.
  * @param  favicon_response : Pointer to favicon response buffer.
  * @param  favicon_size     : favicon body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_favicon_response(char *favicon_response,
                                                               int favicon_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous data */
  memset(favicon_response, 0, strlen((char *)favicon_response));

  /* Add http accept header */
  strcpy(favicon_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http server header */
  strcat(favicon_response, http_headers[HTTP_HEADER_SERVER]);

  /* Add http cache control header */
  strcat(favicon_response, http_headers[HTTP_HEADER_CACHE_CONTROL]);

  /* Add http content type header */
  strcat(favicon_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(favicon_response, http_content_types[HTTP_HEADER_CONTENT_FAVICON]);

  /* Add http content range header */
  strcat(favicon_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(favicon_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", favicon_size);
  strcat(favicon_response, body_length);
  strcat(favicon_response, "\r\n");

  /* Add http close connection header */
  strcat(favicon_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(favicon_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode sensor response.
  * @param  sensor_response : Pointer to sensor response buffer.
  * @param  sensor_size     : sensor body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_sensor_response(char *sensor_response,
                                                              int sensor_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous response */
  memset(sensor_response, 0, strlen(sensor_response));

  /* Add http accept header */
  strcpy(sensor_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http content type header */
  strcat(sensor_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);

  /* Add http content range header */
  strcat(sensor_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(sensor_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", sensor_size);
  strcat(sensor_response, body_length);
  strcat(sensor_response, "\r\n");

  /* Add http close connection header */
  strcat(sensor_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(sensor_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode woff2 response.
  * @param  woff2_response : Pointer to sensor response buffer.
  * @param  woff2_size     : sensor body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_woff2_response(char *woff2_response,
                                                             int woff2_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous response */
  memset(woff2_response, 0, strlen(woff2_response));

  /* Add http accept header */
  strcpy(woff2_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http content type header */
  strcat(woff2_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);

  /* Add http content range header */
  strcat(woff2_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(woff2_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", woff2_size);
  strcat(woff2_response, body_length);
  strcat(woff2_response, "\r\n");

  /* Add http close connection header */
  strcat(woff2_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(woff2_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode json response.
  * @param  json_response : Pointer to json response buffer.
  * @param  json_size     : json body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_json_response(char *json_response,
                                                            int json_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous response */
  memset(json_response, 0, strlen(json_response));

  /* Add http accept header */
  strcpy(json_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http content type header */
  strcat(json_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(json_response, http_content_types[HTTP_HEADER_CONTENT_JSON]);

  /* Add http content range header */
  strcat(json_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(json_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", json_size);
  strcat(json_response, body_length);
  strcat(json_response, "\r\n");

  /* Add http close connection header */
  strcat(json_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(json_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}

/**
  * @brief  Encode image response.
  * @param  image_response : Pointer to image response buffer.
  * @param  image_size     : image body size.
  * @retval Web Server status.
  */
WebServer_StatusTypeDef webserver_http_encode_image_response(char *image_response,
                                                             int image_size)
{
  char body_length[20] = {0};

  /* Clear response buffer from any previous response */
  memset(image_response, 0, strlen(image_response));

  /* Add http accept header */
  strcpy(image_response, http_headers[HTTP_HEADER_ACCEPT]);

  /* Add http content type header */
  strcat(image_response, http_headers[HTTP_HEADER_CONTENT_TYPE]);
  strcat(image_response, http_content_types[HTTP_HEADER_CONTENT_IMAGE]);

  /* Add http content range header */
  strcat(image_response, http_headers[HTTP_HEADER_CONTENT_RANGE]);

  /* Add http content length header */
  strcat(image_response, http_headers[HTTP_HEADER_CONTENT_LENGTH]);
  sprintf(body_length, "%d", image_size);
  strcat(image_response, body_length);
  strcat(image_response, "\r\n");

  /* Add http close connection header */
  strcat(image_response, http_headers[HTTP_HEADER_CONNECTION_CLOSE]);

  /* Add http end of headers */
  strcat(image_response, http_headers[HTTP_HEADER_HEADERS_END]);

  return WEBSERVER_OK;
}
