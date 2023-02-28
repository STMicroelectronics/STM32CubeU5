/**
  ******************************************************************************
  * @file    Examples/BSP/Src/hspi.c
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
#define HSPI_BUFFER_SIZE     ((uint32_t)0x8000)
#define HSPI_WRITE_READ_ADDR ((uint32_t)0x0050)
#define HSPI_BASE_ADDR       ((uint32_t)0xA0000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t hspi_aTxBuffer[HSPI_BUFFER_SIZE];
uint8_t hspi_aRxBuffer[HSPI_BUFFER_SIZE];
uint8_t ubHspiRamDmaCplt = 0;

extern __IO TS_State_t TS_State;
/* Private function prototypes -----------------------------------------------*/
static void     hspi_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  HSPI RAM Demo
* @param  None
* @retval None
*/
int32_t OSPI_RAM_demo (void)
{
  int32_t result = 0;
  BSP_HSPI_RAM_Cfg_t sHSPI_Init;
  uint8_t mode;

  /* Demo Hint */
  hspi_SetHint();
  
  printf("\r\n################## Start HSPI test #####################");
  printf("\r\n");
  for (mode = 0; mode < 5; mode++)
  {
    /********************************** PART 1 **********************************/

    /*##-1- Configure the HSPI RAM device ######################################*/
    /* HSPI RAM device configuration */
    if (mode == 0)
    {
      printf("\r\n ---- Fixed Latency 2*3, Linear burst, x8 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_16_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_3;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_3;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 1)
    {
      printf("\r\n ---- Variable Latency 4, Hybrid 16-bytes burst, x8 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_VARIABLE_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_16_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_4;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_4;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 2)
    {
      printf("\r\n ---- Fixed Latency 2*5, Hybrid 32-bytes burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_32_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_5;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_5;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 3)
    {
      printf("\r\n ---- Variable Latency 6, Hybrid 64-bytes burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_VARIABLE_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_64_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_6;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_6;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    else if (mode == 4)
    {
      printf("\r\n ---- Fixed Latency 2*7, Linear burst, x16 IO ----");
      sHSPI_Init.LatencyType      = BSP_HSPI_RAM_FIXED_LATENCY;
      sHSPI_Init.BurstType        = BSP_HSPI_RAM_LINEAR_BURST;
      sHSPI_Init.BurstLength      = BSP_HSPI_RAM_BURST_64_BYTES;
      sHSPI_Init.ReadLatencyCode  = BSP_HSPI_RAM_READ_LATENCY_7;
      sHSPI_Init.WriteLatencyCode = BSP_HSPI_RAM_WRITE_LATENCY_7;
      sHSPI_Init.IOMode           = BSP_HSPI_RAM_IO_X16_MODE;
    }
    printf("\r\n");

    if (BSP_HSPI_RAM_Init(0, &sHSPI_Init) != BSP_ERROR_NONE)
    {
      printf("\r\nHSPI RAM Initialization : Failed");
      printf("\r\nHSPI RAM Test Aborted");
      printf("\r\n");
      result--;
    }
    else
    {
      /*##-2- HSPI RAM memory read/write access  ###############################*/
      /* Fill the buffer to write */
      Fill_Buffer(hspi_aTxBuffer, HSPI_BUFFER_SIZE, 0xD20F);

      /* Write data to the HSPI RAM memory */
      if(BSP_HSPI_RAM_Write(0, hspi_aTxBuffer, HSPI_WRITE_READ_ADDR, HSPI_BUFFER_SIZE) != BSP_ERROR_NONE)
      {
        printf("\r\nHSPI RAM Write : Failed");
        printf("\r\nHSPI RAM Test Aborted");
        printf("\r\n");
        result--;
      }
      else
      {
        /* Read back data from the HSPI RAM memory */
        if(BSP_HSPI_RAM_Read(0, hspi_aRxBuffer, HSPI_WRITE_READ_ADDR, HSPI_BUFFER_SIZE) != BSP_ERROR_NONE)
        {
          printf("\r\nHSPI RAM Read : Failed");
          printf("\r\nHSPI RAM Test Aborted");
          printf("\r\n");
          result--;
        }
        else
        {
          /*##-3- Checking data integrity ######################################*/
          if(Buffercmp(hspi_aRxBuffer, hspi_aTxBuffer, HSPI_BUFFER_SIZE) > 0)
          {
            printf("\r\nHSPI RAM Compare : Failed");
            printf("\r\nHSPI RAM Test Aborted");
            printf("\r\n");
            result--;
          }
          else
          {
            printf("\r\nHSPI Test : OK");
            printf("\r\n");
          }
        }
      }
    }

    /* De-initialization in order to have correct configuration memory on next try */
    BSP_HSPI_RAM_DeInit(0);
  }
  printf("\r\n################## End HSPI test #####################");
  printf("\r\n");

  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);
  
  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"Please check the result", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 250, (uint8_t *)"on IO Terminal", CENTER_MODE);
  
  while ((TS_State.TouchX < 190));
  while ((TS_State.TouchX > 300));
  while ((TS_State.TouchY > 55));
  DrawBackround = 0;
  return 0;
}

/**
  * @brief  Display hspi demo hint
  * @param  None
  * @retval None
  */
static void hspi_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);  
  
  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 170, (uint8_t *)"Test of HSPI", CENTER_MODE); 
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
  * @brief  Rx Transfer completed callback.
  * @param  hhspi: HSPI handle
  * @retval None
  */
void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hhspi)
{
  ubHspiRamDmaCplt++;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hhspi: HSPI handle
  * @retval None
  */
void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hhspi)
{
  ubHspiRamDmaCplt++;
}
/**
* @}
*/

/**
* @}
*/

