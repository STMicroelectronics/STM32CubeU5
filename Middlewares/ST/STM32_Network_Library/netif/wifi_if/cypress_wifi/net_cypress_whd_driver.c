/**
  ******************************************************************************
  * @file    net_cypress_whd_driver.c
  * @author  MCD Application Team
  * @brief   Wi-Fi specific BSD-like socket wrapper
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
#include "net_ip_lwip.h"
#include "lwip/etharp.h"
#include "lwip/tcpip.h"

#include "whd.h"
#include "whd_debug.h"
#include "whd_resource_api.h"
#include "whd_wifi_api.h"
#include "whd_network_types.h"
#include "whd_types_int.h"
#include "whd_int.h"

#define NET_CYPRESS_MAX_INTERFACE       4

#define NET_WHD_LINK_DOWN_FLAG              0x0001
#define NET_WHD_LINK_UP_FLAG                0x0002

static const whd_event_num_t sta_link_change_events[] =
{
  WLC_E_SET_SSID,
  WLC_E_LINK,
  WLC_E_AUTH,
  WLC_E_ASSOC,
  WLC_E_DEAUTH_IND,
  WLC_E_DISASSOC_IND,
  WLC_E_DISASSOC,
  WLC_E_REASSOC,
  WLC_E_PSK_SUP,
  WLC_E_ACTION_FRAME_COMPLETE,
  WLC_E_NONE
};


typedef struct net_cypress_key_s
{
  void   *key;
  void   *value;
} net_cypress_key_t;

static net_cypress_key_t netif2ifp[NET_CYPRESS_MAX_INTERFACE];
static uint32_t CypressAliveInterfaceCount = 0;
static whd_driver_t WhdDriver;
static osEventFlagsId_t LinkStatusFlags = NULL;
static osThreadId_t LinkThread = NULL;
static uint16_t EventIndex = 0xFF;

/* Create and boot WHD driver */
cy_rslt_t whd_boot(whd_driver_t *pWhdDriver);
cy_rslt_t whd_powerdown(whd_driver_t *pWhdDriver);


/* Global constructor of the WiFi network interface. */
int32_t cypress_whd_net_driver(net_if_handle_t *pnetif);



