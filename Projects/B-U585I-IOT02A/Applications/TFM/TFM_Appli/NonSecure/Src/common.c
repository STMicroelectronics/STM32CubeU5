/**
  ******************************************************************************
  * @file    common.c
  * @author  MCD Application Team
  * @brief   COMMON module.
  *          This file provides set of firmware functions to manage Common
  *          functionalities.
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

/** @addtogroup USER_APP User App Example
  * @{
  */
/** @addtogroup USER_APP_COMMON Common
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Convert a string to an integer
  * @param  pInputStr: The string to be converted
  * @param  pIntNum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *pInputStr, uint32_t *pIntNum)
{
  uint32_t i = 0U;
  uint32_t res = 0U;
  uint32_t val = 0U;

  if ((pInputStr[0U] == '0') && ((pInputStr[1U] == 'x') || (pInputStr[1U] == 'X')))
  {
    i = 2U;
    while ((i < 11U) && (pInputStr[i] != '\0'))
    {
      if (ISVALIDHEX(pInputStr[i]))
      {
        val = (val << 4U) + CONVERTHEX(pInputStr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0U;
        break;
      }
      i++;
    }

    /* valid result */
    if (pInputStr[i] == '\0')
    {
      *pIntNum = val;
      res = 1U;
    }
  }
  else /* max 10-digit decimal input */
  {
    while ((i < 11U) && (res != 1U))
    {
      if (pInputStr[i] == '\0')
      {
        *pIntNum = val;
        /* return 1 */
        res = 1U;
      }
      else if (((pInputStr[i] == 'k') || (pInputStr[i] == 'K')) && (i > 0U))
      {
        val = val << 10U;
        *pIntNum = val;
        res = 1U;
      }
      else if (((pInputStr[i] == 'm') || (pInputStr[i] == 'M')) && (i > 0U))
      {
        val = val << 20U;
        *pIntNum = val;
        res = 1U;
      }
      else if (ISVALIDDEC(pInputStr[i]))
      {
        val = val * 10U + CONVERTDEC(pInputStr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0U;
        break;
      }
      i++;
    }
  }

  return res;
}


/**
  * @brief  Transmit a byte to the HyperTerminal
  * @param  param The byte to be sent
  * @retval HAL_StatusTypeDef HAL_OK if OK
  */
HAL_StatusTypeDef Serial_PutByte(uint8_t uParam)
{
  return COM_Transmit_Y(&uParam, 1U, TX_TIMEOUT);
}
/**
  * @}
  */

/**
  * @}
  */
