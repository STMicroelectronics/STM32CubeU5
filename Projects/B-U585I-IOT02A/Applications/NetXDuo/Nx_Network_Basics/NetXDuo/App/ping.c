/**
  ******************************************************************************
  * @file    ping.c
  * @author  MCD Application Team
  * @brief   Ping command.
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
#include "msg.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "app_netxduo.h"
#include "main.h"
#include "nxd_bsd.h"
#include "nx_icmp.h"


#define PING_HOST               "www.st.com"

#define PING_ITERATION  (uint32_t)10

#define PING_DELAY_S    (uint32_t)1

/** ping identifier - must fit on a 16 bit word. */
#ifndef PING_ID
#define PING_ID        (USHORT)0xAFAF
#endif /* PING_ID */

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE (size_t)32
#endif /* PING_DATA_SIZE */

#define FOLD_U32T(u)          ((ULONG)(((u) >> 16) + ((u) & 0x0000ffffUL)))
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)


/* Private variables ---------------------------------------------------------*/


static uint16_t standard_chksum(const void *pData, int32_t Len);
static void ping_prepare_echo(NX_ICMPV4_ECHO *pEcho, uint16_t Len, uint16_t PingSeqNum);
static int32_t icmp_ping(struct sockaddr *pS_Addr, uint32_t S_Addr_len, uint32_t Count, uint32_t Timeout,
                         int32_t Response[]);
static int32_t ping_gethostbyname(struct sockaddr *pS_Addr, const char *Name);



/* Functions Definition ------------------------------------------------------*/

