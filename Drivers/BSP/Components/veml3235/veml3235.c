/**
  ******************************************************************************
  * @file    veml3235.c
  * @author  MCD Application Team
  * @brief   VEML3235 driver file
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

/* Includes ------------------------------------------------------------------*/
#include "veml3235.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VEML3235
  * @brief     This file provides a set of functions needed to drive the
  *            VEML3235 light sensor.
  * @{
  */

/** @defgroup VEML3235_Private_Types_Definitions Private Types Definitions
  * @{
  */
/**
  * @}
  */
/** @defgroup VEML3235_Private_Variables
  * @{
  */
VEML3235_Drv_t VEML3235_Driver =
{
  VEML3235_Init,
  VEML3235_DeInit,
  VEML3235_ReadID,
  VEML3235_GetCapabilities,
  VEML3235_SetExposureTime,
  VEML3235_GetExposureTime,
  VEML3235_SetGain,
  VEML3235_GetGain,
  VEML3235_SetInterMeasurementTime,
  VEML3235_GetInterMeasurementTime,
  VEML3235_Start,
  VEML3235_Stop,
  VEML3235_StartFlicker,
  VEML3235_StopFlicker,
  VEML3235_GetValues,
  VEML3235_SetControlMode
};

/**
  * @}
  */

/** @defgroup VEML3235_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t VEML3235_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);
static int32_t VEML3235_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);
static int32_t VEML3235_Pwr_On(VEML3235_Object_t *pObj);
static int32_t VEML3235_Shutdown(VEML3235_Object_t *pObj);
static void VEML3235_Delay(VEML3235_Object_t *pObj, uint32_t Delay);

/**
  * @}
  */

/**
  * @brief Initializes the VEML3235 context object.
  * @param pObj    VEML3235 context object.
  * @param pIO     BSP IO struct.
  * @retval VEML3235 status
  */
int32_t VEML3235_RegisterBusIO(VEML3235_Object_t *pObj, VEML3235_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    pObj->IO.Init          = pIO->Init;
    pObj->IO.DeInit        = pIO->DeInit;
    pObj->IO.ReadAddress   = pIO->ReadAddress;
    pObj->IO.WriteAddress  = pIO->WriteAddress;
    pObj->IO.IsReady       = pIO->IsReady;
    pObj->IO.WriteReg      = pIO->WriteReg;
    pObj->IO.ReadReg       = pIO->ReadReg;
    pObj->IO.GetTick       = pIO->GetTick;

    pObj->Ctx.ReadReg  = VEML3235_ReadRegWrap;
    pObj->Ctx.WriteReg = VEML3235_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = VEML3235_ERROR;
    }
  }

  return ret;
}

/**
  * @brief Initializes the VEML3235 light sensor.
  * @param pObj    VEML3235 context object.
  * @retval VEML3235 status
  */
int32_t VEML3235_Init(VEML3235_Object_t *pObj)
{
  int32_t ret = VEML3235_OK;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  if (pObj->IsInitialized == 0U)
  {
    if (VEML3235_Pwr_On(pObj) < 0)
    {
      ret = VEML3235_ERROR;
    }
    else
    {
      pObj->IsInitialized = 1;
      pObj->IsContinuous = 1;
      pObj->IsStarted = 1;
    }
  }
  return ret;
}

/**
  * @brief Deinitializes the VEML3235 light sensor.
  * @param pObj    VEML3235 context object.
  * @retval VEML3235 status
  */
int32_t VEML3235_DeInit(VEML3235_Object_t *pObj)
{
  int32_t ret = VEML3235_OK;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  if (pObj->IsInitialized == 1U)
  {
    if (VEML3235_Shutdown(pObj) < 0)
    {
      ret = VEML3235_ERROR;
    }
    else
    {
      pObj->IsInitialized = 0;
    }
  }
  return ret;
}

/**
  * @brief Read the VEML3235 device ID.
  * @param pObj    VEML3235 context object.
  * @param pId    Pointer to the device ID.
  * @retval VEML3235 status
  */
