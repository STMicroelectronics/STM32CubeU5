/**
  ******************************************************************************
  * @file    optionbytes_interface.c
  * @author  MCD Application Team
  * @brief   Contains Option Bytes access functions
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
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef OB_Descriptor =
{
  OB_START_ADDRESS,
  OB_END_ADDRESS,
  OB_SIZE,
  OB_AREA,
  OPENBL_OB_Read,
  OPENBL_OB_Write,
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
uint8_t OPENBL_OB_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in Option bytes.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  /* Unlock the FLASH & Option Bytes Registers access */
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  /* Write RDP Level */
  if (DataLength >= 1)
  {
    WRITE_REG(FLASH->OPTR, *(Data));
  }
  /* Write OPTR */
  if (DataLength >= 4)
  {
    WRITE_REG(FLASH->OPTR, (*(Data) | (*(Data + 1) << 8) | (*(Data + 2) << 16) | (*(Data + 3) << 24)));
  }
  /* Write NSBOOTADD0R */
  if (DataLength >= 8)
  {
    WRITE_REG(FLASH->NSBOOTADD0R, (*(Data + 4) | (*(Data + 5) << 8) | (*(Data + 6) << 16) | (*(Data + 7) << 24)));
  }
  /* Write NSBOOTADD1R */
  if (DataLength >= 12)
  {
    WRITE_REG(FLASH->NSBOOTADD1R, (*(Data + 8) | (*(Data + 9) << 8) | (*(Data + 10) << 16) | (*(Data + 11) << 24)));
  }
  /* Write WRP1AR */
  if (DataLength >= 28)
  {
    WRITE_REG(FLASH->WRP1AR, (*(Data + 24) | (*(Data + 25) << 8) | (*(Data + 26) << 16) | (*(Data + 27) << 24)));
  }
  /* Write WRP1BR */
  if (DataLength >= 32)
  {
    WRITE_REG(FLASH->WRP1BR, (*(Data + 28) | (*(Data + 29) << 8) | (*(Data + 30) << 16) | (*(Data + 31) << 24)));
  }
  /* Write WRP2AR */
  if (DataLength >= 44)
  {
    WRITE_REG(FLASH->WRP2AR, (*(Data + 40) | (*(Data + 41) << 8) | (*(Data + 42) << 16) | (*(Data + 43) << 24)));
  }
  /* Write WRP2BR */
  if (DataLength >= 48)
  {
    WRITE_REG(FLASH->WRP2BR, (*(Data + 44) | (*(Data + 45) << 8) | (*(Data + 46) << 16) | (*(Data + 47) << 24)));
  }

  /* OEMKEYR OB can be added by user if needed */

  SET_BIT(FLASH->NSCR, FLASH_NSCR_OPTSTRT);
}
