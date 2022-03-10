/**
  ******************************************************************************
  * @file    otp_interface.c
  * @author  MCD Application Team
  * @brief   Contains One-time programmable access functions
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
#include "platform.h"
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "otp_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef OTP_Descriptor =
{
  OTP_START_ADDRESS,
  OTP_END_ADDRESS,
  OTP_BL_SIZE,
  OTP_AREA,
  OPENBL_OTP_Read,
  OPENBL_OTP_Write,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OTP_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in OTP.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_OTP_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  uint32_t index;
  uint32_t length = DataLength;
  uint32_t remainder;
  uint8_t remainder_data[16] = {0x0};

  /* Check the remaining of quad-word */
  remainder = length & 0xFU;

  if (remainder)
  {
    length = (length & 0xFFFFFFF0U);

    /* copy the remaining bytes */
    for (index = 0U; index < remainder; index++)
    {
      remainder_data[index] = *(Data + length + index);
    }

    /* fill the upper bytes with 0xFF */
    for (index = remainder; index < 16U; index++)
    {
      remainder_data[index] = 0xFF;
    }
  }

  /* Unlock the flash memory for write operation */
  HAL_FLASH_Unlock();

  for (index = 0U; index < length; (index += 16U))
  {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, (Address + index), (uint32_t)((Data + index)));
  }

  if (remainder)
  {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, (Address + length), (uint32_t)((remainder_data)));
  }

  /* Lock the Flash to disable the flash control register access */
  HAL_FLASH_Lock();
}
