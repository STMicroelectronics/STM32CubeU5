/**
  ******************************************************************************
  * @file    stm32u575i_eval_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio driver for the STM32U575I-EVAL
  *          evaluation board.
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
  How To use this driver:
  -----------------------
   + This driver supports STM32U5xx devices on STM32U575I-EVAL (MB1550) Evaluation board.
   + Call the function BSP_AUDIO_OUT_Init() for AUDIO OUT initialization:
        Instance : Select the output instance. Can only be 0 (SAI).
        AudioInit: Audio Out structure to select the following parameters.
                   - Device: Select the output device (headphone, speaker, ..).
                   - SampleRate: Select the output sample rate (8Khz .. 96Khz).
                   - BitsPerSample: Select the output resolution (16 or 32bits per sample).
                   - ChannelsNbr: Select the output channels number(1 for mono, 2 for stereo).
                   - Volume: Select the output volume(0% .. 100%).

      This function configures all the hardware required for the audio application (codec, I2C, SAI/MDF,
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec or the MFX has failed (try to un-plug the power or reset device in this case).

      User can update the SAI or the clock configurations by overriding the weak MX functions MX_SAI1_Init()
      and MX_SAI1_ClockConfig().
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_OUT_RegisterMspCallbacks() function.
      User can restore the default MSP configuration by calling BSP_AUDIO_OUT_RegisterDefaultMspCallbacks().
      To use these two functions, user has to enable USE_HAL_SAI_REGISTER_CALLBACKS within stm32u5xx_hal_conf.h file.

   + Call the function BSP_AUDIO_OUT_Play() to play audio stream:
        Instance : Select the output instance. Can only be 0 (SAI).
        pBuf: pointer to the audio data file address.
        NbrOfBytes: Total size of the buffer to be sent in Bytes.

   + Call the function BSP_AUDIO_OUT_Pause() to pause playing.
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + Call the function BSP_AUDIO_OUT_Stop() to stop playing.
   + Call the function BSP_AUDIO_OUT_Mute() to mute the player.
   + Call the function BSP_AUDIO_OUT_UnMute() to unmute the player.
   + Call the function BSP_AUDIO_OUT_IsMute() to get the mute state(BSP_AUDIO_MUTE_ENABLED or BSP_AUDIO_MUTE_DISABLED).
   + Call the function BSP_AUDIO_OUT_SetDevice() to update the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_GetDevice() to get the AUDIO OUT device.
   + Call the function BSP_AUDIO_OUT_SetSampleRate() to update the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_GetSampleRate() to get the AUDIO OUT sample rate.
   + Call the function BSP_AUDIO_OUT_SetBitsPerSample() to update the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_GetBitPerSample() to get the AUDIO OUT resolution.
   + Call the function BSP_AUDIO_OUT_SetChannelsNbr() to update the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_GetChannelsNbr() to get the AUDIO OUT number of channels.
   + Call the function BSP_AUDIO_OUT_SetVolume() to update the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetVolume() to get the AUDIO OUT volume.
   + Call the function BSP_AUDIO_OUT_GetState() to get the AUDIO OUT state.

   + BSP_AUDIO_OUT_SetDevice(), BSP_AUDIO_OUT_SetSampleRate(), BSP_AUDIO_OUT_SetBitsPerSample() and
     BSP_AUDIO_OUT_SetChannelsNbr() cannot be called while the state is AUDIO_OUT_STATE_PLAYING.
   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in
      the stm32u575i_eval_audio.h file. (refer to the example for more details on the callbacks implementations).

   + Call the function BSP_AUDIO_IN_Init() for AUDIO IN initialization:
        Instance : Select the input instance. Can be 0 (SAI) or 1 (MDF).
        AudioInit: Audio In structure to select the following parameters.
                   - Device: Select the input device (analog or digital).
                   - SampleRate: Select the input sample rate (8Khz .. 96Khz).
                   - BitsPerSample: Select the input resolution (16 or 32bits per sample).
                   - ChannelsNbr: Select the input channels number(1 for mono, 2 for stereo).
                   - Volume: Select the input volume(0% .. 100%).

      This function configures all the hardware required for the audio application (codec, I2C, SAI,
      GPIOs, DMA and interrupt if needed). This function returns BSP_ERROR_NONE if configuration is OK.
      If the returned value is different from BSP_ERROR_NONE or the function is stuck then the communication with
      the codec or the MFX has failed (try to un-plug the power or reset device in this case).

      User can update the SAI or the clock configurations by overriding the weak MX functions MX_SAI1_Init() and
      MX_SAI1_ClockConfig()
      User can override the default MSP configuration and register his own MSP callbacks (defined at application level)
      by calling BSP_AUDIO_IN_RegisterMspCallbacks() function.
      User can restore the default MSP configuration by calling BSP_AUDIO_IN_RegisterDefaultMspCallbacks().
      To use these two functions, user have to enable USE_HAL_SAI_REGISTER_CALLBACKS
      within stm32u5xx_hal_conf.h file.

   + Call the function BSP_EVAL_AUDIO_IN_Record() to record audio stream. The recorded data are stored to user buffer
        in raw (L, R, L, R ...).
        Instance : Select the input instance. Can be 0 (SAI) or 1 (MDF).
        pBuf: pointer to user buffer.
        NbrOfBytes: Total size of the buffer to be sent in Bytes.

   + Call the function BSP_AUDIO_IN_Pause() to pause recording.
   + Call the function BSP_AUDIO_IN_Resume() to resume recording.
   + Call the function BSP_AUDIO_IN_Stop() to stop recording.
   + Call the function BSP_AUDIO_IN_SetDevice() to update the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_GetDevice() to get the AUDIO IN device.
   + Call the function BSP_AUDIO_IN_SetSampleRate() to update the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_GetSampleRate() to get the AUDIO IN sample rate.
   + Call the function BSP_AUDIO_IN_SetBitPerSample() to update the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_GetBitPerSample() to get the AUDIO IN resolution.
   + Call the function BSP_AUDIO_IN_SetChannelsNbr() to update the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_GetChannelsNbr() to get the AUDIO IN number of channels.
   + Call the function BSP_AUDIO_IN_SetVolume() to update the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetVolume() to get the AUDIO IN volume.
   + Call the function BSP_AUDIO_IN_GetState() to get the AUDIO IN state.

   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_IN_XXX_CallBack() and only their prototypes are declared in
      the stm32u575i_eval_audio.h file (refer to the example for more details on the callbacks implementations).

   + The driver API and the callback functions are at the end of the stm32u575i_eval_audio.h file.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_audio.h"
#include "stm32u575i_eval_bus.h"
#if (USE_BSP_IO_CLASS > 0)
#include "stm32u575i_eval_io.h"
#endif /* USE_BSP_IO_CLASS */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @defgroup STM32U575I_EVAL_AUDIO AUDIO
  * @{
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Private_Macros AUDIO Private Macros
  * @{
  */
#define MDF_DECIMATION_RATIO(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))  ? (88U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_11K)) ? (64U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K)) ? (44u) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_22K)) ? (32U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K)) ? (22U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_44K)) ? (16U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K)) ? (11U) : (64U)

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Exported_Variables AUDIO Exported Variables
  * @{
  */
/* Audio in and out context */
AUDIO_OUT_Ctx_t Audio_Out_Ctx[AUDIO_OUT_INSTANCES_NBR] = {{
    AUDIO_OUT_HEADPHONE,
    AUDIO_FREQUENCY_11K,
    AUDIO_RESOLUTION_16B,
    50U,
    1U,
    AUDIO_MUTE_DISABLED,
    AUDIO_OUT_STATE_RESET
  }
};

AUDIO_IN_Ctx_t  Audio_In_Ctx[AUDIO_IN_INSTANCES_NBR] = {{
    AUDIO_IN_DEVICE_ANALOG_MIC,
    AUDIO_FREQUENCY_8K,
    AUDIO_RESOLUTION_16B,
    1U,
    NULL,
    0U,
    50U,
    AUDIO_IN_STATE_RESET
  },
  {
    AUDIO_IN_DEVICE_DIGITAL_MIC,
    AUDIO_FREQUENCY_11K,
    AUDIO_RESOLUTION_16B,
    1U,
    NULL,
    0U,
    50U,
    AUDIO_IN_STATE_RESET
  }
};

/* Audio component object */
void *Audio_CompObj = NULL;

/* Audio driver */
AUDIO_Drv_t *Audio_Drv = NULL;

/* Audio in and out SAI handles */
SAI_HandleTypeDef haudio_out_sai = {0};
SAI_HandleTypeDef haudio_in_sai  = {0};

/* Audio in MDF handles */
MDF_HandleTypeDef   haudio_in_mdf_filter = {0};

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Private_Variables AUDIO Private Variables
  * @{
  */
/* Audio in and out DMA handles used by SAI */
DMA_HandleTypeDef hDmaSaiTx, hDmaSaiRx;

/* Audio in DMA handle used by MDF */
DMA_HandleTypeDef   haudio_mdf;

/* Queue variables declaration */
static DMA_QListTypeDef SAITxQueue, SAIRxQueue, MdfQueue;

/* Audio in MDF global variables */
static uint32_t Audio_DmaDigMicRecHalfBuffCplt;
static uint32_t Audio_DmaDigMicRecBuffCplt;
static MDF_FilterConfigTypeDef filterConfig;
static MDF_DmaConfigTypeDef    dmaConfig;

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
static uint32_t AudioOut_IsMspCbValid[AUDIO_OUT_INSTANCES_NBR] = {0};
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS */
#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1))
static uint32_t AudioIn_IsMspCbValid[AUDIO_IN_INSTANCES_NBR] = {0};
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Private_Function_Prototypes AUDIO Private Function Prototypes
  * @{
  */
static int32_t CS42L51_Probe(void);
static int32_t CS42L51_PowerUp(void);
static int32_t CS42L51_PowerDown(void);
static void    SAI_MspInit(SAI_HandleTypeDef *hsai);
static void    SAI_MspDeInit(SAI_HandleTypeDef *hsai);
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
static void    SAI_TxCpltCallback(SAI_HandleTypeDef *hsai);
static void    SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai);
static void    SAI_RxCpltCallback(SAI_HandleTypeDef *hsai);
static void    SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai);
static void    SAI_ErrorCallback(SAI_HandleTypeDef *hsai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

static void    MDF_BlockMspInit(MDF_HandleTypeDef *hmdf);
static void    MDF_BlockMspDeInit(MDF_HandleTypeDef *hmdf);
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
static void    MDF_AcquisitionCpltCallback(MDF_HandleTypeDef *hadf_filter);
static void    MDF_AcquisitionHalfCpltCallback(MDF_HandleTypeDef *hadf_filter);
static void    MDF_ErrorCallback(MDF_HandleTypeDef *hadf_filter);
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_AUDIO_OUT_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @param  AudioInit Audio out init structure.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((AudioInit->BitsPerSample == AUDIO_RESOLUTION_32B)
                                || (AudioInit->BitsPerSample == AUDIO_RESOLUTION_8B)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET) &&
           ((Audio_In_Ctx[0].SampleRate != AudioInit->SampleRate) ||
            (Audio_In_Ctx[0].BitsPerSample != AudioInit->BitsPerSample)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Un-reset audio codec if not currently used by audio in instance 0 */
    if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
    {
      if (CS42L51_PowerUp() != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* Fill audio out context structure */
      Audio_Out_Ctx[Instance].Device         = AudioInit->Device;
      Audio_Out_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
      Audio_Out_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
      Audio_Out_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
      Audio_Out_Ctx[Instance].Volume         = AudioInit->Volume;

      /* Probe the audio codec */
      if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
      {
        if (CS42L51_Probe() != BSP_ERROR_NONE)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Set SAI instance */
        haudio_out_sai.Instance = SAI1_Block_B;

        /* Configure the SAI PLL according to the requested audio frequency if not already done by other instances */
        if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
        {
          if (MX_SAI1_ClockConfig(&haudio_out_sai, AudioInit->SampleRate) != HAL_OK)
          {
            status = BSP_ERROR_CLOCK_FAILURE;
          }
        }

        if (status == BSP_ERROR_NONE)
        {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
          SAI_MspInit(&haudio_out_sai);
#else
          /* Register the SAI MSP Callbacks */
          if (AudioOut_IsMspCbValid[Instance] == 0U)
          {
            if (BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }
          }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
        }

        if (status == BSP_ERROR_NONE)
        {
          /* Prepare SAI peripheral initialization */
          MX_SAI_InitTypeDef mxSaiInit;
          mxSaiInit.AudioFrequency    = AudioInit->SampleRate;
          mxSaiInit.AudioMode         = SAI_MODEMASTER_TX;
          mxSaiInit.ClockStrobing     = SAI_CLOCKSTROBING_FALLINGEDGE;
          mxSaiInit.MonoStereoMode    = (AudioInit->ChannelsNbr == 1U) ? SAI_MONOMODE : SAI_STEREOMODE;
          if (AudioInit->BitsPerSample == AUDIO_RESOLUTION_24B)
          {
            mxSaiInit.DataSize          = SAI_DATASIZE_24;
            mxSaiInit.FrameLength       = 64;
            mxSaiInit.ActiveFrameLength = 32;
          }
          else
          {
            mxSaiInit.DataSize          = SAI_DATASIZE_16;
            mxSaiInit.FrameLength       = 32;
            mxSaiInit.ActiveFrameLength = 16;
          }
          mxSaiInit.OutputDrive       = SAI_OUTPUTDRIVE_ENABLE;
          mxSaiInit.Synchro           = SAI_ASYNCHRONOUS;
          mxSaiInit.SynchroExt        = SAI_SYNCEXT_DISABLE;
          mxSaiInit.SlotActive        = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;

          /* SAI peripheral initialization */
          if (MX_SAI1_Init(&haudio_out_sai, &mxSaiInit) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
          /* Register SAI TC, HT and Error callbacks */
          else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback)
                   != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
          else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
          else
          {
            /* Initialize audio codec */
            CS42L51_Init_t codec_init;
            codec_init.InputDevice  = (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
                                      ? CS42L51_IN_NONE
                                      : CS42L51_IN_MIC1;
            codec_init.OutputDevice = CS42L51_OUT_HEADPHONE;
            codec_init.Frequency    = AudioInit->SampleRate;
            codec_init.Resolution   = CS42L51_RESOLUTION_16B; /* Not used */
            codec_init.Volume       = AudioInit->Volume;
            if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
            {
              status = BSP_ERROR_COMPONENT_FAILURE;
            }
            else
            {
              /* Update audio out context state */
              Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
            }
          }
        }
      }
    }
  }
  return status;
}

/**
  * @brief  De-initialize the audio out peripherals.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_RESET)
  {
    /* Reset audio codec if not currently used by audio in instance 0 */
    if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
    {
      if (CS42L51_PowerDown() != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* SAI peripheral de-initialization */
      if (HAL_SAI_DeInit(&haudio_out_sai) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      /* De-initialize audio codec if not currently used by audio in instance 0 */
      else
      {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
        SAI_MspDeInit(&haudio_out_sai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
        if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
        {
          if (Audio_Drv->DeInit(Audio_CompObj) < 0)
          {
            status = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Update audio out context */
        Audio_Out_Ctx[Instance].State  = AUDIO_OUT_STATE_RESET;
        Audio_Out_Ctx[Instance].IsMute = 0U;
      }
    }
  }
  else
  {
    /* Nothing to do */
  }
  return status;
}

/**
  * @brief  Start playing audio stream from a data buffer for a determined size.
  * @param  Instance Audio out instance.
  * @param  pData Pointer on data buffer.
  * @param  NbrOfBytes Size of buffer in bytes. Maximum size is 65535 bytes.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Play(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes)
{
  int32_t  status = BSP_ERROR_NONE;
  uint16_t NbrOfDmaDatas;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (pData == NULL)/* || (NbrOfBytes > 65535U)*/)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Compute number of DMA data to tranfser according resolution */
    if (Audio_Out_Ctx[Instance].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 2U);
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      NbrOfDmaDatas = (uint16_t)(NbrOfBytes / 4U);
    }
    /* Initiate a DMA transfer of audio samples towards the serial audio interface */
    if (HAL_SAI_Transmit_DMA(&haudio_out_sai, pData, NbrOfDmaDatas) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Call the audio codec play function */
    else if (Audio_Drv->Play(Audio_CompObj) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Update audio out state */
      Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
    }
  }
  return status;
}

/**
  * @brief  Pause playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Pause(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec pause function */
  else if (Audio_Drv->Pause(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Pause DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAPause(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PAUSE;
  }
  return status;
}

/**
  * @brief  Resume playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Resume(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec resume function */
  else if (Audio_Drv->Resume(Audio_CompObj) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Resume DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAResume(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_PLAYING;
  }
  return status;
}

/**
  * @brief  Stop playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Stop(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PLAYING) &&
           (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_PAUSE))
  {
    status = BSP_ERROR_BUSY;
  }
  /* Call the audio codec stop function */
  else if (Audio_Drv->Stop(Audio_CompObj, CS42L51_PDWN_SW) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Stop DMA transfer of audio samples towards the serial audio interface */
  else if (HAL_SAI_DMAStop(&haudio_out_sai) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Update audio out state */
    Audio_Out_Ctx[Instance].State = AUDIO_OUT_STATE_STOP;
  }
  return status;
}

/**
  * @brief  Mute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_Mute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 1U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, CS42L51_MUTE_ON) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 1U;
  }
  return status;
}

/**
  * @brief  Unmute playback of audio stream.
  * @param  Instance Audio out instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_UnMute(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check audio out mute status */
  else if (Audio_Out_Ctx[Instance].IsMute == 0U)
  {
    /* Nothing to do */
  }
  /* Call the audio codec mute function */
  else if (Audio_Drv->SetMute(Audio_CompObj, CS42L51_MUTE_OFF) < 0)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Update audio out mute status */
    Audio_Out_Ctx[Instance].IsMute = 0U;
  }
  return status;
}

/**
  * @brief  Check audio out mute status.
  * @param  Instance Audio out instance.
  * @param  IsMute Pointer to mute status. Value is 1 for mute, 0 for unmute status.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_IsMute(uint32_t Instance, uint32_t *IsMute)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out mute status */
  else
  {
    *IsMute = Audio_Out_Ctx[Instance].IsMute;
  }
  return status;
}

/**
  * @brief  Set audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || (Volume > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Call the audio codec volume control function */
    if (Audio_Drv->SetVolume(Audio_CompObj, VOLUME_OUTPUT, (uint8_t) Volume) < 0)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Store volume on audio out context */
      Audio_Out_Ctx[Instance].Volume = Volume;
    }
  }
  return status;
}

