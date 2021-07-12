/**
  ******************************************************************************
  * @file    k_tools.c
  * @author  MCD Application Team
  * @brief   This file contains the kernel tools functions
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

/* Includes ------------------------------------------------------------------*/
#include "k_tools.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Detect if an icon is touched
  * @param  x the x coordinates
  * @param  y the y coordinates
  * @param  psCurrentMenu
  * @retval 0  : No icon touched
  *         >0 : The index of the touched icon
  */
int8_t kMenu_IconTouched(uint16_t x, uint16_t y, tMenu psCurrentMenu)
{
  for (int8_t icon = 0; icon <= psCurrentMenu.nItems; icon++)
  {
    if ((y < psCurrentMenu.psItems[icon].y + ICON_HEIGHT) && (y > psCurrentMenu.psItems[icon].y + 1)
        && (x < psCurrentMenu.psItems[icon].x + ICON_WIDTH) && (x > psCurrentMenu.psItems[icon].x + 1))
    {
      return icon + 1;
    }
  }

  return 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2   buffers to be compared.
  * @param  BufferLength         length of the buffer to compare
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
uint16_t kTools_Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
