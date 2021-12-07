/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/main.h
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

#define DMA_TOGGLEIO_QUEUES_NUM   (0x02U)
#define TOGGLEIO_0_IDX            (0x00U)
#define TOGGLEIO_1_IDX            (0x01U)
#define LPGPIO_TOGGLE_BUFFER_SIZE (0x02U)

/* LPTIM pulse and period defines */
#define LPTIM1_PWM_PERIOD  32258U
#define LPTIM1_PWM_PULSE   LPTIM1_PWM_PERIOD / 2U
#define LPTIM3_PWM_PERIOD  3226U
#define LPTIM3_PWM_PULSE   LPTIM3_PWM_PERIOD / 2U

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* MAIN_H */
