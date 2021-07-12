/**
  ******************************************************************************
  * @file    b_u585i_iot02a_motion_sensors.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the
  *          motion sensors mounted on the B_U585I_IOT02A board.
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

/* Includes ------------------------------------------------------------------*/
#include "b_u585i_iot02a_motion_sensors.h"
#include "b_u585i_iot02a_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_U585I_IOT02A
  * @{
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS MOTION_SENSORS
  * @{
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Private_Defines MOTION_SENSORS Private Defines
  * @{
  */
/* Functions IDx */
#define GYRO_ID     0U
#define ACCELERO_ID 1U
#define MAGNETO_ID  2U
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Variables MOTION_SENSORS Exported Variables
  * @{
  */
MOTION_SENSOR_Ctx_t        Motion_Sensor_Ctx[MOTION_SENSOR_INSTANCES_NBR] = {{0}, {0}};
void                      *Motion_Sensor_CompObj[MOTION_SENSOR_INSTANCES_NBR] = {0, 0};
MOTION_SENSOR_CommonDrv_t *Motion_Sensor_Drv[MOTION_SENSOR_INSTANCES_NBR] = {0, 0};
MOTION_SENSOR_FuncDrv_t   *Motion_Sensor_FuncDrv[MOTION_SENSOR_INSTANCES_NBR][MOTION_SENSOR_FUNCTIONS_NBR] = {{0, 0, 0},
  {0, 0, 0}
};
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Private_Function_Prototypes MOTION_SENSORS Private Function Prototypes
  * @{
  */
/* Components probe functions prototypes */
static int32_t ISM330DLC_Probe(uint32_t Functions);
static int32_t IIS2MDC_Probe(uint32_t Functions);
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Exported_Functions MOTION_SENSORS Exported Functions
  * @{
  */
/**
  * @brief  Initialize the motion sensor.
  * @param  Instance Motion sensor instance.
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 1U) && ((Functions & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    /* Probe the motion sensor */
    if (Instance == 0U)
    {
      /* Probe the ISM330DLC sensor */
      if (ISM330DLC_Probe(Functions) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else /* Instance = 1 */
    {
      /* Probe the IIS2MDC sensor */
      if (IIS2MDC_Probe(Functions) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
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
    /* De-initialize the motion sensor */
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
    /* Driver not initialized */
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the capabilities of the motion sensor */
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
    /* Driver not initialized */
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    /* Driver not initialized */
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Enable the motion sensor function */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
  * @retval BSP status
  */
int32_t BSP_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    /* Driver not initialized */
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Disable the motion sensor function */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
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
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor row axes */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
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
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
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
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
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
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the motion sensor full scale */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
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
  * @param  Function Motion sensor function. Could be :
  *         - MOTION_GYRO or MOTION_ACCELERO for instance 0
  *         - MOTION_MAGNETO for instance 1
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
  else if ((Instance == 0U) && ((Function & MOTION_MAGNETO) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 1U) && ((Function & (MOTION_GYRO | MOTION_ACCELERO)) != 0U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Motion_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Set the motion sensor full scale */
    index = (Function == MOTION_GYRO) ? GYRO_ID : ((Function == MOTION_ACCELERO) ? ACCELERO_ID : MAGNETO_ID);
    if (index == MAGNETO_ID)
    {
      /* Feature not supported for Magneto */
      status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }
    else
    {
      if (Motion_Sensor_FuncDrv[Instance][index]->SetFullScale(Motion_Sensor_CompObj[Instance], Fullscale) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_MOTION_SENSORS_Private_Functions MOTION_SENSORS Private Functions
  * @{
  */
/**
  * @brief  Probe the ISM330DLC motion sensor driver.
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status.
  */
static int32_t ISM330DLC_Probe(uint32_t Functions)
{
  int32_t                 status = BSP_ERROR_NONE;
  ISM330DLC_IO_t            IOCtx;
  uint8_t                 ism330dlc_id;
  static ISM330DLC_Object_t ISM330DLC_Obj;

  /* Configure the motion sensor driver */
  IOCtx.BusType     = ISM330DLC_I2C_BUS;
  IOCtx.Address     = ISM330DLC_I2C_ADD_H;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  /* Register Component Bus IO operations */
  if (ISM330DLC_RegisterBusIO(&ISM330DLC_Obj, &IOCtx) != ISM330DLC_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  /* Read the sensor ID */
  else if (ISM330DLC_ReadID(&ISM330DLC_Obj, &ism330dlc_id) != ISM330DLC_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Check if the returned sensor ID is correct */
  else if ((ism330dlc_id != ISM330DLC_ID) && (ism330dlc_id !=ISM330DLC_ID_2))
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Motion_Sensor_CompObj[0] = &ISM330DLC_Obj;
    Motion_Sensor_Drv[0]     = (MOTION_SENSOR_CommonDrv_t *) &ISM330DLC_COMMON_Driver;
    /* Initialize the component */
    if (Motion_Sensor_Drv[0]->Init(Motion_Sensor_CompObj[0]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Link the MS driver with the component driver */
      if ((Functions & MOTION_GYRO) != 0U)
      {
        Motion_Sensor_FuncDrv[0][GYRO_ID] = (MOTION_SENSOR_FuncDrv_t *) &ISM330DLC_GYRO_Driver;
      }
      if ((Functions & MOTION_ACCELERO) != 0U)
      {
        Motion_Sensor_FuncDrv[0][ACCELERO_ID] = (MOTION_SENSOR_FuncDrv_t *) &ISM330DLC_ACC_Driver;
      }
    }
  }

  return status;
}

/**
  * @brief  Probe the IIS2MDC motion sensor driver.
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_MAGNETO
  * @retval BSP status.
  */
static int32_t IIS2MDC_Probe(uint32_t Functions)
{
  int32_t                 status = BSP_ERROR_NONE;
  IIS2MDC_IO_t            IOCtx;
  uint8_t                 iis2mdc_id;
  static IIS2MDC_Object_t IIS2MDC_Obj;

  /* Configure the motion sensor driver */
  IOCtx.BusType     = IIS2MDC_I2C_BUS;
  IOCtx.Address     = IIS2MDC_I2C_ADD;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  /* Register Component Bus IO operations */
  if (IIS2MDC_RegisterBusIO(&IIS2MDC_Obj, &IOCtx) != IIS2MDC_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  /* Read the sensor ID */
  else if (IIS2MDC_ReadID(&IIS2MDC_Obj, &iis2mdc_id) != IIS2MDC_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Check if the returned sensor ID is correct */
  else if (iis2mdc_id != IIS2MDC_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Motion_Sensor_CompObj[1] = &IIS2MDC_Obj;
    Motion_Sensor_Drv[1]     = (MOTION_SENSOR_CommonDrv_t *) &IIS2MDC_COMMON_Driver;
    /* Initialize the component */
    if (Motion_Sensor_Drv[1]->Init(Motion_Sensor_CompObj[1]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Link the MS driver with the component driver */
      if ((Functions & MOTION_MAGNETO) != 0U)
      {
        Motion_Sensor_FuncDrv[1][MAGNETO_ID] = (MOTION_SENSOR_FuncDrv_t *) &IIS2MDC_MAG_Driver;
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
