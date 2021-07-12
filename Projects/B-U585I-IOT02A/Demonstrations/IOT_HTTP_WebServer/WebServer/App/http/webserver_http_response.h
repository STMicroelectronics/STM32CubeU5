/**
  **********************************************************************************************************************
  * @file    webserver_http_response.h
  * @author  MCD Application Team
  * @brief   header file for webserver_http_response.c
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"
#include "webserver_http_cmd.h"
#include "res.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
WebServer_StatusTypeDef webserver_http_start(void);

#endif /* WEBSERVER_HTTP_RESPONSE_H */
