/**
  **********************************************************************************************************************
  * @file    net_interface.c
  * @author  MCD Application Team
  * @brief   Configuration of the Network interface
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "net_connect.h"
#include "net_interface.h"

#define NET_STATE_TRANSITION_TIMEOUT 60000
#define MAX_SCAN_AP 100

static net_wifi_scan_results_t APs[MAX_SCAN_AP];

__weak uint32_t sys_now(void)
{
  return NET_TICK();
}

void NetWifiGetDefaultStation(net_wifi_credentials_t *WifiCreds, ap_t net_wifi_registred_hotspot[])
{
  int32_t ret;

  ret = net_wifi_scan(0, NET_WIFI_SCAN_PASSIVE, NULL);

  if (ret == NET_OK)
  {
    ret = net_wifi_get_scan_results(0, APs, MAX_SCAN_AP);
  }
  if (ret > 0)
  {
    for (uint32_t i = 0; i < ret; i++)
    {
      for (uint32_t j = 0; net_wifi_registred_hotspot[j].ssid != 0 ; j++)
      {
        if (strcmp((const char *)APs[i].ssid.value, net_wifi_registred_hotspot[j].ssid) == 0)
        {
          WifiCreds->ssid = net_wifi_registred_hotspot[j].ssid;
          WifiCreds->psk = net_wifi_registred_hotspot[j].pwd;
          WifiCreds->security_mode = APs[i].security;
          printf("\r\n");
          printf("\r\n");
          printf("Selected Access Point\t%32s ch %2d rss %d Security %10s country %4s bssid %02x.%02x.%02x.%02x.%02x.%02x\r\n",
                 APs[i].ssid.value, APs[i].channel, APs[i].rssi, net_wifi_security_to_string(APs[i].security), APs[i].country,
                 APs[i].bssid[0], APs[i].bssid[1], APs[i].bssid[2], APs[i].bssid[3], APs[i].bssid[4], APs[i].bssid[5]);
          return;
        }
      }
    }
  }
  return;
}

int32_t scan_cmd(int32_t argc, char **argv)
{
  int32_t ret;

  ret = net_wifi_scan(0, NET_WIFI_SCAN_PASSIVE, NULL);

  if (ret == NET_OK)
  {
    ret = net_wifi_get_scan_results(0, APs, MAX_SCAN_AP);
  }
  if (ret > 0)
  {
    printf("\r\n");
    printf("*** Start wifi scan \r\n");
    printf("\r\n");
    for (int i = 0; i < ret; i++)
    {
      printf("\t%d\t%32s ch %2d rss %d Security %10s country %4s bssid %02x.%02x.%02x.%02x.%02x.%02x\r\n",
             i, APs[i].ssid.value, APs[i].channel, APs[i].rssi, net_wifi_security_to_string(APs[i].security), APs[i].country,
             APs[i].bssid[0], APs[i].bssid[1], APs[i].bssid[2], APs[i].bssid[3], APs[i].bssid[4], APs[i].bssid[5]);
    }
    printf("\r\n");
    printf("*** End of wifi scan \r\n");
    printf("\r\n");
  }

  return 0;
}

net_if_handle_t *NetInterfaceOn(net_if_driver_init_func registred_driver, net_if_notify_func hnet_notify)
{
  net_if_handle_t *netif;
  net_event_handler_t *net_handler;
  int32_t ret;

  netif = NET_MALLOC(sizeof(net_if_handle_t));

  if (netif == NULL)
  {
    printf("Unable to allocate network interface");
  }
  memset(netif, 0, sizeof(net_if_handle_t));

  net_handler = NET_MALLOC(sizeof(net_event_handler_t));
  if (net_handler == NULL)
  {
    printf("Unable to allocate net_handler");
  }

  net_handler->callback = hnet_notify;
  net_handler->context = netif;

  /* Initialize the network stack */
  ret = net_if_init(netif, registred_driver, net_handler);
  if (NET_OK == ret)
  {
    ret = net_if_wait_state(netif, NET_STATE_INITIALIZED, NET_STATE_TRANSITION_TIMEOUT);
  }
  if (ret)
  {
    printf("Network interface initialization failed\r\n");
  }
  return netif;
}

