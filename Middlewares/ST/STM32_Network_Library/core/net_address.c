/**
  ******************************************************************************
  * @file    net_address.c
  * @author  MCD Application Team
  * @brief   Implements network address conversion routines
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
#include "net_internals.h"

#define NET_IN_RANGE(c, lo, up)  (((c) >= (lo)) && ((c) <= (up)))
#define NET_IS_PRINT(c)          (NET_IN_RANGE((c), 0x20, 0x7f))
#define NET_ISDIGIT(c)           (NET_IN_RANGE((c), '0', '9'))
#define NET_ISXDIGIT(c)          (NET_ISDIGIT(c) || NET_IN_RANGE((c), 'a', 'f') || NET_IN_RANGE((c), 'A', 'F'))
#define NET_ISLOWER(c)           (NET_IN_RANGE((c), 'a', 'z'))
#define NET_ISSPACE(c) \
  (((c) == ' ') || ((c) == '\f') || ((c) == '\n') || ((c) == '\r') || ((c) == '\t') || ((c) == '\v'))

#if !defined(NET_USE_LWIP_DEFINITIONS)

/**
  * @brief Convert IPv4 address from structure to string
  * @note  IPv4 ONLY
  *
  * @param addr: an IP address structure
  * @param buf: buffer to fill with string corresponding to the given IP address
  * @param buflen: the length of the given buffer to fill
  * @return IPv4 address string, like "192.168.1.10"
  */
char_t *net_ntoa_r(const net_ip_addr_t *addr, char_t *buf, int32_t buflen)
{
  char_t inv[3];
  int32_t len = 0;
  char_t *buf_ret = NULL;
  const uint32_t addr_32bits = addr->addr;
  const uint8_t *ap = (const uint8_t *)&addr_32bits;

  for (uint8_t n = 0; n < (uint8_t) 4; n++)
  {
    uint8_t i = 0;
    uint8_t val = ap[n];
    do
    {
      const uint8_t rem = val % 10U;
      val /=  10U;
      inv[i] = (char_t)'0' + rem;
      i++;
    } while (val != 0U);

    while (i != 0U)
    {
      i--;
      if (len < buflen)
      {
        buf[len] = inv[i];
        len++;
      }
    }

    if ((n < 3U) && (len < buflen))
    {
      buf[len] = (char_t) '.';
      len++;
    }
  }

  if (len < buflen)
  {
    buf[len] = (char_t) '\0';
    buf_ret = buf;
  }

  return buf_ret;
}


/**
  * @brief  Convert IPv4 address from structure to string
  * @param  addr: an IP address structure
  * @retval IP address as a string
  */
char_t *net_ntoa(const net_ip_addr_t *addr)
{
  static char_t str[16];

  memset(str, 0, sizeof(str));

  return net_ntoa_r(addr, str, sizeof(str));
}


/**
  * @brief  Convert IP address from string to uint32
  * @param  ptr: IP string buffer
  * @param  addr: IP address structure
  * @retval status 1 success, otherwise failed
  */
