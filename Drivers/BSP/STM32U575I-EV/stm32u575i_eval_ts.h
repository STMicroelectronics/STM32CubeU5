/**
  ******************************************************************************
  * @file    stm32u575i_eval_ts.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u575i_eval_ts.c driver.
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
#ifndef STM32U575I_EVAL_TS_H
#define STM32U575I_EVAL_TS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"
#include "../Components/Common/ts.h"
#include "../Components/sx8651/sx8651.h"
#include "../Components/mfxstm32l152/mfxstm32l152.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_TS
  * @{
  */

/** @defgroup STM32U575I_EVAL_TS_Exported_Constants TS Exported Constants
  * @{
  */
/* TS instances */
#define TS_INSTANCES_NBR 1UL

/* TS orientations */
#define TS_ORIENTATION_PORTRAIT          0U
#define TS_ORIENTATION_LANDSCAPE         1U
#define TS_ORIENTATION_PORTRAIT_ROT180   2U
#define TS_ORIENTATION_LANDSCAPE_ROT180  3U
#define TS_ORIENTATION_LANDSCAPE_MIRROR  4U

/* TS I2C address */
#define MFX_I2C_ADDRESS             0x84U
#define TS_I2C_ADDRESS              0x90U

/* TS EXTI Line */
#define TS_EXTI_LINE                EXTI_LINE_15
/**
  * @}
  */


/** @defgroup STM32U575I_EVAL_TS_Exported_Types TS Exported Types
  * @{
  */
typedef struct
{
  uint32_t   Width;                  /* Screen width */
  uint32_t   Height;                 /* Screen height */
  uint32_t   Orientation;            /* Touch screen orientation */
  uint32_t   Accuracy;               /* Expressed in pixel and means the x or y difference vs old
                                        position to consider the new values valid */
} TS_Init_t;

typedef struct
{
  uint8_t   MultiTouch;
  uint8_t   Gesture;
  uint8_t   MaxTouch;
  uint32_t  MaxXl;
  uint32_t  MaxYl;
} TS_Capabilities_t;

typedef struct
{
  uint32_t  TouchDetected;
  uint32_t  TouchX;
  uint32_t  TouchY;
} TS_State_t;

typedef struct
{
  uint32_t  TouchDetected;
  uint32_t  TouchX[TS_TOUCH_NBR];
  uint32_t  TouchY[TS_TOUCH_NBR];
  uint32_t  TouchWeight[TS_TOUCH_NBR];
  uint32_t  TouchEvent[TS_TOUCH_NBR];
  uint32_t  TouchArea[TS_TOUCH_NBR];
} TS_MultiTouch_State_t;

typedef struct
{
  uint32_t  Radian;
  uint32_t  OffsetLeftRight;
  uint32_t  OffsetUpDown;
  uint32_t  DistanceLeftRight;
  uint32_t  DistanceUpDown;
  uint32_t  DistanceZoom;
} TS_Gesture_Config_t;

typedef struct
{
  uint32_t   Width;
  uint32_t   Height;
  uint32_t   Orientation;
  uint32_t   Accuracy;
  uint32_t   MaxX;
  uint32_t   MaxY;
  uint32_t   PreviousX[TS_TOUCH_NBR];
  uint32_t   PreviousY[TS_TOUCH_NBR];
} TS_Ctx_t;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_TS_Exported_Variables TS Exported Variables
  * @{
  */
extern void     *Ts_CompObj[TS_INSTANCES_NBR];
extern TS_Drv_t *Ts_Drv[TS_INSTANCES_NBR];
extern TS_Ctx_t  Ts_Ctx[TS_INSTANCES_NBR];

extern EXTI_HandleTypeDef hts_exti[TS_INSTANCES_NBR];
extern IRQn_Type          Ts_IRQn[TS_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_TS_Exported_Functions TS Exported Functions
  * @{
  */
int32_t  BSP_TS_Init(uint32_t Instance, TS_Init_t *TS_Init);
int32_t  BSP_TS_DeInit(uint32_t Instance);
int32_t  BSP_TS_EnableIT(uint32_t Instance);
int32_t  BSP_TS_DisableIT(uint32_t Instance);
int32_t  BSP_TS_Set_Orientation(uint32_t Instance, uint32_t Orientation);
int32_t  BSP_TS_Get_Orientation(uint32_t Instance, uint32_t *Orientation);
int32_t  BSP_TS_GetState(uint32_t Instance, TS_State_t *TS_State);
int32_t  BSP_TS_Get_MultiTouchState(uint32_t Instance, TS_MultiTouch_State_t *TS_State);
int32_t  BSP_TS_GestureConfig(uint32_t Instance, TS_Gesture_Config_t *GestureConfig);
int32_t  BSP_TS_GetGestureId(uint32_t Instance, uint32_t *GestureId);
int32_t  BSP_TS_GetCapabilities(uint32_t Instance, TS_Capabilities_t *Capabilities);
void     BSP_TS_Callback(uint32_t Instance);
void     BSP_TS_IRQHandler(uint32_t Instance);
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

#endif /* STM32U575I_EVAL_TS_H */
