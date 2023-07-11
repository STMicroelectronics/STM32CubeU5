/**
  ******************************************************************************
  * @file      http_download.c
  * @brief     This file provides code for HTTP download service.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"
#include "main.h"
#include <inttypes.h>
#include <stdbool.h>
#include "msg.h"
#include "nxd_bsd.h"
#include "nx_tcp.h"
#include "nx_driver_emw3080.h"


#define MAX_BUFFER              (NX_DRIVER_PACKET_SIZE + 1)

#define MAX_LINES               10
static const uint32_t REPORT_TIMEPERIOD_MS = 10000;

static const uint16_t HTTPS_PORT = 443;
static const uint16_t HTTP_PORT = 80;

static const uint32_t HTTP_RESPONSE_OK = 200;
static const uint32_t HTTP_RESPONSE_MULTIPLE_CHOICES = 300;
static const uint32_t HTTP_RESPONSE_BAD_REQUEST = 400;

#define DOWNLOAD_LIMITED_SIZE            (uint32_t)140000
#define SOCKET_TIMEOUT_S                 (uint32_t)10


static const char DOWNLOAD_HEADER[] =
  "GET %s HTTP/1.1\r\n"
  "Host: %s\r\n"\
  "User-Agent: STSTVS\r\n"\
  "\r\n";

typedef struct t_http_context
{
  int32_t  hSocket;
  uint32_t status;
  uint64_t contentLen;
  uint64_t posFile;
} http_context_t;

static void http_split_host_request(const char *pURL, char **pHttpHost, const char **pHttpReq, bool *Secure);
static int32_t http_read_response(int32_t Sock, unsigned char *pHeader, uint32_t MaxSize, uint32_t *pRetSize);
static uint32_t service_lines(unsigned char *pBuffer, char *tLines[], uint32_t MaxLines);
static char *http_seek_while(char *pString, char Key);
static char *http_seek_while_not(char *pString, char Key);
static int32_t http_check_response(http_context_t *pCtx, unsigned char *pBuffer);
static char *http_seek_to(char *pString, char Key);

#ifdef ENABLE_IOT_DEBUG
static void tcp_infos(void);
#endif /* ENABLE_IOT_DEBUG */


static void http_split_host_request(const char *pURL, char **pHttpHost, const char **pHttpReq, bool *Secure)
{
  static char host[50] = {0};
  const char *p_str = NULL;

  *pHttpHost = NULL;
  *pHttpReq = NULL;

  if (strncmp(pURL, "http://", strlen("http://")) == 0)
  {
    p_str = pURL + strlen("http://");
    *Secure = false;
  }
  else if (strncmp(pURL, "https://", strlen("https://")) == 0)
  {
    p_str = pURL + strlen("https://");
    *Secure = true;
  }
  else
  {
    p_str = NULL;
  }

  if (p_str != NULL)
  {
    char *pHost = host;
    while ((*p_str != 0) && (*p_str != '/'))
    {
      *pHost++ = *p_str++;
    }
    *pHost = 0;
    *pHttpHost = host;
    *pHttpReq  = p_str;
  }
}


static int32_t http_read_response(int32_t Sock, unsigned char *pHeader, uint32_t MaxSize, uint32_t *pRetSize)
{
  const char *p_string = (const char *)pHeader;

  uint32_t header_size = MaxSize;
  uint32_t count = 0;

  if (pRetSize)
  {
    *pRetSize = 0;
  }
  else
  {
    return -1;
  }

  while (header_size > 0)
  {
    INT ret = recv((INT)Sock, (VOID *)p_string, 1, 0);
    if (ret < 0)
    {
      return ret;
    }

    p_string++;
    header_size--;
    count++;
    if ((count > 4) && (strncmp(p_string - 4, "\r\n\r\n", 4) == 0))
    {
      break;
    }
  }

  if (header_size == 0)
  {
    return -1;
  }

  *pRetSize = count;

  return 0;
}


/* Fill a table with each line entries */
static uint32_t service_lines(unsigned char *pBuffer, char *tLines[], uint32_t MaxLines)
{
  uint32_t index = 0;

  if (*pBuffer != 0)
  {
    while ((index  < MaxLines) && (0 != *pBuffer))
    {
      tLines[index] = (char *)pBuffer;
      /* Seek to the new line. */
      while ((0 != *pBuffer) && (*pBuffer != '\n'))
      {
        pBuffer++;
      }
      index++;
      if (*pBuffer)
      {
        pBuffer++;
      }
    }
  }

  return index;
}


/* move to char ... */
static char *http_seek_to(char *pString, char Key)
{
  while (*pString && *pString != Key)
  {
    pString++;
  }
  if (*pString)
  {
    pString ++;
  }
  return pString;
}


/* move to char ... */
static char *http_seek_while(char *pString, char Key)
{
  if (pString)
  {
    while (*pString && (*pString == Key))
    {
      pString++;
    }
  }
  return pString;
}

/* move to char ... */
static char *http_seek_while_not(char *pString, char Key)
{
  if (pString)
  {
    while (*pString && (*pString != Key))
    {
      pString++;
    }
  }
  return pString;
}

/* Check the response. */
static int32_t http_check_response(http_context_t *pCtx, unsigned char *pBuffer)
{
  char *lines[MAX_LINES] = {0};
  uint32_t nb_lines = service_lines(pBuffer, &lines[0], MAX_LINES);

  pCtx->contentLen = (uint64_t) -1;
  pCtx->status     = HTTP_RESPONSE_BAD_REQUEST;
  pCtx->posFile    = 0;

  if (nb_lines < 1)
  {
    return -1;
  }

  /* Extract the status from the first line. */
  char *p_string = lines[0];
  p_string = http_seek_while_not(p_string, ' ');
  p_string = http_seek_while(p_string, ' ');
  sscanf(p_string, "%" PRIu32 "", &pCtx->status);

  /* Parses each header lines and only manage mandatory information. */
  for (uint32_t a = 0 ; a < nb_lines ; a++)
  {
    if (strncmp(lines[a], "Content-Length", strlen("Content-Length")) == 0)
    {
      /* move to the size */
      char *p_param = http_seek_to(lines[a], ':');
      p_param = http_seek_while(p_param, ' ');
      if (NULL != p_param)
      {
        uint64_t len = 0;
        sscanf(p_param, "%" PRIu64 "", &len);
        /* Fill len */
        pCtx->contentLen = len;
      }
    }
  }

  /* Check error status. */
  if ((pCtx->status < HTTP_RESPONSE_OK) || (pCtx->status >= HTTP_RESPONSE_MULTIPLE_CHOICES))
  {
    return -1;
  }
  return 0;
}


