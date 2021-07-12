/**
  ******************************************************************************
  * @file    Examples/BSP/Src/sd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the BSP SD Driver
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
#define SD_CARD_PRESENCE_POLLING_MODE        0
#define SD_CARD_PRESENCE_INTERRUPT_MODE      1

#define SD_CARD_PRESENCE_VALIDATION_MODE     SD_CARD_PRESENCE_INTERRUPT_MODE       

#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
__IO uint32_t SDWriteStatus = 0, SDReadStatus = 0, SDDetectStatus = 0, SDDetectIT = 0;

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo(void)
{
  int32_t SD_state = BSP_ERROR_NONE;
  static uint8_t prev_status = 2;  /* Undefined state */

  SD_SetHint();

  SD_state = BSP_SD_Init(0);

#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
  if (SD_state == BSP_ERROR_NONE)
  {
    SD_state = BSP_SD_DetectITConfig(0);
  }
#endif
  
  if (SD_state != BSP_ERROR_NONE)
  {
    if (SD_state == BSP_ERROR_PERIPH_FAILURE)
    {
      UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD shall be inserted before running test", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD Initialization : FAIL.", LEFT_MODE);
    }
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD Initialization : OK.", LEFT_MODE);

    UTIL_LCD_DisplayStringAt(20, 210, (uint8_t *)"SD Connected    ", LEFT_MODE);
    prev_status = SD_PRESENT;

    SD_state = BSP_SD_Erase(0, BLOCK_START_ADDR, (BLOCKSIZE * NUM_OF_BLOCKS));
  
    /* Wait until SD card is ready to use for new operation */
    while (BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
    {
    }

    if (SD_state != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);

      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks_DMA(0, aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);

      /* Wait for the write process is completed */
      while((SDWriteStatus == 0))
      {
      }
      SDWriteStatus = 0;

      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState(0) != SD_TRANSFER_OK))
      {
      } 

      if (SD_state != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        SD_state = BSP_SD_ReadBlocks_DMA(0, aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);

        /* Wait for the read process is completed */
        while(SDReadStatus == 0)
        {
        }
        SDReadStatus = 0;

        /* Wait until SD card is ready to use for new operation */
        while(BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
        {
        }

        if (SD_state != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          if (Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD can be removed.", LEFT_MODE);
          }
        }
      }
    }
  }

  while (1)
  {
#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
    if(SDDetectIT != 0)
    {
#endif
      /* Check if the SD card is plugged in the slot */
      if (SDDetectStatus != SD_PRESENT)
      {
        if (prev_status != SD_NOT_PRESENT)
        {
          prev_status = SD_NOT_PRESENT;
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
          UTIL_LCD_DisplayStringAt(20, 210, (uint8_t *)"SD Not Connected", LEFT_MODE);
        }
      }
      else if (prev_status != SD_PRESENT)
      {
        UTIL_LCD_DisplayStringAt(20, 210,   (uint8_t *)"SD Connected    ", LEFT_MODE);
        prev_status = SD_PRESENT;
      }
#if (SD_CARD_PRESENCE_VALIDATION_MODE == SD_CARD_PRESENCE_INTERRUPT_MODE)
      SDDetectIT = 0;
    }
#endif
    if (prev_status == SD_PRESENT)
    {
      BSP_LED_Toggle(LED_GREEN);
    }
    else
    {
      BSP_LED_Toggle(LED_YELLOW);
    }
    HAL_Delay(100);
    if (UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;

      BSP_LED_Off(LED_GREEN);
      BSP_LED_Off(LED_YELLOW);

      return;
    }
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User push-button to exit", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief Tx Transfer completed callback
  * @param  Instance     SD Instance
  * @retval None
  */
void BSP_SD_WriteCpltCallback(uint32_t Instance)
{
  if (Instance == 0)
  {
    SDWriteStatus = 1;
  }
}

/**
  * @brief Rx Transfer completed callback
  * @param  Instance     SD Instance
  * @retval None
  */
void BSP_SD_ReadCpltCallback(uint32_t Instance)
{
  if (Instance == 0)
  {
    SDReadStatus = 1;
  }
}

/**
  * @brief  BSP SD Callback.
  * @param  Instance SD Instance
  * @param  Status   Pin status
  * @retval None.
  */
void BSP_SD_DetectCallback(uint32_t Instance, uint32_t Status)
{
  if (Instance == 0)
  {
    SDDetectIT = 1;
    SDDetectStatus = Status;
  }
}

/**
  * @brief SDMMC error callback
  * @param None
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */
