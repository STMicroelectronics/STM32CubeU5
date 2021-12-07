/**
  ******************************************************************************
  * @file    mx_wifi_io.c
  * @author  MCD Application Team
  * @brief   This file implements the IO operations to deal with the mx_wifi
  *          module. It mainly Inits and Deinits the UART interface. Send and
  *          receive data over it.
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
#include "mx_wifi_conf.h"
#if MX_WIFI_USE_SPI == 1
#include "net_conf_mxchip_spi.c"
#else
#include "net_conf_mxchip_uart.c"
#endif /* MX_WIFI_USE_SPI */