int32_t VEML3235_ReadID(VEML3235_Object_t *pObj, uint32_t *pId)
{
  int32_t ret;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (pObj->IO.IsReady(VEML3235_I2C_READ_ADD, 20) != 0U)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    /* softowre ID defined */
    *pId = VEML3235_ID;
    ret = VEML3235_OK;
  }
  return ret;
}
/**
  * @brief Get the VEML3235 capabilities.
  * @param pObj    VEML3235 context object.
  * @param pCapabilities    Pointer to the VEML3235 capabilities.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetCapabilities(VEML3235_Object_t *pObj, VEML3235_Capabilities_t *pCapabilities)
{
  int32_t ret;

  if ((pObj != NULL) && (pCapabilities != NULL))
  {
    pCapabilities->NumberOfChannels = VEML3235_MAX_CHANNELS;
    pCapabilities->FlickerDetection = 0U;
    pCapabilities->Autogain = 0U;

    ret = VEML3235_OK;
  }
  else
  {
    ret = VEML3235_INVALID_PARAM;
  }

  return ret;
}

/**
  * @brief Set the exposure time.
  * @param pObj    VEML3235 context object.
  * @param ExposureTime    New exposure time to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetExposureTime(VEML3235_Object_t *pObj, uint32_t ExposureTime)
{
  int32_t ret;
  uint16_t config = 0;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    config &= ~VEML3235_CONF_IT_MASK;
    config |= (uint16_t)ExposureTime;
    ret = VEML3235_OK;
    if (veml3235_write_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
    {
      ret = VEML3235_ERROR;
    }
  }
  return ret;
}
/**
  * @brief Get the exposure time.
  * @param pObj    VEML3235 context object.
  * @param pExposureTime    Pointer to the current exposure time value.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetExposureTime(VEML3235_Object_t *pObj, uint32_t *pExposureTime)
{
  int32_t ret;
  uint16_t tmp = 0;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &tmp, 2) != 0)
  {
    ret = VEML3235_ERROR ;
  }
  else
  {
    *pExposureTime = (tmp & VEML3235_CONF_IT_MASK);
    ret = VEML3235_OK;
  }
  return ret;
}

/**
  * @brief Set the current gain of a channel.
  * @param pObj     VEML3235 context object.
  * @param Channel  Device channel.
  * @param pGain    Pointer to the current gain value.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetGain(VEML3235_Object_t *pObj, uint8_t Channel, uint32_t Gain)
{
  int32_t ret;
  uint16_t config = 0;
  if ((pObj == NULL) || (Channel >= VEML3235_MAX_CHANNELS))
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    config &= (uint16_t) ~VEML3235_CONF_GAIN_4;
    config |= (uint16_t)Gain;
    ret = VEML3235_OK;
    if (veml3235_write_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
    {
      ret = VEML3235_ERROR ;
    }
  }
  return ret;
}

/**
  * @brief Get the current gain of a channel.
  * @param pObj    VEML3235 context object.
  * @param Channel    Device channel.
  * @param pGain    Pointer to the current gain value.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetGain(VEML3235_Object_t *pObj, uint8_t Channel, uint32_t *pGain)
{
  int32_t ret;
  uint16_t tmp = 0x00U;

  if ((pObj == NULL) || (Channel >= VEML3235_MAX_CHANNELS))
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &tmp, 2) != 0)
  {
    ret = VEML3235_ERROR ;
  }
  else
  {
    *pGain = ((uint32_t)tmp & VEML3235_GAIN_MASK);
    ret = VEML3235_OK;
  }
  return ret;
}

/**
  * @brief Set the inter-measurement time.
  * @param pObj                    VEML3235 context object.
  * @param InterMeasurementTime    Inter-measurement to be applied expressed in microseconds.
  * @note This should be configured only when using the device in continuous mode.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
 */
