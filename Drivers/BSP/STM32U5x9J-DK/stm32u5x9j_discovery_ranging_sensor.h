/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_ranging_sensor.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the 53l5a1_ranging_sensor.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5x9J_DISCOVERY_RANGING_SENSOR_H
#define STM32U5x9J_DISCOVERY_RANGING_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_errno.h"
#include "stm32u5x9j_discovery_conf.h"
#include "ranging_sensor.h"
#include "../Components/vl53l5cx/vl53l5cx.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY STM32U5x9J_DISCOVERY
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_RANGING_SENSOR RANGING SENSOR
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_RANGING_SENSOR_Exported_Constants RANGING SENSOR Exported Constants
  * @{
  */
#define RANGING_SENSOR_INSTANCES_NBR       3U
#define RANGING_SENSOR_VL53L5CX_ADDRESS    (VL53L5CX_DEVICE_ADDRESS)
#define RANGING_SENSOR_NB_TARGET_PER_ZONE  (VL53L5CX_NB_TARGET_PER_ZONE)
#define RANGING_SENSOR_MAX_NB_ZONES        (VL53L5CX_MAX_NB_ZONES)

#define RANGING_SENSOR_POWERMODE_WAKEUP    (VL53L5CX_POWER_MODE_WAKEUP)
#define RANGING_SENSOR_POWERMODE_SLEEP     (VL53L5CX_POWER_MODE_SLEEP)

#define RS_PROFILE_4x4_CONTINUOUS          (VL53L5CX_PROFILE_4x4_CONTINUOUS)
#define RS_PROFILE_4x4_AUTONOMOUS          (VL53L5CX_PROFILE_4x4_AUTONOMOUS)
#define RS_PROFILE_8x8_CONTINUOUS          (VL53L5CX_PROFILE_8x8_CONTINUOUS)
#define RS_PROFILE_8x8_AUTONOMOUS          (VL53L5CX_PROFILE_8x8_AUTONOMOUS)

#define RS_IT_DEFAULT        (VL53L5CX_IT_DEFAULT)       /*!< IT if new measurement is ready (no thresholds) */
#define RS_IT_IN_WINDOW      (VL53L5CX_IT_IN_WINDOW)     /*!< IT if distance > thresh_high */
#define RS_IT_OUT_OF_WINDOW  (VL53L5CX_IT_OUT_OF_WINDOW) /*!< IT if distance < LowThreshold OR distance > HighThreshold */
#define RS_IT_BELOW_LOW      (VL53L5CX_IT_BELOW_LOW)     /*!< IT if distance <= LowThreshold */
#define RS_IT_ABOVE_HIGH     (VL53L5CX_IT_ABOVE_HIGH)    /*!< IT if distance > HighThreshold */
#define RS_IT_EQUAL_LOW      (VL53L5CX_IT_EQUAL_LOW)     /*!< IT if distance == LowThreshold */
#define RS_IT_NOT_EQUAL_LOW  (VL53L5CX_IT_NOT_EQUAL_LOW) /*!< IT if distance != LowThreshold */

#define RS_MODE_BLOCKING_CONTINUOUS        (VL53L5CX_MODE_BLOCKING_CONTINUOUS)
#define RS_MODE_BLOCKING_ONESHOT           (VL53L5CX_MODE_BLOCKING_ONESHOT)
#define RS_MODE_ASYNC_CONTINUOUS           (VL53L5CX_MODE_ASYNC_CONTINUOUS)
#define RS_MODE_ASYNC_ONESHOT              (VL53L5CX_MODE_ASYNC_ONESHOT)

#define VL53L5A1_DEV_LEFT      (0U) /*!< left satellite device */
#define VL53L5A1_DEV_CENTER    (1U) /*!< center (built-in) device */
#define VL53L5A1_DEV_RIGHT     (2U) /*!< right satellite device */

/* Definition Sensor control Pins */
#define VL53L5A1_LP_PIN        GPIO_PIN_14
#define VL53L5A1_LP_PORT       GPIOE

/* Definition Sensor interrupt Pins */
#define VL53L5A1_INT_PIN       GPIO_PIN_5
#define VL53L5A1_INT_PORT      GPIOB

/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_RANGING_SENSOR_Exported_Types RANGING SENSOR Exported Types
  * @{
  */
typedef struct
{
  uint32_t NumberOfZones;
  uint32_t MaxNumberOfTargetsPerZone;
  uint32_t CustomROI;               /*!< Not available: 0, Available: 1 */
  uint32_t ThresholdDetection;      /*!< Not available: 0, Available: 1 */
} RANGING_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t RangingProfile;
  uint32_t TimingBudget;            /*!< Expressed in milliseconds */
  uint32_t Frequency;               /*!< Expressed in Hz */
  uint32_t EnableAmbient;           /*!< Enable: 1, Disable: 0 */
  uint32_t EnableSignal;            /*!< Enable: 1, Disable: 0 */
} RANGING_SENSOR_ProfileConfig_t;

typedef struct
{
  uint8_t TopLeftX;   /*!< Top Left x coordinate */
  uint8_t TopLeftY;   /*!< Top Left y coordinate */
  uint8_t BotRightX;  /*!< Bot Right x coordinate */
  uint8_t BotRightY;  /*!< Bot Right y coordinate */
} RANGING_SENSOR_ROIConfig_t;

typedef struct
{
  uint32_t Criteria;      /*!< Interrupt generation criteria */
  uint32_t LowThreshold;  /*!< Expressed in millimeters */
  uint32_t HighThreshold; /*!< Expressed in millimeters */
} RANGING_SENSOR_ITConfig_t;

typedef struct
{
  uint8_t NumberOfTargets;
  uint32_t Distance[RANGING_SENSOR_NB_TARGET_PER_ZONE]; /*!< millimeters */
  uint32_t Status[RANGING_SENSOR_NB_TARGET_PER_ZONE];   /*!< OK: 0, NOK: !0 */
  float_t Ambient[RANGING_SENSOR_NB_TARGET_PER_ZONE];   /*!< kcps / spad */
  float_t Signal[RANGING_SENSOR_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
} RANGING_SENSOR_ZoneResult_t;

typedef struct
{
  uint32_t NumberOfZones;
  RANGING_SENSOR_ZoneResult_t ZoneResult[RANGING_SENSOR_MAX_NB_ZONES];
} RANGING_SENSOR_Result_t;
/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_RANGING_SENSOR_Exported_Variables RANGING SENSOR Exported Variables
  * @{
  */
extern void *VL53L5A1_RANGING_SENSOR_CompObj[RANGING_SENSOR_INSTANCES_NBR];
/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_RANGING_SENSOR_Exported_Functions RANGING SENSOR Exported Functions
  * @{
  */
int32_t BSP_RANGING_SENSOR_Init(uint32_t Instance);
int32_t BSP_RANGING_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_RANGING_SENSOR_ReadID(const uint32_t Instance, uint32_t *pId);
int32_t BSP_RANGING_SENSOR_GetCapabilities(const uint32_t Instance, RANGING_SENSOR_Capabilities_t *pCapabilities);
int32_t BSP_RANGING_SENSOR_ConfigProfile(const uint32_t Instance, RANGING_SENSOR_ProfileConfig_t *pConfig);
int32_t BSP_RANGING_SENSOR_ConfigROI(const uint32_t Instance, RANGING_SENSOR_ROIConfig_t *pConfig);
int32_t BSP_RANGING_SENSOR_ConfigIT(const uint32_t Instance, RANGING_SENSOR_ITConfig_t *pConfig);
int32_t BSP_RANGING_SENSOR_GetDistance(const uint32_t Instance, RANGING_SENSOR_Result_t *pResult);
int32_t BSP_RANGING_SENSOR_Start(uint32_t Instance, uint32_t Mode);
int32_t BSP_RANGING_SENSOR_Stop(uint32_t Instance);
int32_t BSP_RANGING_SENSOR_SetAddress(uint32_t Instance, uint32_t Address);
int32_t BSP_RANGING_SENSOR_GetAddress(const uint32_t Instance, uint32_t *pAddress);
int32_t BSP_RANGING_SENSOR_SetPowerMode(uint32_t Instance, uint32_t PowerMode);
int32_t BSP_RANGING_SENSOR_GetPowerMode(const uint32_t Instance, uint32_t *pPowerMode);
int32_t BSP_RANGING_SENSOR_XTalkCalibration(uint32_t Instance, uint16_t Reflectance, uint16_t Distance);
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

#endif /* STM32U5x9J_DISCOVERY_RANGING_SENSOR_H */