int32_t net_aton(const char_t *ptr, net_ip_addr_t *addr)
{
  uint32_t val = 0;
  uint32_t base;
  char_t c0;
  const char_t *cp = ptr;
  uint32_t parts[4];
  uint32_t *pp = parts;
  int32_t ret = 1;
  int32_t done = 0;

  c0 = *cp;

  for (;;)
  {
    /*
     * Collect number up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, 1-9=decimal.
     */
    if (done == 1)
    {
      break;
    }

    if (!NET_ISDIGIT(c0))
    {
      ret = 0;
      done = 1;
    }
    else
    {
      val = 0;
      base = 10;
      if (c0 == '0')
      {
        ++cp;
        c0 = (char_t) * cp;
        if ((c0 == (char_t) 'x') || (c0 == (char_t) 'X'))
        {
          base = 16;
          ++cp;
          c0 = (char_t) * cp;
        }
        else
        {
          base = 8;
        }
      }

      for (;;)
      {
        if (NET_ISDIGIT(c0))
        {
          val = (val * base) + (uint32_t)c0 - (uint32_t) '0';
          ++cp;
          c0 = (char_t) * cp;
        }
        else if ((base == 16U) && NET_ISXDIGIT(c0))
        {
          val = (val << 4) | ((uint32_t)c0 + 10U - (uint32_t)(NET_ISLOWER(c0) ? 'a' : 'A'));
          ++cp;
          c0 = (char_t) * cp;
        }
        else
        {
          break;
        }
      }

      if (c0 == '.')
      {
        /*
         * Internet format:
         *  a.b.c.d
         *  a.b.c   (with c treated as 16 bits)
         *  a.b     (with b treated as 24 bits)
         */
        if (pp >= (parts + 3))
        {
          ret = 0;
          done = 1;
        }
        else
        {
          *pp = val;
          pp++;
          ++cp;
          c0 = (char_t) * cp;
        }
      }
      else
      {
        done = 1;
      }
    }
  }

  /* Check for trailing characters. */
  if ((c0 != (char_t)'\0') && (NET_ISSPACE((c0)) == false))
  {
    ret = 0;
  }
  else

    /* Concoct the address according to the number of parts specified. */
  {
    switch (pp - parts + 1)
    {
      case 0:
        ret = 0;          /* initial nondigit */
        break;

      case 1:             /* a -- 32 bits */
        break;

      case 2:             /* a.b -- 8.24 bits */
        if (val > 0xffffffUL)
        {
          ret = 0;
        }
        val |= parts[0] << 24;
        break;

      case 3:             /* a.b.c -- 8.8.16 bits */
        if (val > 0xffffU)
        {
          ret = 0;
          break;
        }
        val |= (parts[0] << 24) | (parts[1] << 16);
        break;

      case 4:             /* a.b.c.d -- 8.8.8.8 bits */
        if (val > 0xffU)
        {
          ret = 0;
          break;
        }
        val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
        break;
      default:
        ret = 0;
        break;
    }
  }

  if (ret == 1)
  {
    if (addr != NULL)
    {
      addr->addr = NET_HTONL(val);
    }
  }
  return ret;
}


/**
  * @brief  Function description
  * @param  cp: IP string buffer
  * @retval IPv4 address value
  */
int32_t net_aton_r(const char_t *cp)
{
  net_ip_addr_t val = {0};
  int32_t ret = 0;

  if (net_aton(cp, &val) != 0)
  {
    ret = (int32_t) val.addr;
  }

  return ret;
}

#endif /* NET_USE_LWIP_DEFINITIONS */


/**
  * @brief  Function description
  * @param  pAddr: a socket structure
  * @retval the port value
  */
uint16_t net_get_port(const net_sockaddr_t *pAddr)
{
  return NET_NTOHS(((const net_sockaddr_in_t *)pAddr)->sin_port);
}

/**
  * @brief  Function description
  * @param  pAddr: a socket structure
  * @param  Port: the port value
  * @retval
  */
void net_set_port(net_sockaddr_t *pAddr, uint16_t Port)
{
#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (pAddr->sa_family == NET_AF_INET6)
  {
    ((net_sockaddr_in6_t *)pAddr)->sin6_port = NET_HTONS(Port);
  }
  else

#endif /* NET_USE_IPV6 */
  {
    ((net_sockaddr_in_t *)pAddr)->sin_port = NET_HTONS(Port);
  }
}


/**
  * @brief  Function description
  * @param  pAddr: a socket structure
  * @retval IP address value
  */
net_ip_addr_t net_get_ip_addr(const net_sockaddr_t *pAddr)
{
  net_ip_addr_t ip_addr = {0};

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (pAddr->sa_family == NET_AF_INET6)
  {
    net_sockaddr_in6_t *const p_s_addr_in6 = (net_sockaddr_in6_t *)pAddr;

    ip_addr.type = (u8_t)IPADDR_TYPE_V6;
    inet6_addr_to_ip6addr(&ip_addr.u_addr.ip6, &p_s_addr_in6->sin6_addr);
  }
  else
#endif /* NET_USE_IPV6 */
  {
    const net_sockaddr_in_t *const p_s_addr_in = (const net_sockaddr_in_t *)pAddr;

#ifdef NET_USE_LWIP_DEFINITIONS
    ip_addr.u_addr.ip4.addr = p_s_addr_in->sin_addr.s_addr;
#else
    ip_addr.addr = p_s_addr_in->sin_addr.s_addr;
#endif /* NET_USE_LWIP_DEFINITIONS */
  }

  return ip_addr;
}
