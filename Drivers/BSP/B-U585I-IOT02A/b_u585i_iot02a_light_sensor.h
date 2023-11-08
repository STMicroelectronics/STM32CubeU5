/**
  ******************************************************************************
  * @file    b_u585i_iot02a_light_sensor.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the b_u585i_iot02a_light_sensor driver.
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
#ifndef B_U585I_IOT02A_LIGHT_SENSORS_H
#define B_U585I_IOT02A_LIGHT_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "b_u585i_iot02a_conf.h"
#include "b_u585i_iot02a_errno.h"
#include "../Components/Common/light_sensor.h"
#include "../Components/veml3235/veml3235.h"
#include <math.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_U585I_IOT02A
  * @{
  */

/** @addtogroup B_U585I_IOT02A_LIGHT_SENSORS
  * @{
  */

/** @defgroup B_U585I_IOT02A_LIGHT_SENSORS_Exported_Types LIGHT SENSORS Exported Types
  * @{
  */
typedef struct
{
  uint8_t NumberOfChannels;  /*!< Max: LIGHT_SENSOR_MAX_CHANNELS */
  uint8_t FlickerDetection;  /*!< Not available: 0, Available: 1 */
  uint8_t Autogain;          /*!< Not available: 0, Available: 1 */
  uint8_t Flicker;           /*!< Not available: 0, Available: 1 */
} LIGHT_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Gain;
  uint32_t ExposureTime;
  uint32_t Persistence;
} LIGHT_SENSOR_Ctx_t;
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_LIGHT_SENSORS_Exported_Constants LIGHT SENSORS Exported Constants
  * @{
  */

#define LIGHT_SENSOR_INSTANCES_NBR      1U

/* light sensor channel definitions */
#define LIGHT_SENSOR_MAX_CHANNELS                    VEML3235_MAX_CHANNELS
#define LIGHT_SENSOR_ALS_CHANNEL                     VEML3235_ALS_CHANNEL
#define LIGHT_SENSOR_WHITE_CHANNEL                   VEML3235_WHITE_CHANNEL

/* Light sensor Mode */
#define LIGHT_SENSOR_MODE_CONTINUOUS                 VEML3235_MODE_CONTINUOUS

/* light sensor exposure time definitions (ms)*/
#define LIGHT_SENSOR_EXPOSURE_TIME_50                VEML3235_CONF_IT50
#define LIGHT_SENSOR_EXPOSURE_TIME_100               VEML3235_CONF_IT100
#define LIGHT_SENSOR_EXPOSURE_TIME_200               VEML3235_CONF_IT200
#define LIGHT_SENSOR_EXPOSURE_TIME_400               VEML3235_CONF_IT400
#define LIGHT_SENSOR_EXPOSURE_TIME_800               VEML3235_CONF_IT800

/*light sensor gain definitions */
#define LIGHT_SENSOR_GAIN_1                          VEML3235_CONF_GAIN_1
#define LIGHT_SENSOR_GAIN_2                          VEML3235_CONF_GAIN_2
#define LIGHT_SENSOR_GAIN_4                          VEML3235_CONF_GAIN_4

/* Light sensor InterMeasurement Time (ms) (Referesh Time) */

/*#define LIGHT_SENSOR_INTER_MEAS_TIME_600             VEML3235_REFRESH_TIME_600*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_700             VEML3235_REFRESH_TIME_700*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_900             VEML3235_REFRESH_TIME_900*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_1100            VEML3235_REFRESH_TIME_1100*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_1200            VEML3235_REFRESH_TIME_1200*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_1300            VEML3235_REFRESH_TIME_1300*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_1400            VEML3235_REFRESH_TIME_1400*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_1800            VEML3235_REFRESH_TIME_1800*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_2100            VEML3235_REFRESH_TIME_2100*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_2200            VEML3235_REFRESH_TIME_2200*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_2400            VEML3235_REFRESH_TIME_2400*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_2800            VEML3235_REFRESH_TIME_2800*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_4100            VEML3235_REFRESH_TIME_4100*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_4200            VEML3235_REFRESH_TIME_4200*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_4400            VEML3235_REFRESH_TIME_4400*/
/*#define LIGHT_SENSOR_INTER_MEAS_TIME_4800            VEML3235_REFRESH_TIME_4800*/
/**
  * @}
  */

/** @addtogroup B_U585I_IOT02A_LIGHT_SENSORS_Exported_Variables LIGHT SENSORS Exported Variables
  * @{
  */
extern void                *VEML3235_LIGHT_SENSOR_CompObj[LIGHT_SENSOR_INSTANCES_NBR] ;
extern LIGHT_SENSOR_Ctx_t LIGHTSENSOR_Ctx[LIGHT_SENSOR_INSTANCES_NBR];
/**
  * @}
  */

/** @addtogroup B_U585I_IOT02A_LIGHT_SENSORS_Exported_Functions LIGHT SENSORS Exported Functions
  * @{
  */

int32_t BSP_LIGHT_SENSOR_Init(uint32_t Instance);
int32_t BSP_LIGHT_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_LIGHT_SENSOR_ReadID(uint32_t Instance, uint32_t *pId);
int32_t BSP_LIGHT_SENSOR_GetCapabilities(uint32_t Instance, LIGHT_SENSOR_Capabilities_t *pCapabilities);
int32_t BSP_LIGHT_SENSOR_SetExposureTime(uint32_t Instance, uint32_t ExposureTime);
int32_t BSP_LIGHT_SENSOR_GetExposureTime(uint32_t Instance, uint32_t *pExposureTime);
int32_t BSP_LIGHT_SENSOR_SetGain(uint32_t Instance, uint8_t Channel, uint32_t Gain);
int32_t BSP_LIGHT_SENSOR_GetGain(uint32_t Instance, uint8_t Channel, uint32_t *Gain);
int32_t BSP_LIGHT_SENSOR_SetInterMeasurementTime(uint32_t Instance, uint32_t InterMeasurementTime);
int32_t BSP_LIGHT_SENSOR_GetInterMeasurementTime(uint32_t Instance, uint32_t *pInterMeasurementTime);
int32_t BSP_LIGHT_SENSOR_Start(uint32_t Instance, uint8_t Mode);
int32_t BSP_LIGHT_SENSOR_Stop(uint32_t Instance);
int32_t BSP_LIGHT_SENSOR_StartFlicker(uint32_t Instance, uint8_t Channel, uint8_t OutputMode);
int32_t BSP_LIGHT_SENSOR_StopFlicker(uint32_t Instance);
int32_t BSP_LIGHT_SENSOR_GetValues(uint32_t Instance, uint32_t *pResult);
int32_t BSP_LIGHT_SENSOR_SetControlMode(uint32_t Instance, uint32_t ControlMode, uint32_t Value);

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

#endif /* B_U585I_IOT02A_LIGHT_SENSORS_H */

