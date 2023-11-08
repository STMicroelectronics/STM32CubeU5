/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_lcd.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u5x9j_discovery_lcd.c driver (ARGB8888 color format).
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5x9J_DISCOVERY_LCD_H
#define STM32U5x9J_DISCOVERY_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_conf.h"
#include "stm32u5x9j_discovery_errno.h"

#include "../Components/Common/lcd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY_LCD
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_LCD_Exported_Constants LCD Exported Constants
  * @{
  */
/* LCD instances */
#define LCD_INSTANCES_NBR 1U

/* LCD orientations */
#define LCD_ORIENTATION_PORTRAIT          0U
#define LCD_ORIENTATION_LANDSCAPE         1U
#define LCD_ORIENTATION_PORTRAIT_ROT180   2U
#define LCD_ORIENTATION_LANDSCAPE_ROT180  3U

/* LCD colors (ARGB8888) */
#define LCD_COLOR_BLUE          0xFF0000FFUL
#define LCD_COLOR_GREEN         0xFF00FF00UL
#define LCD_COLOR_RED           0xFFFF0000UL
#define LCD_COLOR_CYAN          0xFF00FFFFUL
#define LCD_COLOR_MAGENTA       0xFFFF00FFUL
#define LCD_COLOR_YELLOW        0xFFFFFF00UL
#define LCD_COLOR_LIGHTBLUE     0xFF8080FFUL
#define LCD_COLOR_LIGHTGREEN    0xFF80FF80UL
#define LCD_COLOR_LIGHTRED      0xFFFF8080UL
#define LCD_COLOR_LIGHTCYAN     0xFF80FFFFUL
#define LCD_COLOR_LIGHTMAGENTA  0xFFFF80FFUL
#define LCD_COLOR_LIGHTYELLOW   0xFFFFFF80UL
#define LCD_COLOR_DARKBLUE      0xFF000080UL
#define LCD_COLOR_DARKGREEN     0xFF008000UL
#define LCD_COLOR_DARKRED       0xFF800000UL
#define LCD_COLOR_DARKCYAN      0xFF008080UL
#define LCD_COLOR_DARKMAGENTA   0xFF800080UL
#define LCD_COLOR_DARKYELLOW    0xFF808000UL
#define LCD_COLOR_WHITE         0xFFFFFFFFUL
#define LCD_COLOR_LIGHTGRAY     0xFFD3D3D3UL
#define LCD_COLOR_GRAY          0xFF808080UL
#define LCD_COLOR_DARKGRAY      0xFF404040UL
#define LCD_COLOR_BLACK         0xFF000000UL
#define LCD_COLOR_BROWN         0xFFA52A2AUL
#define LCD_COLOR_ORANGE        0xFFFFA500UL

#define LCD_COLOR_BER1 0xAAAAAAAAUL
#define LCD_COLOR_BER2 0x33333333UL
#define LCD_COLOR_BER3 0xF0F0F0F0UL
#define LCD_COLOR_BER4 0x7F7F7F7FUL
#define LCD_COLOR_BER5 0x55555555UL
#define LCD_COLOR_BER6 0xCCCCCCCCUL
#define LCD_COLOR_BER7 0x0F0F0F0FUL
#define LCD_COLOR_BER8 0x80808080UL


/* LCD display enable pin */
#define DSI_POWERON_GPIO_PORT           GPIOI
#define DSI_POWERON_GPIO_PIN            GPIO_PIN_5
#define DSI_POWERON_GPIO_CLOCK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()

#define DSI_RESET_GPIO_PORT             GPIOD
#define DSI_RESET_GPIO_PIN              GPIO_PIN_5
#define DSI_RESET_GPIO_CLOCK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define VSYNC        1
#define VBP          12
#define VFP          50
#define VACT         481
#define HSYNC        2
#define HBP          1
#define HFP          1
#define HACT         480
#define LCD_WIDTH    480
#define LCD_HEIGHT   480

#define PIXEL_PER_LINE    768U
#define BYTE_PER_PIXEL    4U   /* ARGB8888 */
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LCD_Exported_Types LCD Exported Types
  * @{
  */

#if (USE_HAL_GFXMMU_REGISTER_CALLBACKS == 1)
typedef struct
{
  pGFXMMU_CallbackTypeDef  pMspGfxmmuInitCb;
  pGFXMMU_CallbackTypeDef  pMspGfxmmuDeInitCb;
} BSP_LCD_GFXMMU_Cb_t;
#endif /* (USE_HAL_GFXMMU_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)
typedef struct
{
  pLTDC_CallbackTypeDef  pMspLtdcInitCb;
  pLTDC_CallbackTypeDef  pMspLtdcDeInitCb;
} BSP_LCD_LTDC_Cb_t;
#endif /* (USE_HAL_LTDC_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pDSI_CallbackTypeDef  pMspDsiInitCb;
  pDSI_CallbackTypeDef  pMspDsiDeInitCb;
} BSP_LCD_DSI_Cb_t;
#endif /* (USE_HAL_DSI_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
typedef struct
{
  pDMA2D_CallbackTypeDef  pMspDma2dInitCb;
  pDMA2D_CallbackTypeDef  pMspDma2dDeInitCb;
} BSP_LCD_DMA2D_Cb_t;
#endif /* (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LCD_Exported_Variables LCD Exported Variables
  * @{
  */
extern GFXMMU_HandleTypeDef hlcd_gfxmmu;
extern LTDC_HandleTypeDef   hlcd_ltdc;
extern DSI_HandleTypeDef    hlcd_dsi;
extern DMA2D_HandleTypeDef  hlcd_dma2d;
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LCD_Exported_Functions LCD Exported Functions
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
int32_t  BSP_LCD_GetFormat(uint32_t Instance, uint32_t *Format);
int32_t  BSP_LCD_IsFrameBufferAvailable(uint32_t Instance);
int32_t  BSP_LCD_Refresh(uint32_t Instance);

#if (USE_HAL_GFXMMU_REGISTER_CALLBACKS == 1)
int32_t  BSP_LCD_GFXMMU_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t  BSP_LCD_GFXMMU_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_GFXMMU_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_GFXMMU_Init(GFXMMU_HandleTypeDef *hgfxmmu);

#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 1)
int32_t  BSP_LCD_LTDC_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t  BSP_LCD_LTDC_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_LTDC_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_LTDC_ClockConfig(LTDC_HandleTypeDef *hltdc);
HAL_StatusTypeDef MX_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, uint32_t LayerIndex);
HAL_StatusTypeDef MX_LTDC_Init(LTDC_HandleTypeDef *hltdc);

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 1)
int32_t  BSP_LCD_DSI_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t  BSP_LCD_DSI_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_DSI_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_DSI_Init(DSI_HandleTypeDef *hdsi);


#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
int32_t  BSP_LCD_DMA2D_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t  BSP_LCD_DMA2D_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_DMA2D_Cb_t *Callback);
#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d, uint32_t Mode, uint32_t OffLine);
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

#endif /* STM32U5x9J_DISCOVERY_LCD_H */