/**
  * @brief  Get audio out volume.
  * @param  Instance Audio out instance.
  * @param  Volume Pointer to volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out volume */
  else
  {
    *Volume = Audio_Out_Ctx[Instance].Volume;
  }
  return status;
}

/**
  * @brief  Set audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetSampleRate(uint32_t Instance, uint32_t SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check if record on instance 0 is on going and corresponding sample rate */
  else if ((Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET) &&
           (Audio_In_Ctx[0].SampleRate != SampleRate))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check if sample rate is modified */
  else if (Audio_Out_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    /* Update SAI1 clock config */
    haudio_out_sai.Init.AudioFrequency = SampleRate;
    if (MX_SAI1_ClockConfig(&haudio_out_sai, SampleRate) != HAL_OK)
    {
      status = BSP_ERROR_CLOCK_FAILURE;
    }
    /* Re-initialize SAI1 with new sample rate */
    else if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
    /* Register SAI TC, HT and Error callbacks */
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
    /* Store new sample rate on audio out context */
    else
    {
      Audio_Out_Ctx[Instance].SampleRate = SampleRate;
    }
  }
  return status;
}

/**
  * @brief  Get audio out sample rate.
  * @param  Instance Audio out instance.
  * @param  SampleRate Pointer to sample rate of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out sample rate */
  else
  {
    *SampleRate = Audio_Out_Ctx[Instance].SampleRate;
  }
  return status;
}

