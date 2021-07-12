/**
  **********************************************************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @brief   This file implements the bsp resources.
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "bsp.h"

/** @addtogroup STM32U5xx_Demonstration
  * @{
  */

/** @addtogroup IOT_HTTP_WebServer
  * @{
  */

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/

/**
  * @brief  Initialize the bsp modules
  * @param  None
  * @retval Web Server status
  */
int bsp_init(void)
{
  int status = BSP_ERROR_NONE;

  /* Initialize GREEN led */
  status |= BSP_LED_Init(LED_GREEN);

  /* Initialize RED led */
  status |= BSP_LED_Init(LED_RED);

  return status;
}
