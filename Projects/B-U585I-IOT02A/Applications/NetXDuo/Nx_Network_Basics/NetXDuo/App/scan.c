/**
  ******************************************************************************
  * @file    scan.c
  * @author  MCD Application Team
  * @brief   Scan command.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "msg.h"
#include "app_netxduo.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "mx_wifi_conf.h"
#include "mx_wifi.h"
#include "io_pattern/mx_wifi_io.h"

#define NET_STATE_TRANSITION_TIMEOUT 60000

#define MAX_SCAN_AP                  MX_WIFI_MAX_DETECTED_AP


static int32_t mx_wifi_scan(mc_wifi_scan_mode_t Mode, char *Ssid);
static int32_t mx_wifi_get_scan_result(mwifi_ap_info_t *pScanBssArray,
                                       uint8_t ScanBssCount);
static const char *wifi_security_to_string(uint8_t Security);

/**
  * @brief                   Mxchip WiFi scan function
  * @param  Mode             scan mode
  * @param  Ssid             SSID to scan in active mode
  * @return LONG          0 if success, otherwise failed
  */
static int32_t mx_wifi_scan(mc_wifi_scan_mode_t Mode, char *Ssid)
{
  int32_t ret;
  int32_t len = 0;

  if (Ssid != NULL)
  {
    len = strlen(Ssid);
  }

  ret = MX_WIFI_Scan(wifi_obj_get(), Mode, Ssid, len);

  return ret;
}

/**
  * @brief                       Mxchip WiFi get scan result
  * @param  pScanBssArray        BSS array buffer
  * @param  ScanBssCount         BSS count
  * @return int32_t              number of the BSS got
  */
static int32_t mx_wifi_get_scan_result(mwifi_ap_info_t *pScanBssArray,
                                       uint8_t ScanBssCount)
{
  int32_t ret;
  uint8_t number = ScanBssCount;

  if ((NULL == pScanBssArray) || (0 == number))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }

  /* get real mx_wifi scan results data */
  ret = MX_WIFI_Get_scan_result(wifi_obj_get(), (uint8_t *) pScanBssArray, number);

  return ret;
}

/**
  * @brief  Convert WiFi security enum value to string
  * @param  Security is an unsigned integer
  * @retval a constant string, for instance "Open" or "WPA2-AES"
  */
static const char *wifi_security_to_string(uint8_t Security)
{
  const char *s;
  if (Security == MX_WIFI_SEC_NONE)
  {
    s = "Open";
  }
  else if (Security == MX_WIFI_SEC_WEP)
  {
    s = "WEP-shared";
  }
  else if (Security == MX_WIFI_SEC_WPA_TKIP)
  {
    s = "WPA-TKIP";
  }
  else if (Security == MX_WIFI_SEC_WPA_AES)
  {
    s = "WPA-AES";
  }
  else if (Security == MX_WIFI_SEC_WPA2_TKIP)
  {
    s = "WPA2-TKIP";
  }
  else if (Security == MX_WIFI_SEC_WPA2_AES)
  {
    s = "WPA2-AES";
  }
  else if (Security == MX_WIFI_SEC_WPA2_MIXED)
  {
    s = "WPA2_Mixed";
  }
  else
  {
    s = "Unknown";
  }
  return s;
}


int32_t scan_cmd(int32_t argc, char *argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  static mwifi_ap_info_t APs[MAX_SCAN_AP] = {0};

  int32_t ret = mx_wifi_scan(MC_SCAN_PASSIVE, NULL);

  if (ret == MX_WIFI_STATUS_OK)
  {
    memset(APs, 0, sizeof(APs));
    ret = mx_wifi_get_scan_result(APs, MAX_SCAN_AP);
  }

  if (ret > 0)
  {
    MSG_INFO("######### Scan %" PRIi32 " BSS ##########\n", ret);
    for (int32_t i = 0; i < ret; i++)
    {
#define AP_SSID_LENGTH sizeof(((mwifi_ap_info_t *)0)->ssid)

      /* Ensure that the string ends with '\0'. */
      APs[i].ssid[AP_SSID_LENGTH - 1] = '\0';

      MSG_INFO("\t%2" PRIi32 "\t%40s ch %2" PRIi32 " rss %" PRIi32 " Security %10s country %4s"
               " bssid %02x.%02x.%02x.%02x.%02x.%02x\n",
               i, APs[i].ssid, APs[i].channel, APs[i].rssi,
               wifi_security_to_string(APs[i].security), ".CN", /* NOT SUPPORT for MX_WIFI */
               APs[i].bssid[0], APs[i].bssid[1], APs[i].bssid[2], APs[i].bssid[3], APs[i].bssid[4], APs[i].bssid[5]);
    }
    MSG_INFO("######### End of Scan ##########\n");
  }
  else
  {
    MSG_ERROR("Scan operation failed (%" PRIi32 ")!", ret);
  }
  return 0;
}