/**
  * @brief  Set audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t status = BSP_ERROR_NONE;

  UNUSED(Device);

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because there is only one device (AUDIO_OUT_HEADPHONE) */
  }
  return status;
}

/**
  * @brief  Get audio out device.
  * @param  Instance Audio out instance.
  * @param  Device Pointer to device of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio out device */
  else
  {
    *Device = Audio_Out_Ctx[Instance].Device;
  }
  return status;
}

/**
  * @brief  Set bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && ((BitsPerSample == AUDIO_RESOLUTION_32B) || (BitsPerSample == AUDIO_RESOLUTION_8B)))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Store new bits per sample on audio out context */
    Audio_Out_Ctx[Instance].BitsPerSample = BitsPerSample;

    /* Update data size, frame length and active frame length parameters of SAI handle */
    if (BitsPerSample == AUDIO_RESOLUTION_24B)
    {
      haudio_out_sai.Init.DataSize               = SAI_DATASIZE_24;
      haudio_out_sai.FrameInit.FrameLength       = 64;
      haudio_out_sai.FrameInit.ActiveFrameLength = 32;
    }
    else
    {
      haudio_out_sai.Init.DataSize               = SAI_DATASIZE_16;
      haudio_out_sai.FrameInit.FrameLength       = 32;
      haudio_out_sai.FrameInit.ActiveFrameLength = 16;
    }

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
    SAI_MspInit(&haudio_out_sai);
#else
    /* Update SAI state only to keep current MSP functions */
    haudio_out_sai.State = HAL_SAI_STATE_RESET;
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */

    /* Re-initialize SAI1 with new parameters */
    if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
    /* Register SAI TC, HT and Error callbacks */
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
    else
    {
      /* Nothing to do */
    }
  }
  return status;
}

/**
  * @brief  Get bits per sample for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  BitsPerSample Pointer to bits per sample of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio out stream */
  else
  {
    *BitsPerSample = Audio_Out_Ctx[Instance].BitsPerSample;
  }
  return status;
}

/**
  * @brief  Set channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_OUT_INSTANCES_NBR) || ((ChannelNbr != 1U) && (ChannelNbr != 2U)))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State != AUDIO_OUT_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Update mono or stereo mode of SAI handle */
    haudio_out_sai.Init.MonoStereoMode = (ChannelNbr == 1U) ? SAI_MONOMODE : SAI_STEREOMODE;

    /* Re-initialize SAI1 with new parameter */
    if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
    /* Register SAI TC, HT and Error callbacks */
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_COMPLETE_CB_ID, SAI_TxCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_TX_HALFCOMPLETE_CB_ID, SAI_TxHalfCpltCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
    else
    {
      /* Store new channels number on audio out context */
      Audio_Out_Ctx[Instance].ChannelsNbr = ChannelNbr;
    }
  }
  return status;
}

/**
  * @brief  Get channels number for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  ChannelNbr Pointer to channels number of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio out state */
  else if (Audio_Out_Ctx[Instance].State == AUDIO_OUT_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio out stream */
  else
  {
    *ChannelNbr = Audio_Out_Ctx[Instance].ChannelsNbr;
  }
  return status;
}

/**
  * @brief  Get current state for the audio out stream.
  * @param  Instance Audio out instance.
  * @param  State Pointer to state of the audio out stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Get the current state of audio out stream */
  else
  {
    *State = Audio_Out_Ctx[Instance].State;
  }
  return status;
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPINIT_CB_ID, SAI_MspInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPDEINIT_CB_ID, SAI_MspDeInit) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      AudioOut_IsMspCbValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO OUT msp callbacks.
  * @param  Instance AUDIO OUT Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_OUT_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_OUT_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_OUT_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPINIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SAI_RegisterCallback(&haudio_out_sai, HAL_SAI_MSPDEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      AudioOut_IsMspCbValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Manage the BSP audio out transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manage the BSP audio out half transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manages the BSP audio out error event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  BSP AUDIO OUT interrupt handler.
  * @param  Instance Audio out instance.
  * @param  Device Device of the audio out stream.
  * @retval None.
  */
void BSP_AUDIO_OUT_IRQHandler(uint32_t Instance, uint32_t Device)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
  UNUSED(Device);

  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

/**
  * @brief  SAI1 clock Config.
  * @param  hsai SAI handle.
  * @param  SampleRate Audio sample rate used to play the audio stream.
  * @note   The SAI PLL configuration done within this function assumes that
  *         the SAI PLL input is MSI clock and that MSI is already enabled at 4 MHz.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_SAI1_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  HAL_StatusTypeDef ret = HAL_OK;
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;

  rcc_ex_clk_init_struct.PLL2.PLL2Source = RCC_PLLSOURCE_MSI;
  rcc_ex_clk_init_struct.PLL2.PLL2RGE = 0;
  rcc_ex_clk_init_struct.PLL2.PLL2FRACN = 0;
  rcc_ex_clk_init_struct.PLL2.PLL2ClockOut = RCC_PLL2_DIVP;
  rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  rcc_ex_clk_init_struct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  rcc_ex_clk_init_struct.PLL2.PLL2Q = 28;
  rcc_ex_clk_init_struct.PLL2.PLL2R = 28;

  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    /* SAI clock config:
    PLL2_VCO Input = MSI_4Mhz/PLL2M = 4 Mhz
    PLL2_VCO Output = PLL2_VCO Input * PLL2N = 320 Mhz
    SAI_CLK_x = PLL2_VCO Output/PLL2P = 320/28 = 11.428 Mhz */
    rcc_ex_clk_init_struct.PLL2.PLL2M = 1;
    rcc_ex_clk_init_struct.PLL2.PLL2N = 80;
    rcc_ex_clk_init_struct.PLL2.PLL2P = 28;
  }
  else  /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K, AUDIO_FREQUENCY_96K */
  {
    /* SAI clock config:
    PLL2_VCO Input = MSI_4Mhz/PLL2M = 2 Mhz
    PLL2_VCO Output = PLL2_VCO Input * PLL2N = 344 Mhz
    SAI_CLK_x = PLL2_VCO Output/PLL2P = 344/7 = 49.142 Mhz */
    rcc_ex_clk_init_struct.PLL2.PLL2M = 2;
    rcc_ex_clk_init_struct.PLL2.PLL2N = 172;
    rcc_ex_clk_init_struct.PLL2.PLL2P = 7;
  }

  if (HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  __HAL_RCC_SAI1_CLK_ENABLE();

  return ret;
}

