/**
  ******************************************************************************
  * @file    net_ip_lwip.h
  * @author  MCD Application Team
  * @brief   Header for the network IP functions.
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
#ifndef NET_IP_LWIP_H
#define NET_IP_LWIP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "net_address.h"

#include "lwip/err.h"
#include "lwip/netif.h"

#define NET_IP_IF_TIMEOUT              1000
#define NET_IP_INPUT_QUEUE_TIMEOUT     1000
#define NET_IP_INPUT_QUEUE_SIZE        128
#define NET_IP_THREAD_SIZE             1024

#define NET_IP_FLAG_DEFAULT_INTERFACE        (1U<<0)
#define NET_IP_FLAG_TCPIP_STARTED_EXTERNALLY (1U<<1)

void net_ip_init(void);
int32_t net_ip_add_if(net_if_handle_t *pnetif, err_t (*if_init)(struct netif *netif), uint32_t flag);
int32_t net_ip_remove_if(net_if_handle_t *pnetif, err_t (*if_deinit)(struct netif *netif));
int32_t net_ip_connect(net_if_handle_t *pnetif);
int32_t net_ip_disconnect(net_if_handle_t *pnetif);
void net_ip_status_cb(struct netif *netif);
void net_ip_link_status(struct netif *netif, uint8_t status);
int32_t returncode_lwip2net(int32_t ret);

#endif /* NET_IP_LWIP_H */
