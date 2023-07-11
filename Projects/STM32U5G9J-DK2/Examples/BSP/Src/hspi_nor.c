/**
******************************************************************************
* @file    Examples/BSP/Src/HSPI_nor.c
* @author  MCD Application Team
* @brief   This example code shows how to use the HSPI Driver
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
#define HSPI_NOR_BUFFER_SIZE     ((uint32_t)0x0200)
#define HSPI_NOR_WRITE_READ_ADDR ((uint32_t)0x0050)
#define HSPI_NOR_BASE_ADDR       ((uint32_t)0xA0000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t HSPI_nor_aTxBuffer[HSPI_NOR_BUFFER_SIZE];
uint8_t HSPI_nor_aRxBuffer[MX66UW1G45G_BLOCK_4K];

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void     HSPI_NOR_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static uint8_t  DataCmp     (uint8_t* pBuffer, uint8_t Pattern, uint32_t BufferLength);
BSP_HSPI_NOR_Info_t Hspi_NOR_Info = {0};
BSP_HSPI_NOR_Init_t Hspi_NOR_Init;
/* Private functions ---------------------------------------------------------*/
int32_t HSPI_NOR_demo (void);
/**
* @brief  HSPI NOR Demo
* @param  None
* @retval None
*/
int32_t HSPI_NOR_demo (void)
{
  /* HSPI info structure */
  int32_t status;
  __IO uint8_t *data_ptr;
  uint32_t index;
  uint8_t mode;
  Text_AlignModeTypdef display_mode;
  uint32_t xpos , ypos;
  
  /* Demo Hint */
  HSPI_NOR_SetHint();

  
  for (mode = 0; mode < 3; mode++)
  {
    /********************************** PART 1 **********************************/
    
    /*##-1- Configure the HSPI NOR device ######################################*/
    /* HSPI NOR device configuration */
    if (mode == 0)
    {
      /*-------- SPI mode --------*/
      Hspi_NOR_Init.InterfaceMode = BSP_HSPI_NOR_SPI_MODE;
      Hspi_NOR_Init.TransferRate  = BSP_HSPI_NOR_STR_TRANSFER;
      display_mode = LEFT_MODE;
      xpos = 10;
      ypos = 65;
    }
    else if (mode == 1)
    {
      /* -------- OPI STR mode -------- */
      Hspi_NOR_Init.InterfaceMode = BSP_HSPI_NOR_OPI_MODE;
      Hspi_NOR_Init.TransferRate  = BSP_HSPI_NOR_STR_TRANSFER;
      display_mode = RIGHT_MODE;
      xpos = 15;
      ypos = 65;
    }
    else
    {
      /*-------- OPI DTR mode --------*/
      Hspi_NOR_Init.InterfaceMode = BSP_HSPI_NOR_OPI_MODE;
      Hspi_NOR_Init.TransferRate  = BSP_HSPI_NOR_DTR_TRANSFER;
      display_mode = CENTER_MODE;
      xpos = 0;
      ypos = 290;
    }

    UTIL_LCD_SetFont(&Font16);
    

      /*##-1- Read & check the HSPI NOR info ###################################*/
    /* Read the HSPI NOR memory info */
    if(BSP_HSPI_NOR_GetInfo(0,&Hspi_NOR_Info) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    else if((Hspi_NOR_Info.FlashSize        != MX66UW1G45G_FLASH_SIZE)   ||
       (Hspi_NOR_Info.EraseSectorSize       != MX66UW1G45G_BLOCK_64K)   ||
       (Hspi_NOR_Info.EraseSectorsNumber    != (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_64K)) ||
       (Hspi_NOR_Info.EraseSubSectorSize    != MX66UW1G45G_BLOCK_4K) ||
       (Hspi_NOR_Info.EraseSubSectorNumber  != (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K))    ||
       (Hspi_NOR_Info.EraseSubSector1Size   != MX66UW1G45G_BLOCK_4K)   ||
       (Hspi_NOR_Info.EraseSubSector1Number != (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K)) ||
       (Hspi_NOR_Info.ProgPageSize          != MX66UW1G45G_PAGE_SIZE) ||
       (Hspi_NOR_Info.ProgPagesNumber       != (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_PAGE_SIZE)))
    {
      /* Test the correctness */
      UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" GET INFO : FAILED ", display_mode);
      UTIL_LCD_DisplayStringAt(xpos, ypos + 15, (uint8_t *)"Test Aborted", CENTER_MODE);
      break;
    }
    
    
    status = BSP_HSPI_NOR_Init(0, &Hspi_NOR_Init);
    
    if (status != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    else
    {
       ypos += 15;
      /*##-2- Erase HSPI NOR memory ##########################################*/
      if(BSP_HSPI_NOR_Erase_Block(0, HSPI_NOR_WRITE_READ_ADDR, MX66UW1G45G_ERASE_64K) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase : Failed", display_mode); 
        UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
        break;
      }
      else
      {
        UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase : OK", display_mode); 
        
        /*##-4- HSPI NOR memory read/write access  ###########################*/
        /* Fill the buffer to write */
        Fill_Buffer(HSPI_nor_aTxBuffer, HSPI_NOR_BUFFER_SIZE, 0xD20F);
        
        /* Write data to the HSPI NOR memory */
        if(BSP_HSPI_NOR_Write(0, HSPI_nor_aTxBuffer, HSPI_NOR_WRITE_READ_ADDR, HSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Write : Failed", display_mode); 
          UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
          break;
        }
        else
        {
          ypos += 15;
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Write : OK", display_mode); 
          /* Read back data from the HSPI NOR memory */
          if(BSP_HSPI_NOR_Read(0, HSPI_nor_aRxBuffer, HSPI_NOR_WRITE_READ_ADDR, HSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
          {

            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Read : Failed", display_mode);
            UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
            break;
          }
          else
          {
            ypos += 15;
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Read : OK", display_mode); 
            
            /*##-5- Checking data integrity ##################################*/  
            uint16_t error = 0;
            for (uint16_t index = 0; index < HSPI_NOR_BUFFER_SIZE; index++)
            {             
              if (HSPI_nor_aRxBuffer[index] != HSPI_nor_aTxBuffer[index])
              {
                error ++;
              }
            }
            
            if(error > 0)
            {
              if(Buffercmp(HSPI_nor_aRxBuffer, HSPI_nor_aTxBuffer, HSPI_NOR_BUFFER_SIZE) > 0)
              {
                UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : Failed", display_mode);
                UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                break;
              }
              else
              {
                ypos += 15;
                UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : OK", display_mode);
                
                /*##-6- HSPI NOR memory in memory-mapped mode###################*/
                if(BSP_HSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
                {
                  UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Cfg : Failed", display_mode);
                  UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                  break;
                }
                else
                {
                  ypos += 15; 
                  
                  for(index = 0, data_ptr = (__IO uint8_t *)(HSPI_NOR_BASE_ADDR + HSPI_NOR_WRITE_READ_ADDR);
                      index < HSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
                  {
                    if(*data_ptr != HSPI_nor_aTxBuffer[index])
                    {
                      UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Access : Failed", display_mode);
                      UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                      break;
                    }
                    else
                    {
                      UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Access : OK", display_mode);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    
    /********************************** PART 2 **********************************/
    
    /*##-1- Deconfigure the HSPI NOR device ####################################*/
    status = BSP_HSPI_NOR_DeInit(0);
    
    if (status != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    else
    {      /*##-2- Reconfigure the HSPI NOR device ##################################*/
      /* QSPI device configuration */
      status = BSP_HSPI_NOR_Init(0, &Hspi_NOR_Init);
      
      if (status != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      else
      {
        ypos += 15;
        /*##-3- Erase HSPI NOR memory ##########################################*/
        if(BSP_HSPI_NOR_Erase_Block(0, 0, MX66UW1G45G_ERASE_4K) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase : Failed", display_mode);
          UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
          break;
        }
        else
        {
          /*##-4- Suspend erase HSPI NOR memory ################################*/
          if(BSP_HSPI_NOR_SuspendErase(0) != BSP_ERROR_NONE)
          {
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase Suspend : Failed", display_mode);
            UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
            break;
          }
          else
          {
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase Suspend : OK", display_mode);
            
            ypos += 15;
            
            /*##-6- Resume erase HSPI NOR memory ###############################*/
            if(BSP_HSPI_NOR_ResumeErase(0) != BSP_ERROR_NONE)
            {
              UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase Resume : Failed", display_mode);
              UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
              break;
            }
            else
            {
              UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase Resume : OK", display_mode);
            
              /*##-7- Check HSPI NOR memory status  ############################*/
              /* Wait the end of the current operation on memory side */
              do
              {
                status = BSP_HSPI_NOR_GetStatus(0);
              } while((status != BSP_ERROR_NONE) && (status != BSP_ERROR_COMPONENT_FAILURE));
              
              if(status != BSP_ERROR_NONE)
              {
                UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Memory Status : Failed", display_mode);
                UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                break;
              }
              else
              {
                ypos += 15;
                /*##-8- HSPI NOR memory read access  ###########################*/
                /* Read back data from the HSPI NOR memory */
                if(BSP_HSPI_NOR_Read(0, HSPI_nor_aRxBuffer, 0, MX66UW1G45G_BLOCK_4K) != BSP_ERROR_NONE)
                {
                  UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Read : Failed", display_mode);
                  UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                  break;
                }
                else
                {
                  UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Read : OK", display_mode);
                  
                  /*##-9- Checking data integrity ##############################*/
                  if(DataCmp(HSPI_nor_aRxBuffer, 0xFF, MX66UW1G45G_BLOCK_4K) > 0)
                  {
                    UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : Failed", display_mode);
                    UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                    break;
                  }
                  else
                  {
                    UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : OK", display_mode);
                  }
                }
              }
            }
          }
        }
      }
    }
    
    
    /********************************** PART 3 **********************************/
    
    /*##-1- Deconfigure the HSPI NOR device ####################################*/
    status = BSP_HSPI_NOR_DeInit(0);
    
    if (status != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    else
    {
      /*##-2- Reconfigure the HSPI NOR device ##################################*/
      /* HSPI NOR device configuration */
      status = BSP_HSPI_NOR_Init(0, &Hspi_NOR_Init);
      
      if (status != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      else
      {
        /*##-3- Erase HSPI NOR memory ##########################################*/
        if(BSP_HSPI_NOR_Erase_Block(0, HSPI_NOR_WRITE_READ_ADDR, MX66UW1G45G_ERASE_4K) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase : Failed", display_mode);
          UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
          break;
        }
        else
        {
          ypos += 15;
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Erase : OK", display_mode);
          
          /*##-4- HSPI NOR memory write access #################################*/
          /* Fill the buffer to write */
          Fill_Buffer(HSPI_nor_aTxBuffer, HSPI_NOR_BUFFER_SIZE, 0xD20F);
          
          /* Write data to the HSPI NOR memory */
          if(BSP_HSPI_NOR_Write(0, HSPI_nor_aTxBuffer, HSPI_NOR_WRITE_READ_ADDR, HSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
          {
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Write : Failed", display_mode);
            UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
            break;
          }
          else
          {
            ypos += 15;
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Write : OK", display_mode);
            
            /*##-5- HSPI NOR memory in memory-mapped mode#######################*/
            if(BSP_HSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
            {
              UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Cfg : Failed", display_mode);
              UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
              break;
            }
            else
            {  
              for(index = 0, data_ptr = (__IO uint8_t *)(HSPI_NOR_BASE_ADDR + HSPI_NOR_WRITE_READ_ADDR);
                  index < HSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
              {
                if(*data_ptr != HSPI_nor_aTxBuffer[index])
                {
                  UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Access : Failed", display_mode);
                  UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                  break;
                }
              }
              
              if(index == HSPI_NOR_BUFFER_SIZE)
              {
                UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Mem-Mapped Access : OK", display_mode);
              }
            }
          }
        }
      }
    }
    
    
    /********************************** PART 4 **********************************/
    
    /*##-1- Deconfigure the HSPI NOR device ####################################*/
    status = BSP_HSPI_NOR_DeInit(0);
    
    if (status != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    else
    {
      /*##-2- Reconfigure the HSPI NOR device ##################################*/
      /* HSPI NOR device configuration */
      status = BSP_HSPI_NOR_Init(0, &Hspi_NOR_Init);
      
      if (status != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      else
      {
        ypos += 15;
        /*##-3- Erase HSPI NOR memory ##########################################*/
        if(BSP_HSPI_NOR_Erase_Chip(0) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Chip Erase : Failed", display_mode);
          UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Chip Erase : OK", display_mode);
          
          /* Wait the end of the current operation on memory side */
          do
          {
            status = BSP_HSPI_NOR_GetStatus(0);
          } while((status != BSP_ERROR_NONE) && (status != BSP_ERROR_COMPONENT_FAILURE));
          
          if(status != BSP_ERROR_NONE)
          {
            UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)"Memory Status : Failed", display_mode);
            UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
            break;
          }
          else
          {
            ypos += 15;
            for (index = 0; index < (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K); index++)
            {
              /*##-4- HSPI NOR memory read access  ###############################*/
              /* Read back data from the HSPI NOR memory */
              if(BSP_HSPI_NOR_Read(0, HSPI_nor_aRxBuffer, (index * MX66UW1G45G_BLOCK_4K), MX66UW1G45G_BLOCK_4K) != BSP_ERROR_NONE)
              {
                UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Read : Failed", display_mode);
                UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                break;
              }
              else
              {
                /*##-5- Checking data integrity ################################*/
                if(DataCmp(HSPI_nor_aRxBuffer, 0xFF, MX66UW1G45G_BLOCK_4K) > 0)
                {
                  UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : Failed", display_mode);
                  UTIL_LCD_DisplayStringAt(xpos, ypos+15, (uint8_t *)"Test Aborted", display_mode);
                  break;
                }
              }
            }
            
            if (index == (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K))
            {
              UTIL_LCD_DisplayStringAt(xpos, ypos, (uint8_t *)" Compare : OK", display_mode);
            }
          }
        }
      }
    }
    
    /* De-initialization in order to have correct configuration memory on next try */
    BSP_HSPI_NOR_DeInit(0);
    
  }
  
  BSP_LCD_FillRect(0, 0, 0, 800, 30, LCD_COLOR_BLACK);

  BSP_LCD_DrawBitmap(0, 360, 0, (uint8_t *)back);

  while ((x < 380));
  while ((x > 440));
  while ((y > 57));

  DrawBackround = 0;
  return 0;
}


/**
* @brief  Display HSPI NOR Demo Hint
* @param  None
* @retval None
*/
static void HSPI_NOR_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)" HSPI NOR Test", CENTER_MODE); 
  
  UTIL_LCD_DrawHLine(100, 240 , 600 , UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_DrawVLine(400, 60 , 160 , UTIL_LCD_COLOR_GRAY);
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"            SPI mode", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"OPI STR mode              ", RIGHT_MODE); 
  UTIL_LCD_DisplayStringAt(0, 270, (uint8_t *)"OPI DTR mode", CENTER_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
}

/**
* @brief  Fills buffer with user predefined data.
* @param  pBuffer: pointer on the buffer to fill
* @param  uwBufferLenght: size of the buffer to fill
* @param  uwOffset: first value to fill on the buffer
* @retval None
*/
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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
* @brief  Compares a buffer with data.
* @param  pBuffer, pBuffer2: buffers to be compared.
* @param  Pattern: data reference pattern.
* @param  BufferLength: buffer's length
* @retval 1: pBuffer identical to pBuffer1
*         0: pBuffer differs from pBuffer1
*/
static uint8_t DataCmp(uint8_t* pBuffer, uint8_t Pattern, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != Pattern)
    {
      return 1;
    }

    pBuffer++;
  }

  return 0;
}
/**
* @}
*/

/**
* @}
*/