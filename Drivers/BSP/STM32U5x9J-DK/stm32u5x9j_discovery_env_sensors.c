/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_env_sensors.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the
  *          environmental sensors mounted on the STM32U5x9J_DISCOVERY board.
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
#include "stm32u5x9j_discovery_env_sensors.h"
#include "stm32u5x9j_discovery_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS ENVIRONMENTAL SENSORS
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Private_Defines ENVIRONMENTAL SENSORS Private Defines
  * @{
  */
/* Functions IDx */
#define TEMPERATURE_ID  0U
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Variables ENVIRONMENTAL SENSORS Exported Variables
  * @{
  */
/* Environmental sensor context */
ENV_SENSOR_Ctx_t Env_Sensor_Ctx[ENV_SENSOR_INSTANCES_NBR] = {{0}};

/* Environmental sensor component object */
void *Env_Sensor_CompObj[ENV_SENSOR_INSTANCES_NBR] = {0};

/* Environmental sensor common driver */
ENV_SENSOR_CommonDrv_t *Env_Sensor_Drv[ENV_SENSOR_INSTANCES_NBR] = {0};

/* Environmental sensor function driver */
ENV_SENSOR_FuncDrv_t *Env_Sensor_FuncDrv[ENV_SENSOR_INSTANCES_NBR][ENV_SENSOR_FUNCTIONS_NBR] = {{0}};
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Private_Function_Prototypes ENVIRONMENTAL SENSORS Private Function Prototypes
  * @{
  */
static int32_t STTS22H_Probe(uint32_t Functions);
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Exported_Functions ENVIRONMENTAL SENSORS Exported Functions
  * @{
  */

/**
  * @brief  Initialize the environmental sensor.
  * @param  Instance Environmental sensor instance.
  * @param  Functions Environmental sensor functions :
  *         - ENV_TEMPERATURE for instance 0
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= ENV_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Functions != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Functions == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    /* Probe the motion sensor */
    if (Instance == 0U)
    {
      if (STTS22H_Probe(Functions) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else /* Instance != 0 */
    {
      status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
    }

    if (status == BSP_ERROR_NONE)
    {
      /* Store current initialized functions */
      Env_Sensor_Ctx[Instance].Functions |= Functions;
    }
  }
  return status;
}

/**
  * @brief  De-initialize environmental sensor.
  * @param  Instance Environmental sensor instance.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= ENV_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Env_Sensor_Ctx[Instance].Functions != 0U)
  {
    /* De-initialize the sensor */
    if (Env_Sensor_Drv[Instance]->DeInit(Env_Sensor_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Reset current initialized functions */
      Env_Sensor_Ctx[Instance].Functions = 0U;
    }
  }
  else
  {
    /* Nothing to do (not initialized) */
  }
  return status;
}

/**
  * @brief  Get environmental sensor capabilities.
  * @param  Instance Environmental sensor instance.
  * @param  Capabilities Pointer to environmental sensor capabilities.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_GetCapabilities(const uint32_t Instance, ENV_SENSOR_Capabilities_t *Capabilities)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= ENV_SENSOR_INSTANCES_NBR) || (Capabilities == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Env_Sensor_Ctx[Instance].Functions == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the sensor capabilities */
    if (Env_Sensor_Drv[Instance]->GetCapabilities(Env_Sensor_CompObj[Instance], Capabilities) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Read environmental sensor ID.
  * @param  Instance Environmental sensor instance.
  * @param  Id Pointer to environmental sensor ID.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_ReadID(const uint32_t Instance, uint8_t *Id)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= ENV_SENSOR_INSTANCES_NBR) || (Id == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Env_Sensor_Ctx[Instance].Functions == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Read the environmental sensor ID */
    if (Env_Sensor_Drv[Instance]->ReadID(Env_Sensor_CompObj[Instance], Id) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Enable one environmental sensor function.
  * @param  Instance Environmental sensor instance.
  * @param  Function Environmental sensor function :
  *         - ENV_TEMPERATURE
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index = 0;

  if (Instance >= ENV_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Function != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Function == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Env_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Enable the environmental sensor function */
    if (Function == ENV_TEMPERATURE)
    {
      index = TEMPERATURE_ID;
    }
    else
    {
      status = BSP_ERROR_WRONG_PARAM;
    }

    if (status == BSP_ERROR_NONE)
    {
      if (Env_Sensor_FuncDrv[Instance][index]->Enable(Env_Sensor_CompObj[Instance]) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  Disable one environmental sensor function.
  * @param  Instance Environmental sensor instance.
  * @param  Function Environmental sensor function :
  *         - ENV_TEMPERATURE for instance 0
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index = 0;

  if (Instance >= ENV_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Function != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Function == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Env_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Disable the environmental sensor function */
    if (Function == ENV_TEMPERATURE)
    {
      index = TEMPERATURE_ID;
    }
    else
    {
      status = BSP_ERROR_WRONG_PARAM;
    }

    if (status == BSP_ERROR_NONE)
    {
      if (Env_Sensor_FuncDrv[Instance][index]->Disable(Env_Sensor_CompObj[Instance]) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  Get output data rate from one environmental sensor function.
  * @param  Instance Environmental sensor instance.
  * @param  Function Environmental sensor function :
  *         - ENV_TEMPERATURE for instance 0
  * @param  Odr Pointer to output data rate.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_GetOutputDataRate(const uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index = 0;

  if ((Instance >= ENV_SENSOR_INSTANCES_NBR) || (Odr == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Function != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Function == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Env_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get the environmental sensor output data rate */
    if (Function == ENV_TEMPERATURE)
    {
      index = TEMPERATURE_ID;
    }
    else
    {
      status = BSP_ERROR_WRONG_PARAM;
    }

    if (status == BSP_ERROR_NONE)
    {
      if (Env_Sensor_FuncDrv[Instance][index]->GetOutputDataRate(Env_Sensor_CompObj[Instance], Odr) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  Set output data rate of one environmental sensor function.
  * @param  Instance Environmental sensor instance.
  * @param  Function Environmental sensor function :
  *         - ENV_TEMPERATURE for instance 0
  * @param  Odr Output data rate.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index = 0;

  if (Instance >= ENV_SENSOR_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Function != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Function == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Env_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Set the environmental sensor output data rate */
    if (Function == ENV_TEMPERATURE)
    {
      index = TEMPERATURE_ID;
    }
    else
    {
      status = BSP_ERROR_WRONG_PARAM;
    }

    if (status == BSP_ERROR_NONE)
    {
      if (Env_Sensor_FuncDrv[Instance][index]->SetOutputDataRate(Env_Sensor_CompObj[Instance], Odr) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  Get environmental sensor value.
  * @param  Instance Environmental sensor instance.
  * @param  Function Environmental sensor function :
  *         - ENV_TEMPERATURE for instance 0
  * @param  Value Pointer to environmental sensor value.
  * @retval BSP status.
  */
int32_t BSP_ENV_SENSOR_GetValue(const uint32_t Instance, uint32_t Function, float_t *Value)
{
  int32_t status = BSP_ERROR_NONE;
  uint8_t index = 0;

  if ((Instance >= ENV_SENSOR_INSTANCES_NBR) || (Value == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (((Instance == 0U) && (Function != ENV_TEMPERATURE))
           || ((Instance != 0U) && (Function == ENV_TEMPERATURE)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Env_Sensor_Ctx[Instance].Functions & Function) == 0U)
  {
    status = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Get environmental sensor value */
    if (Function == ENV_TEMPERATURE)
    {
      index = TEMPERATURE_ID;
    }
    else
    {
      status = BSP_ERROR_WRONG_PARAM;
    }

    if (status == BSP_ERROR_NONE)
    {
      if (Env_Sensor_FuncDrv[Instance][index]->GetValue(Env_Sensor_CompObj[Instance], Value) < 0)
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

/** @defgroup STM32U5x9J_DISCOVERY_ENV_SENSORS_Private_Functions ENVIRONMENTAL SENSORS Private Functions
  * @{
  */
/**
  * @brief  Probe the STTS22H environmental sensor driver.
  * @param  Functions Environmental sensor functions :
  *         - ENV_TEMPERATURE
  * @retval BSP status.
  */
static int32_t STTS22H_Probe(uint32_t Functions)
{
  int32_t                 status = BSP_ERROR_NONE;
  STTS22H_IO_t            IOCtx;
  uint8_t                 stts22h_id;
  static STTS22H_Object_t STTS22H_Obj;

  /* Configure the environmental sensor driver */
  IOCtx.BusType     = STTS22H_I2C_BUS;
  IOCtx.Address     = STTS22H_I2C_ADD_L;
  IOCtx.Init        = BSP_I2C3_Init;
  IOCtx.DeInit      = BSP_I2C3_DeInit;
  IOCtx.ReadReg     = BSP_I2C3_ReadReg;
  IOCtx.WriteReg    = BSP_I2C3_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  /* Register Component Bus IO operations */
  if (STTS22H_RegisterBusIO(&STTS22H_Obj, &IOCtx) != STTS22H_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  /* Read the sensor ID */
  else if (STTS22H_ReadID(&STTS22H_Obj, &stts22h_id) != STTS22H_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Check if the returned sensor ID is correct */
  else if (stts22h_id != STTS22H_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Env_Sensor_CompObj[0] = &STTS22H_Obj;
    Env_Sensor_Drv[0]     = (ENV_SENSOR_CommonDrv_t *) &STTS22H_COMMON_Driver;
    /* Initialize the component */
    if (Env_Sensor_Drv[0]->Init(Env_Sensor_CompObj[0]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      if ((Functions & ENV_TEMPERATURE) != 0U)
      {
        Env_Sensor_FuncDrv[0][TEMPERATURE_ID] = (ENV_SENSOR_FuncDrv_t *) &STTS22H_TEMP_Driver;
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
