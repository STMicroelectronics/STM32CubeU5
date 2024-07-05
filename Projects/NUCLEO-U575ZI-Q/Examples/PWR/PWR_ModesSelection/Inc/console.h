/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/console.h
  * @author  MCD Application Team
  * @brief   Header for console.c module
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
#ifndef CONSOLE_H
#define CONSOLE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  Console Status Enumeration Definition.
  * @{
  */
typedef enum
{
  CONSOLE_OK    = 0x00, /*!< Console state OK             */
  CONSOLE_ERROR = 0x01  /*!< Console state ERROR          */

} Console_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */

#define CODE_ASCII_0 (0x30) /* "0" code ascii */
#define CODE_ASCII_1 (0x31) /* "1" code ascii */
#define CODE_ASCII_2 (0x32) /* "2" code ascii */
#define CODE_ASCII_3 (0x33) /* "3" code ascii */
#define CODE_ASCII_4 (0x34) /* "4" code ascii */
#define CODE_ASCII_5 (0x35) /* "5" code ascii */
#define CODE_ASCII_6 (0x36) /* "6" code ascii */
#define CODE_ASCII_7 (0x37) /* "7" code ascii */
#define CODE_ASCII_8 (0x38) /* "8" code ascii */
#define CODE_ASCII_9 (0x39) /* "9" code ascii */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void                  Console_Log       (const char * msg);
Console_StatusTypeDef Console_GetCommand(uint8_t *Cmd, uint16_t CmdSize);

#endif /* CONSOLE_H */
