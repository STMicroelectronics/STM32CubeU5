/**
  ******************************************************************************
  * @file    net_socket.c
  * @author  MCD Application Team
  * @brief   BSD like socket APIs
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
#include "net_errors.h"
#include "net_core.h"

#include <inttypes.h>

#ifndef NET_BYPASS_NET_SOCKET
#define OPTCHECKTYPE(type, optlen)  if (sizeof(type)!= (optlen)) {ret = NET_ERROR_PARAMETER; break;}

#define OPTCHECKSTRING(opt, optlen) if (strlen(opt)!= ((optlen)-1U)) {ret = NET_ERROR_PARAMETER; break;}


static net_socket_t *net_socket_get_and_lock(int32_t sock);
static bool is_valid_socket(int32_t sock);
static int32_t create_low_level_socket(int32_t sock);
static int32_t check_low_level_socket(int32_t sock);
static int32_t find_free_socket(void);
static int32_t clone_socket(int32_t sock);

static net_socket_t Sockets[NET_MAX_SOCKETS_NBR] = {0};


static net_socket_t *net_socket_get_and_lock(int32_t sock)
{
  LOCK_SOCK(sock);
  return &Sockets[sock];
}

/**
  * @brief  Verify if internal socket is already allocated
  * @param  sock  [in] the internal socket index
  * @retval socket status
  */
static bool is_valid_socket(int32_t sock)
{
  bool ret = false;
  if ((sock >= 0) && (sock < (int32_t) NET_MAX_SOCKETS_NBR))
  {
    if (Sockets[sock].status != SOCKET_NOT_ALIVE)
    {
      ret = true;
    }
  }
  return ret;
}


/**
  * @brief  function description
  * @param  sock  [in] the internal socket index
  * @retval socket status
  */
static int32_t create_low_level_socket(int32_t sock)
{
  net_socket_t *const p_socket = &Sockets[sock];

  if (p_socket->ulsocket == -1)
  {
    if (net_access_control(p_socket->pnetif, NET_ACCESS_SOCKET, &p_socket->ulsocket))
    {
      if (0 == Sockets[sock].protocol)
      {
        if (Sockets[sock].type == NET_SOCK_STREAM)
        {
          Sockets[sock].protocol = NET_IPPROTO_TCP;
        }
        if (Sockets[sock].type == NET_SOCK_DGRAM)
        {
          Sockets[sock].protocol = NET_IPPROTO_UDP;
        }
      }

      if (NULL != p_socket->pnetif->pdrv->psocket)
      {
        p_socket->ulsocket = p_socket->pnetif->pdrv->psocket(Sockets[sock].domain,
                                                             Sockets[sock].type,
                                                             Sockets[sock].protocol);
      }
    }
  }
  return p_socket->ulsocket;
}

static int32_t check_low_level_socket(int32_t sock)
{
  int32_t ret = -1;

  if (is_valid_socket(sock))
  {
    ret = Sockets[sock].ulsocket;
  }
  return ret;
}


/**
  * @brief  function description
  * @retval the internal socket index
  */
static int32_t find_free_socket(void)
{
  int32_t ret = NET_ERROR_INVALID_SOCKET;

  LOCK_SOCK_ARRAY();

  for (int32_t sidx = 0; sidx < (int32_t)NET_MAX_SOCKETS_NBR; sidx++)
  {
    if (Sockets[sidx].status == SOCKET_NOT_ALIVE)
    {
      Sockets[sidx].idx      = sidx;
      Sockets[sidx].status   = SOCKET_ALLOCATED;
      Sockets[sidx].domain   = 0;
      Sockets[sidx].type     = 0;
      Sockets[sidx].protocol = 0;
#ifdef NET_MBEDTLS_HOST_SUPPORT
      Sockets[sidx].is_secure = false;
      Sockets[sidx].tlsData   = NULL;
#endif /* NET_MBEDTLS_HOST_SUPPORT */
      Sockets[sidx].read_timeout  = NET_SOCK_DEFAULT_RECEIVE_TO;
      Sockets[sidx].write_timeout = NET_SOCK_DEFAULT_SEND_TO;
      Sockets[sidx].blocking = true;
      Sockets[sidx].ulsocket = -1;
      Sockets[sidx].pnetif   = net_if_find(NULL);

      LOCK_SOCK(sidx);
      ret = sidx;
      break;
    }
  }
  UNLOCK_SOCK_ARRAY();
  return ret;
}

