/**
  ******************************************************************************
  * @file    net_cellular.h
  * @author  MCD Application Team
  * @brief   Header for the network Cellular class.
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
#ifndef NET_CELLULAR_H
#define NET_CELLULAR_H

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NET_CELLULAR_CREDENTIAL_V2

/* SIM socket type */
#define NET_SIM_SLOT_MODEM_SOCKET        0   /* Modem Socket SIM Slot   */
#define NET_SIM_SLOT_MODEM_EMBEDDED_SIM  1   /* Modem Embedded SIM Slot */
#define NET_CELLULAR_MAX_SUPPORTED_SLOT  2   /* Number max of supported SIM slot */
#define NET_CELLULAR_DEFAULT_SIM_SLOT   NET_SIM_SLOT_MODEM_SOCKET   /* Default SIM Slot   */

typedef char_t net_sim_slot_type_t ;

/* SIM Slot parameters */
typedef struct net_cellular_sim_slot_s
{
  net_sim_slot_type_t   sim_slot_type;   /* sim slot type                               */
  char_t                *apn;            /* APN (string)                                */
  char_t                cid;             /* CID (1-9)                                   */
  char_t                *username;       /* username: empty string => no username       */
  char_t                *password;       /* password (used only is username is defined) */
} net_cellular_sim_slot_t;

/* Credential configuration */
typedef struct net_cellular_credentials_s
{
  uint8_t sim_socket_nb;                                                 /* number of sim slot used */
  net_cellular_sim_slot_t  sim_slot[NET_CELLULAR_MAX_SUPPORTED_SLOT];    /* sim slot parameters     */
} net_cellular_credentials_t;

#else /*NET_CELLULAR_CREDENTIAL_V2 */

/* Credential configuration */
typedef struct net_cellular_credentials_s
{
  const char_t *apn;
  const char_t *username;
  const char_t *password;
  bool_t use_internal_sim;
} net_cellular_credentials_t;

#endif /* NET_CELLULAR_CREDENTIAL_V2 */


/* Network radio results */
typedef struct net_cellular_radio_results_s
{
  int8_t signal_level_db;
} net_cellular_radio_results_t;

/* network extension for Cellular class interface */
int32_t net_cellular_set_credentials(net_if_handle_t *pnetif_in, const net_cellular_credentials_t *credentials);
int32_t net_cellular_get_radio_results(net_if_handle_t *pnetif_in, net_cellular_radio_results_t *results);

/* Declaration of cellular network interface constructor */
int32_t cellular_net_driver(net_if_handle_t *pnetif);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_CELLULAR_H */
