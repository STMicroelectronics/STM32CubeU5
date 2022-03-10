/**
  ******************************************************************************
  * @file    tfm_low_level_security.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Secure Firmware Update security
  *          low level interface.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TFM_LOW_LEVEL_SECURITY_H
#define TFM_LOW_LEVEL_SECURITY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "bootutil/bootutil_log.h"

/** @addtogroup TFM Secure Secure Boot / Firmware Update
  * @{
  */

/** @addtogroup TFM_LOW_LEVEL
  * @{
  */
/** @defgroup TFM_LOW_LEVEL_SECURITY Security Low Level define
  * @{
  */
#define SRAM2_PAGE_SIZE (0x400)
/**
  * @}
  */
/** @defgroup TFM_LOW_LEVEL_SECURITY Security Low Level Interface
  * @{
  */
/** @defgroup TFM_SECURITY_Configuration Security Configuration
  * @{
  */


/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup TFM_SECURITY_Exported_Constants_BOOL SFU Bool definition
  * @{
  */
typedef enum
{
  TFM_FALSE = 0U,
  TFM_TRUE = !TFM_FALSE
} TFM_BoolTypeDef;

/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Constants_State TFM Functional State definition
  * @{
  */
typedef enum
{
  TFM_DISABLE = 0U,
  TFM_ENABLE = !TFM_DISABLE
} TFM_FunctionalState;

/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Functions Exported Functions
  * @{
  */
void    TFM_LL_SECU_ApplyRunTimeProtections(void);
void    TFM_LL_SECU_UpdateRunTimeProtections(void);
void    TFM_LL_SECU_CheckStaticProtections(void);

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

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* TFM_LOW_LEVEL_SECURITY_H */
