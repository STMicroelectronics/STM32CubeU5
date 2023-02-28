/**
  ******************************************************************************
  * @file    optionbytes_interface.c
  * @author  MCD Application Team
  * @brief   Contains Option Bytes access functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "platform.h"
#include "common_interface.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
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
  * @brief  Launch the option byte loading.
  * @retval None.
  */
void OPENBL_OB_Launch(void)
{
  /* Set the option start bit */
  (void)HAL_FLASH_OB_Launch();
}

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
  * @param  pData The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(uint32_t Address, uint8_t *pData, uint32_t DataLength)
{
  /* Unlock the FLASH & Option Bytes Registers access */
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_FLASH_OB_Unlock() != HAL_OK)
  {
    Error_Handler();
  }

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  /* Write RDP Level */
  if (DataLength >= 1U)
  {
    WRITE_REG(FLASH->OPTR, *(pData));
  }

  /* Write OPTR */
  if (DataLength >= 4U)
  {
    WRITE_REG(FLASH->OPTR, ((uint32_t)pData[0]
                            | ((uint32_t)pData[1] << 8U)
                            | ((uint32_t)pData[2] << 16U)
                            | ((uint32_t)pData[3] << 24U)));
  }

  /* Write NSBOOTADD0R */
  if (DataLength >= 8U)
  {
    WRITE_REG(FLASH->NSBOOTADD0R, ((uint32_t)pData[4]
                                   | ((uint32_t)pData[5] << 8U)
                                   | ((uint32_t)pData[6] << 16U)
                                   | ((uint32_t)pData[7] << 24U)));
  }

  /* Write NSBOOTADD1R */
  if (DataLength >= 12U)
  {
    WRITE_REG(FLASH->NSBOOTADD1R, ((uint32_t)pData[8]
                                   | ((uint32_t)pData[9] << 8U)
                                   | ((uint32_t)pData[10U] << 16U)
                                   | ((uint32_t)pData[11] << 24U)));
  }

  /* Write WRP1AR */
  if (DataLength >= 28U)
  {
    WRITE_REG(FLASH->WRP1AR, ((uint32_t)pData[24]
                              | ((uint32_t)pData[25] << 8U)
                              | ((uint32_t)pData[26] << 16U)
                              | ((uint32_t)pData[27] << 24U)));
  }

  /* Write WRP1BR */
  if (DataLength >= 32U)
  {
    WRITE_REG(FLASH->WRP1BR, ((uint32_t)pData[28]
                              | ((uint32_t)pData[29] << 8U)
                              | ((uint32_t)pData[30] << 16U)
                              | ((uint32_t)pData[31] << 24U)));
  }

  /* Write WRP2AR */
  if (DataLength >= 44U)
  {
    WRITE_REG(FLASH->WRP2AR, ((uint32_t)pData[40]
                              | ((uint32_t)pData[41] << 8U)
                              | ((uint32_t)pData[42] << 16U)
                              | ((uint32_t)pData[43] << 24U)));
  }

  /* Write WRP2BR */
  if (DataLength >= 48U)
  {
    WRITE_REG(FLASH->WRP2BR, ((uint32_t)pData[44]
                              | ((uint32_t)pData[45] << 8U)
                              | ((uint32_t)pData[46] << 16U)
                              | ((uint32_t)pData[47] << 24U)));
  }

  /* OEMKEYR OB can be added by user if needed */

  SET_BIT(FLASH->NSCR, FLASH_NSCR_OPTSTRT);

  /* Register system reset callback */
  Common_SetPostProcessingCallback(OPENBL_OB_Launch);
}
