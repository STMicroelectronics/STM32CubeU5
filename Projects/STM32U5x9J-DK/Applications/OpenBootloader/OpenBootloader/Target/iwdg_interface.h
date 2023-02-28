/**
  ******************************************************************************
  * @file    iwdg_interface.h
  * @author  MCD Application Team
  * @brief   Header for iwdg_interface.c module
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
#ifndef IWDG_INTERFACE_H
#define IWDG_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_IWDG_Configuration(void);
void OPENBL_IWDG_Refresh(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IWDG_INTERFACE_H */
