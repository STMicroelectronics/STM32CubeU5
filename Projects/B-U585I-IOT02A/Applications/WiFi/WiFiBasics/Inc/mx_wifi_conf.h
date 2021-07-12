/**
  ******************************************************************************
  * @file    mx_wifi_conf.h
  * @author  MCD Application Team
  * @brief   Header for mx_wifi_conf module
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
#ifndef MX_WIFI_CONF_H
#define MX_WIFI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

/* Exported macros -----------------------------------------------------------*/
#define MX_WIFI_USE_SPI                                                     (1)
#define MX_WIFI_USE_CMSIS_OS                                                (0)
#define MX_WIFI_NETWORK_BYPASS_MODE                                         (0)
#define DMA_ON_USE                                                          (1)


/* Includes ------------------------------------------------------------------*/
#include "mx_wifi_conf_template.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WIFI_CONF_H */
