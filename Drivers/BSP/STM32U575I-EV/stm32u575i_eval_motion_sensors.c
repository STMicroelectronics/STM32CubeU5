/**
  ******************************************************************************
  * @file    stm32u575i_eval_motion_sensors.c
  * @author  MCD Application Team
  * @brief   This file provides the motion sensor driver for the
  *          STM32U575I-EVAL board.
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
  @verbatim
  1. How To use this driver:
  --------------------------
   - This driver is used to drive the motion sensor module of the STM32U575I-EVAL
     discovery board.
     The motion sensor IC is a LSM6DSO.

  2. Driver description:
  ---------------------
    + Initialization steps:
       o Call BSP_MOTION_SENSOR_Init() to initialize motion sensor. You can
         select functions with "Functions" parameter (gyroscope, accelerometer
         or magnetometer). You can initialize multiple functions with only one
         call of BSP_MOTION_SENSOR_Init().
       o Call BSP_MOTION_SENSOR_ReadID() to get motion sensor chipset identifier.
       o Call BSP_MOTION_SENSOR_GetCapabilities() to get motion sensor
         capabilities.

    + Motion sensor use
       o Call BSP_MOTION_SENSOR_Enable() (BSP_MOTION_SENSOR_Disable()) to enable
         (disable) motion sensor function.
       o Call BSP_MOTION_SENSOR_GetAxes() to get axes of one function.
       o Call BSP_MOTION_SENSOR_GetAxesRaw() to get raw axes of one function.
       o Call BSP_MOTION_SENSOR_GetSensitivity() to get sensitivity of one function.
       o Call BSP_MOTION_SENSOR_GetOutputDataRate() to get the output data rate
         of one function.
       o Call BSP_MOTION_SENSOR_SetOutputDataRate() to set the output data rate
         of one function.
       o Call BSP_MOTION_SENSOR_GetFullScale() to get the full scale of one
         function.
       o Call BSP_MOTION_SENSOR_SetFullScale() to set the full scale of one
         function.

    + De-initialization steps:
       o Call BSP_MOTION_SENSOR_DeInit() to de-initialize motion sensor.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_motion_sensors.h"
#include "stm32u575i_eval_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS MOTION SENSORS
  * @{
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Private_Defines MOTION SENSORS Private Defines
  * @{
  */
#define GYRO_ID     MOTION_GYRO
#define ACCELERO_ID MOTION_ACCELERO
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Variables MOTION SENSORS Exported Variables
  * @{
  */
MOTION_SENSOR_Ctx_t        Motion_Sensor_Ctx[MOTION_SENSOR_INSTANCES_NBR] = {{0}};
void                      *Motion_Sensor_CompObj[MOTION_SENSOR_INSTANCES_NBR] = {0};
MOTION_SENSOR_CommonDrv_t *Motion_Sensor_Drv[MOTION_SENSOR_INSTANCES_NBR] = {0};
MOTION_SENSOR_FuncDrv_t   *Motion_Sensor_FuncDrv[MOTION_SENSOR_INSTANCES_NBR][MOTION_SENSOR_FUNCTIONS_NBR] = {{
    0,
    0,
    0
  }
};
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Private_Function_Prototypes MOTION SENSORS Private
  *   Function Prototypes
  * @{
  */
static int32_t LSM6DSO_Probe(uint32_t Functions);
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_MOTION_SENSORS_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the motion sensor.
  * @param  Instance Motion sensor instance.
  * @param  Functions Motion sensor functions.
  *   This parameter can be any combination of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((Functions & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    /* Probe the motion sensor */
    if (LSM6DSO_Probe(Functions) != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Store current initialized functions */
      Motion_Sensor_Ctx[Instance].Functions |= Functions;
    }
  }
  return status;
}

/**
  * @brief  De-initialize the motion sensor.
  * @param  Instance Motion sensor instance.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Motion_Sensor_Ctx[Instance].Functions != 0U)
  {
    if (Motion_Sensor_Drv[Instance]->DeInit(Motion_Sensor_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Reset current initialized functions */
      Motion_Sensor_Ctx[Instance].Functions = 0U;
    }
  }
  else
  {
    /* Nothing to do (not initialized) */
  }
  return status;
}

