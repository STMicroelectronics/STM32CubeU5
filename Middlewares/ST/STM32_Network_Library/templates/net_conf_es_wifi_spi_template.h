/**
  ******************************************************************************
  * @file    net_conf.h
  * @author  MCD Application Team
  * @brief   This file provides the configuration for net
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

#ifndef NET_CONF_H
#define NET_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

int32_t wifi_probe(void **ll_drv_obj);
void    SPI_WIFI_ISR(void);

#ifndef GENERATOR_WAKAAMACLIENT_CLOUD
#define NET_MBEDTLS_HOST_SUPPORT
/* to use Inventek Wifi native TLS */
/* #define NET_MBEDTLS_WIFI_MODULE_SUPPORT */
#endif /* GENERATOR_WAKAAMACLIENT_CLOUD */
/*#define NET_ALLOC_DEBUG     */
/*#define NET_ALLOC_DEBUG_TREE  */

#include "net_conf_template.h"



#ifdef __cplusplus
}
#endif

#endif /* NET_CONF_H */
