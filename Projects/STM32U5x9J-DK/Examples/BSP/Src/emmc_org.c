/**
******************************************************************************
* @file    Examples/BSP/Src/emmc.c
* @author  MCD Application Team
* @brief   This example code shows how to use the eMMC Driver
******************************************************************************
* @attention
*
* Copyright (c) 2022 STMicroelectronics.
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
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((MMC_BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
static uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
__IO uint32_t MMCWriteStatus = 0, MMCReadStatus = 0;
/* Private function prototypes -----------------------------------------------*/
static void eMMC_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  MMC Demo
* @param  None
* @retval None
*/
int32_t eMMC_demo (void)
{
  int32_t MMC_state ;
  
  /* Demo Hint */
  eMMC_SetHint();  
  
  MMC_state = BSP_MMC_Init();
  
  if(MMC_state != BSP_ERROR_NONE)
  {
    printf("\r\nMMC Initialization : Failed");
    printf("\r\nMMC Test Aborted");
    printf("\r\n");
  }
  else
  {
    printf("\r\nMMC Initialization : OK");

    MMC_state = BSP_MMC_Erase(BLOCK_START_ADDR, (MMC_BLOCKSIZE * NUM_OF_BLOCKS));
    while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
    {
    }

    if(MMC_state != BSP_ERROR_NONE)
    {
          printf("\r\nMMC Erase : Failed");
          printf("\r\nMMC Test Aborted");
          printf("\r\n");
    }
    else
    {
      printf("\r\nMMC Erase : OK");

      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      MMC_state = BSP_MMC_WriteBlocks((uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);

      /* Wait until MMC cards are ready to use for new operation */
      while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
      {
      }
      if(MMC_state != BSP_ERROR_NONE)
      {
        printf("\r\nMMC Write : Failed");
        printf("\r\nMMC Test Aborted");
        printf("\r\n");
      }
      else
      {
        printf("\r\nMMC Write : OK");
        MMC_state = BSP_MMC_ReadBlocks((uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);

        /* Wait until MMC cards are ready to use for new operation */
        while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
        {
        }
        if(MMC_state != BSP_ERROR_NONE)
        {
          printf("\r\nMMC Read : Failed");
          printf("\r\nMMC Test Aborted");
          printf("\r\n");
        }
        else
        {
          printf("\r\nMMC Read : OK");
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            printf("\r\nMMC Compare : Failed");
            printf("\r\nMMC Test Aborted");
            printf("\r\n");
          }
          else
          {
            printf("\r\nMMC Compare : OK");
          }
        }
      }
    }
  }

  BSP_LCD_DrawBitmap(0, 196, 0, (uint8_t *)back);
  /* Wait frame buffer available */
  while (BSP_LCD_IsFrameBufferAvailable(0) != BSP_ERROR_NONE);
  BSP_LCD_Refresh(0); 
  
  UTIL_LCD_DisplayStringAt(0, 200, (uint8_t *)"Test of eMMC Done", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"Please check the result", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 250, (uint8_t *)"on IO Terminal", CENTER_MODE);
  
  /* Wait frame buffer available */
  BSP_LCD_IsFrameBufferAvailable(0);
  BSP_LCD_Refresh(0); 
  
  while ((x < 190) | (x > 300) | (y > 55));
  DrawBackround = 0;
  return 0;
}

/**
  * @brief  Display eMMC demo hint
  * @param  None
  * @retval None
  */
static void eMMC_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 454, 454, LCD_COLOR_BLACK);  

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 170, (uint8_t *)"Test of eMMC", CENTER_MODE); 
  
  /* Wait frame buffer available */
  while (BSP_LCD_IsFrameBufferAvailable(0) != BSP_ERROR_NONE);
  BSP_LCD_Refresh(0); 
}

/**
  * @brief Tx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_MMC_WriteCpltCallback(void)
{
  MMCWriteStatus = 1;

}

/**
  * @brief Rx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_MMC_ReadCpltCallback(void)
{
  MMCReadStatus = 1;
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
* @}
*/

/**
* @}
*/

