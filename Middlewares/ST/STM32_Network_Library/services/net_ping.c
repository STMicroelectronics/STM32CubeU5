/**
  ******************************************************************************
  * @file    net_ping.c
  * @author  MCD Application Team
  * @brief   application to send ICMP ping
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
#include "net_conf.h"
#include "net_connect.h"
#include "net_internals.h"
#include "net_perf.h"

#include "lwip/tcpip.h"
#include "lwip/icmp.h"

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
#include "lwip/ip6.h"
#endif /* NET_USE_IPV6 */

#include "lwip/inet_chksum.h"
#include "lwip/api.h"
#include "lwip/ip4.h"

#include <inttypes.h>

/* Private macro -------------------------------------------------------------*/
#define PING_ASSERT(test)    NET_ASSERT((test) != 0U, "PING")

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
#define PING_ID        0xAFAF
#endif /* PING_ID */

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE 32
#endif /* PING_DATA_SIZE */

static void ping_prepare_echo(struct icmp_echo_hdr *pEcho, uint16_t Len, uint16_t PingSeqNum, int32_t UseIPv6);


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void ping_prepare_echo(struct icmp_echo_hdr *pEcho, uint16_t Len, uint16_t PingSeqNum, int32_t UseIPv6)
{
  const size_t data_len = Len - sizeof(struct icmp_echo_hdr);

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 0))
  (void)UseIPv6;
#endif /* NET_USE_IPV6 */

  ICMPH_CODE_SET(pEcho, 0);
  pEcho->chksum = 0;
  pEcho->id     = PING_ID;
  pEcho->seqno  = (uint16_t) lwip_htons(PingSeqNum);

  NET_DBG_INFO("tx seq: %" PRIu32 "\n", (uint32_t)pEcho->seqno);

  /* Fill the additional data buffer with some data. */
  for (size_t i = 0; i < data_len; i++)
  {
    ((char_t *)pEcho)[sizeof(struct icmp_echo_hdr) + i] = (char_t)i;
  }

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (UseIPv6)
  {
    ICMPH_TYPE_SET(pEcho, ICMP6_TYPE_EREQ);
  }
  else
#endif /* NET_USE_IPV6 */
  {
    ICMPH_TYPE_SET(pEcho, ICMP_ECHO);

    /* Ping data are sent in RAW mode, so LwIP is not computing the checksum by SW. */

#ifndef CHECKSUM_BY_HARDWARE
    pEcho->chksum = inet_chksum(pEcho, Len);
#endif /* CHECKSUM_BY_HARDWARE */
  }
}


#define NET_IPH_HL(hdr) ((hdr)->_v_hl & 0x0fU)

