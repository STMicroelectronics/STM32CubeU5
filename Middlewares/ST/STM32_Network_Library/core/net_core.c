/**
  ******************************************************************************
  * @file    net_core.c
  * @author  MCD Application Team
  * @brief   Network interface core implementation
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
#include "net_conf.h"

static void netif_add_to_list(net_if_handle_t *pnetif);
static void netif_remove_from_list(net_if_handle_t *pnetif);
static net_if_handle_t *NetIfList = NULL;

#ifndef __IO
#define __IO volatile
#endif /* IO */


static void netif_add_to_list(net_if_handle_t *pnetif)
{
  LOCK_NETIF_LIST();
  if (NetIfList == NULL)
  {
    NetIfList = pnetif;
  }
  else
  {
    /*add it to end of the list*/
    net_if_handle_t *p_last_netif = NetIfList;

    while (p_last_netif->next != NULL)
    {
      p_last_netif = p_last_netif->next;
    }
    p_last_netif->next = pnetif;
  }
  UNLOCK_NETIF_LIST();
}


static void netif_remove_from_list(net_if_handle_t *pnetif)
{
  LOCK_NETIF_LIST();

  if (NetIfList == pnetif)
  {
    NetIfList = NetIfList->next;
  }
  else
  {
    for (net_if_handle_t *p_netif_prev = NetIfList; p_netif_prev->next != NULL; p_netif_prev = p_netif_prev->next)
    {
      if (p_netif_prev->next == pnetif)
      {
        p_netif_prev->next = pnetif->next;
        break;
      }
    }
  }
  UNLOCK_NETIF_LIST();
}


/**
  * @brief  Function description
  * @param  addr
  * @retval socket status
  */
net_if_handle_t *net_if_find(net_sockaddr_t *addr)
{
  const net_if_handle_t *ptr;
  net_ip_addr_t ipaddr = {0};
  net_ip_addr_t ipaddr_zero = {0};

  if (addr != NULL)
  {
    ipaddr = net_get_ip_addr(addr);
  }

  LOCK_NETIF_LIST();

  ptr = NetIfList;
  if (NET_DIFF(ipaddr, ipaddr_zero) != 0)
  {
    do
    {
      if (NET_EQUAL(ptr->ipaddr, ipaddr))
      {
        break;
      }
      ptr = ptr->next;
    } while (ptr != NULL);
  }
  UNLOCK_NETIF_LIST();

  return (net_if_handle_t *)ptr;
}


net_if_handle_t *netif_check(net_if_handle_t *pnetif)
{
  net_if_handle_t *p_netif = pnetif;
  if (p_netif == NULL)
  {
    /* Get default interface. */
    p_netif = net_if_find(NULL);
    if (p_netif == NULL)
    {
      NET_DBG_ERROR("No network interface defined\n");
    }
  }
  return p_netif;
}


/**
  * @brief  Wait for state transition
  * @param  pnetif a pointer to the selected network interface
  * @param  state  the expected state
  * @param  timeout max time to wait in ms for the transition
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_wait_state(net_if_handle_t *pnetif, net_state_t state, uint32_t timeout)
{
  int32_t ret = NET_OK;
  __IO net_state_t *const p = &pnetif->state;

  uint32_t start_time = HAL_GetTick();
  while (*p != state)
  {
    if (HAL_GetTick() >= (start_time + timeout))
    {
      ret = NET_TIMEOUT;
      break;
    }
    WAIT_STATE_CHANGE(timeout - (HAL_GetTick() - start_time));
  }
  return ret;
}


void net_if_notify(net_if_handle_t *pnetif, net_evt_t event_class, uint32_t event_id, void *event_data)
{
  /* call the user Handler first ,FIXME , first or not , race between wait state transition and user handler */
  if ((NULL != pnetif->event_handler) && (NULL != pnetif->event_handler->callback))
  {
    pnetif->event_handler->callback(pnetif->event_handler->context, event_class, event_id, event_data);
  }
}


#ifdef NET_USE_RTOS
static int32_t NetInitialized = 0;
#endif /* NET_USE_RTOS */

/** @defgroup State State Management Network Framework
  * Application uses this set of function to control the network interface state.
  * Normal state flow is init => start => connect.
  * Socket interface can be used when connected state is reached.Network interface is connected and got an IP address.
  * To finish, flow is disconnect,stop,deinit. All connection are closed and allocated resources are freed.
  * State transition are asynchronous but could be implemented in a synchronous way depending on the selected
  * network interface. Once a transition is requested,the net_wait_state primitive should be used to wait
  * for transition to occur.
  * @{
  */


/**
  * @brief  Perform network interface initialization
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  driver_init a pointer to a function which define the driver to use
  * @param  event_handler  a callback function to manage event from network framework
  * @retval 0 in case of success, an error code otherwise
   * This function is a synchronous function.
  */
