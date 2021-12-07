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
#include "stm32u5xx_hal.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_eeprom.h"
#include "stm32u575i_eval_audio.h"
#include "stm32u575i_eval_bus.h"
#include "stm32u575i_eval_camera.h"
#include "stm32u575i_eval_lcd.h"
#include "stm32u575i_eval_sd.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_sram.h"
#include "stm32u575i_eval_ospi.h"
#include "stm32u575i_eval_motion_sensors.h"
#include "stm32_lcd.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FMC_ADDRESS           FMC_BANK1_4
#define REGION_SIZE           0x0CFFFFFF

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern __IO FlagStatus UserButtonPressed;
extern __IO JOYPin_TypeDef JoyPinPressed;
extern const unsigned char stlogo[];
extern uint32_t CameraTest;

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void Led_demo(void);
void LCD_demo(void);
void Joystick_demo(void);
void SD_demo(void);
void SRAM_demo(void);
void Touchscreen_demo(void);
void OSPI_demo(void);
void Camera_demo(void);
void AudioRecord_demo(void);
void MotionSensor_demo(void);
void AudioPlay_demo(void);
void EEPROM_demo(void);

#endif /* MAIN_H */
