/**
  ******************************************************************************
  * @file    openbl_i2c_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_i2c_cmd.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_I2C_CMD_H
#define OPENBL_I2C_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_I2C_VERSION                 0x12U               /* Open Bootloader I2C protocol V1.2 */
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern OPENBL_CommandsTypeDef OPENBL_I2C_Commands;

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_I2C_GetCommandsList(void);

#endif /* OPENBL_I2C_CMD_H */