static int32_t clone_socket(int32_t sock)
{
  const int32_t newsock = find_free_socket();

  if (newsock >= 0)
  {
    Sockets[newsock] = Sockets[sock];
  }
  return newsock;
}


/** @defgroup Socket Socket Management API
  * Application uses this manage socket. Its a BSD like socket interface. It supports TCP and UDP
  * protocol on IPV4 address. IPV6 address to be supported in future versions.
  * @{
  */

/**
  * @brief  Create a socket
  * @param  domain [in] integer should be set to NET_AF_INET
  * @param  type [in] integer should be NET_SOCK_STREAM,NET_SOCK_DGRAM or NET_SOCK_RAW
  * @param  protocol [in] integer should be NET_IPPROTO_TCP,NET_IPPROTO_ICMP,NET_IPPROTO_UDP or NET_IPPROTO_TCP_TLS
  * @retval socket number as an integer greater than zero in case of success, zero or less than zero otherwise
  */
int32_t net_socket(int32_t domain, int32_t type, int32_t protocol)
{
  const int32_t newsock = find_free_socket();

  if (newsock >= 0)
  {
    Sockets[newsock].domain   = domain;
    Sockets[newsock].type     = type;
    Sockets[newsock].protocol = protocol;
    UNLOCK_SOCK(newsock);
  }
  else
  {
    NET_DBG_ERROR("Socket allocation failed.\n");
  }
  return newsock;
}

/**
  * @brief  Bind a socket
  * @param  sock [in] integer socket number
  * @param  addr [in] pointer to net_sockaddr_t structure
  * @param  addrlen [in] unsigned integer length of the net_sockaddr_t
  * @retval zero in case of success, error code otherwise
  */
int32_t net_bind(int32_t sock, net_sockaddr_t *addr, uint32_t addrlen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#if (NET_USE_DEFAULT_INTERFACE == 1)
    if (p_socket->pnetif == NULL)
    {
      p_socket->pnetif = net_if_find(NULL);
    }
#endif /* NET_USE_DEFAULT_INTERFACE */

    if (create_low_level_socket(sock) < 0)
    {
      ret = NET_ERROR_SOCKET_FAILURE;
      NET_DBG_ERROR("low level socket creation failed.\n");
    }
    else
    {
      if (net_access_control(p_socket->pnetif, NET_ACCESS_BIND, &ret))
      {
        UNLOCK_SOCK(sock);
        ret = p_socket->pnetif->pdrv->pbind(p_socket->ulsocket, addr, addrlen);
        LOCK_SOCK(sock);
        if (ret != NET_OK)
        {
          NET_DBG_ERROR("Socket cannot be bound.\n");
        }
      }
    }
    UNLOCK_SOCK(sock);
  }
  return ret;
}


/**
  * @brief  Accept a connection from a client
  * @param  sock [in] integer socket number
  * @param  addr [out] pointer to net_sockaddr_t structure of remote connection
  * @param  addrlen [out] pointer to unsigned integer, length of the remote net_sockaddr_t
  * @retval socket number as an integer greater than zero in case of success, zero or less than zero otherwise
  */
int32_t net_accept(int32_t sock, net_sockaddr_t *addr, uint32_t *addrlen)
{
  int32_t newsock;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    newsock = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (check_low_level_socket(sock) < 0)
    {
      NET_DBG_ERROR("low level Socket has not been created.\n");
      newsock = NET_ERROR_SOCKET_FAILURE;
    }
    else
    {
      net_socket_t *const p_socket = net_socket_get_and_lock(sock);
      int32_t ulnewsock = -1;

      if (net_access_control(p_socket->pnetif, NET_ACCESS_BIND, &ulnewsock))
      {
        UNLOCK_SOCK(sock);
        ulnewsock = p_socket->pnetif->pdrv->paccept(p_socket->ulsocket, addr, addrlen);
        LOCK_SOCK(sock);

      }
      if (ulnewsock < 0)
      {
        NET_DBG_ERROR("No connection has been established.\n");
        newsock = ulnewsock;
      }
      else
      {
        Sockets[sock].status = SOCKET_CONNECTED;
        newsock = clone_socket(sock);
        if (newsock >= 0)
        {
          Sockets[newsock].ulsocket = ulnewsock;
          Sockets[newsock].cloneserver = true;
          UNLOCK_SOCK(newsock);
        }
      }
      UNLOCK_SOCK(sock);
    }
  }
  return newsock;
}


