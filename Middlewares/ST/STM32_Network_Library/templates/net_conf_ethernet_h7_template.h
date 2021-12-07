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

#include <stdint.h>
void ethernetif_low_init(void (*event_callback)(void), void (*buffer_output_callback)(uint8_t *));
void  ethernetif_low_deinit(void);

int16_t ethernetif_low_inputsize(void);
int16_t ethernetif_low_input(uint8_t *payload, uint16_t len);
int16_t ethernetif_low_output(uint8_t *payload, uint16_t len);

void ethernetif_low_get_mac_addr(uint8_t *MACAddr_in);
uint8_t ethernetif_low_get_link_status(void);

#define NET_USE_RTOS
#define ETHERNET_MAC_GENERATION_FROM_SHA256
#define NET_DHCP_SERVER_HOST_SUPPORT

#ifdef GENERATOR_WAKAAMACLIENT_CLOUD
#define USE_TINY_DTLS
#else
#define NET_MBEDTLS_HOST_SUPPORT
#endif /* GENERATOR_WAKAAMACLIENT_CLOUD */

#include "net_conf_template.h"


#ifdef __cplusplus
}
#endif

#endif /* NET_CONF_H */
