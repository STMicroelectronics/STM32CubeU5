/**
  ******************************************************************************
  * @file    audio_recplay.c
  * @author  MCD Application Team
  * @brief   Audio recorder/player implementation
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
#include "stm32u5xx_hal.h"
#include "app_audiofilt.h"
#include "audio_recplay.h"
#include "main.h"
#include "stm32u575i_eval_bus.h"
#include "stm32u575i_eval_io.h"
#include "audio.h"
/* Include audio component Driver */
#include "../Components/cs42l51/cs42l51.h"
/* Private typedef ----------------------------------------------------------*/

/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Private variables ---------------------------------------------------------*/
MDF_HandleTypeDef            MDFHandle;
MDF_FilterConfigTypeDef      MDFFilterConfig;
SAI_HandleTypeDef            SaiHandle;
DMA_HandleTypeDef            hSaiDma;
DMA_HandleTypeDef            hMdfDma;
MDF_DmaConfigTypeDef         DMAConfig;

static AUDIO_Drv_t *AudioDrv = NULL;
void *AudioCompObj;
extern uint32_t Digital_filter_module;
/*Buffer location and size should aligned to cache line size (32 bytes) */
int16_t        RecBuff[REC_BUFF_SIZE * 2U];
uint32_t       PlaybackStarted         = 0;

__IO int16_t UpdatePointer = -1;


static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static uint8_t FilterModeIndex = 4,  PrevIndex = 4;
static uint8_t FilterModeStrTab[6][10]={"FASTSINC"," SINC1  "," SINC2  "," SINC3  "," SINC4  ","SINC5  "};

static uint32_t FilterModeTab[6]={MDF_TWO_FILTERS_MCIC_FASTSINC,MDF_TWO_FILTERS_MCIC_SINC1,MDF_TWO_FILTERS_MCIC_SINC2,MDF_TWO_FILTERS_MCIC_SINC3,MDF_ONE_FILTER_SINC4,MDF_ONE_FILTER_SINC5};
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void Playback_Init(void);
static void MDF_Init(void);
static void MDF_FilterConfig(uint32_t CicMode );
static void MDF_DMAConfig(void);
static int32_t CS42L51_Probe(void);
static int32_t CS42L51_PowerUp(void);
static int32_t CS42L51_PowerDown(void);
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Audio_Recplay_Demo(void)
{
  uint32_t joyState = JOY_NONE;
  uint32_t Button   = 0;
  /* IMPLEMENT APPLICATION HERE */
  
  Digital_filter_module = 1;
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"MDF Audio Recorder", CENTER_MODE);
  
  /* Control Panel */
  
  UTIL_LCD_FillRect(2, 30, 316, 90,UTIL_LCD_COLOR_WHITE);  
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 36, (uint8_t *)"MDF Filter CIC Mode", CENTER_MODE);
  
  /* Button*/
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 82, (uint8_t *)" SINC4  ", CENTER_MODE);
  
  /* Button*/
  UTIL_LCD_DrawRect(110, 76, 100, 26,UTIL_LCD_COLOR_RED); 
  
  UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_RED);
  UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_RED);
  UTIL_LCD_FillRect(61, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(72, 82, (uint8_t *)"<<", LEFT_MODE);    
  
  UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(221, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(222, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(232, 82, (uint8_t *)">>", LEFT_MODE); 
  
  /* Guide Panel */
  UTIL_LCD_FillRect(2, 126, 316, 90,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 140, (uint8_t *)"Please modify MDF ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 160, (uint8_t *)"filters mode and hear", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 180, (uint8_t *)"the audio quality", CENTER_MODE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(4, 200, (uint8_t *)"! Sound maybe very high !", CENTER_MODE);
  
  /* Log Panel */
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);   
  /* Return button */  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);
  
  /* Initialize playback */
  Playback_Init();
  
  /* Start the playback */
  if(0 != AudioDrv->Play(AudioCompObj))
  {
    Error_Handler();
  }
  
  /* Initialize MDF */
  MDF_Init();
  
  /* Configure MDF Filter */
  MDF_FilterConfig(MDF_ONE_FILTER_SINC4);
  
  /* Configure MDF DMA */
  MDF_DMAConfig();
  
  if (HAL_MDF_AcqStart_DMA(&MDFHandle, &MDFFilterConfig, &DMAConfig) != HAL_OK)
  {
    Error_Handler();
  }
     /* Audio play*/
  if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *) &RecBuff[0], (REC_BUFF_SIZE * 2U)))
  {
    Error_Handler();
  }
   PrevIndex = FilterModeIndex;
  do 
  {
    joyState = JOY_NONE;
    while( joyState == JOY_NONE)
    {  
      joyState = BSP_JOY_GetState(JOY1);
      
    }
    
    /* anti bounding assert */ 
    while( BSP_JOY_GetState(JOY1) != JOY_NONE);
    HAL_Delay(100);
    
    if ( joyState != JOY_SEL)
    {    
      Button++;
      if (Button == 3)
      {
        Button = 0;
      }
      UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(219, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
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
        if (FilterModeIndex > 0)
        {
          FilterModeIndex--;
          
        }
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, FilterModeStrTab[FilterModeIndex], CENTER_MODE); 
        
        
      }
      if (Button == 1)
      {
        
        if (FilterModeIndex < 5)
        {
          FilterModeIndex++;
        }
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, FilterModeStrTab[FilterModeIndex], CENTER_MODE); 
        
      }  
      
      if ( PrevIndex != FilterModeIndex)
      {
        if(HAL_OK != HAL_SAI_DMAStop(&SaiHandle))
        {
          Error_Handler();
        }   
        if (HAL_MDF_AcqStop_DMA(&MDFHandle) != HAL_OK)
        {
          Error_Handler();
        }      
        
        MDF_FilterConfig(FilterModeTab[FilterModeIndex]);
        
        /* Configure MDF DMA */
        MDF_DMAConfig();
        
        if (HAL_MDF_AcqStart_DMA(&MDFHandle, &MDFFilterConfig, &DMAConfig) != HAL_OK)
        {
          Error_Handler();
        }
        /* Audio play*/
        if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *) &RecBuff[0], (REC_BUFF_SIZE * 2U)))
        {
          Error_Handler();
        }
        PrevIndex = FilterModeIndex;
      }
      if (Button == 2)
      {
        break;  
      }
      
    }

 
    
  } while (1);
  
  
    /* Enable SAI to generate clock used by audio driver */

  /* Initialize the codec internal registers */
  if(AudioDrv->DeInit(AudioCompObj) < 0)
  {
    Error_Handler();
  }  
  CS42L51_PowerDown();

   
  HAL_SAI_DMAStop(&SaiHandle);
  
  
  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }  
  
  HAL_MDF_AcqStop_DMA(&MDFHandle);
  __HAL_SAI_DISABLE(&SaiHandle);
  HAL_MDF_DeInit(&MDFHandle);  

}

