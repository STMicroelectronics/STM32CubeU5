/**
  ******************************************************************************
  * @file    sig_filtering.h
  * @author  MCD Application Team
  * @brief   Generic siganls filtering (Power metering) implementation header file
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
#ifndef __SIG_FILTERING_H
#define __SIG_FILTERING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Sig_Filtering_Demo(void);
void Sig_Filtering_UserAction(uint8_t sel);

#ifdef __cplusplus
}
#endif

#endif /* __SIG_FILTERING_H */
