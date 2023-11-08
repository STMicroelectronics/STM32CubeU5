/**
  ******************************************************************************
  * @file    stm32u575i_eval_eeprom.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for
  *          the stm32u575i_eval_eeprom.c firmware driver.
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
#ifndef STM32U575I_EVAL_EEPROM_H
#define STM32U575I_EVAL_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"
#include "../Components/m24256/m24256.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_EEPROM
  * @brief This file includes the I2C EEPROM driver of STM32U575I_EVAL board.
  * @{
  */

/** @defgroup STM32U575I_EVAL_EEPROM_Exported_Types EEPROM Exported Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_EEPROM_Exported_Constants EEPROM Exported Constants
  * @{
  */
#define EEPROM_INSTANCES_NBR       1U

/* EEPROM hardware address and page size */
#define EEPROM_PAGESIZE             64U /* 64 Byte */
#define EEPROM_MAX_SIZE             0x4000U /* 16 Kbyte*/

#define EEPROM_I2C_ADDRESS          0xA0U

/**
  * @}
  */
/** @addtogroup STM32U575I_EVAL_EEPROM_Exported_Variables EEPROM Exported Variables
  * @{
  */
extern void  *Eeprom_CompObj;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_EEPROM_Exported_FunctionsPrototypes EEPROM Exported Functions Prototypes
  * @{
  */
int32_t BSP_EEPROM_Init(uint32_t Instance);
int32_t BSP_EEPROM_DeInit(uint32_t Instance);
int32_t BSP_EEPROM_WritePage(uint32_t Instance, uint8_t *pBuffer, uint32_t PageNbr);
int32_t BSP_EEPROM_ReadPage(uint32_t Instance, uint8_t *pBuffer, uint32_t PageNbr);
int32_t BSP_EEPROM_WriteBuffer(uint32_t Instance, uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NbrOfBytes);
int32_t BSP_EEPROM_ReadBuffer(uint32_t Instance, uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NbrOfBytes);
int32_t BSP_EEPROM_IsDeviceReady(uint32_t Instance);

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

#endif /* STM32U575I_EVAL_EEPROM_H */