/**
* @brief  Get User action
* @param  sel User selection (JOY_SEL,...)
* @note   This example is the only way to get user information.
* @retval None
*/
void Audio_Recplay_UserAction(uint8_t sel)
{
  UserEvent = sel;
  UserEntry = 1;
}



/**
* @brief  Register Bus IOs if component ID is OK
* @retval error status
*/
static int32_t CS42L51_Probe(void)
{
  int32_t                   ret = BSP_ERROR_NONE;
  CS42L51_IO_t              IOCtx;
  static CS42L51_Object_t   CS42L51Obj;
  uint32_t                  cs42l51_id;
  
  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;
  
  if(CS42L51_RegisterBusIO (&CS42L51Obj, &IOCtx) != CS42L51_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if(CS42L51_ReadID(&CS42L51Obj, &cs42l51_id) != CS42L51_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if((cs42l51_id & CS42L51_ID_MASK) != CS42L51_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    AudioDrv = (AUDIO_Drv_t *) &CS42L51_Driver;
    AudioCompObj = &CS42L51Obj;
  }
  
  return ret;
}

/**
* @brief  Un-reset CS42L51.
* @param  None.
* @retval BSP status.
*/
static int32_t CS42L51_PowerUp(void)
{
  int32_t ret = BSP_ERROR_NONE;;
  
  
#if (USE_BSP_IO_CLASS == 1)
  BSP_IO_Init_t IO_Init;
  
  /* Initialize the BSP IO driver and configure the CS42L51 reset pin */
  IO_Init.Pin  = IO_PIN_15;
  IO_Init.Mode = IO_MODE_OUTPUT_PP;
  IO_Init.Pull = IO_PULLDOWN;
  if(BSP_IO_Init(0, &IO_Init) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if (BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_RESET) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    /* Un-reset the CS42L51 */
    HAL_Delay(10);
    if(BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_SET) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      HAL_Delay(10);
    }
  }
  
  /* Wait 1ms according CS42L51 datasheet */
  HAL_Delay(1);
#endif /* USE_BSP_IO_CLASS == 1 */
  
  return ret;
}

/**
* @brief  Power Down CS42L51.
* @param  None.
* @retval BSP status.
*/
static int32_t CS42L51_PowerDown(void)
{

  if (BSP_IO_WritePin(0, IO_PIN_15, IO_PIN_RESET) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_BUS_FAILURE;
  }

  HAL_Delay(10);

  
  return BSP_ERROR_NONE;
}
/**
* @brief  Playback initialization
* @param  None
* @retval None
*/
static void Playback_Init(void)
{
  CS42L51_Init_t codec_init;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* SAI clock config:
  PLL3_VCO Input = MSI_4Mhz/PLL3M= 4 Mhz
  PLL3_VCO Output = PLL3_VCO Input * PLL3N = 320 Mhz
  SAI_CLK_x = PLL3_VCO Output/PLL3P = 320/28 = 11.328 Mhz */
  RCC_PeriphCLKInitStruct.PLL3.PLL3Source = RCC_PLLSOURCE_MSI;
  RCC_PeriphCLKInitStruct.PLL3.PLL3M = 1;
  RCC_PeriphCLKInitStruct.PLL3.PLL3N = 80;
  RCC_PeriphCLKInitStruct.PLL3.PLL3P = 28;
  RCC_PeriphCLKInitStruct.PLL3.PLL3Q = 28;
  RCC_PeriphCLKInitStruct.PLL3.PLL3R = 2;
  RCC_PeriphCLKInitStruct.PLL3.PLL3RGE = 0;
  RCC_PeriphCLKInitStruct.PLL3.PLL3FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL3.PLL3ClockOut = RCC_PLL3_DIVP;
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);
  
  SaiHandle.Instance = AUDIO_SAIx;
  
  __HAL_SAI_DISABLE(&SaiHandle);
  
  SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;
  SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
  SaiHandle.Init.Mckdiv         = 0; /* N.U */
  SaiHandle.Init.MonoStereoMode = SAI_MONOMODE; /* Audio Mono Mode */
  SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
  SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
  SaiHandle.Init.MckOverSampling      = SAI_MCK_OVERSAMPLING_DISABLE;
  SaiHandle.Init.PdmInit.Activation   = DISABLE;
  
  SaiHandle.FrameInit.FrameLength       = 32;
  SaiHandle.FrameInit.ActiveFrameLength = 16;
  SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;
  
  SaiHandle.SlotInit.FirstBitOffset = 0;
  SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiHandle.SlotInit.SlotNumber     = 2;
  SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);
  
  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiHandle);
  
  CS42L51_PowerUp();
  CS42L51_Probe();
  
  /* Fill codec_init structure */
  codec_init.InputDevice  = CS42L51_IN_MIC1;
  codec_init.OutputDevice = CS42L51_OUT_HEADPHONE;
  codec_init.Frequency    = AUDIO_FREQUENCY_44K;
  codec_init.Resolution   = CS42L51_RESOLUTION_16B; /* Not used */
  codec_init.Volume       = 100;
  
  /* Initialize the codec internal registers */
  if(AudioDrv->Init(AudioCompObj, &codec_init) < 0)
  {
    Error_Handler();
  }
}