void NetInterfaceOff(net_if_handle_t *netif)
{
  uint32_t ret;
  ret = net_if_stop(netif);
  if (NET_OK == ret)
  {
    ret = net_if_wait_state(netif, NET_STATE_INITIALIZED, NET_STATE_TRANSITION_TIMEOUT);
    if (NET_OK == ret)
    {
      ret = net_if_deinit(netif);
      if (NET_OK == ret)
      {
        NET_FREE((void *)netif->event_handler);
        NET_FREE(netif);
        printf("Network interface stopped!\r\n");
      }
    }
  }
  if (NET_OK != ret)
  {
    printf("Cannot stop interface\r\n");
  }
  return;
}

void NetInterfaceConnect(net_if_handle_t *netif, bool dhcp_mode, void *credential, net_wifi_mode_t mode)
{
  int32_t ret = NET_ERROR_GENERIC;

  /* Set credentials for Wifi modules*/
  if (NET_INTERFACE_IS_WIFI(netif))
  {
    net_wifi_credentials_t *wifi_credential = (net_wifi_credentials_t *) credential;
    if (wifi_credential->psk == NULL)
    {
      wifi_credential->security_mode = NET_WIFI_SM_OPEN;
    }
    if (NET_WIFI_MODE_AP == mode)
    {
      dhcp_mode = 0;
      wifi_credential->ssid = "MyHotSpot";
      wifi_credential->psk = " ";
      wifi_credential->security_mode = NET_WIFI_SM_OPEN;;
    }

    net_wifi_set_credentials(netif, wifi_credential);
    net_wifi_set_access_mode(netif, mode);
    net_wifi_set_access_channel(netif, 8u);
    net_wifi_set_ap_max_connections(netif, 4u);
    NET_DBG_PRINT("main ... %p %p %s\r\n", wifi_credential, wifi_credential->ssid, wifi_credential->ssid);
  }

  /* Set credentials for Cellular modules*/
  if (NET_INTERFACE_IS_CELLULAR(netif))
  {
    net_cellular_credentials_t *cellular_credential = (net_cellular_credentials_t *) credential;
    net_cellular_set_credentials(netif, cellular_credential);
    NET_DBG_PRINT("main ... %p %p %s\r\n", cellular_credential, cellular_credential->apn, cellular_credential->apn);
  }

  ret = net_if_start(netif);
  if (NET_OK == ret)
  {
    ret = net_if_wait_state(netif, NET_STATE_READY, NET_STATE_TRANSITION_TIMEOUT);
    if (NET_OK == ret)
    {
      if (dhcp_mode)
      {
        net_if_set_dhcp_mode(netif, true);
      }
      else
      {
        net_ip_addr_t ip = NET_IPADDR4_INIT_BYTES(192, 168, 1, 1);
        net_ip_addr_t gw = NET_IPADDR4_INIT_BYTES(192, 168, 1, 1);
        net_ip_addr_t msk = NET_IPADDR4_INIT_BYTES(255, 255, 254, 0);
        net_if_set_dhcp_mode(netif, false);
        net_if_set_dhcp_server_mode(netif, true);
        net_if_set_ipaddr(netif, ip, gw, msk);
      }

      ret = net_if_connect(netif);
      if (NET_OK == ret)
      {
        ret = net_if_wait_state(netif, NET_STATE_CONNECTED, NET_STATE_TRANSITION_TIMEOUT);
      }
    }
    if (NET_OK != ret)
    {
      printf("ERROR: Cannot connect interface\r\n");
      printf("  If not done , Please set your connection parameter in main_app.c (net_wifi_net_wifi_registred_hotspot) !\r\n");
    }
  }
  if (NET_OK != ret)
  {
    printf("ERROR: Cannot connect interface !\r\n");
    printf("  If not done , Please set your connection parameter in main_app.c (net_wifi_net_wifi_registred_hotspot) !\r\n");
  }
}

void NetInterfaceDisconnect(net_if_handle_t *netif)
{
  int32_t ret = net_if_disconnect(netif) ;
  if (NET_OK == ret)
  {
    ret = net_if_wait_state(netif, NET_STATE_READY, NET_STATE_TRANSITION_TIMEOUT);
    if (NET_OK == ret)
    {
      ret = net_if_stop(netif);
      if (NET_OK == ret)
      {
        ret = net_if_wait_state(netif, NET_STATE_INITIALIZED, NET_STATE_TRANSITION_TIMEOUT);
      }
    }
  }
  if (NET_OK != ret)
  {
    printf("ERROR: Cannot disconnect interface !\r\n");
  }
  return;
}
