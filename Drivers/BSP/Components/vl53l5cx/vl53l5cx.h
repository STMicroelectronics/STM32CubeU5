/**
  ******************************************************************************
  * @file    vl53l5cx.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the functions prototypes for the vl53l5cx.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VL53L5CX_H
#define VL53L5CX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include "vl53l5cx_api.h"
#include "vl53l5cx_plugin_xtalk.h"
#include "vl53l5cx_plugin_detection_thresholds.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L5CX
  * @{
  */

/** @defgroup VL53L5CX_Exported_Constants Exported Constants
  * @{
  */
#define VL53L5CX_ID       (0xF002U)
#define VL53L5CX_ID_REG   (0x010FU)

#define VL53L5CX_DEVICE_ADDRESS      (VL53L5CX_DEFAULT_I2C_ADDRESS)
#define VL53L5CX_TARGET_PER_ZONE     (VL53L5CX_NB_TARGET_PER_ZONE)
#define VL53L5CX_MAX_NB_ZONES        (VL53L5CX_RESOLUTION_8X8)

/* vl53l5cx ranging profiles */
#define VL53L5CX_PROFILE_4x4_CONTINUOUS        (1U)
#define VL53L5CX_PROFILE_4x4_AUTONOMOUS        (2U)
#define VL53L5CX_PROFILE_8x8_CONTINUOUS        (3U)
#define VL53L5CX_PROFILE_8x8_AUTONOMOUS        (4U)

/* vl53l5cx threshold/interrupt criteria */
#define VL53L5CX_IT_DEFAULT           (0xFFU) /*!< IT if new measurement is ready (thresholds are not considered) */
#define VL53L5CX_IT_IN_WINDOW         (VL53L5CX_IN_WINDOW) /*!< IT if distance > HighThreshold */
#define VL53L5CX_IT_OUT_OF_WINDOW     (VL53L5CX_OUT_OF_WINDOW) /*!< IT if distance < LowThreshold OR distance > HighThreshold */
#define VL53L5CX_IT_BELOW_LOW         (VL53L5CX_LESS_THAN_EQUAL_MIN_CHECKER) /*!< IT if distance <= LowThreshold */
#define VL53L5CX_IT_ABOVE_HIGH        (VL53L5CX_GREATER_THAN_MAX_CHECKER) /*!< IT if distance > HighThreshold */
#define VL53L5CX_IT_EQUAL_LOW         (VL53L5CX_EQUAL_MIN_CHECKER) /*!< IT if distance == LowThreshold */
#define VL53L5CX_IT_NOT_EQUAL_LOW     (VL53L5CX_NOT_EQUAL_MIN_CHECKER) /*!< IT if distance != LowThreshold */

/* vl53l5cx ranging modes */
#define VL53L5CX_MODE_BLOCKING_CONTINUOUS   (1U)
#define VL53L5CX_MODE_BLOCKING_ONESHOT      (2U)
#define VL53L5CX_MODE_ASYNC_CONTINUOUS      (3U)
#define VL53L5CX_MODE_ASYNC_ONESHOT         (4U)

/* vl53l5cx error codes */
#define VL53L5CX_OK                  (0)
#define VL53L5CX_ERROR               (-1)
#define VL53L5CX_INVALID_PARAM       (-2)
#define VL53L5CX_TIMEOUT             (-3)
#define VL53L5CX_NOT_IMPLEMENTED     (-4)
/**
  * @}
  */

/** @defgroup VL53L5CX_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*VL53L5CX_Init_Func)(void);
typedef int32_t (*VL53L5CX_DeInit_Func)(void);
typedef int32_t (*VL53L5CX_GetTick_Func)(void);
typedef int32_t (*VL53L5CX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VL53L5CX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  VL53L5CX_Init_Func Init;
  VL53L5CX_DeInit_Func DeInit;
  uint16_t Address;
  VL53L5CX_WriteReg_Func WriteReg;
  VL53L5CX_ReadReg_Func ReadReg;
  VL53L5CX_GetTick_Func GetTick;
} VL53L5CX_IO_t;

typedef struct
{
  VL53L5CX_IO_t IO;
  VL53L5CX_Configuration Dev;
  uint8_t IsInitialized;      /*!< NotInitialized: 0, Initialized: 1 */
  uint8_t IsRanging;          /*!< Not Started: 0, Started: 1 */
  uint8_t IsBlocking;         /*!< Interrupt: 0, Polling: 1 */
  uint8_t IsContinuous;       /*!< One shot: 0, Continuous: 1 */
  uint8_t IsAmbientEnabled;   /*!< Enabled: 0, Disabled: 1 */
  uint8_t IsSignalEnabled;    /*!< Enabled: 0, Disabled: 1 */
  uint8_t RangingProfile;
} VL53L5CX_Object_t;