/**
  * @brief  Initialize SAI1.
  * @param  hsai SAI handle.
  * @param  MXInit SAI configuration structure.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_SAI1_Init(SAI_HandleTypeDef *hsai, MX_SAI_InitTypeDef *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (hsai->Instance == SAI1_Block_B)
  {
    /* Disable SAI peripheral to allow access to SAI internal registers */
    __HAL_SAI_DISABLE(hsai);

    /* Configure SAI1_Block_B */
    hsai->Init.MonoStereoMode       = MXInit->MonoStereoMode;
    hsai->Init.AudioFrequency       = MXInit->AudioFrequency;
    hsai->Init.AudioMode            = MXInit->AudioMode;
    hsai->Init.NoDivider            = SAI_MASTERDIVIDER_ENABLE;
    hsai->Init.Protocol             = SAI_FREE_PROTOCOL;
    hsai->Init.DataSize             = MXInit->DataSize;
    hsai->Init.FirstBit             = SAI_FIRSTBIT_MSB;
    hsai->Init.ClockStrobing        = MXInit->ClockStrobing;
    hsai->Init.Synchro              = MXInit->Synchro;
    hsai->Init.OutputDrive          = MXInit->OutputDrive;
    hsai->Init.FIFOThreshold        = SAI_FIFOTHRESHOLD_1QF;
    hsai->Init.SynchroExt           = MXInit->SynchroExt;
    hsai->Init.CompandingMode       = SAI_NOCOMPANDING;
    hsai->Init.TriState             = SAI_OUTPUT_NOTRELEASED;
    hsai->Init.Mckdiv               = 0U;
    hsai->Init.MckOutput            = SAI_MCK_OUTPUT_ENABLE;
    hsai->Init.MckOverSampling      = SAI_MCK_OVERSAMPLING_DISABLE;
    hsai->Init.PdmInit.Activation   = DISABLE;

    /* Configure SAI1_Block_B Frame */
    hsai->FrameInit.FrameLength       = MXInit->FrameLength;
    hsai->FrameInit.ActiveFrameLength = MXInit->ActiveFrameLength;
    hsai->FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
    hsai->FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
    hsai->FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

    /* Configure SAI1_Block_B Slot */
    hsai->SlotInit.FirstBitOffset     = 0;
    if (MXInit->DataSize == AUDIO_RESOLUTION_24B)
    {
      hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_32B;
    }
    else
    {
      hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_16B;
    }
    hsai->SlotInit.SlotNumber         = 2;
    hsai->SlotInit.SlotActive         = MXInit->SlotActive;

    if (HAL_SAI_Init(hsai) != HAL_OK)
    {
      status = HAL_ERROR;
    }
  }
  else /* (hsai->Instance == SAI1_Block_A) */
  {
    __HAL_SAI_DISABLE(hsai);

    /*****************************/
    /* SAI block used for record */
    /*****************************/
    /* Configure SAI_Block_A used for receive */
    hsai->Init.AudioMode      = SAI_MODESLAVE_RX;
    hsai->Init.Synchro        = SAI_SYNCHRONOUS;
    hsai->Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
    hsai->Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
    hsai->Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
    hsai->Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
    hsai->Init.AudioFrequency = SAI_AUDIO_FREQUENCY_MCKDIV;
    hsai->Init.Mckdiv         = 0U;
    hsai->Init.MonoStereoMode = SAI_MONOMODE;
    hsai->Init.CompandingMode = SAI_NOCOMPANDING;
    hsai->Init.TriState       = SAI_OUTPUT_NOTRELEASED;
    hsai->Init.Protocol       = SAI_FREE_PROTOCOL;
    hsai->Init.DataSize       = MXInit->DataSize;
    hsai->Init.FirstBit       = SAI_FIRSTBIT_MSB;
    hsai->Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;

    /* Configure SAI_Block_A Frame
    Frame Length: 64 or 32
    Frame active Length: 32 or 16
    FS Definition: Start frame + Channel Side identification
    FS Polarity: FS active Low
    FS Offset: FS asserted one bit before the first bit of slot 0 */
    hsai->FrameInit.FrameLength = MXInit->FrameLength;
    hsai->FrameInit.ActiveFrameLength = MXInit->ActiveFrameLength;
    hsai->FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    hsai->FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
    hsai->FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;

    /* Configure SAI Block_A Slot
    Slot First Bit Offset: 0
    Slot Size  : 16
    Slot Number: 2
    Slot Active: Slots 0 and 1 actives */
    hsai->SlotInit.FirstBitOffset = 0;
    hsai->SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
    hsai->SlotInit.SlotNumber = 2;
    hsai->SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;

    /**********************************/
    /* Initializes the SAI peripheral */
    /**********************************/
    if (HAL_SAI_Init(hsai) != HAL_OK)
    {
      status = HAL_ERROR;
    }
  }
  return status;
}
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_AUDIO_IN_Exported_Functions
  * @{
  */
/**
  * @brief  Initialize the audio in peripherals.
  * @param  Instance Audio in instance.
  * @param  AudioInit Audio in init structure.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Init(uint32_t Instance, BSP_AUDIO_Init_t *AudioInit)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  else if ((Instance == 0U) && (AudioInit->ChannelsNbr != 1U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET) && (Audio_Out_Ctx[0].SampleRate != AudioInit->SampleRate))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_In_Ctx[0].State != AUDIO_IN_STATE_RESET)
           && (Audio_In_Ctx[0].SampleRate != AudioInit->SampleRate))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if ((Instance == 0U) && (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET) &&
           (Audio_Out_Ctx[0].BitsPerSample != AudioInit->BitsPerSample))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Un-reset audio codec if not currently used by audio out instance 0 */
      if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
      {
        if (CS42L51_PowerUp() != BSP_ERROR_NONE)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Fill audio in context structure */
        Audio_In_Ctx[Instance].Device         = AudioInit->Device;
        Audio_In_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
        Audio_In_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
        Audio_In_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
        Audio_In_Ctx[Instance].Volume         = AudioInit->Volume;

        /* Probe the audio codec */
        if (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET)
        {
          if (CS42L51_Probe() != BSP_ERROR_NONE)
          {
            status = BSP_ERROR_COMPONENT_FAILURE;
          }
        }

        if (status == BSP_ERROR_NONE)
        {
          /* Set SAI instances (SAI1_Block_B needed for MCLK, FS and CLK signals) */
          haudio_in_sai.Instance  = SAI1_Block_A;
          haudio_out_sai.Instance = SAI1_Block_B;

          /* Configure the SAI PLL according to the requested audio frequency if not already done by other instances */
          if ((Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET) && (Audio_In_Ctx[0].State == AUDIO_IN_STATE_RESET))
          {
            if (MX_SAI1_ClockConfig(&haudio_in_sai, AudioInit->SampleRate) != HAL_OK)
            {
              status = BSP_ERROR_CLOCK_FAILURE;
            }
          }

          if (status == BSP_ERROR_NONE)
          {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
            SAI_MspInit(&haudio_in_sai);
            if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
            {
              SAI_MspInit(&haudio_out_sai);
            }
#else
            /* Register the SAI MSP Callbacks */
            if (AudioIn_IsMspCbValid[Instance] == 0U)
            {
              if (BSP_AUDIO_IN_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
            }
            if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
            {
              if (AudioOut_IsMspCbValid[0] == 0U)
              {
                if (BSP_AUDIO_OUT_RegisterDefaultMspCallbacks(0) != BSP_ERROR_NONE)
                {
                  status = BSP_ERROR_PERIPH_FAILURE;
                }
              }
            }
#endif /* #if (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
          }

          if (status == BSP_ERROR_NONE)
          {
            /* Prepare SAI peripheral initialization */
            MX_SAI_InitTypeDef mxSaiInit;
            mxSaiInit.AudioFrequency    = AudioInit->SampleRate;
            mxSaiInit.AudioMode         = SAI_MODESLAVE_RX;
            mxSaiInit.ClockStrobing     = SAI_CLOCKSTROBING_FALLINGEDGE;
            mxSaiInit.MonoStereoMode    = SAI_MONOMODE;
            if (AudioInit->BitsPerSample == AUDIO_RESOLUTION_24B)
            {
              mxSaiInit.DataSize          = SAI_DATASIZE_24;
              mxSaiInit.FrameLength       = 64;
              mxSaiInit.ActiveFrameLength = 32;
            }
            else
            {
              mxSaiInit.DataSize          = SAI_DATASIZE_16;
              mxSaiInit.FrameLength       = 32;
              mxSaiInit.ActiveFrameLength = 16;
            }
            mxSaiInit.OutputDrive       = SAI_OUTPUTDRIVE_ENABLE;
            mxSaiInit.Synchro           = SAI_SYNCHRONOUS;
            mxSaiInit.SynchroExt        = SAI_SYNCEXT_DISABLE;
            mxSaiInit.SlotActive        = SAI_SLOTACTIVE_0;

            /* SAI peripheral initialization */
            if (MX_SAI1_Init(&haudio_in_sai, &mxSaiInit) != HAL_OK)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }
            else
            {
              if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
              {
                /* Initialize SAI peripheral used to generate clock and synchro */
                mxSaiInit.AudioMode  = SAI_MODEMASTER_TX;
                mxSaiInit.Synchro    = SAI_ASYNCHRONOUS;
                mxSaiInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;
                if (MX_SAI1_Init(&haudio_out_sai, &mxSaiInit) != HAL_OK)
                {
                  status = BSP_ERROR_PERIPH_FAILURE;
                }
              }
            }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
            if (status == BSP_ERROR_NONE)
            {
              /* Register SAI TC, HT and Error callbacks */
              if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_COMPLETE_CB_ID, SAI_RxCpltCallback) != HAL_OK)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
              else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_HALFCOMPLETE_CB_ID, SAI_RxHalfCpltCallback)
                       != HAL_OK)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
              else
              {
                if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
                {
                  status = BSP_ERROR_PERIPH_FAILURE;
                }
              }
            }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
            if (status == BSP_ERROR_NONE)
            {
              /* Initialize audio codec */
              CS42L51_Init_t codec_init;
              codec_init.InputDevice  = CS42L51_IN_MIC1;
              codec_init.OutputDevice = (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
                                        ? CS42L51_OUT_NONE
                                        : CS42L51_OUT_HEADPHONE;
              codec_init.Frequency    = AudioInit->SampleRate;
              codec_init.Resolution   = CS42L51_RESOLUTION_16B; /* Not used */
              codec_init.Volume       = Audio_Out_Ctx[0].Volume;
              if (Audio_Drv->Init(Audio_CompObj, &codec_init) < 0)
              {
                status = BSP_ERROR_COMPONENT_FAILURE;
              }
              else
              {
                /* Update audio in context state */
                Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
              }
            }
          }
        }
      }
    }
    else /* (Instance == 1U) */
    {
      /* Fill audio in context structure */
      Audio_In_Ctx[Instance].Device         = AudioInit->Device;
      Audio_In_Ctx[Instance].SampleRate     = AudioInit->SampleRate;
      Audio_In_Ctx[Instance].BitsPerSample  = AudioInit->BitsPerSample;
      Audio_In_Ctx[Instance].ChannelsNbr    = AudioInit->ChannelsNbr;
      Audio_In_Ctx[Instance].Volume         = AudioInit->Volume;

      /* Set MDF instance */
      haudio_in_mdf_filter.Instance  = (MDF_Filter_TypeDef *)ADF1_Filter0;

      /* Configure ADF clock according to the requested audio frequency */
      if (MX_ADF1_ClockConfig(&haudio_in_mdf_filter, AudioInit->SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      else
      {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 0)
        if ((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
        {
          MDF_BlockMspInit(&haudio_in_mdf_filter);
        }
#else
        /* Register the MDF MSP Callbacks */
        if (AudioIn_IsMspCbValid[Instance] == 0U)
        {
          if (BSP_AUDIO_IN_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 0) */
        if (status == BSP_ERROR_NONE)
        {
          /* Prepare MDF peripheral initialization */
          MX_ADF_InitTypeDef mxAdfInit;
          if ((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
          {
            if (MX_ADF1_Init(&haudio_in_mdf_filter, &mxAdfInit) != HAL_OK)
            {
              status = BSP_ERROR_PERIPH_FAILURE;
            }

#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
            if (status == BSP_ERROR_NONE)
            {
              /* Register MDF filter TC, HT and Error callbacks */
              if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_ACQ_COMPLETE_CB_ID,
                                           MDF_AcquisitionCpltCallback) != HAL_OK)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
              else if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_ACQ_HALFCOMPLETE_CB_ID,
                                                MDF_AcquisitionHalfCpltCallback) != HAL_OK)
              {
                status = BSP_ERROR_PERIPH_FAILURE;
              }
              else
              {
                if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_ERROR_CB_ID, MDF_ErrorCallback) != HAL_OK)
                {
                  status = BSP_ERROR_PERIPH_FAILURE;
                }
              }
            }
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
          }

          if (status == BSP_ERROR_NONE)
          {
            /* Initialise transfer control flag */
            Audio_DmaDigMicRecHalfBuffCplt = 0;
            Audio_DmaDigMicRecBuffCplt     = 0;

            /* Update audio in context state */
            Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
          }
        }
      }
    }
  }
  return status;
}

