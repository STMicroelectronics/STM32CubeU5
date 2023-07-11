/**
  ******************************************************************************
  * @file    mx66uw1g45g.c
  * @modify  MCD Application Team
  * @brief   This file provides the MX66UW1G45G XSPI drivers.
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
    * MX66UW1G45G action :
  *   STR Octal IO protocol (SOPI) and DTR Octal IO protocol (DOPI) bits of
  *   Configuration Register 2 :
  *     DOPI = 1 and SOPI = 0: Operates in DTR Octal IO protocol (accepts 8-8-8 commands)
  *     DOPI = 0 and SOPI = 1: Operates in STR Octal IO protocol (accepts 8-8-8 commands)
  *     DOPI = 0 and SOPI = 0: Operates in Single IO protocol (accepts 1-1-1 commands)
  *   Enter SOPI mode by configuring DOPI = 0 and SOPI = 1 in CR2-Addr0
  *   Exit SOPI mode by configuring DOPI = 0 and SOPI = 0 in CR2-Addr0
  *   Enter DOPI mode by configuring DOPI = 1 and SOPI = 0 in CR2-Addr0
  *   Exit DOPI mode by configuring DOPI = 0 and SOPI = 0 in CR2-Addr0
  *
  *   Memory commands support STR(Single Transfer Rate) &
  *   DTR(Double Transfer Rate) modes in OPI
  *
  *   Memory commands support STR(Single Transfer Rate) &
  *   DTR(Double Transfer Rate) modes in SPI
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx66uw1g45g.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup MX66UW1G45G MX66UW1G45G
  * @{
  */

/** @defgroup MX66UW1G45G_Exported_Functions MX66UW1G45G Exported Functions
  * @{
  */

/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval error status
  */
int32_t MX66UW1G45G_GetFlashInfo(MX66UW1G45G_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize = MX66UW1G45G_FLASH_SIZE;
  pInfo->EraseSectorSize = MX66UW1G45G_BLOCK_64K;
  pInfo->EraseSectorsNumber = (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_64K);
  pInfo->EraseSubSectorSize = MX66UW1G45G_BLOCK_4K;
  pInfo->EraseSubSectorNumber = (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K);
  pInfo->EraseSubSector1Size = MX66UW1G45G_BLOCK_4K;
  pInfo->EraseSubSector1Number = (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_BLOCK_4K);
  pInfo->ProgPageSize = MX66UW1G45G_PAGE_SIZE;
  pInfo->ProgPagesNumber = (MX66UW1G45G_FLASH_SIZE / MX66UW1G45G_PAGE_SIZE);

  return MX66UW1G45G_OK;
};

/**
  * @brief  Polling WIP(Write In Progress) bit become to 0
  *         SPI/OPI;
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate
  * @retval error status
  */
