/*
 * Copyright (c) 2013-2018 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_Flash.h"
#include "low_level_flash.h"
#include <string.h>
#include "cmsis.h"
#include "flash_layout.h"
#include "stm32l5xx_hal.h"
#include <stdio.h>

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif /* ARG_UNUSED */
void NMI_Handler(void);
/* config for flash driver */
#define FLASH0_SECTOR_SIZE  0x1000
#define FLASH0_PAGE_SIZE 0x800
#define FLASH0_PROG_UNIT 0x8
#define FLASH0_ERASED_VAL 0xff
/*
#define DEBUG_FLASH_ACCESS
#define CHECK_ERASE
*/
/* Driver version */
#define ARM_FLASH_DRV_VERSION   ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

static const ARM_DRIVER_VERSION DriverVersion =
{
  ARM_FLASH_API_VERSION,  /* Defined in the CMSIS Flash Driver header file */
  ARM_FLASH_DRV_VERSION
};

/**
  * \brief Flash driver capability macro definitions \ref ARM_FLASH_CAPABILITIES
  */
/* Flash Ready event generation capability values */
#define EVENT_READY_NOT_AVAILABLE   (0u)
#define EVENT_READY_AVAILABLE       (1u)
/* Data access size values */
#define DATA_WIDTH_8BIT             (0u)
#define DATA_WIDTH_16BIT            (1u)
#define DATA_WIDTH_32BIT            (2u)
/* Chip erase capability values */
#define CHIP_ERASE_NOT_SUPPORTED    (0u)
#define CHIP_ERASE_SUPPORTED        (1u)

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities =
{
  EVENT_READY_NOT_AVAILABLE,
  DATA_WIDTH_32BIT,
  CHIP_ERASE_SUPPORTED
};

/**
  * \brief Flash status macro definitions \ref ARM_FLASH_STATUS
  */
/* Busy status values of the Flash driver  */
#define DRIVER_STATUS_IDLE      (0u)
#define DRIVER_STATUS_BUSY      (1u)
/* Error status values of the Flash driver */
#define DRIVER_STATUS_NO_ERROR  (0u)
#define DRIVER_STATUS_ERROR     (1u)

/**
  * \brief Arm Flash device structure.
  */
struct arm_flash_dev_t
{
  struct low_level_device *dev;
  ARM_FLASH_INFO *data;       /*!< FLASH memory device data */
};
/**
  * @}
  */
/** @defgroup FLASH_Private_Variables Private Variables
  * @{
  */
static __IO uint32_t DoubleECC_Error_Counter = 0U;

/**
  * \brief      Check if the Flash memory boundaries are not violated.
  * \param[in]  flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]  offset     Highest Flash memory address which would be accessed.
  * \return     Returns true if Flash memory boundaries are not violated, false
  *             otherwise.
  */

static bool is_range_valid(struct arm_flash_dev_t *flash_dev,
                           uint32_t offset)
{
  uint32_t flash_limit = 0;

  /* Calculating the highest address of the Flash memory address range */
  flash_limit = FLASH_TOTAL_SIZE - 1;

  return (offset > flash_limit) ? (false) : (true) ;
}
/**
  * \brief        Check if the range is secure .
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to program_unit, false
  *               otherwise.
  */
static bool is_range_secure(struct arm_flash_dev_t *flash_dev,
                            uint32_t start, uint32_t len)
{
  /*  allow write access in area provided by device info */
  struct flash_vect *vect = &flash_dev->dev->secure;
  uint32_t nb;
  /* NULL descriptor , means range is only secure */
  if (!vect->range)
  {
    return true;
  }
  for (nb = 0; nb < vect->nb; nb++)
    if ((start >= vect->range[nb].base) && ((start + len - 1) <= vect->range[nb].limit))
    {
      return true;
    }
  return false;
}
/**
  * \brief        Check if the parameter is an erasebale page.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is a sector eraseable, false
  *               otherwise.
  */
static bool is_erase_allow(struct arm_flash_dev_t *flash_dev,
                           uint32_t param)
{
  /*  allow erase in range provided by device info */
  struct flash_vect *vect = &flash_dev->dev->erase;
  uint32_t nb;
  for (nb = 0; nb < vect->nb; nb++)
    if ((param >= vect->range[nb].base) && (param <= vect->range[nb].limit))
    {
      return true;
    }
  return false;
}
/**
  * \brief        Check if the parameter is writeable area.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to program_unit, false
  *               otherwise.
  */
