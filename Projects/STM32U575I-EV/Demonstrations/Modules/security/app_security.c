/**
  ******************************************************************************
  * @file    app_lpbam.c
  * @author  MCD Application Team
  * @brief   Audio application implementation.
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

#define __APP_TRUSTZONE_C

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32u5xx_hal.h"
#include "app_security.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_lcd.h"
#include "stm32u575i_eval_ts.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define ICON_WIDTH  64
#define ICON_HEIGHT 64

/* Display columns positions */
#define X1_POS    64
#define X2_POS    X1_POS + ICON_WIDTH + 40
#define X3_POS    (X1_POS + X2_POS) / 2

#define Y1_POS     54
#define Y2_POS     Y1_POS + ICON_HEIGHT + 10

/* Private function prototypes -----------------------------------------------*/
static KMODULE_RETURN _TrustZoneDemoExec(void);
void myControl_Access_UserAction(uint8_t sel);

/* Private Variable ----------------------------------------------------------*/

/* TrustZone menu */
static const tMenuItem TrustZoneMenuMenuItems[] =
{
    {"Control Access"  , X1_POS, Y1_POS, SEL_EXEC,    MODULE_NONE, Control_Access_Demo, NULL, NULL, (char*)IconTrustZonePerith0   , (char*)IconTrustZonePerith1   },
    {"Memory Access"    , X2_POS, Y1_POS, SEL_EXEC,    MODULE_NONE, Memory_Access_Demo  , NULL, NULL, (char*)IconTrustZoneMemory0   , (char*)IconTrustZoneMemory1 },
    {"Return"           , X3_POS, Y2_POS, SEL_EXIT,    MODULE_NONE, NULL                , NULL, NULL, (char*)IconReturn0            , (char*)IconReturn1     },
};

static const tMenu TrustZoneMenu = {
  "TRUSTZONE", TrustZoneMenuMenuItems, countof(TrustZoneMenuMenuItems), TYPE_ICON, 1, 1 };

/* Private typedef -----------------------------------------------------------*/
/* Exported variables -----TrustZoneMenuMenuItems---------------------------------------------------*/
const K_ModuleItem_Typedef ModuleTrustZone =
{
  MODULE_SECURITY,
  NULL,
  _TrustZoneDemoExec,
  NULL,
  NULL
};


/* Exported variables --------------------------------------------------------*/
/**
  * @brief  Run the applications
  * @param  None.
  * @retval KMODULE_RETURN status.
  */
static KMODULE_RETURN _TrustZoneDemoExec(void)
{
  /* Prepare the main MMI */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  kMenu_Execute(TrustZoneMenu);

  /* App initialization    */
  return KMODULE_OK;
}

