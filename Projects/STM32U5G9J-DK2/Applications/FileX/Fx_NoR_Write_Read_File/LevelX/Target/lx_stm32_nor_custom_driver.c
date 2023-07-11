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

#include "lx_stm32_nor_custom_driver.h"

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static UINT is_initialized = LX_FALSE;

/* The following semaphore is being to notify about RX/TX completion.
It needs to be released in the transfer callbacks */
static TX_SEMAPHORE xspi_rx_semaphore;
static TX_SEMAPHORE xspi_tx_semaphore;

extern XSPI_HandleTypeDef hxspi1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
#if (LX_STM32_XSPI_INIT == 1)
extern void MX_HSPI1_Init(void);
#endif

static uint8_t xspi_memory_reset            (XSPI_HandleTypeDef *hxspi);
static uint8_t xspi_set_write_enable        (XSPI_HandleTypeDef *hxspi);
static uint8_t xspi_auto_polling_ready      (XSPI_HandleTypeDef *hxspi, uint32_t timeout);
static uint8_t xspi_set_octal_mode          (XSPI_HandleTypeDef *hxspi);
/* USER CODE END PFP */

static UINT  lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words);
static UINT  lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words);

static UINT  lx_nor_driver_block_erase(ULONG block, ULONG erase_count);
static UINT  lx_nor_driver_block_erased_verify(ULONG block);

/* USER CODE BEGIN USER_CODE_SECTION_1 */

/**
* @brief check the status of the xspi memory.
* @param none
* @retval LX_SUCCESS if the xspi is ready, LX_ERROR otherwise
*/

static UINT check_status(void)
{
  ULONG start = LX_STM32_XSPI_CURRENT_TIME();
  while (LX_STM32_XSPI_CURRENT_TIME() - start < LX_STM32_XSPI_DEFAULT_TIMEOUT)
  {
    if (lx_stm32_xspi_get_status(LX_STM32_XSPI_INSTANCE) == 0)
    {
      return LX_SUCCESS;
    }
  }

  return LX_ERROR;
}

/* USER CODE END USER_CODE_SECTION_1 */

#ifndef LX_DIRECT_READ

#ifndef NOR_SECTOR_BUFFER_SIZE
#define NOR_SECTOR_BUFFER_SIZE 512
#endif

static ULONG nor_sector_memory[NOR_SECTOR_BUFFER_SIZE];
#endif

UINT  lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;

  ULONG total_blocks = 0;
  ULONG words_per_block = 0;

  /* USER CODE BEGIN Init_Section_0 */
  ULONG block_size;
  /* USER CODE END Init_Section_0 */

  nor_flash->lx_nor_flash_total_blocks    = total_blocks;
  nor_flash->lx_nor_flash_words_per_block = words_per_block;

  /* USER CODE BEGIN Init_Section_1 */
  if (is_initialized == LX_FALSE)
  {

    ret = lx_stm32_xspi_lowlevel_init(LX_STM32_XSPI_INSTANCE);

    if (ret != 0)
    {
      return LX_ERROR;
    }

#if (LX_STM32_XSPI_ERASE == 1)

    ret = lx_stm32_xspi_erase(LX_STM32_XSPI_INSTANCE, (ULONG)0, (ULONG)0, 1);

    if (ret != 0)
    {
      return LX_ERROR;
    }
#endif

    if (check_status() != LX_SUCCESS)
    {
      return LX_ERROR;
    }

    ret = lx_stm32_xspi_get_info(LX_STM32_XSPI_INSTANCE, &block_size, &total_blocks);

    if (ret != 0)
    {
      return LX_ERROR;
    }
  }
  /* Setup the base address of the flash memory.  */
  nor_flash->lx_nor_flash_base_address = 0;
  /* Setup geometry of the flash.  */
  nor_flash->lx_nor_flash_total_blocks = total_blocks;
  nor_flash->lx_nor_flash_words_per_block = block_size / sizeof(ULONG);

  nor_flash->lx_nor_flash_driver_system_error = lx_xspi_driver_system_error;
  /* USER CODE END Init_Section_1 */

  nor_flash->lx_nor_flash_driver_read = lx_nor_driver_read;
  nor_flash->lx_nor_flash_driver_write = lx_nor_driver_write;

  nor_flash->lx_nor_flash_driver_block_erase = lx_nor_driver_block_erase;
  nor_flash->lx_nor_flash_driver_block_erased_verify = lx_nor_driver_block_erased_verify;

