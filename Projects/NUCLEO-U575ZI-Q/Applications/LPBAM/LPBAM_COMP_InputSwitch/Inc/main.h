/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_COMP_InputSwitch/Inc/main.h
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

#define COMP_OUTPUT_VALUE_MASK      0x40000000U
#define OUTPUT_BUFFERSIZE           4U
#define LPTIM_PWM_PERIOD            49999U
#define INPUTMINUS_ABOVE_INPUTPLUS  0U
#define INPUTMINUS_BELOW_INPUTPLUS  1U

/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/

#endif /* MAIN_H */
