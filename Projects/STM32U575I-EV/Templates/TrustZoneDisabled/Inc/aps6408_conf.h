/**
  ******************************************************************************
  * @file    aps6408_conf.h
  * @author  MCD Application Team
  * @brief   APS6408 PSRAM OctoSPI memory configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32u5xx_hal.h"

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
