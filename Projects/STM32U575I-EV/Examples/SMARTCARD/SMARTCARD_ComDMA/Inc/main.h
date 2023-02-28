/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_io.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
/* SmartCard power state structure */
typedef enum
{
  SC_POWER_ON     = 0x00,
  SC_RESET_LOW    = 0x01,
  SC_RESET_HIGH   = 0x02,
  SC_ACTIVE       = 0x03,
  SC_ACTIVE_ON_T0 = 0x04,
  SC_POWER_OFF    = 0x05
} SmartCard_State;

/* Answer To Reset structure ATR */
typedef struct
{
  uint8_t TS;               /* Bit Convention */
  uint8_t T0;               /* High nibble = Number of setup byte; low nibble = Number of historical byte */
  uint8_t T[20];            /* Setup array */
  uint8_t H[20];            /* Historical array */
  uint8_t Tlength;          /* Setup array dimension */
  uint8_t Hlength;          /* Historical array dimension */
} SmartCard_ATR;

/* ADPU-Header command structure */
typedef struct
{
  uint8_t CLA;  /* Command class */
  uint8_t INS;  /* Operation code */
  uint8_t P1;   /* Selection Mode */
  uint8_t P2;   /* Selection Option */
} SmartCard_Header;

/* ADPU-Body command structure */
typedef struct
{
  uint8_t LC;           /* Data field length */
  uint8_t Data[256];  /* Command parameters */
  uint8_t LE;           /* Expected length of data to be returned */
} SmartCard_Body;

/* ADPU Command structure */
typedef struct
{
  SmartCard_Header Header;
  SmartCard_Body Body;
} SmartCard_ADPU_Commands;

/* SmartCard response structure -----------------------------------------------------*/
typedef struct
{
  uint8_t Data[256];    /* Data returned from the card */
  uint8_t SW1;          /* Command Processing status */
  uint8_t SW2;          /* Command Processing qualification */
} SmartCard_ADPU_Response;

/* Exported constants --------------------------------------------------------*/

/* Answer to reset Commands */
#define SC_GET_A2R         0x00

/* SC ADPU Command: Operation Code */
#define SC_CLA_GSM11       0xA0

/* Data Area Management Commands */
#define SC_SELECT_FILE     0xA4


#define T0_PROTOCOL         0x00  /* T0 protocol */
#define SC_RECEIVE_TIMEOUT  1000  /* Direction to reader */
#define SC_TRANSMIT_TIMEOUT 200   /* Direction to transmit */


#define SMARTCARD_USART_INSTANCE         USART3
#define SMARTCARD_PRESCALER              2

#define SMARTCARD_3_5V_PIN               IO_PIN_10
#define SMARTCARD_RESET_PIN              IO_PIN_7
#define SMARTCARD_CMDVCC_PIN             IO_PIN_9
#define SMARTCARD_OFF_PIN                IO_PIN_6
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
