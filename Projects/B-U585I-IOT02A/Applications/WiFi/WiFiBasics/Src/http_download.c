/**
  ******************************************************************************
  * @file      http_download.c
  * @author    MCD Application Team
  * @brief     This file provides code for HTTP download service.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "net_connect.h"
#include "net_mem.h"
#include "net_perf.h"
#include <inttypes.h>

#define MAX_BUFFER              1460
#define MAX_LINES               10
#define REPORT_TIMEPERIOD_MS    10000

#define HTTPS_PORT              443
#define HTTP_PORT               80
#define MIN(A,B)                (A)<(B)?(A):(B)


int32_t http_download_cmd(int32_t argc, char *argv[]);


static const char DOWNLOAD_HEADER[] =
  "GET  %s HTTP/1.1\r\n"
  "Host: %s\r\n"\
  "User-Agent: STSTVS\r\n"\
  "\r\n";

typedef struct t_http_context
{
  int32_t    hSocket ;
  uint32_t   status;
  uint64_t   contentLen;
  uint64_t   posFile;
} http_context_t;

static void http_split_host_request(const char *pURL, char **pHttpHost, char **pHttpReq, bool *secure);
static int32_t http_read_response(int32_t sock, unsigned char *pHeader, uint32_t maxSize, uint32_t *pRetSize);
static uint32_t service_lines(unsigned char *pBuffer, char *tLines[], uint32_t maxLines);
static char *http_seek_while(char *pString, char key);
static int32_t http_check_response(http_context_t *pCtx, unsigned char *pBuffer);
static char *http_seek_to(char *pString, char key);


static void http_split_host_request(const char *pURL, char **pHttpHost, char **pHttpReq, bool *secure)
{
  static char host[50];
  char *pStr = NULL;

  NET_ASSERT(pHttpHost, "Http host is undefined");
  NET_ASSERT(pHttpReq, "Http req file name is incorrect");

  *pHttpHost = 0;
  *pHttpReq = 0;

  if (strncmp(pURL, "http://", strlen("http://")) == 0)
  {
    pStr = ((char *)pURL) + strlen("http://");
    *secure = false;
  }
  if (strncmp(pURL, "https://", strlen("https://")) == 0)
  {
    pStr = ((char *)pURL) + strlen("https://");
    *secure = true;
  }

  if (pStr != NULL)
  {
    char *pHost = host;
    while (*pStr && *pStr != '/')
    {
      *pHost++ = *pStr++;
    }
    *pHost = 0;
    *pHttpHost = host;
    *pHttpReq  = pStr;
  }
}

static int32_t http_read_response(int32_t sock, unsigned char *pHeader, uint32_t maxSize, uint32_t *pRetSize)
{
  unsigned char *pString = pHeader;
  int32_t ret = 0;
  int32_t headerSize = maxSize;
  uint32_t count = 0;

  while (headerSize > 0)
  {
    ret = net_recv(sock, (void *)pString, 1, 0);
    if (ret < 0)
    {
      return ret;
    }

    pString++;
    headerSize--;
    count++;
    if (count > 4 && strncmp((char *)pString - 4, "\r\n\r\n", 4) == 0)
    {
      break;
    }
  }
  if (pRetSize)
  {
    *pRetSize = count;
  }
  if (headerSize == 0)
  {
    return -1;
  }
  return 0;
}


/* Fill a table with each line entries */
static uint32_t service_lines(unsigned char *pBuffer, char *tLines[], uint32_t maxLines)
{
  uint32_t index = 0;
  while (index  < maxLines && *pBuffer)
  {
    tLines[index] = (char *)pBuffer;
    /* Seek to the new line. */
    while (*pBuffer && *pBuffer != '\n')
    {
      pBuffer++;
    }
    index++;
    if (*pBuffer)
    {
      pBuffer++;
    }

  }
  return index;
}