/**
* @brief  MDF initialization
* @param  None
* @retval None
*/
static void MDF_Init(void)
{ 
  /* Initialize MDF */
  MDFHandle.Instance                                        = ADF1_Filter0;
  MDFHandle.Init.CommonParam.InterleavedFilters             = 0U;
  MDFHandle.Init.CommonParam.ProcClockDivider               = 1U;
  MDFHandle.Init.CommonParam.OutputClock.Activation         = ENABLE;
  MDFHandle.Init.CommonParam.OutputClock.Pins               = MDF_OUTPUT_CLOCK_0;
  MDFHandle.Init.CommonParam.OutputClock.Divider            = 4U;
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
static void MDF_FilterConfig(uint32_t CicMode )
{
  /* Initialize filter configuration parameters */
  MDFFilterConfig.DataSource                         = MDF_DATA_SOURCE_BSMX;
  MDFFilterConfig.Delay                              = 0U;
  MDFFilterConfig.CicMode                            = CicMode;
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
  MDFFilterConfig.SoundActivity.Activation           = DISABLE;
  MDFFilterConfig.SoundActivity.Mode                 = MDF_SAD_AMBIENT_NOISE_DETECTOR;
  MDFFilterConfig.SoundActivity.FrameSize            = MDF_SAD_8_PCM_SAMPLES;
  MDFFilterConfig.SoundActivity.Hysteresis           = DISABLE;
  MDFFilterConfig.SoundActivity.SoundTriggerEvent    = MDF_SAD_ENTER_DETECT;
  MDFFilterConfig.SoundActivity.DataMemoryTransfer   = MDF_SAD_NO_MEMORY_TRANSFER;
  MDFFilterConfig.SoundActivity.MinNoiseLevel        = 0U;
  MDFFilterConfig.SoundActivity.HangoverWindow       = MDF_SAD_HANGOVER_4_FRAMES;
  MDFFilterConfig.SoundActivity.LearningFrames       = MDF_SAD_LEARNING_2_FRAMES;
  MDFFilterConfig.SoundActivity.AmbientNoiseSlope    = 0U;
  MDFFilterConfig.SoundActivity.SignalNoiseThreshold = MDF_SAD_SIGNAL_NOISE_3_5DB;
  MDFFilterConfig.FifoThreshold                      = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  MDFFilterConfig.DiscardSamples                     = 0U;
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
  DMAConfig.MsbOnly                              = ENABLE;
}



