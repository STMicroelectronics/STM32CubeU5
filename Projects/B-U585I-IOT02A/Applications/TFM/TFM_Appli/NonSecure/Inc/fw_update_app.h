/**
  ******************************************************************************
  * @file    fw_update_app.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Firmware Update module
  *          functionalities.
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
#ifndef FW_UPDATE_APP_H
#define FW_UPDATE_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup FW_UPDATE Firmware Update Example
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/** @addtogroup  FW_UPDATE_Exported_Functions
  * @{
  */

void FW_UPDATE_Run(void);

/**
  * @}
  */
typedef struct
{
  uint32_t  MaxSizeInBytes;        /*!< The maximum allowed size for the FwImage in User Flash (in Bytes) */
  uint32_t  DownloadAddr;          /*!< The download address for the FwImage in UserFlash */
  uint32_t  ImageOffsetInBytes;    /*!< Image write starts at this offset */
  uint32_t  ExecutionAddr;         /*!< The execution address for the FwImage in UserFlash */
} SFU_FwImageFlashTypeDef;

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* FW_UPDATE_APP_H */