int32_t VEML3235_SetInterMeasurementTime(VEML3235_Object_t *pObj, uint32_t InterMeasurementTime)
{
  int32_t ret;
  uint16_t als_it;
  uint16_t config;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else
  {
    als_it = (uint16_t)(InterMeasurementTime & 0x03U);

    /* set the integration Time */
    if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
    {
      ret = VEML3235_ERROR;
    }
    else
    {
      config &= ~VEML3235_CONF_IT_MASK;
      config |= (uint16_t)als_it;
      if (veml3235_write_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
      {
        ret = VEML3235_ERROR;
      }
      else
      {
        ret = VEML3235_OK;
      }
    }
  }
  return ret;
}
/**
  * @brief Get the inter-measurement time.
  * @param pObj                     VEML3235 context object.
  * @param pInterMeasurementTime    Pointer to the current inter-measurement time expressed in microseconds.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetInterMeasurementTime(VEML3235_Object_t *pObj, uint32_t *pInterMeasurementTime)
{
  int32_t ret;
  uint32_t tmp = 0;
  uint32_t als_it = 0;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, (uint16_t *)tmp, 2) != 0)
  {
    ret = VEML3235_ERROR ;
  }
  else
  {
    als_it = (tmp & VEML3235_CONF_IT_MASK);
    ret = VEML3235_OK ;
  }

  *pInterMeasurementTime = als_it;
  return ret;
}
/**
  * @brief Start the light measurement on all channels.
  * @param pObj    VEML3235 context object.
  * @param Mode    Measurement mode (continuous)
  * @retval VEML3235 status
  */
int32_t VEML3235_Start(VEML3235_Object_t *pObj, uint32_t Mode)
{
  int32_t ret;
  if ((pObj == NULL) || (Mode != VEML3235_MODE_CONTINUOUS))
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else
  {

    ret = VEML3235_Pwr_On(pObj);
    if (ret == VEML3235_OK)
    {
      pObj->IsStarted = 1;
    }

  }
  return ret;
}
/**
  * @brief Stop the measurement on all channels.
  * @param pObj    VEML3235 context object.
  * @retval VEML3235 status
  */
int32_t VEML3235_Stop(VEML3235_Object_t *pObj)
{
  int32_t ret;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else
  {
    ret = VEML3235_Shutdown(pObj);
    if (ret == VEML3235_OK)
    {
      pObj->IsStarted = 0;
    }
  }
  return ret;
}

/**
  * @brief Stop flicker capture.
  * @param pObj     VEML3235 context object.
  * @retval BSP status
  */
int32_t VEML3235_StartFlicker(VEML3235_Object_t *pObj, uint8_t Channel, uint8_t OutputMode)
{
  (void)(pObj);
  (void)(Channel);
  (void)(OutputMode);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Stop flicker capture.
  * @param pObj     VEML3235 context object.
  * @retval BSP status
  */
int32_t VEML3235_StopFlicker(VEML3235_Object_t *pObj)
{
  (void)(pObj);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief Returns the measurement values for all the channels.
  * @param pObj       VEML3235 context object.
  * @param pValues    Pointer to an array which will be filled with the values of each channel.
  * @note The array size must match the number of channels of the device (4 bytes for 2 channels).
  * @retval VEML3235 status
  */
int32_t VEML3235_GetValues(VEML3235_Object_t *pObj, uint32_t *pValues)
{
  int32_t ret = VEML3235_OK;

  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  /* Derogation MISRAC2012-Rule-11.3 */
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS, (uint16_t *)&pValues[0], 2) != 0)
  {
    ret = VEML3235_ERROR ;
  }
  /* Derogation MISRAC2012-Rule-11.3 */
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_WHITE, (uint16_t *)&pValues[1], 2) != 0)
  {
    ret = VEML3235_ERROR ;
  }
  else
  {
    /* nothing to do */
  }
  return ret;
}

/**
  * @brief Enable and disable control features.
  * @param pObj    VEML3235 context object.
  * @param ControlMode    Feature to be be enabled or disabled.
  * @param Value    Value to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetControlMode(VEML3235_Object_t *pObj, uint32_t ControlMode, uint32_t Value)
{
  (void)(pObj);
  (void)(ControlMode);
  (void)(Value);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief Set the persistence protect number setting.
  * @param pObj           VEML3235 context object.
  * @param Persistence    perssistance value to be applied .
  * @retval VEML3235 status
  */
