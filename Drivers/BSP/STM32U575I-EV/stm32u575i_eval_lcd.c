/**
  ******************************************************************************
  * @file    stm32u575i_eval_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Liquid Crystal Display (LCD) module
  *          mounted on STM32U575I-EVAL evaluation board.
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
  @verbatim
  1. How To use this driver:
  --------------------------
     - This driver is used to drive indirectly an LCD TFT.
     - This driver supports the HX8347I LCD mounted on MB989 daughter board
     - The HX8347I component driver MUST be included with this driver.

  2. Driver description:
  ----------------------
    + Initialization steps:
       o Initialize the LCD using the BSP_LCD_Init() function. You can select
         display orientation with "Orientation" parameter (portrait, landscape,
         portrait with 180 degrees rotation or landscape with 180 degrees
         rotation).
       o Call BSP_LCD_GetXSize() and BSP_LCD_GetYsize() to get respectively
         width and height in pixels of LCD in the current orientation.
       o Call BSP_LCD_SetBrightness() and BSP_LCD_GetBrightness() to
         respectively set and get LCD brightness.
       o Call BSP_LCD_SetActiveLayer() to select the current active layer.
       o Call BSP_LCD_GetFormat() to get LCD pixel format supported.

    + Display on LCD:
       o Call BSP_LCD_DisplayOn() and BSP_LCD_DisplayOff() to respectively
         switch on and switch off the LCD display.
       o Call BSP_LCD_WritePixel() and BSP_LCD_ReadPixel() to respectively write
         and read a pixel.
       o Call BSP_LCD_DrawHLine() to draw a horizontal line.
       o Call BSP_LCD_DrawVLine() to draw a vertical line.
       o Call BSP_LCD_DrawBitmap() to draw a bitmap.
       o Call BSP_LCD_FillRect() to draw a rectangle.
       o Call BSP_LCD_FillRGBRect() to draw a rectangle with RGB buffer.

    + De-initialization steps:
       o De-initialize the LCD using the BSP_LCD_DeInit() function.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_lcd.h"
#include "stm32u575i_eval_io.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @defgroup STM32U575I_EVAL_LCD LCD
  * @{
  */

/** @defgroup STM32U575I_EVAL_LCD_Private_Defines LCD Private Defines
  * @{
  */
#define LCD_BACKLIGHT_GPIO_PORT           GPIOA
#define LCD_BACKLIGHT_GPIO_PIN            GPIO_PIN_8
#define LCD_BACKLIGHT_GPIO_CLOCK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define LCD_REGISTER_ADDR FMC_BANK1_4
#define LCD_DATA_ADDR     (FMC_BANK1_4 | 0x00800000UL)

