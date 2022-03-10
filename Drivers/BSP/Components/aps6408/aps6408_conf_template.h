/**
  ******************************************************************************
  * @file    aps6408_conf_template.h
  * @author  MCD Application Team
  * @brief   APS6408 PSRAM OctoSPI memory configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to aps6408_conf.h
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
#ifndef APS6408_CONF_H
#define APS6408_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32xxxx_hal.h"

/** @addtogroup BSP
  * @{
  */
/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ         5U
#define DUMMY_CLOCK_CYCLES_WRITE        4U
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* APS6408_CONF_H */


