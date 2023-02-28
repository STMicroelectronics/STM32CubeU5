/**
  ******************************************************************************
  * @file    sad.c
  * @author  MCD Application Team
  * @brief   Sound Acquisition Detection (SAD) implementation
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
#include <string.h>
#include <stdio.h>
#include "stm32u5xx_hal.h"
#include "app_audiofilt.h"
#include "audio_recplay.h"
#include "sad.h"

/* Private typedef ----------------------------------------------------------*/
RCC_TypeDef *RCC_DEBUG;
/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static uint32_t count=0;
extern  MDF_HandleTypeDef           MDFHandle;
static MDF_FilterConfigTypeDef      MDFFilterConfig;
static MDF_DmaConfigTypeDef         DMAConfig;

extern  uint32_t Digital_filter_module;

static uint32_t ThresholdTab[10]={
  MDF_SAD_SIGNAL_NOISE_3_5DB,  
  MDF_SAD_SIGNAL_NOISE_6DB, 
  MDF_SAD_SIGNAL_NOISE_9_5DB,  
  MDF_SAD_SIGNAL_NOISE_12DB,   
  MDF_SAD_SIGNAL_NOISE_15_6DB, 
  MDF_SAD_SIGNAL_NOISE_18DB,   
  MDF_SAD_SIGNAL_NOISE_21_6DB,
  MDF_SAD_SIGNAL_NOISE_24_1DB,
  MDF_SAD_SIGNAL_NOISE_27_6DB,
  MDF_SAD_SIGNAL_NOISE_30_1DB
};


static uint8_t ThresholdStrTab[10][8]={"3,5  dB","  6  dB","9,5  dB"," 12  dB","15,6 dB",
" 18  dB","21,6 dB","24,1 dB","27,6 dB","30,1 dB"};

static uint8_t ThresholdIndex = 5;
/* Place following data in section  SDRAM4 LPBAM_Section */
#if defined ( __ICCARM__ )
#pragma default_variable_attributes =@".LPBAM_Section"   
#endif
#if defined ( __CC_ARM )||( __GNUC__ )
 __attribute__((section(".LPBAM_Section")))
#endif
static int16_t RecBuff[REC_BUFF_SIZE * 2U];
#if defined ( __ICCARM__ )
/* Stop placing functions in section SDRAM4 LPBAM_Sectionn */
#pragma default_variable_attributes =   
#endif

