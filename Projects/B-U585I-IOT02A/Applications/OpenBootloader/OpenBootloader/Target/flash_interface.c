/**
  ******************************************************************************
  * @file    flash_interface.c
  * @author  MCD Application Team
  * @brief   Contains FLASH access functions
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
#include "platform.h"
#include "common_interface.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
#include "flash_interface.h"
#include "i2c_interface.h"
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t Flash_BusyState = FLASH_BUSY_STATE_DISABLED;
static FLASH_ProcessTypeDef FlashProcess = {.Lock = HAL_UNLOCKED, \
                                            .ErrorCode = HAL_FLASH_ERROR_NONE, \
                                            .ProcedureOnGoing = 0U, \
                                            .Address = 0U, \
                                            .Bank = FLASH_BANK_1, \
                                            .Page = 0U, \
                                            .NbPagesToErase = 0U
                                           };

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FLASH_Program(uint32_t Address, uint32_t Data);
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *ListOfPages, uint32_t Length);
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void);
#if defined (__ICCARM__)
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t Timeout);
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t Timeout);
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *pPageError);
#else
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t Timeout);
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t Timeout);
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(
  FLASH_EraseInitTypeDef *pEraseInit, uint32_t *pPageError);
#endif /* (__ICCARM__) */

/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef FLASH_Descriptor =
{
  FLASH_START_ADDRESS,
  FLASH_END_ADDRESS,
  FLASH_MEM_SIZE,
  FLASH_AREA,
  OPENBL_FLASH_Read,
  OPENBL_FLASH_Write,
  OPENBL_FLASH_SetReadOutProtectionLevel,
  OPENBL_FLASH_SetWriteProtection,
  OPENBL_FLASH_JumpToAddress,
  NULL,
  OPENBL_FLASH_Erase
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Unlock(void)
{
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Lock(void)
{
  if (HAL_FLASH_Lock() != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @retval None.
  */
void OPENBL_FLASH_OB_Unlock(void)
{
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_FLASH_OB_Unlock() != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_FLASH_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in FLASH memory.
  * @param  Address The address where that data will be written.
  * @param  pData The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_FLASH_Write(uint32_t Address, uint8_t *pData, uint32_t DataLength)
{
  uint32_t index;
  uint32_t length;
  uint32_t remainder;
  uint8_t remainder_data[16] = {0x0U};

  /* Check the remaining of quad-word */
  length = DataLength;
  remainder = length & 0xFU;

  if (remainder != 0U)
  {
    length = (length & 0xFFFFFFF0U);

    /* Copy the remaining bytes */
    for (index = 0U; index < remainder; index++)
    {
      remainder_data[index] = pData[length + index];
    }

    /* Fill the upper bytes with 0xFF */
    for (index = remainder; index < 16U; index++)
    {
      remainder_data[index] = 0xFFU;
    }
  }

  /* Unlock the flash memory for write operation */
  OPENBL_FLASH_Unlock();

  for (index = 0U; index < length; (index += 16U))
  {
    OPENBL_FLASH_Program((Address + index), (uint32_t)(&pData[index]));
  }

  if (remainder != 0U)
  {
    OPENBL_FLASH_Program((Address + length), (uint32_t)(remainder_data));
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void OPENBL_FLASH_JumpToAddress(uint32_t Address)
{
  Function_Pointer jump_to_address;

  /* De-initialize all HW resources used by the Open Bootloader to their reset values */
  OPENBL_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @retval The return value can be one of the following values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  */
uint32_t OPENBL_FLASH_GetReadOutProtectionLevel(void)
{
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Get the Option bytes configuration */
  HAL_FLASHEx_OBGetConfig(&flash_ob);

  return flash_ob.RDPLevel;
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @param  Level Can be one of these values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  * @retval None.
  */
void OPENBL_FLASH_SetReadOutProtectionLevel(uint32_t Level)
{
  FLASH_OBProgramInitTypeDef flash_ob;

  if (Level != OB_RDP_LEVEL2)
  {
    flash_ob.OptionType = OPTIONBYTE_RDP;
    flash_ob.RDPLevel   = Level;

    /* Unlock the FLASH registers & Option Bytes registers access */
    OPENBL_FLASH_OB_Unlock();

    /* Change the RDP level */
    if (HAL_FLASHEx_OBProgram(&flash_ob) == HAL_OK)
    {
      /* Register system reset callback */
      Common_SetPostProcessingCallback(OPENBL_OB_Launch);
    }
  }
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  State Can be one of these values:
  *         @arg DISABLE: Disable FLASH write protection
  *         @arg ENABLE: Enable FLASH write protection
  * @param  pListOfPages Contains the list of pages to be protected.
  * @param  Length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
ErrorStatus OPENBL_FLASH_SetWriteProtection(FunctionalState State, uint8_t *pListOfPages, uint32_t Length)
{
  ErrorStatus status;

  if (State == ENABLE)
  {
    status = OPENBL_FLASH_EnableWriteProtection(pListOfPages, Length);
  }
  else if (State == DISABLE)
  {
    status = OPENBL_FLASH_DisableWriteProtection();
  }
  else
  {
    status = ERROR;
  }

  if (status == SUCCESS)
  {
    /* Register system reset callback */
    Common_SetPostProcessingCallback(OPENBL_OB_Launch);
  }

  return status;
}

/**
  * @brief  This function is used to start FLASH mass erase operation.
  * @param  *pData Pointer to the buffer that contains mass erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Mass erase operation done
  *          - ERROR:   Mass erase operation failed or the value of one parameter is not OK
  */
ErrorStatus OPENBL_FLASH_MassErase(uint8_t *pData, uint32_t DataLength)
{
  uint32_t page_error;
  uint16_t bank_option;
  ErrorStatus status   = SUCCESS;
  FLASH_EraseInitTypeDef erase_init_struct;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  erase_init_struct.TypeErase = FLASH_TYPEERASE_MASSERASE;

  if (DataLength >= 2U)
  {
    bank_option  = (uint16_t)pData[0] << 8U;
    bank_option |= (uint16_t)pData[1];

    if (bank_option == FLASH_MASS_ERASE)
    {
      erase_init_struct.Banks = 0U;
    }
    else if (bank_option == FLASH_BANK1_ERASE)
    {
      erase_init_struct.Banks = FLASH_BANK_1;
    }
    else if (bank_option == FLASH_BANK2_ERASE)
    {
      erase_init_struct.Banks = FLASH_BANK_2;
    }
    else
    {
      status = ERROR;
    }

    if (status == SUCCESS)
    {
      if (OPENBL_FLASH_ExtendedErase(&erase_init_struct, &page_error) != HAL_OK)
      {
        status = ERROR;
      }
      else
      {
        status = SUCCESS;
      }
    }
  }
  else
  {
    status = ERROR;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  return status;
}

/**
  * @brief  This function is used to erase the specified FLASH pages.
  * @param  *pData Pointer to the buffer that contains erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Erase operation done
  *          - ERROR:   Erase operation failed or the value of one parameter is not OK
  */
ErrorStatus OPENBL_FLASH_Erase(uint8_t *pData, uint32_t DataLength)
{
  uint32_t counter;
  uint32_t page_error;
  uint32_t errors    = 0U;
  ErrorStatus status = SUCCESS;
  FLASH_EraseInitTypeDef erase_init_struct;
  uint16_t bytes_number;
  uint16_t page_code;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  bytes_number  = (uint16_t)pData[1] << 8U;
  bytes_number |= (uint16_t)pData[0];
  bytes_number *= 2U;

  erase_init_struct.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init_struct.NbPages   = 1U;

  /* The first two bytes contain the number of pages to be erased, that is why counter starts from the index 2 */
  for (counter = 2U; ((counter <= bytes_number) && (counter < DataLength)) ; counter += 2U)
  {
    page_code  = (uint16_t)pData[counter + 1] << 8U;
    page_code |= (uint16_t)pData[counter];

    erase_init_struct.Page = ((uint32_t)page_code);

    if (erase_init_struct.Page <= 127U)
    {
      erase_init_struct.Banks = FLASH_BANK_1;
    }
    else if (erase_init_struct.Page <= 255U)
    {
      erase_init_struct.Banks = FLASH_BANK_2;
    }
    else
    {
      status = ERROR;
    }

    if (status != ERROR)
    {
      if (OPENBL_FLASH_ExtendedErase(&erase_init_struct, &page_error) != HAL_OK)
      {
        errors++;
      }
    }
    else
    {
      /* Reset the status for next erase operation */
      status = SUCCESS;
    }
  }

  if (errors > 0U)
  {
    status = ERROR;
  }
  else
  {
    status = SUCCESS;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  return status;
}

/**
  * @brief  This function is used to Set Flash busy state variable to activate busy state sending
  *         during flash operations
  * @retval None.
  */
void OPENBL_Enable_BusyState_Flag(void)
{
  /* Enable Flash busy state sending */
  Flash_BusyState = FLASH_BUSY_STATE_ENABLED;
}

/**
  * @brief  This function is used to disable the send of busy state in I2C non stretch mode.
  * @retval None.
  */
void OPENBL_Disable_BusyState_Flag(void)
{
  /* Disable Flash busy state sending */
  Flash_BusyState = FLASH_BUSY_STATE_DISABLED;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Program double word at a specified FLASH address.
  * @param  Address specifies the address to be programmed.
  * @param  Data specifies the data to be programmed.
  * @retval None.
  */
static void OPENBL_FLASH_Program(uint32_t Address, uint32_t Data)
{
  (void)HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, Address, Data);
}

/**
  * @brief  This function is used to enable write protection of the specified FLASH areas.
  * @param  pListOfPages Contains the list of pages to be protected.
  * @param  Length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *pListOfPages, uint32_t Length)
{
  FLASH_OBProgramInitTypeDef flash_ob;
  ErrorStatus status  = SUCCESS;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Write protection of bank 1 area WRPA 1 area */
  if (Length >= 2U)
  {
    flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAA;
    flash_ob.WRPStartOffset = pListOfPages[0];
    flash_ob.WRPEndOffset   = pListOfPages[1];
    flash_ob.WRPLock        = DISABLE;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }
  if (status == SUCCESS)
  {
    /* Write protection of bank 1 area WRPA 2 area */
    if (Length >= 4U)
    {
      flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAB;
      flash_ob.WRPStartOffset = pListOfPages[2];
      flash_ob.WRPEndOffset   = pListOfPages[3];
      flash_ob.WRPLock        = DISABLE;

      status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
    }
  }

  if (status == SUCCESS)
  {
    /* Write protection of bank 2 area WRPB 1 area */
    if (Length >= 6U)
    {
      flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAA;
      flash_ob.WRPStartOffset = pListOfPages[4];
      flash_ob.WRPEndOffset   = pListOfPages[5];
      flash_ob.WRPLock        = DISABLE;

      status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
    }
  }

  if (status == SUCCESS)
  {
    /* Write protection of bank 2 area WRPB 2 area */
    if (Length >= 8U)
    {
      flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAB;
      flash_ob.WRPStartOffset = pListOfPages[6];
      flash_ob.WRPEndOffset   = pListOfPages[7];
      flash_ob.WRPLock        = DISABLE;

      status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
    }
  }

  return status;
}

/**
  * @brief  This function is used to disable write protection.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void)
{
  ErrorStatus status;
  FLASH_OBProgramInitTypeDef flash_ob;
  uint8_t wrp_start_offset = 0x7FU;
  uint8_t wrp_end_offset   = 0x00U;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Disable write protection of bank 1 area WRPA A area */
  flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAA;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;
  flash_ob.WRPLock        = DISABLE;

  status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;

  if (status == SUCCESS)
  {
    /* Disable write protection of bank 1 area WRPA B area */
    flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAB;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;
    flash_ob.WRPLock        = DISABLE;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }

  if (status == SUCCESS)
  {
    /* Disable write protection of bank 2 area WRPB A area */
    flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAA;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;
    flash_ob.WRPLock        = DISABLE;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }

  if (status == SUCCESS)
  {
    /* Disable write protection of bank 2 area WRPB B area */
    flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAB;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;
    flash_ob.WRPLock        = DISABLE;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }

  return status;
}

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout.
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t Timeout)
#else
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t Timeout)
#endif /* (__ICCARM__) */
{
  uint32_t tick = 0;
  uint32_t error;
  __IO uint32_t *reg_sr;
  HAL_StatusTypeDef status = HAL_OK;

  /* While the FLASH is in busy state, send busy byte to the host */
  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) == 0U)
  {
    tick++;
    if (tick > Timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
    else
    {
      /* Send busy byte */
      OPENBL_I2C_SendBusyByte();
    }
  }

  /* Access to SECSR or NSSR registers depends on operation type */
  reg_sr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECSR) : &(FLASH_NS->NSSR);

  /* Check FLASH operation error flags */
  error = ((*reg_sr) & FLASH_FLAG_SR_ERRORS);

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  error |= (FLASH->NSSR & FLASH_FLAG_OPTWERR);
#endif /* __ARM_FEATURE_CMSE */

  if (error != 0U)
  {
    /* Clear error programming flags */
    (*reg_sr) = error;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if ((error & FLASH_FLAG_OPTWERR) != 0U)
    {
      FLASH->NSSR = FLASH_FLAG_OPTWERR;
    }
#endif /* __ARM_FEATURE_CMSE */

    status = HAL_ERROR;
  }

  /* Check FLASH End of Operation flag  */
  if (((*reg_sr) & FLASH_FLAG_EOP) != 0U)
  {
    /* Clear FLASH End of Operation pending bit */
    (*reg_sr) = FLASH_FLAG_EOP;
  }

  return status;
}

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout.
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t Timeout)
#else
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t Timeout)
#endif /* (__ICCARM__) */
{
  uint32_t tick = 0U;
  uint32_t error;
  __IO uint32_t *reg_sr;
  HAL_StatusTypeDef status = HAL_OK;

  /* While the FLASH is in busy state, send busy byte to the host */
  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0U)
  {
    tick++ ;
    if (tick > Timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  /* Access to SECSR or NSSR registers depends on operation type */
  reg_sr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECSR) : &(FLASH_NS->NSSR);

  /* Check FLASH operation error flags */
  error = ((*reg_sr) & FLASH_FLAG_SR_ERRORS);

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  error |= (FLASH->NSSR & FLASH_FLAG_OPTWERR);
#endif /* __ARM_FEATURE_CMSE */

  if (error != 0U)
  {
    /*Save the error code*/
    FlashProcess.ErrorCode |= error;

    /* Clear error programming flags */
    (*reg_sr) = error;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if ((error & FLASH_FLAG_OPTWERR) != 0U)
    {
      FLASH->NSSR = FLASH_FLAG_OPTWERR;
    }
#endif /* __ARM_FEATURE_CMSE */

    status = HAL_ERROR;
  }

  /* Check FLASH End of Operation flag  */
  if (((*reg_sr) & FLASH_FLAG_EOP) != 0U)
  {
    /* Clear FLASH End of Operation pending bit */
    (*reg_sr) = FLASH_FLAG_EOP;
  }

  return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages.
  * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * @param[out]  pPageError pointer to variable that contains the configuration
  *         information on faulty page in case of error (0xFFFFFFFF means that all
  *         the pages have been correctly erased).
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc static HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *pPageError)
#else
__attribute__((section(".ramfunc"))) static HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(
  FLASH_EraseInitTypeDef *pEraseInit, uint32_t *pPageError)
#endif /* (__ICCARM__) */
{
  HAL_StatusTypeDef status;
  uint32_t errors = 0U;
  __IO uint32_t *reg_cr;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  uint32_t primask_bit;
#endif

  /* Process Locked */
  __HAL_LOCK(&FlashProcess);

  /* Reset error code */
  FlashProcess.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Verify that next operation can be proceed */
  status = OPENBL_FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);

  if (status == HAL_OK)
  {
    FlashProcess.ProcedureOnGoing = pEraseInit->TypeErase;

    /* Access to SECCR or NSCR depends on operation type */
    reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH->NSCR);

    /*Initialization of PageError variable*/
    *pPageError = 0xFFFFFFFFU;

    /* Access to SECCR or NSCR registers depends on operation type */
    reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);

    if (((pEraseInit->Banks) & FLASH_BANK_1) != 0U)
    {
      CLEAR_BIT((*reg_cr), FLASH_NSCR_BKER);
    }
    else
    {
      SET_BIT((*reg_cr), FLASH_NSCR_BKER);
    }

    /* Proceed to erase the page */
    MODIFY_REG((*reg_cr), (FLASH_NSCR_PNB | FLASH_NSCR_PER | FLASH_NSCR_STRT),
               (((pEraseInit->Page) << FLASH_NSCR_PNB_Pos) | FLASH_NSCR_PER | FLASH_NSCR_STRT));

    if (Flash_BusyState == FLASH_BUSY_STATE_ENABLED)
    {
      /* Wait for last operation to be completed to send busy byte */
      if (OPENBL_FLASH_SendBusyState(PROGRAM_TIMEOUT) != HAL_OK)
      {
        errors++;
      }
    }
    else
    {
      /* Wait for last operation to be completed */
      if (OPENBL_FLASH_WaitForLastOperation(PROGRAM_TIMEOUT) != HAL_OK)
      {
        errors++;
      }
    }

    /* If the erase operation is completed, disable the associated bits */
    CLEAR_BIT((*reg_cr), (pEraseInit->TypeErase) & (~(FLASH_NON_SECURE_MASK)));
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&FlashProcess);

  if (errors > 0U)
  {
    status = HAL_ERROR;
  }
  else
  {
    status = HAL_OK;
  }

  return status;
}
