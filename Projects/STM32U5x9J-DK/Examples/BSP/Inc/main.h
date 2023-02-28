/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "stm32u5x9j_discovery.h"
#include "stm32u5x9j_discovery_conf.h"
#include "stm32u5x9j_discovery_ospi.h"
#include "stm32u5x9j_discovery_lcd.h"
#include "stm32u5x9j_discovery_ts.h"
#include "stm32u5x9j_discovery_hspi.h"
#include "stm32u5x9j_discovery_mmc.h"
#include "stm32u5x9j_discovery_ranging_sensor.h"

#include "stm32_lcd.h"


/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  int32_t  (*DemoFunc)(void);
  uint8_t  DemoName[50];
} BSP_DemoTypedef;

/* Exported variables --------------------------------------------------------*/
extern __IO FlagStatus UserButtonPressed;
extern uint8_t DrawBackround;
extern uint8_t AutoMode;
extern uint16_t x;
extern uint16_t y;
extern uint16_t A;
extern uint16_t B;
extern const unsigned char back[7986UL + 1];
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
int32_t OSPI_RAM_demo(void);
int32_t Lcd_demo(void);
int32_t OSPI_NOR_demo (void);
int32_t eMMC_demo (void);
int32_t Led_demo (void);
int32_t Touchscreen_demo(void);
void    Error_Handler(void);
uint32_t CheckResult(void);
void     StartTest(void);
int32_t  Rs_demo(void);
#endif /* MAIN_H */