int32_t net_if_init(net_if_handle_t *pnetif, net_if_driver_init_func driver_init,
                    const net_event_handler_t *event_handler)
{
  int32_t ret;
  net_if_handle_t *const p_netif = pnetif;

#ifdef NET_USE_RTOS
  if (NetInitialized == 0)
  {
    net_init_locks();
    NetInitialized = 1;
  }
#endif /* NET_USE_RTOS */

  if (p_netif != NULL)
  {
    p_netif->event_handler = event_handler;
    p_netif->state = NET_STATE_INITIALIZED;
    netif_add_to_list(p_netif);
    ret = (*driver_init)(p_netif);
    if (NET_OK != ret)
    {
      p_netif->state = NET_STATE_DEINITIALIZED;
      netif_remove_from_list(p_netif);

      NET_DBG_ERROR("Interface cannot be initialized.\n");
      ret = NET_ERROR_INTERFACE_FAILURE;
    }
  }
  else
  {
    NET_DBG_ERROR("Invalid interface.\n");
    ret = NET_ERROR_PARAMETER;
  }

#ifdef NET_USE_RTOS
  if (NET_OK != ret)
  {
    net_destroy_locks();
    NetInitialized = 0;
  }
#endif /* NET_USE_RTOS */

  return ret;
}


/**
  * @brief  Perform network interface de-initialization
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_deinit(net_if_handle_t *pnetif)
{
  const int32_t ret = net_state_manage_event(pnetif, NET_EVENT_CMD_DEINIT);

  pnetif->state = NET_STATE_DEINITIALIZED;

  if (ret == NET_OK)
  {
    netif_remove_from_list(pnetif);
    pnetif->next = NULL;
  }

#ifdef NET_USE_RTOS
  if (NetInitialized == 1)
  {
    net_destroy_locks();
    NetInitialized = 0;
  }
#endif /* NET_USE_RTOS */

  return ret;
}


/**

  * @brief  Start network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_start(net_if_handle_t *pnetif)
{
  return net_state_manage_event(pnetif, NET_EVENT_CMD_START);
}


/**
  * @brief  Stop network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_stop(net_if_handle_t *pnetif)
{
  return net_state_manage_event(pnetif, NET_EVENT_CMD_STOP);
}

/**

  * @brief  Yield data from network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_yield(net_if_handle_t *pnetif, uint32_t timeout)
{
  int32_t ret = NET_OK;

  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    net_state_t state = NET_STATE_DEINITIALIZED;

    (void) net_if_getState(p_netif, &state);

    if (state == NET_STATE_CONNECTED)
    {
      if (NULL != p_netif->pdrv->if_yield)
      {
        ret = p_netif->pdrv->if_yield(p_netif, timeout);
      }
      if (ret != NET_OK)
      {
        NET_DBG_ERROR("Interface yield failed!\n");
        ret = NET_ERROR_STATE_TRANSITION;
      }
    }
    else
    {
      NET_DBG_ERROR("Incorrect requested State transition\n");
      ret = NET_ERROR_INVALID_STATE_TRANSITION;
    }
  }
  else
  {
    NET_DBG_ERROR("Invalid interface.\n");
    ret = NET_ERROR_PARAMETER;
  }
  return ret;
}

/**

  * @brief  Connect network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_connect(net_if_handle_t *pnetif)
{
  return net_state_manage_event(pnetif, NET_EVENT_CMD_CONNECT);
}

/**

  * @brief  Disconnect network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_disconnect(net_if_handle_t *pnetif)
{
  return net_state_manage_event(pnetif, NET_EVENT_CMD_DISCONNECT);
}

/**
  * @brief  Send a direct ASCII command to the network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  cmd  a pointer to an allocated string for the command
  * @param  resp  a pointer to an allocated string for the response
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_atcmd(net_if_handle_t *pnetif, char_t *cmd, char_t *resp)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if ((p_netif != NULL) && (p_netif->pdrv->if_atcmd != NULL))
  {
    ret = p_netif->pdrv->if_atcmd(p_netif, cmd, resp);
  }
  return ret;
}


/**
  * @brief  get network interface state
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  state  a pointer to a net_state_t enum
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_getState(net_if_handle_t *pnetif, net_state_t *state)
{
  int32_t ret;
  const net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    *state = p_netif->state;
    ret = NET_OK;
  }
  else
  {
    NET_DBG_ERROR("Invalid interface.\n");
    *state = NET_STATE_DEINITIALIZED;
    ret = NET_ERROR_PARAMETER;
  }
  return ret;
}

/** @defgroup State
  * @}
  */


