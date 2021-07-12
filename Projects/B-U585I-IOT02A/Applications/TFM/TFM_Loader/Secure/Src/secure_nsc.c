/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
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
#include "main.h"
#include "secure_nsc.h"
#include "Driver_Flash.h"
#include "region_defs.h"
/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */
#if defined(MCUBOOT_PRIMARY_ONLY)
extern ARM_DRIVER_FLASH LOADER_FLASH_DEV_NAME;
/** @addtogroup Templates
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure Flash Program Non Secure Calleable
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY int32_t SECURE_Flash_ProgramData(int32_t addr,const void *data, uint32_t cnt)
{
 /* Data outside of non seure data are not flashed */
  if (((uint32_t)data < LOADER_NS_DATA_START ) || (((uint32_t)data + cnt)> LOADER_NS_DATA_LIMIT))
	return -1;
  return LOADER_FLASH_DEV_NAME.ProgramData(addr, data, cnt);
}

/**
  * @brief  Secure Flash Erase Sector Non Secure Calleable
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY int32_t SECURE_Flash_EraseSector(uint32_t addr)
{
   return LOADER_FLASH_DEV_NAME.EraseSector(addr);
}
#else
CMSE_NS_ENTRY void dummy(void)
{
	return;
}
#endif /* MCUBOOT_PRIMARY_ONLY */
/**
  * @}
  */

/**
  * @}
  */