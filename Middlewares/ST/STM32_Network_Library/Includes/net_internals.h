/**
  ******************************************************************************
  * @file    net_internals.h
  * @author  MCD Application Team
  * @brief   Header for the network interface with mbedTLS (if used)
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
#ifndef NET_INTERNALS_H
#define NET_INTERNALS_H

/* Includes ------------------------------------------------------------------*/
#include "net_core.h"

#ifdef  NET_MBEDTLS_HOST_SUPPORT
#include "net_mbedtls.h"
#endif /* NET_MBEDTLS_HOST_SUPPORT */

#endif /* NET_INTERNALS_H */