/**
  * @brief  socket is waiting for a connection
  * @param  sock [in] integer socket number
  * @param  backlog [in] integer maximum number of queued connection
  * @retval zero in case of success, none zero value in case of error
  */
int32_t net_listen(int32_t sock, int32_t backlog)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (check_low_level_socket(sock) < 0)
    {
      NET_DBG_ERROR("low level socket has not been created.\n");
      ret = NET_ERROR_SOCKET_FAILURE;
    }
    else
    {
      net_socket_t *const p_socket = net_socket_get_and_lock(sock);
      if (net_access_control(p_socket->pnetif, NET_ACCESS_LISTEN, &ret))
      {
        UNLOCK_SOCK(sock);
        ret = p_socket->pnetif->pdrv->plisten(p_socket->ulsocket, backlog);
        LOCK_SOCK(sock);

        if (ret != NET_OK)
        {
          NET_DBG_ERROR("Listen state cannot be set.\n");
        }
      }
      UNLOCK_SOCK(sock);
    }
  }
  return ret;
}


/**
  * @brief  connect socket to a server
  * @param  sock [in] integer socket number
  * @param  addr [in] pointer to net_sockaddr_t structure of server
  * @param  addrlen [in] length of the server net_sockaddr_t
  * @retval zero in case of success, non zero value in case of error
  */

int32_t net_connect(int32_t sock, net_sockaddr_t *addr, uint32_t addrlen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#if (NET_USE_DEFAULT_INTERFACE == 1)
    if (p_socket->pnetif == NULL)
    {
      p_socket->pnetif = net_if_find(NULL);
    }
#endif /* NET_USE_DEFAULT_INTERFACE */

    if (p_socket->pnetif == NULL)
    {
      ret = NET_ERROR_INTERFACE_FAILURE;
      NET_DBG_ERROR("No physical interface can be bound.\n");
    }
    else
    {
      if (create_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket creation failed.\n");
        if (create_low_level_socket(sock) < 0)
        {
          ret = NET_ERROR_SOCKET_FAILURE;
        }
        else
        {
          NET_DBG_ERROR("2nd try ok level socket creation success.\n");

        }
      }
      else
      {
        if (net_access_control(p_socket->pnetif, NET_ACCESS_CONNECT, &ret))
        {
          UNLOCK_SOCK(sock);
          ret = p_socket->pnetif->pdrv->pconnect(p_socket->ulsocket, addr, addrlen);
          LOCK_SOCK(sock);

          if (ret != NET_OK)
          {
            /* clear flag to avoid issue on clean up, mbedtls not started */
#ifdef NET_MBEDTLS_HOST_SUPPORT
            if ((p_socket->is_secure == true) && (p_socket->tlsData != NULL))
            {
              NET_FREE(p_socket->tlsData);
            }
            p_socket->is_secure = false;
#endif /* NET_MBEDTLS_HOST_SUPPORT */
            NET_DBG_ERROR("Connection cannot be established.\n");
          }
        }
      }
      if (ret == NET_OK)
      {
#ifdef NET_MBEDTLS_HOST_SUPPORT
        if (p_socket->is_secure)
        {
          if (net_mbedtls_start(p_socket) != NET_OK)
          {
            /* to avoid useless cleanup */
            p_socket->is_secure = false;
            UNLOCK_SOCK(sock);
            (void) net_closesocket(sock);
            LOCK_SOCK(sock);
            ret = NET_ERROR_SOCKET_FAILURE;
          }
          else
          {
            p_socket->tls_started = true;
          }
        }
        if (NET_OK == ret)
        {
#endif /* NET_MBEDTLS_HOST_SUPPORT */
          p_socket->status = SOCKET_CONNECTED;
#ifdef NET_MBEDTLS_HOST_SUPPORT
        }
#endif /* NET_MBEDTLS_HOST_SUPPORT */
      }
    }
    UNLOCK_SOCK(sock);
  }
  return ret;
}


