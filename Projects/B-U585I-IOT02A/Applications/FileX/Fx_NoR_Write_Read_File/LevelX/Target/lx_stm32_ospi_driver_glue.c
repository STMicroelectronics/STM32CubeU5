
/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include "lx_stm32_ospi_driver.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
* @brief Initializes the OctoSPI IP instance
* @param UINT Instance OctoSPI instance to initialize
* @retval 0 on success error value otherwise
*/
INT lx_stm32_ospi_lowlevel_init(UINT instance)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Init */
#if (LX_STM32_OSPI_INIT  != 0)
  BSP_OSPI_NOR_Init_t flash;

  /* OSPI device configuration */
  flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;

  if (BSP_OSPI_NOR_Init(instance, &flash) != BSP_ERROR_NONE)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
#endif
  /* USER CODE END OSPI_Init */

  return status;
}

/**
* @brief Get the status of the OctoSPI instance
* @param UINT Instance OctoSPI instance
* @retval 0 if the OctoSPI is ready 1 otherwise
*/
INT lx_stm32_ospi_get_status(UINT instance)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Get_Status */
  if(BSP_OSPI_NOR_GetStatus(instance) != BSP_ERROR_NONE)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  /* USER CODE END OSPI_Get_Status */

  return status;
}

/**
* @brief Get size info of the flash meomory
* @param UINT Instance OctoSPI instance
* @param UINT * block_size pointer to be filled with Flash block size
* @param UINT * total_blocks pointer to be filled with Flash total number of blocks
* @retval 0 on Success and block_size and total_blocks are correctly filled
1 on Failure, block_size = 0, total_blocks = 0
*/
INT lx_stm32_ospi_get_info(UINT instance, ULONG *block_size, ULONG *total_blocks)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Get_Info */
  BSP_OSPI_NOR_Info_t pInfo;

  if (BSP_OSPI_NOR_GetInfo(instance, &pInfo) != BSP_ERROR_NONE)
  {
    status = BSP_ERROR_COMPONENT_FAILURE;
  }

  *block_size = pInfo.EraseSectorSize;
  *total_blocks = pInfo.EraseSectorsNumber;
  /* USER CODE END OSPI_Get_Info */

  return status;
}

/**
* @brief Read data from the OctoSPI memory into a buffer
* @param UINT Instance OctoSPI instance
* @param ULONG * address the start address to read from
* @param ULONG * buffer the destination buffer
* @param ULONG words the total number of words to be read
* @retval 0 on Success
1 on Failure
*/

INT lx_stm32_ospi_read(UINT instance, ULONG *address, ULONG *buffer, ULONG words)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Read */
  if(BSP_OSPI_NOR_Read(instance, (uint8_t*) buffer, (uint32_t) address, (uint32_t) words * sizeof(ULONG)) != BSP_ERROR_NONE)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  /* USER CODE END OSPI_Read */

  return status;
}

/**
* @brief write a data buffer into the OctoSPI memory
* @param UINT Instance OctoSPI instance
* @param ULONG * address the start address to write into
* @param ULONG * buffer the data source buffer
* @param ULONG words the total number of words to be written
* @retval 0 on Success
1 on Failure
*/

INT lx_stm32_ospi_write(UINT instance, ULONG *address, ULONG *buffer, ULONG words)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Write */
  if(BSP_OSPI_NOR_Write(instance, (uint8_t*) buffer, (uint32_t) address, (uint32_t) words * sizeof(ULONG)) != BSP_ERROR_NONE)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  /* USER CODE END OSPI_Write */

  return status;
}

/**
* @brief Erase the whole flash or a single block
* @param UINT Instance OctoSPI instance
* @param ULONG  block the block to be erased
* @param ULONG  erase_count the number of times the block was erased
* @param UINT full_chip_erase if set to 0 a single block is erased otherwise the whole flash is
* @retval 0 on Success
1 on Failure
*/
INT lx_stm32_ospi_erase(UINT instance, ULONG block, ULONG erase_count, UINT full_chip_erase)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Erase_Block */
  UINT block_index = 0;
  BSP_OSPI_NOR_Info_t pInfo;

  if (full_chip_erase)
  {
    if (BSP_OSPI_NOR_GetInfo(instance, &pInfo) != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_COMPONENT_FAILURE;
    }
    for(block_index = 0; block_index < pInfo.EraseSectorsNumber; block_index++)
    {
      if(BSP_OSPI_NOR_Erase_Block(instance, block_index, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_PERIPH_FAILURE;
        return status;
      }
      while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
    }
  }
  else
  {
    if(BSP_OSPI_NOR_Erase_Block(instance, block, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
  }
  /* USER CODE END OSPI_Erase_Block */

  return status;
}

/**
* @brief Check that a block was actually erased
* @param UINT Instance OctoSPI instance
* @param ULONG  block the block to be checked
* @retval 0 on Success
1 on Failure
*/
INT lx_stm32_ospi_is_block_erased(UINT instance, ULONG block)
{
  INT status = 0;
  /* USER CODE BEGIN OSPI_Block_Erased */

  /* USER CODE END OSPI_Block_Erased */

  return status;
}

UINT  lx_ospi_driver_system_error(UINT error_code)
{
  UINT status = LX_ERROR;
  /* USER CODE BEGIN OSPI_Block_Erased */

  /* USER CODE END OSPI_Block_Erased */

  return status;
}
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
