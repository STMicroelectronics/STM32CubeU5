/**
  ******************************************************************************
  * @file    Examples/BSP/Inc/main.h
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
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "stm32u5xx_hal.h"
#include "b_u585i_iot02a.h"
#include "b_u585i_iot02a_bus.h"
#include "b_u585i_iot02a_ospi.h"
#include "b_u585i_iot02a_motion_sensors.h"
#include "b_u585i_iot02a_env_sensors.h"
#include "b_u585i_iot02a_camera.h"
#include "b_u585i_iot02a_audio.h"
#include "b_u585i_iot02a_eeprom.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  int32_t  (*DemoFunc)(void);
  uint8_t  DemoName[50];
} BSP_DemoTypedef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported variables --------------------------------------------------------*/
extern __IO FlagStatus UserButtonPressed;

/* Exported functions ------------------------------------------------------- */
int32_t  Led_demo(void);
int32_t  Button_demo(void);
int32_t  Ms_demo(void);
int32_t  Es_demo(void);
int32_t  Camera_demo(void);
int32_t  AudioRecord_demo(void);
int32_t  OSPI_NOR_demo(void);
int32_t  OSPI_RAM_demo(void);
int32_t  EEPROM_demo(void);

void Error_Handler(void);
uint32_t CheckResult(void);
void     StartExample(void);

#endif /* MAIN_H */
