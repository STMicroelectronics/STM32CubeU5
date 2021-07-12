/**
  ******************************************************************************
  * @file    stm32u575i_eval_lcd.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u575i_eval_lcd.c driver.
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
#ifndef STM32U575I_EVAL_LCD_H
#define STM32U575I_EVAL_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"
#include "../Components/Common/lcd.h"
#include "../Components/hx8347i/hx8347i.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_LCD
  * @{
  */

/** @defgroup STM32U575I_EVAL_LCD_Exported_Constants LCD Exported Constants
  * @{
  */
/* LCD instances */
#define LCD_INSTANCES_NBR 1U

/* LCD orientations */
#define LCD_ORIENTATION_PORTRAIT          0U
#define LCD_ORIENTATION_LANDSCAPE         1U
#define LCD_ORIENTATION_PORTRAIT_ROT180   2U
#define LCD_ORIENTATION_LANDSCAPE_ROT180  3U

/* LCD colors */
#define LCD_COLOR_RGB565_BLUE                 0x001FU
#define LCD_COLOR_RGB565_GREEN                0x07E0U
#define LCD_COLOR_RGB565_RED                  0xF800U
#define LCD_COLOR_RGB565_CYAN                 0x07FFU
#define LCD_COLOR_RGB565_MAGENTA              0xF81FU
#define LCD_COLOR_RGB565_YELLOW               0xFFE0U
#define LCD_COLOR_RGB565_LIGHTBLUE            0x841FU
#define LCD_COLOR_RGB565_LIGHTGREEN           0x87F0U
#define LCD_COLOR_RGB565_LIGHTRED             0xFC10U
#define LCD_COLOR_RGB565_LIGHTCYAN            0x87FFU
#define LCD_COLOR_RGB565_LIGHTMAGENTA         0xFC1FU
#define LCD_COLOR_RGB565_LIGHTYELLOW          0xFFF0U
#define LCD_COLOR_RGB565_DARKBLUE             0x0010U
#define LCD_COLOR_RGB565_DARKGREEN            0x0400U
#define LCD_COLOR_RGB565_DARKRED              0x8000U
#define LCD_COLOR_RGB565_DARKCYAN             0x0410U
#define LCD_COLOR_RGB565_DARKMAGENTA          0x8010U
#define LCD_COLOR_RGB565_DARKYELLOW           0x8400U
#define LCD_COLOR_RGB565_WHITE                0xFFFFU
#define LCD_COLOR_RGB565_LIGHTGRAY            0xD69AU
#define LCD_COLOR_RGB565_GRAY                 0x8410U
#define LCD_COLOR_RGB565_DARKGRAY             0x4208U
#define LCD_COLOR_RGB565_BLACK                0x0000U
#define LCD_COLOR_RGB565_BROWN                0xA145U
#define LCD_COLOR_RGB565_ORANGE               0xFD20U
/* Definition of Official ST COLOR */
#define LCD_COLOR_RGB565_ST_BLUE_DARK         0x0001U
#define LCD_COLOR_RGB565_ST_BLUE              0x01EBU
#define LCD_COLOR_RGB565_ST_BLUE_LIGHT        0x06A7U
#define LCD_COLOR_RGB565_ST_GREEN_LIGHT       0x05ECU
#define LCD_COLOR_RGB565_ST_GREEN_DARK        0x001CU
#define LCD_COLOR_RGB565_ST_YELLOW            0x07F0U
#define LCD_COLOR_RGB565_ST_BROWN             0x02C8U
#define LCD_COLOR_RGB565_ST_PINK              0x0681U
#define LCD_COLOR_RGB565_ST_PURPLE            0x02CDU
#define LCD_COLOR_RGB565_ST_GRAY_DARK         0x0251U
#define LCD_COLOR_RGB565_ST_GRAY              0x04BAU
#define LCD_COLOR_RGB565_ST_GRAY_LIGHT        0x05E7U
/**
  * @}
  */


/** @defgroup STM32U575I_EVAL_LCD_Exported_Types LCD Exported Types
  * @{
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
typedef struct
{
  pSRAM_CallbackTypeDef  pMspFmcInitCb;
  pSRAM_CallbackTypeDef  pMspFmcDeInitCb;
} BSP_LCD_Cb_t;
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS > 0) */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Exported_Variables LCD Exported Variables
  * @{
  */
extern SRAM_HandleTypeDef hlcd_sram[LCD_INSTANCES_NBR];

extern void      *Lcd_CompObj[LCD_INSTANCES_NBR];
extern LCD_Drv_t *Lcd_Drv[LCD_INSTANCES_NBR];
extern const      LCD_UTILS_Drv_t LCD_Driver;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Exported_Functions LCD Exported Functions
  * @{
  */
int32_t  BSP_LCD_Init(uint32_t Instance, uint32_t Orientation);
int32_t  BSP_LCD_DeInit(uint32_t Instance);
int32_t  BSP_LCD_DisplayOn(uint32_t Instance);
int32_t  BSP_LCD_DisplayOff(uint32_t Instance);
int32_t  BSP_LCD_SetBrightness(uint32_t Instance, uint32_t Brightness);
int32_t  BSP_LCD_GetBrightness(uint32_t Instance, uint32_t *Brightness);
int32_t  BSP_LCD_GetXSize(uint32_t Instance, uint32_t *Xsize);
int32_t  BSP_LCD_GetYSize(uint32_t Instance, uint32_t *Ysize);
int32_t  BSP_LCD_SetActiveLayer(uint32_t Instance, uint32_t LayerIndex);
int32_t  BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t  BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width,
                             uint32_t Height);
int32_t  BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t  BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t  BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height,
                          uint32_t Color);
int32_t  BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t  BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t  BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *Format);

#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
int32_t  BSP_LCD_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t  BSP_LCD_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_FMC_BANK4_Init(SRAM_HandleTypeDef *hSram);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U575I_EVAL_LCD_H */