#define LCD_FMC_ADDRESS   1U
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Private_Variables LCD Private Variables
  * @{
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
static uint32_t Lcd_IsSramMspCbValid[LCD_INSTANCES_NBR] = {0};
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_LCD_Exported_Variables
  * @{
  */
SRAM_HandleTypeDef hlcd_sram[LCD_INSTANCES_NBR] = {0};

void      *Lcd_CompObj[LCD_INSTANCES_NBR] = {NULL};
LCD_Drv_t *Lcd_Drv[LCD_INSTANCES_NBR] = {NULL};
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Private_FunctionPrototypes LCD Private Function Prototypes
  * @{
  */
static int32_t HX8347I_Probe(uint32_t Orientation);
static int32_t HX8347I_PowerUp(void);
static int32_t HX8347I_PowerDown(void);

static int32_t LCD_FMC_Init(void);
static int32_t LCD_FMC_DeInit(void);
static int32_t LCD_FMC_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint32_t Length);
static int32_t LCD_FMC_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint32_t Length);
static int32_t LCD_FMC_GetTick(void);
static void    FMC_MspInit(SRAM_HandleTypeDef *hSram);
static void    FMC_MspDeInit(SRAM_HandleTypeDef *hSram);
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Private_TypesDefinitions LCD Private TypesDefinitions
  * @{
  */
const LCD_UTILS_Drv_t LCD_Driver =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  BSP_LCD_GetXSize,
  BSP_LCD_GetYSize,
  BSP_LCD_SetActiveLayer,
  BSP_LCD_GetPixelFormat
};

/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_LCD_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the LCD.
  * @param  Instance LCD Instance.
  * @param  Orientation LCD_ORIENTATION_PORTRAIT, LCD_ORIENTATION_LANDSCAPE,
  *                     LCD_ORIENTATION_PORTRAIT_ROT180 or LCD_ORIENTATION_LANDSCAPE_ROT180.
  * @retval BSP status.
  */
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= LCD_INSTANCES_NBR) || (Orientation > LCD_ORIENTATION_LANDSCAPE_ROT180))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Power up LCD */
    if (HX8347I_PowerUp() != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Probe the LCD driver */
      if (HX8347I_Probe(Orientation) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  De-Initialize the LCD.
  * @param  Instance LCD Instance.
  * @retval BSP status.
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* De-Init the LCD driver */
    if (Lcd_Drv[Instance]->DeInit(Lcd_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Power down LCD */
    if (HX8347I_PowerDown() != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set the display on.
  * @param  Instance LCD Instance.
  * @retval BSP status.
  */
int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set display backlight on */
    HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_GPIO_PIN, GPIO_PIN_RESET);

    /* Set the display on */
    if (Lcd_Drv[Instance]->DisplayOn(Lcd_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set the display off.
  * @param  Instance LCD Instance.
  * @retval BSP status.
  */
int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Deactivate backlight */
    HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_GPIO_PIN, GPIO_PIN_SET);

    /* Set the display off */
    if (Lcd_Drv[Instance]->DisplayOff(Lcd_CompObj[Instance]) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set the display brightness.
  * @param  Instance LCD Instance.
  * @param  Brightness [00: Min (black), 100 Max].
  * @retval BSP status.
  */
int32_t BSP_LCD_SetBrightness(uint32_t Instance, uint32_t Brightness)
{
  int32_t status;

  if ((Instance >= LCD_INSTANCES_NBR) || (Brightness > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Feature not supported */
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return status;
}

/**
  * @brief  Get the display brightness.
  * @param  Instance LCD Instance.
  * @param  Brightness [00: Min (black), 100 Max].
  * @retval BSP status.
  */
int32_t BSP_LCD_GetBrightness(uint32_t Instance, uint32_t *Brightness)
{
  int32_t status;

  if ((Instance >= LCD_INSTANCES_NBR) || (Brightness == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Feature not supported */
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return status;
}

/**
  * @brief  Get the LCD X size.
  * @param  Instance LCD Instance.
  * @param  Xsize LCD X size.
  * @retval BSP status.
  */
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *Xsize)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= LCD_INSTANCES_NBR) || (Xsize == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Get the display Xsize */
    if (Lcd_Drv[Instance]->GetXSize(Lcd_CompObj[Instance], Xsize) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get the LCD Y size.
  * @param  Instance LCD Instance.
  * @param  Ysize LCD Y size.
  * @retval BSP status.
  */
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *Ysize)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= LCD_INSTANCES_NBR) || (Ysize == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Get the display Ysize */
    if (Lcd_Drv[Instance]->GetYSize(Lcd_CompObj[Instance], Ysize) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Set the LCD active layer.
  * @param  Instance LCD Instance.
  * @param  LayerIndex Active layer index.
  * @retval BSP status.
  */
int32_t BSP_LCD_SetActiveLayer(uint32_t Instance, uint32_t LayerIndex)
{
  int32_t status;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Nothing to do */
    UNUSED(LayerIndex);

    /* Feature not supported */
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return status;
}

/**
  * @brief  Draw a bitmap on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  pBmp Pointer to bitmap.
  * @retval BSP status.
  */
int32_t BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Draw the bitmap on LCD */
    if (Lcd_Drv[Instance]->DrawBitmap(Lcd_CompObj[Instance], Xpos, Ypos, pBmp) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Fill rectangle with RGB buffer.
  * @param  Instance LCD Instance.
  * @param  Xpos X position on LCD.
  * @param  Ypos Y position on LCD.
  * @param  pData Pointer on RGB pixels buffer.
  * @param  Width Width of the rectangle.
  * @param  Height Height of the rectangle.
  * @retval BSP status.
  */
int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width,
                            uint32_t Height)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Fill rectangle on LCD */
    if (Lcd_Drv[Instance]->FillRGBRect(Lcd_CompObj[Instance], Xpos, Ypos, pData, Width, Height) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Draw a horizontal line on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Length Length of the line.
  * @param  Color Color of the line.
  * @retval BSP status.
  */
int32_t BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Draw the horizontal line on LCD */
    if (Lcd_Drv[Instance]->DrawHLine(Lcd_CompObj[Instance], Xpos, Ypos, Length, Color) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Draw a vertical line on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Length Length of the line.
  * @param  Color Color of the line.
  * @retval BSP status.
  */
int32_t BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Draw the vertical line on LCD */
    if (Lcd_Drv[Instance]->DrawVLine(Lcd_CompObj[Instance], Xpos, Ypos, Length, Color) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Draw and fill a rectangle on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Width Width of the rectangle.
  * @param  Height Height of the rectangle.
  * @param  Color Color of the rectangle.
  * @retval BSP status.
  */
int32_t BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height,
                         uint32_t Color)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Draw and fill rectangle on LCD */
    if (Lcd_Drv[Instance]->FillRect(Lcd_CompObj[Instance], Xpos, Ypos, Width, Height, Color) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Read a pixel on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Color Pointer to the pixel.
  * @retval BSP status.
  */
int32_t BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Read pixel on LCD */
    if (Lcd_Drv[Instance]->GetPixel(Lcd_CompObj[Instance], Xpos, Ypos, Color) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Write a pixel on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  Color Pixel.
  * @retval BSP status.
  */
int32_t BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Write pixel on LCD */
    if (Lcd_Drv[Instance]->SetPixel(Lcd_CompObj[Instance], Xpos, Ypos, Color) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  Get pixel format supported by LCD.
  * @param  Instance LCD Instance.
  * @param  Format Pointer on pixel format.
  * @retval BSP status.
  */
int32_t BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *Format)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Get pixel format supported by LCD */
    *Format = LCD_PIXEL_FORMAT_RGB565;
  }

  return status;
}

/**
  * @brief  MX FMC BANK4 initialization.
  * @param  hSram SRAM handle.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_FMC_BANK4_Init(SRAM_HandleTypeDef *hSram)
{
  static FMC_NORSRAM_TimingTypeDef  SramTiming = {0};

  /* SRAM device configuration */
  hSram->Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
  hSram->Init.MemoryType             = FMC_MEMORY_TYPE_SRAM;
  hSram->Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hSram->Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
  hSram->Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hSram->Init.WaitSignalActive       = FMC_WAIT_TIMING_BEFORE_WS;
  hSram->Init.WriteOperation         = FMC_WRITE_OPERATION_ENABLE;
  hSram->Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
  hSram->Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
  hSram->Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hSram->Init.WriteBurst             = FMC_WRITE_BURST_DISABLE;
  hSram->Init.ContinuousClock        = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
  hSram->Init.WriteFifo              = FMC_WRITE_FIFO_DISABLE;
  hSram->Init.PageSize               = FMC_PAGE_SIZE_NONE;
  hSram->Init.NBLSetupTime           = FMC_NBL_SETUPTIME_0;
  hSram->Init.MaxChipSelectPulse     = DISABLE;
  hSram->Init.MaxChipSelectPulseTime = 1;

  SramTiming.AddressSetupTime      = 10;
  SramTiming.AddressHoldTime       = 10;
  SramTiming.DataSetupTime         = 10;
  SramTiming.DataHoldTime          = 3;
  SramTiming.BusTurnAroundDuration = 1;
  SramTiming.CLKDivision           = 2;
  SramTiming.DataLatency           = 2;
  SramTiming.AccessMode            = FMC_ACCESS_MODE_A;

  return HAL_SRAM_Init(hSram, &SramTiming, &SramTiming);
}

#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
/**
  * @brief  Register Default LCD Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_LCD_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_SRAM_RESET_HANDLE_STATE(&hlcd_sram[Instance]);

    /* Register default MspInit/MspDeInit Callback */
    if (HAL_SRAM_RegisterCallback(&hlcd_sram[Instance], HAL_SRAM_MSP_INIT_CB_ID, FMC_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SRAM_RegisterCallback(&hlcd_sram[Instance], HAL_SRAM_MSP_DEINIT_CB_ID, FMC_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Lcd_IsSramMspCbValid[Instance] = 1U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief Register LCD Msp Callback registering
  * @param Callbacks pointer to LCD MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_LCD_RegisterMspCallbacks(uint32_t Instance, BSP_LCD_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_SRAM_RESET_HANDLE_STATE(&hlcd_sram[Instance]);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_SRAM_RegisterCallback(&hlcd_sram[Instance], HAL_SRAM_MSP_INIT_CB_ID, Callback->pMspFmcInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SRAM_RegisterCallback(&hlcd_sram[Instance], HAL_SRAM_MSP_DEINIT_CB_ID,
                                       Callback->pMspFmcDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Lcd_IsSramMspCbValid[Instance] = 1U;
    }
  }

  /* BSP status */
  return ret;
}
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LCD_Private_Functions LCD Private Functions
  * @{
  */
/**
  * @brief  Probe the HX8347I LCD driver.
  * @param  Orientation LCD_ORIENTATION_PORTRAIT or LCD_ORIENTATION_LANDSCAPE.
  * @retval BSP status.
  */
static int32_t HX8347I_Probe(uint32_t Orientation)
{
  int32_t                  status;
  HX8347I_IO_t             IOCtx;
  uint32_t                 hx8347i_id;
  static HX8347I_Object_t  HX8347IObj;
  uint32_t                 lcd_orientation;

  /* Configure the LCD driver */
  IOCtx.Address     = LCD_FMC_ADDRESS;
  IOCtx.Init        = LCD_FMC_Init;
  IOCtx.DeInit      = LCD_FMC_DeInit;
  IOCtx.ReadReg     = LCD_FMC_ReadReg16;
  IOCtx.WriteReg    = LCD_FMC_WriteReg16;
  IOCtx.GetTick     = LCD_FMC_GetTick;

  if (HX8347I_RegisterBusIO(&HX8347IObj, &IOCtx) != HX8347I_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  else if (HX8347I_ReadID(&HX8347IObj, &hx8347i_id) != HX8347I_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (hx8347i_id != HX8347I_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Lcd_CompObj[0] = &HX8347IObj;
    Lcd_Drv[0] = (LCD_Drv_t *) &HX8347I_Driver;
    if (Orientation == LCD_ORIENTATION_PORTRAIT)
    {
      lcd_orientation = HX8347I_ORIENTATION_PORTRAIT;
    }
    else if (Orientation == LCD_ORIENTATION_LANDSCAPE)
    {
      lcd_orientation = HX8347I_ORIENTATION_LANDSCAPE;
    }
    else if (Orientation == LCD_ORIENTATION_PORTRAIT_ROT180)
    {
      lcd_orientation = HX8347I_ORIENTATION_PORTRAIT_ROT180;
    }
    else
    {
      lcd_orientation = HX8347I_ORIENTATION_LANDSCAPE_ROT180;
    }
    if (Lcd_Drv[0]->Init(Lcd_CompObj[0], HX8347I_FORMAT_RBG565, lcd_orientation) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      status = BSP_ERROR_NONE;
    }
  }

  return status;
}

/**
  * @brief  Un-reset HX8347I and activate backlight.
  * @retval BSP status.
  */
static int32_t HX8347I_PowerUp(void)
{
  int32_t status;

#if (USE_BSP_IO_CLASS > 0)
  BSP_IO_Init_t    IO_Init;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Initialize the BSP IO driver and configure the HX8347I reset pin */
  IO_Init.Pin  = IO_PIN_12;
  IO_Init.Mode = IO_MODE_OUTPUT_OD;
  IO_Init.Pull = IO_NOPULL;
  status = BSP_IO_Init(0, &IO_Init);
  if (status == BSP_ERROR_NONE)
  {
    /* Un-reset the HX8347I */
    status = BSP_IO_WritePin(0, IO_PIN_12, IO_PIN_SET);
    if (status == BSP_ERROR_NONE)
    {
      HAL_Delay(5);
      /* Reset the HX8347I */
      status = BSP_IO_WritePin(0, IO_PIN_12, IO_PIN_RESET);
      if (status == BSP_ERROR_NONE)
      {
        HAL_Delay(5);
        /* Un-reset the HX8347I */
        status = BSP_IO_WritePin(0, IO_PIN_12, IO_PIN_SET);
        if (status == BSP_ERROR_NONE)
        {
          /* Initialize GPIO for backlight control and enable backlight */
          LCD_BACKLIGHT_GPIO_CLOCK_ENABLE();
          GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
          GPIO_InitStruct.Pull      = GPIO_PULLUP;
          GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM ;
          GPIO_InitStruct.Pin       = LCD_BACKLIGHT_GPIO_PIN;
          HAL_GPIO_Init(LCD_BACKLIGHT_GPIO_PORT, &GPIO_InitStruct);
          HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_GPIO_PIN, GPIO_PIN_RESET);

          /* Wait 120ms according HX8347I datasheet */
          HAL_Delay(120);
        }
      }
    }
  }
#else /* USE_BSP_IO_CLASS > 0 */
  status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* USE_BSP_IO_CLASS > 0 */

  return status;
}

/**
  * @brief  Reset HX8347I and deactivate backlight.
  * @retval BSP status.
  */
static int32_t HX8347I_PowerDown(void)
{
  int32_t status;

#if (USE_BSP_IO_CLASS > 0)
  /* Deactivate backlight */
  HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_GPIO_PIN, GPIO_PIN_SET);

  /* Reset the HX8347I */
  status = BSP_IO_WritePin(0, IO_PIN_12, IO_PIN_RESET);
  if (status == BSP_ERROR_NONE)
  {
    /* De-initialize the BSP IO driver */
    status = BSP_IO_DeInit(0);
  }
#else /* USE_BSP_IO_CLASS > 0 */
  status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* USE_BSP_IO_CLASS > 0 */

  return status;
}

/**
  * @brief  Initialize FMC.
  * @retval BSP status.
  */
static int32_t LCD_FMC_Init(void)
{
  int32_t status = BSP_ERROR_NONE;

  hlcd_sram[0].Instance    = FMC_NORSRAM_DEVICE;
  hlcd_sram[0].Extended    = FMC_NORSRAM_EXTENDED_DEVICE;
  hlcd_sram[0].Init.NSBank = FMC_NORSRAM_BANK4;

  if (HAL_SRAM_GetState(&hlcd_sram[0]) == HAL_SRAM_STATE_RESET)
  {
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 0)
    /* Init the FMC Msp */
    FMC_MspInit(&hlcd_sram[0]);

    if (MX_FMC_BANK4_Init(&hlcd_sram[0]) != HAL_OK)
    {
      status = BSP_ERROR_BUS_FAILURE;
    }
#else
    if (Lcd_IsSramMspCbValid[0] == 0U)
    {
      if (BSP_LCD_RegisterDefaultMspCallbacks(0) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_MSP_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      if (MX_FMC_BANK4_Init(&hlcd_sram[0]) != HAL_OK)
      {
        status = BSP_ERROR_BUS_FAILURE;
      }
    }
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */
  }
  return status;
}

/**
  * @brief  DeInitialize BSP FMC.
  * @retval BSP status.
  */
static int32_t LCD_FMC_DeInit(void)
{
  int32_t status = BSP_ERROR_NONE;

#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 0)
  FMC_MspDeInit(&hlcd_sram[0]);
#endif /* USE_HAL_SRAM_REGISTER_CALLBACKS */

  /* De-Init the FMC */
  if (HAL_SRAM_DeInit(&hlcd_sram[0]) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }

  return status;
}

/**
  * @brief  Write 16bit values in registers of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg     The target register start address to write.
  * @param  pData   Pointer to data buffer.
  * @param  Length  Number of data.
  * @retval BSP status.
  */
static int32_t LCD_FMC_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint32_t Length)
{
  int32_t  ret = BSP_ERROR_NONE;
  uint32_t i = 0;

  if ((DevAddr != LCD_FMC_ADDRESS) || (pData == NULL) || (Length == 0U))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Write register address */
    *(uint16_t *)LCD_REGISTER_ADDR = Reg;
    while (i < (2U * Length))
    {
      /* Write register value */
      *(uint16_t *)LCD_DATA_ADDR = (((uint16_t)pData[i + 1U] << 8U) & 0xFF00U) | ((uint16_t)pData[i] & 0x00FFU);
      /* Update data pointer */
      i += 2U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief  Read 16bit values in registers of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg     The target register start address to read.
  * @param  pData   Pointer to data buffer.
  * @param  Length  Number of data.
  * @retval BSP status.
  */
static int32_t LCD_FMC_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint32_t Length)
{
  int32_t  ret = BSP_ERROR_NONE;
  uint32_t i = 0;
  uint16_t tmp;

  if ((DevAddr != LCD_FMC_ADDRESS) || (pData == NULL) || (Length == 0U))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Write register address */
    *(uint16_t *)LCD_REGISTER_ADDR = Reg;
    while (i < (2U * Length))
    {
      tmp = *(uint16_t *)LCD_DATA_ADDR;
      pData[i]    = (uint8_t) tmp;
      pData[i + 1U] = (uint8_t)(tmp >> 8U);
      /* Update data pointer */
      i += 2U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief  Provide a tick value in millisecond.
  * @retval Tick value.
  */
static int32_t LCD_FMC_GetTick(void)
{
  uint32_t ret;
  ret = HAL_GetTick();
  return (int32_t)ret;
}

/**
  * @brief  Initializes FMC MSP.
  * @param  hSram : SRAM handler
  * @retval None
  */
static void  FMC_MspInit(SRAM_HandleTypeDef *hSram)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hSram);

  /*** Configure the GPIOs ***/

  /* Enable VddIO2 for GPIOG */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_MEDIUM ;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  gpio_init_structure.Pull      = GPIO_NOPULL;

  /* NE configuration */
  /* NE4 : LCD */
  gpio_init_structure.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* NOE and NWE configuration */
  gpio_init_structure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* RS configuration */
  gpio_init_structure.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* Data Bus */
  gpio_init_structure.Pull      = GPIO_NOPULL;
  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 | \
                            GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */
  gpio_init_structure.Pin = GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | \
                            GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | \
                            GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
}

/**
  * @brief  DeInitializes FMC MSP.
  * @param  hSram : SRAM handler
  * @retval None
  */
static void FMC_MspDeInit(SRAM_HandleTypeDef *hSram)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hSram);

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5  | \
                              GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_14 | \
                              GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOD, gpio_init_structure.Pin);

  /* GPIOE configuration */
  gpio_init_structure.Pin   = GPIO_PIN_6  | GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | \
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
                              GPIO_PIN_14 | GPIO_PIN_15;
  \

  HAL_GPIO_DeInit(GPIOE, gpio_init_structure.Pin);

  /* GPIOG configuration */
  gpio_init_structure.Pin   = GPIO_PIN_12;
  HAL_GPIO_DeInit(GPIOG, gpio_init_structure.Pin);

  /* Disable FMC clock */
  __HAL_RCC_FMC_CLK_DISABLE();
}
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
