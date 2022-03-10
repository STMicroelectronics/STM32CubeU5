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
#include "low_level_ospi_flash.h"
#include <string.h>
#include "stm32l5xx.h"
#include "flash_layout.h"
#include "stm32l562e_discovery_ospi.h"
#include <stdio.h>


#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif /* ARG_UNUSED */

/* config for flash driver */
#define OSPI_FLASH0_TOTAL_SIZE   OSPI_FLASH_TOTAL_SIZE
#define OSPI_FLASH0_SECTOR_SIZE  MX25LM51245G_SUBSECTOR_4K /* Must be same as internal flash sector size */
#define OSPI_FLASH0_PAGE_SIZE    MX25LM51245G_PAGE_SIZE
#define OSPI_FLASH0_PROG_UNIT    0x2 /* 2 bytes for OSPI DTR mode */
#define OSPI_FLASH0_ERASED_VAL   0xff

/*
#define DEBUG_OSPI_FLASH_ACCESS
*/
/* Driver version */
#define ARM_OSPI_FLASH_DRV_VERSION   ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

static const ARM_DRIVER_VERSION DriverVersion =
{
  ARM_FLASH_API_VERSION,  /* Defined in the CMSIS Flash Driver header file */
  ARM_OSPI_FLASH_DRV_VERSION
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
  DATA_WIDTH_16BIT, /* 16bits for DTR mode */
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
struct arm_ospi_flash_dev_t
{
  struct low_level_ospi_device *dev;
  ARM_FLASH_INFO *data;       /*!< OSPI FLASH memory device data */
};
/**
  * @}
  */

/**
  * \brief      Check if the Flash memory boundaries are not violated.
  * \param[in]  flash_dev  Flash device structure \ref arm_ospi_flash_dev_t
  * \param[in]  offset     Highest Flash memory address which would be accessed.
  * \return     Returns true if Flash memory boundaries are not violated, false
  *             otherwise.
  */

static bool is_range_valid(struct arm_ospi_flash_dev_t *flash_dev,
                           uint32_t offset)
{
  uint32_t flash_limit = 0;

  /* Calculating the highest address of the Flash memory address range */
  flash_limit = OSPI_FLASH_TOTAL_SIZE - 1;

  return (offset > flash_limit) ? (false) : (true) ;
}
/**
  * \brief        Check if the parameter is an erasable page.
  * \param[in]    flash_dev  Flash device structure \ref arm_ospi_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is a sector eraseable, false
  *               otherwise.
  */
static bool is_erase_allow(struct arm_ospi_flash_dev_t *flash_dev,
                           uint32_t param)
{
  /*  allow erase in range provided by device info */
  struct ospi_flash_vect *vect = &flash_dev->dev->erase;
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
  * \param[in]    flash_dev  Flash device structure \ref arm_ospi_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to program_unit, false
  *               otherwise.
  */

static bool is_write_allow(struct arm_ospi_flash_dev_t *flash_dev,
                           uint32_t start, uint32_t len)
{
  /*  allow write access in area provided by device info */
  struct ospi_flash_vect *vect = &flash_dev->dev->write;
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
  * \param[in]    flash_dev  Flash device structure \ref arm_ospi_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to program_unit, false
  *               otherwise.
  */

static bool is_write_aligned(struct arm_ospi_flash_dev_t *flash_dev,
                             uint32_t param)
{
  return ((param % flash_dev->data->program_unit) != 0) ? (false) : (true);
}
/**
  * \brief        Check if the parameter is aligned to page_unit.
  * \param[in]    flash_dev  Flash device structure \ref arm_ospi_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          program_unit, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to sector_unit, false
  *               otherwise.
  */
static bool is_erase_aligned(struct arm_ospi_flash_dev_t *flash_dev,
                             uint32_t param)
{
  return ((param % (flash_dev->data->sector_size)) != 0) ? (false) : (true);
}


static ARM_FLASH_INFO ARM_OSPI_FLASH0_DEV_DATA =
{
  .sector_info    = NULL,     /* Uniform sector layout */
  .sector_count   = OSPI_FLASH0_TOTAL_SIZE / OSPI_FLASH0_SECTOR_SIZE,
  .sector_size    = OSPI_FLASH0_SECTOR_SIZE,
  .page_size      = OSPI_FLASH0_PAGE_SIZE,
  .program_unit   = OSPI_FLASH0_PROG_UNIT, /* Minimum write size in bytes */
  .erased_value   = OSPI_FLASH0_ERASED_VAL
};

static struct arm_ospi_flash_dev_t ARM_OSPI_FLASH0_DEV =
{
  .dev    = &(OSPI_FLASH0_DEV),
  .data   = &(ARM_OSPI_FLASH0_DEV_DATA)
};

/* Flash Status */
static ARM_FLASH_STATUS ARM_OSPI_FLASH0_STATUS = {0, 0, 0};

static ARM_DRIVER_VERSION Ospi_Flash_GetVersion(void)
{
  return DriverVersion;
}

static ARM_FLASH_CAPABILITIES Ospi_Flash_GetCapabilities(void)
{
  return DriverCapabilities;
}

static int32_t Ospi_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
  ARG_UNUSED(cb_event);
  BSP_OSPI_NOR_Init_t flash;

  /* OSPI device configuration:
     OPI mode, DTR rate: instruction, address and data on eight lines with
     sampling on both edges of clock.
   */
  flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  if (BSP_OSPI_NOR_Init(0, &flash) != BSP_ERROR_NONE)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
  else
  {
    return ARM_DRIVER_OK;
  }
}

static int32_t Ospi_Flash_Uninitialize(void)
{
  if (BSP_OSPI_NOR_DeInit(0) != BSP_ERROR_NONE)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
  else
  {
    return ARM_DRIVER_OK;
  }
}

static int32_t Ospi_Flash_PowerControl(ARM_POWER_STATE state)
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

static int32_t Ospi_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
  int32_t err = BSP_ERROR_NONE;
  uint8_t data_tmp[2];

  ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;

#ifdef DEBUG_OSPI_FLASH_ACCESS
  printf("read ospi 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif /*  DEBUG_OSPI_FLASH_ACCESS */

  /* Check Flash memory boundaries */
  if (!is_range_valid(&ARM_OSPI_FLASH0_DEV, addr + cnt - 1))
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("read ospi not allowed 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif
    ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;

  /* ensure previous operation is finished (erase/write) : GetStatus()
     such verification is done (inside BSP driver) at the begining of erase or write operation  but
     not for read operation ==> in order to maximise BSP driver execution timing efficency */
  while (BSP_OSPI_NOR_GetStatus(0) !=  BSP_ERROR_NONE)
  {
  }

  /* OSPI DTR mode constraint: split read request to ensure read at
   * even address with even number of bytes.
   * Flash address to read is the offset from begin of external flash.
   */
  if (addr % 2)
  {
    err = BSP_OSPI_NOR_Read(0, (uint8_t *)data_tmp, addr - 1, 2);
    *(uint8_t*)data = data_tmp[1];

    if (cnt > 1)
    {
      if (cnt % 2)
      {
        if (err == BSP_ERROR_NONE)
        {
          err = BSP_OSPI_NOR_Read(0, (uint8_t *)data + 1, (addr + 1), cnt - 1);
        }
      }
      else
      {
        if (err == BSP_ERROR_NONE)
        {
          err = BSP_OSPI_NOR_Read(0, (uint8_t *)data + 1, (addr + 1), cnt - 2);
        }

        if (err == BSP_ERROR_NONE)
        {
          err = BSP_OSPI_NOR_Read(0, (uint8_t *)data_tmp, addr + cnt - 1, 2);
          *((uint8_t*)data + cnt - 1) = data_tmp[0];
        }
      }
    }
  }
  else
  {
    if (cnt % 2)
    {
      if (cnt > 1)
      {
        err = BSP_OSPI_NOR_Read(0, (uint8_t *)data, addr, cnt - 1);
      }

      if (err == BSP_ERROR_NONE)
      {
        err = BSP_OSPI_NOR_Read(0, (uint8_t *)data_tmp, addr + cnt - 1, 2);
        *((uint8_t*)data + cnt -1) = data_tmp[0];
      }
    }
    else
    {
      err = BSP_OSPI_NOR_Read(0, (uint8_t *)data, addr, cnt);
    }
  }

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_IDLE;

  if (err != BSP_ERROR_NONE)
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("failed read ospi 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif /* DEBUG_OSPI_FLASH_ACCESS */
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

static int32_t Ospi_Flash_ProgramData(uint32_t addr,
                                      const void *data, uint32_t cnt)
{
  int32_t err;

  ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;

#ifdef DEBUG_OSPI_FLASH_ACCESS
  printf("write ospi 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif /* DEBUG_OSPI_FLASH_ACCESS */
  /* Check Flash memory boundaries and alignment with minimum write size
   * (program_unit), data size also needs to be a multiple of program_unit.
   */
  if ((!is_range_valid(&ARM_OSPI_FLASH0_DEV, addr + cnt - 1)) ||
      (!is_write_aligned(&ARM_OSPI_FLASH0_DEV, addr))     ||
      (!is_write_aligned(&ARM_OSPI_FLASH0_DEV, cnt))      ||
      (!is_write_allow(&ARM_OSPI_FLASH0_DEV, addr, cnt))
     )
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("write ospi not allowed 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif
    ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;

  /* ospi flash address to write is the offset from begin of external flash */
  err = BSP_OSPI_NOR_Write(0, (uint8_t *) data, addr, cnt);

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_IDLE;

  if (err != BSP_ERROR_NONE)
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("failed write ospi 0x%x n=%x \r\n", (addr + OSPI_FLASH_BASE_ADDRESS), cnt);
#endif /* DEBUG_OSPI_FLASH_ACCESS */
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

static int32_t Ospi_Flash_EraseSector(uint32_t addr)
{
  int32_t err;

  ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_NO_ERROR;

#ifdef DEBUG_OSPI_FLASH_ACCESS
  printf("erase ospi 0x%x\r\n", (addr + OSPI_FLASH_BASE_ADDRESS));
#endif /* DEBUG_OSPI_FLASH_ACCESS */
  if (!(is_range_valid(&ARM_OSPI_FLASH0_DEV, addr)) ||
      !(is_erase_aligned(&ARM_OSPI_FLASH0_DEV, addr)) ||
      !(is_erase_allow(&ARM_OSPI_FLASH0_DEV, addr)))
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("erase ospi not allowed 0x%x\r\n", (addr + OSPI_FLASH_BASE_ADDRESS));
#endif
    ARM_OSPI_FLASH0_STATUS.error = DRIVER_STATUS_ERROR;
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;

  if (ARM_OSPI_FLASH0_DEV.data->sector_size == MX25LM51245G_SUBSECTOR_4K)
  {
    err = BSP_OSPI_NOR_Erase_Block(0, addr, MX25LM51245G_ERASE_4K);
  }
  else if (ARM_OSPI_FLASH0_DEV.data->sector_size == MX25LM51245G_SECTOR_64K)
  {
    err = BSP_OSPI_NOR_Erase_Block(0, addr, MX25LM51245G_ERASE_64K);
  }
  else
  {
    err = BSP_ERROR_WRONG_PARAM;
  }

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_IDLE;

  if (err != BSP_ERROR_NONE)
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("erase ospi failed 0x%x\r\n", (addr + OSPI_FLASH_BASE_ADDRESS));
#endif
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

static int32_t Ospi_Flash_EraseChip(void)
{
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_FLASH_STATUS Ospi_Flash_GetStatus(void)
{
  return ARM_OSPI_FLASH0_STATUS;
}

static ARM_FLASH_INFO *Ospi_Flash_GetInfo(void)
{
  return ARM_OSPI_FLASH0_DEV.data;
}

ARM_DRIVER_FLASH TFM_Driver_OSPI_FLASH0 =
{
  Ospi_Flash_GetVersion,
  Ospi_Flash_GetCapabilities,
  Ospi_Flash_Initialize,
  Ospi_Flash_Uninitialize,
  Ospi_Flash_PowerControl,
  Ospi_Flash_ReadData,
  Ospi_Flash_ProgramData,
  Ospi_Flash_EraseSector,
  Ospi_Flash_EraseChip,
  Ospi_Flash_GetStatus,
  Ospi_Flash_GetInfo
};

/**
  * @brief This function configures the ospi flash in execution mode.
  * @note
  * @retval execution_status
  */
int32_t Ospi_Flash_Config_Exe(void)
{
  int32_t err;

#ifdef DEBUG_OSPI_FLASH_ACCESS
  printf("memory mapped ospi\r\n");
#endif /*  DEBUG_OSPI_FLASH_ACCESS */

  ARM_OSPI_FLASH0_STATUS.busy = DRIVER_STATUS_BUSY;

  /* ensure previous operation is finished (erase/write) : GetStatus()
     such verification is done (inside BSP driver) at the begining of erase or write operation  but
     not for read operation ==> in order to maximise BSP driver execution timing efficency */
  while (BSP_OSPI_NOR_GetStatus(0) !=  BSP_ERROR_NONE)
  {
  }

  /* Enable memory map mode */
  err = BSP_OSPI_NOR_EnableMemoryMappedMode(0);

  if (err != BSP_ERROR_NONE)
  {
#ifdef DEBUG_OSPI_FLASH_ACCESS
    printf("failed memory mapped ospi\r\n");
#endif /* DEBUG_OSPI_FLASH_ACCESS */
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}