int32_t VEML3235_SetPersistence(VEML3235_Object_t *pObj, uint32_t Persistence)
{
  (void)(pObj);
  (void)(Persistence);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief Get the persistence protect number setting.
  * @param pObj           VEML3235 context object.
  * @param pPersistence   Pointer to the current persistence value.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetPersistence(VEML3235_Object_t *pObj, uint32_t *pPersistence)
{
  (void)(pObj);
  (void)(pPersistence);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/** @brief Set Power Saving Mode.
  * @param pObj         VEML3235 context object.
  * @param PowerMode    New PowerMode Value to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetPowerSavingMode(VEML3235_Object_t *pObj, uint32_t PowerMode)
{
  (void)(pObj);
  (void)(PowerMode);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/** @brief Get Power Saving Mode.
  * @param pObj          VEML3235 context object.
  * @param pPowerMode    Pointer to the current PowerMode value.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetPowerSavingMode(VEML3235_Object_t *pObj, uint32_t *pPowerMode)
{
  (void)(pObj);
  (void)(pPowerMode);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Set the High Threshold.
  * @param pObj         VEML3235 context object.
  * @param Threshold    New Threshold Value to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetHighThreshold(VEML3235_Object_t *pObj, uint16_t Threshold)
{
  (void)(pObj);
  (void)(Threshold);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Get the High Threshold.
  * @param pObj         VEML3235 context object.
  * @param pThreshold   New Threshold Value  time to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetHighThreshold(VEML3235_Object_t *pObj, uint32_t *pThreshold)
{
  (void)(pObj);
  (void)(pThreshold);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Set the Low Threshold.
  * @param pObj         VEML3235 context object.
  * @param pThreshold   New Threshold Value to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_SetLowThreshold(VEML3235_Object_t *pObj, uint16_t pThreshold)
{
  (void)(pObj);
  (void)(pThreshold);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief Get the Low Threshold.
  * @param pObj         VEML3235 context object.
  * @param Threshold    New Threshold Value  time to be applied.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetLowThreshold(VEML3235_Object_t *pObj, uint32_t *Threshold)
{
  (void)(pObj);
  (void)(Threshold);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief Disable the VEML3235  Interrupt.
  * @param pObj    VEML3235 context object.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_Enable_IT(VEML3235_Object_t *pObj)
{
  (void)(pObj);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Disable the VEML3235  Interrupt.
  * @param pObj    VEML3235 context object.
  * @warning This function must not be called when a capture is ongoing.
  * @retval VEML3235 status
  */
int32_t VEML3235_Disable_IT(VEML3235_Object_t *pObj)
{
  (void)(pObj);

  /* Feature not supported */
  return VEML3235_ERROR;
}
/**
  * @brief Get the Interrupt status (High Threshold or Low Threshold Interrupt).
  * @param pObj      VEML3235 context object.
  * @param pStatus   Pointer to the interrupt status.
  * @retval VEML3235 status
  */
int32_t VEML3235_GetIntStatus(VEML3235_Object_t *pObj, uint32_t *pStatus)
{
  (void)(pObj);
  (void)(pStatus);

  /* Feature not supported */
  return VEML3235_ERROR;
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t VEML3235_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  VEML3235_Object_t *pObj = (VEML3235_Object_t *)handle; /* Derogation MISRAC2012-Rule-11.5 */

  return pObj->IO.ReadReg(pObj->IO.ReadAddress, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t VEML3235_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length)
{
  VEML3235_Object_t *pObj = (VEML3235_Object_t *)handle; /* Derogation MISRAC2012-Rule-11.5 */

  return pObj->IO.WriteReg(pObj->IO.WriteAddress, Reg, Data, Length);
}

/**
  * @brief Shutdown the VEML3235.
  * @param pObj  VEML3235 context object.
  * @retval      VEML3235 status
  */
static int32_t VEML3235_Shutdown(VEML3235_Object_t *pObj)
{
  int32_t ret;
  uint16_t config = 0;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    config |= VEML3235_CONF_SHUTDOWN0;
    ret = VEML3235_OK ;
    if (veml3235_write_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
    {
      ret = VEML3235_ERROR;
    }
  }
  return ret;
}

/**
  * @brief Set Power ON the VEML3235.
  * @param pObj  VEML3235 context object.
  * @retval      VEML3235 status
  */
static int32_t VEML3235_Pwr_On(VEML3235_Object_t *pObj)
{
  int32_t ret;
  uint16_t config = 0;
  if (pObj == NULL)
  {
    ret = VEML3235_INVALID_PARAM;
  }
  else if (veml3235_read_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
  {
    ret = VEML3235_ERROR;
  }
  else
  {
    config &= ~VEML3235_CONF_SHUTDOWN0;
    ret = VEML3235_OK ;
    if (veml3235_write_reg(&pObj->Ctx, VEML3235_REG_ALS_CONF, &config, 2) != 0)
    {
      ret = VEML3235_ERROR;
    }

    VEML3235_Delay(pObj, 4);
  }
  return ret;
}

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param pObj  pointer to component object
  * @param Delay specifies the delay time length, in milliseconds
  * @retval Component status
  */
static void VEML3235_Delay(VEML3235_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while ((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
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
