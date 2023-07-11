/**
  ******************************************************************************
  * @file    m24256.h
  * @author  MCD Application Team
  * @brief   This file is the header of m24256.c
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
#ifndef M24256_H
#define M24256_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @addtogroup M24256
  * @{
  */

/** @defgroup M24256_Exported_Types   M24256 Exported Types
  * @{
  */
typedef int32_t (*M24256_Init_Func)(void);
typedef int32_t (*M24256_DeInit_Func)(void);
typedef int32_t (*M24256_GetTick_Func)(void);
typedef int32_t (*M24256_Delay_Func)(uint32_t);
typedef int32_t (*M24256_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*M24256_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*M24256_IsDeviceReady_Func)(uint16_t, uint32_t);


typedef int32_t (*M24256_Write_Func)(const void *, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*M24256_Read_Func)(const void *, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*M24256_IsReady_Func)(const void *, uint32_t);

typedef struct
{
  M24256_Write_Func     Write;
  M24256_Read_Func      Read;
  M24256_IsReady_Func   IsReady;
  void                  *handle;
} M24256_Ctx_t;

typedef struct
{
  M24256_Init_Func           Init;
  M24256_DeInit_Func         DeInit;
  uint16_t                   Address;
  M24256_WriteReg_Func       Write;
  M24256_ReadReg_Func        Read;
  M24256_IsDeviceReady_Func  IsReady;
  M24256_GetTick_Func        GetTick;
} M24256_IO_t;

typedef struct
{
  M24256_IO_t         IO;
  M24256_Ctx_t        Ctx;
  uint8_t             IsInitialized;
} M24256_Object_t;

typedef struct
{
  int32_t (*Init)(M24256_Object_t *);
  int32_t (*DeInit)(M24256_Object_t *);
  int32_t (*Write)(const M24256_Object_t *, uint16_t, uint8_t *, uint16_t);
  int32_t (*Read)(const M24256_Object_t *, uint16_t, uint8_t *, uint16_t);
  int32_t (*IsReady)(const M24256_Object_t *, uint32_t);
} M24256_EEPROM_Drv_t;
/**
  * @}
  */

/** @defgroup M24256_Exported_Constants   M24256 Exported Constants
  * @{
  */
#define M24256_OK      0
#define M24256_ERROR  -1

/**
  * @}
  */

/** @defgroup M24256_Exported_FunctionsPrototypes   M24256 Exported FunctionsPrototypes
  * @{
  */
/*  public function  --------------------------------------------------------------------------*/
int32_t M24256_RegisterBusIO(M24256_Object_t *pObj, M24256_IO_t *pIO);
int32_t M24256_Init(M24256_Object_t *pObj);
int32_t M24256_DeInit(M24256_Object_t *pObj);
int32_t M24256_Write(const M24256_Object_t *pObj, uint16_t Addr, uint8_t *pData, uint16_t Length);
int32_t M24256_Read(const M24256_Object_t *pObj, uint16_t Addr, uint8_t *pData, uint16_t Length);
int32_t M24256_IsReady(const M24256_Object_t *pObj, uint32_t Trials);


extern M24256_EEPROM_Drv_t   M24256_EEPROM_Driver;
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

#endif /* M24256_H */
