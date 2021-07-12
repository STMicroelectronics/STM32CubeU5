/**
  ******************************************************************************
  * @file    stm32u575i_eval_camera.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Camera modules mounted on
  *          STM32U575I_EVAL board.
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

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The OV5640 component driver MUST be included with this driver.

2. Driver description:
---------------------
     o Initialize the camera instance using the BSP_CAMERA_Init() function with the required
       Resolution and Pixel format where:
       - Instance: Is the physical camera interface and is always 0 on this board.
       - Resolution: The camera resolution
       - PixelFormat: The camera Pixel format

     o DeInitialize the camera instance using the BSP_CAMERA_DeInit() . This
       function will firstly stop the camera to insure the data transfer complete.
       Then de-initializes the dcmi peripheral.

     o Get the camera instance capabilities using the BSP_CAMERA_GetCapabilities().
       This function must be called after the BSP_CAMERA_Init() to get the sensor capabilities

     o Start the camera using the CAMERA_Start() function by specifying the capture Mode:
       - CAMERA_MODE_CONTINUOUS: For continuous capture
       - CAMERA_MODE_SNAPSHOT  : For on shot capture

     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()

     o Call BSP_CAMERA_SetResolution()/BSP_CAMERA_GetResolution() to set/get the camera resolution
       Resolution: - CAMERA_R160x120
                   - CAMERA_R320x240
                   - CAMERA_R480x272
                   - CAMERA_R640x480
                   - CAMERA_R800x480

     o Call BSP_CAMERA_SetPixelFormat()/BSP_CAMERA_GetPixelFormat() to set/get the camera pixel format
       PixelFormat: - CAMERA_PF_RGB565
                    - CAMERA_PF_RGB888
                    - CAMERA_PF_YUV422

     o Call BSP_CAMERA_SetLightMode()/BSP_CAMERA_GetLightMode() to set/get the camera light mode
       LightMode: - CAMERA_LIGHT_AUTO
                  - CAMERA_LIGHT_SUNNY
                  - CAMERA_LIGHT_OFFICE
                  - CAMERA_LIGHT_HOME
                  - CAMERA_LIGHT_CLOUDY

     o Call BSP_CAMERA_SetColorEffect()/BSP_CAMERA_GetColorEffect() to set/get the camera color effects
       Effect: - CAMERA_COLOR_EFFECT_NONE
               - CAMERA_COLOR_EFFECT_BLUE
               - CAMERA_COLOR_EFFECT_RED
               - CAMERA_COLOR_EFFECT_GREEN
               - CAMERA_COLOR_EFFECT_BW
               - CAMERA_COLOR_EFFECT_SEPIA
               - CAMERA_COLOR_EFFECT_NEGATIVE

     o Call BSP_CAMERA_SetBrightness()/BSP_CAMERA_GetBrightness() to set/get the camera Brightness
       Brightness is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetSaturation()/BSP_CAMERA_GetSaturation() to set/get the camera Saturation
       Saturation is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetContrast()/BSP_CAMERA_GetContrast() to set/get the camera Contrast
       Contrast is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetHueDegree()/BSP_CAMERA_GetHueDegree() to set/get the camera Hue Degree
       HueDegree is value between -4(180 degree negative) and 4(150 degree positive).

     o Call BSP_CAMERA_SetMirrorFlip()/BSP_CAMERA_GetMirrorFlip() to set/get the camera mirror and flip
       MirrorFlip could be any combination of: - CAMERA_MIRRORFLIP_NONE
                                               - CAMERA_MIRRORFLIP_FLIP
                                               - CAMERA_MIRRORFLIP_MIRROR

     o Call BSP_CAMERA_SetZoom()/BSP_CAMERA_GetZoom() to set/get the camera zooming
       Zoom could be any value of:
       - CAMERA_ZOOM_x8 for CAMERA_R160x120 resolution only
       - CAMERA_ZOOM_x4 For all resolutions except CAMERA_R640x480 and CAMERA_R800x480
       - CAMERA_ZOOM_x2 For all resolutions except CAMERA_R800x480
       - CAMERA_ZOOM_x1 For all resolutions

     o Call BSP_CAMERA_EnableNightMode() to enable night mode.

     o Call BSP_CAMERA_DisableNightMode() to disable night mode.

     o Call BSP_CAMERA_RegisterDefaultMspCallbacks() to register Msp default callbacks
     o Call BSP_CAMERA_RegisterMspCallbacks() to register application Msp callbacks.

     o Error, line event, vsync event and frame event are handled through dedicated weak
      callbacks that can be override at application level: BSP_CAMERA_LineEventCallback(),
      BSP_CAMERA_FrameEventCallback(), BSP_CAMERA_VsyncEventCallback(), BSP_CAMERA_ErrorCallback()
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_camera.h"
#include "stm32u575i_eval_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_CAMERA
  * @{
  */

