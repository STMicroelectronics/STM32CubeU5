/**
  ******************************************************************************
  * @file    sitronix.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          sitronix.c driver.
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
#ifndef SITRONIX_H
#define SITRONIX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sitronix_reg.h"
#include "sitronix_conf.h"

/* Macros --------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @addtogroup SITRONIX
  * @{
  */

/** @defgroup SITRONIX_Exported_Constants SITRONIX Exported Constants
  * @{
  */
#define SITRONIX_OK                           (0)
#define SITRONIX_ERROR                        (-1)

/* Max detectable simultaneous touches */
#define SITRONIX_MAX_DETECTABLE_TOUCH           10U

/* Touch FT6XX6 IDs */
#define SITRONIX_ID                           0x02U

/* Values Pn_XH and Pn_YH related */
#define SITRONIX_TOUCH_EVT_FLAG_PRESS_DOWN    0x20U
#define SITRONIX_TOUCH_EVT_FLAG_LIFT_UP       0x60U
#define SITRONIX_TOUCH_EVT_FLAG_CONTACT       0x80U
#define SITRONIX_TOUCH_EVT_FLAG_NO_EVENT      0x00U
#define SITRONIX_TOUCH_POS_MSB_MASK           0x07U
#define SITRONIX_TOUCH_POS_LSB_MASK           0x70U

/* Point 1 registers */
#define SITRONIX_P1_XH_REG                    0x09U
#define SITRONIX_P1_XL_REG                    0x0AU
#define SITRONIX_P1_YH_REG                    0x0BU
#define SITRONIX_P1_YL_REG                    0x0CU

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup SITRONIX_Exported_Types SITRONIX Exported Types
  * @{
  */
typedef struct
{
  uint32_t  Radian;
  uint32_t  OffsetLeftRight;
  uint32_t  OffsetUpDown;
  uint32_t  DistanceLeftRight;
  uint32_t  DistanceUpDown;
  uint32_t  DistanceZoom;
} SITRONIX_Gesture_Init_t;

typedef int32_t (*SITRONIX_Init_Func)(void);
typedef int32_t (*SITRONIX_DeInit_Func)(void);
typedef int32_t (*SITRONIX_GetTick_Func)(void);
typedef int32_t (*SITRONIX_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*SITRONIX_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*SITRONIX_ReadData_Func)(uint16_t, uint8_t *, uint16_t);

typedef struct
{
  SITRONIX_Init_Func          Init;
  SITRONIX_DeInit_Func        DeInit;
  uint16_t                    Address;
  SITRONIX_WriteReg_Func      WriteReg;
  SITRONIX_ReadReg_Func       ReadReg;
  SITRONIX_ReadData_Func      ReadData;
  SITRONIX_GetTick_Func       GetTick;
} SITRONIX_IO_t;

typedef struct
{
  uint32_t  TouchDetected;
  uint32_t  TouchX;
  uint32_t  TouchY;
} SITRONIX_State_t;

typedef struct
{
  uint32_t  TouchDetected;
  uint32_t  TouchX[SITRONIX_MAX_DETECTABLE_TOUCH];
  uint32_t  TouchY[SITRONIX_MAX_DETECTABLE_TOUCH];
  uint32_t  TouchWeight[SITRONIX_MAX_DETECTABLE_TOUCH];
  uint32_t  TouchEvent[SITRONIX_MAX_DETECTABLE_TOUCH];
  uint32_t  TouchArea[SITRONIX_MAX_DETECTABLE_TOUCH];
} SITRONIX_MultiTouch_State_t;

typedef struct
{
  SITRONIX_IO_t         IO;
  sitronix_ctx_t        Ctx;
  uint8_t             IsInitialized;
} SITRONIX_Object_t;

typedef struct
{
  uint8_t   MultiTouch;
  uint8_t   Gesture;
  uint8_t   MaxTouch;
  uint32_t  MaxXl;
  uint32_t  MaxYl;
} SITRONIX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(SITRONIX_Object_t *);
  int32_t (*DeInit)(SITRONIX_Object_t *);
  int32_t (*GestureConfig)(SITRONIX_Object_t *, SITRONIX_Gesture_Init_t *);
  int32_t (*ReadID)(SITRONIX_Object_t *, uint32_t *);
  int32_t (*GetState)(SITRONIX_Object_t *, SITRONIX_State_t *);
  int32_t (*GetMultiTouchState)(SITRONIX_Object_t *, SITRONIX_MultiTouch_State_t *);
  int32_t (*GetGesture)(SITRONIX_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(SITRONIX_Object_t *, SITRONIX_Capabilities_t *);
  int32_t (*EnableIT)(SITRONIX_Object_t *);
  int32_t (*DisableIT)(SITRONIX_Object_t *);
  int32_t (*ClearIT)(SITRONIX_Object_t *);
  int32_t (*ITStatus)(SITRONIX_Object_t *);
} SITRONIX_TS_Drv_t;
/**
  * @}
  */

/** @addtogroup SITRONIX_Exported_Variables
  * @{
  */
extern SITRONIX_TS_Drv_t SITRONIX_TS_Driver;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup SITRONIX_Exported_Functions
  * @{
  */

int32_t SITRONIX_RegisterBusIO(SITRONIX_Object_t *pObj, SITRONIX_IO_t *pIO);
int32_t SITRONIX_Init(SITRONIX_Object_t *pObj);
int32_t SITRONIX_DeInit(SITRONIX_Object_t *pObj);
int32_t SITRONIX_GestureConfig(SITRONIX_Object_t *pObj, SITRONIX_Gesture_Init_t *GestureInit);
int32_t SITRONIX_ReadID(SITRONIX_Object_t *pObj, uint32_t *Id);
int32_t SITRONIX_GetState(SITRONIX_Object_t *pObj, SITRONIX_State_t *State);
int32_t SITRONIX_GetMultiTouchState(SITRONIX_Object_t *pObj, SITRONIX_MultiTouch_State_t *State);
int32_t SITRONIX_GetGesture(SITRONIX_Object_t *pObj, uint8_t *GestureId);
int32_t SITRONIX_EnableIT(SITRONIX_Object_t *pObj);
int32_t SITRONIX_DisableIT(SITRONIX_Object_t *pObj);
int32_t SITRONIX_ITStatus(SITRONIX_Object_t *pObj);
int32_t SITRONIX_ClearIT(SITRONIX_Object_t *pObj);
int32_t SITRONIX_GetCapabilities(SITRONIX_Object_t *pObj, SITRONIX_Capabilities_t *Capabilities);

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
#endif /* SITRONIX_H */