/**
  * @brief  De-initialize the audio in peripherals.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RESET)
  {
    if (Instance == 0U)
    {
      /* Reset audio codec if not currently used by audio out instance 0 */
      if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
      {
        if (CS42L51_PowerDown() != BSP_ERROR_NONE)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* SAI peripheral de-initialization */
        if (HAL_SAI_DeInit(&haudio_in_sai) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        /* De-initialize audio codec if not currently used by audio out instance 0 */
        else
        {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 0)
          SAI_MspDeInit(&haudio_in_sai);
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 0) */
          if (Audio_Out_Ctx[0].State == AUDIO_OUT_STATE_RESET)
          {
            if (Audio_Drv->DeInit(Audio_CompObj) < 0)
            {
              status = BSP_ERROR_COMPONENT_FAILURE;
            }
          }
        }

        if (status == BSP_ERROR_NONE)
        {
          /* Update audio in context */
          Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RESET;
        }
      }
    }
    else
    {
      if (((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
          && (status == BSP_ERROR_NONE))
      {
        /* MDF peripheral de-initialization */
        if (HAL_MDF_DeInit(&haudio_in_mdf_filter) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        else
        {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 0)
          MDF_BlockMspDeInit(&haudio_in_mdf_filter);
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 0) */
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Update audio in context */
        Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RESET;
      }
    }
  }
  else
  {
    /* Nothing to do */
  }
  return status;
}