static void MDF_Init(void);
static void MDF_FilterConfig(void);
static void MDF_DMAConfig(void);
static void EnterLowPowerMode(void);
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Sad_Demo(void)
{
  uint32_t joyState = JOY_NONE;
  uint32_t Button   = 0;
  /* IMPLEMENT APPLICATION HERE */
  Digital_filter_module = 0;
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Sound Activity Detect", CENTER_MODE);
  
  /* Control Panel */
  
  UTIL_LCD_FillRect(2, 30, 316, 90,UTIL_LCD_COLOR_WHITE);  
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 36, (uint8_t *)"Sound Detect Threshold", CENTER_MODE);
  
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 82, (uint8_t *)"18  dB", CENTER_MODE);
  
  /* Button*/
  UTIL_LCD_DrawRect(110, 76, 100, 26,UTIL_LCD_COLOR_RED); 
  
  UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_RED);
  UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_RED);
  UTIL_LCD_FillRect(61, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(72, 82, (uint8_t *)"-", LEFT_MODE);    
  
  UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(221, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(222, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(232, 82, (uint8_t *)"+", LEFT_MODE); 
  
  
  
  /* Guide Panel */
  UTIL_LCD_FillRect(2, 126, 316, 90,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 130, (uint8_t *)"When detected sound is above", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 150, (uint8_t *)"the threshold, the system will", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 170, (uint8_t *)"wakeup and resume running", CENTER_MODE);
  
  UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(61, 187, 188, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 188, 186, 22,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(63, 192, (uint8_t *)"System Stop Mode", LEFT_MODE); 
  
  /* Log Panel */
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  
  UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Run mode ", LEFT_MODE);
  
  /* Return button */  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);  
  
  
  
  do 
  {
    
    joyState = JOY_NONE;
    while( joyState == JOY_NONE)
    {  
      joyState = BSP_JOY_GetState(JOY1);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
      
      if (count%4 == 0)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "/", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "/", LEFT_MODE);      
        
      }
      if (count%4 == 1)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "-", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "-", LEFT_MODE);      
      }
      if (count%4 == 2)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "\\", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "\\", LEFT_MODE);      
        
      }
      if (count%4 == 3)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "|", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "|", LEFT_MODE);      
      }
      
      
      count++;
      HAL_Delay(200);
      
    }
    
    /* anti bounding assert */ 
    while( BSP_JOY_GetState(JOY1) != JOY_NONE);
    HAL_Delay(100);
    
    if ( joyState != JOY_SEL)
    {    
      Button++;
      if (Button == 4)
      {
        Button = 0;
      }
      UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(219, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_WHITE);
      switch(Button)
      {
      case 0 :
        UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 1 :
        UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(219, 79, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 2 :
        UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_RED);
        break;
      case 3 :
        UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_RED);
        break;        
      default:
        break;
      }
      
    }
    

    
    if ( joyState == JOY_SEL) 
    {
      
      if (Button == 0)
      {
        if (ThresholdIndex > 0)
        {
          ThresholdIndex--;
          
        }
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, ThresholdStrTab[ThresholdIndex], CENTER_MODE); 
        
        
      }
      if (Button == 1)
      {
        
        if (ThresholdIndex < 9)
        {
          ThresholdIndex++;
        }
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, ThresholdStrTab[ThresholdIndex], CENTER_MODE); 
        
      }
      
      if (Button == 2)
      {
        HAL_Delay(200);
        
        /* Initialize MDF */
        MDF_Init();
        
        /* Configure MDF Filter */
        MDF_FilterConfig();
        
        
        /* Configure MDF DMA */
        MDF_DMAConfig();
        
        /* Start MDF Acquisition */
        if (HAL_MDF_AcqStart_DMA(&MDFHandle, &MDFFilterConfig, &DMAConfig) != HAL_OK)
        {
          Error_Handler();
        }
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Stop mode ", LEFT_MODE);
        HAL_Delay(10);        
        EnterLowPowerMode();
        
        /* Check that the system was resumed from stop 2 */
        if (__HAL_PWR_GET_FLAG(PWR_FLAG_STOPF) == 0U)
        {
          Error_Handler();
        }
        
        /* Clear stop flag */
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOPF);
        
        /* Check that stop flag is cleared */
        if (__HAL_PWR_GET_FLAG(PWR_FLAG_STOPF) != 0U)
        {
          Error_Handler();
        }
        SystemClock_Config();
        
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
        
        UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Run mode ", LEFT_MODE); 
        
        if (HAL_MDF_AcqStop_DMA(&MDFHandle) != HAL_OK)
        {
          Error_Handler();
        }
        
        if (HAL_MDF_DeInit(&MDFHandle) != HAL_OK)
        {
          Error_Handler();
        }
        HAL_Delay(10);
      }

      if (Button == 3)
      {
        break;  
      }
    }
    
  } while (1); 
  

}

/**
* @brief  Get User action
* @param  sel User selection (JOY_SEL,...)
* @note   This example is the only way to get user information.
* @retval None
*/
void Sad_UserAction(uint8_t sel)
{
  UserEvent = sel;
  UserEntry = 1;
}