static int32_t http_file_download(int32_t socket, const char *pHost, const char *pReq)
{
  CHAR          *p_buffer = NULL;
  uint32_t       start_time = 0;
  uint32_t       local_start_time = 0;
  int32_t        ret = 0;
  uint32_t       cpt_count = 0;
  uint32_t       ret_size = 0;
  uint64_t       len = 0;
  http_context_t http_ctx = {0};
  const uint32_t limited_size = DOWNLOAD_LIMITED_SIZE;

#ifdef ENABLE_IOT_DEBUG
  static struct
  {
    int32_t done;
    uint32_t when;
    uint32_t duration;
  } history[101];
  static const uint32_t history_count = sizeof(history) / sizeof(history[0]);
  uint32_t history_index = 0;
#endif /* ENABLE_IOT_DEBUG */

  /* Construct the header from the host and the request. */
  if (tx_byte_allocate(AppBytePool, (VOID **) &p_buffer, MAX_BUFFER, TX_NO_WAIT) != TX_SUCCESS)
  {
    MSG_ERROR("Memory allocation failed\n");
    return -1;
  }

  const uint32_t buffer_size = MAX_BUFFER - 1;
  p_buffer[buffer_size] = '\0';

  snprintf((char *)p_buffer, buffer_size, DOWNLOAD_HEADER, pReq, pHost);

  /* Send GET HTTP request. */
  if (send((INT)socket, p_buffer, (INT)strlen((const char *)p_buffer), 0) < 0)
  {
    MSG_ERROR("send() failed\n");
  }

  /* Wait for the response. */
  if (http_read_response(socket, (unsigned char *) p_buffer, buffer_size, &ret_size) != 0)
  {
    MSG_ERROR("No answer from HTTP server\n");
    ret = -1;
    goto exit;
  }

  if (http_check_response(&http_ctx, (unsigned char *)p_buffer) != 0)
  {
    p_buffer[buffer_size/8] = 0; /* Arbitrary set a end of string. */
    MSG_INFO("  <--- \"%s\"\n\n", p_buffer);
    MSG_ERROR("Incorrect HTTP server response\n");
    ret = -1;
    goto exit;
  }

  len = http_ctx.contentLen;
  MSG_INFO("File size %" PRIu32 " bytes\n", (uint32_t)len);


  if (len > limited_size)
  {
    MSG_INFO("Limiting transfer to first %" PRIu32 " bytes with report time of %" PRIu32 " ms\n",
             limited_size, REPORT_TIMEPERIOD_MS);
    len = limited_size;
  }

  local_start_time = start_time = HAL_GetTick();

  while (len)
  {
    INT count;
    uint32_t sz_in_byte;
    uint32_t elapsed_time;

    sz_in_byte = (buffer_size < len) ? buffer_size : (uint32_t)len;

#ifdef ENABLE_IOT_DEBUG
    uint32_t measure_start = HAL_GetTick();
#endif /* ENABLE_IOT_DEBUG */

    count = recv(socket, (VOID *)p_buffer, (INT)sz_in_byte, 0);
    if (count == -1)
    {
      ret = -1;
      break;
    }
    if (count == 0)
    {
      break;
    }

    MSG_INFO(".");

    cpt_count += (uint32_t)count;
    /* MSG_DEBUG("%" PRId32 " ", (int32_t)count); */
    elapsed_time = HAL_GetTick() - local_start_time;

#ifdef ENABLE_IOT_DEBUG
    {
      uint32_t measure_end = HAL_GetTick();
      history[history_index].done = count;
      history[history_index].when = measure_end;
      history[history_index++].duration = measure_end - measure_start;
      if (history_index > history_count)
      {
        history_index = 0;
      }
    }
#endif /* ENABLE_IOT_DEBUG */

    if (elapsed_time  > REPORT_TIMEPERIOD_MS)
    {
      const uint32_t ref_band_width = (8 * cpt_count) / elapsed_time;
      MSG_INFO("\nTransfer %" PRIu32 " bytes, remain %" PRIu32 " bytes, bitrate %" PRIu32 " kbit/s\n",
               cpt_count, (uint32_t)len, ref_band_width);
      cpt_count = 0;
      local_start_time = start_time = HAL_GetTick();
    }

    if (len >= (uint64_t)count)
    {
      len -= (uint64_t)count;
    }
    else
    {
      len = 0;
    }
  }

  /* Actually close the receive data stream. */
  soc_close(socket);

  {
    const uint32_t duration = HAL_GetTick() - start_time;
    const uint32_t ref_band_width  = (8 * cpt_count) / duration;
    MSG_INFO("\nTransfer %" PRIu32 " bytes, duration %" PRIu32 " ms, bitrate %" PRIu32 " kbit/s\n",
             cpt_count, duration, ref_band_width);
  }
#ifdef ENABLE_IOT_DEBUG
  for (uint32_t i = 0; i < history_count; i++)
  {
    MSG_DEBUG("[%4" PRId32 "] - [%4" PRIu32 " - %4" PRIu32 "]\n", history[i].done, history[i].when, history[i].duration);
  }
#endif /* ENABLE_IOT_DEBUG */

exit:
  soc_close(socket);
  tx_byte_release(p_buffer);

  return ret;
}


static const char HTTP_FILE_URL[] = "http://public.st.free.fr/500MO.bin";