/**
  * @brief  Start recording audio stream to a data buffer for a determined size.
  * @param  Instance Audio in instance.
  * @param  pData Pointer on data buffer.
  * @param  NbrOfBytes Size of buffer in bytes. Maximum size is 65535 bytes.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Record(uint32_t Instance, uint8_t *pData, uint32_t NbrOfBytes)
{
  int32_t  status = BSP_ERROR_NONE;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (pData == NULL) || (NbrOfBytes > 65535U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check the internal buffer size */
  else if ((Instance == 1U) && (NbrOfBytes > BSP_AUDIO_IN_DEFAULT_BUFFER_SIZE))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* If no playback is on going, transmit some bytes on audio out stream to generate SAI clk and sync signals */
      if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_PLAYING) && (Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_PAUSE))
      {
        uint8_t TxData[2] = {0x00U, 0x00U};
        if (HAL_SAI_Transmit(&haudio_out_sai, TxData, 2, 1000) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (status == BSP_ERROR_NONE)
      {
        /* Call the audio Codec Play function */
        if (Audio_Drv->Play(Audio_CompObj) < 0)
        {
          status = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Initiate a DMA transfer of audio samples from the serial audio interface */
          /* Because only 16 bits per sample is supported, DMA transfer is in halfword size */
          if (HAL_SAI_Receive_DMA(&haudio_in_sai, pData,
                                  (uint16_t)(NbrOfBytes / (Audio_In_Ctx[Instance].BitsPerSample / 8U))) != HAL_OK)
          {
            status = BSP_ERROR_PERIPH_FAILURE;
          }
        }
      }
    }
    else /* Instance = 1 */
    {
      Audio_In_Ctx[Instance].pBuff = pData;
      Audio_In_Ctx[Instance].Size  = NbrOfBytes;

      /* Initialise transfer control flag */
      Audio_DmaDigMicRecHalfBuffCplt = 0;
      Audio_DmaDigMicRecBuffCplt     = 0;

      if (((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
          && (status == BSP_ERROR_NONE))
      {
        /* Initialize filter configuration parameters */
        filterConfig.DataSource      = MDF_DATA_SOURCE_BSMX;
        filterConfig.Delay           = 0U;
        filterConfig.CicMode         = MDF_ONE_FILTER_SINC5;
        filterConfig.DecimationRatio = MDF_DECIMATION_RATIO(Audio_In_Ctx[Instance].SampleRate);
        filterConfig.Offset          = 0;
        filterConfig.Gain            = 0;
        filterConfig.ReshapeFilter.Activation      = DISABLE;
        filterConfig.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
        filterConfig.HighPassFilter.Activation      = DISABLE;
        filterConfig.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;
        filterConfig.Integrator.Activation     = DISABLE;
        filterConfig.Integrator.Value          = 4U;
        filterConfig.Integrator.OutputDivision = MDF_INTEGRATOR_OUTPUT_NO_DIV;
        filterConfig.SoundActivity.Activation           = DISABLE;
        filterConfig.SoundActivity.Mode                 = MDF_SAD_VOICE_ACTIVITY_DETECTOR;
        filterConfig.SoundActivity.FrameSize            = MDF_SAD_8_PCM_SAMPLES;
        filterConfig.SoundActivity.Hysteresis           = DISABLE;
        filterConfig.SoundActivity.SoundTriggerEvent    = MDF_SAD_ENTER_DETECT;
        filterConfig.SoundActivity.DataMemoryTransfer   = MDF_SAD_NO_MEMORY_TRANSFER;
        filterConfig.SoundActivity.MinNoiseLevel        = 0U;
        filterConfig.SoundActivity.HangoverWindow       = MDF_SAD_HANGOVER_4_FRAMES;
        filterConfig.SoundActivity.LearningFrames       = MDF_SAD_LEARNING_2_FRAMES;
        filterConfig.SoundActivity.AmbientNoiseSlope    = 0U;
        filterConfig.SoundActivity.SignalNoiseThreshold = MDF_SAD_SIGNAL_NOISE_3_5DB;
        filterConfig.AcquisitionMode = MDF_MODE_ASYNC_CONT;
        filterConfig.FifoThreshold   = MDF_FIFO_THRESHOLD_NOT_EMPTY;
        filterConfig.DiscardSamples  = 1U;
        filterConfig.Trigger.Source  = MDF_FILTER_TRIG_TRGO;
        filterConfig.Trigger.Edge    = MDF_FILTER_TRIG_RISING_EDGE;
        filterConfig.SnapshotFormat  = MDF_SNAPSHOT_23BITS;

        /* Initialize DMA configuration parameters */
        dmaConfig.Address    = (uint32_t) Audio_In_Ctx[Instance].pBuff;
        dmaConfig.DataLength = Audio_In_Ctx[Instance].Size;
        dmaConfig.MsbOnly    = ENABLE;

        /* Call the Media layer start function for MIC1 channel */
        if (HAL_MDF_AcqStart_DMA(&haudio_in_mdf_filter, &filterConfig, &dmaConfig) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        if (HAL_MDF_GenerateTrgo(&haudio_in_mdf_filter) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }
  return status;
}

/**
  * @brief  Pause record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Pause DMA transfer of audio samples from the serial audio interface */
      if (HAL_SAI_DMAPause(&haudio_in_sai) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else
    {
      /* Call the Media layer stop function */
      if (((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
          && (status == BSP_ERROR_NONE))
      {
        if (HAL_MDF_AcqStop_DMA(&haudio_in_mdf_filter) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_PAUSE;
    }
  }
  return status;
}

/**
  * @brief  Resume record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Resume(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Resume DMA transfer of audio samples from the serial audio interface */
      if (HAL_SAI_DMAResume(&haudio_in_sai) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else /* Instance == 1 */
    {
      /* Initialise transfer control flag */
      Audio_DmaDigMicRecHalfBuffCplt = 0;
      Audio_DmaDigMicRecBuffCplt     = 0;

      if ((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
      {
        /* Call the Media layer start function for MIC channel */
        if (HAL_MDF_AcqStart_DMA(&haudio_in_mdf_filter, &filterConfig, &dmaConfig) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
        if (HAL_MDF_GenerateTrgo(&haudio_in_mdf_filter) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
      else
      {
        status = BSP_ERROR_WRONG_PARAM;
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_RECORDING;
    }
  }
  return status;
}

/**
  * @brief  Stop record of audio stream.
  * @param  Instance Audio in instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_STOP)
  {
    /* Nothing to do */
  }
  else if ((Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_RECORDING) &&
           (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_PAUSE))
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    if (Instance == 0U)
    {
      /* Call the audio codec stop function */
      if (Audio_Drv->Stop(Audio_CompObj, CS42L51_PDWN_SW) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Stop DMA transfer of audio samples from the serial audio interface */
      else
      {
        if (HAL_SAI_DMAStop(&haudio_in_sai) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    else
    {
      /* Call the Media layer stop function */
      if (((Audio_In_Ctx[Instance].Device & AUDIO_IN_DEVICE_DIGITAL_MIC) == AUDIO_IN_DEVICE_DIGITAL_MIC)
          && (status == BSP_ERROR_NONE))
      {
        if (HAL_MDF_AcqStop_DMA(&haudio_in_mdf_filter) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
    if (status == BSP_ERROR_NONE)
    {
      /* Update audio in state */
      Audio_In_Ctx[Instance].State = AUDIO_IN_STATE_STOP;
    }
  }
  return status;
}

/**
  * @brief  Set audio in volume.
  * @param  Instance Audio in instance.
  * @param  Volume Volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t status;

  if ((Instance >= AUDIO_IN_INSTANCES_NBR) || (Volume > 100U))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }

  else /* Feature not supported */
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return status;
}

/**
  * @brief  Get audio in volume.
  * @param  Instance Audio in instance.
  * @param  Volume Pointer to volume level in percentage from 0% to 100%.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t status;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else /* Feature not supported */
  {
    *Volume = 0U;
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return status;
}

/**
  * @brief  Set audio in sample rate.
  * @param  Instance Audio in instance.
  * @param  SampleRate Sample rate of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Check if playback on instance 0 is on going and corresponding sample rate */
  else if ((Audio_Out_Ctx[0].State != AUDIO_OUT_STATE_RESET) &&
           (Audio_Out_Ctx[0].SampleRate != SampleRate))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check if sample rate is modified */
  else if (Audio_In_Ctx[Instance].SampleRate == SampleRate)
  {
    /* Nothing to do */
  }
  else
  {
    if (Instance == 0U)
    {
      /* Update SAI1 clock config */
      haudio_in_sai.Init.AudioFrequency = SampleRate;
      haudio_out_sai.Init.AudioFrequency = SampleRate;
      if (MX_SAI1_ClockConfig(&haudio_in_sai, SampleRate) != HAL_OK)
      {
        status = BSP_ERROR_CLOCK_FAILURE;
      }
      /* Re-initialize SAI1 with new sample rate */
      else if (HAL_SAI_Init(&haudio_in_sai) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_SAI_Init(&haudio_out_sai) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
      /* Register SAI TC, HT and Error callbacks */
      else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_COMPLETE_CB_ID, SAI_RxCpltCallback) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_RX_HALFCOMPLETE_CB_ID, SAI_RxHalfCpltCallback)
               != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_ERROR_CB_ID, SAI_ErrorCallback) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
      /* Store new sample rate on audio in context */
      else
      {
        Audio_In_Ctx[Instance].SampleRate = SampleRate;
      }
    }
    else /* Instance == 1U */
    {
      /* Sample rate will change on audio record restart */
      Audio_In_Ctx[Instance].SampleRate = SampleRate;
    }
  }
  return status;
}

/**
  * @brief  Get audio in sample rate.
  * @param  Instance Audio in instance.
  * @param  SampleRate Pointer to sample rate of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio in sample rate */
  else
  {
    *SampleRate = Audio_In_Ctx[Instance].SampleRate;
  }
  return status;
}

/**
  * @brief  Set audio in device.
  * @param  Instance Audio in instance.
  * @param  Device Device of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t status;

  UNUSED(Device);

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  return status;
}

/**
  * @brief  Get audio in device.
  * @param  Instance Audio in instance.
  * @param  Device Pointer to device of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current audio in device */
  else
  {
    *Device = Audio_In_Ctx[Instance].Device;
  }
  return status;
}

/**
  * @brief  Set bits per sample for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  BitsPerSample Bits per sample of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if (BitsPerSample != AUDIO_RESOLUTION_16B)
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because there is only one bits per sample supported (AUDIO_RESOLUTION_16b) */
  }
  return status;
}

/**
  * @brief  Get bits per sample for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  BitsPerSample Pointer to bits per sample of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current bits per sample of audio in stream */
  else
  {
    *BitsPerSample = Audio_In_Ctx[Instance].BitsPerSample;
  }
  return status;
}

/**
  * @brief  Set channels number for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  ChannelNbr Channels number of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((Instance == 0U) && (ChannelNbr != 1U))
  {
    status = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State != AUDIO_IN_STATE_STOP)
  {
    status = BSP_ERROR_BUSY;
  }
  else
  {
    /* Nothing to do because channels are already configurated and can't be modified */
  }
  return status;
}

/**
  * @brief  Get channels number for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  ChannelNbr Pointer to channels number of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Check audio in state */
  else if (Audio_In_Ctx[Instance].State == AUDIO_IN_STATE_RESET)
  {
    status = BSP_ERROR_BUSY;
  }
  /* Get the current channels number of audio in stream */
  else
  {
    *ChannelNbr = Audio_In_Ctx[Instance].ChannelsNbr;
  }
  return status;
}

/**
  * @brief  Get current state for the audio in stream.
  * @param  Instance Audio in instance.
  * @param  State Pointer to state of the audio in stream.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  /* Get the current state of audio in stream */
  else
  {
    *State = Audio_In_Ctx[Instance].State;
  }
  return status;
}

#if ((USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1))
/**
  * @brief  Register default BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @retval BSP status.
  */
int32_t BSP_AUDIO_IN_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == 0U)
    {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
      /* Register SAI MspInit/MspDeInit callbacks */
      if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPINIT_CB_ID, SAI_MspInit) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPDEINIT_CB_ID, SAI_MspDeInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS == 1 */
    }
    else /* Instance == 1 */
    {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
      /* Register MDF MspInit/MspDeInit callbacks */
      if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_MSPINIT_CB_ID, MDF_BlockMspInit) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_MSPDEINIT_CB_ID, MDF_BlockMspDeInit) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_MDF_REGISTER_CALLBACKS == 1 */
    }

    if (status == BSP_ERROR_NONE)
    {
      AudioIn_IsMspCbValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP AUDIO IN msp callbacks.
  * @param  Instance AUDIO IN Instance.
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_AUDIO_IN_RegisterMspCallbacks(uint32_t Instance, BSP_AUDIO_IN_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == 0U)
    {
#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
      /* Register MspInit/MspDeInit callbacks */
      if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPINIT_CB_ID, CallBacks->pMspSaiInitCb) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_SAI_RegisterCallback(&haudio_in_sai, HAL_SAI_MSPDEINIT_CB_ID, CallBacks->pMspSaiDeInitCb) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_SAI_REGISTER_CALLBACKS == 1 */
    }
    else /* Instance == 1 */
    {
#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
      /* Register MDF MspInit/MspDeInit callbacks */
      if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_MSPINIT_CB_ID, CallBacks->pMspMdfInitCb) != HAL_OK)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if (HAL_MDF_RegisterCallback(&haudio_in_mdf_filter, HAL_MDF_MSPDEINIT_CB_ID, CallBacks->pMspMdfDeInitCb)
            != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_MDF_REGISTER_CALLBACKS == 1 */
    }
  }
  if (status == BSP_ERROR_NONE)
  {
    AudioIn_IsMspCbValid[Instance] = 1U;
  }
  /* Return BSP status */
  return status;
}
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) || (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Manage the BSP audio in transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manage the BSP audio in half transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  Manages the BSP audio in error event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
__weak void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief  BSP AUDIO IN interrupt handler.
  * @param  Instance Audio in instance.
  * @param  Device Device of the audio in stream.
  * @retval None.
  */
void BSP_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device)
{
  UNUSED(Device);

  if (Instance == 0U)
  {
    HAL_DMA_IRQHandler(haudio_in_sai.hdmarx);
  }
  else /* Instance == 1U */
  {
    HAL_DMA_IRQHandler(&haudio_mdf);
  }
}
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_AUDIO_Private_Functions AUDIO Private Functions
  * @{
  */

/**
  * @brief  Initialize MDF filter MSP.
  * @param  hmdf MDF filter handle.
  * @retval None.
  */
static void MDF_BlockMspInit(MDF_HandleTypeDef *hmdf)
{
  static DMA_NodeTypeDef     DmaNode;
  DMA_NodeConfTypeDef        dmaNodeConfig;
  GPIO_InitTypeDef           GPIO_InitStruct;

  /* Reset ADF1 and enable clock */
  __HAL_RCC_ADF1_FORCE_RESET();
  __HAL_RCC_ADF1_RELEASE_RESET();
  __HAL_RCC_ADF1_CLK_ENABLE();

  /* Enable ADF clock */
  AUDIO_ADF1_CLK_ENABLE();

  /* ADF pins configuration: ADF1_CKOUT, ADF1_DATIN1 pins */
  AUDIO_ADF1_CCK0_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_ADF1_CCK0_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_ADF1_CCK0_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_ADF1_CCK0_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_ADF1_SDIN0_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_ADF1_SDIN0_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_ADF1_SDIN0_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_ADF1_SDIN0_GPIO_PORT, &GPIO_InitStruct);

  /* Enable the DMA clock */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  if (MdfQueue.Head == NULL)
  {
    dmaNodeConfig.NodeType                    = DMA_GPDMA_LINEAR_NODE;
    dmaNodeConfig.Init                        = haudio_mdf.Init;
    dmaNodeConfig.Init.Request                = GPDMA1_REQUEST_ADF1_FLT0;
    dmaNodeConfig.Init.BlkHWRequest           = DMA_BREQ_SINGLE_BURST;
    dmaNodeConfig.Init.Direction              = DMA_PERIPH_TO_MEMORY;
    dmaNodeConfig.Init.SrcInc                 = DMA_SINC_FIXED;
    dmaNodeConfig.Init.DestInc                = DMA_DINC_INCREMENTED;
    if (Audio_In_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
    {
      dmaNodeConfig.Init.SrcDataWidth         = DMA_SRC_DATAWIDTH_HALFWORD;
      dmaNodeConfig.Init.DestDataWidth        = DMA_DEST_DATAWIDTH_HALFWORD;
    }
    else /* AUDIO_RESOLUTION_24b */
    {
      dmaNodeConfig.Init.SrcDataWidth         = DMA_SRC_DATAWIDTH_WORD;
      dmaNodeConfig.Init.DestDataWidth        = DMA_DEST_DATAWIDTH_WORD;
    }
    dmaNodeConfig.Init.Priority               = DMA_HIGH_PRIORITY;
    dmaNodeConfig.Init.SrcBurstLength         = 1;
    dmaNodeConfig.Init.DestBurstLength        = 1;
    dmaNodeConfig.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    dmaNodeConfig.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
    dmaNodeConfig.Init.Mode                   = DMA_NORMAL;

    dmaNodeConfig.DataHandlingConfig.DataExchange       = DMA_EXCHANGE_NONE;
    dmaNodeConfig.DataHandlingConfig.DataAlignment      = DMA_DATA_UNPACK;
    dmaNodeConfig.TriggerConfig.TriggerMode             = DMA_TRIGM_BLOCK_TRANSFER;
    dmaNodeConfig.TriggerConfig.TriggerPolarity         = DMA_TRIG_POLARITY_MASKED;
    dmaNodeConfig.TriggerConfig.TriggerSelection        = GPDMA1_TRIGGER_EXTI_LINE0;
    dmaNodeConfig.RepeatBlockConfig.RepeatCount         = 1U;
    dmaNodeConfig.RepeatBlockConfig.SrcAddrOffset       = 0;
    dmaNodeConfig.RepeatBlockConfig.DestAddrOffset      = 0;
    dmaNodeConfig.RepeatBlockConfig.BlkSrcAddrOffset    = 0;
    dmaNodeConfig.RepeatBlockConfig.BlkDestAddrOffset   = 0;

    /* Build node */
    if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &DmaNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }

    /* Insert node to queue */
    if (HAL_DMAEx_List_InsertNode(&MdfQueue, NULL, &DmaNode) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }

    /* Set queue in circular mode */
    if (HAL_DMAEx_List_SetCircularMode(&MdfQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }
  }
  haudio_mdf.Instance               = GPDMA1_Channel6;

  /* Fill linked list structure */
  haudio_mdf.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  haudio_mdf.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  haudio_mdf.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
  haudio_mdf.InitLinkedList.TransferEventMode = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  haudio_mdf.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* DMA linked list init */
  if (HAL_DMAEx_List_Init(&haudio_mdf) != HAL_OK)
  {
    BSP_AUDIO_IN_Error_CallBack(1);
  }

  /* Link queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&haudio_mdf, &MdfQueue) != HAL_OK)
  {
    BSP_AUDIO_IN_Error_CallBack(1);
  }

  __HAL_LINKDMA(hmdf, hdma, haudio_mdf);

  HAL_NVIC_SetPriority(GPDMA1_Channel6_IRQn, BSP_AUDIO_IN_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);
}

/**
  * @brief  DeInitialize MDF filter MSP.
  * @param  hmdf MDF filter handle.
  * @retval None.
  */
static void MDF_BlockMspDeInit(MDF_HandleTypeDef *hmdf)
{
  /* De-initialize ADF1_CKOUT, ADF1_DATIN1 pins */
  HAL_GPIO_DeInit(AUDIO_ADF1_CCK0_GPIO_PORT, AUDIO_ADF1_CCK0_GPIO_PIN);
  HAL_GPIO_DeInit(AUDIO_ADF1_SDIN0_GPIO_PORT, AUDIO_ADF1_SDIN0_GPIO_PIN);

  if (hmdf->Instance == ADF1_Filter0)
  {
    /* Disable ADF1 clock */
    AUDIO_ADF1_CLK_DISABLE();

    /* Disable DMA  Channel IRQ */
    HAL_NVIC_DisableIRQ(GPDMA1_Channel6_IRQn);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&haudio_mdf) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }

    /* Reset MdfQueue */
    if (HAL_DMAEx_List_ResetQ(&MdfQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(1);
    }
  }
}

#if (USE_HAL_MDF_REGISTER_CALLBACKS == 1)
/**
  * @brief  MDF filter regular conversion complete callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
static void MDF_AcquisitionCpltCallback(MDF_HandleTypeDef *hadf_filter)
{
  /* Invoke 'TransferCompete' callback function */
  if (hadf_filter == &haudio_in_mdf_filter)
  {
    Audio_DmaDigMicRecBuffCplt = 1;
  }

  if ((Audio_DmaDigMicRecBuffCplt != 0U) && (Audio_In_Ctx[1].Device == AUDIO_IN_DEVICE_DIGITAL_MIC))
  {
    BSP_AUDIO_IN_TransferComplete_CallBack(1);
    Audio_DmaDigMicRecBuffCplt = 0;
  }
}

