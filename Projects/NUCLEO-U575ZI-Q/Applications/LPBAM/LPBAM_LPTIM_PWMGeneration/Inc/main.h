/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPTIM_PWMGeneration/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"
#include "stm32_lpbam.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* #define DEBUG_CONFIGURATION */

#define PWM_FORM_NUM (0x05U)

#define PWM_CONFIG_1 (0x00U)
#define PWM_CONFIG_2 (0x01U)
#define PWM_CONFIG_3 (0x02U)
#define PWM_CONFIG_4 (0x03U)
#define PWM_CONFIG_5 (0x04U)

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* MAIN_H */
