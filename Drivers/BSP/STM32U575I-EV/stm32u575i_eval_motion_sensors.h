/**
  ******************************************************************************
  * @file    stm32u575i_eval_motion_sensors.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u575i_eval_motion_sensors.c driver.
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
#ifndef STM32U575I_EVAL_MOTION_SENSORS_H
#define STM32U575I_EVAL_MOTION_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"
#include "../Components/Common/motion_sensor.h"
#include "../Components/lsm6dso/lsm6dso.h"
#include <math.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_MOTION_SENSORS
  * @{
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Types MOTION SENSORS Exported Types
  * @{
  */
/* Motion sensor axes */
typedef struct
{
  int32_t xval;
  int32_t yval;
  int32_t zval;
} BSP_MOTION_SENSOR_Axes_t;

/* Motion sensor raw axes */
typedef struct
{
  int16_t xval;
  int16_t yval;
  int16_t zval;
} BSP_MOTION_SENSOR_AxesRaw_t;

/* Motion sensor capabilities */
typedef struct
{
  uint8_t  Acc;        /* 1 means available, 0 means not available */
  uint8_t  Gyro;       /* 1 means available, 0 means not available */
  uint8_t  Magneto;    /* 1 means available, 0 means not available */
  uint8_t  LowPower;   /* 1 means low power supported, 0 means low power not supported */
  uint32_t GyroMaxFS;  /* Maximum full scale of gyroscope */
  uint32_t AccMaxFS;   /* Maximum full scale of accelerometer */
  uint32_t MagMaxFS;   /* Maximum full scale of magnetometer */
  float_t  GyroMaxOdr; /* Maximum output data rate of gyroscope */
  float_t  AccMaxOdr;  /* Maximum output data rate of accelerometer */
  float_t  MagMaxOdr;  /* Maximum output data rate of magnetometer */
} MOTION_SENSOR_Capabilities_t;

/* Motion sensor context */
typedef struct
{
  uint32_t Functions;
} MOTION_SENSOR_Ctx_t;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Constants MOTION SENSORS Exported Constants
  * @{
  */
/* One motion sensor is mounted on the STM32U575I_EVAL board,
   so the number of motion sensors instances is set to 1.
   Instance 0 corresponds to LSM6DS0 (gyroscope and accelerometer capabilities). */
#define MOTION_SENSOR_INSTANCES_NBR 1U

#define MOTION_SENSOR_FUNCTIONS_NBR 3U

/* Motion sensor functions */
#define MOTION_GYRO      1U
#define MOTION_ACCELERO  2U
#define MOTION_MAGNETO   4U
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Variables MOTION SENSORS Exported Variables
  * @{
  */
extern MOTION_SENSOR_Ctx_t        Motion_Sensor_Ctx[MOTION_SENSOR_INSTANCES_NBR];
extern void                      *Motion_Sensor_CompObj[MOTION_SENSOR_INSTANCES_NBR];
extern MOTION_SENSOR_CommonDrv_t *Motion_Sensor_Drv[MOTION_SENSOR_INSTANCES_NBR];
extern MOTION_SENSOR_FuncDrv_t   *Motion_Sensor_FuncDrv[MOTION_SENSOR_INSTANCES_NBR][MOTION_SENSOR_FUNCTIONS_NBR];
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Functions MOTION SENSORS Exported Functions
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

#endif /* STM32U575I_EVAL_MOTION_SENSORS_H */
