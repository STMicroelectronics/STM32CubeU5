/**
  ******************************************************************************
  * @file    app_audiofilt.c
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
#define __APP_AUDIOFILT_C

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32u5xx_hal.h"
#include "app_audiofilt.h"
#include "audio_recplay.h"
#include "sad.h"
#include "sig_filtering.h"
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
#define X1_POS    32
#define X2_POS    X1_POS + ICON_WIDTH + 20
#define X3_POS    X2_POS + ICON_WIDTH + 20

#define Y1_POS     54
#define Y2_POS     Y1_POS + ICON_HEIGHT + 10

/* Private function prototypes -----------------------------------------------*/
static KMODULE_RETURN _AudiofiltDemoExec(void);

/* Private Variable ----------------------------------------------------------*/

/* AudioFiltering menu */
static const tMenuItem AudiofiltMenuMenuItems[] =
{
    {"Sound Detector" , X1_POS, Y1_POS, SEL_EXEC,    MODULE_NONE,Sad_Demo , Sad_UserAction, NULL, (char*)  IconAudioSad0 , (char*)  IconAudioSad1 },
    {"Audio Record/Play"     , X2_POS, Y1_POS, SEL_EXEC,    MODULE_NONE,     Audio_Recplay_Demo      ,     Audio_Recplay_UserAction      , NULL, (char*) IconAudioPlay0, (char*)IconAudioPlay1 },
    {"Signal Filtering"      , X3_POS, Y1_POS, SEL_EXEC,    MODULE_NONE, Sig_Filtering_Demo, Sig_Filtering_UserAction, NULL, (char*)IconDataFltr0 , (char*)IconDataFltr1 },
    {"Return"                , X2_POS, Y2_POS, SEL_EXIT,    MODULE_NONE, NULL              , NULL                    , NULL, (char*)IconReturn0      , (char*)IconReturn1     },
};

static const tMenu AudiofiltMenu = {
  "Audio/Filtering", AudiofiltMenuMenuItems, countof(AudiofiltMenuMenuItems), TYPE_ICON, 1, 1 };

/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAudiofilt =
{
  MODULE_AUDIOFILT,
  NULL,
  _AudiofiltDemoExec,
  NULL,
  NULL
};
uint32_t Digital_filter_module = 0;
/* Exported variables --------------------------------------------------------*/
/**
  * @brief  Run the applications
  * @param  None.
  * @retval KMODULE_RETURN status.
  */
static KMODULE_RETURN _AudiofiltDemoExec(void)
{
  /* Prepare the main MMI */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  kMenu_Execute(AudiofiltMenu);

  /* App initialization    */
  return KMODULE_OK;
}
