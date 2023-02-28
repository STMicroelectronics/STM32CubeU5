/**
  ******************************************************************************
  * @file    net_class_extension.c
  * @author  MCD Application Team
  * @brief   Specific class interface function implementation
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

#define MATCH(a,b) ((a) & (b) == (b))

/*static*/ const char *NO_NETWORK_INTERFACE_DEFINED = "No network interface defined\n";


net_interface_class_t net_wifi_get_class(net_if_handle_t *pnetif)
{
  return pnetif->pdrv->if_class;
}


/**
  * @brief  Convert WiFi security enumerated value to string
  * @param  sec is an unsigned integer
  * @retval a constant string, for instance "Open" or "WPA2-AES"
  */
const char_t *net_wifi_security_to_string(uint32_t sec)
{
  const char_t *s;
  if (sec == NET_WIFI_SM_OPEN)
  {
    s = "Open";
  }
  else if (sec == NET_WIFI_SM_WEP_SHARED)
  {
    s = "WEP-shared";
  }
  else if (sec == NET_WIFI_SM_WPA_TKIP_PSK)
  {
    s = "WPA-TKIP";
  }
  else if (sec == NET_WIFI_SM_WPA_MIXED_PSK)
  {
    s = "WPA-Mixed";
  }
  else if (sec == NET_WIFI_SM_WPA2_AES_PSK)
  {
    s = "WPA2-AES";
  }
  else if (sec == NET_WIFI_SM_WPA2_TKIP_PSK)
  {
    s = "WPA2-TKIP";
  }
  else if (sec == NET_WIFI_SM_WPA2_MIXED_PSK)
  {
    s = "WPA2_Mixed";
  }
  else if (sec == NET_WIFI_SM_WPA2_FBT_PSK)
  {
    s = "WPA2-FBT";
  }
  else if (sec == NET_WIFI_SM_WPA3_SAE)
  {
    s = "WPA3";
  }
  else if (sec == NET_WIFI_SM_WPA3_WPA2_PSK)
  {
    s = "WPA3-WPA2";
  }

  else if (sec == NET_WIFI_SM_WPA_TKIP_ENT)
  {
    s = "WPA-TKIP-Ent";
  }
  else if (sec == NET_WIFI_SM_WPA_AES_ENT)
  {
    s = "WPA-AES-Ent";
  }
  else if (sec == NET_WIFI_SM_WPA2_TKIP_ENT)
  {
    s = "WPA2-TKIP-Ent";
  }
  else if (sec == NET_WIFI_SM_WPA2_AES_ENT)
  {
    s = "WPA2-AES-Ent";
  }
  else if (sec == NET_WIFI_SM_WPA2_MIXED_ENT)
  {
    s = "WPA2-Mixed-Ent";
  }
  else if (sec == NET_WIFI_SM_WPA2_FBT_ENT)
  {
    s = "WPA-FBT-Ent";
  }

  else if (sec == NET_WIFI_SM_IBSS_OPEN)
  {
    s = "IBS";
  }
  else if (sec == NET_WIFI_SM_WPS_OPEN)
  {
    s = "WPS";
  }
  else if (sec == NET_WIFI_SM_WPS_SECURE)
  {
    s = "WPS-AES";
  }
  else
  {
    s = "unknown";
  }
  return s;
}

/**
  * @brief  Convert a string to a security enumerated value
  * @param  sec is a pointer to a string
  */