static uint16_t standard_chksum(const void *pData, int32_t Len)
{
  const uint8_t *pb = (const uint8_t *)pData;
  const uint16_t *ps;
  uint16_t t = 0;
  uint32_t sum = 0;
  uint32_t odd = ((uintptr_t)pb & 1);

  /* Get aligned to uint16_t */
  if (odd && (Len > 0))
  {
    ((uint8_t *)&t)[1] = *pb++;
    Len--;
  }

  /* Add the bulk of the data */
  ps = (const uint16_t *)(const VOID *)pb;
  while (Len > 1)
  {
    sum += *ps++;
    Len -= 2;
  }

  /* Consume left-over byte, if any */
  if (Len > 0)
  {
    ((uint8_t *)&t)[0] = *(const uint8_t *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is probably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd)
  {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return ~(uint16_t)sum;
}


static void ping_prepare_echo(NX_ICMPV4_ECHO *pEcho, uint16_t Len, uint16_t PingSeqNum)
{
  const size_t data_len = Len - sizeof(NX_ICMPV4_ECHO);

  pEcho->nx_icmpv4_echo_header.nx_icmpv4_header_type = NX_ICMP_ECHO_REQUEST_TYPE;
  pEcho->nx_icmpv4_echo_header.nx_icmpv4_header_code = NX_ICMP_ZERO_CODE;
  pEcho->nx_icmpv4_echo_header.nx_icmpv4_header_checksum = 0;
  pEcho->nx_icmpv4_echo_identifier = PING_ID;
  pEcho->nx_icmpv4_echo_sequence_num  = (uint16_t) htons(PingSeqNum);

  /* fill the additional data buffer with some data */
  for (size_t i = 0; i < data_len; i++)
  {
    ((CHAR *)pEcho)[sizeof(NX_ICMPV4_ECHO) + i] = (CHAR) i;
  }
  /* Ping data are sent in RAW mode, so compute also the checksum. */
  ((NX_ICMPV4_ECHO *)pEcho)->nx_icmpv4_echo_header.nx_icmpv4_header_checksum = standard_chksum(pEcho, Len);
}


static int32_t icmp_ping(struct sockaddr *pS_Addr, uint32_t S_Addr_len, uint32_t Count, uint32_t Timeout,
                         int32_t Response[])
{
  int32_t ret = 0;
  uint16_t ping_seq_num = 1;
  NX_ICMPV4_ECHO *p_echo_buffer = NULL;
  const size_t ping_size = sizeof(NX_ICMPV4_ECHO) + PING_DATA_SIZE;
  struct timeval timeout_opt = {0};

  timeout_opt.tv_sec = Timeout;

  for (uint32_t i = 0; i < Count; i++)
  {
    Response[i] = -1;
  }

  const INT sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  MSG_DEBUG("-> %" PRId32 "\n", (int32_t)sock);

  if (NX_SOC_ERROR == sock)
  {
    MSG_ERROR("socket() failed\n");
    ret = -1;
  }
  else if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout_opt, sizeof(timeout_opt)) < 0)
  {
    MSG_ERROR("Setting option for socket failed\n");
    ret = -1;
  }
  else
  {
    if (tx_byte_allocate(AppBytePool, (VOID **) &p_echo_buffer, ping_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      MSG_ERROR("Allocation failed\n");
      ret = -1;
    }

    if (p_echo_buffer == NULL)
    {
      MSG_ERROR("Allocation failed\n");
      ret = -1;
    }
  }

  if (ret == 0)
  {
    for (uint32_t i = 0; i < Count; i++)
    {
      if (p_echo_buffer != NULL)
      {
        ping_prepare_echo(p_echo_buffer, ping_size, ping_seq_num);
      }

      const uint32_t ping_start_time = HAL_GetTick();

      if (sendto(sock, (CHAR *)p_echo_buffer, (INT) ping_size, 0, pS_Addr, (INT) S_Addr_len) < 0)
      {
        MSG_ERROR("send failed\n");
        break;
      }

      do
      {
        struct sockaddr_in s_addr_in_from = {0};
        INT s_addr_in_from_len = sizeof(s_addr_in_from);
        CHAR buf[64] = "";
        const INT len = recvfrom(sock, buf, (INT) ping_size, 0, (struct sockaddr *)&s_addr_in_from, &s_addr_in_from_len);
        const uint32_t now = HAL_GetTick();

        MSG_DEBUG("Received from %s\n", inet_ntoa(s_addr_in_from.sin_addr));

        if (len >= (INT)(sizeof(NX_IPV4_HEADER) + sizeof(NX_ICMPV4_ECHO)))
        {
          const NX_IPV4_HEADER * const ip_header = (NX_IPV4_HEADER *)buf;
          const NX_ICMPV4_ECHO * const p_echo = (NX_ICMPV4_ECHO *)(buf + ((((ntohl(ip_header->nx_ip_header_word_0)) >> 24) & 0x0f) * 4U));
          const USHORT seq_num = htons(ping_seq_num);

          if ((p_echo->nx_icmpv4_echo_identifier == PING_ID) && (p_echo->nx_icmpv4_echo_sequence_num == seq_num))
          {
            if (p_echo->nx_icmpv4_echo_header.nx_icmpv4_header_type == (uint8_t) NX_ICMP_ECHO_REPLY_TYPE)
            {
              Response[i] = (int32_t) (now - ping_start_time);
              ret = 0;
              break;
            }
            else
            {
              MSG_INFO("ICMP Other Response received\n");
            }
          }
        }
        else
        {
          MSG_INFO("No data start %" PRId32 " : %" PRIu32 " .. %" PRIu32 "\n", (int32_t)len, ping_start_time, now);
        }
      } while (HAL_GetTick() < (ping_start_time + Timeout));
      ping_seq_num++;
    }
    tx_byte_release(p_echo_buffer);
  }

  soc_close(sock);

  return ret;
}


static int32_t ping_gethostbyname(struct sockaddr *pS_Addr, const char *Name)
{
  int32_t ret = -1;
  struct addrinfo hints = {0};
  struct addrinfo *host_info = NULL;

  hints.ai_family = pS_Addr->sa_family;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if (0 == getaddrinfo((const CHAR *)Name, NULL, &hints, &host_info))
  {
    if (host_info->ai_family == pS_Addr->sa_family)
    {
      struct sockaddr_in *const s_addr_in = (struct sockaddr_in *) pS_Addr;

      s_addr_in->sin_addr.s_addr = ((struct sockaddr_in *)host_info->ai_addr)->sin_addr.s_addr;

      ret = 0;
    }
    freeaddrinfo(host_info);
  }
  return ret;
}


int32_t ping_cmd(int32_t argc, char *argv[])
{
  const char *server_name = PING_HOST;
  int32_t ping_res[PING_ITERATION];
  struct sockaddr_in s_addr_in = {0};

  s_addr_in.sin_family = AF_INET;
  s_addr_in.sin_port = htons(0);

  for (uint32_t i = 0; i < (sizeof(ping_res) / sizeof(ping_res[0])); i++)
  {
    ping_res[i] = -1;
  }

  if (argc == 2)
  {
    server_name = argv[1];
  }

  MSG_INFO("ping <%s>\n", server_name);

  if (ping_gethostbyname((struct sockaddr *)&s_addr_in, server_name) < 0)
  {
    MSG_INFO("Could not find hostname \"%s\"\n", server_name);
    return -1;
  }
  else
  {
    MSG_INFO("Pinging hostname \"%s\" with %s:%" PRIu32 "\n",
             server_name, inet_ntoa(s_addr_in.sin_addr), (uint32_t)s_addr_in.sin_port);
  }

  if (icmp_ping((struct sockaddr *)&s_addr_in, sizeof(s_addr_in), PING_ITERATION, PING_DELAY_S, ping_res) >= 0)
  {
    for (uint32_t i = 0; i < PING_ITERATION; i++)
    {
      if (ping_res[i] >= 0)
      {
        MSG_INFO("Ping iteration #%" PRIu32 " round trip %" PRId32 "\n", i, ping_res[i]);
      }
    }
  }
  else
  {
    MSG_INFO("Ping failure\n");
  }

  return 0;
}
