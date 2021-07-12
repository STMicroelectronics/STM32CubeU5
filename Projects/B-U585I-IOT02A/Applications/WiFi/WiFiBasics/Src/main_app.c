/**
  ******************************************************************************
  * @file           : main_app.c
  * @brief          : main_app program body
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
#include "main.h"
#include "net_connect.h"
#include "net_interface.h"
#include "console.h"

/* Exported functions --------------------------------------------------------*/
void main_app(void);

int32_t ping_cmd(int32_t argc, char **argv);
int32_t scan_cmd(int32_t argc, char **argv);
int32_t test_echo_server(int32_t argc, char **argv);
int32_t http_download_cmd(int32_t argc, char *argv[]);

cmd_t cmdlist[] =
{
  {"help",                                         "print this message",          help_cmd},
  {"quit",                                                       "quit",          quit_cmd},
  {"scan",                                                  "Wifi Scan",          scan_cmd},
  {"ping",                    "Ping <hostname> (www.st.com by default)",          ping_cmd},
  {"echo",                                           "Echo Server Test",  test_echo_server},
  {"http", "http <url> (http://public.st.free.fr/500MO.bin by default)", http_download_cmd},
  {  NULL,                                                         NULL,              NULL}
};

/* Private typedef -----------------------------------------------------------*/
int32_t mx_wifi_driver(net_if_handle_t *pnetif);
#define NET_INTERFACE mx_wifi_driver


static const ap_t net_wifi_net_wifi_registred_hotspot[];
static void hnet_notify(void *context, uint32_t event_class, uint32_t event_id, void *event_data);

static void hnet_notify(void *context, uint32_t event_class, uint32_t event_id, void *event_data)
{
  net_if_handle_t *netif = context;

  UNUSED(event_data);

  if (NET_EVENT_STATE_CHANGE == event_class)
  {
    net_state_t new_state = (net_state_t) event_id;
    switch (new_state)
    {
      case NET_STATE_INITIALIZED:
        printf("- Network Interface initialized:\n");
        break;

      case NET_STATE_STARTING:
        printf("- Network Interface starting:\n");
        break;

      case NET_STATE_READY:
        printf("- Network Interface ready:\n");
        printf(" - Device Name    : %s. \n", netif->DeviceName);
        printf(" - Device ID      : %s. \n", netif->DeviceID);
        printf(" - Device Version : %s. \n", netif->DeviceVer);
        printf(" - MAC address    : %02X.%02X.%02X.%02X.%02X.%02X\n",
               netif->macaddr.mac[0], netif->macaddr.mac[1], netif->macaddr.mac[2],
               netif->macaddr.mac[3], netif->macaddr.mac[4], netif->macaddr.mac[5]);
        break;

      case NET_STATE_CONNECTING:
        printf("- Network Interface connecting:\n");
        break;

      case NET_STATE_CONNECTED:
        printf("\n- Network Interface connected:\n");
        printf(" - IP address : %s. \n\n", NET_NTOA(&netif->ipaddr));
        break;

      case NET_STATE_DISCONNECTING:
        printf("- Network Interface disconnecting\n");
        break;

      case NET_STATE_STOPPING:
        printf("- Network Interface stopping\n");
        break;

      case NET_STATE_DEINITIALIZED:
        printf("- Network Interface de-initialized\n");
        break;

      case NET_STATE_CONNECTION_LOST:
        printf("- Network Interface connection lost\n");
        break;

      default:
        break;
    }
  }
}


void main_app(void)
{
  net_if_handle_t *netif;

  printf("\n\n\n\n\r#### Welcome to WiFi Basics Application #####\n");

  printf("# build: %s-%s, %s, %s %s\r\n",
#if MX_WIFI_USE_SPI==1
         "SPI",
#else
         "UART",
#endif /*MX_WIFI_USE_SPI*/
#if MX_WIFI_USE_CMSIS_OS==1
         "RTOS",
#else
         "NoOS",
#endif /* MX_WIFI_USE_CMSIS_OS */
#if MX_WIFI_NETWORK_BYPASS_MODE==1
         "Network bypass",
#else
         "Network on module",
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
         __TIME__, __DATE__);

  /* Start the network interface. */
  netif = NetInterfaceOn(NET_INTERFACE, hnet_notify);

  if (NET_INTERFACE_IS_WIFI(netif))
  {
    static net_wifi_credentials_t WifiCredentials;
    WifiCredentials.ssid = "SSID-Unset";
    WifiCredentials.psk = "none";
    WifiCredentials.security_mode = 0;

    scan_cmd(0, NULL);
    NetWifiGetDefaultStation(&WifiCredentials, net_wifi_net_wifi_registred_hotspot);
    NetInterfaceConnect(netif, true, &WifiCredentials, NET_WIFI_MODE_STA);
  }

  printf("\n##### Please enter one of the following command:\n\n");
  help_cmd(0, NULL);

  /* Application body. */
  console("App>", cmdlist);

  /* Clean up the network interface. */
  NetInterfaceDisconnect(netif);
  NetInterfaceOff(netif);

#ifdef NET_ALLOC_DEBUG
  net_alloc_report();
#endif /* NET_ALLOC_DEBUG */
}

static const ap_t net_wifi_net_wifi_registred_hotspot[] =
{
  {         WIFI_SSID, WIFI_PASSWORD },
  {              NULL,          NULL }
};
