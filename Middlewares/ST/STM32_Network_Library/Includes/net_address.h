/**
  ******************************************************************************
  * @file    net_address.h
  * @author  MCD Application Team
  * @brief   Header for the network address management functions
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "net_conf.h"
#include "net_types.h"

#define NET_ZERO(A)      (void) memset(&(A), 0, sizeof(A))
#define NET_EQUAL(A, B)  (memcmp(&(A), &(B), sizeof(A)) == 0)
#define NET_DIFF(A, B)   (memcmp(&(A), &(B), sizeof(A)) != 0)
#define NET_COPY(A, B)   (void) memcpy(&(A), &(B), sizeof(A))

#ifdef NET_USE_LWIP_DEFINITIONS
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"

#define net_sockaddr_in          sockaddr_in

typedef struct sockaddr_in       net_sockaddr_in_t;
typedef struct sockaddr_in6      net_sockaddr_in6_t;
typedef struct sockaddr          net_sockaddr_t;
typedef ip_addr_t                net_ip_addr_t;
typedef in_addr_t                net_in_addr_t;


#define IP4ADDR_PORT_TO_SOCKADDR(sin, ipaddr, port)         \
  do {                                                      \
    (sin)->sin_len = (uint8_t) sizeof(struct sockaddr_in);  \
    (sin)->sin_family = AF_INET;                            \
    (sin)->sin_port = lwip_htons((port));                   \
    inet_addr_from_ip4addr(&(sin)->sin_addr, ipaddr);       \
    (void) memset((void*)(sin)->sin_zero, 0, SIN_ZERO_LEN); \
  } while(0)


#define NET_IPADDR_PORT_TO_SOCKADDR(sockaddr, ipaddr, port)          \
  IP4ADDR_PORT_TO_SOCKADDR(( struct sockaddr_in*)( void*)(sockaddr), \
                           ipaddr, port)


#define SOCKADDR4_TO_IP4ADDR_PORT(sin, ipaddr, port)            \
  do {                                                          \
    inet_addr_to_ip4addr(ip_2_ip4(ipaddr), &((sin)->sin_addr)); \
    (port) = lwip_ntohs((sin)->sin_port);                       \
  } while(0)

#define NET_SOCKADDR_TO_IPADDR_PORT(sockaddr, ipaddr, port)                     \
  SOCKADDR4_TO_IP4ADDR_PORT((const struct sockaddr_in*)(const void*)(sockaddr), \
                            ipaddr, port)


#define NET_IP_ADDR_CMP                         ip_addr_cmp
#define NET_IP_ADDR_ISANY_VAL                   ip_addr_isany_val
#define NET_IP_ADDR_COPY                        ip_addr_copy
#define NET_HTONL                               htonl
#define NET_NTOHL                               ntohl
#define NET_HTONS                               htons
#define NET_NTOHS                               ntohs
#define NET_NTOA_R                              ipaddr_ntoa_r
#define NET_NTOA                                ipaddr_ntoa
#define NET_ATON                                ip4addr_aton
#define NET_ATON_R                              ipaddr_addr

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 0))
#define NET_IPADDR4_INIT(u32val)                {{ (u32val) }}

#else
#define NET_IPADDR4_INIT(u32val)                {{{{ (u32val) }}}}
#endif /* NET_USE_IPV6 */

#if (LWIP_IPV4 && LWIP_IPV6)
#define NET_IPADDR4_INIT_BYTES(a, b, c, d)      { .type = IPADDR_TYPE_V4, .u_addr.ip4.addr = (PP_HTONL(LWIP_MAKEU32((a), (b) , (c) , (d)))) }
#endif /* LWIP_IPV4 && LWIP_IPV6 */

#else /* NET_USE_LWIP_DEFINITIONS */

#define NET_IP_ADDR_CMP(addr1, addr2) ((addr1)->addr == (addr2)->addr)
#define NET_IP_ADDR_COPY(dest, src)   ((dest).addr = (src).addr)
/** Safely copy one IP address to another (src may be NULL) */
#define NET_IP_ADDR_SET(dest, src)    ((dest)->addr = ((src) == NULL ? 0 : (src)->addr))

/** Set complete address to zero */
#define NET_IP_ADDR_SET_ZERO(ipaddr)   ((ipaddr)->addr = 0U)
#define NET_IP_ADDR_ISANY_VAL(addr1)   ((addr1).addr == 0U)

#define NET_SOCKADDR_TO_IPADDR_PORT(sockaddr, ipaddr, port) \
  do { \
    (ipaddr)->addr = (sockaddr)->sin_addr.s_addr; \
    (port) = NET_NTOHS((sockaddr)->sin_port); } while(0)

