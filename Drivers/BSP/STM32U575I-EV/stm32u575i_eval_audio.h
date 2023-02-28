/**
  ******************************************************************************
  * @file    stm32u575i_eval_audio.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u575i_eval_audio.c driver.
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
#ifndef STM32U575I_EVAL_AUDIO_H
#define STM32U575I_EVAL_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"
#include "../Components/Common/audio.h"
#include "../Components/cs42l51/cs42l51.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_AUDIO
  * @{
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Exported_Types AUDIO Exported Types
  * @{
  */
typedef struct
{
  uint32_t Device;        /* Output or input device */
  uint32_t SampleRate;    /* From 8kHz to 192 kHz */
  uint32_t BitsPerSample; /* From 8 bits per sample to 32 bits per sample */
  uint32_t ChannelsNbr;   /* 1 for mono and 2 for stereo */
  uint32_t Volume;        /* In percentage from 0 to 100 */
} BSP_AUDIO_Init_t;

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pSAI_CallbackTypeDef  pMspInitCb;
  pSAI_CallbackTypeDef  pMspDeInitCb;
} BSP_AUDIO_OUT_Cb_t;
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1))
typedef struct
{
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
  pSAI_CallbackTypeDef  pMspSaiInitCb;
  pSAI_CallbackTypeDef  pMspSaiDeInitCb;
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
  pMDF_CallbackTypeDef  pMspMdfInitCb;
  pMDF_CallbackTypeDef  pMspMdfDeInitCb;
#endif /* USE_HAL_MDF_REGISTER_CALLBACKS == 1 */
} BSP_AUDIO_IN_Cb_t;
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

typedef struct
{
  uint32_t AudioFrequency;
  uint32_t MonoStereoMode;
  uint32_t AudioMode;
  uint32_t ClockStrobing;
  uint32_t Synchro;
  uint32_t OutputDrive;
  uint32_t SynchroExt;
  uint32_t DataSize;
  uint32_t FrameLength;
  uint32_t ActiveFrameLength;
  uint32_t SlotActive;
} MX_SAI_InitTypeDef;

typedef struct
{
  /* Filter parameters */
  MDF_Filter_TypeDef   *FilterInstance;
  uint32_t               RegularTrigger;
  uint32_t               SincOrder;
  uint32_t               Oversampling;
  /* Channel parameters */
  MDF_SerialInterfaceTypeDef *ChannelInstance;
  uint32_t              DigitalMicPins;
  uint32_t              DigitalMicType;
  uint32_t              Channel4Filter;
  uint32_t              ClockDivider;
  uint32_t              RightBitShift;
} MX_ADF_InitTypeDef;

/* Audio in and out context */
typedef struct
{
  uint32_t  Device;              /* Audio IN device to be used     */
  uint32_t  SampleRate;          /* Audio IN Sample rate           */
  uint32_t  BitsPerSample;       /* Audio IN Sample resolution     */
  uint32_t  ChannelsNbr;         /* Audio IN number of channel     */
  uint8_t   *pBuff;              /* Audio IN record buffer         */
  uint32_t  Size;                /* Audio IN record buffer size    */
  uint32_t  Volume;              /* Audio IN volume                */
  uint32_t  State;               /* Audio IN State                 */
} AUDIO_IN_Ctx_t;