/**
  * @brief  Enable power save mode
  * @param  pnetif a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_powersave_enable(net_if_handle_t *pnetif)
{
  int32_t ret;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    if (p_netif->state == NET_STATE_CONNECTED)
    {
      ret = p_netif->pdrv->if_powersave_enable(p_netif);
    }
    else
    {
      NET_DBG_ERROR("Power-save cannot be enabled when the device is not connected.\n");
      ret = NET_ERROR_INVALID_STATE_TRANSITION;
    }
  }
  else
  {
    ret = NET_ERROR_PARAMETER;
    NET_DBG_ERROR("Invalid interface.\n");
  }
  return ret;
}

/** @defgroup GetAndSet Get and Set Network Interface information
  * @{
  */


/**
  * @brief  get MAC address
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  mac a pointer to an allocated macaddr_t structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_get_mac_address(net_if_handle_t *pnetif, macaddr_t *mac)
{
  int32_t ret;
  const net_if_handle_t *const p_netif = netif_check(pnetif);

  if ((p_netif != NULL) && (mac != NULL))
  {
    if (NET_STATE_DEINITIALIZED != p_netif->state)
    {
      *mac = p_netif->macaddr;
      ret = NET_OK;
    }
    else
    {
      ret = NET_ERROR_INTERFACE_FAILURE;
      NET_DBG_ERROR("Interface not yet initialized or in error state.\n");
    }
  }
  else
  {
    NET_DBG_ERROR("Invalid interface.\n");
    ret = NET_ERROR_PARAMETER;
  }
  return ret;
}

/**
  * @brief  get IP address
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  ip a pointer to an allocated net_ip_addr_t structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_get_ip_address(net_if_handle_t *pnetif, net_ip_addr_t *ip)
{
  int32_t ret;
  const net_if_handle_t *const p_netif = netif_check(pnetif);

  if ((p_netif != NULL) && (ip != NULL))
  {
    if (p_netif->state == NET_STATE_CONNECTED)
    {
      *ip = p_netif->ipaddr;
      ret = NET_OK;
    }
    else
    {
      NET_DBG_ERROR("Can get ipaddr for un-connected network interface.\n");
      ret = NET_ERROR_INTERFACE_FAILURE;
    }
  }
  else
  {
    NET_DBG_ERROR("Invalid interface.\n");
    ret = NET_ERROR_PARAMETER;
  }
  return ret;
}


/**
  * @brief  get host by name
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  name is a pointer to the host name string
  * @param  addr is a pointer to the structure net_sockaddr_t
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_gethostbyname(net_if_handle_t *pnetif, net_sockaddr_t *addr, char_t *name)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    ret = p_netif->pdrv->pgethostbyname(p_netif, addr, name);
  }
  return ret;
}


/**
  * @brief  Ping a remote host
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  addr is a pointer to the socketaddr of the remote host
  * @param  count is an integer, number of iteration to ping the remote machine
  * @param  delay is an integer, maximum delay in millisecond to wait for remote answer
  * @param  response is an array of <count> integer, containing the time to get response for each iteration.
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_ping(net_if_handle_t *pnetif, net_sockaddr_t *addr, int32_t count, int32_t delay, int32_t response[])
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    ret = p_netif->pdrv->pping(p_netif, addr, count, delay, response);
  }
  return ret;
}


/**
  * @brief  Enable or disable the DHCP mode
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  mode is a boolean, true to activate DHCP
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_set_dhcp_mode(net_if_handle_t *pnetif, bool mode)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    p_netif->dhcp_mode = mode;
    ret = NET_OK;
  }
  return ret;
}


/**
  * @brief  Enable or disable the DHCP server
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  mode is a boolean, true to activate the DHCP server
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_set_dhcp_server_mode(net_if_handle_t *pnetif, bool mode)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    p_netif->dhcp_server = mode;
    ret = NET_OK;
  }
  return ret;
}


/**
  * @brief  Set DHCP version IPv4 or IPv6
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  version is an option from dhcp_client_ver_t enum
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_set_dhcp_version(net_if_handle_t *pnetif, dhcp_client_ver_t version)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    p_netif->dhcp_version = version;
    ret = NET_OK;
  }
  return ret;
}


/**
  * @brief  Setting IP address, gateway and network mask for the current network interface
  * @param  pnetif a pointer to an allocated network interface structure
  * @param  ipaddr is a pointer to and net_ip_addr_t structure used as IP address
  * @param  gateway is a pointer to the net_ip_addr_t structure used as gateway address
  * @param  netmask is a pointer to the net_ip_addr_t structure used as the netmask
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_if_set_ipaddr(net_if_handle_t *pnetif, net_ip_addr_t ipaddr,
                          net_ip_addr_t gateway, net_ip_addr_t netmask)
{
  int32_t ret = NET_ERROR_FRAMEWORK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif != NULL)
  {
    p_netif->static_ipaddr  = ipaddr;
    p_netif->static_gateway = gateway;
    p_netif->static_netmask = netmask;
    ret = NET_OK;
  }
  return ret;
}

/** @defgroup GetAndSet
  * @}
  */
