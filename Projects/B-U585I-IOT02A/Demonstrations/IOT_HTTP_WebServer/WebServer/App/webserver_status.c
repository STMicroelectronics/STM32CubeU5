/**
  **********************************************************************************************************************
  * @file    webserver_status.c
  * @author  MCD Application Team
  * @brief   This file implements the web server status services.
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
#include "webserver_status.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void webserver_process_error(void)
{
  /* Disable all interrupts */
  __disable_irq();

  /* De-Initialize GREEN led */
  BSP_LED_DeInit(LED_GREEN);

  /* Turn ON RED led */
  BSP_LED_On(LED_RED);

  while(1)
  {
  }
}