/**
* @brief  MDF initialization
* @param  None
* @retval None
*/
static void MDF_Init(void)
{
  
  /* Initialize channel 1 (left channel)*/
  MDFHandle.Instance = ADF1_Filter0;
  MDFHandle.Init.CommonParam.InterleavedFilters             = 0U;
  MDFHandle.Init.CommonParam.ProcClockDivider               = 1U;
  MDFHandle.Init.CommonParam.OutputClock.Activation         = ENABLE;
  MDFHandle.Init.CommonParam.OutputClock.Pins               = MDF_OUTPUT_CLOCK_0;
  MDFHandle.Init.CommonParam.OutputClock.Divider            = 4UL;
  MDFHandle.Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  MDFHandle.Init.CommonParam.OutputClock.Trigger.Source     = MDF_CLOCK_TRIG_TRGO;
  MDFHandle.Init.CommonParam.OutputClock.Trigger.Edge       = MDF_CLOCK_TRIG_RISING_EDGE;
  MDFHandle.Init.SerialInterface.Activation                 = ENABLE;
  MDFHandle.Init.SerialInterface.Mode                       = MDF_SITF_NORMAL_SPI_MODE;
  MDFHandle.Init.SerialInterface.ClockSource                = MDF_SITF_CCK0_SOURCE;
  MDFHandle.Init.SerialInterface.Threshold                  = 31U;
  MDFHandle.Init.FilterBistream                             = MDF_BITSTREAM0_RISING;
  
  if (HAL_MDF_Init(&MDFHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  MDF filter configuration
* @param  None
* @retval None
*/
static void MDF_FilterConfig(void)
{
  /* Initialize filter configuration parameters */
  MDFFilterConfig.DataSource                         = MDF_DATA_SOURCE_BSMX;
  MDFFilterConfig.Delay                              = 0U;
  MDFFilterConfig.CicMode                            = MDF_ONE_FILTER_SINC4;
  MDFFilterConfig.DecimationRatio                    = 64U;
  MDFFilterConfig.Offset                             = 0;
  MDFFilterConfig.Gain                               = 0;
  MDFFilterConfig.ReshapeFilter.Activation           = DISABLE;
  MDFFilterConfig.ReshapeFilter.DecimationRatio      = MDF_RSF_DECIMATION_RATIO_4;
  MDFFilterConfig.HighPassFilter.Activation          = DISABLE;
  MDFFilterConfig.HighPassFilter.CutOffFrequency     = MDF_HPF_CUTOFF_0_000625FPCM;
  MDFFilterConfig.Integrator.Activation              = DISABLE;
  MDFFilterConfig.Integrator.Value                   = 4U;
  MDFFilterConfig.Integrator.OutputDivision          = MDF_INTEGRATOR_OUTPUT_NO_DIV;
  MDFFilterConfig.SoundActivity.Activation           = ENABLE;
  MDFFilterConfig.SoundActivity.Mode                 = MDF_SAD_SOUND_DETECTOR;
  MDFFilterConfig.SoundActivity.FrameSize            = MDF_SAD_8_PCM_SAMPLES;
  MDFFilterConfig.SoundActivity.Hysteresis           = DISABLE;
  MDFFilterConfig.SoundActivity.SoundTriggerEvent    = MDF_SAD_ENTER_DETECT;
  MDFFilterConfig.SoundActivity.DataMemoryTransfer   = MDF_SAD_MEMORY_TRANSFER_ALWAYS;
  MDFFilterConfig.SoundActivity.MinNoiseLevel        = 200U;
  MDFFilterConfig.SoundActivity.HangoverWindow       = MDF_SAD_HANGOVER_4_FRAMES;
  MDFFilterConfig.SoundActivity.LearningFrames       = MDF_SAD_LEARNING_2_FRAMES;
  MDFFilterConfig.SoundActivity.AmbientNoiseSlope    = 10U;
  MDFFilterConfig.SoundActivity.SignalNoiseThreshold = ThresholdTab[ThresholdIndex];
  MDFFilterConfig.SoundActivity.SoundLevelInterrupt  = DISABLE;
  MDFFilterConfig.FifoThreshold                      = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  MDFFilterConfig.DiscardSamples                     = 4U;
  MDFFilterConfig.Trigger.Source                     = MDF_FILTER_TRIG_TRGO;
  MDFFilterConfig.Trigger.Edge                       = MDF_FILTER_TRIG_FALLING_EDGE;
  MDFFilterConfig.SnapshotFormat                     = MDF_SNAPSHOT_23BITS;
  MDFFilterConfig.AcquisitionMode                    = MDF_MODE_ASYNC_CONT;
}

/**
* @brief  MDF DMA configuration
* @param  None
* @retval None
*/
static void MDF_DMAConfig(void)
{
  /* Initialize DMA configuration parameters */
  DMAConfig.Address                              = (uint32_t)&RecBuff[0];
  DMAConfig.DataLength                           = (REC_BUFF_SIZE * 4U);
  DMAConfig.MsbOnly                              = DISABLE;
  
  HAL_NVIC_DisableIRQ(LPDMA1_Channel0_IRQn);
}

/**
* @brief  Enter low power mode
* @param  None
* @retval None
*/
static void EnterLowPowerMode(void)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter the system to STOP2 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
}
