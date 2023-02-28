/**
  ******************************************************************************
  * @file    net_class_extension.h
  * @author  MCD Application Team
  * @brief   Header for the network class extensions
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
#ifndef NET_CLASS_EXTENSION_H
#define NET_CLASS_EXTENSION_H

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "net_address.h"
#include "net_wifi.h"

typedef struct net_if_wifi_class_extension_s
{
  int32_t (*scan)(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid);
  int32_t (*get_scan_results)(net_if_handle_t *pnetif, net_wifi_scan_results_t *results, uint8_t number);
  int32_t (*set_credentials)(const net_wifi_credentials_t *cred);
  int32_t (*get_system_info)(const net_wifi_system_info_t info, void *data);
  int32_t (*set_param)(const net_wifi_param_t info, void *data);
  int32_t (*switch_mode)(net_if_handle_t *pnetif, net_wifi_mode_t target_mode);
  const net_wifi_credentials_t *credentials;
  net_wifi_mode_t       mode;
  /* Access Point parameter */
  uint8_t               access_channel;
  uint8_t               max_connections;
  bool                  AP_hidden;

  const                 net_wifi_powersave_t *powersave;
  void                  *ifp; /* Interface STA or AP handler. */
  net_wifi_ap_ie_t      *information_element;
} net_if_wifi_class_extension_t;


typedef struct net_if_ethernet_class_extension_s
{
  int32_t (*version)(void);
} net_if_ethernet_class_extension_t;


typedef struct net_if_custom_class_extension_s
{
  int32_t (*version)(void);
} net_if_custom_class_extension_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_CLASS_EXTENSION_H */