int32_t icmp_ping(net_if_handle_t *pnetif, net_sockaddr_t *addr, int32_t count, int32_t timeout, int32_t response[])
{
  int32_t sock;
  int32_t ret = 0;
  net_sockaddr_t *p_from = NULL;
  uint32_t from_len = 0;
  net_sockaddr_in_t s_addr_in_from = {0};

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  net_sockaddr_in6_t s_addr_in6_from = {0};
#endif /* NET_USE_IPV6 */

  static int32_t ping_seq_num = 1;
  struct ip_hdr *iphdr = NULL;
  struct icmp_echo_hdr *p_echo = NULL;
  const size_t ping_size = sizeof(struct icmp_echo_hdr) + (uint32_t) PING_DATA_SIZE;
  int32_t reply_min_size = 0;

  PING_ASSERT(pnetif);

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (addr->sa_family == NET_AF_INET6)
  {
    s_addr_in6_from.sin6_len = sizeof(s_addr_in6_from);
    s_addr_in6_from.sin6_family = NET_AF_INET6;
    from_len = sizeof(s_addr_in6_from);
    p_from = (net_sockaddr_t *)&s_addr_in6_from;

    sock = net_socket(NET_AF_INET6, NET_SOCK_RAW, IP6_NEXTH_ICMP6);
    reply_min_size = sizeof(struct ip6_hdr) + sizeof(struct icmp_echo_hdr);
  }
  else
#endif /* NET_USE_IPV6 */
  {
    s_addr_in_from.sin_len = sizeof(s_addr_in_from);
    s_addr_in_from.sin_family = NET_AF_INET;
    from_len = sizeof(s_addr_in_from);
    p_from = (net_sockaddr_t *)&s_addr_in_from;

    sock = net_socket(NET_AF_INET, NET_SOCK_RAW, NET_IPPROTO_ICMP);
    reply_min_size = (int32_t)sizeof(struct icmp_echo_hdr);
  }

  if (sock < 0)
  {
    NET_DBG_ERROR("ping: socket failed\n");
    ret = -1;
  }
  else if (net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
  {
    NET_DBG_ERROR("ping: net_setsockopt() failed\n");
    ret = -1;
  }
  else
  {
    p_echo = (struct icmp_echo_hdr *)mem_malloc((mem_size_t)ping_size);

    if (p_echo == NULL)
    {
      NET_DBG_ERROR("ping_client_process: message allocation failed\n");
      ret = -1;
    }
  }

  net_setsockopt(sock, NET_SOL_SOCKET, NET_SO_BINDTODEVICE, pnetif, sizeof(*pnetif));

  if (ret == 0)
  {
    net_set_port(addr, 0U);

    for (int32_t i = 0; i < count; i++)
    {
      response[i] = -1;

      if (p_echo != NULL)
      {
        ping_prepare_echo(p_echo, (uint16_t) ping_size, (uint16_t) ping_seq_num,
                          addr->sa_family == NET_AF_INET6 ? 1 : 0);
      }

      NET_DBG_INFO("TX len: %" PRId32 "\n", (uint32_t)ping_size);

      if (net_sendto(sock, (uint8_t *)p_echo, (uint32_t)ping_size, 0, addr, from_len) < 0)
      {
        NET_DBG_ERROR("ping_client_process : send fail\n");
        break;
      }

      const uint32_t ping_start_time = NET_TICK();
      do
      {
        char_t buf[64 + 40] = {0}; /* extra 40 bytes for IPv6 header */
        const int32_t len = net_recvfrom(sock, (uint8_t *)buf, (uint32_t)sizeof(buf), 0, p_from, &from_len);

        NET_DBG_INFO("RX len %" PRId32 " >= %" PRId32 " ?\n", len, reply_min_size);

        if (len >= reply_min_size)
        {
          const u16_t seqnum = lwip_htons((u16_t) ping_seq_num);
          int32_t iphdr_len = 0;
          struct icmp_echo_hdr *iecho = NULL;

          iphdr = (struct ip_hdr *)buf;

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
          if (addr->sa_family == NET_AF_INET6)
          {
            iphdr_len = sizeof(struct ip6_hdr);
          }
          else
#endif /* NET_USE_IPV6 */
          {
            iphdr_len = IPH_HL(iphdr) * 4;
          }
          iecho = (struct icmp_echo_hdr *)(buf + iphdr_len);

          NET_DBG_INFO("RC type: %" PRIu32 ", code: %" PRIu32 ", id: 0x%04" PRIX32 ", seq: %" PRIu32 "\n",
                       (uint32_t)iecho->type, (uint32_t)iecho->code, (uint32_t)iecho->id, (uint32_t)iecho->seqno);

          if ((iecho->id == (uint16_t)PING_ID) && (iecho->seqno == seqnum))
          {
            if (ICMPH_CODE(iecho) == (uint8_t) ICMP_ER)
            {
              const uint32_t delta = NET_TICK() - ping_start_time;
              ret = 0;
              response[i] = (int32_t) delta;
              break;
            }
            else
            {
              NET_DBG_ERROR("ICMP Other Response received\n");
            }
          }
        }
        else
        {
          NET_DBG_ERROR("No data start %"PRId32": %"PRIu32" .. %"PRIu32"\n", len, ping_start_time, NET_TICK());
        }
      } while (NET_TICK() < (ping_start_time + (uint32_t) timeout));

      ping_seq_num++;
    }

    if (p_echo != NULL)
    {
      mem_free(p_echo);
    }
    (void) net_closesocket(sock);
  }

  return ret;
}
