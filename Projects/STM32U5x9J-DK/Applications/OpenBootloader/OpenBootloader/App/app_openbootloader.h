/**
  ******************************************************************************
  * @file    app_openbootloader.h
  * @author  MCD Application Team
  * @brief   Header for app_openbootloader.c module
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
#ifndef APP_OPENBOOTLOADER_H
#define APP_OPENBOOTLOADER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SPECIAL_CMD_MAX_NUMBER            0x01U    /* Special command command max length array */
#define EXTENDED_SPECIAL_CMD_MAX_NUMBER   0x01U    /* Extended special command max length array */
#define SPECIAL_CMD_DEFAULT               0x0102U  /* Default special command */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OpenBootloader_Init(void);
void OpenBootloader_DeInit(void);
void OpenBootloader_ProtocolDetection(void);

/* External variables --------------------------------------------------------*/
extern OPENBL_MemoryTypeDef FLASH_Descriptor;
extern OPENBL_MemoryTypeDef RAM_Descriptor;
extern OPENBL_MemoryTypeDef OB_Descriptor;
extern OPENBL_MemoryTypeDef OTP_Descriptor;
extern OPENBL_MemoryTypeDef ICP_Descriptor;
extern OPENBL_MemoryTypeDef EB_Descriptor;

extern uint16_t SpecialCmdList[SPECIAL_CMD_MAX_NUMBER];
extern uint16_t ExtendedSpecialCmdList[EXTENDED_SPECIAL_CMD_MAX_NUMBER];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APP_OPENBOOTLOADER_H */
