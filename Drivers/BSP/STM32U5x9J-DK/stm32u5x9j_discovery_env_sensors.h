/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_env_sensors.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u5x9j_discovery_env_sensors driver.
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
#ifndef STM32U5x9J_DISCOVERY_ENV_SENSORS_H
#define STM32U5x9J_DISCOVERY_ENV_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_conf.h"
#include "stm32u5x9j_discovery_errno.h"
#include "../Components/Common/env_sensor.h"
#include "../Components/stts22h/stts22h.h"
#include <math.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY_ENV_SENSORS
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Types ENVIRONMENTAL SENSORS Exported Types
  * @{
  */
typedef struct
{
  uint8_t Temperature;
  uint8_t LowPower;
  float   TempMaxOdr;
} ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} ENV_SENSOR_Ctx_t;
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Constants ENVIRONMENTAL SENSORS Exported Constants
  * @{
  */
/* One environmental sensor is mounted on the STM32U5x9J_DISCOVERY board,
   so the number of environmental sensors instances is set to 1.
   Instance 0 corresponds to STTS22H (temperature and humidity capabilities). */
#define ENV_SENSOR_INSTANCES_NBR    1U

#define ENV_SENSOR_FUNCTIONS_NBR    1U

#define ENV_TEMPERATURE             1U
/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Variables ENVIRONMENTAL SENSORS Exported Variables
  * @{
  */
/* Environmental sensor context */
extern ENV_SENSOR_Ctx_t Env_Sensor_Ctx[ENV_SENSOR_INSTANCES_NBR];

/* Environmental sensor component object */
extern void *Env_Sensor_CompObj[ENV_SENSOR_INSTANCES_NBR];

/* Environmental sensor common driver */
extern ENV_SENSOR_CommonDrv_t *Env_Sensor_Drv[ENV_SENSOR_INSTANCES_NBR];

/* Environmental sensor function driver */
extern ENV_SENSOR_FuncDrv_t *Env_Sensor_FuncDrv[ENV_SENSOR_INSTANCES_NBR][ENV_SENSOR_FUNCTIONS_NBR];
/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Functions ENVIRONMENTAL SENSORS Exported Functions
  * @{
  */
int32_t BSP_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t BSP_ENV_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_ENV_SENSOR_GetCapabilities(uint32_t Instance, ENV_SENSOR_Capabilities_t *Capabilities);
int32_t BSP_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t BSP_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t BSP_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t BSP_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t BSP_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float_t *Value);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5x9J_DISCOVERY_ENV_SENSORS_H */
