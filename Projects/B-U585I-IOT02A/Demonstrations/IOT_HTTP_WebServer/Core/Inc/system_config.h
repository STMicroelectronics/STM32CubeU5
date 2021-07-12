/**
  **********************************************************************************************************************
  * @file    system_config.h
  * @author  MCD Application Team
  * @brief   Header for system_config.c module
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
#ifndef WEBSERVER_SYSTEM_CONFIG_H
#define WEBSERVER_SYSTEM_CONFIG_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
WebServer_StatusTypeDef system_init(void);
WebServer_StatusTypeDef system_clock_config(void);
void instruction_cache_enable(void);

/* Private defines ---------------------------------------------------------------------------------------------------*/

#endif /* WEBSERVER_SYSTEM_CONFIG_H */
