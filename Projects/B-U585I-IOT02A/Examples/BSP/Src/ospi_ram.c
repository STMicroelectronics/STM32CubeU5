/**
  ******************************************************************************
  * @file    ospi_nor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the OSPI RAM feature in the 
  *          b_u585i_iot02_ospi driver
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
#include "string.h"

/** @addtogroup STM32U5xx_HAL_Examples
* @{
*/

/** @addtogroup BSP
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t) 0x1000)
#define OSPI_RAM_START_ADDRESS   0x00
#define TEST_VALUE_START    0x41

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t ospir_ram_aTxBuffer[BUFFER_SIZE];
uint8_t ospir_ram_aRxBuffer[BUFFER_SIZE];
/* Buffer used for transmission */
uint8_t aTxBuffer_ospi[] = " ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication**** ";

/* Private function prototypes -----------------------------------------------*/
static void OSPI_RAM_ReadWrite(void);
static void OSPI_RAM_MemoryMapped(void);
static void    Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t RMABuffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength, uint32_t *ErrorAd);

/* Private functions ---------------------------------------------------------*/
/**
* @brief  OSPI Demo
* @param  None
* @retval None
*/
int32_t OSPI_RAM_demo(void)
{
  printf("\n******OSPI RAM EXAMPLE******\n");

  StartExample();

  /* OSPI Read/Write operation *************************************/
  OSPI_RAM_ReadWrite();

  /* OSPI Memory Mapped *************************************/
  OSPI_RAM_MemoryMapped();

  return 0;
}

/**
* @brief  OSPI RAM Read/Write
* @param  None
* @retval None
*/
void OSPI_RAM_ReadWrite(void)
{
  uint32_t Offset;
  uint32_t error_code = 0;
  uint8_t Id;

  printf("OSPI_RAM_ReadWrite.\n");

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(ospir_ram_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);
  
  /* OSPI device configuration */
  if(BSP_OSPI_RAM_Init(0) != BSP_ERROR_NONE)
  {
    printf("Deinitialization failed Failed\n");
    Error_Handler();
  }
 
  BSP_OSPI_RAM_ReadID(0, &Id);

  if(BSP_OSPI_RAM_Write(0, ospir_ram_aTxBuffer, OSPI_RAM_START_ADDRESS, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    printf("Write : FAILED.\n");
    printf("Test Aborted.\n");
    error_code++;
  }
  
  if(BSP_OSPI_RAM_Read(0, ospir_ram_aRxBuffer, OSPI_RAM_START_ADDRESS, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    printf("Read : FAILED.\n");
    printf("Test Aborted.\n");
    error_code++;
  }

  if(RMABuffercmp(ospir_ram_aRxBuffer, ospir_ram_aTxBuffer, (uint32_t)BUFFER_SIZE, &Offset) != 0)
  {
    printf("Comparison : FAILED.\n");
    printf("Test Aborted.\n");
    error_code++;
  }

  if (error_code == 0)
  {
    printf("OSPI_RAM_ReadWrite Successful\n");
  }
  else
  {
    printf("OSPI_RAM_ReadWrite Failed\n");
  }

  /* Clear the buffer for the next test */    
  memset(ospir_ram_aRxBuffer, 0, sizeof(ospir_ram_aRxBuffer));
  memset(ospir_ram_aTxBuffer, 0, sizeof(ospir_ram_aTxBuffer));   
  
  /* OSPI device configuration */
  if(BSP_OSPI_RAM_DeInit(0) != BSP_ERROR_NONE)
  {
    printf("Deinitialization failed Failed\n");
    Error_Handler();
  }
}

/**
* @brief  OSPI RAM Read/Write
* @param  None
* @retval None
*/
void OSPI_RAM_MemoryMapped(void)
{
  uint32_t error_code = 0;
  uint16_t index;
  uint8_t *mem_addr;
  uint8_t temp;

  printf("OSPI_RAM_MemoryMapped.\n");

  /* OSPI device configuration */
  if(BSP_OSPI_RAM_Init(0) != BSP_ERROR_NONE)
  {
    printf("Deinitialization failed Failed\n");
    Error_Handler();
  }
 
  if(BSP_OSPI_RAM_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    printf("EnableMemoryMappedMode : FAILED.\n");
    printf("Test Aborted.\n");
    error_code++;
  }
  
  if(error_code == 0)
  {
    /* Writing Sequence ----------------------------------------------- */
    mem_addr = (uint8_t *)(OCTOSPI1_BASE);
    for (index = 0; index < 0x100; index++)
    {
      *mem_addr = aTxBuffer_ospi[index];
      mem_addr++;
    }
    
    /* In memory-mapped mode, not possible to check if the memory is ready 
    after the programming. So a delay corresponding to max page programming
    time is added */
    HAL_Delay(100);
    
    /* Reading Sequence ----------------------------------------------- */
    mem_addr = (uint8_t *)(OCTOSPI1_BASE);
    for (index = 0; index < 0x100; index++)
    {
      temp = *mem_addr;
      if (temp != aTxBuffer_ospi[index])
      {
        printf("Memory mapped read failed.\n");
      }
      mem_addr++;
    }
    printf("OSPI_RAM_MemoryMapped : Successful\n");
  }
  
  /* OSPI device configuration */
  if(BSP_OSPI_RAM_DeInit(0) != BSP_ERROR_NONE)
  {
    printf("Deinitialization failed Failed\n");
    Error_Handler();
  }
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
*         ErrorAd: Difference address
* @retval 0: pBuffer identical to pBuffer1
*         1: pBuffer differs from pBuffer1
*/
static uint8_t RMABuffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength, uint32_t *ErrorAd)
{
  (*ErrorAd) = 0;

  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    (*ErrorAd)++;
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