/**
  * @brief  MDF filter regular conversion half complete callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
static void MDF_AcquisitionHalfCpltCallback(MDF_HandleTypeDef *hadf_filter)
{
  /* Invoke the 'HalfTransfer' callback function */
  if (hadf_filter == &haudio_in_mdf_filter)
  {
    Audio_DmaDigMicRecHalfBuffCplt = 1;
  }

  if ((Audio_DmaDigMicRecHalfBuffCplt != 0U) && (Audio_In_Ctx[1].Device == AUDIO_IN_DEVICE_DIGITAL_MIC))
  {
    BSP_AUDIO_IN_HalfTransfer_CallBack(1);
    Audio_DmaDigMicRecHalfBuffCplt = 0;
  }
}

/**
  * @brief  MDF filter error callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
static void MDF_ErrorCallback(MDF_HandleTypeDef *hadf_filter)
{
  UNUSED(hadf_filter);

  BSP_AUDIO_IN_Error_CallBack(1);
}
#else /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */
/**
  * @brief  MDF filter regular conversion complete callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
void HAL_MDF_AcqCpltCallback(MDF_HandleTypeDef *hadf_filter)
{
  /* Invoke 'TransferCompete' callback function */
  if (hadf_filter == &haudio_in_mdf_filter)
  {
    Audio_DmaDigMicRecBuffCplt = 1;
  }

  if ((Audio_DmaDigMicRecBuffCplt != 0U) && (Audio_In_Ctx[1].Device == AUDIO_IN_DEVICE_DIGITAL_MIC))
  {
    BSP_AUDIO_IN_TransferComplete_CallBack(1);
    Audio_DmaDigMicRecBuffCplt = 0;
  }
}

/**
  * @brief  MDF filter regular conversion half complete callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
void HAL_MDF_AcqHalfCpltCallback(MDF_HandleTypeDef *hadf_filter)
{
  /* Invoke the 'HalfTransfer' callback function */
  if (hadf_filter == &haudio_in_mdf_filter)
  {
    Audio_DmaDigMicRecHalfBuffCplt = 1;
  }

  if ((Audio_DmaDigMicRecHalfBuffCplt != 0U) && (Audio_In_Ctx[1].Device == AUDIO_IN_DEVICE_DIGITAL_MIC))
  {
    BSP_AUDIO_IN_HalfTransfer_CallBack(1);
    Audio_DmaDigMicRecHalfBuffCplt = 0;
  }
}

/**
  * @brief  MDF filter error callback.
  * @param  hadf_filter ADF filter handle.
  * @retval None.
  */
void HAL_MDF_ErrorCallback(MDF_HandleTypeDef *hadf_filter)
{
  UNUSED(hadf_filter);

  BSP_AUDIO_IN_Error_CallBack(1);
}
#endif /* (USE_HAL_MDF_REGISTER_CALLBACKS == 1) */

