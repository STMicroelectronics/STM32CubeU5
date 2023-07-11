/**
  ******************************************************************************
  * @file    vl53l5cx.c
  * @author  IMG SW Application Team
  * @brief   This file provides the VL53L5CX ranging sensor component driver
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

/* Includes ------------------------------------------------------------------*/
#include "vl53l5cx.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L5CX
  * @brief     This file provides a set of functions needed to drive the
  *            VL53L5CX ranging sensor.
  * @{
  */

/** @defgroup VL53L5CX_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/* 0x1388 corresponds to 5000 decimal. This will do a timeout of 5 seconds */
#define V53L5CX_POLL_TIMEOUT  (0x1388U)
#define UNUSED(x) (void)(x)

/**
  * @}
  */

/** @defgroup VL53L5CX_Private_Variables Private Variables
  * @{
  */

VL53L5CX_RANGING_SENSOR_Drv_t VL53L5CX_RANGING_SENSOR_Driver =
{
  VL53L5CX_Init,
  VL53L5CX_DeInit,
  VL53L5CX_ReadID,
  VL53L5CX_GetCapabilities,
  VL53L5CX_ConfigProfile,
  VL53L5CX_ConfigROI,
  VL53L5CX_ConfigIT,
  VL53L5CX_GetDistance,
  VL53L5CX_Start,
  VL53L5CX_Stop,
  VL53L5CX_SetAddress,
  VL53L5CX_GetAddress,
  VL53L5CX_SetPowerMode,
  VL53L5CX_GetPowerMode
};

/**
  * @}
  */


/** @defgroup VL53L5CX_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t vl53l5cx_poll_for_measurement(VL53L5CX_Object_t *pObj, uint32_t Timeout);
static int32_t vl53l5cx_get_result(VL53L5CX_Object_t *pObj, VL53L5CX_Result_t *pResult);
static uint8_t vl53l5cx_map_target_status(uint8_t status);
/**
  * @}
  */

/**
  * @brief Initializes the vl53l5cx context object.
  * @param pObj    vl53l5cx context object.
  * @param pIO     BSP IO struct.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_RegisterBusIO(VL53L5CX_Object_t *pObj, VL53L5CX_IO_t *pIO)
{
  int32_t ret;

  if ((pObj == NULL) || (pIO == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    /* fill vl53l5cx platform structure */
    pObj->Dev.platform.address = pIO->Address;
    pObj->Dev.platform.Read = pIO->ReadReg;
    pObj->Dev.platform.Write = pIO->WriteReg;
    pObj->Dev.platform.GetTick = pIO->GetTick;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = VL53L5CX_ERROR;
    }
  }

  return ret;
}

/**
  * @brief Initializes the vl53l5cx.
  * @param pObj    vl53l5cx context object.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_Init(VL53L5CX_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (pObj->IsInitialized != 0U)
  {
    ret =  VL53L5CX_ERROR;
  }
  else if (vl53l5cx_init(&pObj->Dev) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    pObj->IsRanging = 0U;
    pObj->IsBlocking = 0U;
    pObj->IsContinuous = 0U;
    pObj->IsAmbientEnabled = 0U;
    pObj->IsSignalEnabled = 0U;
    pObj->IsInitialized = 1U;
    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Deinitializes the vl53l5cx.
  * @param pObj    vl53l5cx context object.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_DeInit(VL53L5CX_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (pObj->IsInitialized == 1U)
  {
    /* De-initialize the vl53l5cx interface */
    if (pObj->IO.DeInit() != 0)
    {
      ret = VL53L5CX_ERROR;
    }
    else
    {
      ret = VL53L5CX_OK;
      pObj->IsInitialized = 0;
    }
  }
  else
  {
    /* if device not initialized return error */
    ret = VL53L5CX_ERROR;
  }

  return ret;
}

