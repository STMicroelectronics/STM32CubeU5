/*
 *  Copyright (C) 2017, Linaro Ltd
 *  Copyright (c) 2018-2020, Arm Limited.
 *
 *  SPDX-License-Identifier: Apache-2.0
 */

/*
 * Original code taken from mcuboot project at:
 * https://github.com/mcu-tools/mcuboot
 * Git SHA of the original version: ac55554059147fff718015be9f4bd3108123f50a
 */

#ifndef H_TARGETS_TARGET_
#define H_TARGETS_TARGET_

/* Target specific defines: flash partitions; flash driver name, etc.
 * Comes from: platform/ext/target/<BOARD>/<SUBSYSTEM>/partition
 */
#include "flash_layout.h"

#ifndef FLASH_BASE_ADDRESS
#error "FLASH_BASE_ADDRESS must be defined by the target"
#endif

#ifndef FLASH_AREA_IMAGE_SECTOR_SIZE
#error "FLASH_AREA_IMAGE_SECTOR_SIZE must be defined by the target"
#endif

#ifdef MCUBOOT_RAM_LOAD
#ifndef IMAGE_EXECUTABLE_RAM_START
#error "If MCUBOOT_RAM_LOAD is set then IMAGE_EXECUTABLE_RAM_START must be \
defined by the target"
#endif

#ifndef IMAGE_EXECUTABLE_RAM_SIZE
#error "If MCUBOOT_RAM_LOAD is set then IMAGE_EXECUTABLE_RAM_SIZE must be \
defined by the target"
#endif
#endif /* MCUBOOT_RAM_LOAD */

#ifndef FLASH_AREA_0_OFFSET
#error "FLASH_AREA_0_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_0_SIZE
#error "FLASH_AREA_0_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_0) != defined(FLASH_DEVICE_ID_0)
#error "FLASH DEV_NAME_0 and DEVICE_ID_0 must be simultaneously defined or not \
by target"
#endif
#ifndef MCUBOOT_PRIMARY_ONLY
#ifndef FLASH_AREA_2_OFFSET
#error "FLASH_AREA_2_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_2_SIZE
#error "FLASH_AREA_2_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_2) != defined(FLASH_DEVICE_ID_2)
#error "FLASH DEV_NAME_2 and DEVICE_ID_2 must be simultaneously defined or not \
by target"
#endif
#endif
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#ifndef FLASH_AREA_1_OFFSET
#error "FLASH_AREA_1_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_1_SIZE
#error "FLASH_AREA_1_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_1) != defined(FLASH_DEVICE_ID_1)
#error "FLASH DEV_NAME_1 and DEVICE_ID_1 must be simultaneously defined or not \
by target"
#endif

#ifndef MCUBOOT_PRIMARY_ONLY
#ifndef FLASH_AREA_3_OFFSET
#error "FLASH_AREA_3_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_3_SIZE
#error "FLASH_AREA_3_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_3) != defined(FLASH_DEVICE_ID_3)
#error "FLASH DEV_NAME_3 and DEVICE_ID_3 must be simultaneously defined or not \
by target"
#endif
#endif
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */

#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#ifndef FLASH_AREA_4_OFFSET
#error "FLASH_AREA_4_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_4_SIZE
#error "FLASH_AREA_4_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_4) != defined(FLASH_DEVICE_ID_4)
#error "FLASH DEV_NAME_4 and DEVICE_ID_4 must be simultaneously defined or not \
by target"
#endif

#ifndef MCUBOOT_PRIMARY_ONLY
#ifndef FLASH_AREA_6_OFFSET
#error "FLASH_AREA_6_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_6_SIZE
#error "FLASH_AREA_6_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_6) != defined(FLASH_DEVICE_ID_6)
#error "FLASH DEV_NAME_6 and DEVICE_ID_6 must be simultaneously defined or not \
by target"
#endif
#endif
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */

#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#ifndef FLASH_AREA_5_OFFSET
#error "FLASH_AREA_5_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_5_SIZE
#error "FLASH_AREA_5_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_5) != defined(FLASH_DEVICE_ID_5)
#error "FLASH DEV_NAME_5 and DEVICE_ID_5 must be simultaneously defined or not \
by target"
#endif

#ifndef MCUBOOT_PRIMARY_ONLY
#ifndef FLASH_AREA_7_OFFSET
#error "FLASH_AREA_7_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_7_SIZE
#error "FLASH_AREA_7_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_7) != defined(FLASH_DEVICE_ID_7)
#error "FLASH DEV_NAME_7 and DEVICE_ID_7 must be simultaneously defined or not \
by target"
#endif
#endif
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */

#ifndef MCUBOOT_PRIMARY_ONLY
#ifndef FLASH_AREA_SCRATCH_OFFSET
#error "FLASH_AREA_SCRATCH_OFFSET must be defined by the target"
#endif

#ifndef FLASH_AREA_SCRATCH_SIZE
#error "FLASH_AREA_SCRATCH_SIZE must be defined by the target"
#endif

#if defined(FLASH_DEV_NAME_SCRATCH) != defined(FLASH_DEVICE_ID_SCRATCH)
#error "FLASH DEV_NAME_SCRATCH and DEVICE_ID_SCRATCH must be simultaneously defined \
or not by target"
#endif
#endif
#ifndef FLASH_DEV_NAME
#error "BL2 supports CMSIS flash interface and device name must be specified"
#endif

#ifndef MCUBOOT_STATUS_MAX_ENTRIES
#error "MCUBOOT_STATUS_MAX_ENTRIES must be defined by the target"
#endif

#ifndef MCUBOOT_MAX_IMG_SECTORS
#error "MCUBOOT_MAX_IMG_SECTORS must be defined by the target"
#endif

#endif /* H_TARGETS_TARGET_ */
