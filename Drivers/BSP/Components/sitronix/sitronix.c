/**
  ******************************************************************************
  * @file    sitronix.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the SITRONIX
  *          device.
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
#include "sitronix.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup SITRONIX SITRONIX
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/** @defgroup SITRONIX_Exported_Variables SITRONIX Exported Variables
  * @{
  */

/* Touch screen driver structure initialization */
SITRONIX_TS_Drv_t SITRONIX_TS_Driver =
{
  SITRONIX_Init,
  SITRONIX_DeInit,
  SITRONIX_GestureConfig,
  SITRONIX_ReadID,
  SITRONIX_GetState,
  SITRONIX_GetMultiTouchState,
  SITRONIX_GetGesture,
  SITRONIX_GetCapabilities,
  SITRONIX_EnableIT,
  SITRONIX_DisableIT,
  SITRONIX_ClearIT,
  SITRONIX_ITStatus
};
/**
  * @}
  */

/** @defgroup SITRONIX_Private_Function_Prototypes SITRONIX Private Function Prototypes
  * @{
  */
#if (SITRONIX_AUTO_CALIBRATION_ENABLED == 1)
static int32_t SITRONIX_TS_Calibration(SITRONIX_Object_t *pObj);
static int32_t SITRONIX_Delay(SITRONIX_Object_t *pObj, uint32_t Delay);
#endif /* SITRONIX_AUTO_CALIBRATION_ENABLED == 1 */
static int32_t SITRONIX_DetectTouch(SITRONIX_Object_t *pObj);
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint16_t Length);
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint16_t Length);
static int32_t ReadDataWrap(void *handle, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup SITRONIX_Exported_Functions SITRONIX Exported Functions
  * @{
  */

/**
  * @brief  Register IO bus to component object
  * @param  Component object pointer
  * @retval error status
  */
int32_t SITRONIX_RegisterBusIO(SITRONIX_Object_t *pObj, SITRONIX_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = SITRONIX_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.ReadData  = pIO->ReadData;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = ReadRegWrap;
    pObj->Ctx.WriteReg = WriteRegWrap;
    pObj->Ctx.ReadData = ReadDataWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = SITRONIX_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Get SITRONIX sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to SITRONIX sensor capabilities
  * @retval Component status
  */
int32_t SITRONIX_GetCapabilities(SITRONIX_Object_t *pObj, SITRONIX_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Store component's capabilities */
  Capabilities->MultiTouch = 1;
  Capabilities->Gesture    = 0;  /* Gesture feature is currently not activated on FW chipset */
  Capabilities->MaxTouch   = SITRONIX_MAX_DETECTABLE_TOUCH;
  Capabilities->MaxXl      = SITRONIX_MAX_X_LENGTH;
  Capabilities->MaxYl      = SITRONIX_MAX_Y_LENGTH;

  return SITRONIX_OK;
}

/**
  * @brief  Initialize the SITRONIX communication bus
  *         from MCU to SITRONIX : ie I2C channel initialization (if required).
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_Init(SITRONIX_Object_t *pObj)
{

  int32_t ret = SITRONIX_OK;
  uint8_t data[28U];

  if (pObj->IsInitialized == 0U)
  {
    /* Initialize IO BUS layer */
    pObj->IO.Init();

    if (sitronix_read_data(&pObj->Ctx, data, (uint16_t)sizeof(data)) != SITRONIX_OK)
    {
      ret = SITRONIX_ERROR;
    }

    pObj->IsInitialized = 1;
  }

  if (ret != SITRONIX_OK)
  {
    ret = SITRONIX_ERROR;
  }

  return ret;
}

/**
  * @brief  De-Initialize the SITRONIX communication bus
  *         from MCU to SITRONIX : ie I2C channel initialization (if required).
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_DeInit(SITRONIX_Object_t *pObj)
{
  if (pObj->IsInitialized == 1U)
  {
    pObj->IsInitialized = 0;
  }

  return SITRONIX_OK;
}

/**
  * @brief  Configure the SITRONIX gesture
  *         from MCU to SITRONIX : ie I2C channel initialization (if required).
  * @param  pObj  Component object pointer
  * @param  GestureInit Gesture init structure
  * @retval Component status
  */
int32_t SITRONIX_GestureConfig(SITRONIX_Object_t *pObj, SITRONIX_Gesture_Init_t *GestureInit)
{
  return SITRONIX_ERROR;
}

/**
  * @brief  Read the SITRONIX device ID, pre initialize I2C in case of need to be
  *         able to read the SITRONIX device ID, and verify this is a SITRONIX.
  * @param  pObj Component object pointer
  * @param  Id Pointer to component's ID
  * @retval Component status
  */
int32_t SITRONIX_ReadID(SITRONIX_Object_t *pObj, uint32_t *Id)
{
  int32_t ret = SITRONIX_OK;
  uint8_t data[28];
  uint8_t trial = 0;

  for (trial = 0; trial < 10; trial++)
  {
    if (sitronix_read_data(&pObj->Ctx, data, 28) != SITRONIX_OK)
    {
      ret = SITRONIX_ERROR;
    }
    else
    {
      if ((uint32_t)data[0] == SITRONIX_ID)
      {
        *Id = (uint32_t)data[0];
        return ret;
      }
    }
  }
  return ret;
}

/**
  * @brief  Get the touch screen X and Y positions values
  * @param  pObj Component object pointer
  * @param  State Single Touch structure pointer
  * @retval Component status.
  */
int32_t SITRONIX_GetState(SITRONIX_Object_t *pObj, SITRONIX_State_t *State)
{
  int32_t ret = SITRONIX_OK;
  uint8_t  data[64];

  State->TouchDetected = (uint32_t)SITRONIX_DetectTouch(pObj);
  if (sitronix_read_data(&pObj->Ctx, data, (uint16_t)sizeof(data)) != SITRONIX_OK)
  {
    ret = SITRONIX_ERROR;
  }
  else
  {
    State->TouchX = (((uint32_t)data[2] & SITRONIX_TOUCH_POS_LSB_MASK) << 4);

    /* Send back first ready X position to caller */
    State->TouchX = ((((uint32_t)data[2] & SITRONIX_TOUCH_POS_LSB_MASK) << 4) | ((uint32_t)data[3]));
    /* Send back first ready Y position to caller */
    State->TouchY = (((uint32_t)data[2] & SITRONIX_TOUCH_POS_MSB_MASK) << 8) | ((uint32_t)data[4]);
  }

  return ret;
}

/**
  * @brief  Get the touch screen Xn and Yn positions values in multi-touch mode
  * @param  pObj Component object pointer
  * @param  State Multi Touch structure pointer
  * @retval Component status.
  */
int32_t SITRONIX_GetMultiTouchState(SITRONIX_Object_t *pObj, SITRONIX_MultiTouch_State_t *State)
{
  int32_t ret = SITRONIX_OK;
  uint8_t  data[28];

  State->TouchDetected = (uint32_t)SITRONIX_DetectTouch(pObj);

  if (sitronix_read_reg(&pObj->Ctx, SITRONIX_P1_XH_REG, data, (uint16_t)sizeof(data)) != SITRONIX_OK)
  {
    ret = SITRONIX_ERROR;
  }
  else
  {
    /* To be implemented */
  }

  return ret;
}

/**
  * @brief  Get Gesture ID
  * @param  pObj Component object pointer
  * @param  GestureId gesture ID
  * @retval Component status
  */
int32_t SITRONIX_GetGesture(SITRONIX_Object_t *pObj, uint8_t *GestureId)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Always return SITRONIX_OK as feature not supported by SITRONIX */
  return SITRONIX_ERROR;
}

