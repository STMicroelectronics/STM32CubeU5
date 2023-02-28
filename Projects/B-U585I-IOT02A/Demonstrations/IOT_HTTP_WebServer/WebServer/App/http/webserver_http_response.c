/**
  **********************************************************************************************************************
  * @file    webserver_http_response.c
  * @author  MCD Application Team
  * @brief   This file implements the web server http response services
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
#include "webserver_http_response.h"
#include "webserver_http_encoder.h"
#include "net_connect.h"
#include "net_interface.h"
#include "mx_wifi.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
#define HTTP_SERVER_PORT         (80U)
#define HTTP_RECEIVE_BUFFER_SIZE (1500U)
#define HTTP_SENSORS_BUFFER_SIZE (20U)
#define HTTP_HEADERS_BUFFER_SIZE (500U)

#define MAX_SOCKET_DATASIZE      (MX_WIFI_BUFFER_SIZE - 100U)

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* HTTP buffers declaration */
char http_sensor_value[HTTP_SENSORS_BUFFER_SIZE];
char http_header_response[HTTP_HEADERS_BUFFER_SIZE];

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static WebServer_StatusTypeDef http_treat_request(int32_t socket);
static WebServer_StatusTypeDef http_send_headers_response(int32_t socket,
                                                          uint32_t headers_id,
                                                          char *headers_buff,
                                                          uint32_t data_size);
static WebServer_StatusTypeDef http_send_response(int32_t socket,
                                                  uint32_t headers_id,
                                                  char *headers_buff,
                                                  const char *body_buff,
                                                  uint32_t data_size);
static WebServer_StatusTypeDef http_send(int32_t socket,
                                         const char *frame,
                                         uint32_t frame_size);

/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  Start HTTP web server process
  * @param  None
  * @retval Web Server status
  */
WebServer_StatusTypeDef webserver_http_start(void)
{
  /* The IPv4 network socket for this server, to bind with the port to listen to. */
  struct net_sockaddr_in s_addr_in = {0};
  net_ip_addr_t ip_addr_in = {0};
  int32_t timeout = MX_WIFI_CMD_TIMEOUT;
  int32_t sock = 0;

  /* Create a TCP socket. */
  printf("\r\n");
  printf("*** Create TCP socket\r\n");
  if ((sock = net_socket(NET_AF_INET, NET_SOCK_STREAM, NET_IPPROTO_TCP)) < 0)
  {
    printf("*** Fail : Socket not created !!!!\r\n");
    return SOCKET_ERROR;
  }
  printf("*** TCP socket created\r\n");

  printf("*** net_setsockopt ...\r\n");
  net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_BINDTODEVICE, Netif, sizeof(&Netif));
  net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_RCVTIMEO, &timeout, sizeof(timeout));

  /* Bind socket */
  printf("*** Set port and bind socket\r\n");
  s_addr_in.sin_family = NET_AF_INET;
  s_addr_in.sin_len    = sizeof(s_addr_in);

  net_if_get_ip_address(Netif, &ip_addr_in);
  s_addr_in.sin_addr.s_addr = ip_addr_in.addr;

  net_set_port((struct net_sockaddr *)&s_addr_in, HTTP_SERVER_PORT);
  if (net_bind(sock, (struct net_sockaddr *)&s_addr_in, sizeof(s_addr_in)) != 0U)
  {
    printf("*** Fail : Socket not binded !!!!\r\n");
    return SOCKET_ERROR;
  }
  printf("*** Port and socket binded\r\n");

  /* listen for incoming connections */
  printf("*** Listen for incoming connections\r\n");
  if (net_listen(sock, 5) != 0U)
  {
    printf("*** Fail : Listening not started !!!!\r\n");
    return SOCKET_ERROR;
  }
  printf("*** Listening started \r\n");

  printf("--> Please connect to %s:%" PRIu32 "\n",
         net_ntoa(&ip_addr_in), (uint32_t)NET_NTOHS(s_addr_in.sin_port));

  /* Infinite loop to serve socket communication */
  while (1)
  {
    struct net_sockaddr_in s_addr_in_remote_host = {0};
    uint32_t s_addr_in_remote_host_len = sizeof(s_addr_in_remote_host);

    /* Accept net socket requests */
    const int32_t newconn = net_accept(sock, (struct net_sockaddr *)&s_addr_in_remote_host, (uint32_t *)&s_addr_in_remote_host_len);

    /* Check if a valid new connection is requested */
    if (newconn > 0)
    {
      net_ip_addr_t ip_addr_in_remote_host = {0};
      ip_addr_in_remote_host.addr = s_addr_in_remote_host.sin_addr.s_addr;

      printf("Request from %s:%" PRIu32 "\n",
             net_ntoa(&ip_addr_in_remote_host), (uint32_t)NET_NTOHS(s_addr_in_remote_host.sin_port));

      /* Treat net socket requests */
      if (http_treat_request(newconn) != WEBSERVER_OK)
      {
        printf("*** Fail : Invalid HTTP request !!!!\r\n");
        return SOCKET_ERROR;
      }
    }
    else
    {
      printf("*** Fail : Invalid socket connection !!!! \r\n");
      return SOCKET_ERROR;
    }
  }
}

