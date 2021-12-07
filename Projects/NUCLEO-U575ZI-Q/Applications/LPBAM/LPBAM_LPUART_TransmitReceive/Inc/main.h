/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPUART_TransmitReceive/Inc/main.h
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

/* Uncomment this line to use the board as transmitter, otherwise as receiver */
#define TRANSMITTER_BOARD

#define TX_BUFFER_SIZE  (COUNTOF(Tx_Buffer) - 1)
#define RX_BUFFER_SIZE  (2 * TX_BUFFER_SIZE)

/* Exported functions ------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))


#endif /* MAIN_H */
