/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Cypress Semiconductor Corporation. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 *
 * Flash layout on nucleo_l552ze_q with BL2 (multiple image boot):
 *
 * 0x0000_0000 BL2 - MCUBoot (72 KB)
 * 0x0000_e000 NV counters area (4 KB)
 * 0x0000_f000 Secure Storage Area (8 KB)
 * 0x0001_1000 Internal Trusted Storage Area (8 KB)
 * 0x0001_3000 Secure image     primary slot (180 KB)
 * 0x0004_0000 Non-secure image primary slot (36 KB)
 * 0x0004_9000 Secure image     secondary slot (180 KB)
 * 0x0007_6000 Non-secure image secondary slot (36 KB)
 * 0x0007_F000 Unused (4 KB)
 *
 * Flash layout on nucleo_l552ze_q with BL2 (multiple image boot, layout for test):
 * No Firmware update , ITS, PS in RAM.
 * 0x0000_0000 BL2 - MCUBoot (72 KB)
 * 0x0000_e000 NV counters area (4 KB)
 * 0x0000_f000 Secure Storage Area (8 KB)
 * 0x0001_1000 Internal Trusted Storage Area (8 KB)
 * 0x0001_3000 Secure image     primary slot (224 KB)
 * 0x0004_b000 Non-secure image primary slot (168 KB)
 * 0x0007_5000 Unused (44K)
 * The size of a partition. This should be large enough to contain a S or NS
 * sw binary. Each FLASH_AREA_IMAGE contains two partitions. See Flash layout
 * above.
 */

/* Sector size of the flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)   /* 4KB */

#define FLASH_TOTAL_SIZE                (0x80000) /* 512Kbytes*/

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x0c000000) /* same as FLASH0_BASE_S */

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */

/* area for BL2 code protected by hdp */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#define FLASH_AREA_BL2_SIZE             (0xd800)
/* HDP area end at this address */
#define FLASH_BL2_HDP_END               (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE-1)
/* area for BL2 code not protected by hdp */
#define FLASH_AREA_BL2_NOHDP_OFFSET     (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_BL2_NOHDP_SIZE       (0x800)

/* scratch area */
#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_SIZE)
#define FLASH_AREA_SCRATCH_SIZE         (0x2000)
/* according to test flash layout can change */
#if defined(FLASH_LAYOUT_FOR_TEST) || defined(TEST_FRAMEWORK_S) || defined(TEST_FRAMEWORK_NS)
/* Non Volatile Counters definitions */
#define FLASH_NV_COUNTERS_SECTOR_SIZE      (0x1000)
/* fix me with overwrite scratch is not required */
#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_AREA_SCRATCH_OFFSET)

/* fix me with test config PS and ITS in RAM */
/* Secure Storage (PS) Service definitions */
#define FLASH_PS_AREA_SIZE             (0x2000)
#define FLASH_PS_AREA_OFFSET           (FLASH_NV_COUNTERS_AREA_OFFSET+FLASH_NV_COUNTERS_SECTOR_SIZE)

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET+FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0x2000)   /* 8 KB */

#define FLASH_S_PARTITION_SIZE          (0x38000) /* S partition */
#define FLASH_NS_PARTITION_SIZE         (0x2A000) /* NS partition */
#define FLASH_PARTITION_SIZE (FLASH_S_PARTITION_SIZE+FLASH_NS_PARTITION_SIZE)
/* Secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_OFFSET             (FLASH_ITS_AREA_OFFSET+FLASH_ITS_AREA_SIZE)
#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)
#else
/* Non Volatile Counters definitions */
#define FLASH_NV_COUNTERS_SECTOR_SIZE      (0x1000)
/* fix me with overwrite scratch is not required */
#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_AREA_SCRATCH_OFFSET)

/* fix me with test config PS and ITS in RAM */
/* Secure Storage (PS) Service definitions */
#define FLASH_PS_AREA_SIZE             (0x2000)
#define FLASH_PS_AREA_OFFSET           (FLASH_NV_COUNTERS_AREA_OFFSET+FLASH_NV_COUNTERS_SECTOR_SIZE)

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET+FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0x2000)   /* 8 KB */

#define FLASH_S_PARTITION_SIZE          (0x2D000) /* S partition */
#define FLASH_NS_PARTITION_SIZE         (0x9000) /* NS partition */
#define FLASH_PARTITION_SIZE (FLASH_S_PARTITION_SIZE+FLASH_NS_PARTITION_SIZE)
/* Secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_OFFSET             (FLASH_ITS_AREA_OFFSET+FLASH_ITS_AREA_SIZE)
#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)
#endif
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE               (FLASH_NS_PARTITION_SIZE)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE               (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID                 (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET             (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE               (FLASH_NS_PARTITION_SIZE)

#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_3_ID + 1)

/*
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES         ((2 * FLASH_PARTITION_SIZE) / \
                                         FLASH_AREA_SCRATCH_SIZE)
/** Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS           ((2 * FLASH_PARTITION_SIZE) / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)

#define FLASH_NV_COUNTERS_AREA_SIZE     (0x18)     /* 16 Bytes */

#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2 and NV Counter
  * Name is defined in flash driver file: Driver_Flash.c
  */

#define FLASH_DEV_NAME TFM_Driver_FLASH0

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER TFM_Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR    FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x8)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide.
 */
#define TFM_HAL_ITS_FLASH_DRIVER TFM_Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR    FLASH_ITS_AREA_OFFSET
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE    FLASH_ITS_AREA_SIZE
#define ITS_RAM_FS_SIZE                TFM_HAL_ITS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x8)

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    (0x18)/* 24 Bytes */
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_AREA_IMAGE_SECTOR_SIZE

/* BL2 NV Counters definitions  */
#define BL2_NV_COUNTERS_AREA_ADDR    FLASH_BL2_NVCNT_AREA_OFFSET
#define BL2_NV_COUNTERS_AREA_SIZE    FLASH_BL2_NVCNT_AREA_SIZE

#define BL2_S_RAM_ALIAS_BASE  (0x30000000)
#define BL2_NS_RAM_ALIAS_BASE (0x20000000)

/*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)
#define BOOT_TFM_SHARED_DATA_BASE (0x3003fc00)

#endif /* __FLASH_LAYOUT_H__ */
