/**
  ******************************************************************************
  * @file    net_buffers.h
  * @author  MCD Application Team
  * @brief   Defines buffer allocation functions
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
#ifndef NET_BUFFERS_H
#define NET_BUFFERS_H

/* Includes ------------------------------------------------------------------*/
#include "lwip/pbuf.h"

#define NET_BUF_ALLOC(n)        (net_buf_t*)pbuf_alloc(PBUF_RAW, (n), PBUF_POOL)
#define NET_BUF_REF_ALLOC()     (net_buf_t*)pbuf_alloc(PBUF_RAW, 0, PBUF_REF);
#define NET_BUF_FREE(p)         pbuf_free((struct pbuf*)(p));
#define NET_BUF_REF(p)          pbuf_ref((struct pbuf*)(p));

#endif /* NET_BUFFERS_H */
