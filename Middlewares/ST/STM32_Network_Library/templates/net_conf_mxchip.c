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
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "mx_wifi_conf.h"
#if MX_WIFI_USE_SPI == 1
#include "net_conf_mxchip_spi.c"
#else
#include "net_conf_mxchip_uart.c"
#endif /* MX_WIFI_USE_SPI */
