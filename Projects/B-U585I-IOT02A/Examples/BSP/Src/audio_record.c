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
#define REC_BUFF_SIZE 16*1024
/* Private variables ---------------------------------------------------------*/
uint8_t          RecordBuff[REC_BUFF_SIZE];
__IO uint32_t   RecHalfBuffCplt  = 0;
__IO uint32_t   RecBuffCplt      = 0;

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
  
  printf("Press USER button to start the example\n");
  
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  
  while (UserButtonPressed == RESET)
  {
  }
  UserButtonPressed = RESET;
  
  /* Initialize record */
  Record_Init();
  
  /* Start record */
  
  printf("Recording Started..\n");
  if (BSP_AUDIO_IN_Record(0, (uint8_t*) RecordBuff, REC_BUFF_SIZE) != BSP_ERROR_NONE)
  {
    printf("AUDIO IN : FAILED.\n");
    printf("AUDIO IN example Aborted.\n");
    Error_Handler();
  }
  
  
  HAL_Delay(3000);
  
  /* Stop playback */
  if (BSP_AUDIO_IN_Stop(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  printf("Recording Stopped.\n");
  /* De-initialize record */
  if(BSP_AUDIO_IN_DeInit(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Reset global variables */
  RecBuffCplt      = 0;
  RecHalfBuffCplt  = 0; 
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
  AudioInit.Device        = AUDIO_IN_DEVICE_DIGITAL_MIC1;
  AudioInit.SampleRate    = AUDIO_FREQUENCY_11K;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 100; /* Not used */
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
  RecHalfBuffCplt ++;
}

/**
* @brief  Manage the BSP audio in transfer complete event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  RecBuffCplt ++;
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
