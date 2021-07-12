/**
  **********************************************************************************************************************
  * @file    net_interface.h
  * @author  MCD Application Team
  * @brief   Header file for net_interface.c
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "net_connect.h"
#include <inttypes.h>
#include <stdbool.h>

typedef struct
{
  char *ssid;
  char *pwd;
} ap_t;


void NetWifiGetDefaultStation(net_wifi_credentials_t *WifiCreds, ap_t net_wifi_registred_hotspot[]);
net_if_handle_t *NetInterfaceOn(net_if_driver_init_func driver_init, net_if_notify_func notify_func);
void NetInterfaceConnect(net_if_handle_t *netif, bool dhcp_mode, void *credential, net_wifi_mode_t mode);
void NetInterfaceDisconnect(net_if_handle_t *netif);
void NetInterfaceOff(net_if_handle_t *netif);
int32_t scan_cmd(int32_t argc, char **argv);
uint32_t sys_now(void);

#ifdef __cplusplus
}
#endif

#endif /* NETINTERFACE_H */
