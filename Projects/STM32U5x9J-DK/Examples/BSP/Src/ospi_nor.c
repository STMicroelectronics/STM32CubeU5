/**
******************************************************************************
* @file    Examples/BSP/Src/ospi_nor.c
* @author  MCD Application Team
* @brief   This example code shows how to use the OSPI Driver
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
#define OSPI_NOR_BUFFER_SIZE     ((uint32_t)0x0200)
#define OSPI_NOR_WRITE_READ_ADDR ((uint32_t)0x0050)
#define OSPI_NOR_BASE_ADDR       ((uint32_t)0x90000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ospi_nor_aTxBuffer[OSPI_NOR_BUFFER_SIZE];
uint8_t ospi_nor_aRxBuffer[MX25UM51245G_SUBSECTOR_4K];

/* External variables --------------------------------------------------------*/
extern __IO uint32_t TamperButtonPressed;
extern __IO TS_State_t TS_State;
/* Private function prototypes -----------------------------------------------*/
static void     OSPI_NOR_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static uint8_t  DataCmp     (uint8_t* pBuffer, uint8_t Pattern, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  OSPI NOR Demo
* @param  None
* @retval None
*/
int32_t OSPI_NOR_demo (void)
{
  int32_t result = 0;
  /* OSPI info structure */
  BSP_OSPI_NOR_Info_t sOSPI_NOR_Info;
  BSP_OSPI_NOR_Init_t sOSPI_NOR_Init;
  int32_t status;
  __IO uint8_t *data_ptr;
  uint32_t index;
  uint8_t mode;

  /* Demo Hint */
  OSPI_NOR_SetHint();

  for (mode = 0; mode < 3; mode++)
  {
    /********************************** PART 1 **********************************/

    /*##-1- Configure the OSPI NOR device ######################################*/
    /* OSPI NOR device configuration */
    if (mode == 0)
    {
      printf("\r\n -------- SPI mode --------");
      sOSPI_NOR_Init.InterfaceMode = BSP_OSPI_NOR_SPI_MODE;
      sOSPI_NOR_Init.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
    }
    else if (mode == 1)
    {
      printf("\r\n -------- OPI STR mode --------");
      sOSPI_NOR_Init.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
      sOSPI_NOR_Init.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
    }
    else
    {
      printf("\r\n -------- OPI DTR mode --------");
      sOSPI_NOR_Init.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
      sOSPI_NOR_Init.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
    }
    status = BSP_OSPI_NOR_Init(0, &sOSPI_NOR_Init);

    if (status != BSP_ERROR_NONE)
    {
      printf("\r\nOSPI NOR Initialization : Failed");
      printf("\r\nOSPI NOR Test Aborted");
      printf("\r\n");
      result --;
    }
    else
    {
      printf("\r\nOSPI NOR Initialization : OK");

      /*##-2- Read & check the OSPI NOR info ###################################*/
      /* Initialize the structure */
      sOSPI_NOR_Info.FlashSize             = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSectorSize       = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSectorsNumber    = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSubSectorSize    = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSubSectorNumber  = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSubSector1Size   = (uint32_t)0x00;
      sOSPI_NOR_Info.EraseSubSector1Number = (uint32_t)0x00;
      sOSPI_NOR_Info.ProgPageSize          = (uint32_t)0x00;
      sOSPI_NOR_Info.ProgPagesNumber       = (uint32_t)0x00;

      /* Read the OSPI NOR memory info */
      BSP_OSPI_NOR_GetInfo(0,&sOSPI_NOR_Info);

      /* Test the correctness */
      if((sOSPI_NOR_Info.FlashSize             != MX25UM51245G_FLASH_SIZE)   ||
         (sOSPI_NOR_Info.EraseSectorSize       != MX25UM51245G_SECTOR_64K)   ||
           (sOSPI_NOR_Info.EraseSubSectorSize    != MX25UM51245G_SUBSECTOR_4K) ||
             (sOSPI_NOR_Info.EraseSubSector1Size   != MX25UM51245G_SUBSECTOR_4K) ||
               (sOSPI_NOR_Info.ProgPageSize          != MX25UM51245G_PAGE_SIZE)    ||
                 (sOSPI_NOR_Info.EraseSectorsNumber    != (MX25UM51245G_FLASH_SIZE/MX25UM51245G_SECTOR_64K))   ||
                   (sOSPI_NOR_Info.EraseSubSectorNumber  != (MX25UM51245G_FLASH_SIZE/MX25UM51245G_SUBSECTOR_4K)) ||
                     (sOSPI_NOR_Info.EraseSubSector1Number != (MX25UM51245G_FLASH_SIZE/MX25UM51245G_SUBSECTOR_4K)) ||
                       (sOSPI_NOR_Info.ProgPagesNumber       != (MX25UM51245G_FLASH_SIZE/MX25UM51245G_PAGE_SIZE)))
      {
        printf("\r\nOSPI NOR Get Info : Failed");
        printf("\r\nOSPI NOR Test Aborted");
        printf("\r\n");
        result --;
      }
      else
      {
        printf("\r\nOSPI NOR Get Info : OK");

        /*##-3- Erase OSPI NOR memory ##########################################*/
        if(BSP_OSPI_NOR_Erase_Block(0, OSPI_NOR_WRITE_READ_ADDR, MX25UM51245G_ERASE_64K) != BSP_ERROR_NONE)
        {
          printf("\r\nOSPI NOR Erase : Failed");
          printf("\r\nOSPI NOR Test Aborted");
          printf("\r\n");
          result --;
        }
        else
        {
          printf("\r\nOSPI NOR Erase : OK");

          /*##-4- OSPI NOR memory read/write access  ###########################*/
          /* Fill the buffer to write */
          Fill_Buffer(ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE, 0xD20F);

          /* Write data to the OSPI NOR memory */
          if(BSP_OSPI_NOR_Write(0, ospi_nor_aTxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
          {
            printf("\r\nOSPI NOR Write : Failed");
            printf("\r\nOSPI NOR Test Aborted");
            printf("\r\n");
            result --;
          }
          else
          {
            printf("\r\nOSPI NOR Write : OK");

            /* Read back data from the OSPI NOR memory */
            if(BSP_OSPI_NOR_Read(0, ospi_nor_aRxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
            {
              printf("\r\nOSPI NOR Read : Failed");
              printf("\r\nOSPI NOR Test Aborted");
              printf("\r\n");
              result --;
            }
            else
            {
              printf("\r\nOSPI NOR Read : OK");

              /*##-5- Checking data integrity ##################################*/
              uint16_t error = 0;
              for (uint16_t index = 0; index < OSPI_NOR_BUFFER_SIZE; index++)
              {
                if (ospi_nor_aRxBuffer[index] != ospi_nor_aTxBuffer[index])
                {
                  error ++;
                }
              }

              if(error > 0)
							{
                if(Buffercmp(ospi_nor_aRxBuffer, ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE) > 0)
                {
                  printf("\r\nOSPI NOR Compare : Failed");
                  printf("\r\nOSPI NOR Test Aborted");
                  printf("\r\n");
                  result --;
                }
                else
                {
                  printf("\r\nOSPI NOR Compare : OK");

                  /*##-6- OSPI NOR memory in memory-mapped mode###################*/
                  if(BSP_OSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
                  {
                    printf("\r\nOSPI NOR Mem-Mapped Cfg : Failed");
                    printf("\r\nOSPI NOR Test Aborted");
                    printf("\r\n");
                    result --;
                  }
                  else
                  {
                    printf("\r\nOSPI NOR Mem-Mapped Cfg : OK");

                    for(index = 0, data_ptr = (__IO uint8_t *)(OSPI_NOR_BASE_ADDR + OSPI_NOR_WRITE_READ_ADDR);
                        index < OSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
                    {
                      if(*data_ptr != ospi_nor_aTxBuffer[index])
                      {
                        printf("\r\nOSPI NOR Mem-Mapped Access : Failed");
                        printf("\r\nOSPI NOR Test Aborted");
                        printf("\r\n");
                        result --;
                        break;
                      }
                    }

                    if(index == OSPI_NOR_BUFFER_SIZE)
                    {
                      printf("\r\nOSPI NOR Mem-Mapped Access : OK");
                      printf("\r\nOSPI NOR Test : OK");
                      printf("\r\n");
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

    /*##-1- Deconfigure the OSPI NOR device ####################################*/
    status = BSP_OSPI_NOR_DeInit(0);

    if (status != BSP_ERROR_NONE)
    {
      printf("\r\nOSPI NOR De-Initialization : Failed");
      printf("\r\nOSPI NOR Test Aborted");
      printf("\r\n");
      result --;
    }
    else
    {
      printf("\r\nOSPI NOR De-Initialization : OK");

      /*##-2- Reconfigure the OSPI NOR device ##################################*/
      /* QSPI device configuration */
      status = BSP_OSPI_NOR_Init(0, &sOSPI_NOR_Init);

      if (status != BSP_ERROR_NONE)
      {
        printf("\r\nOSPI NOR Initialization : Failed");
        printf("\r\nOSPI NOR Test Aborted");
        printf("\r\n");
        result --;
      }
      else
      {
        printf("\r\nOSPI NOR Initialization : OK");

        /*##-3- Erase OSPI NOR memory ##########################################*/
        if(BSP_OSPI_NOR_Erase_Block(0, 0, MX25UM51245G_ERASE_4K) != BSP_ERROR_NONE)
        {
          printf("\r\nOSPI NOR Erase : Failed");
          printf("\r\nOSPI NOR Test Aborted");
          printf("\r\n");
          result --;
        }
        else
        {

          /*##-4- Suspend erase OSPI NOR memory ################################*/
          if(BSP_OSPI_NOR_SuspendErase(0) != BSP_ERROR_NONE)
          {
            printf("\r\nOSPI NOR Erase : OK");
            printf("\r\nOSPI NOR Erase Suspend : Failed");
            printf("\r\nOSPI NOR Test Aborted");
            printf("\r\n");
            result --;
          }
          else
          {
            printf("\r\nOSPI NOR Erase : OK");
            printf("\r\nOSPI NOR Erase Suspend : OK");

            /*##-6- Resume erase OSPI NOR memory ###############################*/
            if(BSP_OSPI_NOR_ResumeErase(0) != BSP_ERROR_NONE)
            {
              printf("\r\nOSPI NOR Erase Resume : Failed");
              printf("\r\nOSPI NOR Test Aborted");
              printf("\r\n");
              result --;
            }
            else
            {
              printf("\r\nOSPI NOR Erase Resume : OK");

              /*##-7- Check OSPI NOR memory status  ############################*/
              /* Wait the end of the current operation on memory side */
              do
              {
                status = BSP_OSPI_NOR_GetStatus(0);
              } while((status != BSP_ERROR_NONE) && (status != BSP_ERROR_COMPONENT_FAILURE));

              if(status != BSP_ERROR_NONE)
              {
                printf("\r\nOSPI NOR Memory Status : Failed");
                printf("\r\nOSPI NOR Test Aborted");
                printf("\r\n");
                result --;
              }
              else
              {
                printf("\r\nOSPI NOR Memory Status : OK");

                /*##-8- OSPI NOR memory read access  ###########################*/
                /* Read back data from the OSPI NOR memory */
                if(BSP_OSPI_NOR_Read(0, ospi_nor_aRxBuffer, 0, MX25UM51245G_SUBSECTOR_4K) != BSP_ERROR_NONE)
                {
                  printf("\r\nOSPI NOR Read : Failed");
                  printf("\r\nOSPI NOR Test Aborted");
                  printf("\r\n");
                  result --;
                }
                else
                {
                  printf("\r\nOSPI NOR Read : OK");

                  /*##-9- Checking data integrity ##############################*/
                  if(DataCmp(ospi_nor_aRxBuffer, 0xFF, MX25UM51245G_SUBSECTOR_4K) > 0)
                  {
                    printf("\r\nOSPI NOR Compare : Failed");
                    printf("\r\nOSPI NOR Test Aborted");
                    printf("\r\n");
                    result --;
                  }
                  else
                  {
                    printf("\r\nOSPI NOR Compare : OK");
                    printf("\r\nOSPI NOR Test : OK");
                    printf("\r\n");
                  }
                }
              }
            }
          }
        }
      }
    }


    /********************************** PART 3 **********************************/

    /*##-1- Deconfigure the OSPI NOR device ####################################*/
    status = BSP_OSPI_NOR_DeInit(0);

    if (status != BSP_ERROR_NONE)
    {
      printf("\r\nOSPI NOR De-Initialization : Failed");
      printf("\r\nOSPI NOR Test Aborted");
      printf("\r\n");
      result --;
    }
    else
    {
      printf("\r\nOSPI NOR De-Initialization : OK");

      /*##-2- Reconfigure the OSPI NOR device ##################################*/
      /* OSPI NOR device configuration */
      status = BSP_OSPI_NOR_Init(0, &sOSPI_NOR_Init);

      if (status != BSP_ERROR_NONE)
      {
        printf("\r\nOSPI NOR Initialization : Failed");
        printf("\r\nOSPI NOR Test Aborted");
        printf("\r\n");
        result --;
      }
      else
      {
        printf("\r\nOSPI NOR Initialization : OK");

        /*##-3- Erase OSPI NOR memory ##########################################*/
        if(BSP_OSPI_NOR_Erase_Block(0, OSPI_NOR_WRITE_READ_ADDR, MX25UM51245G_ERASE_4K) != BSP_ERROR_NONE)
        {
          printf("\r\nOSPI NOR Erase : Failed");
          printf("\r\nOSPI NOR Test Aborted");
          printf("\r\n");
          result --;
        }
        else
        {
          printf("\r\nOSPI NOR Erase : OK");

          /*##-4- OSPI NOR memory write access #################################*/
          /* Fill the buffer to write */
          Fill_Buffer(ospi_nor_aTxBuffer, OSPI_NOR_BUFFER_SIZE, 0xD20F);

          /* Write data to the OSPI NOR memory */
          if(BSP_OSPI_NOR_Write(0, ospi_nor_aTxBuffer, OSPI_NOR_WRITE_READ_ADDR, OSPI_NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
          {
            printf("\r\nOSPI NOR Write : Failed");
            printf("\r\nOSPI NOR Test Aborted");
            printf("\r\n");
            result --;
          }
          else
          {
            printf("\r\nOSPI NOR Write : OK");

            /*##-5- OSPI NOR memory in memory-mapped mode#######################*/
            if(BSP_OSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
            {
              printf("\r\nOSPI NOR Mem-Mapped Cfg : Failed");
              printf("\r\nOSPI NOR Test Aborted");
              printf("\r\n");
              result --;
            }
            else
            {
              printf("\r\nOSPI NOR Mem-Mapped Cfg : OK");

              for(index = 0, data_ptr = (__IO uint8_t *)(OSPI_NOR_BASE_ADDR + OSPI_NOR_WRITE_READ_ADDR);
                  index < OSPI_NOR_BUFFER_SIZE; index++, data_ptr++)
              {
                if(*data_ptr != ospi_nor_aTxBuffer[index])
                {
                  printf("\r\nOSPI NOR Mem-Mapped Access : Failed");
                  printf("\r\nOSPI NOR Test Aborted");
                  printf("\r\n");
                  result --;
                  break;
                }
              }

              if(index == OSPI_NOR_BUFFER_SIZE)
              {
                printf("\r\nOSPI NOR Mem-Mapped Access : OK");
                printf("\r\nOSPI NOR Test : OK");
                printf("\r\n");
              }
            }
          }
        }
      }
    }


    /********************************** PART 4 **********************************/

    /*##-1- Deconfigure the OSPI NOR device ####################################*/
    status = BSP_OSPI_NOR_DeInit(0);

    if (status != BSP_ERROR_NONE)
    {
      printf("\r\nOSPI NOR De-Initialization : Failed");
      printf("\r\nOSPI NOR Test Aborted");
      printf("\r\n");
      result --;
    }
    else
    {
      printf("\r\nOSPI NOR De-Initialization : OK");

      /*##-2- Reconfigure the OSPI NOR device ##################################*/
      /* OSPI NOR device configuration */
      status = BSP_OSPI_NOR_Init(0, &sOSPI_NOR_Init);

      if (status != BSP_ERROR_NONE)
      {
        printf("\r\nOSPI NOR Initialization : Failed");
        printf("\r\nOSPI NOR Test Aborted");
        printf("\r\n");
        result --;
      }
      else
      {
        printf("\r\nOSPI NOR Initialization : OK");

        /*##-3- Erase OSPI NOR memory ##########################################*/
        if(BSP_OSPI_NOR_Erase_Chip(0) != BSP_ERROR_NONE)
        {
          printf("\r\nOSPI NOR Chip Erase : Failed");
          printf("\r\nOSPI NOR Test Aborted");
          printf("\r\n");
          result --;
        }
        else
        {
          printf("\r\nOSPI NOR Chip Erase : OK");

          /* Wait the end of the current operation on memory side */
          do
          {
            status = BSP_OSPI_NOR_GetStatus(0);
          } while((status != BSP_ERROR_NONE) && (status != BSP_ERROR_COMPONENT_FAILURE));

          if(status != BSP_ERROR_NONE)
          {
            printf("\r\nOSPI NOR Memory Status : Failed");
            printf("\r\nOSPI NOR Test Aborted");
            printf("\r\n");
            result --;
          }
          else
          {
            printf("\r\nOSPI NOR Memory Status : OK");

            for (index = 0; index < (MX25UM51245G_FLASH_SIZE / MX25UM51245G_SUBSECTOR_4K); index++)
            {
              /*##-4- OSPI NOR memory read access  ###############################*/
              /* Read back data from the OSPI NOR memory */
              if(BSP_OSPI_NOR_Read(0, ospi_nor_aRxBuffer, (index * MX25UM51245G_SUBSECTOR_4K), MX25UM51245G_SUBSECTOR_4K) != BSP_ERROR_NONE)
              {
                printf("\r\nOSPI NOR Read : Failed");
                printf("\r\nOSPI NOR Test Aborted");
                printf("\r\n");
                result --;
                break;
              }
              else
              {
                /*##-5- Checking data integrity ################################*/
                if(DataCmp(ospi_nor_aRxBuffer, 0xFF, MX25UM51245G_SUBSECTOR_4K) > 0)
                {
                  printf("\r\nOSPI NOR Compare : Failed");
                  printf("\r\nOSPI NOR Test Aborted");
                  printf("\r\n");
                  result --;
                  break;
                }
              }
            }

            if (index == (MX25UM51245G_FLASH_SIZE / MX25UM51245G_SUBSECTOR_4K))
            {
              printf("\r\nOSPI NOR Test : OK");
              printf("\r\n");
            }
          }
        }
      }
    }

    /* De-initialization in order to have correct configuration memory on next try */
    BSP_OSPI_NOR_DeInit(0);

  }

  BSP_LCD_DrawBitmap(0, 210, 0, (uint8_t *)back);

  while (x < 190);
  while (x > 300);
  while (y > 55);
  DrawBackround = 0;
  return 0;
}


/**
* @brief  Display OSPI NOR Demo Hint
* @param  None
* @retval None
*/
static void OSPI_NOR_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_BLACK);

  /* Display text */
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTBLUE);
  UTIL_LCD_DisplayStringAt(0, 170, (uint8_t *)"Test of OSPI", CENTER_MODE);

  UTIL_LCD_DisplayStringAt(0, 230, (uint8_t *)"Please check the result", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 250, (uint8_t *)"on IO Terminal", CENTER_MODE);
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
