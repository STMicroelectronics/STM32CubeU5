/**
  ******************************************************************************
  * @file    secure_nsc.c
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
#include <stdio.h>
#if !defined(MCUBOOT_OVERWRITE_ONLY) && !(MCUBOOT_IMAGE_NUMBER == 1)
#include "flash_layout.h"
#include "Driver_Flash.h"
extern ARM_DRIVER_FLASH FLASH_PRIMARY_SECURE_DEV_NAME ;
#define TRAILER_MAGIC_SIZE 16
#endif
/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if (func != NULL)
  {
    switch (CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

/**
  * @brief  Secure GPIO Toggle function.
  * @param  None
  * @param  None
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_GPIO_Toggle(void)
{
  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

/**
  * @brief  Secure Operation to confirm Secure Image.
  * @param  None
  * @param  None
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_ConfirmSecureImage(void)
{
#if defined(MCUBOOT_OVERWRITE_ONLY) || (MCUBOOT_IMAGE_NUMBER == 1)
	return;
#else
  const uint8_t FlagPattern[16]={0x1 ,0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff };
  #define TRAILER_MAGIC_SIZE 16
  const uint32_t ConfirmAddress = FLASH_AREA_0_OFFSET  + FLASH_AREA_0_SIZE - (TRAILER_MAGIC_SIZE + sizeof(FlagPattern));  
  if (FLASH_PRIMARY_SECURE_DEV_NAME.ProgramData(ConfirmAddress, FlagPattern, sizeof(FlagPattern)) == ARM_DRIVER_OK)
  {
        return;
  }
  return;
#endif
}
/**
  * @}
  */

/**
  * @}
  */
