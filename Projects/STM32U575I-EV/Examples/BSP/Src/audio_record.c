/**
  ******************************************************************************
  * @file    audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use BSP AUDIO driver to record.
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

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SizeBuffer 2048

/* Private variables ---------------------------------------------------------*/
int32_t                      RecordBuff[SizeBuffer];
int8_t                       PlaybackBuff[SizeBuffer]={0};
uint32_t                     RecHalfBuffCplt  = 0;
uint32_t                     RecBuffCplt      = 0;
uint32_t                     PlaybackStarted  = 0;
uint32_t                     Instance;

/* Private function prototypes -----------------------------------------------*/
static void AudioRecord_SetHint(void);
static void Record_Init(void);
static void Playback_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test BSP AUDIO for record.
  * @param  None
  * @retval None
  */
void AudioRecord_demo(void)
{
  uint32_t PauseStatus = 0;

  /* Display test information */
  AudioRecord_SetHint();

  /* Wait for user input */
  while((JoyPinPressed != JOY_LEFT)&&(JoyPinPressed != JOY_RIGHT));

  if (JoyPinPressed == JOY_LEFT)
  {
    /* SAI selected for Audio IN */
    Instance = 0;
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Using the analog mic connected to (CN27)", CENTER_MODE);
  }
  else if (JoyPinPressed == JOY_RIGHT)
  {
    /* MDF selected for Audio IN */
    Instance = 1;
    UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Using  the  digital  microphone  (U48)", CENTER_MODE);
  }
  JoyPinPressed = JOY_NONE;

  /* Initialize playback */
  Playback_Init();

  /* Initialize record */
  Record_Init();
  
  /* Start record */
  if (BSP_AUDIO_IN_Record(Instance, (uint8_t*) PlaybackBuff, SizeBuffer) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Start loopback */
  while(UserButtonPressed != SET)
  {
    if(RecBuffCplt == 1)
    {
      if (PlaybackStarted == 0)
      {
        /* Insert a little delay before starting playback to be sure that data are available for playback */
        /* Without this delay, potential noise when optimization high is selected for compiler */
        HAL_Delay(10);
        /* Start the playback */
        if (BSP_AUDIO_OUT_Play(0, (uint8_t *) &PlaybackBuff[0], SizeBuffer) != BSP_ERROR_NONE)
        {
          Error_Handler();
        }
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"You can hear yourself when you talk", CENTER_MODE); 
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
        UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"Playback ongoing", CENTER_MODE); 
        PlaybackStarted = 1;
      }
      RecBuffCplt = 0;
      }    
      
      if (JoyPinPressed == JOY_SEL)
      {
        if (PauseStatus == 0)
        {
          /* Turn ON LED orange: Audio play in pause */
          BSP_LED_On(LED_YELLOW);
      
          if (BSP_AUDIO_IN_Pause(Instance) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }        
          PauseStatus = 1;
          HAL_Delay(500);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
          UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"Playback paused ", CENTER_MODE); 
        }
        else
        {
          /* Turn OFF LED orange: Audio play running */
          BSP_LED_Off(LED_YELLOW);

          if (BSP_AUDIO_IN_Resume(Instance) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }        
          PauseStatus = 0;
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
          UTIL_LCD_DisplayStringAt(0, 140, (uint8_t *)"Playback ongoing", CENTER_MODE); 
        }
        JoyPinPressed = JOY_NONE;
    }
  }
  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  UserButtonPressed = RESET;

  /* Stop playback */
  if (BSP_AUDIO_IN_Stop(Instance) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if (BSP_AUDIO_OUT_Stop(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* De-initialize playback */
  if(BSP_AUDIO_OUT_DeInit(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* De-initialize record */
  if(BSP_AUDIO_IN_DeInit(Instance) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Switch off LEDs */
  BSP_LED_Off(LED_GREEN);
  
  /* Reset global variables */
  RecBuffCplt      = 0;
  PlaybackStarted  = 0;
}

/**
  * @brief  Display audio record demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK); 
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Record", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User push-button to stop play", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"Press SEL to pause/resume loopback", CENTER_MODE); 

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press LEFT for SAI or RIGHT for MDF", CENTER_MODE);   
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);
}

/**
  * @brief  Record initialization
  * @param  None
  * @retval None
  */
static void Record_Init(void)
{
  BSP_AUDIO_Init_t AudioInit;

  /* Select device depending on the Instance */
  AudioInit.Device        = (Instance == 0) ? AUDIO_IN_DEVICE_ANALOG_MIC : AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInit.SampleRate    = AUDIO_FREQUENCY_16K;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 70; /* Not used */
  if (BSP_AUDIO_IN_Init(Instance, &AudioInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  Playback initialization
  * @param  None
  * @retval None
  */
static void Playback_Init(void)
{
  BSP_AUDIO_Init_t AudioInit;

  AudioInit.Device        = AUDIO_OUT_HEADPHONE;
  AudioInit.SampleRate    = AUDIO_FREQUENCY_16K;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 90;
  if (BSP_AUDIO_OUT_Init(0, &AudioInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  Manage the BSP audio in half transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  BSP_LED_Toggle(LED_GREEN);
  RecHalfBuffCplt = 1;
}

/**
  * @brief  Manage the BSP audio in transfer complete event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  BSP_LED_Toggle(LED_GREEN);
  RecBuffCplt = 1;
}

/**
  * @brief  Manages the BSP audio in error event.
  * @param  Instance Audio in instance.
  * @retval None.
  */
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */
