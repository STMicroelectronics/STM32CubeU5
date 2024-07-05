/**
  **********************************************************************************************************************
  * @file    console.h
  * @author  MCD Application Team
  * @brief   Header for console.c module
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef CONSOLE_H
#define CONSOLE_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver.h"
#include "net_interface.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
#define CONSOLE_BUFFER_SIZE (50U)

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------------------------------- */
void webserver_console_print_header(void);
WebServer_StatusTypeDef webserver_console_config(void);
WebServer_StatusTypeDef webserver_console_get_ssid(ap_t *net_wifi_registred_hotspot,
                                                   char *SSID);
WebServer_StatusTypeDef webserver_console_get_password(ap_t *net_wifi_registred_hotspot,
                                                       char *PassWord);

/* Private defines ---------------------------------------------------------------------------------------------------*/
#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#elif defined ( __GNUC__)
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __ICCARM__ */

#endif /* CONSOLE_H */
