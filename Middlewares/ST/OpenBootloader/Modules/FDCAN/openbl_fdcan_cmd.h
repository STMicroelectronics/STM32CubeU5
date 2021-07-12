/**
  ******************************************************************************
  * @file    openbl_fdcan_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_fdcan_cmd.c module
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
#ifndef OPENBL_FDCAN_CMD_H
#define OPENBL_FDCAN_CMD_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_FDCAN_VERSION             0x10U                /* Open Bootloader FDCAN protocol V1.0 */
#define FDCAN_RAM_BUFFER_SIZE             1156                /* Size of FDCAN buffer used to store received data from the host */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t TxData[FDCAN_RAM_BUFFER_SIZE];
extern uint8_t RxData[FDCAN_RAM_BUFFER_SIZE];

extern OPENBL_CommandsTypeDef OPENBL_FDCAN_Commands;

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_FDCAN_GetCommandsList(void);

#endif /* OPENBL_FDCAN_CMD_H */
