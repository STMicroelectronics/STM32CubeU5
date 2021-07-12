/**
  ******************************************************************************
  * @file    Examples/BSP/Src/sram.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the BSP SRAM Driver
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
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

/* Private macro -------------------------------------------------------------*/
#define LCD_TEXT_XPOS       20
#define LCD_TEXT_YPOS_START 100
#define LCD_TEXT_YPOS_INC   15

/* Private variables ---------------------------------------------------------*/
uint16_t sram_aTxBuffer[BUFFER_SIZE];
uint16_t sram_aRxBuffer[BUFFER_SIZE];
uint8_t ubSramDmaCplt = 0;

/* Private function prototypes -----------------------------------------------*/
static void SRAM_SetHint(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void SRAM_demo(void)
{ 
  uint16_t lcd_text_ypos = LCD_TEXT_YPOS_START;
  uint8_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0, ubSramDmaWrite = 0, ubSramDmaRead = 0, ubSramCmp = 0, ubSramDmaCmp = 0;

  SRAM_SetHint();

  /*##-1- Configure the SRAM device ##########################################*/
  /* SRAM device configuration */ 
  if(BSP_SRAM_Init(0) != BSP_ERROR_NONE)
  {
    ubSramInit++;
  }
  else
  {
    /*##-2- SRAM memory read/write access ######################################*/  
    /* Fill the buffer to write */
    Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xC20F);   
    
    /* Write data to the SRAM memory */
    if (HAL_SRAM_Write_16b(&hsram[0], (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), sram_aTxBuffer, BUFFER_SIZE) != HAL_OK)
    {
      ubSramWrite++;
    } 
    else
    {
      /* Read back data from the SRAM memory */
      HAL_Delay(10);
      if (HAL_SRAM_Read_16b(&hsram[0], (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), sram_aRxBuffer, BUFFER_SIZE) != HAL_OK)
      {
        ubSramRead++;
      } 
      else
      {
        /*##-3- Checking data integrity ############################################*/
        if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
        {
          ubSramCmp++;
        }
        else
        {    
          /*##-4- SRAM memory read/write access in DMA mode ##########################*/  
          /* Fill the buffer to write */
          Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xA1F0);   
          ubSramDmaCplt =0;
          
          /* Write data to the SRAM memory */
          if (HAL_SRAM_Write_DMA(&hsram[0], (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), (uint32_t *)sram_aTxBuffer, BUFFER_SIZE) != HAL_OK)
          {
            ubSramDmaWrite++;
          }   
          else
          {
            /* Wait end of DMA transfer */
            while (ubSramDmaCplt == 0)
            {
              if(UserButtonPressed == SET)
              {
                /* Add delay to avoid rebound and reset it status */
                HAL_Delay(500);
                UserButtonPressed = RESET;
                
                break;
              }
              BSP_LED_Toggle(LED_GREEN);
              HAL_Delay(100);
            }
            
            if(ubSramDmaCplt != 0)
            {
              ubSramDmaCplt =0;
              
              /* Read back data from the SRAM memory */
              if (HAL_SRAM_Read_DMA(&hsram[0], (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), (uint32_t *)sram_aRxBuffer, BUFFER_SIZE) != HAL_OK)
              {
                ubSramDmaRead++;
              }
              else
              {
                /* Wait end of DMA transfer */
                while (ubSramDmaCplt == 0)
                {
                  if(UserButtonPressed == SET)
                  {
                    /* Add delay to avoid rebound and reset it status */
                    HAL_Delay(500);
                    UserButtonPressed = RESET;
                    
                    break;
                  }
                  BSP_LED_Toggle(LED_GREEN);
                  HAL_Delay(100);
                }
                
                if(ubSramDmaCplt != 0)
                {
                  /*##-5- Checking data integrity ############################################*/
                  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
                  {
                    ubSramDmaCmp++;
                  }
                }
                else
                {
                  ubSramDmaRead++;
                }
              }
            }
            else
            {
              ubSramDmaWrite++;
            }
          }
        }
      }
    }
  }
  
  if (ubSramInit == 0)
  {
    UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Initialization : OK.", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    
    if (ubSramWrite == 0)
    {
      UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM WRITE : OK.", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      
      if (ubSramRead == 0)
      {
        UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM READ : OK.", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        
        if (ubSramCmp == 0)
        {
          UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM COMPARE : OK.", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;

          if (ubSramDmaWrite == 0)
          {
            UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM DMA WRITE : OK.", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            
            if (ubSramDmaRead == 0)
            {
              UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM DMA READ : OK.", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              
              if (ubSramDmaCmp == 0)
              {
                UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM COMPARE : OK.", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test : OK.", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              }
              else
              {
                UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
                UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
                lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              }
            }
            else
            {
              UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM DMA READ : FAILED.", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
              UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
              lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            }
          }
          else
          {
            UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM DMA WRITE : FAILED.", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
            UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
            lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          }
        }
        else
        {
          UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
          UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
          lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        }
      }
      else
      {
        UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM READ : FAILED.", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
        UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
        lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      }
    }
    else
    {
      UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM WRITE : FAILED.", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
      UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
      lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    }
  }
  else
  {
    UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Initialization : FAILED.", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
    UTIL_LCD_DisplayStringAt(LCD_TEXT_XPOS, lcd_text_ypos, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
    lcd_text_ypos+=LCD_TEXT_YPOS_INC;
  }
      
  while (1)
  {    
    if(UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;
      
      BSP_LED_Off(LED_GREEN);
      BSP_LED_Off(LED_YELLOW);

      return;
    }
    if ((ubSramWrite == 0) && (ubSramRead == 0) && (ubSramInit == 0) && (ubSramDmaWrite == 0) &&
        (ubSramDmaRead == 0) && (ubSramCmp == 0) && (ubSramDmaCmp == 0))
    {
      BSP_LED_Toggle(LED_GREEN);
    }
    else
    {
      BSP_LED_Toggle(LED_YELLOW);
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display SRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SRAM_SetHint(void)
{
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, 320, 80, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK); 
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SRAM", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Write, read and compare", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"in direct and DMA mode", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press User push-button to exit", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, 300, 140, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DrawRect(11, 91, 298, 138, UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpindex = 0;

  /* Put in global buffer different values */
  for (tmpindex = 0; tmpindex < uwBufferLength; tmpindex++ )
  {
    pBuffer[tmpindex] = tmpindex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
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
  * @brief  DMA transfer complete callback.
  * @param  hdma: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
  ubSramDmaCplt++;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