static err_t low_level_output(struct netif *netif, struct pbuf *p);
static err_t low_level_init(struct netif *netif);
static int32_t net_cypress_whd_if_init(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_deinit(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_start(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_stop(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_connect(net_if_handle_t *pnetif);
static int32_t net_cypress_whd_if_disconnect(net_if_handle_t *pnetif);

static int32_t net_cypress_whd_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid);
static int32_t net_cypress_whd_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *results, uint8_t number);

static int32_t net_cypress_whd_id_switch_mode(net_if_handle_t *pnetif, net_wifi_mode_t target_mode);

static void *net_cypress_whd_event_handler(whd_interface_t ifp, const whd_event_header_t *event_header,
                                           const uint8_t *event_data, void *handler_user_data)
{
  UNUSED_PARAMETER(event_data);

  if (event_header->bsscfgidx >= WHD_INTERFACE_MAX)
  {
    NET_DBG_PRINT("event_header: Bad interface\n");
    return NULL;
  }

  if ((event_header->event_type == WLC_E_DEAUTH_IND) ||
      (event_header->event_type == WLC_E_DISASSOC_IND) ||
      ((event_header->event_type == WLC_E_PSK_SUP) &&
       (event_header->status == WLC_SUP_KEYED) &&
       (event_header->reason == WLC_E_SUP_DEAUTH)) ||
      ((event_header->event_type == WLC_E_LINK) &&
       (event_header->reason == WLC_E_REASON_LOW_RSSI)))
  {
    osEventFlagsSet(LinkStatusFlags, NET_WHD_LINK_DOWN_FLAG);
    return handler_user_data;
  }

  if (((event_header->event_type == WLC_E_PSK_SUP) &&
       (event_header->status == WLC_SUP_KEYED) &&
       (event_header->reason == WLC_E_SUP_OTHER)) ||
      (whd_wifi_is_ready_to_transceive(ifp) == WHD_SUCCESS))
  {
    osEventFlagsSet(LinkStatusFlags, NET_WHD_LINK_UP_FLAG);

    return handler_user_data;
  }

  return handler_user_data;
}

static void whd_link_thread_handler(void *user_data)
{
  struct netif * const netif = (struct netif *) user_data;
  uint32_t flags;
  for (;;)
  {
    flags = osEventFlagsWait(LinkStatusFlags, (NET_WHD_LINK_UP_FLAG | NET_WHD_LINK_DOWN_FLAG),
                             osFlagsWaitAny, osWaitForever);

    if (flags & osFlagsError)
    {
      continue;
    }
    if ((flags & NET_WHD_LINK_DOWN_FLAG) && netif_is_link_up(netif))
    {
      netif_set_link_down(netif);
    }
    else if ((flags & NET_WHD_LINK_UP_FLAG) && !netif_is_link_up(netif))
    {
      netif_set_link_up(netif);
    }
  }
}


#if LWIP_IGMP

static err_t lwip_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
  whd_mac_t mac;
  whd_interface_t ifp = whd_get_primary_interface(WhdDriver);

  /* Build multicast MAC address from IP */
  mac.octet[0] = 0x01;
  mac.octet[1] = 0x00;
  mac.octet[2] = 0x5E;
  mac.octet[3] = ip4_addr2(group) & 0x7F;
  mac.octet[4] = ip4_addr3(group);
  mac.octet[5] = ip4_addr4(group);

  switch (action)
  {
    case NETIF_DEL_MAC_FILTER:
      whd_wifi_unregister_multicast_address(ifp, &mac);
      NET_DBG_PRINT("WHD unregister_multicast_address: %02x.%02x.%02x.%02x.%02x.%02x\n", mac.octet[0], mac.octet[1],
                    mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);
      break;

    case NETIF_ADD_MAC_FILTER:
      whd_wifi_register_multicast_address(ifp, &mac);
      NET_DBG_PRINT("WHD register_multicast_address: %02x.%02x.%02x.%02x.%02x.%02x\n", mac.octet[0], mac.octet[1],
                    mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);
      break;
  }
  return ERR_OK;

}
#endif /* LWIP_IGMP */

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
err_t lwip_mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
  whd_mac_t mac;
  whd_interface_t ifp = whd_get_primary_interface(WhdDriver);
  uint32_t group32 = NET_NTOHL(group->addr[3]);

  /* Build multicast MAC address from IP */
  mac.octet[0] = 0x33;
  mac.octet[1] = 0x33;
  mac.octet[2] = group32 >> 24U;
  mac.octet[3] = group32 >> 16U;
  mac.octet[4] = group32 >> 8U;
  mac.octet[5] = group32;

  switch (action)
  {
    case NETIF_DEL_MAC_FILTER:
      whd_wifi_unregister_multicast_address(ifp, &mac);
      NET_DBG_PRINT("WHD unregister_multicast_address: %02x.%02x.%02x.%02x.%02x.%02x\n", mac.octet[0], mac.octet[1],
                    mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);
      break;
    case NETIF_ADD_MAC_FILTER:
      whd_wifi_register_multicast_address(ifp, &mac);
      NET_DBG_PRINT("WHD register_multicast_address: %02x.%02x.%02x.%02x.%02x.%02x\n", mac.octet[0], mac.octet[1],
                    mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);
      break;
  }

  return ERR_OK;
}
#endif /* NET_USE_IPV6 */

int32_t cypress_whd_net_driver(net_if_handle_t *pnetif)
{
  int32_t ret;
  /* Initialize the LwIP library here if not already done by another network interface. */
  net_ip_init();

  ret = net_cypress_whd_if_init(pnetif);
  return ret;
}

