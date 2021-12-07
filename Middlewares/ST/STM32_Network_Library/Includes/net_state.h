/**
  ******************************************************************************
  * @file    net_state.h
  * @author  MCD Application Team
  * @brief   Header for the network state management functions
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
#ifndef NET_STATE_H
#define NET_STATE_H

/* Includes ------------------------------------------------------------------*/
#include "net_address.h"
#include "net_connect.h"

int32_t net_state_manage_event(net_if_handle_t *pnetif, net_state_event_t state_to);
void net_state_transition_done(net_if_handle_t *pnetif, net_state_t state_to);

#endif /* NET_STATE_H */