static bool is_write_allow(struct arm_flash_dev_t *flash_dev,
                           uint32_t start, uint32_t len)
{
  /*  allow write access in area provided by device info */
  struct flash_vect *vect = &flash_dev->dev->write;
  uint32_t nb;
  for (nb = 0; nb < vect->nb; nb++)
    if ((start >= vect->range[nb].base) && ((start + len - 1) <= vect->range[nb].limit))
    {
      return true;
    }
  return false;
}

/**
  * \brief        Check if the parameter is aligned to program_unit.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to program_unit, false
  *               otherwise.
  */

static bool is_write_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
  return ((param % flash_dev->data->program_unit) != 0) ? (false) : (true);
}
/**
  * \brief        Check if the parameter is aligned to page_unit.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to sector_unit, false
  *               otherwise.
  */
static bool is_erase_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
  /*  2 pages */
  return ((param % (flash_dev->data->sector_size)) != 0) ? (false) : (true);
}

/**
  * \brief        compute bank number.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      &n address in flash
  * \return       Returns true if param is aligned to sector_unit, false
  *               otherwise.
  */
static uint32_t bank_number(struct arm_flash_dev_t *flash_dev,
                            uint32_t param)
{
  return ((param >= (FLASH_TOTAL_SIZE / 2)) ? 2 : 1);
}

/**
  * \brief        compute page number.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      &n address in flash
  * \return       Returns true if param is aligned to sector_unit, false
  *               otherwise.
  */
static uint32_t page_number(struct arm_flash_dev_t *flash_dev,
                            uint32_t param)
{
  uint32_t page = param / flash_dev->data->page_size ;
  page = ((page > (flash_dev->data->sector_count))) ? page - ((flash_dev->data->sector_count)) : page;
#ifdef DEBUG_FLASH_ACCESS
  printf("page = %x \r\n", page);
#endif /* DEBUG_FLASH_ACCESS */
  return page;
}

static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA =
{
  .sector_info    = NULL,     /* Uniform sector layout */
  .sector_count   = FLASH_TOTAL_SIZE / FLASH0_SECTOR_SIZE,
  .sector_size    = FLASH0_SECTOR_SIZE,
  .page_size      = FLASH0_PAGE_SIZE,
  .program_unit   = FLASH0_PROG_UNIT,       /* Minimum write size in bytes */
  .erased_value   = FLASH0_ERASED_VAL,
};

static struct arm_flash_dev_t ARM_FLASH0_DEV =
{
  .dev = &(FLASH0_DEV),
  .data   = &(ARM_FLASH0_DEV_DATA)
};

/* Flash Status */
static ARM_FLASH_STATUS ARM_FLASH0_STATUS = {0, 0, 0};

static ARM_DRIVER_VERSION Flash_GetVersion(void)
{
  return DriverVersion;
}

static ARM_FLASH_CAPABILITIES Flash_GetCapabilities(void)
{
  return DriverCapabilities;
}

static int32_t Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
  ARG_UNUSED(cb_event);
  FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  return ARM_DRIVER_OK;
}

static int32_t Flash_Uninitialize(void)
{
  return ARM_DRIVER_OK;
}

static int32_t Flash_PowerControl(ARM_POWER_STATE state)
{
  switch (state)
  {
    case ARM_POWER_FULL:
      /* Nothing to be done */
      return ARM_DRIVER_OK;
    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    default:
      return ARM_DRIVER_ERROR_PARAMETER;
  }
}

static int32_t Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
  int32_t ret = ARM_DRIVER_ERROR_SPECIFIC;
  bool is_valid = true;

  ARM_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;

  /* Check Flash memory boundaries */
  is_valid = is_range_valid(&ARM_FLASH0_DEV, addr + cnt - 1);
  if (is_valid != true)
  {
    if (ARM_FLASH0_DEV.dev->read_error)
    {
      ARM_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
      return ARM_DRIVER_ERROR_PARAMETER;
    }
    memset(data, 0xff, cnt);
    return ARM_DRIVER_OK;
  }
  /*  ECC to implemeny with NMI */
  /*  do a memcpy */
#ifdef DEBUG_FLASH_ACCESS
  printf("read %x n=%x \r\n", (addr + FLASH_BASE), cnt);
#endif /*  DEBUG_FLASH_ACCESS */
  DoubleECC_Error_Counter = 0U;
  /* area secure and non secure are done with a non secure access */
  if (is_range_secure(&ARM_FLASH0_DEV, addr, cnt))
  {
    memcpy(data, (void *)((uint32_t)addr + FLASH_BASE), cnt);
  }
  else
  {
    memcpy(data, (void *)((uint32_t)addr + FLASH_BASE_NS), cnt);
  }
  if (DoubleECC_Error_Counter == 0U)
  {
    ret = ARM_DRIVER_OK;
  }
  DoubleECC_Error_Counter = 0U;

  return ret;
}