#define NET_IP4ADDR_PORT_TO_SOCKADDR(sin, ipaddr, port) \
  do { \
    (sin)->sin_len = (uint8_t) sizeof(net_sockaddr_in_t); \
    (sin)->sin_family = NET_AF_INET; \
    (sin)->sin_port = NET_HTONS((port)); \
    (sin)->sin_addr.s_addr = (ipaddr)->addr;\
    memset((void*)(sin)->sin_zero, 0, NET_SIN_ZERO_LEN); } while(0)

#define NET_IPADDR_PORT_TO_SOCKADDR(sockaddr, ipaddr, port) \
  NET_IP4ADDR_PORT_TO_SOCKADDR((net_sockaddr_in_t*)( void*)(sockaddr), (ipaddr), (port))


#define NET_HTONL(A)                       \
  ((((uint32_t)(A) & 0xff000000U) >> 24) | \
   (((uint32_t)(A) & 0x00ff0000U) >>  8) | \
   (((uint32_t)(A) & 0x0000ff00U) <<  8) | \
   (((uint32_t)(A) & 0x000000ffU) << 24))

#define NET_NTOHL       NET_HTONL

#define NET_HTONS(A) \
  ((uint16_t)((((uint16_t)(A) & 0xff00U) >> 8U) | (((uint16_t)(A) & 0x00ffU) << 8U)))

#define NET_NTOHS       NET_HTONS


#define NET_NTOA        net_ntoa
#define NET_NTOA_R      net_ntoa_r
#define NET_ATON        net_aton
#define NET_ATON_R      net_aton_r


#define NET_NULL_IP_ADDR       0U

#define NET_PP_HTONL(X)           \
  ((((X) & 0x000000ffUL) << 24) | \
   (((X) & 0x0000ff00UL) <<  8) | \
   (((X) & 0x00ff0000UL) >>  8) | \
   (((X) & 0xff000000UL) >> 24))


#define NET_MAKEU32_FROM_4BYTES(A, B, C, D) \
  (((uint32_t)((A) & 0xff) << 24) |         \
   ((uint32_t)((B) & 0xff) << 16) |         \
   ((uint32_t)((C) & 0xff) << 8)  |         \
   ((uint32_t)((D) & 0xff)))


#define NET_IPADDR4_INIT(u32val)            { (u32val) }
#define NET_IPADDR4_INIT_BYTES(a, b, c, d)  NET_IPADDR4_INIT(NET_PP_HTONL(NET_MAKEU32_FROM_4BYTES((a), (b), (c), (d))))


/* Generic socket address structures to support IPv6 and IPv4                */
/* For IPv4, must be a structured object with a uint32_t member called addr. */
typedef struct
{
  uint32_t addr;
} net_ip4_addr_t;

/* The IPv4 address with the network byte order organization. */
typedef struct net_in_addr
{
  uint32_t s_addr;
} net_in_addr_t;

/* Only IPv4 is managed. */
typedef net_ip4_addr_t net_ip_addr_t;

/* Same byte alignment as it could be with LwIp definitions. */
typedef struct net_sockaddr
{
  uint8_t sa_len;
  uint8_t sa_family;
  char_t  sa_data[14];
} net_sockaddr_t;


/* IPV4 address, with 8 stuffing bytes. */
#define NET_SIN_ZERO_LEN    8
typedef struct net_sockaddr_in
{
  uint8_t       sin_len;
  uint8_t       sin_family;
  uint16_t      sin_port;
  net_in_addr_t sin_addr;
  char_t        sin_zero[NET_SIN_ZERO_LEN];
} net_sockaddr_in_t;

char_t *net_ntoa(const net_ip_addr_t *addr);
char_t *net_ntoa_r(const net_ip_addr_t *addr, char_t *buf, int32_t buflen);
int32_t net_aton_r(const char_t *cp);
int32_t net_aton(const char_t *ptr, net_ip_addr_t *addr);

#endif /* NET_USE_LWIP_DEFINITIONS */


/** MAC address. */
typedef struct
{
  uint8_t mac[6];
} macaddr_t;


void          net_set_port(net_sockaddr_t *pAddr, uint16_t Port);
uint16_t      net_get_port(const net_sockaddr_t *pAddr);
net_ip_addr_t net_get_ip_addr(const net_sockaddr_t *pAddr);

typedef struct net_if_handle_s net_if_handle_t;

#endif /* NET_ADDRESS_H */