static int32_t net_cypress_whd_if_init(net_if_handle_t *pnetif)
{
  int32_t ret = NET_OK;

  net_if_drv_t *pdrv = NET_MALLOC(sizeof(*pdrv));

  if (pdrv != NULL)
  {
    pdrv->if_class = NET_INTERFACE_CLASS_WIFI;
    pdrv->if_init = net_cypress_whd_if_init;
    pdrv->if_deinit = net_cypress_whd_if_deinit;
    pdrv->if_start = net_cypress_whd_if_start;
    pdrv->if_stop = net_cypress_whd_if_stop;
    pdrv->if_connect = net_cypress_whd_if_connect;
    pdrv->if_disconnect = net_cypress_whd_if_disconnect;
    pdrv->pping = icmp_ping;
    pnetif->pdrv = pdrv;
    pdrv->extension.wifi = NET_MALLOC(sizeof(*pdrv->extension.wifi));
    if (NULL == pdrv->extension.wifi)
    {
      NET_DBG_ERROR("Can't allocate memory for wifi extension\n");
      NET_FREE(pdrv);
      ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
      (void) memset(pdrv->extension.wifi, 0, sizeof(*pdrv->extension.wifi));
      pdrv->extension.wifi->scan = net_cypress_whd_scan;
      pdrv->extension.wifi->get_scan_results = net_cypress_whd_scan_result;
      pdrv->extension.wifi->switch_mode = net_cypress_whd_id_switch_mode;

      if (CypressAliveInterfaceCount == 0)
      {
        /* Boot cypress module and start whd driver for very first interface */
        if (WHD_SUCCESS != whd_boot(&WhdDriver))
        {
          NET_DBG_ERROR("Can't perform initialization of WHD driver and module\n");
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }

        NET_DBG_PRINT("WHD init driver done\n");
        (void) memset(netif2ifp, 0, sizeof(netif2ifp));

        if (NET_OK == ret)
        {
          /* create flags for Wifi UP DOWN event before starting WIFI */
          LinkStatusFlags = osEventFlagsNew(NULL);

          if (WHD_SUCCESS != whd_wifi_on(WhdDriver, (whd_interface_t *) &pdrv->extension.wifi->ifp))
          {
            NET_DBG_PRINT("Failed when creating WIFI default interface\n");
            NET_FREE(pdrv->extension.wifi);
            NET_FREE(pdrv);
            ret = NET_ERROR_MODULE_INITIALIZATION;
          }
          else
          {
            NET_DBG_PRINT("WHD init interface done\n");
            CypressAliveInterfaceCount++;
            ret = NET_OK;
          }
        }
      }
      else
      {
        whd_mac_t mac_addr = {{0xA0, 0xC9, 0xA0, 0X3D, 0x43, 0x41}};
        if (WHD_SUCCESS != whd_add_secondary_interface(WhdDriver, &mac_addr,
                                                       (whd_interface_t *) &pdrv->extension.wifi->ifp))
        {
          NET_DBG_PRINT("Failed when creating WIFI default interface\n");
          NET_FREE(pdrv->extension.wifi);
          NET_FREE(pdrv);
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
        {
          NET_DBG_PRINT("WHD init interface done\n");
          CypressAliveInterfaceCount++;
          ret = NET_OK;
        }
      }
    }
  }
  else
  {
    NET_DBG_ERROR("Can't allocate memory for WHD driver class\n");
    ret = NET_ERROR_NO_MEMORY;
  }

  return ret;
}


static void convert_credential(const net_wifi_credentials_t *credentials, whd_security_t *privacy, whd_ssid_t *myssid)
{
  *privacy = (whd_security_t) credentials->security_mode;
  strcpy((char *) myssid->value, (char *) credentials->ssid);
  myssid->length = strlen((char *)myssid->value);
}

static int32_t net_cypress_whd_if_start_sta(net_if_handle_t *pnetif)
{
  int32_t        ret = 0;
  whd_security_t privacy;
  whd_ssid_t     myssid;
  const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;
  whd_wifi_set_event_handler((whd_interface_t)pnetif->pdrv->extension.wifi->ifp,
                             (uint32_t const *) sta_link_change_events,
                             net_cypress_whd_event_handler, NULL, &EventIndex);

  convert_credential(credentials, &privacy, &myssid);

  NET_DBG_PRINT("Joining ... %s\n", myssid.value);

  ret = whd_wifi_join((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, (whd_ssid_t const *) &myssid, privacy,
                      (uint8_t const *) credentials->psk, strlen(credentials->psk));
  if (ret != 0)
  {
    NET_DBG_ERROR("can't join %s\n", myssid.value);
    ret = NET_ERROR_MODULE_INITIALIZATION;
  }
  else
  {
    NET_DBG_PRINT("Joined Access point %s\n", myssid.value);
    ret = net_ip_add_if(pnetif, low_level_init, NET_ETHERNET_FLAG_DEFAULT_IF);
    if (ret == NET_OK)
    {
      (void) strncpy(pnetif->DeviceName, "Wlan WHD murata 1LD", sizeof(pnetif->DeviceName));
      (void) strncpy(pnetif->DeviceID, "Unknown", sizeof(pnetif->DeviceID));
      (void) strncpy(pnetif->DeviceVer, "Unknown", sizeof(pnetif->DeviceVer));
      (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
      netif_set_link_up(pnetif->netif);
    }
    else
    {
      NET_DBG_ERROR("Can't add interface (netif)\n");
    }
  }
  return ret;
}

static int32_t net_cypress_whd_if_start_ap(net_if_handle_t *pnetif)
{
  int32_t        ret = 0;
  whd_security_t privacy;
  whd_ssid_t     myssid;
  const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;

  NET_DBG_PRINT("Init Access Point ... %s\n", myssid.value);
  convert_credential(credentials, &privacy, &myssid);

  ret = whd_wifi_init_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, &myssid, privacy,
                         (uint8_t const *) credentials->psk, strlen(credentials->psk),
                         pnetif->pdrv->extension.wifi->access_channel);
  if (ret != 0)
  {
    NET_DBG_ERROR("Can't init access point %s\n", myssid.value);
    ret = NET_ERROR_MODULE_INITIALIZATION;
  }
  else
  {
    if (pnetif->pdrv->extension.wifi->information_element != NULL)
    {
      ret = whd_wifi_manage_custom_ie((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, WHD_ADD_CUSTOM_IE,
                                      pnetif->pdrv->extension.wifi->information_element->oui,
                                      pnetif->pdrv->extension.wifi->information_element->subtype,
                                      (const void *)pnetif->pdrv->extension.wifi->information_element->data,
                                      pnetif->pdrv->extension.wifi->information_element->length,
                                      VENDOR_IE_PROBE_RESPONSE | VENDOR_IE_BEACON);
    }

    ret = whd_wifi_start_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp);
    if (ret != 0)
    {
      NET_DBG_ERROR("Can't start access point %s\n", myssid.value);
      ret = NET_ERROR_MODULE_INITIALIZATION;
    }
    else
    {
      NET_DBG_PRINT("Start Access point %s\n", myssid.value);
      ret = net_ip_add_if(pnetif, low_level_init, NET_ETHERNET_FLAG_DEFAULT_IF);
      if (ret == NET_OK)
      {
        (void) strncpy(pnetif->DeviceName, "Wlan WHD murata 1LD", sizeof(pnetif->DeviceName));
        (void) strncpy(pnetif->DeviceID, "Unknown", sizeof(pnetif->DeviceID));
        (void) strncpy(pnetif->DeviceVer, "Unknown", sizeof(pnetif->DeviceVer));
        (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        netif_set_link_up(pnetif->netif);
      }
      else
      {
        NET_DBG_ERROR("Can't add interface (netif)\n");
      }
    }
  }
  return ret;
}

static int32_t net_cypress_whd_id_switch_mode(net_if_handle_t *pnetif, net_wifi_mode_t target_mode)
{
  int32_t ret = 0;
  whd_security_t privacy;
  whd_ssid_t     myssid;
  const net_wifi_credentials_t *credentials = pnetif->pdrv->extension.wifi->credentials;

  switch (target_mode)
  {
    case NET_WIFI_MODE_STA:
      if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_AP)
      {
        NET_DBG_PRINT("Switching from AP to STA mode\n");
        if (pnetif->pdrv->extension.wifi->information_element != NULL)
        {
          ret = whd_wifi_manage_custom_ie((whd_interface_t)pnetif->pdrv->extension.wifi->ifp,
                                          WHD_REMOVE_CUSTOM_IE,
                                          pnetif->pdrv->extension.wifi->information_element->oui,
                                          pnetif->pdrv->extension.wifi->information_element->subtype,
                                          (const void *)pnetif->pdrv->extension.wifi->information_element->data,
                                          pnetif->pdrv->extension.wifi->information_element->length,
                                          VENDOR_IE_PROBE_RESPONSE | VENDOR_IE_BEACON);
        }
        (void) whd_wifi_stop_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp);
        convert_credential(credentials, &privacy, &myssid);

        NET_DBG_PRINT("Joining ... %s\n", myssid.value);

        ret = whd_wifi_join((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, (whd_ssid_t const *) &myssid, privacy,
                            (uint8_t const *) credentials->psk, strlen(credentials->psk));
        if (ret != 0)
        {
          NET_DBG_ERROR("can't join %s\n", myssid.value);
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
        {
          NET_DBG_PRINT("Joined Access point %s\n", myssid.value);
          (void) whd_wifi_set_event_handler((whd_interface_t)pnetif->pdrv->extension.wifi->ifp,
                                            (uint32_t const *) sta_link_change_events,
                                            net_cypress_whd_event_handler, NULL, &EventIndex);
          pnetif->pdrv->extension.wifi->mode = NET_WIFI_MODE_STA;
          ret = NET_OK;
        }
      }
      else
      {
        ret = NET_OK;
      }
      break;

    case NET_WIFI_MODE_AP:
      if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
      {
        NET_DBG_PRINT("Switching from STA to AP mode\n");
        netif_set_link_down(pnetif->netif);
        (void) whd_wifi_leave(pnetif->pdrv->extension.wifi->ifp);
        (void) whd_wifi_deregister_event_handler(pnetif->pdrv->extension.wifi->ifp, EventIndex);

        NET_DBG_PRINT("Init Access Point ... %s\n", myssid.value);
        convert_credential(credentials, &privacy, &myssid);

        ret = whd_wifi_init_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, &myssid, privacy,
                               (uint8_t const *) credentials->psk, strlen(credentials->psk),
                               pnetif->pdrv->extension.wifi->access_channel);
        if (ret != 0)
        {
          NET_DBG_ERROR("Can't init access point %s\n", myssid.value);
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
        {
          if (pnetif->pdrv->extension.wifi->information_element != NULL)
          {
            ret = whd_wifi_manage_custom_ie((whd_interface_t)pnetif->pdrv->extension.wifi->ifp, WHD_ADD_CUSTOM_IE,
                                            pnetif->pdrv->extension.wifi->information_element->oui,
                                            pnetif->pdrv->extension.wifi->information_element->subtype,
                                            (const void *)pnetif->pdrv->extension.wifi->information_element->data,
                                            pnetif->pdrv->extension.wifi->information_element->length,
                                            VENDOR_IE_PROBE_RESPONSE | VENDOR_IE_BEACON);
          }

          ret = whd_wifi_start_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp);
          if (ret != 0)
          {
            NET_DBG_ERROR("can't start access point %s\n", myssid.value);
            ret = NET_ERROR_MODULE_INITIALIZATION;
          }
          else
          {
            NET_DBG_PRINT("Start Access point %s\n", myssid.value);
            (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
            netif_set_link_up(pnetif->netif);
            pnetif->pdrv->extension.wifi->mode = NET_WIFI_MODE_AP;
            ret = NET_OK;
          }
        }
      }
      else
      {
        ret = NET_OK;
      }
      break;

    default:
      break;
  }

  return ret;
}

int32_t net_cypress_whd_if_start(net_if_handle_t *pnetif)
{
  int32_t ret = 0;
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    ret =  net_cypress_whd_if_start_sta(pnetif);
  }
  else
  {
    ret =  net_cypress_whd_if_start_ap(pnetif);
  }
  return ret;
}

static int32_t net_cypress_whd_if_connect(net_if_handle_t *pnetif)
{
  int32_t ret;

  ret = net_ip_connect(pnetif);
  if (ret != NET_OK)
  {
    NET_DBG_ERROR("Failed to connect\n");
    ret = NET_ERROR_NO_CONNECTION;
  }

  return ret;
}


static int32_t net_cypress_whd_if_disconnect(net_if_handle_t *pnetif)
{
  int32_t ret;
  ret = net_ip_disconnect(pnetif);
  if (ret == NET_OK)
  {
    ret = net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
  }
  return ret;
}

static int32_t net_cypress_whd_if_stop(net_if_handle_t *pnetif)
{
  int32_t ret;

  ret = net_ip_remove_if(pnetif, NULL);
  if (ret == NET_OK)
  {
    ret = net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
  }
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    whd_wifi_leave(pnetif->pdrv->extension.wifi->ifp);
    whd_wifi_deregister_event_handler(pnetif->pdrv->extension.wifi->ifp, EventIndex);
  }
  else
  {
    whd_wifi_stop_ap((whd_interface_t)pnetif->pdrv->extension.wifi->ifp);
  }
  return ret;
}

static int32_t net_cypress_whd_if_deinit(net_if_handle_t *pnetif)
{
  int32_t ret = NET_OK;
  uint32_t i;

  /* Switch off the WiFi. */
  whd_wifi_off(pnetif->pdrv->extension.wifi->ifp);

  for (i = 0; i < NET_CYPRESS_MAX_INTERFACE; i++)
  {
    if (netif2ifp[i].key == pnetif->pdrv->extension.wifi->ifp)
    {
      break;
    }
  }

  if (i == NET_CYPRESS_MAX_INTERFACE)
  {
    WPRINT_WHD_DEBUG(("Couldn't find the interface\n"));
    return ERR_VAL;
  }
  else
  {
    netif2ifp[i].key = NULL;
    netif2ifp[i].value = NULL;

    if (CypressAliveInterfaceCount == 1)
    {
      /* Deletes all the interface and De-init the whd, free whd_driver memory. */
      whd_deinit(pnetif->pdrv->extension.wifi->ifp);
      whd_powerdown(&WhdDriver);
    }

    CypressAliveInterfaceCount--;
    NET_FREE(pnetif->pdrv->extension.wifi);
    NET_FREE(pnetif->pdrv);
    pnetif->pdrv = NULL;
  }
  return ret;
}

static err_t low_level_init(struct netif *netif)
{
  err_t ret = ERR_VAL;
  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  net_if_handle_t * const pnetif = netif->state;
  whd_interface_t ifp = pnetif->pdrv->extension.wifi->ifp;

  /* to retrieve back netif from ifp */
  for (int32_t i = 0; i < NET_CYPRESS_MAX_INTERFACE; i++)
  {
    if (netif2ifp[i].key == NULL)
    {
      netif2ifp[i].key = ifp;
      netif2ifp[i].value = netif;
      ret = (err_t) ERR_OK;
      break;
    }
  }

  netif->name[0] = 'c';
  netif->name[1] = 'y';

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...)
   */
  netif->output = etharp_output;

  netif->linkoutput = low_level_output;


  /* Set MAC hardware address length (6)*/
  netif->hwaddr_len = (u8_t) ETHARP_HWADDR_LEN;

  /* Setup the physical address of this IP instance. */
  if (whd_wifi_get_mac_address(ifp, (whd_mac_t *)netif->hwaddr) != WHD_SUCCESS)
  {
    WPRINT_WHD_DEBUG(("Couldn't get MAC address\n"));
    return ERR_VAL;
  }
  WPRINT_WHD_DEBUG((" MAC address %x.%x.%x.%x.%x.%x\n",
                    netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2],
                    netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]));

  /* Set Maximum Transfer Unit */
  netif->mtu = (u16_t) WHD_PAYLOAD_MTU;

  /* Set device capabilities. Don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one. */
  netif->flags = (u8_t)(NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET);

  /* Do whatever else is needed to initialize interface. */
