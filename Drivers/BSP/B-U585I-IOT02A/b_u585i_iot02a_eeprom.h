/**
  ******************************************************************************
  * @file    b_u585i_iot02a_eeprom.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for
  *          the b_u585i_iot02a_eeprom.c firmware driver.
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
#ifndef B_U585I_IOT02A_EEPROM_H
#define B_U585I_IOT02A_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "b_u585i_iot02a_conf.h"
#include "b_u585i_iot02a_errno.h"
#include "../Components/m24256/m24256.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_U585I_IOT02A
  * @{
  */

/** @addtogroup B_U585I_IOT02A_EEPROM
  * @brief This file includes the I2C EEPROM driver of B_U585I_IOT02A board.
  * @{
  */

/** @defgroup B_U585I_IOT02A_EEPROM_Exported_Types EEPROM Exported Types
  * @{
  */
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_EEPROM_Exported_Constants EEPROM Exported Constants
  * @{
  */
#define EEPROM_INSTANCES_NBR       1U

/* EEPROM hardware address and page size */
#define EEPROM_PAGESIZE             64U /* 64 Byte */
#define EEPROM_MAX_SIZE             0x8000U /* 32 Kbyte*/

#define EEPROM_I2C_ADDRESS          0xACU

/**
  * @}
  */
/** @addtogroup B_U585I_IOT02A_EEPROM_Exported_Variables EEPROM Exported Variables
  * @{
  */
extern M24256_Object_t  *Eeprom_CompObj;
/**
  * @}
  */

/** @defgroup B_U585I_IOT02A_EEPROM_Exported_FunctionsPrototypes EEPROM Exported Functions Prototypes
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

#endif /* B_U585I_IOT02A_EEPROM_H */