/**
  * @brief  send data to a connected socket
  * @param  sock [in] integer source socket number
  * @param  buf [in] pointer to an array of unsigned byte
  * @param  len [in] number of byte to send
  * @retval number of byte transmitted, negative value in case of error or timeout
  */
int32_t net_send(int32_t sock, const uint8_t *buf, uint32_t len, int32_t flags)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (buf == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (check_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket has not been created.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#ifdef NET_MBEDTLS_HOST_SUPPORT
        if (p_socket->is_secure)
        {
          ret = (int32_t) net_mbedtls_sock_send(p_socket, buf, len);
        }
        else
#endif /* NET_MBEDTLS_HOST_SUPPORT */
        {
          if (net_access_control(p_socket->pnetif, NET_ACCESS_SEND, &ret))
          {
            UNLOCK_SOCK(sock);
            ret = p_socket->pnetif->pdrv->psend(p_socket->ulsocket, buf, (int32_t)len, flags);
            LOCK_SOCK(sock);

            if ((ret < 0) && (ret != NET_ERROR_DISCONNECTED))
            {
              NET_DBG_ERROR("Error during sending data.\n");
            }
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }
  return ret;
}


/**
  * @brief  receive data from a connected socket
  * @param  sock [in] integer socket number
  * @param  buf [in] pointer to an array of unsigned byte
  * @param  len [in] number of byte to read
  * @param  flags [in] specify blocking or non blocking, 0 is blocking mode, NET_MSG_DONTWAIT is non blocking
  * @retval number of byte received, negative value in case of error or timeout
  */
int32_t net_recv(int32_t sock, uint8_t *buf, uint32_t len, int32_t flags_in)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  int32_t flags = flags_in;


  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (buf == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (check_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket has not been created.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#ifdef NET_MBEDTLS_HOST_SUPPORT
        if (p_socket->is_secure)
        {
          ret = net_mbedtls_sock_recv(p_socket, buf, len);
        }
        else
#endif /* NET_MBEDTLS_HOST_SUPPORT */
        {
          if (net_access_control(p_socket->pnetif, NET_ACCESS_RECV, &ret))
          {
            UNLOCK_SOCK(sock);
            if (p_socket->read_timeout == 0)
            {
              flags = (int8_t) NET_MSG_DONTWAIT;
            }
            ret = p_socket->pnetif->pdrv->precv(p_socket->ulsocket, buf, (int32_t)len, flags);
            LOCK_SOCK(sock);
            if ((ret < 0) && (ret != NET_TIMEOUT) && (ret != NET_ERROR_DISCONNECTED))
            {
              NET_DBG_ERROR("Error (%"PRId32") during receiving data.\n", ret);
            }
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }

  return ret;
}

/**
  * @brief  send data to a socket at specific address
  * @param  sock [in] integer source socket number
  * @param  buf [in] pointer to an array of unsigned byte
  * @param  len [in] number of byte to send
  * @param  flags [in] specify blocking or non blocking, 0 is blocking mode, NET_MSG_DONTWAIT is non blocking
  * @param  to [in] pointer to net_sockaddr_t structure destination address
  * @param  tolen [in] pointer to unsigned integer, length of the destination net_sockaddr_t
  * @retval number of byte transmitted, negative value in case of error or timeout
  */
int32_t net_sendto(int32_t sock, const uint8_t *buf, uint32_t len, int32_t flags, net_sockaddr_t *to, uint32_t tolen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (buf == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (create_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket creation failed.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);
        if (net_access_control(p_socket->pnetif, NET_ACCESS_SENDTO, &ret))
        {
          UNLOCK_SOCK(sock);
          ret = p_socket->pnetif->pdrv->psendto(p_socket->ulsocket, buf, (int32_t)len, flags, to, tolen);
          LOCK_SOCK(sock);
          if ((ret < 0) && (ret != NET_ERROR_DISCONNECTED))
          {
            NET_DBG_ERROR("Error during sending data.\n");
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }
  return ret;
}


/**
  * @brief  receive data from a socket at specific address
  * @param  sock [in] integer source socket number
  * @param  buf [in] pointer to an array of unsigned byte
  * @param  len [in] number of byte to receive
  * @param  flags_in [in] specify blocking or non blocking, 0 is blocking mode, NET_MSG_DONTWAIT is non blocking
  * @param  from [in] pointer to net_sockaddr_t structure to store the source address
  * @param  fromlen [in] pointer to unsigned integer, length of the source net_sockaddr_t
  * @retval number of byte received, negative value in case of error or timeout
  */
int32_t net_recvfrom(int32_t sock, uint8_t *buf, uint32_t len, int32_t flags_in, net_sockaddr_t *from,
                     uint32_t *fromlen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (buf == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (create_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket creation failed.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);
        int32_t flags = flags_in;

        if (net_access_control(p_socket->pnetif, NET_ACCESS_RECVFROM, &ret))
        {
          UNLOCK_SOCK(sock);
          if (p_socket->read_timeout == 0)
          {
            flags = (int8_t) NET_MSG_DONTWAIT;
          }
          ret = p_socket->pnetif->pdrv->precvfrom(p_socket->ulsocket, buf, (int32_t)len, flags, from, fromlen);
          LOCK_SOCK(sock);
          if ((ret < 0) && (ret != NET_TIMEOUT) && (ret != NET_ERROR_DISCONNECTED))
          {
            /* Common Error during receiving data. */
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }
  return ret;
}


/**
  * @brief  shutdown a connection
  * @param  sock [in] integer socket number
  * @param  mode [in] integer, shutdown mode among NET_SHUTDOWN_R,NET_SHUTDOWN_W or NET_SHUTDOWN_RW
  * @retval zero on success, negative value in case of error
  */
int32_t net_shutdown(int32_t sock, int32_t mode)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#ifdef NET_MBEDTLS_HOST_SUPPORT
    if (p_socket->is_secure)
    {
      if (p_socket->tls_started)
      {
        p_socket->tls_started = false;
        (void) net_mbedtls_stop(p_socket);
      }
      p_socket->is_secure = false;
    }
#endif /* NET_MBEDTLS_HOST_SUPPORT */

    if (check_low_level_socket(sock) < 0)
    {
      NET_WARNING("Failed to shutdown: low level socket not existing.\n");
      Sockets[sock].status = SOCKET_NOT_ALIVE;
      ret = NET_OK;
    }
    else
    {
      if (net_access_control(p_socket->pnetif, NET_ACCESS_CLOSE, &ret))
      {
        UNLOCK_SOCK(sock);
        ret = p_socket->pnetif->pdrv->pshutdown(p_socket->ulsocket, mode);
        LOCK_SOCK(sock);

        if (ret != NET_OK)
        {
          NET_DBG_ERROR("Socket cannot be shutdown.\n");
        }
      }
    }
    UNLOCK_SOCK(sock);
  }

  return ret;
}


/**
  * @brief  close a socket
  * @param  sock [in] integer socket number
  * @retval zero on success, negative value in case of error
  */
int32_t net_closesocket(int32_t sock)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_WARNING("Invalid socket, can not close it.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    net_socket_t *const p_socket = net_socket_get_and_lock(sock);

#ifdef NET_MBEDTLS_HOST_SUPPORT
    if (p_socket->is_secure)
    {
      if (p_socket->tls_started)
      {
        p_socket->tls_started = false;
        (void) net_mbedtls_stop(p_socket);
      }
      p_socket->is_secure = false;
    }
#endif /* NET_MBEDTLS_HOST_SUPPORT */

    if (check_low_level_socket(sock) < 0)
    {
      NET_WARNING("Failed to close: low level socket not existing.\n");
      Sockets[sock].status = SOCKET_NOT_ALIVE;
      ret = NET_OK;
    }
    else
    {
      if (net_access_control(p_socket->pnetif, NET_ACCESS_CLOSE, &ret))
      {
        UNLOCK_SOCK(sock);
        ret = p_socket->pnetif->pdrv->pclose(p_socket->ulsocket, p_socket->cloneserver);
        LOCK_SOCK(sock);

        if (ret != NET_OK)
        {
          NET_DBG_ERROR("Socket cannot be closed.\n");
        }
        p_socket->ulsocket = -1;
        p_socket->status = SOCKET_NOT_ALIVE;
      }
    }
    UNLOCK_SOCK(sock);
  }

  return ret;
}


/**
  * @brief  get socket peer information
  * @param  sock [in] integer source socket number
  * @param  name [in] pointer to net_sockaddr_t structure to write the peer address
  * @param  namelen [in] pointer to unsigned integer, length of the peer net_sockaddr_t
  * @retval negative value in case of error
  */
int32_t net_getpeername(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (name == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (check_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket has not been created.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);
        {
          if (net_access_control(p_socket->pnetif, NET_ACCESS_SOCKET, &ret))
          {
            UNLOCK_SOCK(sock);
            ret = p_socket->pnetif->pdrv->pgetpeername(p_socket->ulsocket, name, namelen);
            LOCK_SOCK(sock);
            if (ret < 0)
            {
              NET_DBG_ERROR("Error during getpeername data.\n");
            }
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }
  return ret;
}


/**
  * @brief  get socket name information
  * @param  sock [in] integer source socket number
  * @param  name [in] pointer to net_sockaddr_t structure to write the socket address
  * @param  namelen [in] pointer to unsigned integer, length of the socket net_sockaddr_t
  * @retval negative value in case of error
  */
int32_t net_getsockname(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    if (name == NULL)
    {
      ret = NET_ERROR_INVALID_SOCKET;
    }
    else
    {
      if (check_low_level_socket(sock) < 0)
      {
        NET_DBG_ERROR("low level socket has not been created.\n");
        ret = NET_ERROR_SOCKET_FAILURE;
      }
      else
      {
        net_socket_t *const p_socket = net_socket_get_and_lock(sock);
        {
          if (net_access_control(p_socket->pnetif, NET_ACCESS_SOCKET, &ret))
          {
            UNLOCK_SOCK(sock);
            ret = p_socket->pnetif->pdrv->pgetsockname(p_socket->ulsocket, name, namelen);
            LOCK_SOCK(sock);
            if (ret < 0)
            {
              NET_DBG_ERROR("Error during getsockname data.\n");
            }
          }
        }
        UNLOCK_SOCK(sock);
      }
    }
  }
  return ret;
}


/**
  * @brief  set socket option
  * @param  sock [in] integer socket number
  * @param  level [in] integer, protocol layer that option is to be applied, must be set to NET_SOL_SOCKET
  * @param  optname [in] integer, option from the supported option list
  * @param  optvalue [in] void pointer to the wanted option value
  * @param  optlen [in] length of data pointed by optvalue
  * @retval zero on success, negative value in case of error
  */
int32_t net_setsockopt(int32_t sock, int32_t level, net_socketoption_t optname, const void *optvalue, uint32_t optlen)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  bool forward = false;
#ifdef NET_MBEDTLS_HOST_SUPPORT
  const char_t *optvalue_string = optvalue;
#endif /* NET_MBEDTLS_HOST_SUPPORT */

  if (!is_valid_socket(sock))
  {
    NET_DBG_ERROR("Invalid socket.\n");
    ret = NET_ERROR_INVALID_SOCKET;
  }
  else
  {
    net_socket_t *const p_socket = net_socket_get_and_lock(sock);

    switch (optname)
    {
      case NET_SO_BINDTODEVICE:
      {
        OPTCHECKTYPE(void *, optlen);

        p_socket->pnetif = (net_if_handle_t *)optvalue;
        if (p_socket->pnetif != NULL)
        {
          forward = true; /* forward to driver */
        }
      }
      break;

      case NET_SO_RCVTIMEO:
      {
        OPTCHECKTYPE(int32_t, optlen);
        p_socket->read_timeout = *(const int32_t *)optvalue;

#ifdef NET_MBEDTLS_HOST_SUPPORT
        net_mbedtls_set_read_timeout(p_socket);
#endif /* NET_MBEDTLS_HOST_SUPPORT */
        forward = true;
      }
      break;


      case NET_SO_SNDTIMEO:
      {
        OPTCHECKTYPE(int32_t, optlen);
        p_socket->write_timeout = *(const int32_t *)optvalue;

        forward = true;
      }
      break;

#ifdef NET_MBEDTLS_HOST_SUPPORT
      case NET_SO_SECURE:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS device certificate, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->is_secure = true;
            ret = NET_OK;
          }
        }
        break;
      }
      case NET_SO_TLS_DEV_CERT:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS device certificate, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_dev_cert = optvalue_string;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_DEV_KEY:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS device key, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_dev_key = optvalue_string;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_PASSWORD:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS password, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_dev_pwd = (const  uint8_t *) optvalue;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_CA_CERT:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS root CA, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_ca_certs = optvalue_string;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_CA_CRL:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS certificate revocation list, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_ca_crl = optvalue_string;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_SERVER_VERIFICATION:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKTYPE(bool, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS server verification mode, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_srv_verification = (*(const bool *)optvalue > 0) ? true : false;
            ret = NET_OK;
          }
        }
        break;
      }

      case NET_SO_TLS_SERVER_NAME:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKSTRING(optvalue_string, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS server name, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_srv_name = optvalue_string;
            ret = NET_OK;
          }
        }
        break;
      }

      /* Set the X.509 security profile */
      case NET_SO_TLS_CERT_PROF:
      {
        if (p_socket->status == SOCKET_CONNECTED)
        {
          ret = NET_ERROR_IS_CONNECTED;
        }
        else
        {
          OPTCHECKTYPE(mbedtls_x509_crt_profile, optlen);
          if (!net_mbedtls_check_tlsdata(p_socket))
          {
            NET_DBG_ERROR("Failed to set TLS X.509 security profile, Allocation failure\n");
            ret = NET_ERROR_NO_MEMORY;
          }
          else
          {
            p_socket->tlsData->tls_cert_prof = (const mbedtls_x509_crt_profile *) optvalue;
            ret = NET_OK;
          }
        }
        break;
      }
#endif /* NET_MBEDTLS_HOST_SUPPORT */

      default:
        forward = true;
        break;
    }

    if (true == forward)
    {
#if (NET_USE_DEFAULT_INTERFACE == 1)
      if (p_socket->pnetif == NULL)
      {
        p_socket->pnetif = net_if_find(NULL);
      }
#endif /* NET_USE_DEFAULT_INTERFACE */

      if (p_socket->pnetif == NULL)
      {
        NET_DBG_ERROR("No physical interface can be bound");
        ret = NET_ERROR_INTERFACE_FAILURE;
      }
      else
      {
        if (create_low_level_socket(sock) < 0)
        {
          NET_DBG_ERROR("low level socket creation failed.\n");
          ret = NET_ERROR_SOCKET_FAILURE;
        }
        else
        {
          if (net_access_control(p_socket->pnetif, NET_ACCESS_SETSOCKOPT, &ret))
          {
            UNLOCK_SOCK(sock);
            ret = p_socket->pnetif->pdrv->psetsockopt(p_socket->ulsocket, level, optname, optvalue, optlen);
            LOCK_SOCK(sock);
            if (ret < 0)
            {
              NET_DBG_ERROR("Error %"PRId32" while setting socket option (optname=%d).\n", ret, optname);
            }
          }
        }
      }
    }
    UNLOCK_SOCK(sock);
  }
  return ret;
}


/** @defgroup Socket
  * @}
  */
bool net_access_control(net_if_handle_t *pnetif, net_access_t func, int32_t *code)
{
  bool ret = true;
  if ((pnetif == NULL) || (pnetif->state == NET_STATE_CONNECTION_LOST))
  {
    /* send, receive function return zero, so user application should normally retry transfer */
    ret = false;
    *code = 0;
  }

  switch (func)
  {
    case NET_ACCESS_SOCKET:
      ret = true;
      break;

    case NET_ACCESS_BIND:
      break;

    case NET_ACCESS_LISTEN:
      break;

    case NET_ACCESS_CONNECT:
      break;

    case NET_ACCESS_CLOSE:
      ret = true;
      break;

    case NET_ACCESS_SEND:
      *code = 0;
      break;

    case NET_ACCESS_SENDTO:
      *code = 0;
      break;

    case NET_ACCESS_RECV:
      *code = 0;
      break;

    case NET_ACCESS_RECVFROM:
      *code = 0;
      break;

    case NET_ACCESS_SETSOCKOPT:
      ret = true;
      break;

    default:
      *code = NET_ERROR_FRAMEWORK;
      break;
  }
  return ret;
}

#endif /* NET_BYPASS_NET_SOCKET */