#if LWIP_IGMP
  netif->flags |= NETIF_FLAG_IGMP;
  netif_set_igmp_mac_filter(netif, lwip_igmp_mac_filter);
#endif /* LWIP_IGMP */

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  netif->output_ip6 = ethip6_output;
  netif_set_mld_mac_filter(netif, lwip_mld_mac_filter);
  netif->flags |= NETIF_FLAG_MLD6;
#endif /* NET_USE_IPV6 */

  /* Register a handler for any address changes and when interface goes up or down. */
  netif_set_status_callback(netif, net_ip_status_cb);
  netif_set_link_callback(netif, net_ip_status_cb);

  if (LinkStatusFlags == NULL)
  {
    LinkStatusFlags = osEventFlagsNew(NULL);
  }

  if (LinkThread == NULL)
  {
    osThreadAttr_t attributes = {0};
    attributes.name = "LinkThr";
    attributes.stack_size = WHD_THREAD_STACK_SIZE;
    attributes.priority = osPriorityBelowNormal;
    LinkThread = osThreadNew(whd_link_thread_handler, netif, &attributes);
  }

  return ret;
}


/**
  * This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  net_if_handle_t * const pnetif = netif->state;
  whd_interface_t ifp = (whd_interface_t)pnetif->pdrv->extension.wifi->ifp;

  pbuf_ref(p);

  LWIP_ASSERT("No chained buffers", ((p->next == NULL) && ((p->tot_len == p->len))));

  whd_network_send_ethernet_data(ifp, p);

  LINK_STATS_INC(link.xmit);

  return (err_t) ERR_OK;
}

/**
  * This function should be called when a packet is ready to be read
  * from the interface. It uses the function low_level_input() that
  * should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param p : the incoming Ethernet packet
  */