/**
  * @brief  Configure the SITRONIX device to generate IT on given INT pin
  *         connected to MCU as EXTI.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_EnableIT(SITRONIX_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Always return SITRONIX_OK as feature not supported by SITRONIX */
  return SITRONIX_ERROR;
}

/**
  * @brief  Configure the SITRONIX device to stop generating IT on the given INT pin
  *         connected to MCU as EXTI.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_DisableIT(SITRONIX_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Always return SITRONIX_OK as feature not supported by SITRONIX */
  return SITRONIX_ERROR;
}

/**
  * @brief  Get IT status from SITRONIX interrupt status registers
  *         Should be called Following an EXTI coming to the MCU to know the detailed
  *         reason of the interrupt.
  *         @note : This feature is not supported by SITRONIX.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_ITStatus(SITRONIX_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Always return SITRONIX_OK as feature not supported by SITRONIX */
  return SITRONIX_ERROR;
}

/**
  * @brief  Clear IT status in SITRONIX interrupt status clear registers
  *         Should be called Following an EXTI coming to the MCU.
  *         @note : This feature is not supported by SITRONIX.
  * @param  pObj Component object pointer
  * @retval Component status
  */
int32_t SITRONIX_ClearIT(SITRONIX_Object_t *pObj)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  /* Always return SITRONIX_OK as feature not supported by SITRONIX */
  return SITRONIX_ERROR;
}

/**
  * @}
  */

/** @defgroup SITRONIX_Private_Functions SITRONIX Private Functions
  * @{
  */


/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  pObj Component object pointer
  * @retval Number of active touches detected (can be between 0 and10) or SITRONIX_ERROR
  *         in case of error
  */
static int32_t SITRONIX_DetectTouch(SITRONIX_Object_t *pObj)
{
  int32_t ret;
  uint8_t nb_touch = 0;
  static uint8_t first_event = 0;
  uint8_t data[28];

  if (sitronix_read_data(&pObj->Ctx, (uint8_t *)&data, 28) != SITRONIX_OK)
  {
    ret = SITRONIX_ERROR;
  }
  else
  {
    if (first_event == 0)
    {
      if ((data[0] == 0x09))
      {
        nb_touch = 1;
        first_event = 1;
      }
      else
      {
        nb_touch = 0;
      }
    }
    else
    {
      if (data[8] == 0x60)
      {
        nb_touch = 0;
      }
      else
      {
        nb_touch = 1;
      }
    }
    ret = (int32_t)nb_touch;
  }

  return ret;
}

/**
  * @brief  Wrap IO bus read function to component register red function
  * @param  handle Component object handle
  * @param  Reg The target register address to read
  * @param  pData The target register value to be read
  * @param  Length buffer size to be read
  * @retval Component status.
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  SITRONIX_Object_t *pObj = (SITRONIX_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap IO bus write function to component register write function
  * @param  handle Component object handle
  * @param  Reg The target register address to write
  * @param  pData The target register value to be written
  * @param  Length buffer size to be written
  * @retval Component status.
  */
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  SITRONIX_Object_t *pObj = (SITRONIX_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap IO bus read function to component register red function
  * @param  handle Component object handle
  * @param  pData The target register value to be read
  * @param  Length buffer size to be read
  * @retval Component status.
  */
static int32_t ReadDataWrap(void *handle, uint8_t *pData, uint16_t Length)
{
  SITRONIX_Object_t *pObj = (SITRONIX_Object_t *)handle;

  return pObj->IO.ReadData(pObj->IO.Address, pData, Length);
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