#ifndef LX_DIRECT_READ
    nor_flash->lx_nor_flash_sector_buffer = nor_sector_memory;
#endif

  /* USER CODE BEGIN Init_Section_2 */
  is_initialized = LX_TRUE;
  /* call post init routine*/
  LX_STM32_XSPI_POST_INIT();
  /* USER CODE END Init_Section_2 */

    return ret;
}

/* USER CODE BEGIN USER_CODE_SECTION_2 */

/* USER CODE END USER_CODE_SECTION_2 */

static UINT lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_READ */
    if (check_status() != LX_SUCCESS)
    {
    return LX_ERROR;
    }

    LX_STM32_XSPI_PRE_READ_TRANSFER(status);

    if (ret != LX_SUCCESS)
    {
        return ret;
    }

    if (lx_stm32_xspi_read(LX_STM32_XSPI_INSTANCE, flash_address, destination, words) != 0)
    {
        ret = LX_ERROR;
        LX_STM32_XSPI_READ_TRANSFER_ERROR(ret);
    }
    else
    {
        LX_STM32_XSPI_READ_CPLT_NOTIFY(ret);
    }

    LX_STM32_XSPI_POST_READ_TRANSFER(ret);
    /* USER CODE END  NOR_READ */

    return ret;
}

static UINT lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_WRITE */
    if (check_status() != LX_SUCCESS)
    {
        return LX_ERROR;
    }

    LX_STM32_XSPI_PRE_WRITE_TRANSFER(ret);

    if (ret != LX_SUCCESS)
    {
        return ret;
    }

    if (lx_stm32_xspi_write(LX_STM32_XSPI_INSTANCE, flash_address, source, words) != 0)
    {
        ret = LX_ERROR;
        LX_STM32_XSPI_WRITE_TRANSFER_ERROR(ret);
    }
    else
    {
        LX_STM32_XSPI_WRITE_CPLT_NOTIFY(ret);
    }

    LX_STM32_XSPI_POST_WRITE_TRANSFER(ret);

    /* USER CODE END  NOR_DRIVER_WRITE */

    return ret;
}

static UINT lx_nor_driver_block_erase(ULONG block, ULONG erase_count)
{

    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_BLOCK */
    if (check_status() != LX_SUCCESS)
    {
        return LX_ERROR;
    }

    if (lx_stm32_xspi_erase(LX_STM32_XSPI_INSTANCE, block, erase_count, 0) != 0)
    {
        ret = LX_ERROR;
    }
    else
    {
        ret = check_status();
    }
    /* USER CODE END  NOR_DRIVER_BLOCK */

    return ret;
}

static UINT lx_nor_driver_block_erased_verify(ULONG block)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_VERIFY */
    if (check_status() != LX_SUCCESS)
    {
        return LX_ERROR;
    }

    if (lx_stm32_xspi_is_block_erased(LX_STM32_XSPI_INSTANCE, block) == 0)
    {
        ret = LX_SUCCESS;
    }
    else
    {
        ret = LX_ERROR;
    }
    /* USER CODE END  NOR_DRIVER_VERIFY */

    return ret;
}

/* USER CODE BEGIN USER_CODE_SECTION_3 */

/* HAL DMA API implementation for HSPI component MX66UW1G45G
 * The present implementation assumes the following settings are set:

  Instance              = HSPI1
  FifoThreshold         = 1
  DualQuad              = disabled
  MemoryType            = Macronix
  DeviceSize            = 27
  ChipSelectHighTime    = 2
  FreeRunningClock      = disabled
  ClockMode             = low
  ClockPrescaler        = 2
  SampleShifting        = none
  DelayHoldQuarterCycle = enabled
  ChipSelectBoundary    = 0
  DelayBlockBypass      = used
 */

/**
* @brief system init for xspi levelx driver
* @param UINT instance XSPI instance to initialize
* @retval 0 on success error value otherwise
*/
INT lx_stm32_xspi_lowlevel_init(UINT instance)
{
  INT status = 0;

  UNUSED(instance);

  /* Call the DeInit function to reset the driver */
#if (LX_STM32_XSPI_INIT == 1)
  hxspi1.Instance = HSPI1;
  if (HAL_XSPI_DeInit(&hxspi1) != HAL_OK)
  {
    return 1;
  }

  /* Init the HSPI */
  MX_HSPI1_Init();
#endif

  /* XSPI memory reset */
  if (xspi_memory_reset(&hxspi1) != 0)
  {
    return 1;
  }

  /* Enable octal mode */
  if (xspi_set_octal_mode(&hxspi1) != 0)
  {
    return 1;
  }

  return status;
}