/**
  * @brief Read the vl53l5cx device ID.
  * @param pObj    vl53l5cx context object.
  * @param pId    Pointer to the device ID.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_ReadID(VL53L5CX_Object_t *pObj, uint32_t *pId)
{
  int32_t ret;
  uint8_t device_id = 0;
  uint8_t revision_id = 0;
  uint8_t status = VL53L5CX_STATUS_OK;

  if ((pObj == NULL) || (pId == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else
  {
    status |= WrByte(&pObj->Dev.platform, 0x7fff, 0x00);
    status |= RdByte(&pObj->Dev.platform, 0, &device_id);
    status |= RdByte(&pObj->Dev.platform, 1, &revision_id);
    status |= WrByte(&pObj->Dev.platform, 0x7fff, 0x02);

    if (status == 0U)
    {
      *pId = ((uint32_t)device_id << 8) + revision_id;
      ret = VL53L5CX_OK;
    }
    else
    {
      *pId = 0;
      ret = VL53L5CX_ERROR;
    }
  }

  return ret;
}

/**
  * @brief Get the vl53l5cx capabilities.
  * @param pObj    vl53l5cx context object.
  * @param pCap    Pointer to the vl53l5cx capabilities.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_GetCapabilities(VL53L5CX_Object_t *pObj, VL53L5CX_Capabilities_t *pCap)
{
  int32_t ret;

  if ((pObj == NULL) || (pCap == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else
  {
    pCap->NumberOfZones = VL53L5CX_RESOLUTION_8X8;
    pCap->MaxNumberOfTargetsPerZone = VL53L5CX_TARGET_PER_ZONE;
    pCap->CustomROI = 0;
    pCap->ThresholdDetection = 1;

    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Set the ranging configuration profile.
  * @param pObj    vl53l5cx context object.
  * @param pConfig    Pointer to the new configuration profile to be applied.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_ConfigProfile(VL53L5CX_Object_t *pObj, VL53L5CX_ProfileConfig_t *pConfig)
{
  int32_t ret = VL53L5CX_OK;

  uint8_t profile;
  uint8_t resolution;
  uint8_t ranging_mode;
  uint8_t ranging_frequency;
  uint32_t integration_time;

  if ((pObj != NULL) && (pConfig != NULL))
  {
    profile = pConfig->RangingProfile;
    integration_time = pConfig->TimingBudget;
    ranging_frequency = (uint8_t)pConfig->Frequency;
  }
  else
  {
    return VL53L5CX_INVALID_PARAM;
  }

  switch (profile)
  {
  case VL53L5CX_PROFILE_4x4_CONTINUOUS:
    resolution = VL53L5CX_RESOLUTION_4X4;
    ranging_mode = VL53L5CX_RANGING_MODE_CONTINUOUS;
    break;
  case VL53L5CX_PROFILE_4x4_AUTONOMOUS:
    resolution = VL53L5CX_RESOLUTION_4X4;
    ranging_mode = VL53L5CX_RANGING_MODE_AUTONOMOUS;
    break;
  case VL53L5CX_PROFILE_8x8_CONTINUOUS:
    resolution = VL53L5CX_RESOLUTION_8X8;
    ranging_mode = VL53L5CX_RANGING_MODE_CONTINUOUS;
    break;
  case VL53L5CX_PROFILE_8x8_AUTONOMOUS:
    resolution = VL53L5CX_RESOLUTION_8X8;
    ranging_mode = VL53L5CX_RANGING_MODE_AUTONOMOUS;
    break;
  default:
    resolution = 0; /* silence MISRA rule 1.3 warning */
    ranging_mode = 0; /* silence MISRA rule 1.3 warning */
    ret = VL53L5CX_INVALID_PARAM;
    break;
  }

  if (ret != VL53L5CX_OK)
  {
    return ret;
  }
  else if (vl53l5cx_set_resolution(&pObj->Dev, resolution) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_set_ranging_mode(&pObj->Dev, ranging_mode) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_set_integration_time_ms(&pObj->Dev, integration_time) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_set_ranging_frequency_hz(&pObj->Dev, ranging_frequency) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    pObj->IsAmbientEnabled = (pConfig->EnableAmbient == 0U) ? 0U : 1U;
    pObj->IsSignalEnabled = (pConfig->EnableSignal == 0U) ? 0U : 1U;

    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Configure the Region of Interest of the vl53l5cx.
  * @param pObj    vl53l5cx context object.
  * @param pROIConfig    Pointer to the ROI configuration struct.
  * @note This device does not support this function.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_ConfigROI(VL53L5CX_Object_t *pObj, VL53L5CX_ROIConfig_t *pROIConfig)
{
  UNUSED(pObj);
  UNUSED(pROIConfig);
  return VL53L5CX_NOT_IMPLEMENTED;
}

/**
  * @brief Configure the IT event generation parameters.
  * @param pObj    vl53l5cx context object.
  * @param pITConfig    Pointer to the IT configuration struct.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_ConfigIT(VL53L5CX_Object_t *pObj, VL53L5CX_ITConfig_t *pITConfig)
{
  int32_t ret;
  uint8_t i;
  uint8_t res; /* current resolution */
  uint8_t status = 0U;
  static VL53L5CX_DetectionThresholds thresholds[VL53L5CX_NB_THRESHOLDS];

  if ((pObj == NULL) || (pITConfig == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (pITConfig->Criteria == VL53L5CX_IT_DEFAULT)
  {
    /* disable thresholds detection */
    status |= vl53l5cx_set_detection_thresholds_enable(&pObj->Dev, 0U);
    ret = (status != 0U) ? VL53L5CX_ERROR : VL53L5CX_OK;
  }
  else
  {
    (void)vl53l5cx_get_resolution(&pObj->Dev, &res);

    /* configure thresholds on each active zone */
    for (i = 0; i < res; i++)
    {
      thresholds[i].zone_num = i;
      thresholds[i].measurement = VL53L5CX_DISTANCE_MM;
      thresholds[i].type = (uint8_t)pITConfig->Criteria;
      thresholds[i].mathematic_operation = VL53L5CX_OPERATION_NONE;
      thresholds[i].param_low_thresh = (int32_t)pITConfig->LowThreshold;
      thresholds[i].param_high_thresh = (int32_t)pITConfig->HighThreshold;
    }

    /* the last threshold must be clearly indicated */
    thresholds[i].zone_num |= VL53L5CX_LAST_THRESHOLD;

    /* send array of thresholds to the sensor */
    status |= vl53l5cx_set_detection_thresholds(&pObj->Dev, thresholds);

    /* enable thresholds detection */
    status |= vl53l5cx_set_detection_thresholds_enable(&pObj->Dev, 1U);

    ret = (status != 0U) ? VL53L5CX_ERROR : VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Get the last distance measurement information.
  * @param pObj    vl53l5cx context object.
  * @param pResult    Pointer to the result struct.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_GetDistance(VL53L5CX_Object_t *pObj, VL53L5CX_Result_t *pResult)
{
  int32_t ret;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    if (pObj->IsBlocking == 1U)
    {
      ret = vl53l5cx_poll_for_measurement(pObj, V53L5CX_POLL_TIMEOUT);
    }
    else
    {
      ret = vl53l5cx_poll_for_measurement(pObj, 0U);
    }
  }

  /* a new measure is available if no error is returned by the poll function */
  if (ret == VL53L5CX_OK)
  {
    ret = vl53l5cx_get_result(pObj, pResult);
  }

  return ret;
}

/**
  * @brief Start ranging.
  * @param pObj    vl53l5cx context object.
  * @param Mode        The desired @ref RANGING_SENSOR_Mode_t
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_Start(VL53L5CX_Object_t *pObj, uint32_t Mode)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_start_ranging(&pObj->Dev) == VL53L5CX_STATUS_OK)
  {
    pObj->IsRanging = 1U;
    ret = VL53L5CX_OK;

    switch (Mode)
    {
      case VL53L5CX_MODE_BLOCKING_CONTINUOUS:
        pObj->IsContinuous = 1U;
        pObj->IsBlocking = 1U;
        break;

      case VL53L5CX_MODE_BLOCKING_ONESHOT:
        pObj->IsContinuous = 0U;
        pObj->IsBlocking = 1U;
        break;

      case VL53L5CX_MODE_ASYNC_CONTINUOUS:
        pObj->IsContinuous = 1U;
        pObj->IsBlocking = 0U;
        break;

      case VL53L5CX_MODE_ASYNC_ONESHOT:
        pObj->IsContinuous = 0U;
        pObj->IsBlocking = 0U;
        break;

      default:
        pObj->IsRanging = 0U;
        ret = VL53L5CX_INVALID_PARAM;
        break;
    }
  }
  else
  {
    ret = VL53L5CX_ERROR;
  }

  return ret;
}

/**
  * @brief Stop ranging.
  * @param pObj    vl53l5cx context object.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_Stop(VL53L5CX_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
    /* ranging not started */
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_stop_ranging(&pObj->Dev) == VL53L5CX_STATUS_OK)
  {
    pObj->IsRanging = 0U;
    ret = VL53L5CX_OK;
  }
  else
  {
    ret = VL53L5CX_ERROR;
  }

  return ret;
}

/**
  * @brief Set The I2C address of the device.
  * @param pObj    vl53l5cx context object.
  * @param Address     New I2C address.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_SetAddress(VL53L5CX_Object_t *pObj, uint32_t Address)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_set_i2c_address(&pObj->Dev, (uint8_t)Address) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    pObj->IO.Address = (uint8_t)(Address & 0xFFU);
    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Get The I2C address of the device.
  * @param pObj    vl53l5cx context object.
  * @param *pAddress     New I2C address.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_GetAddress(VL53L5CX_Object_t *pObj, uint32_t *pAddress)
{
  int32_t ret;

  if ((pObj == NULL) || (pAddress == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else
  {
    *pAddress = pObj->IO.Address;
    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Set the power mode.
  * @param pObj    vl53l5cx context object.
  * @param PowerMode    New power mode to be entered.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_SetPowerMode(VL53L5CX_Object_t *pObj, uint32_t PowerMode)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if ((PowerMode != VL53L5CX_POWER_MODE_SLEEP) &&
           (PowerMode != VL53L5CX_POWER_MODE_WAKEUP))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_set_power_mode(&pObj->Dev, (uint8_t)PowerMode) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Get the power mode.
  * @param pObj    vl53l5cx context object.
  * @param *pPowerMode    pointer to variable to be filled with power mode value.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_GetPowerMode(VL53L5CX_Object_t *pObj, uint32_t *pPowerMode)
{
  int32_t ret;
  uint8_t powermode;

  if ((pObj == NULL) || (pPowerMode == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_get_power_mode(&pObj->Dev, &powermode) != VL53L5CX_STATUS_OK)
  {
    *pPowerMode = 0;
    ret = VL53L5CX_ERROR;
  }
  else
  {
    *pPowerMode = (uint32_t)powermode;
    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
  * @brief Perform a xtalk calibration.
  * @param pObj    vl53l5cx context object.
  * @param Reflectance    Target reflectance in percent (range: 1 - 99 %).
  * @param Distance    Target distance in mm (range 600 - 3000 mm).
  * @note The recommended target reflectance value for Xtalk calibration is 3 %.
  * @retval VL53L5CX status
  */
int32_t VL53L5CX_XTalkCalibration(VL53L5CX_Object_t *pObj, uint16_t Reflectance, uint16_t Distance)
{
  int32_t ret;
  uint8_t status;

  /* Number of data samples used for calibration.
   * A higher number of samples means a higher accuracy,
   * but it increases the calibration time (range 1 - 16). */
  uint8_t nb_samples = 2;

  status = vl53l5cx_calibrate_xtalk(
      &pObj->Dev,
      Reflectance,
      nb_samples,
      Distance);

  ret = (status == VL53L5CX_STATUS_OK) ? VL53L5CX_OK : VL53L5CX_ERROR;

  return ret;
}

/** @defgroup VL53L5CX_Private_Functions Private Functions
  * @{
  */
static int32_t vl53l5cx_poll_for_measurement(VL53L5CX_Object_t *pObj, uint32_t Timeout)
{
  int32_t ret;
  uint32_t TickStart;
  uint8_t NewDataReady = 0;

  ret =  VL53L5CX_TIMEOUT;
  TickStart = pObj->IO.GetTick();

  do
  {
    (void)vl53l5cx_check_data_ready(&pObj->Dev, &NewDataReady);

    if (NewDataReady == 1U)
    {
      ret = VL53L5CX_OK;
      break;
    }
  } while ((pObj->IO.GetTick() - TickStart) < Timeout);

  return ret;
}

static int32_t vl53l5cx_get_result(VL53L5CX_Object_t *pObj, VL53L5CX_Result_t *pResult)
{
  int32_t ret;
  uint8_t i, j;
  uint8_t resolution;
  uint8_t target_status;
  static VL53L5CX_ResultsData data;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_get_resolution(&pObj->Dev, &resolution) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_get_ranging_data(&pObj->Dev, &data) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    pResult->NumberOfZones = resolution;

    for (i = 0; i < resolution; i++)
    {
      pResult->ZoneResult[i].NumberOfTargets = data.nb_target_detected[i];

      for (j = 0; j < data.nb_target_detected[i]; j++)
      {
        pResult->ZoneResult[i].Distance[j] = (uint32_t)data.distance_mm[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];

        /* return Ambient value if ambient rate output is enabled */
        if (pObj->IsAmbientEnabled == 1U)
        {
          /* apply ambient value to all targets in a given zone */
          pResult->ZoneResult[i].Ambient[j] = (float_t)data.ambient_per_spad[i];
        }
        else
        {
          pResult->ZoneResult[i].Ambient[j] = 0.0f;
        }

        /* return Signal value if signal rate output is enabled */
        if (pObj->IsSignalEnabled == 1U)
        {
          pResult->ZoneResult[i].Signal[j] =
            (float_t)data.signal_per_spad[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
        }
        else
        {
          pResult->ZoneResult[i].Signal[j] = 0.0f;
        }

        target_status = data.target_status[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
        pResult->ZoneResult[i].Status[j] = vl53l5cx_map_target_status(target_status);
      }
    }

    ret = VL53L5CX_OK;
  }

  return ret;
}

static uint8_t vl53l5cx_map_target_status(uint8_t status)
{
  uint8_t ret;

  if ((status == 5U) || (status == 9U))
  {
    ret = 0U; /* ranging is OK */
  }
  else if (status == 0U)
  {
    ret = 255U; /* no update */
  }
  else
  {
    ret = status; /* return device status otherwise */
  }

  return ret;
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