/* move to char ... */
static char *http_seek_to(char *pString, char key)
{
  while (*pString && *pString != key)
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
static char *http_seek_while(char *pString, char key)
{
  if (pString)
  {
    while (*pString && *pString == key)
    {
      pString++;
    }
  }
  return pString;
}

/* move to char ... */
static char *http_seek_while_not(char *pString, char key)
{
  if (pString)
  {
    while (*pString && *pString != key)
    {
      pString++;
    }
  }
  return pString;
}

/* Check the response. */
static int32_t http_check_response(http_context_t *pCtx, unsigned char *pBuffer)
{
  char *tLines[MAX_LINES] = {0};
  uint32_t nbLines = service_lines(pBuffer, &tLines[0], MAX_LINES);

  pCtx->contentLen = (uint64_t) -1;
  pCtx->status     = 400;
  pCtx->posFile    = 0;
  if (nbLines < 1)
  {
    return NET_ERROR_GENERIC;
  }

  /* Extract the status from the first line. */
  char *pString = tLines[0];
  pString = http_seek_while_not(pString, ' ');
  pString = http_seek_while(pString, ' ');
  sscanf(pString, "%"PRIu32"", &pCtx->status);
  /* Parses each header lines and only manage mandatory information. */
  for (uint32_t a = 0 ; a < nbLines ; a++)
  {
    if (strncmp(tLines[a], "Content-Length", strlen("Content-Length")) == 0)
    {
      /* move to the size */
      char *pParam = http_seek_to(tLines[a], ':');
      pParam = http_seek_while(pParam, ' ');
      uint64_t len;
      sscanf(pParam, "%llu", &len);
      /* fill len */
      pCtx->contentLen = len;
    }
  }

  /* Check error status. */
  if (pCtx->status < 200 || pCtx->status >= 300)
  {
    return NET_ERROR_GENERIC;
  }
  return NET_OK;
}

static int32_t http_file_download(int32_t socket, const char *pHost, char *pReq)
{
  uint8_t        *pBuffer = 0;
  uint32_t       elapsed_time;
  uint32_t       start_time;
  uint32_t       local_start_time;
  int32_t        ret = 0;
  uint32_t       cptCount = 0;
  uint32_t       retSize;
  uint32_t       len;
  http_context_t httpCtx;
  uint32_t       refBw;


  /* Construct the header from the host and the request. */
  pBuffer = NET_MALLOC(MAX_BUFFER);
  NET_ASSERT(pBuffer, "Malloc failed");
  snprintf((char *)pBuffer, MAX_BUFFER - 1, DOWNLOAD_HEADER, pReq, pHost);

  /* Send GET HTTP request. */
  net_send(socket, pBuffer, strlen((char *)pBuffer), 0);

  /* Wait for the response. */
  if (http_read_response(socket, pBuffer, MAX_BUFFER - 1, &retSize) != 0)
  {
    printf("ERROR: no answer from HTTP server\n");
    ret = -1;
    goto exit;
  }

  if (http_check_response(&httpCtx, pBuffer) != NET_OK)
  {
    printf("ERROR: incorrect HTTP server response\n");
    ret = -1;
    goto exit;
  }

  len = httpCtx.contentLen;
  printf("File size %"PRIu32" bytes\n", len);
  local_start_time = start_time = NET_TICK();

  if (len > 100000)
  {
    printf("Warning: limiting transfer to first 100000 bytes\n");
    len = 100000;
  }
  while (len)
  {
    int32_t count;
    uint32_t szInSByte;

    /* szInSByte = MIN(MAX_BUFFER, len); */
    /* FIX from Mxchip needed */
    szInSByte = MIN(100, len);

    count = net_recv(socket, (void *)pBuffer, szInSByte, 0);
    if (count == -1)
    {
      ret = -1;
      break;
    }
    cptCount += count;

    elapsed_time = NET_TICK() - local_start_time;
    if (elapsed_time  > REPORT_TIMEPERIOD_MS)
    {
      uint32_t refBw;
      refBw  = (8 * cptCount) / elapsed_time;
      printf("Transfer %"PRIu32" bytes, remain %"PRIu32" bytes, bitrate %"PRIu32" kbit/s\n", cptCount, len, refBw);
      cptCount = 0;
      local_start_time = NET_TICK();
    }

    len -= count;
  }
  elapsed_time = NET_TICK() - start_time;
  refBw  = (8 * cptCount) / elapsed_time;
  printf("Transfer %"PRIu32" bytes, duration %"PRIu32" ms, bitrate %"PRIu32" kbit/s\n", cptCount, elapsed_time, refBw);


exit:
  NET_FREE(pBuffer);
  return ret;
}

/* extern char tls_root_ca_cert[]; */

#define HTTP_FILE_URL  "http://public.st.free.fr/500MO.bin"

int32_t http_download_cmd(int32_t argc, char *argv[])
{
  char    *hostname;
  char    *pHttpReq;
  bool    secure;
  char    *url;
  int32_t sock;
  int32_t port;
  int32_t ret = 0;

  net_sockaddr_t addr;

  if (argc == 1)
  {
    url = HTTP_FILE_URL;
  }
  else
  {
    url = argv[1];
  }

  http_split_host_request(url, &hostname, &pHttpReq, &secure);

  if ((hostname == 0) || (pHttpReq == 0))
  {
    printf("ERROR: invalid url %s\n", url);
    return -1;
  }

  addr.sa_len = sizeof(sockaddr_in_t);
  if (net_if_gethostbyname(NULL, &addr, hostname) < 0)
  {
    printf("ERROR: could not find hostname %s from url %s\n", hostname, url);
    return -1;
  }

  port = HTTP_PORT;
  if (secure)
  {
    port = HTTPS_PORT;
  }

#if NET_USE_IPV6
  if (addr.sa_family == NET_AF_INET)
  {
    sockaddr_in_t *addr_ip4 = (sockaddr_in_t *) &addr;
    addr_ip4->sin_port = NET_HTONS(port);
  }
  else
  {
    sockaddr_in6_t *addr_ip6 = (sockaddr_in6_t *) &addr;
    addr_ip6->sin6_port = NET_HTONS(port);
  }
#else
  sockaddr_in_t *addr_ip4 = (sockaddr_in_t *) &addr;
  addr_ip4->sin_port = NET_HTONS(port);
#endif /* NET_USE_IPV6 */

  sock = net_socket(NET_AF_INET, NET_SOCK_STREAM, NET_IPPROTO_TCP);
  if (sock < 0)
  {
    printf("ERROR: could not create the socket.\n");
    return -1;
  }

  if (true == secure)
  {
    bool check_server_identity = false;

    ret  = net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_SECURE, NULL, 0);
    /* ret |= net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_TLS_CA_CERT, tls_root_ca_cert, strlen(tls_root_ca_cert)+1);*/
    ret |= net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_TLS_SERVER_NAME, hostname, strlen(hostname) + 1);
    ret |= net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_TLS_SERVER_VERIFICATION, &check_server_identity, sizeof(bool));
  }

  {
    int32_t timeout = 1000;
    ret |= net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_RCVTIMEO, (void *)&timeout, sizeof(timeout));
    ret |= net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_SNDTIMEO, (void *)&timeout, sizeof(timeout));
  }

  if (ret != NET_OK)
  {
    printf("Could not set the socket options.\n");
    return -1;
  }

  if (net_connect(sock, &addr, sizeof(addr)) == 0)
  {
    printf("Downloading file %s from %s\n", &pHttpReq[1], hostname);
    ret = http_file_download(sock, hostname, pHttpReq);
  }
  else
  {
    printf("ERROR: cannot connect to %s port: %"PRIi32"\n", hostname, port);
    ret =  -1;
  }
  net_closesocket(sock);
  return ret;
}
