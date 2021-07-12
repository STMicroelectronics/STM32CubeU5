/**
  ******************************************************************************
  * @file    sig_filtering.c
  * @author  MCD Application Team
  * @brief   Generic siganls filtering (Power metering) implementation
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
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "stm32u5xx_hal.h"
#include "app_audiofilt.h"
#include "sig_filtering.h"

/* Private typedef ----------------------------------------------------------*/

/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static  TS_State_t  TS_State;
uint32_t voltage [140];
uint32_t current [140];


/* Private macro -------------------------------------------------------------*/
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Private variables ---------------------------------------------------------*/
MDF_HandleTypeDef            MdfFallingHandle;
MDF_HandleTypeDef            MdfRisingHandle;
MDF_FilterConfigTypeDef      MdfFilterConfig;
extern  uint32_t Digital_filter_module;

/*Buffer location and size should aligned to cache line size (32 bytes) */
float                        voltageValue = 0;
float                        currentValue = 0;
__IO uint32_t                voltageAvailable = 0;
__IO uint32_t                currentAvailable = 0;
uint32_t measuremnt_start,flt_index;
/* Private functions --------------------------------------------------------*/
static void Show_Results(int flt_index,int voltage,int current);
static void MDF_Init(void);
static void MDF_DeInit(void);
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Sig_Filtering_Demo(void)
{
  /* IMPLEMENT APPLICATION HERE */
  Digital_filter_module = 2;
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Power Metering", CENTER_MODE);
  
 /* Guide Panel */
  UTIL_LCD_FillRect(2, 30, 316, 184,UTIL_LCD_COLOR_WHITE);    

  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_DrawRect(66, 220, 154, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(67, 221, 152, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(68, 222, 150, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12); 
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(80, 224, (uint8_t *)"Start Measurement", LEFT_MODE); 


  
  /* Return button */  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);  
  

  
 /* Results Panel */
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(2, 30, (uint8_t *)"Voltage", LEFT_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(2, 42, (uint8_t *)"Current", LEFT_MODE);


  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
 
  UTIL_LCD_DisplayStringAt(280, 202, (uint8_t *)"Time ", LEFT_MODE);

  UTIL_LCD_DrawLine(20, 201, 280,201,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DrawLine(29, 40, 29,210,UTIL_LCD_COLOR_BLACK);

    
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(94, 30, (uint8_t *)"(Connect JP21_Pin1 to CN24_Pin1)", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(94, 42, (uint8_t *)"(Move the Potentiometer RV1)", LEFT_MODE);  
 /* Initialize MDF */
  MDF_Init();


  measuremnt_start = 0;
  do 
  {
    BSP_TS_GetState(0, &TS_State); 
    while(TS_State.TouchDetected == 0)
    {  
      BSP_TS_GetState(0, &TS_State);
    }
    
    while(TS_State.TouchDetected == 1)
    {  
      BSP_TS_GetState(0, &TS_State);
    }
    HAL_Delay(100);    
    
    if ((TS_State.TouchX > 40) && (TS_State.TouchX < 200) && (TS_State.TouchY > 200) && (TS_State.TouchY < 240)) 
    {
      
      measuremnt_start ++;
      
      HAL_Delay(100);
 
      HAL_Delay(10);
    }
    if (measuremnt_start %2== 1)
    {

      UTIL_LCD_SetFont(&Font12); 
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
      UTIL_LCD_DisplayStringAt(80, 224, (uint8_t *)"Stop  Measurement", LEFT_MODE); 
  
      while (TS_State.TouchDetected == 0)
      {  
        
        if (flt_index ++ >140)
        {
          flt_index = 0;
          UTIL_LCD_FillRect(30, 60, 300, 141,UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_DrawLine(20, 201, 280,201,UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_DrawLine(29, 40, 29,210,UTIL_LCD_COLOR_BLACK);
        }
        
        Show_Results( flt_index,(int)voltageValue, (int)currentValue);
        BSP_TS_GetState(0, &TS_State);
        HAL_Delay(100); 
        BSP_TS_GetState(0, &TS_State);
      } 

    }
    else
    {
      UTIL_LCD_SetFont(&Font12); 
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
      UTIL_LCD_DisplayStringAt(80, 224, (uint8_t *)"Start Measurement", LEFT_MODE); 
      BSP_TS_GetState(0, &TS_State);
      HAL_Delay(100); 
      BSP_TS_GetState(0, &TS_State);
    }
    
    
  } while (( TS_State.TouchX < 270) || (TS_State.TouchY < 210));
  MDF_DeInit();
}


static void Show_Results(int flt_index,int voltage,int current)
{
  static int old_voltage=0, old_current=0;
  char tmp[10];

  

  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  sprintf(tmp,"(%3dmV)", voltage); 
  UTIL_LCD_DisplayStringAt(50, 30, (uint8_t *)tmp, LEFT_MODE);
  sprintf(tmp,"(%3duA)", current);                          
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(50, 42, (uint8_t *)tmp, LEFT_MODE);
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  
  voltage = voltage/4; /* Voltage Scaling  */
  current = current/2; /* Current Scaling  */
  if ((voltage > 140) || (current > 140))
  {
    UTIL_LCD_DisplayStringAt(100, 30, (uint8_t *)"! Max Voltage|Current Reached !", LEFT_MODE);
    return ;
  }
  else
  {  
    
    UTIL_LCD_DisplayStringAt(100, 30, (uint8_t *)"                               ", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(100, 42, (uint8_t *)"                               ", LEFT_MODE);
        
    UTIL_LCD_DrawLine(30+ 2*flt_index, 200-old_voltage, 30+ 2*(flt_index+1), 200-voltage, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawLine(30+ 2*flt_index, 200-old_current, 30+ 2*(flt_index+1), 200-current, UTIL_LCD_COLOR_GREEN);
    old_voltage = voltage;
    old_current = current;
  }
}



/**
  * @brief  MDF channels and filter initialization
  * @param  None
  * @retval None
  */
static void MDF_Init(void)
{


  /* Initialize SDIN5 (Rising Edge)*/
  __HAL_MDF_RESET_HANDLE_STATE(&MdfRisingHandle);
  MdfRisingHandle.Init.CommonParam.InterleavedFilters = 0U;
  MdfRisingHandle.Init.CommonParam.ProcClockDivider   = 10U;
  MdfRisingHandle.Init.CommonParam.OutputClock.Activation = ENABLE;
  MdfRisingHandle.Init.CommonParam.OutputClock.Pins       = MDF_OUTPUT_CLOCK_1;
  MdfRisingHandle.Init.CommonParam.OutputClock.Divider    = 16U;
  MdfRisingHandle.Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  MdfRisingHandle.Init.CommonParam.OutputClock.Trigger.Source     = MDF_CLOCK_TRIG_TRGO;
  MdfRisingHandle.Init.CommonParam.OutputClock.Trigger.Edge       = MDF_CLOCK_TRIG_RISING_EDGE;

  MdfRisingHandle.Init.SerialInterface.Activation  = ENABLE;
  MdfRisingHandle.Init.SerialInterface.Mode        = MDF_SITF_NORMAL_SPI_MODE;
  MdfRisingHandle.Init.SerialInterface.ClockSource = MDF_SITF_CCK1_SOURCE;
  MdfRisingHandle.Init.SerialInterface.Threshold   = 31U;

  MdfRisingHandle.Init.FilterBistream = MDF_BITSTREAM3_RISING;
  MdfRisingHandle.Instance = MDF1_Filter4;

  if (HAL_MDF_Init(&MdfRisingHandle) != HAL_OK)
  {
    Error_Handler();
  }

   /* Initialize SDIN5 (Falling Edge)*/
  __HAL_MDF_RESET_HANDLE_STATE(&MdfFallingHandle);
  MdfFallingHandle = MdfRisingHandle;

  MdfFallingHandle.Init.FilterBistream = MDF_BITSTREAM3_FALLING;
  MdfFallingHandle.Instance = MDF1_Filter3;

  if (HAL_MDF_Init(&MdfFallingHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* Initialize filter configuration parameters */
  MdfFilterConfig.DataSource      = MDF_DATA_SOURCE_BSMX;
  MdfFilterConfig.Delay           = 0U;
  MdfFilterConfig.CicMode         = MDF_TWO_FILTERS_MCIC_FASTSINC;
  MdfFilterConfig.DecimationRatio = 512U;
  MdfFilterConfig.Offset          = 0;
  MdfFilterConfig.Gain            = -2;
  MdfFilterConfig.ReshapeFilter.Activation      = DISABLE;
  MdfFilterConfig.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
  MdfFilterConfig.HighPassFilter.Activation      = DISABLE;
  MdfFilterConfig.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;
  MdfFilterConfig.Integrator.Activation     = DISABLE;
  MdfFilterConfig.Integrator.Value          = 4U;
  MdfFilterConfig.Integrator.OutputDivision = MDF_INTEGRATOR_OUTPUT_NO_DIV;
  MdfFilterConfig.SoundActivity.Activation           = DISABLE;
  MdfFilterConfig.SoundActivity.Mode                 = MDF_SAD_VOICE_ACTIVITY_DETECTOR; 
  MdfFilterConfig.SoundActivity.FrameSize            = MDF_SAD_8_PCM_SAMPLES;
  MdfFilterConfig.SoundActivity.Hysteresis           = DISABLE;
  MdfFilterConfig.SoundActivity.SoundTriggerEvent    = MDF_SAD_ENTER_DETECT;
  MdfFilterConfig.SoundActivity.DataMemoryTransfer   = MDF_SAD_MEMORY_TRANSFER_ALWAYS;
  MdfFilterConfig.SoundActivity.MinNoiseLevel        = 5U;
  MdfFilterConfig.SoundActivity.HangoverWindow       = MDF_SAD_HANGOVER_4_FRAMES;
  MdfFilterConfig.SoundActivity.LearningFrames       = MDF_SAD_LEARNING_2_FRAMES;
  MdfFilterConfig.SoundActivity.AmbientNoiseSlope    = 10U;
  MdfFilterConfig.SoundActivity.SignalNoiseThreshold = MDF_SAD_SIGNAL_NOISE_18DB;
  MdfFilterConfig.FifoThreshold   = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  MdfFilterConfig.DiscardSamples  = 0U;
  MdfFilterConfig.Trigger.Source  = MDF_FILTER_TRIG_TRGO;
  MdfFilterConfig.Trigger.Edge    = MDF_FILTER_TRIG_FALLING_EDGE;
  MdfFilterConfig.SnapshotFormat  = MDF_SNAPSHOT_23BITS;
  MdfFilterConfig.AcquisitionMode = MDF_MODE_ASYNC_CONT;


  if (HAL_MDF_AcqStart_IT(&MdfRisingHandle, &MdfFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_MDF_AcqStart_IT(&MdfFallingHandle, &MdfFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }   
  
}


/**
  * @brief  MDF channels and filter initialization
  * @param  None
  * @retval None
  */
static void MDF_DeInit(void)
{


   if (HAL_MDF_AcqStop_IT(&MdfRisingHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_MDF_AcqStop_IT(&MdfFallingHandle) != HAL_OK)
  {
    Error_Handler();
  }    
  
  if (HAL_MDF_DeInit(&MdfRisingHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_MDF_DeInit(&MdfFallingHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  
}


/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_MDF_GetAcqValue.
  * @param  hmdf : MDF handle.
  * @retval None
  */
void HAL_MDF_AcqCpltCallback(MDF_HandleTypeDef *hmdf)
{
  int32_t  convValue;

  /* read conversion value and corresponding channel */
  HAL_MDF_GetAcqValue(hmdf, &convValue);

  if (hmdf == &MdfRisingHandle)
  {
    /* current channel */
    currentValue =  (convValue >> 8)*2 /1000;
    currentAvailable = 1;
  }
  else if (hmdf == &MdfFallingHandle)
  {
    /* voltage channel */
    voltageValue = (convValue >> 8)* 4.7 /1000;
    voltageAvailable = 1;
  }
}
/**
  * @brief  Get User action
  * @param  sel User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.
  * @retval None
  */
void Sig_Filtering_UserAction(uint8_t sel)
{
    UserEvent = sel;
    UserEntry = 1;
}