/** @defgroup STM32U575I_EVAL_CAMERA_Exported_Variables CAMERA Exported Variables
  * @{
  */
void                *Camera_CompObj = NULL;
DCMI_HandleTypeDef  hcamera_dcmi;
CAMERA_Ctx_t        Camera_Ctx[CAMERA_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_CAMERA_Private_Variables CAMERA Private Variables
  * @{
  */
static CAMERA_Drv_t *Camera_Drv = NULL;
static CAMERA_Capabilities_t Camera_Cap;
static DMA_HandleTypeDef hdma_handler;

static DMA_QListTypeDef  DCMIQueue;

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_CAMERA_Private_FunctionsPrototypes CAMERA Private Functions Prototypes
  * @{
  */
static int32_t GetSize(uint32_t Resolution, uint32_t PixelFormat);
static void DCMI_MspInit(DCMI_HandleTypeDef *hdcmi);
static void DCMI_MspDeInit(DCMI_HandleTypeDef *hdcmi);
#if (USE_HAL_DCMI_REGISTER_CALLBACKS > 0)
static void DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi);
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS > 0) */

static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat);

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_CAMERA_Exported_Functions CAMERA Exported Functions
  * @{
  */

/**
  * @brief  Initializes the camera.
  * @param  Instance    Camera instance.
  * @param  Resolution  Camera sensor requested resolution (x, y) : standard resolution
  *         naming QQVGA, QVGA, VGA ...
  * @param  PixelFormat Capture pixel format
  * @retval BSP status
  */
int32_t BSP_CAMERA_Init(uint32_t Instance, uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef gpio_init_structure;
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set DCMI instance */
    hcamera_dcmi.Instance = DCMI;

    /* DCMI Initialization */
#if (USE_HAL_DCMI_REGISTER_CALLBACKS > 0)
    /* Register the DCMI MSP Callbacks */
    if (Camera_Ctx[Instance].IsMspCallbacksValid == 0U)
    {
      if (BSP_CAMERA_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#else
    /* DCMI Initialization */
    DCMI_MspInit(&hcamera_dcmi);
#endif /* USE_HAL_DCMI_REGISTER_CALLBACKS */
    /* Initialize the camera driver structure */
    if (MX_DCMI_Init(&hcamera_dcmi) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (BSP_CAMERA_HwReset(0) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      /* Enable VddIO2 for Port G */
      __HAL_RCC_PWR_CLK_ENABLE();
      HAL_PWREx_EnableVddIO2();
      __HAL_RCC_PWR_CLK_DISABLE();

      /* PLUG Pin configuration */
      gpio_init_structure.Pin       = CAM_PLUG_PIN;
      gpio_init_structure.Pull      = GPIO_PULLUP;
      gpio_init_structure.Mode      = GPIO_MODE_INPUT;
      HAL_GPIO_Init(CAM_PLUG_PORT, &gpio_init_structure);

      if (((uint32_t)HAL_GPIO_ReadPin(CAM_PLUG_PORT, CAM_PLUG_PIN) & CAM_PLUG_PIN) == CAM_PLUG_PIN)
      {
        ret = BSP_ERROR_UNKNOWN_COMPONENT;
      }
      else
      {
        /* Read ID of Camera module via I2C */
        if (OV5640_Probe(Resolution, PixelFormat) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_UNKNOWN_COMPONENT;
        }
        else
        {
#if (USE_HAL_DCMI_REGISTER_CALLBACKS > 0)
          /* Register DCMI LineEvent, FrameEvent and Error callbacks */
          if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_LINE_EVENT_CB_ID, DCMI_LineEventCallback) != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_FRAME_EVENT_CB_ID, DCMI_FrameEventCallback)
                   != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_VSYNC_EVENT_CB_ID, DCMI_VsyncEventCallback)
                   != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_ERROR_CB_ID, DCMI_ErrorCallback) != HAL_OK)
          {
            ret = BSP_ERROR_PERIPH_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS > 0) */
          Camera_Ctx[Instance].Resolution = Resolution;
          Camera_Ctx[Instance].PixelFormat = PixelFormat;
        }
      }
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief  DeInitializes the camera.
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hcamera_dcmi.Instance = DCMI;

    /* First stop the camera to insure all data are transferred */
    if (BSP_CAMERA_Stop(Instance) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_DCMI_DeInit(&hcamera_dcmi) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
#if (USE_HAL_DCMI_REGISTER_CALLBACKS == 0)
      DCMI_MspDeInit(&hcamera_dcmi);
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS == 0) */

      /* De-initialize the camera module */
      if (Camera_Drv->DeInit(Camera_CompObj) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }

      /* Set Camera in Power Down */
      else if (BSP_CAMERA_PwrDown(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Initializes the DCMI peripheral
  * @param  hdcmi  DCMI handle
  * @note   Being __weak it can be overwritten by the application
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_DCMI_Init(DCMI_HandleTypeDef *hdcmi)
{
  /*** Configures the DCMI to interface with the camera module ***/
  /* DCMI configuration */
  hdcmi->Instance              = DCMI;
  hdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;
  hdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_HIGH;
  hdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  hdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_HIGH;
  hdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;

  if (HAL_DCMI_Init(hdcmi) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

#if (USE_HAL_DCMI_REGISTER_CALLBACKS > 0)
/**
  * @brief Default BSP CAMERA Msp Callbacks
  * @param Instance CAMERA Instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_DCMI_RESET_HANDLE_STATE(&hcamera_dcmi);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_MSPINIT_CB_ID, DCMI_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_MSPDEINIT_CB_ID, DCMI_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Camera_Ctx[Instance].IsMspCallbacksValid = 1;
    }
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP CAMERA Msp Callback registering
  * @param Instance     CAMERA Instance.
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_CAMERA_RegisterMspCallbacks(uint32_t Instance, BSP_CAMERA_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_DCMI_RESET_HANDLE_STATE(&hcamera_dcmi);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_MSPINIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_DCMI_RegisterCallback(&hcamera_dcmi, HAL_DCMI_MSPDEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Camera_Ctx[Instance].IsMspCallbacksValid = 1;
    }
  }
  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS > 0) */

/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  Instance Camera instance.
  * @param  pBff     pointer to the camera output buffer
  * @param  Mode CAMERA_MODE_CONTINUOUS or CAMERA_MODE_SNAPSHOT
  * @retval BSP status
  */
int32_t BSP_CAMERA_Start(uint32_t Instance, uint8_t *pBff, uint32_t Mode)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HAL_DCMI_Start_DMA(&hcamera_dcmi, Mode, (uint32_t)pBff, (uint32_t)GetSize(Camera_Ctx[Instance].Resolution,
                              Camera_Ctx[Instance].PixelFormat)) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Stop the CAMERA capture
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_Stop(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HAL_DCMI_Stop(&hcamera_dcmi) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief Suspend the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Suspend(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HAL_DCMI_Suspend(&hcamera_dcmi) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief Resume the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Resume(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (HAL_DCMI_Resume(&hcamera_dcmi) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the Camera Capabilities.
  * @param  Instance  Camera instance.
  * @param  Capabilities  pointer to camera Capabilities
  * @note   This function should be called after the init. This to get Capabilities
  *         from the camera sensor OV5640
  * @retval Component status
  */
int32_t BSP_CAMERA_GetCapabilities(uint32_t Instance, CAMERA_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Drv->GetCapabilities(Camera_CompObj, Capabilities) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetPixelFormat(uint32_t Instance, uint32_t PixelFormat)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Drv->SetPixelFormat(Camera_CompObj, PixelFormat) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].PixelFormat = PixelFormat;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    *PixelFormat = Camera_Ctx[Instance].PixelFormat;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}


/**
  * @brief  Set the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetResolution(uint32_t Instance, uint32_t Resolution)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Resolution == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetResolution(Camera_CompObj, Resolution) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].Resolution = Resolution;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetResolution(uint32_t Instance, uint32_t *Resolution)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Resolution == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Resolution = Camera_Ctx[Instance].Resolution;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetLightMode(uint32_t Instance, uint32_t LightMode)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.LightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetLightMode(Camera_CompObj, LightMode) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].LightMode = LightMode;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetLightMode(uint32_t Instance, uint32_t *LightMode)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.LightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *LightMode = Camera_Ctx[Instance].LightMode;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetColorEffect(uint32_t Instance, uint32_t ColorEffect)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.ColorEffect == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetColorEffect(Camera_CompObj, ColorEffect) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].ColorEffect = ColorEffect;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetColorEffect(uint32_t Instance, uint32_t *ColorEffect)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.ColorEffect == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *ColorEffect = Camera_Ctx[Instance].ColorEffect;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Brightness Level.
  * @param  Instance   Camera instance.
  * @param  Brightness Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetBrightness(uint32_t Instance, int32_t Brightness)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Brightness < CAMERA_BRIGHTNESS_MIN)
                                             && (Brightness > CAMERA_BRIGHTNESS_MAX)))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Brightness == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetBrightness(Camera_CompObj, Brightness) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].Brightness = Brightness;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Brightness Level.
  * @param  Instance Camera instance.
  * @param  Brightness  Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetBrightness(uint32_t Instance, int32_t *Brightness)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Brightness == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Brightness = Camera_Ctx[Instance].Brightness;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetSaturation(uint32_t Instance, int32_t Saturation)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Saturation < CAMERA_SATURATION_MIN)
                                             && (Saturation > CAMERA_SATURATION_MAX)))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Saturation == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetSaturation(Camera_CompObj, Saturation)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].Saturation = Saturation;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetSaturation(uint32_t Instance, int32_t *Saturation)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Saturation == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Saturation = Camera_Ctx[Instance].Saturation;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Contrast Contrast Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetContrast(uint32_t Instance, int32_t Contrast)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Contrast < CAMERA_CONTRAST_MIN) && (Contrast > CAMERA_CONTRAST_MAX)))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Contrast == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetContrast(Camera_CompObj, Contrast)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].Contrast = Contrast;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Contrast Contrast Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetContrast(uint32_t Instance, int32_t *Contrast)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Contrast == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Contrast = Camera_Ctx[Instance].Contrast;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetHueDegree(uint32_t Instance, int32_t HueDegree)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((HueDegree < CAMERA_HUEDEGREE_MIN) && (HueDegree > CAMERA_HUEDEGREE_MAX)))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.HueDegree == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->SetHueDegree(Camera_CompObj, HueDegree) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].HueDegree = HueDegree;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetHueDegree(uint32_t Instance, int32_t *HueDegree)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.HueDegree == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *HueDegree = Camera_Ctx[Instance].HueDegree;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Mirror/Flip.
  * @param  Instance  Camera instance.
  * @param  MirrorFlip CAMERA_MIRRORFLIP_NONE or any combination of
  *                    CAMERA_MIRRORFLIP_FLIP and CAMERA_MIRRORFLIP_MIRROR
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetMirrorFlip(uint32_t Instance, uint32_t MirrorFlip)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.MirrorFlip == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->MirrorFlipConfig(Camera_CompObj, MirrorFlip)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].MirrorFlip = MirrorFlip;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Mirror/Flip.
  * @param  Instance   Camera instance.
  * @param  MirrorFlip Mirror/Flip config
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetMirrorFlip(uint32_t Instance, uint32_t *MirrorFlip)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.MirrorFlip == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *MirrorFlip = Camera_Ctx[Instance].MirrorFlip;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetZoom(uint32_t Instance, uint32_t Zoom)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Zoom == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->ZoomConfig(Camera_CompObj, Zoom) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Camera_Ctx[Instance].Zoom = Zoom;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetZoom(uint32_t Instance, uint32_t *Zoom)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.Zoom == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Zoom = Camera_Ctx[Instance].Zoom;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Enable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_EnableNightMode(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.NightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->NightModeConfig(Camera_CompObj, CAMERA_NIGHT_MODE_SET) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Disable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DisableNightMode(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (Camera_Cap.NightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Camera_Drv->NightModeConfig(Camera_CompObj, CAMERA_NIGHT_MODE_RESET) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  CAMERA hardware reset
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_HwReset(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef gpio_init_structure;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    gpio_init_structure.Pin  = RSTI_PIN | XSDN_PIN;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Alternate  = GPIO_AF10_DCMI;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOI, &gpio_init_structure);

    /* Camera sensor RESET sequence */
    /* Assert the camera STANDBY and RSTI pins */
    HAL_GPIO_WritePin(XSDN_PORT, XSDN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RSTI_PORT, RSTI_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);   /* RST and XSDN signals asserted during 100ms */

    /* De-assert the camera STANDBY pin (active high) */
    HAL_GPIO_WritePin(XSDN_PORT, XSDN_PIN, GPIO_PIN_RESET);
    HAL_Delay(3);     /* RST de-asserted and XSDN de-asserted during 3ms */

    /* De-assert the camera RSTI pin (active low) */
    HAL_GPIO_WritePin(RSTI_PORT, RSTI_PIN, GPIO_PIN_SET);
    HAL_Delay(20);     /* RST de-asserted during 20ms */
  }

  return ret;
}

/**
  * @brief  CAMERA power down
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_PwrDown(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef gpio_init_structure;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    gpio_init_structure.Pin  = RSTI_PIN | XSDN_PIN;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Alternate  = GPIO_AF10_DCMI;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOI, &gpio_init_structure);

    /* Camera power down sequence */
    /* De-assert the camera STANDBY pin (active high) */
    HAL_GPIO_WritePin(XSDN_PORT, XSDN_PIN, GPIO_PIN_RESET);

    /* Assert the camera RSTI pin (active low) */
    HAL_GPIO_WritePin(RSTI_PORT, RSTI_PIN, GPIO_PIN_RESET);
  }

  return ret;
}

/**
  * @brief  This function handles DCMI interrupt request.
  * @param  Instance Camera instance
  * @retval None
  */
void BSP_CAMERA_IRQHandler(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  HAL_DCMI_IRQHandler(&hcamera_dcmi);
}

/**
  * @brief  This function handles DCMI DMA interrupt request.
  * @param  Instance Camera instance
  * @retval None
  */
void BSP_CAMERA_DMA_IRQHandler(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  HAL_DMA_IRQHandler(hcamera_dcmi.DMA_Handle);
}

/**
  * @brief  Line Event callback.
  * @param  Instance Camera instance.
  * @retval None
  */
__weak void BSP_CAMERA_LineEventCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_LineEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Frame Event callback.
  * @param  Instance Camera instance.
  * @retval None
  */
__weak void BSP_CAMERA_FrameEventCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Vsync Event callback.
  * @param  Instance Camera instance.
  * @retval None
  */
__weak void BSP_CAMERA_VsyncEventCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Error callback.
  * @param  Instance Camera instance.
  * @retval None
  */
__weak void BSP_CAMERA_ErrorCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_ErrorCallback could be implemented in the user file
   */
}

#if (USE_HAL_DCMI_REGISTER_CALLBACKS == 0) || !defined(USE_HAL_DCMI_REGISTER_CALLBACKS)
/**
  * @brief  Line event callback
  * @param  hdcmi  pointer to the DCMI handle
  * @retval None
  */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_LineEventCallback(0);
}

/**
  * @brief  Frame event callback
  * @param  hdcmi pointer to the DCMI handle
  * @retval None
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_FrameEventCallback(0);
}

/**
  * @brief  Vsync event callback
  * @param  hdcmi pointer to the DCMI handle
  * @retval None
  */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_VsyncEventCallback(0);
}

/**
  * @brief  Error callback
  * @param  hdcmi pointer to the DCMI handle
  * @retval None
  */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_ErrorCallback(0);
}
#endif /*(USE_HAL_DCMI_REGISTER_CALLBACKS == 0) || !defined(USE_HAL_DCMI_REGISTER_CALLBACKS) */

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_CAMERA_Private_Functions CAMERA Private Functions
  * @{
  */

/**
  * @brief  Get the capture size in pixels unit.
  * @param  Resolution  the current resolution.
  * @param  PixelFormat Pixel format
  * @retval capture size in pixels unit.
  */
static int32_t GetSize(uint32_t Resolution, uint32_t PixelFormat)
{
  uint32_t size = 0;
  uint32_t pf_div;
  if (PixelFormat == CAMERA_PF_RGB888)
  {
    pf_div = 3; /* each pixel on 3 bytes so 3/4 words */
  }
  else
  {
    pf_div = 2; /* each pixel on 2 bytes so 1/2 words*/
  }
  /* Get capture size */
  switch (Resolution)
  {
    case CAMERA_R160x120:
      size = ((uint32_t)(160 * 120) * pf_div) / 4U;
      break;
    case CAMERA_R320x240:
      size = ((uint32_t)(320 * 240) * pf_div) / 4U;
      break;
    case CAMERA_R480x272:
      size = ((uint32_t)(480 * 272) * pf_div) / 4U;
      break;
    case CAMERA_R640x480:
      size = ((uint32_t)(640 * 480) * pf_div) / 4U;
      break;
    case CAMERA_R800x480:
      size = ((uint32_t)(800 * 480) * pf_div) / 4U;
      break;
    default:
      break;
  }

  return (int32_t)size;
}

/**
  * @brief  Initializes the DCMI MSP.
  * @param  hdcmi  DCMI handle
  * @retval None
  */
static void DCMI_MspInit(DCMI_HandleTypeDef *hdcmi)
{
  static DMA_NodeTypeDef DCMINode1;
  static DMA_NodeTypeDef DCMINode2;
  GPIO_InitTypeDef gpio_init_structure;
  DMA_NodeConfTypeDef node_config;

  /*** Enable peripherals and GPIO clocks ***/
  /* Enable DCMI clock */
  __HAL_RCC_DCMI_PSSI_CLK_ENABLE();

  /* Enable GPDMA1 clock */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* Enable GPIO clocks */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Configure DCMI GPIO as alternate function */
  /* DCMI PIXCLK and HSYNC pins */
  gpio_init_structure.Pin       = GPIO_PIN_5 | GPIO_PIN_8;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF10_DCMI;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* DCMI D0, D1, D2, D3, D4 pins */
  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
                                  GPIO_PIN_14;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* DCMI D5, D6, D7, VSYNC pins */
  gpio_init_structure.Pin       = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* Set node type */
  node_config.NodeType                            = DMA_GPDMA_2D_NODE;
  /* Set common node parameters */
  node_config.Init.Request                        = GPDMA1_REQUEST_DCMI;
  node_config.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  node_config.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
  node_config.Init.SrcInc                         = DMA_SINC_FIXED;
  node_config.Init.DestInc                        = DMA_DINC_INCREMENTED;
  node_config.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_WORD;
  node_config.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_WORD;
  node_config.Init.SrcBurstLength                 = 1;
  node_config.Init.DestBurstLength                = 1;
  node_config.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  node_config.Init.TransferEventMode              = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  /* Set node data handling parameters */
  node_config.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
  node_config.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  /* Set node trigger parameters */
  node_config.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;
  /* Set node repeated block parameters */
  node_config.RepeatBlockConfig.RepeatCount       = 1U;
  node_config.RepeatBlockConfig.SrcAddrOffset     = 0;
  node_config.RepeatBlockConfig.DestAddrOffset    = 0;
  node_config.RepeatBlockConfig.BlkSrcAddrOffset  = 0;
  node_config.RepeatBlockConfig.BlkDestAddrOffset = 0;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Set node attribute parameters */
  node_config.SrcSecure                           = DMA_CHANNEL_SRC_SEC;
  node_config.DestSecure                          = DMA_CHANNEL_DEST_SEC;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Build DCMINode1 */
  if (HAL_DMAEx_List_BuildNode(&node_config, &DCMINode1) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Insert DCMINode1 to DCMI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(&DCMIQueue, &DCMINode1) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Build DCMINode2 */
  if (HAL_DMAEx_List_BuildNode(&node_config, &DCMINode2) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Insert DCMINode2 to DCMI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(&DCMIQueue, &DCMINode2) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Set queue circular mode for DCMI queue */
  if (HAL_DMAEx_List_SetCircularMode(&DCMIQueue) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /*** Configure the DMA channel ***/
  hdma_handler.Instance                         = GPDMA1_Channel12;
  hdma_handler.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  hdma_handler.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  hdma_handler.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  hdma_handler.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;
  hdma_handler.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;

  /* DMA linked list init */
  if (HAL_DMAEx_List_Init(&hdma_handler) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Link DCMI queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&hdma_handler, &DCMIQueue) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Associate the initialized DMA handle to the DCMI handle */
  __HAL_LINKDMA(hdcmi, DMA_Handle, hdma_handler);

  /*** Configure the NVIC for DCMI and DMA ***/
  /* NVIC configuration for DCMI transfer complete interrupt */
  HAL_NVIC_SetPriority(DCMI_PSSI_IRQn, BSP_CAMERA_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(DCMI_PSSI_IRQn);

  /* NVIC configuration for GPDMA1 transfer complete interrupt */
  HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, BSP_CAMERA_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);
}

/**
  * @brief  DeInitializes the DCMI MSP.
  * @param  hdcmi  DCMI handle
  * @retval None
  */
static void DCMI_MspDeInit(DCMI_HandleTypeDef *hdcmi)
{
  GPIO_InitTypeDef gpio_init_structure;

  UNUSED(hdcmi);

  /* Disable NVIC  for DCMI transfer complete interrupt */
  HAL_NVIC_DisableIRQ(DCMI_PSSI_IRQn);

  /* Disable NVIC for GPDMA1 transfer complete interrupt */
  HAL_NVIC_DisableIRQ(GPDMA1_Channel12_IRQn);

  /* DMA linked list De-init */
  if (HAL_DMAEx_List_DeInit(&hdma_handler) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* Reset the DCMI queue */
  if (HAL_DMAEx_List_ResetQ(&DCMIQueue) != HAL_OK)
  {
    HAL_DCMI_ErrorCallback(hdcmi);
  }

  /* DeInit DCMI GPIOs */
  gpio_init_structure.Pin       = GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                                  GPIO_PIN_11 | GPIO_PIN_12 |  GPIO_PIN_14;
  HAL_GPIO_DeInit(GPIOH, gpio_init_structure.Pin);

  gpio_init_structure.Pin       = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);

  /* Disable DCMI clock */
  __HAL_RCC_DCMI_PSSI_CLK_DISABLE();
}

#if (USE_HAL_DCMI_REGISTER_CALLBACKS > 0)
/**
  * @brief  Line event callback
  * @param  hdcmi  pointer to the DCMI handle
  * @retval None
  */
static void DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_LineEventCallback(0);
}

/**
  * @brief  Frame event callback
  * @param  hdcmi pointer to the DCMI handle
  * @retval None
  */
static void DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_FrameEventCallback(0);
}

/**
  * @brief  Vsync event callback
  * @param  hdcmi  pointer to the DCMI handle
  * @retval None
  */
static void DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_VsyncEventCallback(0);
}

/**
  * @brief  Error callback
  * @param  hdcmi pointer to the DCMI handle
  * @retval None
  */
static void DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdcmi);

  BSP_CAMERA_ErrorCallback(0);
}
#endif /* (USE_HAL_DCMI_REGISTER_CALLBACKS > 0) */

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret;
  OV5640_IO_t              IOCtx;
  uint32_t                 id;
  static OV5640_Object_t   OV5640Obj;

  /* Configure the camera driver */
  IOCtx.Address     = CAMERA_OV5640_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  if (OV5640_RegisterBusIO(&OV5640Obj, &IOCtx) != OV5640_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (OV5640_ReadID(&OV5640Obj, &id) != OV5640_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    if (id != OV5640_ID)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {
      Camera_Drv = (CAMERA_Drv_t *) &OV5640_CAMERA_Driver;
      Camera_CompObj = &OV5640Obj;
      if (Camera_Drv->Init(Camera_CompObj, Resolution, PixelFormat) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else if (Camera_Drv->GetCapabilities(Camera_CompObj, &Camera_Cap) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else if (Camera_Drv->MirrorFlipConfig(Camera_CompObj, OV5640_MIRROR_FLIP_NONE) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  return ret;
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
