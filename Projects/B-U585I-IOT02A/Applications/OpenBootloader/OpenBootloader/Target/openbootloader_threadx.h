/**
  ******************************************************************************
  * @file    openbootloader_threadx.h
  * @author  MCD Application Team
  * @brief   Header for app_openbootloader.c module
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
#ifndef OPENBOOTLOADER_THREADX_H
#define OPENBOOTLOADER_THREADX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t OpenBootloader_ThreadxInit(void *memory_ptr);

/* External variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* OPENBOOTLOADER_THREADX_H */
