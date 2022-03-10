/**
  ******************************************************************************
  * @file    eeprom.c
  * @author  MCD Application Team
  * @brief   This example code shows how to manage I2C EEPROM memory
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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define EEPROM_FEATURES_NUM     3
#define EEPROM_WRITE_ADDRESS1   0x0
#define EEPROM_READ_ADDRESS1    0x0
#define BUFFER_SIZE1            8192

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BUFFER_SIZE  countof(Tx1Buffer)

/* Private variables ---------------------------------------------------------*/
static uint8_t EepromFeature = 0;
char Tx1Buffer[] = "***B_U585I_IOT02A EEPROM***READ AND WRITE****";
char RxBuffer[BUFFER_SIZE] = {0};
char Rx1Buffer[BUFFER_SIZE1] = {0};
char Tx2Buffer[BUFFER_SIZE1] = {0};

/* Private function prototypes -----------------------------------------------*/
static void EEPROM_Show_Feature(uint8_t feature);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void FillBuffer(uint8_t* pBuffer1, uint16_t Index, uint16_t BufferLength);
extern __IO uint8_t NbLoop;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
int32_t EEPROM_demo (void)
{
  printf("Press USER button to start the example\n");

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  EepromFeature = 0;

  while (1)
  {
    if(UserButtonPressed == SET)
    {
      UserButtonPressed = RESET;
      if(EepromFeature < EEPROM_FEATURES_NUM)
      {
        EEPROM_Show_Feature(EepromFeature++);
      }
      else
      {
        UserButtonPressed = RESET;
        return 0;
      }
    }
  }
}

/**
  * @brief  Show EEPROM Features
  * @param  feature : feature index
  * @retval None
  */
static void EEPROM_Show_Feature(uint8_t feature)
{
  __IO TestStatus TransferStatus1 = FAILED;
  __IO uint32_t NumDataRead = 0;

  /* Initialize the EEPROM driver --------------------------------------------*/
  if (BSP_EEPROM_Init(0) != BSP_ERROR_NONE)
  {
    printf("Initialization problem\n");
    return;
  }

  switch (feature)
  {
  case 0:
    /* Read old parameter in EEPROM */
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      /* Set the Number of data to be read */
      NumDataRead = BUFFER_SIZE;

      /* Read from EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(0, (uint8_t*)RxBuffer, EEPROM_READ_ADDRESS1, NumDataRead) != BSP_ERROR_NONE)
      {
        printf("Init issue at read old data\n");
        return;
      }
      printf("String read in the current EEPROM selected:\n");
      printf("%s\n", RxBuffer);
    }
    else
    {
      printf("Problem to communicate with EEPROM\n");
    }

    break;

  case 1:
     /* Write new parameter in EEPROM */
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      FillBuffer((uint8_t*)Tx2Buffer, 0x21, BUFFER_SIZE1);

      printf("Writing data to memory ...\n");
      /* First write in the memory followed by a read of the written data ----*/
      /* Write on EEPROM to EEPROM_WRITE_ADDRESS1 */
      if (BSP_EEPROM_WriteBuffer(0, (uint8_t*)Tx2Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE1) != BSP_ERROR_NONE)
      {
        printf("Init issue at write\n");
        return;
      }

      /* Set the Number of data to be read */
      NumDataRead = (uint32_t)BUFFER_SIZE1;

      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(0, (uint8_t*)Rx1Buffer, EEPROM_READ_ADDRESS1, NumDataRead) != BSP_ERROR_NONE)
      {
        printf("Init issue at read\n");
        return;
      }

      /* Check if the data written to the memory is read correctly */
      TransferStatus1 = Buffercmp((uint8_t*)Tx2Buffer, (uint8_t*)Rx1Buffer, BUFFER_SIZE1);
      if(TransferStatus1 != FAILED)
      {
        printf("String writes in the current EEPROM selected:\n");
        printf("READ/WRITE SUCCECC\n");
      }
      else
      {
        printf("FAILED to write!\n");
      }
    }
    else
    {
      printf("Problem to communicate\n");
    }
    break;
  case 2:
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      FillBuffer((uint8_t*)Tx2Buffer, 0xAA, EEPROM_PAGESIZE);

      for(uint32_t i = 0; i < 128; i++)
      {
        /* First write in the memory followed by a read of the written data ----*/
        /* Write on EEPROM to EEPROM_WRITE_ADDRESS1 */
        if (BSP_EEPROM_WritePage(0, (uint8_t*)Tx2Buffer, i) != BSP_ERROR_NONE)
        {
          printf("Init issue at write page\n");
          return;
        }

        /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
        if (BSP_EEPROM_ReadPage(0, (uint8_t*)Rx1Buffer, i) != BSP_ERROR_NONE)
        {
          printf("Init issue at read page\n");
          return;
        }

        /* Check if the data written to the memory is read correctly */
        TransferStatus1 = Buffercmp((uint8_t*)Tx2Buffer, (uint8_t*)Rx1Buffer, EEPROM_PAGESIZE);
        if(TransferStatus1 != FAILED)
        {
          printf("String writes in the current EEPROM selected:\n");
          printf("READ/WRITE PAGE %u  SUCCEEDED\n", i);
        }
        else
        {
          printf("READ/WRITE PAGE %u  FAILED\n", i);
        }
      }
    }
    else
    {
      printf("Problem to communicate again with EEPROM\n");
      printf("Press TAMPER button to end test\n");
    }

    /* Tx1Buffer to the EEPROM */
    if (BSP_EEPROM_WriteBuffer(0, (uint8_t*)Tx1Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE) != BSP_ERROR_NONE)
    {
      printf("Issue to write Tx1Buffer\n");
    }
    break;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

static void FillBuffer(uint8_t* pBuffer1, uint16_t Index, uint16_t BufferLength)
{
  uint32_t i;
  for(i = 0; i < BufferLength; i++)
  {
    pBuffer1[i] = Index + i;
  }
}

/**
  * @}
  */

/**
  * @}
  */