/**
  * @brief  Get motion sensor capabilities.
  * @param  Instance Motion sensor instance.
  * @param  Capabilities Pointer to motion sensor capabilities.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetCapabilities(uint32_t Instance, MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Capabilities == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Motion_Sensor_Ctx[Instance].Functions == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    if (Motion_Sensor_Drv[Instance]->GetCapabilities(Motion_Sensor_CompObj[Instance], Capabilities) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Read motion sensor ID.
  * @param  Instance Motion sensor instance.
  * @param  Id Pointer to motion sensor ID.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Id == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Motion_Sensor_Ctx[Instance].Functions == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Read the motion sensor ID */
    if (Motion_Sensor_Drv[Instance]->ReadID(Motion_Sensor_CompObj[Instance], Id) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Enable one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function to enable.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Enable the motion sensor function */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->Enable(Motion_Sensor_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Disable one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function to disable.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Disable the motion sensor function */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->Disable(Motion_Sensor_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get axes from one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Axes Pointer to motion sensor axes.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, BSP_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Axes == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor axes */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->GetAxes(Motion_Sensor_CompObj[Instance], Axes) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get raw axes from one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Axes Pointer to motion sensor raw axes.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, BSP_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Axes == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor raw axes */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->GetAxesRaw(Motion_Sensor_CompObj[Instance], Axes) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get sensitivity from one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Sensitivity Pointer to sensitivity.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Sensitivity == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor sensitivity */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->GetSensitivity(Motion_Sensor_CompObj[Instance], Sensitivity) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get output data rate from one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Odr Pointer to output data rate.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Odr == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor output data rate */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->GetOutputDataRate(Motion_Sensor_CompObj[Instance], Odr) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set output data rate of one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Odr Output data rate.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Set the motion sensor output data rate */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->SetOutputDataRate(Motion_Sensor_CompObj[Instance], Odr) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get full scale value from one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Fullscale Pointer to full scale value.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if ((Instance >= MOTION_SENSOR_INSTANCES_NBR) || (Fullscale == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor full scale value */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->GetFullScale(Motion_Sensor_CompObj[Instance], Fullscale) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set full scale value of one motion sensor function.
  * @param  Instance Motion sensor instance.
  * @param  Function Motion sensor function.
  *   This parameter can be one of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @param  Fullscale Full scale value.
  * @retval BSP status.
  */
int32_t BSP_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (Function == MOTION_MAGNETO))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Set the motion sensor full scale value */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ACCELERO_ID;
    if (Motion_Sensor_FuncDrv[Instance][index]->SetFullScale(Motion_Sensor_CompObj[Instance], Fullscale) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_MOTION_SENSORS_Private_Functions MOTION SENSORS Private Functions
  * @{
  */
/**
  * @brief  Probe the LSM6DSO motion sensor driver.
  * @param  Functions Motion sensor functions.
  *   This parameter can be any combination of following parameters:
  *     @arg  MOTION_GYRO
  *     @arg  MOTION_ACCELERO
  *     @arg  MOTION_MAGNETO
  * @retval BSP status.
  */
static int32_t LSM6DSO_Probe(uint32_t Functions)
{
  int32_t                 status = BSP_ERROR_NONE;
  LSM6DSO_IO_t            IOCtx;
  uint8_t                 lsm6dso_id;
  static LSM6DSO_Object_t LSM6DSO_Obj;

  /* Configure the motion sensor driver */
  IOCtx.BusType     = LSM6DSO_I2C_BUS;
  IOCtx.Address     = LSM6DSO_I2C_ADD_L;
  IOCtx.Init        = BSP_I2C3_Init;
  IOCtx.DeInit      = BSP_I2C3_DeInit;
  IOCtx.ReadReg     = BSP_I2C3_ReadReg;
  IOCtx.WriteReg    = BSP_I2C3_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if (LSM6DSO_RegisterBusIO(&LSM6DSO_Obj, &IOCtx) != LSM6DSO_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  else if (LSM6DSO_ReadID(&LSM6DSO_Obj, &lsm6dso_id) != LSM6DSO_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (lsm6dso_id != (uint8_t) LSM6DSO_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Motion_Sensor_CompObj[0] = &LSM6DSO_Obj;
    Motion_Sensor_Drv[0] = (MOTION_SENSOR_CommonDrv_t *) &LSM6DSO_COMMON_Driver;
    if (Motion_Sensor_Drv[0]->Init(Motion_Sensor_CompObj[0]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if ((Functions & MOTION_GYRO) != 0U)
      {
        Motion_Sensor_FuncDrv[0][GYRO_ID] = (MOTION_SENSOR_FuncDrv_t *) &LSM6DSO_GYRO_Driver;
      }
      if ((Functions & MOTION_ACCELERO) != 0U)
      {
        Motion_Sensor_FuncDrv[0][ACCELERO_ID] = (MOTION_SENSOR_FuncDrv_t *) &LSM6DSO_ACC_Driver;
      }
    }
  }

  return status;
}
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