typedef struct
{
  uint32_t  Device;              /* Audio out device                  */
  uint32_t  SampleRate;          /* Audio out sample rate             */
  uint32_t  BitsPerSample;       /* Audio out bits per sample         */
  uint32_t  Volume;              /* Audio out volume                  */
  uint32_t  ChannelsNbr;         /* Audio out number of channels      */
  uint32_t  IsMute;              /* Audio out mute state              */
  uint32_t  State;               /* Audio out state                   */
} AUDIO_OUT_Ctx_t;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Exported_Constants AUDIO Exported Constants
  * @{
  */
/* Audio out instances */
#define AUDIO_OUT_INSTANCES_NBR 1U

/* Audio in instances */
#define AUDIO_IN_INSTANCES_NBR 2U

/* Audio out devices */
#define AUDIO_OUT_HEADPHONE 1U

/* Audio input devices */
#define AUDIO_IN_DEVICE_ANALOG_MIC       0x01U
#define AUDIO_IN_DEVICE_DIGITAL_MIC      0x10U /* digital microphone */

/* Audio out states */
#define AUDIO_OUT_STATE_RESET   0U
#define AUDIO_OUT_STATE_PLAYING 1U
#define AUDIO_OUT_STATE_STOP    2U
#define AUDIO_OUT_STATE_PAUSE   3U

/* Audio in states */
#define AUDIO_IN_STATE_RESET     0U
#define AUDIO_IN_STATE_RECORDING 1U
#define AUDIO_IN_STATE_STOP      2U
#define AUDIO_IN_STATE_PAUSE     3U

/* Audio sample rate */
#define AUDIO_FREQUENCY_192K 192000U
#define AUDIO_FREQUENCY_176K 176400U
#define AUDIO_FREQUENCY_96K   96000U
#define AUDIO_FREQUENCY_88K   88200U
#define AUDIO_FREQUENCY_48K   48000U
#define AUDIO_FREQUENCY_44K   44100U
#define AUDIO_FREQUENCY_32K   32000U
#define AUDIO_FREQUENCY_22K   22050U
#define AUDIO_FREQUENCY_16K   16000U
#define AUDIO_FREQUENCY_11K   11025U
#define AUDIO_FREQUENCY_8K     8000U

/* Audio bits per sample */
#define AUDIO_RESOLUTION_8B   8U
#define AUDIO_RESOLUTION_16B 16U
#define AUDIO_RESOLUTION_24B 24U
#define AUDIO_RESOLUTION_32B 32U

/* Audio mute state */
#define AUDIO_MUTE_DISABLED 0U
#define AUDIO_MUTE_ENABLED  1U

/* Audio I2C address */
#define AUDIO_I2C_ADDRESS  0x94U

/* Audio in and out GPIOs */
#define AUDIO_SAI1_MCLK_B_GPIO_PORT           GPIOF
#define AUDIO_SAI1_MCLK_B_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAI1_MCLK_B_GPIO_PIN            GPIO_PIN_7
#define AUDIO_SAI1_MCLK_B_GPIO_AF             GPIO_AF13_SAI1

#define AUDIO_SAI1_FS_B_GPIO_PORT             GPIOF
#define AUDIO_SAI1_FS_B_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAI1_FS_B_GPIO_PIN              GPIO_PIN_9
#define AUDIO_SAI1_FS_B_GPIO_AF               GPIO_AF13_SAI1

#define AUDIO_SAI1_SCK_B_GPIO_PORT            GPIOF
#define AUDIO_SAI1_SCK_B_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAI1_SCK_B_GPIO_PIN             GPIO_PIN_8
#define AUDIO_SAI1_SCK_B_GPIO_AF              GPIO_AF13_SAI1

#define AUDIO_SAI1_SD_B_GPIO_PORT             GPIOF
#define AUDIO_SAI1_SD_B_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAI1_SD_B_GPIO_PIN              GPIO_PIN_6
#define AUDIO_SAI1_SD_B_GPIO_AF               GPIO_AF13_SAI1

#define AUDIO_SAI1_SD_A_GPIO_PORT             GPIOD
#define AUDIO_SAI1_SD_A_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_SAI1_SD_A_GPIO_PIN              GPIO_PIN_6
#define AUDIO_SAI1_SD_A_GPIO_AF               GPIO_AF13_SAI1

#define AUDIO_SAI1_CLK_ENABLE()               __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_SAI1_CLK_DISABLE()              __HAL_RCC_SAI1_CLK_DISABLE()

#define AUDIO_ADF1_CCK0_GPIO_PORT             GPIOB
#define AUDIO_ADF1_CCK0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_ADF1_CCK0_GPIO_PIN              GPIO_PIN_3
#define AUDIO_ADF1_CCK0_GPIO_AF               GPIO_AF3_ADF1

#define AUDIO_ADF1_SDIN0_GPIO_PORT            GPIOB
#define AUDIO_ADF1_SDIN0_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_ADF1_SDIN0_GPIO_PIN             GPIO_PIN_4
#define AUDIO_ADF1_SDIN0_GPIO_AF              GPIO_AF3_ADF1

#define AUDIO_ADF1_CLK_ENABLE()               __HAL_RCC_ADF1_CLK_ENABLE()
#define AUDIO_ADF1_CLK_DISABLE()              __HAL_RCC_ADF1_CLK_DISABLE()
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_AUDIO_Exported_Variables
  * @{
  */
/* Audio in and out DMA handles used by SAI */
extern DMA_HandleTypeDef hDmaSaiTx, hDmaSaiRx;

/* Audio in DMA handle used by MDF */
extern DMA_HandleTypeDef   haudio_mdf;

/* Audio in and out context */
extern AUDIO_OUT_Ctx_t Audio_Out_Ctx[AUDIO_OUT_INSTANCES_NBR];
extern AUDIO_IN_Ctx_t  Audio_In_Ctx[AUDIO_IN_INSTANCES_NBR];

/* Audio component object */
extern void *Audio_CompObj;

/* Audio driver */
extern AUDIO_Drv_t *Audio_Drv;

/* Audio in and out SAI handles */
extern SAI_HandleTypeDef haudio_out_sai;
extern SAI_HandleTypeDef haudio_in_sai;

/* Audio in MDF handle */
extern MDF_HandleTypeDef haudio_in_mdf_filter;
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_OUT_Exported_Functions AUDIO_OUT Exported Functions
  * @{
  */
int32_t           BSP_AUDIO_OUT_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit);
int32_t           BSP_AUDIO_OUT_DeInit(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_Play(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes);
int32_t           BSP_AUDIO_OUT_Pause(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_Resume(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_Stop(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_Mute(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_UnMute(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_IsMute(uint32_t Instance, uint32_t *IsMute);
int32_t           BSP_AUDIO_OUT_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t           BSP_AUDIO_OUT_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t           BSP_AUDIO_OUT_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t           BSP_AUDIO_OUT_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);
int32_t           BSP_AUDIO_OUT_SetDevice(uint32_t Instance, uint32_t Device);
int32_t           BSP_AUDIO_OUT_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t           BSP_AUDIO_OUT_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t           BSP_AUDIO_OUT_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);
int32_t           BSP_AUDIO_OUT_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t           BSP_AUDIO_OUT_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t           BSP_AUDIO_OUT_GetState(uint32_t Instance, uint32_t *State);

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
int32_t           BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t           BSP_AUDIO_OUT_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_OUT_Cb_t *CallBacks);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

void              BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance);
void              BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance);
void              BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance);

void              BSP_AUDIO_OUT_IRQHandler(uint32_t Instance, uint32_t Device);

HAL_StatusTypeDef MX_SAI1_ClockConfig(const SAI_HandleTypeDef *hsai, uint32_t SampleRate);
HAL_StatusTypeDef MX_SAI1_Init(SAI_HandleTypeDef *hsai, MX_SAI_InitTypeDef *MXInit);
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_IN_Exported_Functions AUDIO_IN Exported Functions
  * @{
  */
int32_t           BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit);
int32_t           BSP_AUDIO_IN_DeInit(uint32_t Instance);
int32_t           BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes);
int32_t           BSP_AUDIO_IN_Pause(uint32_t Instance);
int32_t           BSP_AUDIO_IN_Resume(uint32_t Instance);
int32_t           BSP_AUDIO_IN_Stop(uint32_t Instance);
int32_t           BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t           BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t           BSP_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t           BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);
int32_t           BSP_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device);
int32_t           BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t           BSP_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t           BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);
int32_t           BSP_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t           BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t           BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State);

#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1))
int32_t           BSP_AUDIO_IN_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t           BSP_AUDIO_IN_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks);
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

void              BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
void              BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);
void              BSP_AUDIO_IN_Error_CallBack(uint32_t Instance);

void              BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device);

HAL_StatusTypeDef MX_ADF1_ClockConfig(const MDF_HandleTypeDef *hDfsdmBlock, uint32_t SampleRate);
HAL_StatusTypeDef MX_ADF1_Init(MDF_HandleTypeDef *hAdfBlock, MX_ADF_InitTypeDef *MXInit);
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

#endif /* STM32U575I_EVAL_AUDIO_H */