int32_t MX66UW1G45G_AutoPollingMemReady(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                        MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Configure automatic polling mode to wait for memory ready */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_STATUS_REG_CMD
                              : MX66UW1G45G_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  s_config.MatchValue = 0U;
  s_config.MatchMask = MX66UW1G45G_SR_WIP;
  s_config.MatchMode = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime = MX66UW1G45G_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  if (HAL_XSPI_AutoPolling(Ctx, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/* Read/Write Array Commands (3/4 Byte Address Command Set) *********************/
/**
  * @brief  Reads an amount of data from the XSPI memory on STR mode.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressWidth Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_ReadSTR(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                            MX66UW1G45G_AddressWidth_t AddressWidth, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == MX66UW1G45G_OPI_MODE) && (AddressWidth == MX66UW1G45G_3BYTES_SIZE))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the read command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? ((AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                     ? MX66UW1G45G_FAST_READ_CMD
                                     : MX66UW1G45G_4_BYTE_ADDR_FAST_READ_CMD)
                              : MX66UW1G45G_OCTA_READ_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE)
                              ? HAL_XSPI_ADDRESS_1_LINE
                              : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                               ? HAL_XSPI_ADDRESS_24_BITS
                               : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = ReadAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_OCTAL;
  s_command.DataLength = Size;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Reads an amount of data from the XSPI memory on DTR mode.
  *         OPI
  * @param  Ctx Component object pointer
  * @param  AddressWidth Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @note   Only OPI mode support DTR transfer rate
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_ReadDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the read command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = MX66UW1G45G_OCTA_READ_DTR_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = ReadAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles = DUMMY_CYCLES_READ_OCTAL_DTR;
  s_command.DataLength = Size;
  s_command.DQSMode = HAL_XSPI_DQS_ENABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Writes an amount of data to the XSPI memory.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressWidth Address size
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ MX66UW1G45G_PAGE_SIZE
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_PageProgram(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                MX66UW1G45G_AddressWidth_t AddressWidth, uint8_t *pData, uint32_t WriteAddr,
                                uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == MX66UW1G45G_OPI_MODE) && (AddressWidth == MX66UW1G45G_3BYTES_SIZE))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? ((AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                     ? MX66UW1G45G_PAGE_PROG_CMD
                                     : MX66UW1G45G_4_BYTE_PAGE_PROG_CMD)
                              : MX66UW1G45G_OCTA_PAGE_PROG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE)
                              ? HAL_XSPI_ADDRESS_1_LINE
                              : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                               ? HAL_XSPI_ADDRESS_24_BITS
                               : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = Size;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Transmission of the data */
  if (HAL_XSPI_Transmit(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Writes an amount of data to the XSPI memory on DTR mode.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ MX66UW1G45G_PAGE_SIZE
  * @note   Only OPI mode support DTR transfer rate
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_PageProgramDTR(XSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the program command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = MX66UW1G45G_OCTA_PAGE_PROG_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = Size;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Transmission of the data */
  if (HAL_XSPI_Transmit(Ctx, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Erases the specified block of the XSPI memory.
  *         MX66UW1G45G support 4K, 64K size block erase commands.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressWidth Address size
  * @param  BlockAddress Block address to erase
  * @param  BlockSize Block size to erase
  * @retval XSPI memory status
  */

int32_t MX66UW1G45G_BlockErase(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate,
                               MX66UW1G45G_AddressWidth_t AddressWidth, uint32_t BlockAddress,
                               MX66UW1G45G_Erase_t BlockSize)
{

  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the erase command */
  s_command.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect           = HAL_XSPI_SELECT_IO_7_0;
  s_command.InstructionMode    = (Mode == MX66UW1G45G_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_1_LINE
                                 : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                 : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth   = (Mode == MX66UW1G45G_SPI_MODE)
                                 ? HAL_XSPI_INSTRUCTION_8_BITS
                                 : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.AddressMode        = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode     = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth       = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                 ? HAL_XSPI_ADDRESS_24_BITS
                                 : HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address            = BlockAddress;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode           = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  switch (Mode)
  {
    case MX66UW1G45G_OPI_MODE :
      if (BlockSize == MX66UW1G45G_ERASE_64K)
      {
        s_command.Instruction = MX66UW1G45G_OCTA_BLOCK_ERASE_64K_CMD;
      }
      else
      {
        s_command.Instruction = MX66UW1G45G_OCTA_SECTOR_ERASE_4K_CMD;
      }
      break;

    case MX66UW1G45G_SPI_MODE :
    default:
      if (BlockSize == MX66UW1G45G_ERASE_64K)
      {
        s_command.Instruction = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                ? MX66UW1G45G_BLOCK_ERASE_64K_CMD
                                : MX66UW1G45G_4_BYTE_BLOCK_ERASE_64K_CMD;
      }
      else
      {
        s_command.Instruction = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                ? MX66UW1G45G_SECTOR_ERASE_4K_CMD
                                : MX66UW1G45G_4_BYTE_SECTOR_ERASE_4K_CMD;
      }
      break;
  }

  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }


  return MX66UW1G45G_OK;
}

/**
  * @brief  Whole chip erase.
  *         SPI/OPI; 1-0-0/8-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval error status
  */
int32_t MX66UW1G45G_ChipErase(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the erase command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_BULK_ERASE_CMD
                              : MX66UW1G45G_OCTA_BULK_ERASE_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Enable memory mapped mode for the XSPI memory on STR mode.
  *         SPI/OPI; 1-1-1/8-8-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressWidth Address size
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_EnableSTRMemoryMappedMode(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                              MX66UW1G45G_AddressWidth_t AddressWidth)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  XSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  /* OPI mode and 3-bytes address size not supported by memory */
  if ((Mode == MX66UW1G45G_OPI_MODE) && (AddressWidth == MX66UW1G45G_3BYTES_SIZE))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the read command */
  s_command.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? ((AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                     ? MX66UW1G45G_FAST_READ_CMD
                                     : MX66UW1G45G_4_BYTE_ADDR_FAST_READ_CMD)
                              : MX66UW1G45G_OCTA_READ_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = (AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                               ? HAL_XSPI_ADDRESS_24_BITS
                               : HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_OCTAL;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the read command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? ((AddressWidth == MX66UW1G45G_3BYTES_SIZE)
                                     ? MX66UW1G45G_PAGE_PROG_CMD
                                     : MX66UW1G45G_4_BYTE_PAGE_PROG_CMD)
                              : MX66UW1G45G_OCTA_PAGE_PROG_CMD;
  s_command.DummyCycles = 0U;

  /* Send the write command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_XSPI_MemoryMapped(Ctx, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Enable memory mapped mode for the XSPI memory on DTR mode.
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressWidth Address size
  * @note   Only OPI mode support DTR transfer rate
  * @retval XSPI memory status
  */
int32_t MX66UW1G45G_EnableDTRMemoryMappedMode(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Mode);

  XSPI_RegularCmdTypeDef s_command = {0};
  XSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  /* Initialize the read command */
  s_command.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
  s_command.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
  s_command.InstructionWidth = HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = MX66UW1G45G_OCTA_READ_DTR_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
  s_command.DummyCycles = DUMMY_CYCLES_READ_OCTAL_DTR;
  s_command.DQSMode = HAL_XSPI_DQS_ENABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the program command */
  s_command.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
  s_command.Instruction = MX66UW1G45G_OCTA_PAGE_PROG_CMD;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }
  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_XSPI_MemoryMapped(Ctx, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Flash suspend program or erase command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_Suspend(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the suspend command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_PROG_ERASE_SUSPEND_CMD
                              : MX66UW1G45G_OCTA_PROG_ERASE_SUSPEND_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Flash resume program or erase command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_Resume(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the resume command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_PROG_ERASE_RESUME_CMD
                              : MX66UW1G45G_OCTA_PROG_ERASE_RESUME_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/* Register/Setting Commands **************************************************/
/**
  * @brief  This function send a Write Enable and wait it is effective.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_WriteEnable(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  XSPI_AutoPollingTypeDef s_config = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the write enable command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_ENABLE_CMD
                              : MX66UW1G45G_OCTA_WRITE_ENABLE_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_STATUS_REG_CMD
                              : MX66UW1G45G_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DATA_DTR_ENABLE : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  s_config.MatchValue = 2U;
  s_config.MatchMask = 2U;
  s_config.MatchMode = HAL_XSPI_MATCH_MODE_AND;
  s_config.IntervalTime = MX66UW1G45G_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling(Ctx, &s_config, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  This function reset the (WEN) Write Enable Latch bit.
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_WriteDisable(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the write disable command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_DISABLE_CMD
                              : MX66UW1G45G_OCTA_WRITE_DISABLE_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Read Flash Status register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Status register value pointer
  * @retval error status
  */
int32_t MX66UW1G45G_ReadStatusRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                       MX66UW1G45G_Transfer_t Rate, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reading of status register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_STATUS_REG_CMD
                              : MX66UW1G45G_OCTA_READ_STATUS_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Write Flash Status register
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to Status register
  * @retval error status
  */
int32_t MX66UW1G45G_WriteStatusRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                        MX66UW1G45G_Transfer_t Rate, uint8_t Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  uint8_t reg[2];

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* In SPI mode, the status register is configured with configuration register */
  if (Mode == MX66UW1G45G_SPI_MODE)
  {
    if (MX66UW1G45G_ReadCfgRegister(Ctx, Mode, Rate, &reg[1]) != MX66UW1G45G_OK)
    {
      return MX66UW1G45G_ERROR;
    }
  }
  reg[0] = Value;

  /* Initialize the writing of status register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_STATUS_REG_CMD
                              : MX66UW1G45G_OCTA_WRITE_STATUS_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = (Mode == MX66UW1G45G_SPI_MODE) ? 2U : ((Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U);
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  if (HAL_XSPI_Transmit(Ctx, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Write Flash configuration register
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to configuration register
  * @retval error status
  */
int32_t MX66UW1G45G_WriteCfgRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                     MX66UW1G45G_Transfer_t Rate, uint8_t Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};
  uint8_t reg[2];

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* In SPI mode, the configuration register is configured with status register */
  if (Mode == MX66UW1G45G_SPI_MODE)
  {
    if (MX66UW1G45G_ReadStatusRegister(Ctx, Mode, Rate, &reg[0]) != MX66UW1G45G_OK)
    {
      return MX66UW1G45G_ERROR;
    }
    reg[1] = Value;
  }
  else
  {
    reg[0] = Value;
  }

  /* Initialize the writing of configuration register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_STATUS_REG_CMD
                              : MX66UW1G45G_OCTA_WRITE_STATUS_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 1U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = (Mode == MX66UW1G45G_SPI_MODE) ? 2U : ((Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U);
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  if (HAL_XSPI_Transmit(Ctx, reg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Read Flash configuration register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value configuration register value pointer
  * @retval error status
  */
int32_t MX66UW1G45G_ReadCfgRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                    MX66UW1G45G_Transfer_t Rate, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reading of configuration register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_CFG_REG_CMD
                              : MX66UW1G45G_OCTA_READ_CFG_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 1U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Write Flash configuration register 2
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to configuration register
  * @retval error status
  */
int32_t MX66UW1G45G_WriteCfg2Register(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                      MX66UW1G45G_Transfer_t Rate, uint32_t WriteAddr, uint8_t Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the writing of configuration register 2 */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_CFG_REG2_CMD
                              : MX66UW1G45G_OCTA_WRITE_CFG_REG2_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = WriteAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = 0U;
  s_command.DataLength = (Mode == MX66UW1G45G_SPI_MODE) ? 1U : ((Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U);
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  if (HAL_XSPI_Transmit(Ctx, &Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Read Flash configuration register 2 value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value configuration register 2 value pointer
  * @retval error status
  */
int32_t MX66UW1G45G_ReadCfg2Register(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                     MX66UW1G45G_Transfer_t Rate, uint32_t ReadAddr, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reading of status register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_CFG_REG2_CMD
                              : MX66UW1G45G_OCTA_READ_CFG_REG2_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_1_LINE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = ReadAddr;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Write Flash Security register
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Value to write to Security register
  * @retval error status
  */
int32_t MX66UW1G45G_WriteSecurityRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                          MX66UW1G45G_Transfer_t Rate, uint8_t Value)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Value);

  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the write of security register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_WRITE_SECURITY_REG_CMD
                              : MX66UW1G45G_OCTA_WRITE_SECURITY_REG_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Read Flash Security register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value Security register value pointer
  * @retval error status
  */
int32_t MX66UW1G45G_ReadSecurityRegister(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                         MX66UW1G45G_Transfer_t Rate, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reading of security register */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_SECURITY_REG_CMD
                              : MX66UW1G45G_OCTA_READ_SECURITY_REG_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_ADDRESS_NONE : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = (Rate == MX66UW1G45G_DTR_TRANSFER) ? 2U : 1U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/* ID Commands ****************************************************************/
/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  *         SPI/OPI; 1-0-1/1-0-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  ID 3 bytes IDs pointer
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
int32_t MX66UW1G45G_ReadID(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate,
                           uint8_t *ID)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the read ID command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_READ_ID_CMD
                              : MX66UW1G45G_OCTA_READ_ID_CMD;
  s_command.AddressMode = (Mode == MX66UW1G45G_SPI_MODE)
                              ? HAL_XSPI_ADDRESS_NONE
                              : HAL_XSPI_ADDRESS_8_LINES;
  s_command.AddressDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                 ? HAL_XSPI_ADDRESS_DTR_ENABLE
                                 : HAL_XSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
  s_command.Address = 0U;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = (Mode == MX66UW1G45G_SPI_MODE) ? HAL_XSPI_DATA_1_LINE : HAL_XSPI_DATA_8_LINES;
  s_command.DataDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                              ? HAL_XSPI_DATA_DTR_ENABLE
                              : HAL_XSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles = (Mode == MX66UW1G45G_SPI_MODE)
                              ? 0U
                              : ((Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? DUMMY_CYCLES_REG_OCTAL_DTR
                                     : DUMMY_CYCLES_REG_OCTAL);
  s_command.DataLength = 3U;
  s_command.DQSMode = (Rate == MX66UW1G45G_DTR_TRANSFER) ? HAL_XSPI_DQS_ENABLE : HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, ID, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/* Reset Commands *************************************************************/
/**
  * @brief  Flash reset enable command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_ResetEnable(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reset enable command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.IOSelect =  HAL_XSPI_SELECT_IO_3_0;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_RESET_ENABLE_CMD
                              : MX66UW1G45G_OCTA_RESET_ENABLE_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;


  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Flash reset memory command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_ResetMemory(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the reset enable command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_RESET_MEMORY_CMD
                              : MX66UW1G45G_OCTA_RESET_MEMORY_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Flash no operation command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_NoOperation(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode, MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the no operation command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE) ? MX66UW1G45G_NOP_CMD : MX66UW1G45G_OCTA_NOP_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @brief  Flash enter deep power-down command
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @param  Rate Transfer rate STR or DTR
  * @retval error status
  */
int32_t MX66UW1G45G_EnterPowerDown(XSPI_HandleTypeDef *Ctx, MX66UW1G45G_Interface_t Mode,
                                   MX66UW1G45G_Transfer_t Rate)
{
  XSPI_RegularCmdTypeDef s_command = {0};

  /* SPI mode and DTR transfer not supported by memory */
  if ((Mode == MX66UW1G45G_SPI_MODE) && (Rate == MX66UW1G45G_DTR_TRANSFER))
  {
    return MX66UW1G45G_ERROR;
  }

  /* Initialize the enter power down command */
  s_command.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;

  s_command.InstructionMode = (Mode == MX66UW1G45G_SPI_MODE)
                                  ? HAL_XSPI_INSTRUCTION_1_LINE
                                  : HAL_XSPI_INSTRUCTION_8_LINES;
  s_command.InstructionDTRMode = (Rate == MX66UW1G45G_DTR_TRANSFER)
                                     ? HAL_XSPI_INSTRUCTION_DTR_ENABLE
                                     : HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionWidth = (Mode == MX66UW1G45G_SPI_MODE)
                                   ? HAL_XSPI_INSTRUCTION_8_BITS
                                   : HAL_XSPI_INSTRUCTION_16_BITS;
  s_command.Instruction = (Mode == MX66UW1G45G_SPI_MODE)
                              ? MX66UW1G45G_ENTER_DEEP_POWER_DOWN_CMD
                              : MX66UW1G45G_OCTA_ENTER_DEEP_POWER_DOWN_CMD;
  s_command.AddressMode = HAL_XSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  s_command.DataMode = HAL_XSPI_DATA_NONE;
  s_command.DummyCycles = 0U;
  s_command.DQSMode = HAL_XSPI_DQS_DISABLE;
 #if defined (XSPI_CCR_SIOO)
  s_command.SIOOMode            = HAL_XSPI_SIOO_INST_EVERY_CMD;
 #endif

  /* Send the command */
  if (HAL_XSPI_Command(Ctx, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MX66UW1G45G_ERROR;
  }

  return MX66UW1G45G_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */