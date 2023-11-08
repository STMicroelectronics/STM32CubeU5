/**
  ******************************************************************************
  * @file    stm32u5g9j_discovery_ts.h
  * @author  MCD Application Team
  * @brief   This file is the header of stm32u5g9j_discovery_bus.c file
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
#ifndef STM32U5G9J_DK2_TS_H
#define STM32U5G9J_DK2_TS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5g9j_discovery_conf.h"
#include "stm32u5g9j_discovery_errno.h"
#include "../Components/gt911/gt911.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5G9J_DK2
  * @{
  */

/** @addtogroup STM32U5G9J_DK2_TS
  * @{
  */

/** @defgroup STM32U5G9J_DK2_TS_Exported_Constants TS Exported Constants
  * @{
  */
#ifndef USE_TS_MULTI_TOUCH
#define USE_TS_MULTI_TOUCH          1U
#endif /* USE_TS_MULTI_TOUCH */
#ifndef USE_TS_GESTURE
#define USE_TS_GESTURE              1U
#endif /* USE_TS_GESTURE */

#ifndef TS_TOUCH_NBR
#define TS_TOUCH_NBR                5U
#endif /* TS_TOUCH_NBR */

#define TS_INSTANCES_NBR            1U

/* Maximum border values of the touchscreen pad */
#define TS_MAX_WIDTH                GT911_MAX_X_LENGTH     /* Touchscreen pad max width   */
#define TS_MAX_HEIGHT               GT911_MAX_Y_LENGTH     /* Touchscreen pad max height  */

#define TS_SWAP_NONE                0x01U
#define TS_SWAP_X                   0x02U
#define TS_SWAP_Y                   0x04U
#define TS_SWAP_XY                  0x08U

/**
  * @brief TouchScreen Slave I2C address 1
  */
#define TS_I2C_ADDRESS              0xBAU

/**
  * @brief Touch screen interrupt signal
  */
#define TS_INT_PIN                   GPIO_PIN_5
#define TS_INT_GPIO_PORT             GPIOE
#define TS_INT_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define TS_INT_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define TS_INT_EXTI_IRQn             EXTI5_IRQn
#define TS_EXTI_LINE                 EXTI_LINE_5

/**
  * @}
  */


/** @defgroup STM32U5G9J_DK2_TS_Exported_Types TS Exported Types
  * @{
  */
typedef struct
{
  uint32_t   Width;                  /* Screen Width */
  uint32_t   Height;                 /* Screen Height */
  uint32_t   Orientation;            /* Touch Screen orientation from the upper left position  */
  uint32_t   Accuracy;               /* Expressed in pixel and means the x or y difference vs old
                                        position to consider the new values valid */
} TS_Init_t;

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

typedef struct
{
  uint8_t   MultiTouch;              /* Multi-touch supported : 0 means no, 1 means yes */
  uint8_t   Gesture;                 /* Gesture supported : 0 means no, 1 means yes */
  uint8_t   MaxTouch;                /* Maximum number of touches detected if multi-touch supported */
  uint32_t  MaxXl;                   /* Maximum value of x coordinate */
  uint32_t  MaxYl;                   /* Maximum value of y coordinate */
} TS_Capabilities_t;

typedef struct
{
  uint32_t  TouchDetected;          /* Touch detected : 0 means no, 1 means yes */
  uint32_t  TouchX;                 /* x coordinate */
  uint32_t  TouchY;                 /* y coordinate */
} TS_State_t;

typedef struct
{
  uint32_t  TouchDetected;                      /* Number of touch detected (from 0 to maximum of touches) */
  uint32_t  TouchX[TS_TOUCH_NBR];               /* x coordinates */
  uint32_t  TouchY[TS_TOUCH_NBR];               /* y coordinates */
} TS_MultiTouch_State_t;

#if (USE_TS_GESTURE > 0)
/**
  *  @brief TS_Gesture_Id_t
  *  Define Possible managed gesture identification values returned by touch screen
  *  driver.
  */
#define GESTURE_ID_NO_GESTURE    0x00U /*!< Gesture not defined / recognized */
#define GESTURE_ID_SWIPE_UP      0x01U /*!< Gesture Swipe Up */
#define GESTURE_ID_SWIPE_RIGHT   0x02U /*!< Gesture Swipe Right */
#define GESTURE_ID_SWIPE_DOWN    0x03U /*!< Gesture Swipe Down */
#define GESTURE_ID_SWIPE_LEFT    0x04U /*!< Gesture Swipe Left */
#define GESTURE_ID_DOUBLE_TAP    0x05U /*!< Gesture Double Tap */
#define GESTURE_ID_NB_MAX        0x06U /*!< max number of gesture id */

typedef struct
{
  uint32_t  DistanceLeftRight;  /* Maximum distance while moving left and moving right gesture.
                                    Value from 0 to TS_LENGTH/16 (default value is TS_LENGTH/8). */
  uint32_t  DistanceUpDown;     /* Maximum distance while moving up and moving down gesture.
                                    Value from 0 to TS_LENGTH/16 (default value is TS_LENGTH/8). */
  uint8_t   RefreshRate;        /* Report Rate in Gesture Mode. */
  uint8_t   GestureThreshold;   /* Touch Threshold in Gesture Mode. */
  uint8_t   Gain;               /* Gesture PGA gain. */
} TS_Gesture_Config_t;

#endif /* (USE_TS_GESTURE > 0) */

/**
  * @}
  */

/** @addtogroup STM32U5G9J_DK2_TS_Exported_Variables TS Exported Variables
  * @{
  */
extern void               *Ts_CompObj[TS_INSTANCES_NBR];
extern EXTI_HandleTypeDef hts_exti[TS_INSTANCES_NBR];
extern TS_Ctx_t           Ts_Ctx[TS_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup STM32U5G9J_DK2_TS_Exported_FunctionsPrototypes TS Exported Functions Prototypes
  * @{
  */
int32_t BSP_TS_Init(uint32_t Instance, TS_Init_t *TS_Init);
int32_t BSP_TS_DeInit(uint32_t Instance);
int32_t BSP_TS_EnableIT(uint32_t Instance);
int32_t BSP_TS_DisableIT(uint32_t Instance);
int32_t BSP_TS_GetState(uint32_t Instance, TS_State_t *TS_State);
#if (USE_TS_MULTI_TOUCH == 1)
int32_t BSP_TS_Get_MultiTouchState(uint32_t Instance, TS_MultiTouch_State_t *TS_State);
#endif /* USE_TS_MULTI_TOUCH == 1 */
#if (USE_TS_GESTURE == 1)
int32_t BSP_TS_GestureConfig(uint32_t Instance, TS_Gesture_Config_t *GestureConfig);
int32_t BSP_TS_GetGestureId(uint32_t Instance, uint32_t *GestureId);
#endif /* (USE_TS_GESTURE == 1) */
int32_t BSP_TS_Set_Orientation(uint32_t Instance, uint32_t Orientation);
int32_t BSP_TS_Get_Orientation(uint32_t Instance, uint32_t *Orientation);
int32_t BSP_TS_GetCapabilities(uint32_t Instance, TS_Capabilities_t *Capabilities);
void    BSP_TS_Callback(uint32_t Instance);
void    BSP_TS_IRQHandler(uint32_t Instance);

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

#endif /* STM32U5G9J_DK2_TS_H */