/**
* @brief deinit xspi levelx driver, could be called by the fx_media_close()
* @param UINT instance XSPI instance to deinitialize
* @retval 0 on success error value otherwise
*/
INT lx_stm32_xspi_lowlevel_deinit(UINT instance)
{
  INT status = 0;

  UNUSED(instance);

  /* Delete semaphore objects */
  tx_semaphore_delete(&xspi_tx_semaphore);
  tx_semaphore_delete(&xspi_rx_semaphore);

  /* Call the DeInit function to reset the driver */
  if (HAL_XSPI_DeInit(&hxspi1) != HAL_OK)
  {
    return 1;
  }

  return status;
}

/**
* @brief Get the status of the XSPI instance
* @param UINT instance XSPI instance
* @retval 0 if the XSPI is ready 1 otherwise
*/
INT lx_stm32_xspi_get_status(UINT instance)
{
  INT status = 0;

  UNUSED(instance);

  XSPI_RegularCmdTypeDef s_command;
  uint8_t reg[2];

  /* Initialize the read status register command */

  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;
  s_command.Instruction           = LX_STM32_XSPI_OCTAL_READ_STATUS_REG_CMD;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Address               = 0;
  s_command.AddressMode           = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressWidth          = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_8_LINES;
  s_command.DataLength            = 2;
  s_command.DummyCycles           = LX_STM32_XSPI_DUMMY_CYCLES_READ_OCTAL;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.DataDTRMode           = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DQSMode               = HAL_XSPI_DQS_ENABLE;

  /* Configure the command */
  if (HAL_XSPI_Command(&hxspi1, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(&hxspi1, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Check the value of the register */
  if ((reg[0] & LX_STM32_XSPI_SR_WIP) != 0)
  {
    return 1;
  }

  return status;
}

/**
* @brief Get size info of the flash memory
* @param UINT instance XSPI instance
* @param ULONG * block_size pointer to be filled with Flash block size
* @param ULONG * total_blocks pointer to be filled with Flash total number of blocks
* @retval 0 on Success and block_size and total_blocks are correctly filled
          1 on Failure, block_size = 0, total_blocks = 0
*/
INT lx_stm32_xspi_get_info(UINT instance, ULONG *block_size, ULONG *total_blocks)
{
  INT status = 0;

  UNUSED(instance);

  *block_size = LX_STM32_XSPI_SECTOR_SIZE;

  *total_blocks = (LX_STM32_XSPI_FLASH_SIZE / LX_STM32_XSPI_SECTOR_SIZE);

  return status;
}

/**
* @brief Read data from the XSPI memory into a buffer
* @param UINT instance XSPI instance
* @param ULONG * address the start address to read from
* @param ULONG * buffer the destination buffer
* @param ULONG words the total number of words to be read
* @retval 0 on Success 1 on Failure
*/
INT lx_stm32_xspi_read(UINT instance, ULONG *address, ULONG *buffer, ULONG words)
{
  INT status = 0;

  UNUSED(instance);

  XSPI_RegularCmdTypeDef s_command;

  /* Initialize the read command */

  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth   = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Address            = (uint32_t)address;
  s_command.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressWidth       = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_8_LINES;
  s_command.DataLength         = (uint32_t) words * sizeof(ULONG);
  s_command.DummyCycles        = DUMMY_CYCLES_READ_OCTAL_DTR;
  s_command.SIOOMode           = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.Instruction           = LX_STM32_XSPI_OCTAL_READ_DTR_CMD;
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.DataDTRMode           = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DQSMode               = HAL_XSPI_DQS_ENABLE;

  /* Configure the command */
  if (HAL_XSPI_Command(&hxspi1, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive_DMA(&hxspi1, (uint8_t*)buffer) != HAL_OK)
  {
    return 1;
  }

  return status;
}

/**
* @brief write a data buffer into the XSPI memory
* @param UINT instance XSPI instance
* @param ULONG * address the start address to write into
* @param ULONG * buffer the data source buffer
* @param ULONG words the total number of words to be written
* @retval 0 on Success 1 on Failure
*/
INT lx_stm32_xspi_write(UINT instance, ULONG *address, ULONG *buffer, ULONG words)
{
  INT status = 0;

  XSPI_RegularCmdTypeDef s_command;

  uint32_t end_addr;
  uint32_t current_addr;

  uint32_t current_size;
  uint32_t data_buffer;

  UNUSED(instance);
  /* Calculation of the size between the write address and the end of the page */
  current_size = LX_STM32_XSPI_PAGE_SIZE - ((uint32_t)address % LX_STM32_XSPI_PAGE_SIZE);

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > (((uint32_t) words) * sizeof(ULONG)))
  {
    current_size = ((uint32_t) words) * sizeof(ULONG);
  }

  /* Initialize the address variables */
  current_addr = (uint32_t) address;
  end_addr = ((uint32_t) address) + ((uint32_t) words) * sizeof(ULONG);
  data_buffer= (uint32_t)buffer;

  /* Initialize the program command */

  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;
  s_command.Instruction           = LX_STM32_XSPI_OCTAL_PAGE_PROG_CMD;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AddressMode           = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressWidth          = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_8_LINES;
  s_command.DummyCycles           = 0;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.DataDTRMode           = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DQSMode               = HAL_XSPI_DQS_ENABLE;

  /* Perform the write page by page */
  do
  {
    s_command.Address = current_addr;
    s_command.DataLength  = current_size;

    /* Enable write operations */
    if (xspi_set_write_enable(&hxspi1) != 0)
    {
      return 1;
    }

    /* Configure the command */
    if (HAL_XSPI_Command(&hxspi1, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return 1;
    }

    /* Transmission of the data */
    if (HAL_XSPI_Transmit_DMA(&hxspi1, (uint8_t*)data_buffer) != HAL_OK)
    {
      return 1;
    }

    /* Check success of the transmission of the data */
    if(tx_semaphore_get(&xspi_tx_semaphore, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != TX_SUCCESS)
    {
      return 1;
    }

    /* Configure automatic polling mode to wait for end of program */
    if (xspi_auto_polling_ready(&hxspi1, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != 0)
    {
      return 1;
    }

    /* Update the address and data variables for next page programming */
    current_addr += current_size;
    data_buffer += current_size;

    current_size = ((current_addr + LX_STM32_XSPI_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : LX_STM32_XSPI_PAGE_SIZE;
  } while (current_addr < end_addr);

   /* Release xspi_transfer_semaphore in case of writing success */
    tx_semaphore_put(&xspi_tx_semaphore);

  return status;
}

/**
* @brief Erase the whole flash or a single block
* @param UINT instance XSPI instance
* @param ULONG  block the block to be erased
* @param ULONG  erase_count the number of times the block was erased
* @param UINT full_chip_erase if set to 0 a single block is erased otherwise the whole flash is erased
* @retval 0 on Success 1 on Failure
*/
INT lx_stm32_xspi_erase(UINT instance, ULONG block, ULONG erase_count, UINT full_chip_erase)
{
  INT status = 0;

  XSPI_RegularCmdTypeDef s_command;

  UNUSED(instance);
  UNUSED(erase_count);
  /* Initialize the erase command */

  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles           = 0;
  s_command.DQSMode               = HAL_XSPI_DQS_DISABLE;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;

  if(full_chip_erase)
  {
    s_command.Instruction         = LX_STM32_XSPI_OCTAL_BULK_ERASE_CMD;
    s_command.AddressMode         = HAL_XSPI_ADDRESS_NONE;
  }
  else
  {
    s_command.Instruction         = LX_STM32_XSPI_OCTAL_SECTOR_ERASE_CMD;
    s_command.Address             = (block * LX_STM32_XSPI_SECTOR_SIZE);
    s_command.AddressMode         = HAL_XSPI_ADDRESS_8_LINES;
    s_command.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
    s_command.AddressDTRMode      = HAL_XSPI_ADDRESS_DTR_ENABLE;
  }

  /* Enable write operations */
  if (xspi_set_write_enable(&hxspi1) != 0)
  {
    return 1;
  }

  /* Send the command */
  if (HAL_XSPI_Command(&hxspi1, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Configure automatic polling mode to wait for end of erase */
  if (xspi_auto_polling_ready(&hxspi1, LX_STM32_XSPI_BULK_ERASE_MAX_TIME) != 0)
  {
    return 1;
  }

  return status;
}

/**
* @brief Check that a block was actually erased
* @param UINT instance XSPI instance
* @param ULONG  block the block to be checked
* @retval 0 on Success 1 on Failure
*/
INT lx_stm32_xspi_is_block_erased(UINT instance, ULONG block)
{
  INT status = 0;
  UNUSED(instance);
  UNUSED(block);
  return status;
}

/**
* @brief Handle levelx system errors
* @param UINT error_code Code of the concerned error.
* @retval UINT error code.
*/

UINT  lx_xspi_driver_system_error(UINT error_code)
{
  UINT status = LX_ERROR;
  UNUSED(error_code);
  return status;
}

/**
  * @brief  Reset the XSPI memory.
  * @param  hxspi: XSPI handle pointer
  * @retval O on success 1 on Failure.
  */
static uint8_t xspi_memory_reset(XSPI_HandleTypeDef *hxspi)
{
  uint8_t status = 0;

  XSPI_RegularCmdTypeDef s_command;
  XSPI_AutoPollingTypeDef s_config;

  /* Initialize the reset enable command */
  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;
  s_command.Instruction           = LX_STM32_XSPI_RESET_ENABLE_CMD;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles           = 0;
  s_command.DQSMode               = HAL_XSPI_DQS_DISABLE;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Send the reset memory command */
  s_command.Instruction = LX_STM32_XSPI_RESET_MEMORY_CMD;
  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  s_command.Instruction  = LX_STM32_XSPI_READ_STATUS_REG_CMD;
  s_command.DataMode     = HAL_XSPI_DATA_1_LINE;
  s_command.DataLength   = 1;
  s_command.DataDTRMode  = HAL_XSPI_DATA_DTR_DISABLE;

  s_config.MatchValue    = 0;
  s_config.MatchMask     = LX_STM32_XSPI_SR_WIP;
  s_config.MatchMode     = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime  = 0x10;
  s_config.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_AutoPolling(hxspi, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  return status;
}

/**
  * @brief  Send a Write Enable command and wait its effective.
  * @param  hxspi: XSPI handle pointer
  * @retval O on success 1 on Failure.
  */
static uint8_t xspi_set_write_enable(XSPI_HandleTypeDef *hxspi)
{
  uint8_t status = 0;

  XSPI_RegularCmdTypeDef  s_command;

  /* Enable write operations */
  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;
  s_command.Instruction           = LX_STM32_XSPI_OCTAL_WRITE_ENABLE_CMD;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles           = 0U;
  s_command.DQSMode               = HAL_XSPI_DQS_DISABLE;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (xspi_auto_polling_ready(hxspi, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != 0)
  {
    return 1;
  }

  return status;
}

/**
  * @brief  Read the SR of the memory and wait the EOP.
  * @param  hxspi: XSPI handle pointer
  * @param  timeout: timeout value before returning an error
  * @retval O on success 1 on Failure.
  */
static uint8_t xspi_auto_polling_ready(XSPI_HandleTypeDef *hxspi, uint32_t timeout)
{
  uint8_t status = 0;

  XSPI_RegularCmdTypeDef  s_command;
  XSPI_AutoPollingTypeDef s_config;
  uint8_t reg[2];
  uint32_t start = LX_STM32_XSPI_CURRENT_TIME();

  /* Configure automatic polling mode to wait for memory ready */
  s_command.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect              = HAL_XSPI_SELECT_IO_7_0;
  s_command.Instruction           = LX_STM32_XSPI_OCTAL_READ_STATUS_REG_CMD;
  s_command.InstructionMode       = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth      = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Address               = 0U;
  s_command.AddressMode           = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressWidth          = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode              = HAL_XSPI_DATA_8_LINES;
  s_command.DataLength            = 2U;
  s_command.DummyCycles           = LX_STM32_XSPI_DUMMY_CYCLES_READ_OCTAL;
  s_command.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;

  /* DTR mode is enabled */
  s_command.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.DataDTRMode           = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DQSMode               = HAL_XSPI_DQS_ENABLE;

  s_config.MatchValue           = 0U;
  s_config.MatchMask            = LX_STM32_XSPI_SR_WIP;

  while( LX_STM32_XSPI_CURRENT_TIME() - start < timeout)
  {
     if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      status = 1;
      break;
    }

    if (HAL_XSPI_Receive(hxspi, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      status = 1;
      break;
    }
    /* the Memory is ready, break from the loop */
    if ((reg[0] & s_config.MatchMask ) == s_config.MatchValue)
    {
      break;
    }
  }

  return status;
}

/**
  * @brief  This function enables the octal mode of the memory.
  * @param  hxspi: XSPI handle
  * @retval 0 on success 1 on Failure.
  */
static uint8_t xspi_set_octal_mode(XSPI_HandleTypeDef *hxspi)
{
  int status = 0;

  XSPI_RegularCmdTypeDef  s_command;
  XSPI_AutoPollingTypeDef s_config;
  uint8_t reg[2];

  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.AddressWidth       = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_XSPI_SIOO_INST_EVERY_CMD;

  s_config.MatchMode     = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime  = 0x10U;
  s_config.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  /* Activate the Octal mode */

  s_command.Instruction      = LX_STM32_XSPI_WRITE_ENABLE_CMD;
  s_command.InstructionMode  = HAL_XSPI_INSTRUCTION_1_LINE;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
  s_command.AddressMode      = HAL_XSPI_ADDRESS_NONE;
  s_command.DataMode         = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles      = 0U;
  /* Add a short delay to let the IP settle before starting the command */
  HAL_Delay(1);

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.MatchValue = LX_STM32_XSPI_SR_WEL;
  s_config.MatchMask  = LX_STM32_XSPI_SR_WEL;

  s_command.Instruction = LX_STM32_XSPI_READ_STATUS_REG_CMD;
  s_command.DataMode    = HAL_XSPI_DATA_1_LINE;
  s_command.DataLength  = 1;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_AutoPolling(hxspi, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Write Configuration register 2 (with new dummy cycles) */

  s_command.Instruction = LX_STM32_XSPI_WRITE_CFG_REG2_CMD;
  s_command.Address     = LX_STM32_XSPI_CR2_REG3_ADDR;
  s_command.AddressMode = HAL_XSPI_ADDRESS_1_LINE;

  reg[0] = LX_STM32_XSPI_DUMMY_CYCLES_CR_CFG;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_Transmit(hxspi, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Enable write operations */

  s_command.Instruction = LX_STM32_XSPI_WRITE_ENABLE_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.DataMode    = HAL_XSPI_DATA_NONE;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Configure automatic polling mode to wait for write enabling */

  s_command.Instruction = LX_STM32_XSPI_READ_STATUS_REG_CMD;
  s_command.DataMode    = HAL_XSPI_DATA_1_LINE;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_AutoPolling(hxspi, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* Write Configuration register 2 (with Octal I/O SPI protocol) */

  s_command.Instruction = LX_STM32_XSPI_WRITE_CFG_REG2_CMD;
  s_command.Address     = LX_STM32_XSPI_CR2_REG1_ADDR;
  s_command.AddressMode = HAL_XSPI_ADDRESS_1_LINE;

  /* DTR mode is enabled */

  reg[0] = LX_STM32_XSPI_CR2_DOPI;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_Transmit(hxspi, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (xspi_auto_polling_ready(hxspi, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != 0)
  {
    return 1;
  }

  /* Check the configuration has been correctly done */
  s_command.Instruction      = LX_STM32_XSPI_OCTAL_READ_CFG_REG2_CMD;
  s_command.InstructionMode  = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AddressMode      = HAL_XSPI_ADDRESS_8_LINES;
  s_command.DataMode         = HAL_XSPI_DATA_8_LINES;
  s_command.DummyCycles      = LX_STM32_XSPI_DUMMY_CYCLES_READ_OCTAL;
  s_command.DataLength       = 2U;
  reg[0] = 0;

  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DQSMode            = HAL_XSPI_DQS_ENABLE;

  if (HAL_XSPI_Command(hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  if (HAL_XSPI_Receive(hxspi, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return 1;
  }

  /* DTR mode is enabled */
  if (reg[0] != LX_STM32_XSPI_CR2_DOPI)
  {
    return 1;
  }

  return status;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  hxspi XSPI handle
  * @retval None
  */
void HAL_XSPI_RxCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  UNUSED(hxspi);
  tx_semaphore_put(&xspi_rx_semaphore);
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hxspi XSPI handle
  * @retval None
  */
void HAL_XSPI_TxCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  UNUSED(hxspi);
  tx_semaphore_put(&xspi_tx_semaphore);
}

/* USER CODE END USER_CODE_SECTION_3 */
