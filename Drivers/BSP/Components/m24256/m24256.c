/**
  ******************************************************************************
  * @file    m24256.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions to interface with the M24256
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
#include "m24256.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup M24256 M24256
  * @{
  */

/** @defgroup M24256_Private_Types M24256 Private Types
  * @{
  */
M24256_EEPROM_Drv_t   M24256_EEPROM_Driver =
{
  M24256_Init,
  M24256_DeInit,
  M24256_Write,
  M24256_Read,
  M24256_IsReady
};
/**
  * @}
  */
/** @defgroup M24256_Private_FunctionPrototypes M24256 Private Function Prototypes
  * @{
  */
static int32_t M24256_ReadWrap(const void *handle, uint16_t Addr, uint8_t *pData, uint16_t Length);
static int32_t M24256_WriteWrap(const void *handle, uint16_t Addr, uint8_t *pData, uint16_t Length);
static int32_t M24256_IsReadyWrap(const void *handle, uint32_t Trials);

/**
  * @}
  */

/** @defgroup M24256_Private_Functions M24256 Private Functions
  * @{
  */
/**
  * @brief  Function
  * @param  Component object pointer
  * @retval Component status
  */
int32_t M24256_RegisterBusIO(M24256_Object_t *pObj, M24256_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = M24256_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.Write     = pIO->Write;
    pObj->IO.Read      = pIO->Read;
    pObj->IO.IsReady   = pIO->IsReady;

    pObj->Ctx.Read     = M24256_ReadWrap;
    pObj->Ctx.Write    = M24256_WriteWrap;
    pObj->Ctx.IsReady  = M24256_IsReadyWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = M24256_ERROR;
    }
  }

  return ret;
}


/**
  * @brief  Initializes the M24256 EEPROM component.
  * @param  pObj  M24256 object
  * @retval Component status
  */
int32_t M24256_Init(M24256_Object_t *pObj)
{
  if (pObj->IsInitialized != 1U)
  {
    pObj->IO.Init();
    pObj->IsInitialized = 1U;
  }
  return M24256_OK;
}

/**
  * @brief  DeInitializes the M24256 EEPROM component.
  * @param  pObj  M24256 object
  * @retval Component status
  */
int32_t M24256_DeInit(M24256_Object_t *pObj)
{
  if (pObj->IsInitialized != 0U)
  {
    pObj->IsInitialized = 0U;
  }
  return M24256_OK;
}

/**
  * @brief  DeInitializes the M24256 EEPROM component.
  * @param  pObj  M24256 object
  * @retval Component status
  */
int32_t M24256_Write(const M24256_Object_t *pObj, uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  return pObj->Ctx.Write(pObj->Ctx.handle, Addr, pData, Length);
}

/**
  * @brief  DeInitializes the M24256 EEPROM component.
  * @param  pObj  M24256 object
  * @retval Component status
  */
int32_t M24256_Read(const M24256_Object_t *pObj, uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  return pObj->Ctx.Read(pObj->Ctx.handle, Addr, pData, Length);
}

/**
  * @brief  Check if the M24256 EEPROM component is ready.
  * @param  pObj  M24256 object
  * @param  Trials  The number of trials before returning a timeout error
  * @retval Component status
  */
int32_t M24256_IsReady(const M24256_Object_t *pObj, uint32_t Trials)
{
  return pObj->Ctx.IsReady(pObj->Ctx.handle, Trials);
}
/**
  * @}
  */

/** @addtogroup M24256_Private_FunctionPrototypes
  * @{
  */
/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle Component object handle
  * @param  Addr   The target address to read
  * @param  pData  The target register value to be written
  * @param  Length Buffer size to be written
  * @retval Component status
  */
static int32_t M24256_ReadWrap(const void *handle, uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  /* Derogation MISRAC2012-Rule-11.5 */
  const M24256_Object_t *pObj = (const M24256_Object_t *)handle;

  return pObj->IO.Read(pObj->IO.Address, Addr, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Addr    The target address to write
  * @param  pData   The target register value to be written
  * @param  Length  Buffer size to be written
  * @retval Component status
  */
static int32_t M24256_WriteWrap(const void *handle, uint16_t Addr, uint8_t *pData, uint16_t Length)
{
  /* Derogation MISRAC2012-Rule-11.5 */
  const M24256_Object_t *pObj = (const M24256_Object_t *)handle;

  return pObj->IO.Write(pObj->IO.Address, Addr, pData, Length);
}

/**
  * @brief  Wrap component IsReady to Bus IsReady function
  * @param  handle  Component object handle
  * @param  Trials  The number of trials before returning a timeout error
  * @retval Component status
  */
static int32_t M24256_IsReadyWrap(const void *handle, uint32_t Trials)
{
  /* Derogation MISRAC2012-Rule-11.5 */
  const M24256_Object_t *pObj = (const M24256_Object_t *)handle;

  return pObj->IO.IsReady(pObj->IO.Address, Trials);
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
