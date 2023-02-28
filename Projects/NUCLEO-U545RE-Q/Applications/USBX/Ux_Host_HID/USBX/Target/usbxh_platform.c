/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbxh_platform.c

  * @brief          : This file implements the USBX platform
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbxh_platform.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

void USBH_DriverVBUS(uint8_t state)
{

  uint8_t data = state;
  /* USER CODE BEGIN PREPARE_GPIO_DATA_VBUS_FS */
  if(state == 0)
  {
    /* Drive low Charge pump */
    data = GPIO_PIN_RESET;
  }
  else
  {
    /* Drive high Charge pump */
    data = GPIO_PIN_SET;
  }

  /* USER CODE END PREPARE_GPIO_DATA_VBUS_FS */
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,(GPIO_PinState)data);

}
