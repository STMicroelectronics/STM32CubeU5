/**
  ******************************************************************************
  * @file    net_services.h
  * @author  MCD Application Team
  * @brief   Provides the network services APIs.
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
#ifndef NET_SERVICES_H
#define NET_SERVICES_H
/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "net_state.h"
#include "net_wifi.h"
#include "net_class_extension.h"

/* DHCP Server Service */
int32_t service_sdhcp_create(struct netif *netif);
int32_t service_sdhcp_delete(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_SERVICES_H */
