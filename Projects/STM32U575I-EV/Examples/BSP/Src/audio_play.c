/**
  ******************************************************************************
  * @file    audio_play.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use BSP AUDIO features for the play.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

} WAVE_FormatTypeDef;

/* Private define ------------------------------------------------------------*/
/* Audio file size and start address are defined here since the Audio file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_ADDRESS    0x08040000   /* Audio file address */  
#define AUDIO_FILE_SIZE      (180*1024)
#define PLAY_HEADER          0x2C
#define PLAY_BUFF_SIZE       4096
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t PauseStatus = 0;
uint32_t AudioPlayTest = 0;

int16_t       PlayBuff[PLAY_BUFF_SIZE];
__IO uint32_t PlaybackPosition;
__IO uint8_t  Volume = 80;

/* Private function prototypes -----------------------------------------------*/
static void AudioPlay_SetHint(void);
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */
void AudioPlay_demo(void)
{  
  WAVE_FormatTypeDef *waveformat =  NULL;
  uint8_t Volume_string[20] = {0};
  BSP_AUDIO_Init_t AudioInit;
  uint8_t status = 0;

  PlaybackPosition   = (2*PLAY_BUFF_SIZE) + PLAY_HEADER;
  
  AudioPlay_SetHint();

  AudioPlayTest = 1;
  
  /* Check if the audio buffer has been loaded in flash */
  if (*((uint64_t *)AUDIO_FILE_ADDRESS) != 0x017EFE2446464952 )
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);    
    UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"Initialization problem", CENTER_MODE); 
    UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"No Audio data in Flash", CENTER_MODE); 
    status = 1;
  }
  else
  {
    /* Retrieve Wave Sample rate*/
    waveformat = (WAVE_FormatTypeDef*) AUDIO_FILE_ADDRESS;
    AudioPlay_DisplayInfos(waveformat);

    /* Initialize Audio Device */
    AudioInit.Device        = AUDIO_OUT_HEADPHONE;
    AudioInit.SampleRate    = waveformat->SampleRate;
    AudioInit.BitsPerSample = waveformat->BitPerSample;
    AudioInit.ChannelsNbr   = waveformat->NbrChannels;
    AudioInit.Volume        = Volume;
    if (BSP_AUDIO_OUT_Init(0, &AudioInit) != BSP_ERROR_NONE)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);    
      UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"Initialization problem", CENTER_MODE); 
      UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"Audio Codec not detected", CENTER_MODE);
      status = 1;
    }
    else
    {
      /* Initialize the data buffer */
      for(int i=0; i < PLAY_BUFF_SIZE; i++)
      {
        PlayBuff[i]=*((__IO uint16_t *)(AUDIO_FILE_ADDRESS + PLAY_HEADER + (2*i)));
      }
      
      /* Start the audio player */
      if (BSP_AUDIO_OUT_Play(0, (uint8_t *) PlayBuff, 2*PLAY_BUFF_SIZE) == BSP_ERROR_NONE)
      {
        /* Turn ON LED green: start of Audio file play */
        BSP_LED_On(LED_GREEN);
        
        UTIL_LCD_DisplayStringAt(20, 210, (uint8_t *)"Playback on-going", LEFT_MODE);
        sprintf((char *) Volume_string, "Volume : %lu%% ", (unsigned long)Volume);
        UTIL_LCD_DisplayStringAt(20, 210, Volume_string, RIGHT_MODE);
      }
      else
      {
        Error_Handler();
      }
    }
  }
  
  /* Infinite loop */
  while (UserButtonPressed != SET)
  { 
    if (status == 0)
    {
      if (JoyPinPressed == JOY_SEL)
      {
        if (PauseStatus == 0)
        {
          /* Turn ON LED orange: Audio play in pause */
          BSP_LED_On(LED_YELLOW);
      
          if (BSP_AUDIO_OUT_Pause(0) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }        
          PauseStatus = 1;
          HAL_Delay(1000);
          UTIL_LCD_DisplayStringAt(20, 210, (uint8_t *)"Playback paused  ", LEFT_MODE);
        }
        else
        {
          /* Turn OFF LED orange: Audio play running */
          BSP_LED_Off(LED_YELLOW);

          if (BSP_AUDIO_OUT_Resume(0) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }        
          PauseStatus = 0;
          UTIL_LCD_DisplayStringAt(20, 210, (uint8_t *)"Playback on-going", LEFT_MODE);
        }
        JoyPinPressed = JOY_NONE;
      }
      if (JoyPinPressed == JOY_UP)
      {
        if (Volume < 100)
        {
          /* Decrease volume */
          Volume += 5;
          if (BSP_AUDIO_OUT_SetVolume(0, Volume) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }
        }
        JoyPinPressed = JOY_NONE;
        sprintf((char *) Volume_string, " Volume : %d%% ", Volume);
        UTIL_LCD_DisplayStringAt(20, 210, Volume_string, RIGHT_MODE);
      }
      if (JoyPinPressed == JOY_DOWN)
      {
        if (Volume > 50)
        {
          /* Decrease volume */
          Volume -= 5;
          if (BSP_AUDIO_OUT_SetVolume(0, Volume) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }
        }
        JoyPinPressed = JOY_NONE;
        sprintf((char *) Volume_string, " Volume : %d%% ", Volume);
        UTIL_LCD_DisplayStringAt(20, 210, Volume_string, RIGHT_MODE);
      }
    }
  }

  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  UserButtonPressed = RESET;
  
  /* Stop Player before close Test */
  if(status == 0)
  {
    if (BSP_AUDIO_OUT_Stop(0) != BSP_ERROR_NONE)
    {
      /* Audio Stop error */
      Error_Handler();
    }

    /* De-initialize playback */
    if(BSP_AUDIO_OUT_DeInit(0) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }

  AudioPlayTest = 0;

  /* Turn OFF LED green: stop of Audio file play */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_YELLOW); 
}

