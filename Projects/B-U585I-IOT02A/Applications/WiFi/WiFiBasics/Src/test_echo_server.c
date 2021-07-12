/**
  ******************************************************************************
  * @file    test_echo_server.c
  * @author  MCD Application Team
  * @brief   Test a echo with a server.
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
#include <inttypes.h>
#include "main.h"
#include "net_connect.h"
#include "net_internals.h"
#include "net_perf.h"

#define CHECK_ECHO

int32_t test_echo_server(int32_t argc, char **argv);


/* Using ARM Mbed echo server */
#define REMOTE_IP_ADDR     "52.215.34.155"
#define REMOTE_PORT        7

#define TRANSFER_SIZE      2000
#define ITERATION_COUNT    10

static uint8_t buffer[TRANSFER_SIZE + ITERATION_COUNT];
static uint8_t buffin[TRANSFER_SIZE + ITERATION_COUNT];

static void fillbuffer(uint8_t *buff, int32_t n);
static int32_t checkbuffer(uint8_t *buff, int32_t n, int32_t offset);
static void EchoServerTest(uint32_t loop, uint32_t n);


static void fillbuffer(uint8_t *buff, int32_t n)
{
  for (int32_t i = 0; i < n; i++)
  {
    buff[i] = i;
  }
}

static int32_t checkbuffer(uint8_t *buff, int32_t n, int32_t offset)
{
  int32_t error_count = 0;
  for (int32_t i = 0; i < n; i++)
  {
    if (buff[i] != ((i + offset) & 0xff))
    {
      printf("Error: received data are different from data sent %"PRId32" <> %d at index %"PRId32"\n", i & 0xff, buff[i], i);
      error_count++;
    }
  }
  return error_count;
}

static void EchoServerTest(uint32_t loop, uint32_t n)
{
  int32_t error_count = 0;
  int32_t tstart;
  int32_t tstop;
  int32_t transfer = 0;
  int32_t sock;
  sockaddr_in_t addr;
  int32_t timeout = 10000;
  int32_t yes = 1;

  sock                  = net_socket(NET_AF_INET, NET_SOCK_STREAM, NET_IPPROTO_TCP);
  addr.sin_port         = NET_HTONS(REMOTE_PORT);
  addr.sin_family       = NET_AF_INET;
  S_ADDR(addr.sin_addr) = NET_ATON_R(REMOTE_IP_ADDR);

  int32_t ret = net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_RCVTIMEO, &timeout, sizeof(int32_t *));
  if (ret != NET_OK)
  {
    goto end ;
  }
  net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_SNDTIMEO, &timeout, sizeof(int32_t *));
  if (ret != NET_OK)
  {
    goto end ;
  }
  net_setsockopt(sock, NET_IPPROTO_TCP, NET_TCP_NODELAY, &yes, sizeof(int32_t *));
  if (ret != NET_OK)
  {
    goto end ;
  }

  printf("- Trying to connect to echo server  %s \n", REMOTE_IP_ADDR);

  if (net_connect(sock, (sockaddr_t *)&addr, sizeof(addr)) == 0)
  {
    int32_t transfer_m;
    int32_t transfer_l;
    printf("- Device connected to the %s\n", REMOTE_IP_ADDR);
    printf("- Starting transfer ");

    fillbuffer(buffer, n + loop);
    tstart = NET_TICK();
    net_perf_start();
    for (int32_t i = 0; i < loop; i++)
    {
      transfer_m = 0;

      do
      {
        transfer_l = net_send(sock, &buffer[i + transfer_m], n - transfer_m, 0);
        net_stop_cycle();
        printf(".");
        net_start_cycle();

        if (transfer_l < 0)
        {
          printf("Failed to send data to echo server, try again %"PRId32" : ", transfer_l);
          transfer_l = 0;
        }
        transfer_m += transfer_l;
      } while (transfer_m < n);
      transfer += transfer_m;
      transfer_m = 0;

      do
      {
        transfer_l = net_recv(sock, &buffin[transfer_m], n - transfer_m, 0);
        if (transfer_l < 0)
        {
          printf("\nError Transmit %"PRId32" : ", transfer_l);
          goto end;
        }
        transfer_m += transfer_l;
      } while (transfer_m < n);
      error_count = checkbuffer(buffin, n, i);
      net_stop_cycle();
      printf("x");
      net_start_cycle();
      transfer += transfer_m;
    }
    tstop = NET_TICK();
    if (error_count == 0)
    {
      printf("\nSuccessfull Echo Transfer %"PRId32" x %"PRId32" = %"PRId32" bytes in %"PRId32" ms , br = %"PRId32" Kbit/sec\n",
             loop, n, transfer, tstop - tstart, (transfer * 8) / (tstop - tstart));
    }
    else
    {
      printf("\nError: Echo transfer, find %"PRId32" different bytes\n", error_count);
    }
  }
  else
  {
    printf("Failed to connect to %s\n", REMOTE_IP_ADDR);
  }


end:
  net_shutdown(sock, NET_SHUTDOWN_RW);
  net_closesocket(sock);
}


int32_t test_echo_server(int32_t argc, char **argv)
{
  UNUSED(argc);
  UNUSED(argv);

  for (uint32_t transfer_size = 1000; transfer_size <= 2000; transfer_size += 200)
  {
    EchoServerTest(ITERATION_COUNT, TRANSFER_SIZE);
  }
  return 0;
}
