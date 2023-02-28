/**
  ******************************************************************************
  * @file    net_ip_ethernet.h
  * @author  MCD Application Team
  * @brief   Header for the network interface on Ethernet
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
#ifndef NET_IP_ETHERNET_H
#define NET_IP_ETHERNET_H

/* Includes ------------------------------------------------------------------*/
#include "net_connect.h"
#include "net_internals.h"
#include "lwip/netif.h"

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Exported functions ------------------------------------------------------- */
void net_ethernetif_deinit(void);
void net_ethernetif_get_mac_addr(uint8_t *mac_addr);
uint8_t net_ethernetif_get_link_status(void);
err_t net_ethernetif_init(struct netif *netif);
int32_t net_ethernetif_output(void *context, net_buf_t *net_buf);

#endif /* NET_IP_ETHERNET_H */