static int32_t Flash_ProgramData(uint32_t addr,
                                 const void *data, uint32_t cnt)
{
  uint32_t loop = 0;
  uint32_t flash_base = (uint32_t)FLASH_BASE_S;
  uint32_t write_type = FLASH_TYPEPROGRAM_DOUBLEWORD;
  HAL_StatusTypeDef err;
#if defined(CHECK_WRITE) || defined(DEBUG_FLASH_ACCESS)
  void *dest;
#endif
  ARM_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;
  if (is_range_secure(&ARM_FLASH0_DEV, addr, cnt))
  {
    flash_base = (uint32_t)FLASH_BASE_S;
    write_type = FLASH_TYPEPROGRAM_DOUBLEWORD;
  }
  else
  {
    flash_base = (uint32_t)FLASH_BASE_NS;
    write_type = FLASH_TYPEPROGRAM_DOUBLEWORD_NS;
  }
#if defined(CHECK_WRITE) || defined(DEBUG_FLASH_ACCESS)
  dest = (void *)(flash_base + addr);
#endif
#ifdef DEBUG_FLASH_ACCESS
  printf("write %x n=%lx \r\n", dest, cnt);
#endif /* DEBUG_FLASH_ACCESS */
  /* Check Flash memory boundaries and alignment with minimum write size
    * (program_unit), data size also needs to be a multiple of program_unit.
  */
  if ((!is_range_valid(&ARM_FLASH0_DEV, addr + cnt - 1)) ||
      (!is_write_aligned(&ARM_FLASH0_DEV, addr))     ||
      (!is_write_aligned(&ARM_FLASH0_DEV, cnt))      ||
      (!is_write_allow(&ARM_FLASH0_DEV, addr, cnt))
     )
  {

    ARM_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  HAL_FLASH_Unlock();
  ARM_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;
  do
  {
    /* dword api*/
    uint64_t dword;
    memcpy(&dword, (void *)((uint32_t)data + loop), sizeof(dword));
    err = HAL_FLASH_Program(write_type, (flash_base + addr), dword);
    loop += sizeof(dword);
    addr += sizeof(dword);
  } while ((loop != cnt) && (err == HAL_OK));

  ARM_FLASH0_STATUS.busy = DRIVER_STATUS_IDLE;
  HAL_FLASH_Lock();
  /* compare data written */
#ifdef CHECK_WRITE
  if ((err == HAL_OK) && memcmp(dest, data, cnt))
  {
    err = HAL_ERROR;
#ifdef DEBUG_FLASH_ACCESS
    printf("write %lx n=%x (cmp failed)\n", (dest), cnt);
#endif /* DEBUG_FLASH_ACCESS */
  }
#endif /* CHECK_WRITE */
#ifdef DEBUG_FLASH_ACCESS
  if (err != HAL_OK)
  {
    printf("failed write %lx n=%x \r\n", (dest), cnt);
  }
#endif /* DEBUG_FLASH_ACCESS */
  return (err == HAL_OK) ? ARM_DRIVER_OK : ARM_DRIVER_ERROR;
}

static int32_t Flash_EraseSector(uint32_t addr)
{
  FLASH_EraseInitTypeDef EraseInit;
  HAL_StatusTypeDef err;
  uint32_t pageError;
#ifdef CHECK_ERASE
  uint32_t i;
  uint32_t *pt;
#endif /* CHECK_ERASE */
#ifdef DEBUG_FLASH_ACCESS
  printf("erase %x\n", addr);
#endif /* DEBUG_FLASH_ACCESS */
  if (!(is_range_valid(&ARM_FLASH0_DEV, addr)) ||
      !(is_erase_aligned(&ARM_FLASH0_DEV, addr)) ||
      !(is_erase_allow(&ARM_FLASH0_DEV, addr)))
  {
    ARM_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
    printf("failed erase %x\n", addr);
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if (is_range_secure(&ARM_FLASH0_DEV, addr, 4))
  {
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
  }
  else
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES_NS;
  /*  fix me assum dual bank, reading DBANK in OPTR in Flash init is better */
  /*  flash size in  DB256K in OPTR */
  EraseInit.Banks = bank_number(&ARM_FLASH0_DEV, addr);
  /*  erase 2 pages because we declare that we have 4096 pages */
  EraseInit.NbPages = 2;
  EraseInit.Page = page_number(&ARM_FLASH0_DEV, addr);

  ARM_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;
  HAL_FLASH_Unlock();
  ARM_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;
  err = HAL_FLASHEx_Erase(&EraseInit, &pageError);
  ARM_FLASH0_STATUS.busy = DRIVER_STATUS_IDLE;
  HAL_FLASH_Lock();
#ifdef DEBUG_FLASH_ACCESS
  if (err != HAL_OK)
  {
    printf("erase failed \n");
  }
#endif /* DEBUG_FLASH_ACCESS */
#ifdef CHECK_ERASE
  if (is_range_secure(&ARM_FLASH0_DEV, addr, 4))
  {
    pt = (uint32_t *)((uint32_t)FLASH_BASE_S + addr);
  }
  else
  {
    pt = (uint32_t *)((uint32_t)FLASH_BASE_NS + addr);
  }
  for (i = 0; i > 0x400; i++)
  {
    if (pt[i] != 0xffffffff)
    {
#ifdef DEBUG_FLASH_ACCESS
      printf("erase failed off %x %x %x\r\n", addr, &pt[i], pt[i]);
#endif /* DEBUG_FLASH_ACCESS */
      err = HAL_ERROR;
      break;
    }
  }
#endif /* CHECK_ERASE */
  return (err == HAL_OK) ? ARM_DRIVER_OK : ARM_DRIVER_ERROR;
}

static int32_t Flash_EraseChip(void)
{
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_FLASH_STATUS Flash_GetStatus(void)
{
  return ARM_FLASH0_STATUS;
}

static ARM_FLASH_INFO *Flash_GetInfo(void)
{
  return ARM_FLASH0_DEV.data;
}

ARM_DRIVER_FLASH TFM_Driver_FLASH0 =
{
  Flash_GetVersion,
  Flash_GetCapabilities,
  Flash_Initialize,
  Flash_Uninitialize,
  Flash_PowerControl,
  Flash_ReadData,
  Flash_ProgramData,
  Flash_EraseSector,
  Flash_EraseChip,
  Flash_GetStatus,
  Flash_GetInfo
};


/**
  * @brief  Get Link Register value (LR)
  * @param  None.
  * @retval LR Register value
  */
#if !defined ( __ICCARM__ )
__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_LR(void)
{
  register uint32_t result;

#if defined ( __GNUC__ )
  __ASM volatile("MOV %0, LR" : "=r"(result));
#elif defined ( __CC_ARM )
  __ASM volatile("MOV result, __return_address()");
#endif /* ( __GNUC__ )  */

  return result;
}
#endif

/*
   As this handler code relies on stack pointer position to manipulate the PC return value, it is important
   not to use extra registers (on top of scratch registers), because it would change the stack pointer
   position. Then compiler optimizations are customized to ensure that.
*/

/*  to rework the define has changed with  ARMCLANG .... */
#if defined(__ARMCC_VERSION)
/*#pragma O0 */
#elif defined ( __GNUC__ )
/**/
#endif /*__ARMCC_VERSION */

void NMI_Handler(void)
{
  uint32_t *p_sp;
  uint32_t lr;
  uint16_t opcode_msb;
  if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_ECCD))
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ECCD);
    /* Memorize error to ignore the read value */
    DoubleECC_Error_Counter++;

    lr = __get_LR();

    /* Check EXC_RETURN value in LR to know which SP was used prior entering exception */
    if (((lr) & (0xFU)) == 0xDU)
    {
      /* interrupted code was using Process Stack Pointer */
      p_sp = (uint32_t *)__get_PSP();
    }
    else
    {
      /* interrupted code was using Main Stack Pointer */
      p_sp = (uint32_t *)__get_MSP();
    }

    /* Test caller mode T bit from CPSR in stack */
    if ((*(p_sp + 7U) & (1U << xPSR_T_Pos)) != 0U)
    {
      /* Thumb  mode.
         Test PC in stack.
         If bits [15:11] of the halfword being decoded take any of the following values,
         the halfword is the first halfword of a 32-bit instruction: 0b11101, 0b11110, 0b11111.
         Otherwise, the halfword is a 16-bit instruction.
      */
      opcode_msb = (*(uint16_t *)(*(p_sp + 6) & 0xFFFFFFFEU) & 0xF800U);
      if ((opcode_msb == 0xE800U) || (opcode_msb == 0xF000U) || (opcode_msb == 0xF800U))
      {
        /* execute next instruction PC +4  */
        *(p_sp + 6U) += 4U;
      }
      else
      {
        /* execute next instruction PC +2  */
        *(p_sp + 6U) += 2U;
      }
    }
    else
    {
      /* ARM mode execute next instruction PC +4 */
      *(p_sp + 6U) += 4U;
    }
  }
  else
  {
    /* This exception occurs for another reason than flash double ECC errors */
    while (1U);
  }
}
