/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_I2C_SequentialTransfer/Inc/main.h
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
#include "i2c_timing_utility.h"
#include "stm32_lpbam.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* #define DEBUG_CONFIGURATION */

/* Uncomment this line to use the board as master, if not it is used as slave */
#define MASTER_BOARD

/* I2C defines */
#define I2C_ADDRESS          (0x35U)
#define BUS_I2Cx_FREQUENCY   (100000U) /* Frequency of I2C3 = 100 KHz*/
#define I2C_MAX_DATA_SIZE    (0xFFU)

/* Size of Transmission buffer */
#define TXBUFFERSIZE1        (COUNTOF(aTxBuffer1) - 1U)
#define TXBUFFERSIZE2        (COUNTOF(aTxBuffer2) - 1U)

/* Size of Reception buffer */
#define RXBUFFERSIZE1        TXBUFFERSIZE1
#define RXBUFFERSIZE2        TXBUFFERSIZE2

/* RTC defines */
#define RTC_ASYNCH_PREDIV    (0x7F)
#define RTC_SYNCH_PREDIV     (0x00F9)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void Custom_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);
void Custom_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c);

#endif /* MAIN_H */