/**
  * @brief  ADF1 clock Config.
  * @param  hAdfBlock ADF block handle.
  * @param  SampleRate Audio sample rate used to record the audio stream.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_ADF1_ClockConfig(MDF_HandleTypeDef *hAdfBlock, uint32_t SampleRate)
{
  HAL_StatusTypeDef status = HAL_OK;
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hAdfBlock);
  UNUSED(SampleRate);

  /* ADF Clock configuration:
  PLL3_VCO Input = MSI_4Mhz/PLL3M = 4 Mhz
  PLL3_VCO Output = PLL3_VCO Input * PLL3N = 320 Mhz
  ADF_CLK_x = PLL3_VCO Output/PLL3Q = 320/28 = 11.428 Mhz */
  RCC_ExCLKInitStruct.PLL3.PLL3Source = RCC_PLLSOURCE_MSI;
  RCC_ExCLKInitStruct.PLL3.PLL3M = 1;
  RCC_ExCLKInitStruct.PLL3.PLL3N = 80;
  RCC_ExCLKInitStruct.PLL3.PLL3P = 28;
  RCC_ExCLKInitStruct.PLL3.PLL3Q = 28;
  RCC_ExCLKInitStruct.PLL3.PLL3R = 2;
  RCC_ExCLKInitStruct.PLL3.PLL3RGE = 0;
  RCC_ExCLKInitStruct.PLL3.PLL3FRACN = 0;
  RCC_ExCLKInitStruct.PLL3.PLL3ClockOut = RCC_PLL3_DIVQ;
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADF1;
  RCC_ExCLKInitStruct.Adf1ClockSelection = RCC_ADF1CLKSOURCE_PLL3;

  if (HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Initialize ADF1.
  * @param  hAdfBlock ADF channel handle.
  * @param  MXInit ADF configuration structure.
  * @retval HAL_status.
  */
__weak HAL_StatusTypeDef MX_ADF1_Init(MDF_HandleTypeDef *hAdfBlock, MX_ADF_InitTypeDef *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  UNUSED(MXInit);

  /*  Initialize  MDF */
  hAdfBlock->Init.CommonParam.InterleavedFilters     = 0U;
  hAdfBlock->Init.CommonParam.ProcClockDivider       = 1U;
  hAdfBlock->Init.CommonParam.OutputClock.Activation = ENABLE;
  hAdfBlock->Init.CommonParam.OutputClock.Pins       = MDF_OUTPUT_CLOCK_0;
  hAdfBlock->Init.CommonParam.OutputClock.Divider    = 16U; /* ADF_CCK = 11.428MHz/16 = 714 KHz */
  hAdfBlock->Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  hAdfBlock->Init.CommonParam.OutputClock.Trigger.Source     = MDF_CLOCK_TRIG_TRGO;
  hAdfBlock->Init.CommonParam.OutputClock.Trigger.Edge       = MDF_CLOCK_TRIG_RISING_EDGE;

  hAdfBlock->Init.SerialInterface.Activation         = ENABLE;
  hAdfBlock->Init.SerialInterface.Mode               = MDF_SITF_NORMAL_SPI_MODE;
  hAdfBlock->Init.SerialInterface.ClockSource        = MDF_SITF_CCK0_SOURCE;
  hAdfBlock->Init.SerialInterface.Threshold          = 31U;

  hAdfBlock->Init.FilterBistream                     = MDF_BITSTREAM5_RISING;

  if (HAL_OK != HAL_MDF_Init(hAdfBlock))
  {
    status = HAL_ERROR;
  }

  return status;
}
/**
  * @brief  Probe the CS42L51 audio codec.
  * @retval BSP status.
  */
static int32_t CS42L51_Probe(void)
{
  int32_t                  status = BSP_ERROR_NONE;
  CS42L51_IO_t             IOCtx;
  uint32_t                 cs42l51_id;
  static CS42L51_Object_t  CS42L51Obj;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if (CS42L51_RegisterBusIO(&CS42L51Obj, &IOCtx) != CS42L51_OK)
  {
    status = BSP_ERROR_BUS_FAILURE;
  }
  else if (CS42L51_ReadID(&CS42L51Obj, &cs42l51_id) != CS42L51_OK)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if ((cs42l51_id & CS42L51_ID_MASK) != CS42L51_ID)
  {
    status = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    Audio_Drv = (AUDIO_Drv_t *) &CS42L51_Driver;
    Audio_CompObj = &CS42L51Obj;
  }

  return status;
}

/**
  * @brief  Initialize SAI MSP.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  static DMA_NodeTypeDef TxNode, RxNode;
  static DMA_NodeConfTypeDef dmaNodeConfig;
  GPIO_InitTypeDef           GPIO_InitStruct;

  /* Enable SAI clock */
  AUDIO_SAI1_CLK_ENABLE();

  /* SAI pins configuration: FS, SCK, MCLK and SD pins */
  AUDIO_SAI1_MCLK_B_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_SAI1_MCLK_B_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_SAI1_MCLK_B_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_SAI1_MCLK_B_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_SAI1_FS_B_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_SAI1_FS_B_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_SAI1_FS_B_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_SAI1_FS_B_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_SAI1_SCK_B_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_SAI1_SCK_B_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_SAI1_SCK_B_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_SAI1_SCK_B_GPIO_PORT, &GPIO_InitStruct);

  AUDIO_SAI1_SD_B_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Alternate = AUDIO_SAI1_SD_B_GPIO_AF;
  GPIO_InitStruct.Pin       = AUDIO_SAI1_SD_B_GPIO_PIN;
  HAL_GPIO_Init(AUDIO_SAI1_SD_B_GPIO_PORT, &GPIO_InitStruct);

  if (hsai->Instance == SAI1_Block_A)  /* Rx */
  {
    /* SAI pins configuration: SD pin */
    AUDIO_SAI1_SD_A_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Alternate = AUDIO_SAI1_SD_A_GPIO_AF;
    GPIO_InitStruct.Pin       = AUDIO_SAI1_SD_A_GPIO_PIN;
    HAL_GPIO_Init(AUDIO_SAI1_SD_A_GPIO_PORT, &GPIO_InitStruct);

    /* Configure the hDmaSaiRx handle parameters */
    __HAL_RCC_GPDMA1_CLK_ENABLE();

    if (SAIRxQueue.Head == NULL)
    {
      /* DMA for Rx */
      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = GPDMA1_REQUEST_SAI1_A;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
      if (Audio_In_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
      }
      else /* AUDIO_RESOLUTION_24b */
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
      }
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeRx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Insert NodeTx to SAI queue */
      if (HAL_DMAEx_List_InsertNode_Tail(&SAIRxQueue, &RxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Set queue circular mode for sai queue */
      if (HAL_DMAEx_List_SetCircularMode(&SAIRxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }
    }

    /* DMA for Rx */
    hDmaSaiRx.Instance                         = GPDMA1_Channel1;

    hDmaSaiRx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
    hDmaSaiRx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
    hDmaSaiRx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
    hDmaSaiRx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hDmaSaiRx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

    /* DMA linked list init */
    if (HAL_DMAEx_List_Init(&hDmaSaiRx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Link SAI queue to DMA channel */
    if (HAL_DMAEx_List_LinkQ(&hDmaSaiRx, &SAIRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmarx, hDmaSaiRx);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, BSP_AUDIO_IN_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
  }
  else /* SAI1_BlockB */
  {
    /* Configure the hDmaSaiTx handle parameters */
    __HAL_RCC_GPDMA1_CLK_ENABLE();

    if (SAITxQueue.Head == NULL)
    {
      /* DMA for Tx */
      /* Set node type */
      dmaNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
      /* Set common node parameters */
      dmaNodeConfig.Init.Request                        = GPDMA1_REQUEST_SAI1_B;
      dmaNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
      dmaNodeConfig.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
      dmaNodeConfig.Init.SrcInc                         = DMA_SINC_INCREMENTED;
      dmaNodeConfig.Init.DestInc                        = DMA_DINC_FIXED;
      if (Audio_Out_Ctx[0].BitsPerSample == AUDIO_RESOLUTION_16B)
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_HALFWORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_HALFWORD;
      }
      else /* AUDIO_RESOLUTION_24b */
      {
        dmaNodeConfig.Init.SrcDataWidth                 = DMA_SRC_DATAWIDTH_WORD;
        dmaNodeConfig.Init.DestDataWidth                = DMA_DEST_DATAWIDTH_WORD;
      }
      dmaNodeConfig.Init.SrcBurstLength                 = 1;
      dmaNodeConfig.Init.DestBurstLength                = 1;
      dmaNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
      dmaNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
      dmaNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
      /* Set node data handling parameters */
      dmaNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
      dmaNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
      /* Set node trigger parameters */
      dmaNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

      /* Build NodeTx */
      if (HAL_DMAEx_List_BuildNode(&dmaNodeConfig, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Insert NodeTx to SAI queue */
      if (HAL_DMAEx_List_InsertNode_Tail(&SAITxQueue, &TxNode) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }

      /* Set queue circular mode for sai queue */
      if (HAL_DMAEx_List_SetCircularMode(&SAITxQueue) != HAL_OK)
      {
        BSP_AUDIO_IN_Error_CallBack(0);
      }
    }

    /* DMA for Tx */
    hDmaSaiTx.Instance                         = GPDMA1_Channel2;

    hDmaSaiTx.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
    hDmaSaiTx.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
    hDmaSaiTx.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
    hDmaSaiTx.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hDmaSaiTx.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

    /* DMA linked list init */
    if (HAL_DMAEx_List_Init(&hDmaSaiTx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Link SAI queue to DMA channel */
    if (HAL_DMAEx_List_LinkQ(&hDmaSaiTx, &SAITxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hDmaSaiTx);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(GPDMA1_Channel2_IRQn, BSP_AUDIO_OUT_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel2_IRQn);
  }
}

/**
  * @brief  Deinitialize SAI MSP.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_MspDeInit(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_A)
  {
    /* Disable SAI DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(GPDMA1_Channel1_IRQn);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaSaiRx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Reset RxQueue */
    if (HAL_DMAEx_List_ResetQ(&SAIRxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Don't disable the DMA clock potentially used for other SAI block */

    /* De-initialize SD pin */
    HAL_GPIO_DeInit(AUDIO_SAI1_SD_A_GPIO_PORT, AUDIO_SAI1_SD_A_GPIO_PIN);

    /* Don't disable SAI clock potentially used for other SAI block */
    /*AUDIO_SAI1_CLK_DISABLE(); */
  }
  else /* SAI1_BlockB */
  {
    /* Disable SAI DMA Channel IRQ */
    HAL_NVIC_DisableIRQ(GPDMA1_Channel2_IRQn);

    /* Reset the DMA Channel configuration*/
    if (HAL_DMAEx_List_DeInit(&hDmaSaiTx) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Reset TxQueue */
    if (HAL_DMAEx_List_ResetQ(&SAITxQueue) != HAL_OK)
    {
      BSP_AUDIO_IN_Error_CallBack(0);
    }

    /* Don't disable the DMA clock potentially used for other SAI block */
    /* De-initialize FS, SCK, MCK and SD pins*/
    HAL_GPIO_DeInit(AUDIO_SAI1_MCLK_B_GPIO_PORT, AUDIO_SAI1_MCLK_B_GPIO_PIN);
    HAL_GPIO_DeInit(AUDIO_SAI1_FS_B_GPIO_PORT, AUDIO_SAI1_FS_B_GPIO_PIN);
    HAL_GPIO_DeInit(AUDIO_SAI1_SCK_B_GPIO_PORT, AUDIO_SAI1_SCK_B_GPIO_PIN);
    HAL_GPIO_DeInit(AUDIO_SAI1_SD_B_GPIO_PORT, AUDIO_SAI1_SD_B_GPIO_PIN);

    /* Don't disable SAI clock potentially used for other SAI block */
    /*AUDIO_SAI1_CLK_DISABLE(); */
  }
}

#if (USE_HAL_SAI_REGISTER_CALLBACKS == 1)
/**
  * @brief  SAI Tx transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_TransferComplete_CallBack(0);
  }
}

/**
  * @brief  SAI Tx half transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
  }
}

/**
  * @brief  SAI Rx transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_TransferComplete_CallBack(0);
  }
}

/**
  * @brief  SAI Rx half transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_HalfTransfer_CallBack(0);
  }
}

/**
  * @brief  SAI error callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
static void SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_Error_CallBack(0);
  }
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_Error_CallBack(0);
  }
}
#else /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */
/**
  * @brief  SAI Tx transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_TransferComplete_CallBack(0);
  }
}

/**
  * @brief  SAI Tx half transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_HalfTransfer_CallBack(0);
  }
}

/**
  * @brief  SAI Rx transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_TransferComplete_CallBack(0);
  }
}

/**
  * @brief  SAI Rx half transfer complete callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_HalfTransfer_CallBack(0);
  }
}

/**
  * @brief  SAI error callback.
  * @param  hsai SAI handle.
  * @retval None.
  */
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai->Instance == SAI1_Block_B)
  {
    BSP_AUDIO_OUT_Error_CallBack(0);
  }
  if (hsai->Instance == SAI1_Block_A)
  {
    BSP_AUDIO_IN_Error_CallBack(0);
  }
}
#endif /* (USE_HAL_SAI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Un-reset CS42L51.
  * @retval BSP status.
  */
static int32_t CS42L51_PowerUp(void)
{
  int32_t status;

#if (USE_BSP_IO_CLASS == 1)
  BSP_IO_Init_t IO_Init;

  /* Initialize the BSP IO driver and configure the CS42L51 reset pin */
  status = BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);
  if (status == BSP_ERROR_NONE)
  {
    IO_Init.Pin  = IO_PIN_15;
    IO_Init.Mode = IO_MODE_OUTPUT_PP;
    IO_Init.Pull = IO_PULLDOWN;
    status = BSP_IO_Init(0, &IO_Init);
    if (status == BSP_ERROR_NONE)
    {
      /* Un-reset the CS42L51 */
      status = BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_SET);
    }
  }

  /* Wait 1ms according CS42L51 datasheet */
  HAL_Delay(1);
#else /* USE_BSP_IO_CLASS == 1 */
  status = BSP_ERROR_NONE;
#endif /* USE_BSP_IO_CLASS == 1 */

  return status;
}

/**
  * @brief  Reset CS42L51.
  * @retval BSP status.
  */
static int32_t CS42L51_PowerDown(void)
{
  int32_t status;

#if (USE_BSP_IO_CLASS == 1)
  /* Reset the CS42L51 */
  status = BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_RESET);
#else /* USE_BSP_IO_CLASS == 1 */
  status = BSP_ERROR_NONE;
#endif /* USE_BSP_IO_CLASS == 1 */

  return status;
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
