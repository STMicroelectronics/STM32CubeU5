/**
  ******************************************************************************
  * @file    sens_acquisition.h
  * @author  MCD Application Team
  * @brief   Sensor acquisition interface header file
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
#ifndef __SENS_ACQUISITION_H
#define __SENS_ACQUISITION_H

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
void Sens_Acquisition_Demo(void);
void Sens_Acquisition_UserAction(uint8_t sel);

#ifdef __cplusplus
}
#endif

#endif /* __SENS_ACQUISITION_H */
