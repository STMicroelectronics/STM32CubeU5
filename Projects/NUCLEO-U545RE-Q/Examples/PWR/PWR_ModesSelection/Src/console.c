/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/console.c
  * @author  MCD Application Team
  * @brief   This file provides console output input
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "console.h"
#include <stdio.h>
#include <string.h>

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @addtogroup PWR_ModesSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declared in "main.c" file */
extern UART_HandleTypeDef huart1;

/* Receive status flag */
__IO uint8_t ReceiveStatus = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Output a message on Hyperterminal.
  * @param  msg : Pointer to the message printed on the Hyperterminal.
  *
  * @retval None.
  */
void Console_Log(const char * msg)
{
  printf("%s", msg);
}

/**
  * @brief  Receive a command via the Hyperterminal.
  * @param  Cmd     : Pointer to return the command reveived from hyperterminal.
  * @param  CmdSize : Specifies the size of the buffer reveived from hyperterminal.
  * @retval Console Status.
  */
Console_StatusTypeDef Console_GetCommand(uint8_t *Cmd, uint16_t CmdSize)
{
  /* Start the USART Receive process to receive user inputs */
  if (HAL_UART_Receive_IT(&huart1, Cmd, CmdSize) != HAL_OK)
  {
    return CONSOLE_ERROR;
  }

  /* Wait for user inputs */
  while (ReceiveStatus == 0);

  /* Clear receive status flag */
  ReceiveStatus = 0;

  return CONSOLE_OK;
}

/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None.
  * @retval None.
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  UART receive transfer completed callback.
  * @param  husart : UART handle.
  * @retval None.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  ReceiveStatus = 1;
}
