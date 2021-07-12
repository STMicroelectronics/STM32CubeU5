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

/* Private function prototypes -----------------------------------------------*/
static void Record_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test BSP AUDIO for record.
  * @param  None
  * @retval None
  */
int32_t AudioRecord_demo(void)
{
  printf("\n******AUDIO IN EXAMPLE******\n");

  StartExample();

  /* Initialize record */
  Record_Init();
  
  /* Start record */
  if (BSP_AUDIO_IN_Record(0, (uint8_t*) PlaybackBuff, SizeBuffer) != BSP_ERROR_NONE)
  {
    printf("AUDIO IN : FAILED.\n");
    printf("AUDIO IN example Aborted.\n");
    Error_Handler();
  }
  printf("Recording Started..\n");
  
  /* Start loopback */
  CheckResult();

  /* Add delay to avoid rebound and reset it status */
  HAL_Delay(500);
  UserButtonPressed = RESET;

  /* Stop playback */
  if (BSP_AUDIO_IN_Stop(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* De-initialize record */
  if(BSP_AUDIO_IN_DeInit(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Reset global variables */
  RecBuffCplt      = 0;
  
  return 0;
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
  AudioInit.Device        = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInit.SampleRate    = AUDIO_FREQUENCY_16K;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 70; /* Not used */
  if (BSP_AUDIO_IN_Init(0, &AudioInit) != BSP_ERROR_NONE)
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