typedef struct
{
  uint8_t NumberOfTargets;
  uint32_t Distance[VL53L5CX_NB_TARGET_PER_ZONE]; /*!< millimeters */
  uint32_t Status[VL53L5CX_NB_TARGET_PER_ZONE];   /*!< OK: 0, NOK: !0 */
  float_t Ambient[VL53L5CX_NB_TARGET_PER_ZONE];   /*!< kcps / spad */
  float_t Signal[VL53L5CX_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
} VL53L5CX_ZoneResult_t;

typedef struct
{
  uint32_t NumberOfZones;
  VL53L5CX_ZoneResult_t ZoneResult[VL53L5CX_MAX_NB_ZONES];
} VL53L5CX_Result_t;

typedef struct
{
  uint32_t Criteria;       /*!< interrupt generation criteria */
  uint32_t LowThreshold;   /*!< expressed in millimeters */
  uint32_t HighThreshold;  /*!< expressed in millimeters */
} VL53L5CX_ITConfig_t;

typedef struct
{
  uint8_t TopLeftX;  /*!< Top Left x coordinate */
  uint8_t TopLeftY;  /*!< Top Left y coordinate */
  uint8_t BotRightX; /*!< Bot Right x coordinate */
  uint8_t BotRightY; /*!< Bot Right y coordinate */
} VL53L5CX_ROIConfig_t;

typedef struct
{
  uint8_t RangingProfile;
  uint32_t TimingBudget;   /*!< Expressed in milliseconds */
  uint32_t Frequency;      /*!< Expressed in Hz */
  uint32_t EnableAmbient;  /*<! Enable: 1, Disable: 0 */
  uint32_t EnableSignal;   /*<! Enable: 1, Disable: 0 */
} VL53L5CX_ProfileConfig_t;

typedef struct
{
  uint32_t NumberOfZones;
  uint32_t MaxNumberOfTargetsPerZone;
  uint32_t CustomROI;           /*<! Not available: 0, Available: 1 */
  uint32_t ThresholdDetection;  /*<! Not available: 0, Available: 1 */
} VL53L5CX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(VL53L5CX_Object_t *);
  int32_t (*DeInit)(VL53L5CX_Object_t *);
  int32_t (*ReadID)(VL53L5CX_Object_t *, uint32_t *);
  int32_t (*GetCapabilities)(VL53L5CX_Object_t *, VL53L5CX_Capabilities_t *);
  int32_t (*ConfigProfile)(VL53L5CX_Object_t *, VL53L5CX_ProfileConfig_t *);
  int32_t (*ConfigROI)(VL53L5CX_Object_t *, VL53L5CX_ROIConfig_t *);
  int32_t (*ConfigIT)(VL53L5CX_Object_t *, VL53L5CX_ITConfig_t *);
  int32_t (*GetDistance)(VL53L5CX_Object_t *, VL53L5CX_Result_t *);
  int32_t (*Start)(VL53L5CX_Object_t *, uint32_t);
  int32_t (*Stop)(VL53L5CX_Object_t *);
  int32_t (*SetAddress)(VL53L5CX_Object_t *, uint32_t);
  int32_t (*GetAddress)(VL53L5CX_Object_t *, uint32_t *);
  int32_t (*SetPowerMode)(VL53L5CX_Object_t *, uint32_t);
  int32_t (*GetPowerMode)(VL53L5CX_Object_t *, uint32_t *);
} VL53L5CX_RANGING_SENSOR_Drv_t;
/**
  * @}
  */

/** @defgroup VL53L5CX_Exported_Functions VL53L5CX Exported Functions
  * @{
  */
/* RANGING_SENSOR methods */
int32_t VL53L5CX_RegisterBusIO(VL53L5CX_Object_t *pObj, VL53L5CX_IO_t *pIO);
int32_t VL53L5CX_Init(VL53L5CX_Object_t *pObj);
int32_t VL53L5CX_DeInit(VL53L5CX_Object_t *pObj);
int32_t VL53L5CX_ReadID(VL53L5CX_Object_t *pObj, uint32_t *pId);
int32_t VL53L5CX_GetCapabilities(VL53L5CX_Object_t *pObj, VL53L5CX_Capabilities_t *pCap);
int32_t VL53L5CX_ConfigProfile(VL53L5CX_Object_t *pObj, VL53L5CX_ProfileConfig_t *pConfig);
int32_t VL53L5CX_ConfigROI(VL53L5CX_Object_t *pObj, VL53L5CX_ROIConfig_t *pROIConfig);
int32_t VL53L5CX_ConfigIT(VL53L5CX_Object_t *pObj, VL53L5CX_ITConfig_t *pITConfig);
int32_t VL53L5CX_GetDistance(VL53L5CX_Object_t *pObj, VL53L5CX_Result_t *pResult);
int32_t VL53L5CX_Start(VL53L5CX_Object_t *pObj, uint32_t Mode);
int32_t VL53L5CX_Stop(VL53L5CX_Object_t *pObj);
int32_t VL53L5CX_SetAddress(VL53L5CX_Object_t *pObj, uint32_t Address);
int32_t VL53L5CX_GetAddress(VL53L5CX_Object_t *pObj, uint32_t *pAddress);
int32_t VL53L5CX_SetPowerMode(VL53L5CX_Object_t *pObj, uint32_t PowerMode);
int32_t VL53L5CX_GetPowerMode(VL53L5CX_Object_t *pObj, uint32_t *pPowerMode);

/* additional methods */
int32_t VL53L5CX_XTalkCalibration(VL53L5CX_Object_t *pObj, uint16_t Reflectance, uint16_t Distance);
/**
  * @}
  */

/* RANGING_SENSOR driver structure */
extern VL53L5CX_RANGING_SENSOR_Drv_t   VL53L5CX_RANGING_SENSOR_Driver;

#ifdef __cplusplus
}
#endif

#endif /* VL53L5CX_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
