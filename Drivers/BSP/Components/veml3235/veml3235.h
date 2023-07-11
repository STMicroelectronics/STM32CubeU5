/**
  ******************************************************************************
  * @file    veml3235.h
  * @author  MCD Application Team
  * @brief   VEML3235 header driver file
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
#ifndef VEML3235_H
#define VEML3235_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "veml3235_reg.h"
#include <string.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VEML3235
  * @{
  */

/** @defgroup VEML3235_Exported_Types
  * @{
  */

typedef int32_t (*VEML3235_Init_Func)(void);
typedef int32_t (*VEML3235_DeInit_Func)(void);
typedef int32_t (*VEML3235_GetTick_Func)(void);
typedef int32_t (*VEML3235_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VEML3235_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VEML3235_IsReady_Func)(uint16_t, uint32_t);

typedef struct
{
  VEML3235_Init_Func          Init;
  VEML3235_DeInit_Func        DeInit;
  uint16_t                    ReadAddress;
  uint16_t                    WriteAddress;
  VEML3235_IsReady_Func       IsReady;
  VEML3235_WriteReg_Func      WriteReg;
  VEML3235_ReadReg_Func       ReadReg;
  VEML3235_GetTick_Func       GetTick;
} VEML3235_IO_t;


typedef struct
{
  VEML3235_IO_t         IO;
  veml3235_ctx_t        Ctx;
  uint8_t IsInitialized;
  uint8_t IsContinuous;
  uint8_t IsStarted;
} VEML3235_Object_t;


typedef struct
{
  uint8_t NumberOfChannels;  /*!< Max: LIGHT_SENSOR_MAX_CHANNELS */
  uint8_t FlickerDetection;  /*!< Not available: 0, Available: 1 */
  uint8_t Autogain;          /*!< Not available: 0, Available: 1 */
} VEML3235_Capabilities_t;

typedef struct
{
  int32_t (*Init)(VEML3235_Object_t *);
  int32_t (*DeInit)(VEML3235_Object_t *);
  int32_t (*ReadID)(VEML3235_Object_t *, uint32_t *);
  int32_t (*GetCapabilities)(VEML3235_Object_t *, VEML3235_Capabilities_t *);
  int32_t (*SetExposureTime)(VEML3235_Object_t *, uint32_t);
  int32_t (*GetExposureTime)(VEML3235_Object_t *, uint32_t *);
  int32_t (*SetGain)(VEML3235_Object_t *, uint8_t, uint32_t);
  int32_t (*GetGain)(VEML3235_Object_t *, uint8_t, uint32_t *);
  int32_t (*SetInterMeasurementTime)(VEML3235_Object_t *, uint32_t);
  int32_t (*GetInterMeasurementTime)(VEML3235_Object_t *, uint32_t *);
  int32_t (*Start)(VEML3235_Object_t *, uint32_t);
  int32_t (*Stop)(VEML3235_Object_t *);
  int32_t (*StartFlicker)(VEML3235_Object_t *, uint8_t, uint8_t);
  int32_t (*StopFlicker)(VEML3235_Object_t *);
  int32_t (*GetValues)(VEML3235_Object_t *, uint32_t *);
  int32_t (*SetControlMode)(VEML3235_Object_t *, uint32_t, uint32_t);
} VEML3235_Drv_t;

/**
  * @}
  */

/** @defgroup VEML3235_Exported_Constants
  * @{
  */

/* VEML3235 error codes */
#define VEML3235_OK              (0)
#define VEML3235_ERROR           (-1)
#define VEML3235_INVALID_PARAM   (-2)

/* softowre ID */
#define VEML3235_ID              (0x3235U)

/**
  * @brief  VEML3235 Features Parameters
  */

/* VEML3235 Channel */
#define VEML3235_ALS_CHANNEL     (0U)
#define VEML3235_WHITE_CHANNEL   (1U)
#define VEML3235_MAX_CHANNELS    (2U)  /*!< Number of channels of the device */

/* VEML3235 capture modes */
#define VEML3235_MODE_CONTINUOUS (1U)

/* VEML3235 I2C ADDRESS */
#define VEML3235_I2C_READ_ADD    0x21
#define VEML3235_I2C_WRITE_ADD   0x20

/* driver structure */
extern VEML3235_Drv_t VEML3235_Driver;

/**
  * @}
  */

/** @addtogroup VEML3235_Exported_Functions VEML3235 Exported Functions
  * @{
  */

int32_t VEML3235_RegisterBusIO(VEML3235_Object_t *pObj, VEML3235_IO_t *pIO);
int32_t VEML3235_Init(VEML3235_Object_t *pObj);
int32_t VEML3235_DeInit(VEML3235_Object_t *pObj);
int32_t VEML3235_ReadID(VEML3235_Object_t *pObj, uint32_t *pId);
int32_t VEML3235_SetExposureTime(VEML3235_Object_t *pObj, uint32_t ExposureTime);
int32_t VEML3235_GetExposureTime(VEML3235_Object_t *pObj, uint32_t *pExposureTime);
int32_t VEML3235_GetCapabilities(VEML3235_Object_t *pObj,  VEML3235_Capabilities_t *pCapabilities);
int32_t VEML3235_SetGain(VEML3235_Object_t *pObj, uint8_t Channel, uint32_t Gain);
int32_t VEML3235_GetGain(VEML3235_Object_t *pObj, uint8_t Channel, uint32_t *pGain);
int32_t VEML3235_SetInterMeasurementTime(VEML3235_Object_t *pObj, uint32_t InterMeasurementTime);
int32_t VEML3235_GetInterMeasurementTime(VEML3235_Object_t *pObj, uint32_t *pInterMeasurementTime);
int32_t VEML3235_Start(VEML3235_Object_t *pObj, uint32_t Mode);
int32_t VEML3235_Stop(VEML3235_Object_t *pObj);
int32_t VEML3235_GetValues(VEML3235_Object_t *pObj, uint32_t *pValues);
int32_t VEML3235_StartFlicker(VEML3235_Object_t *pObj, uint8_t Channel, uint8_t OutputMode);
int32_t VEML3235_StopFlicker(VEML3235_Object_t *pObj);
int32_t VEML3235_SetControlMode(VEML3235_Object_t *pObj, uint32_t ControlMode, uint32_t Value);

int32_t VEML3235_SetPowerSavingMode(VEML3235_Object_t *pObj, uint32_t PowerMode);
int32_t VEML3235_GetPowerSavingMode(VEML3235_Object_t *pObj, uint32_t *pPowerMode);
int32_t VEML3235_SetPersistence(VEML3235_Object_t *pObj, uint32_t Persistence);
int32_t VEML3235_GetPersistence(VEML3235_Object_t *pObj, uint32_t *pPersistence);
int32_t VEML3235_GetWhiteValues(VEML3235_Object_t *pObj, uint32_t *pValues);
/* interrupt Mangement Functions */
int32_t VEML3235_SetHighThreshold(VEML3235_Object_t *pObj, uint16_t Threshold);
int32_t VEML3235_GetHighThreshold(VEML3235_Object_t *pObj, uint32_t *pThreshold);
int32_t VEML3235_SetLowThreshold(VEML3235_Object_t *pObj, uint16_t Threshold);
int32_t VEML3235_GetLowThreshold(VEML3235_Object_t *pObj, uint32_t *pThreshold);
int32_t VEML3235_GetIntStatus(VEML3235_Object_t *pObj, uint32_t *pStatus);
int32_t VEML3235_Disable_IT(VEML3235_Object_t *pObj);
int32_t VEML3235_Enable_IT(VEML3235_Object_t *pObj);

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

#endif /* VEML3235_H */