/**
  * @brief  Display audio play demo hint
  * @param  None
  * @retval None
  */
static void AudioPlay_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK); 
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Play", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User push-button to stop play", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Press SEL to pause/resume playback", CENTER_MODE); 
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press UP/DOWN to change Volume", CENTER_MODE);   
  
  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
}

/**
  * @brief  Display audio play demo hint
  * @param  format : structure containing information of the file
  * @retval None
  */
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format)
{
  uint8_t string[50] = {0};

  sprintf((char *) string, "Sampling frequency : %lu Hz", (unsigned long)format->SampleRate);
  UTIL_LCD_DisplayStringAt(20, 100, string, CENTER_MODE);

  if (format->NbrChannels == 2)
  {
    sprintf((char *) string, "Format : %d bits stereo", format->BitPerSample);
    UTIL_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
  else if (format->NbrChannels == 1)
  {
    sprintf((char *) string, "Format : %d bits mono", format->BitPerSample);
    UTIL_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
}

/**
  * @brief  Manage the BSP audio out transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  if ((Instance == 0U) && (AudioPlayTest == 1))
  {
    /* Update the first or the second part of the buffer */
    for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
    {
      PlayBuff[i+ (PLAY_BUFF_SIZE/2)] = *(uint16_t *)(AUDIO_FILE_ADDRESS + PlaybackPosition);
      PlaybackPosition+=2; 
    }

    /* check the end of the file */
    if((PlaybackPosition+PLAY_BUFF_SIZE/2) > AUDIO_FILE_SIZE)
    {
      PlaybackPosition = PLAY_HEADER;
    }
  }
}

/**
  * @brief  Manage the BSP audio out half transfer complete event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  if ((Instance == 0U) && (AudioPlayTest == 1))
  {
    /* Update the first or the second part of the buffer */
    for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
    {
      PlayBuff[i] = *(uint16_t *)(AUDIO_FILE_ADDRESS + PlaybackPosition);
      PlaybackPosition+=2; 
    }

    /* check the end of the file */
    if((PlaybackPosition+PLAY_BUFF_SIZE/2) > AUDIO_FILE_SIZE)
    {
      PlaybackPosition = PLAY_HEADER;
    }
  }
}

/**
  * @brief  Manages the BSP audio out error event.
  * @param  Instance Audio out instance.
  * @retval None.
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  if (Instance == 0U)
  {
    Error_Handler();
  }
}

/**
  * @}
  */ 


/**
  * @}
  */ 