int32_t http_download_cmd(int32_t argc, char *argv[])
{
  CHAR *host_name = NULL;
  const char *p_http_req;
  bool secure = false;
  const char *url;
  uint16_t port;
  int32_t ret = 0;
  const INT domain = AF_INET;
  struct sockaddr_in s_addr_in = {0};

  if (argc == 1)
  {
    url = HTTP_FILE_URL;
  }
  else
  {
    url = argv[1];
  }

  http_split_host_request(url, &host_name, &p_http_req, &secure);

  if ((host_name == NULL) || (p_http_req == NULL))
  {
    MSG_ERROR("Invalid URL \"%s\"\n", url);
    return -1;
  }
  MSG_INFO("host \"%s\"\n req %s\n", host_name, p_http_req);

  port = HTTP_PORT;
  if (secure)
  {
    port = HTTPS_PORT;
  }

  s_addr_in.sin_port = htons(port);

  {
    struct addrinfo *host_info = NULL;
    struct addrinfo hints = {0};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(host_name, NULL, &hints, &host_info) != 0)
    {
      MSG_ERROR("Could not find host name %s from URL %s\n", host_name, url);
      return -1;
    }

    MSG_DEBUG("getaddrinfo() done\n");

    if (host_info->ai_family == domain)
    {
      s_addr_in.sin_family = domain;
      s_addr_in.sin_addr = ((struct sockaddr_in *)(host_info->ai_addr))->sin_addr;
      freeaddrinfo(host_info);
    }
  }

  MSG_INFO(" -> %s\n", inet_ntoa(s_addr_in.sin_addr));

  const INT sock = socket(domain, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
  {
    MSG_ERROR("Could not create the socket.\n");
    return -1;
  }

  MSG_DEBUG("socket() done\n");

  {
    struct sock_winsize tcp_size = {0};

    /*
     * NX_BSD_TCP_WINDOW / 16   ->  319 kbit/s
     * NX_BSD_TCP_WINDOW / 8    -> 1210 kbit/s
     * NX_BSD_TCP_WINDOW / 4    -> 1347 kbit/s
     * NX_BSD_TCP_WINDOW / 2    -> 1346 kbit/s
     * NX_BSD_TCP_WINDOW / 1    -> 1019 kbit/s
     */

    tcp_size.winsize = NX_BSD_TCP_WINDOW / 4;

    INT sock_status = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &tcp_size, sizeof(tcp_size));
    if (NX_SOC_ERROR == sock_status)
    {
      MSG_ERROR("setsockopt failed\n");
    }
    MSG_DEBUG("setsockopt() done with %" PRIu32 "\n", (uint32_t)tcp_size.winsize);
  }

  {
    struct timeval timeout = {0};
    timeout.tv_sec = SOCKET_TIMEOUT_S;

    INT sock_status = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (NX_SOC_ERROR == sock_status)
    {
      MSG_ERROR("setsockopt failed\n");
    }

    sock_status = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    if (NX_SOC_ERROR == sock_status)
    {
      MSG_ERROR("setsockopt failed\n");
    }
  }

  MSG_DEBUG("setsockopt() done\n");

  if (connect(sock, (struct sockaddr *)&s_addr_in, sizeof(s_addr_in)) == 0)
  {
    MSG_INFO("Downloading file %s from \"%s\" with %s:%" PRIu32 "\n", &p_http_req[1], host_name,
             inet_ntoa(s_addr_in.sin_addr), (uint32_t)ntohs(s_addr_in.sin_port));

    ret = http_file_download(sock, host_name, p_http_req);

#ifdef ENABLE_IOT_DEBUG
    tcp_infos();
#endif /* ENABLE_IOT_DEBUG */

  }
  else
  {
    MSG_ERROR("Cannot connect to %s port: %" PRIu32 "\n", host_name, (uint32_t)ntohs(s_addr_in.sin_port));
    soc_close(sock);
    ret =  -1;
  }

  return ret;
}

#ifdef ENABLE_IOT_DEBUG
void tcp_infos(void)
{
  ULONG packets_sent = 0;
  ULONG bytes_sent = 0;
  ULONG packets_received = 0;
  ULONG bytes_received = 0;
  ULONG invalid_packets = 0;
  ULONG receive_packets_dropped = 0;
  ULONG checksum_errors = 0;
  ULONG connections = 0;
  ULONG disconnections = 0;
  ULONG connections_dropped = 0;
  ULONG retransmit_packets = 0;

  _nx_tcp_info_get(&IpInstance, &packets_sent, &bytes_sent,
                   &packets_received, &bytes_received,
                   &invalid_packets, &receive_packets_dropped,
                   &checksum_errors, &connections,
                   &disconnections, &connections_dropped,
                   &retransmit_packets);

  MSG_INFO("\npacket sent             : %" PRIu32 "\nbytes sent              : %" PRIu32 "\n",
           (uint32_t)packets_sent, (uint32_t)bytes_sent);
  MSG_INFO("packet received         : %" PRIu32 "\nbytes received          : %" PRIu32 "\n",
           (uint32_t)packets_received, (uint32_t)bytes_received);
  MSG_INFO("invalid packets         : %" PRIu32 "\nreceive packets dropped : %" PRIu32 "\n",
           (uint32_t)invalid_packets,
           (uint32_t)receive_packets_dropped);
  MSG_INFO("checksum errors         : %" PRIu32 "\nconnections             : %" PRIu32 "\n",
           (uint32_t)checksum_errors,
           (uint32_t)connections);
  MSG_INFO("disconnections          : %" PRIu32 "\nconnections dropped     : %" PRIu32 "\n",
           (uint32_t)disconnections, (uint32_t)connections_dropped);
  MSG_INFO("retransmit packets      : %" PRIu32 "\n", (uint32_t)retransmit_packets);
}
#endif /* ENABLE_IOT_DEBUG */