uint32_t net_wifi_string_to_security(char *sec)
{
  uint32_t ret = NET_WIFI_SM_UNKNOWN;
  if (strcmp(sec, "Open") == 0)
  {
    ret = NET_WIFI_SM_OPEN;
  }
  if (strcmp(sec, "WEP-shared") == 0)
  {
    ret = NET_WIFI_SM_WEP_SHARED;
  }
  if (strcmp(sec, "WPA-TKIP") == 0)
  {
    ret = NET_WIFI_SM_WPA_TKIP_PSK;
  }
  if (strcmp(sec, "WPA-Mixed") == 0)
  {
    ret = NET_WIFI_SM_WPA_MIXED_PSK;
  }
  if (strcmp(sec, "WPA2-AES") == 0)
  {
    ret = NET_WIFI_SM_WPA2_AES_PSK;
  }
  if (strcmp(sec, "WPA2-TKIP") == 0)
  {
    ret = NET_WIFI_SM_WPA2_TKIP_PSK;
  }
  if (strcmp(sec, "WPA2-Mixed") == 0)
  {
    ret = NET_WIFI_SM_WPA2_MIXED_PSK;
  }
  if (strcmp(sec, "WPA2-DBT") == 0)
  {
    ret = NET_WIFI_SM_WPA2_FBT_PSK;
  }
  if (strcmp(sec, "WPA3") == 0)
  {
    ret = NET_WIFI_SM_WPA3_SAE;
  }
  if (strcmp(sec, "WPA3-WPA2") == 0)
  {
    ret = NET_WIFI_SM_WPA3_WPA2_PSK;
  }
  if (strcmp(sec, "WPA-TKIP-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA_TKIP_ENT;
  }
  if (strcmp(sec, "WPA-AES-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA_AES_ENT;
  }
  if (strcmp(sec, "WPA2-TKIP-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA2_TKIP_ENT;
  }
  if (strcmp(sec, "WPA2-AES-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA2_AES_ENT;
  }
  if (strcmp(sec, "WPA2-Mixed-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA2_MIXED_ENT;
  }
  if (strcmp(sec, "WPA-FBT-Ent") == 0)
  {
    ret = NET_WIFI_SM_WPA2_FBT_ENT;
  }
  if (strcmp(sec, "IBS") == 0)
  {
    ret = NET_WIFI_SM_IBSS_OPEN;
  }
  if (strcmp(sec, "WPS") == 0)
  {
    ret = NET_WIFI_SM_WPS_OPEN;
  }
  if (strcmp(sec, "WPS-AES") == 0)
  {
    ret = NET_WIFI_SM_WPS_SECURE;
  }

  return ret;
}

/**
  * @brief  Start a WiFi scan operation
  * @param  pnetif_in is a pointer to an allocated network interface structure
  * @param  mode is an enumerated value to specify type of scan mode to be performed
  * @param  ssid is a pointer to a string, when not null, scan searches only this SSID
  * @retval return the number of found access point, max value is "number".
  * This function is a synchronous function.
  */
int32_t net_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid)
{
  int32_t ret = NET_OK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif == NULL)
  {
    NET_DBG_ERROR("%s", NO_NETWORK_INTERFACE_DEFINED);
    ret = NET_ERROR_PARAMETER;
  }
  else if (p_netif->pdrv->if_class != NET_INTERFACE_CLASS_WIFI)
  {
    NET_DBG_ERROR("Incorrect class interface when calling net_wifi_scan function\n");
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    if (p_netif->pdrv->extension.wifi->scan(p_netif, mode, ssid) != NET_OK)
    {
      NET_DBG_ERROR("Error when executing net_wifi_scan function\n");
      ret = NET_ERROR_GENERIC;
    }
  }

  return ret;
}

/**
  * @brief  Get the result of scan operation, once event has been received
  * @param  pnetif is a pointer to an allocated network interface structure
  * @param  results is a pointer to an allocated array of net_wifi_scan_results_t
  * @param  number is unsigned integer, size of the array 'results'
  * @retval return the number of found access point, max value is "number".
  */
int32_t net_wifi_get_scan_results(net_if_handle_t *pnetif, net_wifi_scan_results_t *results, uint8_t number)
{
  int32_t ret;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif == NULL)
  {
    NET_DBG_ERROR("%s", NO_NETWORK_INTERFACE_DEFINED);
    ret = NET_ERROR_PARAMETER;
  }
  else if (p_netif->pdrv->if_class != NET_INTERFACE_CLASS_WIFI)
  {
    NET_DBG_ERROR("Incorrect class interface when calling net_wifi_scan function\n");
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    ret = p_netif->pdrv->extension.wifi->get_scan_results(p_netif, results, number);
  }
  return ret;
}

/**
  * @brief  Set the credential of a WiFi interface, can be AP or STA credentials
  * @param  pnetif_is a pointer to an allocated network interface structure
  * @param  credentials a pointer to a const allocated structure which contain credentials values (SSID, password)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_credentials(net_if_handle_t *pnetif, const net_wifi_credentials_t *credentials)
{
  pnetif->pdrv->extension.wifi->credentials = credentials;
  return NET_OK;
}

/**
  * @brief  set the information element for beacon
  * @param  pnetif is a pointer to an allocated network interface structure
  * @param  ie is a pointer to an information element structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_ie_data(net_if_handle_t *pnetif, net_wifi_ap_ie_t *ie)
{
  pnetif->pdrv->extension.wifi->information_element = ie;
  return NET_OK;
}

/**
  * @brief  Set the access mode for a WIFI interface: AP or STA mode
  * @param  pnetif_is a pointer to an allocated network interface structure
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_access_mode(net_if_handle_t *pnetif, net_wifi_mode_t mode)
{
  pnetif->pdrv->extension.wifi->mode = mode;
  return NET_OK;
}

/**
  * @brief  Set the WiFi channel to be used for an AP
  * @param  pnetif_is a pointer to an allocated network interface structure
  * @param  channel is an unsigned 8 bit integer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_access_channel(net_if_handle_t *pnetif,  uint8_t channel)
{
  pnetif->pdrv->extension.wifi->access_channel = channel;
  return NET_OK;
}

/**
  * @brief  Set the WiFi max count of connections for an AP
  * @param  pnetif is a pointer to an allocated network interface structure
  * @param  count is an unsigned 8 bit integer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_ap_max_connections(net_if_handle_t *pnetif,  uint8_t count)
{
  pnetif->pdrv->extension.wifi->max_connections = count;
  return NET_OK;
}

/**
  * @brief  Set WiFi power save mode
  * @param  pnetif_is a pointer to an allocated network interface structure
  * @param  powersave is a pointer to an allocated structure to define the power save mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_powersave(net_if_handle_t *pnetif, const net_wifi_powersave_t *powersave)
{
  int32_t ret = NET_OK;
  net_if_handle_t *const p_netif = netif_check(pnetif);

  if (p_netif == NULL)
  {
    NET_DBG_ERROR("%s", NO_NETWORK_INTERFACE_DEFINED);
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    if (p_netif->pdrv->if_class != NET_INTERFACE_CLASS_WIFI)
    {
      NET_DBG_ERROR("Incorrect class interface when calling net_wifi_set_powersave function\n");
      ret = NET_ERROR_PARAMETER;
    }
    else
    {
      p_netif->pdrv->extension.wifi->powersave = powersave;
    }
  }
  return ret;
}

/**
  * @brief  Set WiFi extension parameter
  * @param  pnetif_is a pointer to an allocated network interface structure
  * @param  param is an enumerated value to specify which parameter to set
  * @param  data is a pointer to an allocated opaque structure to specify the parameter value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_set_param(net_if_handle_t *pnetif, const net_wifi_param_t param, void *data)
{
  int32_t ret;

  if (pnetif->pdrv->if_class != NET_INTERFACE_CLASS_WIFI)
  {
    NET_DBG_ERROR("Incorrect class interface when calling net_wifi_set_param function\n");
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    ret = pnetif->pdrv->extension.wifi->set_param(param, data);
  }
  return ret;
}

/**
  * @brief  Switch WiFi mode without removing interface to AP or STA
  * @param  pNetif is a pointer to an allocated network interface structure
  * @param  TargetMode is an enumerated value to specify type of target WiFi mode
  * @retval 0 in case of success, an error code otherwise
  */
int32_t net_wifi_switch_mode(net_if_handle_t *pNetif, net_wifi_mode_t TargetMode)
{
  int32_t ret = NET_OK;
  net_if_handle_t *const p_netif = netif_check(pNetif);

  if (p_netif == NULL)
  {
    NET_DBG_ERROR("%s", NO_NETWORK_INTERFACE_DEFINED);
    ret = NET_ERROR_PARAMETER;
  }
  else if (p_netif->pdrv->if_class != NET_INTERFACE_CLASS_WIFI)
  {
    NET_DBG_ERROR("Incorrect class interface when calling net_wifi_switch_mode function\n");
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    if (p_netif->pdrv->extension.wifi->switch_mode(p_netif, TargetMode) != NET_OK)
    {
      NET_DBG_ERROR("Error when executing net_wifi_switch_mode function\n");
      ret = NET_ERROR_GENERIC;
    }
  }

  return ret;
}