/**
  * @brief  Treat webserver HTTP request
  * @param  socket : connection socket
  * @retval Web Server status
  */
static WebServer_StatusTypeDef http_treat_request(int32_t socket)
{
  static unsigned char recv_buffer[HTTP_RECEIVE_BUFFER_SIZE];

  /* Clear receive buffer */
  memset((void*)recv_buffer, 0, sizeof(recv_buffer));

  /* Read the http request */
  if (net_recv(socket, recv_buffer, HTTP_RECEIVE_BUFFER_SIZE, 0) < 0)
  {
    return HTTP_ERROR;
  }

  /* Treat get cmd */
  if (strncmp((char *)recv_buffer, http_get_cmd, http_get_cmd_size) == 0U)
  {
    /* Send html */
    if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_html_cmd, http_html_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_HTML_ID, http_header_response, html_buff, html_buff_size);
    }
    /* Send css shunk */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_css_chunk_cmd, http_css_chunk_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_CSS_ID, http_header_response, css_shunk_buff, css_shunk_buff_size);
    }
    /* Send main css */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_css_main_cmd, http_css_main_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_CSS_ID, http_header_response, css_main_buff, css_main_buff_size);
    }
    /* Send js shunk */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_js_chunk_cmd, http_js_chunk_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_JS_ID, http_header_response, js_shunk_buff, js_shunk_buff_size);
    }
    /* Send main js */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_js_main_cmd, http_js_main_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_JS_ID, http_header_response, js_main_buff, js_main_buff_size);
    }
    /* Send favicon */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_favicon_cmd, http_favicon_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_FAVICON_ID, http_header_response, favicon_buff, favicon_buff_size);
    }
    /* Send json */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_json_cmd, http_json_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_JSON_ID, http_header_response, json_buff, json_buff_size);
    }
    /* Send font */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_font_cmd, http_font_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_FONT_ID, http_header_response, font_buff, font_buff_size);
    }
    /* Send image */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_image_cmd, http_image_cmd_size) == 0U)
    {
      http_send_response(socket, HTTP_HEADER_IMAGE_ID, http_header_response, image_buff, image_buff_size);
    }
    /* Send read temperature response */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_read_temperature_cmd, http_read_temperature_cmd_size) == 0U)
    {
      float temperature_value = 0;
      webserver_temp_sensor_read(&temperature_value);
      sprintf(http_sensor_value, "%g", temperature_value);
      http_send_response(socket, HTTP_HEADER_SENSOR_ID, http_header_response, http_sensor_value, strlen(http_sensor_value));
    }
    /* Send read pressure response */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_read_pressure_cmd, http_read_pressure_cmd_size) == 0U)
    {
      float pressure_value = 0;
      webserver_press_sensor_read(&pressure_value);
      sprintf(http_sensor_value, "%g", pressure_value);
      http_send_response(socket, HTTP_HEADER_SENSOR_ID, http_header_response, http_sensor_value, strlen(http_sensor_value));
    }
    /* Send read humidity response */
    else if (strncmp((char *)&recv_buffer[http_get_cmd_size], http_read_humidity_cmd, http_read_humidity_cmd_size) == 0U)
    {
      float humidity_value = 0;
      webserver_humid_sensor_read(&humidity_value);
      sprintf(http_sensor_value, "%g", humidity_value);
      http_send_response(socket, HTTP_HEADER_SENSOR_ID, http_header_response, http_sensor_value, strlen(http_sensor_value));
    }
  }
  else
  {
    return HTTP_ERROR;
  }

  /* Close connection socket */
  if (net_closesocket(socket) != 0)
  {
    return HTTP_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  HTTP send headers and body responses data via socket
  * @param  socket       : connection socket
  * @param  headers_id   : specifies the header ID
  * @param  headers_buff : pointer to headers buffer
  * @param  data_size    : size of body web resources
  * @retval Web Server status
  */
static WebServer_StatusTypeDef http_send_response(int32_t socket,
                                                  uint32_t headers_id,
                                                  char *headers_buff,
                                                  const char *body_buff,
                                                  uint32_t data_size)
{
  /* Send HTTP header response */
  if (http_send_headers_response(socket, headers_id, headers_buff, data_size) != WEBSERVER_OK)
  {
    return HTTP_ERROR;
  }

  /* Send HTTP body response */
  if (http_send(socket, (const char *)body_buff, data_size) != WEBSERVER_OK)
  {
    return HTTP_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  HTTP send headers responses data via socket
  * @param  socket       : connection socket
  * @param  headers_id   : specifies the header ID
  * @param  headers_buff : pointer to headers buffer
  * @param  data_size    : size of body web resources
  * @retval Web Server status
  */
static WebServer_StatusTypeDef http_send_headers_response(int32_t socket,
                                                          uint32_t headers_id,
                                                          char *headers_buff,
                                                          uint32_t data_size)
{
  /* Check header ID */
  switch(headers_id)
  {
    /* Send HTML header response */
  case HTTP_HEADER_HTML_ID:
    {
      if (webserver_http_encode_html_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send CSS header response */
  case HTTP_HEADER_CSS_ID:
    {
      if (webserver_http_encode_css_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send JS header response */
  case HTTP_HEADER_JS_ID:
    {
      if (webserver_http_encode_js_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send favicon header response */
  case HTTP_HEADER_FAVICON_ID:
    {
      if (webserver_http_encode_favicon_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send font header response */
  case HTTP_HEADER_FONT_ID:
    {
      if (webserver_http_encode_woff2_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send sensor header response */
  case HTTP_HEADER_SENSOR_ID:
    {
      if (webserver_http_encode_sensor_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send JSON header response */
  case HTTP_HEADER_JSON_ID:
    {
      if (webserver_http_encode_json_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Send image header response */
  case HTTP_HEADER_IMAGE_ID:
    {
      if (webserver_http_encode_image_response(headers_buff, data_size) != WEBSERVER_OK)
      {
        return HTTP_ERROR;
      }

      break;
    }

    /* Invalid header ID */
  default:
    return HTTP_ERROR;
    break;
  }

  /* Send HTTP built response */
  if (http_send(socket, (const char *)headers_buff, strlen((char*)headers_buff)) != WEBSERVER_OK)
  {
    return HTTP_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  HTTP send data via socket
  * @param  socket      : connection socket
  * @param  frame       : pointer to frame to be sent
  * @param  frame_size  : size of frame to be sent
  * @retval Web Server status
  */
static WebServer_StatusTypeDef http_send(int32_t socket,
                                         const char *frame,
                                         uint32_t frame_size)
{
  /* Setup send information */
  uint32_t data_size = frame_size;
  uint32_t data_idx  = 0U;

  /* Check remaining data */
  while (data_size > 0U)
  {
    if (data_size >= MAX_SOCKET_DATASIZE)
    {
      /* Send data */
      if (net_send(socket, (uint8_t*)&frame[data_idx], MAX_SOCKET_DATASIZE, 0) <= 0U)
      {
        return HTTP_ERROR;
      }

      /* Update send information */
      data_size -= MAX_SOCKET_DATASIZE;
      data_idx += MAX_SOCKET_DATASIZE;
    }
    else
    {
      /* Send data */
      if (net_send(socket, (uint8_t*)&frame[data_idx], data_size, 0) <= 0U)
      {
        return HTTP_ERROR;
      }

      /* Update send information */
      data_size = 0U;
    }
  }

  return WEBSERVER_OK;
}