void cy_network_process_ethernet_data(whd_interface_t interface, whd_buffer_t buff);
void cy_network_process_ethernet_data(whd_interface_t interface, whd_buffer_t buff)
{

  struct eth_hdr *ethernet_header;
  struct netif   *tmp_netif;
  u8_t            result;
  uint16_t        ethertype;
  struct pbuf *buffer = (struct pbuf *) buff;

  if (buffer == NULL)
  {
    return;
  }

  /* Points to packet payload, which starts with an Ethernet header. */
  ethernet_header = (struct eth_hdr *) buffer->payload;

  ethertype = lwip_htons(ethernet_header->type);

#ifdef FILTER
  if (filter_ethernet_packet_callback != NULL && filter_ethertype == ethertype && filter_interface == interface)
  {
    filter_ethernet_packet_callback(buffer->payload, filter_userdata);
  }
#endif /* FILTER */

  /* Check if this is an 802.1Q VLAN tagged packet */
  if (ethertype == WHD_ETHERTYPE_8021Q)
  {
    /**
      * Need to remove the 4 octet VLAN Tag, by moving src and dest addresses 4 octets to the right,
      * and then read the actual ethertype. The VLAN ID and priority fields are currently ignored.
      */
    uint8_t temp_buffer[12];
    memcpy(temp_buffer, buffer->payload, 12);
    memcpy(((uint8_t *) buffer->payload) + 4, temp_buffer, 12);

    buffer->payload = ((uint8_t *) buffer->payload) + 4;
    buffer->len = (u16_t)(buffer->len - 4);

    ethernet_header = (struct eth_hdr *) buffer->payload;
    ethertype = lwip_htons(ethernet_header->type);
  }
#ifdef DEEP_SLEEP
  if (WHD_DEEP_SLEEP_IS_ENABLED() && (WHD_DEEP_SLEEP_SAVE_PACKETS_NUM != 0))
  {
    if (wiced_deep_sleep_save_packet(buffer, interface))
    {
      return;
    }
  }
#endif /* DEEP_SLEEP */

  switch (ethertype)
  {
    case WHD_ETHERTYPE_IPv4:
    case WHD_ETHERTYPE_IPv6:
    case WHD_ETHERTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */

      /* Find the netif object matching the provided interface */
      tmp_netif = NULL;
      for (int32_t i = 0; i < NET_CYPRESS_MAX_INTERFACE; i++)
      {
        if (netif2ifp[i].key == interface)
        {
          tmp_netif = netif2ifp[i].value;
          break;
        }
      }

      /*NET_DBG_PRINT("Recv %d for netif %x  ifp %x\n",buffer->len,tmp_netif,interface);*/

      if (tmp_netif == NULL)
      {
        NET_DBG_PRINT("This buffer is not for this interface !\n");

        /* Received a packet for a network interface is not initialized Cannot do anything with packet
        - just drop it. */
        result = pbuf_free(buffer);
        LWIP_ASSERT("Failed to release packet buffer", (result != (u8_t)0));
        buffer = NULL;
        return;
      }
#if 0
      NET_DBG_PRINT("process input packet ethertype %x size %d\n", ethertype, buffer->len);
#endif /* debug */

      /* Send to packet to tcpip_thread to process */

      if (tcpip_input(buffer, tmp_netif) != ERR_OK)
      {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));

        /* Stop lint warning - packet has not been released in this case */ /*@-usereleased@*/
        result = pbuf_free(buffer);
        /*@+usereleased@*/
        LWIP_ASSERT("Failed to release packet buffer", (result != (u8_t)0));
        buffer = NULL;
      }
      break;
