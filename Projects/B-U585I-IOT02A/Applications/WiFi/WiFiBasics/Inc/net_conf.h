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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NET_CONF_H
#define NET_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported macros -----------------------------------------------------------*/

#define NET_DBG_PRINT(...)
#define NET_LOG(...)
/* #define NET_ALLOC_DEBUG          */
/* #define NET_PERF_TASK            */
/* #define NET_ALLOC_DEBUG_TREE     */

#include "net_conf_template.h"


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NET_CONF_H */
