/**
  ******************************************************************************
  * @file    ping.c
  * @author  MCD Application Team
  * @brief   Ping cmd.
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
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "net_connect.h"

int32_t ping_cmd(int32_t argc, char **argv);

#define TIME_SOURCE_HTTP_HOST   "www.st.com"

#define PING_ITERATION  10
#define PING_DELAY      100


/* Private variables ---------------------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

int32_t ping_cmd(int32_t argc, char **argv)
{
  char *servername;
  int32_t ping_res[PING_ITERATION];
  net_sockaddr_t addr;

  addr.sa_len = sizeof(net_sockaddr_in_t);

  if (argc == 1)
  {
    servername = TIME_SOURCE_HTTP_HOST;
    printf("ping <hostname>\n");
  }
  else
  {
    servername = argv[1];
  }

  if (net_if_gethostbyname(NULL, &addr, servername) < 0)
  {
    printf("Could not find hostname %s\n", servername);
    return -1;
  }
  else
  {
    net_ip_addr_t ip_addr;
    printf("Pinging hostname %s\n", servername);

#if NET_USE_IPV6
    if (addr.sa_family == NET_AF_INET)
    {
      sockaddr_in_t *addr_ip4 = (sockaddr_in_t *) &addr;
      inet_addr_to_ip4addr(&ip_addr.u_addr.ip4, &addr_ip4->sin_addr);
    }
    else
    {
      printf("Not implemented \n");
      while (1);
    }
#else
    sockaddr_in_t *addr_ip4 = (sockaddr_in_t *) &addr;
    inet_addr_to_ip4addr(&ip_addr, &addr_ip4->sin_addr);
#endif /* NET_USE_IPV6 */

    printf("Pinging %s\n", NET_NTOA(&ip_addr));
  }

  if (net_if_ping(NULL, (sockaddr_t *)&addr, PING_ITERATION, PING_DELAY, ping_res) >= 0)
  {
    for (int32_t i = 0; i < PING_ITERATION ; i++)
    {
      if (ping_res[i] >= 0)
      {
        printf("Ping iteration #%"PRId32" roundtrip %"PRId32"\n", i, ping_res[i]);
      }
    }
  }
  else
  {
    printf("Ping failure\n");
  }
  return 0;
}
