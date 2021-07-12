/**
  ******************************************************************************
  * @file    app_main.c
  * @author  MCD Application Team
  * @brief   Main application (root menu) implementation
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
#define __APP_MAIN_C

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_lcd.h"

#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "resources.h"
/* Private constants ---------------------------------------------------------*/

/* Display columns positions */
#define X1_POS    32
#define X2_POS    X1_POS + ICON_WIDTH + 3
#define X3_POS    X2_POS + ICON_WIDTH + 3
#define X4_POS    X3_POS + ICON_WIDTH + 3

/* Display ranks positions */
#define Y1_POS    32
#define Y2_POS    Y1_POS + ICON_HEIGHT + 3

/* Private function prototypes -----------------------------------------------*/
static KMODULE_RETURN AppMainExec(void);
static KMODULE_RETURN AppMainExecCheckResource(void);


/* Private Variable ----------------------------------------------------------*/
/* Variable used to exit an execution context */
__IO uint8_t exit_func;


tMenuItem MainMenuItems[] =
{
    {"LPBAM"                , X1_POS, Y1_POS, SEL_MODULE, MODULE_LPBAM       , NULL         , NULL             , NULL, (char*) IconLpbam0       ,(char*) IconLpbam1        },
    {"TrustZone"            , X2_POS, Y1_POS, SEL_MODULE, MODULE_SECURITY    , NULL         , NULL             , NULL, (char*) IconTrsutZone0   ,(char*) IconTrsutZone1    },
    {"Digital Filters"      , X3_POS, Y1_POS, SEL_MODULE, MODULE_AUDIOFILT   , NULL         , NULL             , NULL, (char*) IconAudio0       ,(char*) IconAudio1        },
    {"USB-PD"               , X4_POS, Y1_POS, SEL_MODULE, MODULE_USBPD       , NULL         , NULL             , NULL, (char*) IconUsbPd0       ,(char*) IconUsbPd1        },
    {"Low Power"            , X1_POS, Y2_POS, SEL_MODULE, MODULE_LOWPOWER    , NULL         , NULL             , NULL, (char*) IconLowPower0    ,(char*) IconLowPower1     },
    {"Camera"               , X2_POS, Y2_POS, SEL_MODULE, MODULE_CAMERA      , NULL         , NULL             , NULL, (char*) IconCamera0      ,(char*) IconCamera1       },
    {"Calendar"             , X3_POS, Y2_POS, SEL_MODULE, MODULE_CALENDAR    , NULL         , NULL             , NULL, (char*) IconCalender0    ,(char*) IconCalender1     },
    {"About Demo"           , X4_POS, Y2_POS, SEL_MODULE, MODULE_SYSINFO     , NULL         , NULL             , NULL, (char*) IconSysInfo0     ,(char*) IconSysInfo1      },

};


const tMenu MainMenu = {
  "Main menu", MainMenuItems, countof(MainMenuItems), TYPE_ICON, 3, 4
  };

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAppMain =
{
  MODULE_MAIN_APP,
  NULL,
  AppMainExec,
  NULL,
  AppMainExecCheckResource
};

/* Private function ----------------------------------------------------------*/
/**
  * @brief  Run the main application
  * @param  None.
  * @note   run and display main menu.
  * @retval KMODULE_RETURN status.
  */
static KMODULE_RETURN AppMainExec(void)
{
  uint8_t index;
  uint8_t indexcol;
  uint8_t indexlin;
  uint8_t iconnb = countof(MainMenuItems);
  uint32_t x, y;
  uint8_t column_nb;
  uint8_t line_nb;
  uint8_t inter_x;
  uint8_t inter_y;

  BSP_LCD_GetXSize(0, &x);
  BSP_LCD_GetYSize(0, &y);

#define MIN_INTER_X 2 // Min 2 pixel between each icon
  /* compute nb of icon on a line */
  column_nb = ( x - MIN_INTER_X) / (ICON_WIDTH + MIN_INTER_X) ;

  /* compute nb of icon lines */
  line_nb = (iconnb / column_nb);

  if((iconnb % column_nb) != 0)
  {
    line_nb++;
  }
  
  /* compute space between incons on x axis */
  inter_x = ((x - (column_nb * ICON_WIDTH)) / (column_nb + 1)) ;

  /* compute space between incons on x axis */
  inter_y = ((y - UTIL_LCD_GetFont()->Height) - (line_nb * ICON_HEIGHT)) / (line_nb + 1);

  /* fill menu item icon position */
  indexcol = 0;
  indexlin = 0;
  for(index = 0; index < iconnb; index++)
  {
    MainMenuItems[index].x = (inter_x + (indexcol * (inter_x + ICON_WIDTH)));
    MainMenuItems[index].y = (inter_y + (UTIL_LCD_GetFont()->Height) + (indexlin * (inter_y + ICON_HEIGHT)));

    /* increment colomn index */
    indexcol++;
    if(indexcol >= column_nb)
    {
      indexcol = 0;
      indexlin++;
    }
  }

  kMenu_Execute(MainMenu);
  return KMODULE_OK;
}

/**
  * @brief  check the main application resources
  * @param  None.
  * @note   None.
  * @retval KMODULE_RETURN status.
  */
static KMODULE_RETURN AppMainExecCheckResource(void)
{
  return KMODULE_OK;
}