#if 0
    /* Not supported today */
    case WHD_ETHERTYPE_EAPOL:
      whd_eapol_receive_eapol_packet(buffer, interface);
      break;
#endif /* 0 */
    default:
      result = pbuf_free(buffer);
      LWIP_ASSERT("Failed to release packet buffer", (result != (u8_t)0));
      buffer = NULL;
      break;
  }
}


static int32_t net_cypress_whd_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid)
{
  (void) pnetif;
  (void) mode;
  (void) ssid;
  return NET_OK;
}


static int32_t net_cypress_whd_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *scan_bss, uint8_t number)
{
  int32_t ret = NET_ERROR_GENERIC;
  whd_sync_scan_result_t *scan_result;

  if ((NULL == scan_bss) || (0 == number))
  {
    return NET_ERROR_PARAMETER;
  }

  scan_result = (whd_sync_scan_result_t *)NET_MALLOC(sizeof(*scan_result) * number);
  if (NULL == scan_result)
  {
    return NET_ERROR_NO_MEMORY;
  }
  else
  {
    uint32_t ap_count;
    whd_sync_scan_result_t *scan_result_info = scan_result;

    memset(scan_result, 0, sizeof(whd_sync_scan_result_t) * number);
    ap_count = whd_wifi_scan_synch((whd_interface_t) pnetif->pdrv->extension.wifi->ifp, scan_result, number);

    for (uint32_t i = 0; i < ap_count; i++)
    {
      memset(scan_bss, 0, sizeof(*scan_bss));
      memcpy(scan_bss->ssid.value, (void *) scan_result_info->SSID.value, scan_result_info->SSID.length);
      scan_bss->ssid.value[scan_result_info->SSID.length] = 0;
      scan_bss->ssid.length = scan_result_info->SSID.length;

      scan_bss->security = scan_result_info->security;
      memcpy(&scan_bss->bssid, scan_result_info->BSSID.octet, NET_WIFI_MAC_ADDRESS_SIZE);
      scan_bss->rssi = (int8_t)scan_result_info->signal_strength;
      scan_bss->channel = scan_result_info->channel;
      memcpy(scan_bss->country, ".CN", 4);

      scan_bss++;
      scan_result_info++;
    }
    ret = ap_count;
  }

  NET_FREE((void *) scan_result);

  return ret;
}
