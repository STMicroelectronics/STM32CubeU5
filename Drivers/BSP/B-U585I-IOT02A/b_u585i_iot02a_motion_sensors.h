/**
  ******************************************************************************
  * @file    b_u585i_iot02a_motion_sensors.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the b_u585i_iot02a_motion_sensors.c driver.
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
#ifndef B_U585I_IOT02A_MOTION_SENSORS_H
#define B_U585I_IOT02A_MOTION_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "b_u585i_iot02a_conf.h"
#include "b_u585i_iot02a_errno.h"
#include "../Components/Common/motion_sensor.h"
#include "../Components/iis2mdc/iis2mdc.h"
#include "../Components/ism330dhcx/ism330dhcx.h"
#include <math.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_U585I_IOT02A
  * @{
  */

/** @addtogroup B_U585I_IOT02A_MOTION_SENSORS
  * @{
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Types MOTION SENSORS Exported Types
  * @{
  */
typedef struct
{
  int32_t xval;
  int32_t yval;
  int32_t zval;
} BSP_MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t xval;
  int16_t yval;
  int16_t zval;
} BSP_MOTION_SENSOR_AxesRaw_t;

/* Motion Sensor instance Info */
typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float    GyroMaxOdr;
  float    AccMaxOdr;
  float    MagMaxOdr;
} MOTION_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} MOTION_SENSOR_Ctx_t;
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Constants MOTION SENSORS Exported Constants
  * @{
  */
/* Two motion sensors are mounted on the B_U585I_IOT02A board,
   so the number of motion sensors instances is set to 2.
   Instance 0 corresponds to ISM330DLC (gyroscope and accelerometer capabilities).
   Instance 1 corresponds to IIS2MDC (magnetometer capability). */
#define MOTION_SENSOR_INSTANCES_NBR    2U

#define MOTION_SENSOR_FUNCTIONS_NBR    3U

#define MOTION_GYRO             1U
#define MOTION_ACCELERO         2U
#define MOTION_MAGNETO          4U
/**
  * @}
  */

/** @addtogroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Variables MOTION SENSORS Exported Variables
  * @{
  */
extern MOTION_SENSOR_Ctx_t        Motion_Sensor_Ctx[MOTION_SENSOR_INSTANCES_NBR];
extern void                      *Motion_Sensor_CompObj[MOTION_SENSOR_INSTANCES_NBR];
extern MOTION_SENSOR_CommonDrv_t *Motion_Sensor_Drv[MOTION_SENSOR_INSTANCES_NBR];
extern MOTION_SENSOR_FuncDrv_t   *Motion_Sensor_FuncDrv[MOTION_SENSOR_INSTANCES_NBR][MOTION_SENSOR_FUNCTIONS_NBR];
/**
  * @}
  */

/** @addtogroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Functions MOTION SENSORS Exported Functions
  * @{
  */
int32_t BSP_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t BSP_MOTION_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_MOTION_SENSOR_GetCapabilities(uint32_t Instance, MOTION_SENSOR_Capabilities_t *Capabilities);
int32_t BSP_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t BSP_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t BSP_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, BSP_MOTION_SENSOR_Axes_t *Axes);
int32_t BSP_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, BSP_MOTION_SENSOR_AxesRaw_t *Axes);
int32_t BSP_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity);
int32_t BSP_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t BSP_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t BSP_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t BSP_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);
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

#endif /* B_U585I_IOT02A_MOTION_SENSORS_H */
