/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nv_counters_device.h"
#include "tfm_plat_nv_counters.h"
#include <limits.h>
#include <stddef.h>
#include "Driver_Flash.h"
#include "nv_counters.h"


/* Compilation time checks to be sure the defines are well defined */
#ifndef DEVICE_NV_COUNTERS_AREA_SIZE
#error "DEVICE_NV_COUNTERS_AREA_SIZE must be defined"
#endif

#ifndef DEVICE_NV_COUNTERS_AREA_OFFSET
#error "DEVICE_NV_COUNTERS_AREA_OFFSET must be defined"
#endif

#ifndef DEVICE_NV_COUNTERS_SECTOR_SIZE
#error "DEVICE_NV_COUNTERS_SECTOR_SIZE must be defined"
#endif

#if DEVICE_NV_COUNTERS_AREA_SIZE!=(2*DEVICE_NV_COUNTERS_SECTOR_SIZE)
#error "DEVICE_NV_COUNTERS_AREA_SIZE !=  2 x DEVICE_NV_COUNTER_SIZE"
#endif

#ifndef DEVICE_NV_COUNTERS_FLASH_NAME
#error "DEVICE_NV_COUNTERS_FLASH_NAME must be defined"
#endif

#ifndef DEVICE_NUM_NV_COUNTERS
#error "DEVICE_NUM_NV_COUNTERS"
#endif
/* End of compilation time checks to be sure the defines are well defined */

#define SECTOR_OFFSET    0
#define NV_COUNTER_SIZE  sizeof(uint32_t)
#define INIT_VALUE_SIZE  sizeof(uint32_t)
#define CHECKSUM_SIZE    sizeof(uint32_t)
#define NUM_NV_COUNTERS  (DEVICE_NUM_NV_COUNTERS)


#define BACKUP_ADDRESS (DEVICE_NV_COUNTERS_AREA_OFFSET + DEVICE_NV_COUNTERS_SECTOR_SIZE)
#define VALID_ADDRESS  (DEVICE_NV_COUNTERS_AREA_OFFSET)

/* Import the CMSIS flash device driver */
extern ARM_DRIVER_FLASH DEVICE_NV_COUNTERS_FLASH_NAME;

/**
  * @brief  Software CRC16 calculation.
  * @retval uint16_t CRC intermediate value
  */
static uint16_t crc_update(uint16_t crc_in, uint8_t incr)
{
  uint16_t xor = crc_in >> 15;
  uint16_t out = crc_in << 1;

  if (incr)
  {
    out++;
  }
  if (xor)
  {
    out ^= 0x1021;
  }
  return out;
}

/**
  * @brief  Software CRC16 calculation.
  * @param  pdata: pointer to data buffer.
  * @param  size: buffer length in bytes.
  * @retval uint16_t CRC
  */
static uint16_t crc16(const uint8_t *pdata, uint16_t size)
{
  uint16_t crc;
  uint8_t i;

  for (crc = 0; size > 0; size--, pdata++)
  {
    for (i = 0x80; i; i >>= 1)
    {
      crc = crc_update(crc, *pdata & i);
    }
  }
  for (i = 0; i < 16; i++)
  {
    crc = crc_update(crc, 0);
  }
  return crc;
}

static uint16_t calc_crc(const uint8_t *data, size_t len)
{
    return crc16((uint8_t *)data, len);
}

static bool is_valid(const struct nv_counters_t *nv_counters)
{
    return ((nv_counters->init_value == NV_COUNTERS_INITIALIZED) &&
            (!calc_crc((uint8_t*)&nv_counters->crc, sizeof(*nv_counters))));
}

static void set_crc(struct nv_counters_t *nv_counters)
{
    uint16_t crc = calc_crc((uint8_t*)&nv_counters->init_value,
                                 sizeof(*nv_counters)
                                  - sizeof(nv_counters->crc));

    nv_counters->crc = (uint32_t)crc;
}

enum tfm_plat_err_t tfm_plat_init_nv_counter(void)
{
    int32_t  err;
    uint32_t i;
    struct nv_counters_t nv_counters;

    err = DEVICE_NV_COUNTERS_FLASH_NAME.Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Read the whole sector so we can write it back to flash later */
    err = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(VALID_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t));
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (is_valid(&nv_counters)) {
        return TFM_PLAT_ERR_SUCCESS;
    }

    /* Check the backup watermark */
    err = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(BACKUP_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t));
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Erase sector before writing to it */
    err = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(VALID_ADDRESS);
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (is_valid(&nv_counters)) {
        /* Copy from the backup to the main */
        err = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t));
        if (err != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        return TFM_PLAT_ERR_SUCCESS;
    }

    /* Add watermark to indicate that NV counters have been initialized */
    nv_counters.init_value = NV_COUNTERS_INITIALIZED;

    /* Initialize all counters to 0 */
    for (i = 0; i < NUM_NV_COUNTERS; i++) {
        nv_counters.counters[i] = 0;
    }

    set_crc(&nv_counters);
    /* Write the in-memory block content after modification to flash */
    err = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                &nv_counters,
                                                sizeof(struct nv_counters_t));
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                             uint32_t size, uint8_t *val)
{
    int32_t  err;
    uint32_t flash_addr = VALID_ADDRESS
                           + offsetof(struct nv_counters_t, counters)
                           + (counter_id * NV_COUNTER_SIZE);

    if (size != NV_COUNTER_SIZE) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    err = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(flash_addr, val, NV_COUNTER_SIZE);
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                            uint32_t value)
{
    int32_t  err;
    struct nv_counters_t nv_counters;

    /* Read the whole sector so we can write it back to flash later */
    err = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(VALID_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t));
    if (err != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (value != nv_counters.counters[counter_id]) {

        if (value < nv_counters.counters[counter_id]) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        /* Erase backup sector */
        err = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(BACKUP_ADDRESS);
        if (err != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        nv_counters.counters[counter_id] = value;

        set_crc(&nv_counters);

        /* write sector data to backup sector */
        err = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(BACKUP_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t));
        if (err != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Erase sector before writing to it */
        err = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(VALID_ADDRESS);
        if (err != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Write the in-memory block content after modification to flash */
        err = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t));
        if (err != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_increment_nv_counter(
                                           enum tfm_nv_counter_t counter_id)
{
    uint32_t security_cnt;
    enum tfm_plat_err_t err;

    err = tfm_plat_read_nv_counter(counter_id,
                                   sizeof(security_cnt),
                                   (uint8_t *)&security_cnt);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (security_cnt == UINT32_MAX) {
        return TFM_PLAT_ERR_MAX_VALUE;
    }

    return tfm_plat_set_nv_counter(counter_id, security_cnt + 1u);
}