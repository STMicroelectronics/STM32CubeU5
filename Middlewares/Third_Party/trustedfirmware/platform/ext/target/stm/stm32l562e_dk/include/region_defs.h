/*
 * Copyright (c) 2017-2018 ARM Limited
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__
#include "flash_layout.h"

#define BL2_HEAP_SIZE           0x0001000
#define BL2_MSP_STACK_SIZE      0x0001800

#define S_HEAP_SIZE             0x0001000
#define S_MSP_STACK_SIZE_INIT   0x0000400
#define S_MSP_STACK_SIZE        0x0000800
#define S_PSP_STACK_SIZE        0x0000800

#define NS_HEAP_SIZE            0x0001000
#define NS_MSP_STACK_SIZE       0x0000400
#define NS_PSP_STACK_SIZE       0x0000C00


/*
 * This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   0x250

#define _SRAM2_TOP              (0x40000) /* 256Kbytes */
#define _SRAM1_SIZE_MAX         (0x30000)  /*!< SRAM1=192k*/
#define _SRAM2_SIZE_MAX         (0xfc00)  /*!< SRAM2=64k -0x400 */
/* Flash and internal SRAMs base addresses - Non secure aliased */
#define _FLASH_BASE_NS          (0x08000000) /*!< FLASH(up to 512 KB) base address */
#define _SRAM1_BASE_NS          (0x20000000) /*!< SRAM1(up to 192 KB) base address */
#define _SRAM2_BASE_NS          (0x20030000) /*!< SRAM2(64 KB) base address */

/* Flash and internal SRAMs base addresses - Secure aliased */
#define _FLASH_BASE_S           (0x0C000000) /*!< FLASH(up to 512 KB) base address */
#define _SRAM1_BASE_S           (0x30000000) /*!< SRAM1(up to 192 KB) base address */
#define _SRAM2_BASE_S           (0x30030000) /*!< SRAM2(64 KB) base address */


#define TOTAL_ROM_SIZE          FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE          (_SRAM1_SIZE_MAX+_SRAM2_SIZE_MAX )
/* 192 + 64 Kbytes - BOOT info */
/* boot info are placed and locked at top of SRAM2  */
#define S_TOTAL_RAM_SIZE        (0x1fc00 ) /*! size require for Secure part */
#define NS_TOTAL_RAM_SIZE       (TOTAL_RAM_SIZE - S_TOTAL_RAM_SIZE)

#ifdef BL2
#ifndef LINK_TO_SECONDARY_PARTITION
#define  S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_0_OFFSET)
#define  S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET)
#else
#define  S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_2_OFFSET)
#define  S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET)
#endif /* !LINK_TO_SECONDARY_PARTITION */
#else
#define  S_IMAGE_PRIMARY_PARTITION_OFFSET   (0x0)
#endif /* BL2 */



/*
 * Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x7_0000 Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */

#define BL2_HEADER_SIZE                     (0x400)
#define BL2_TRAILER_SIZE                    (0x2000)
#ifdef BL2
#ifndef LINK_TO_SECONDARY_PARTITION
#define S_IMAGE_PRIMARY_PARTITION_OFFSET    (FLASH_AREA_0_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET  (FLASH_AREA_2_OFFSET)
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET    (FLASH_AREA_2_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET  (FLASH_AREA_0_OFFSET)
#endif /* !LINK_TO_SECONDARY_PARTITION */
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET (0x0)
#endif /* BL2 */
#ifndef LINK_TO_SECONDARY_PARTITION
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET + FLASH_S_PARTITION_SIZE)
#else
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET + FLASH_S_PARTITION_SIZE)
#endif /* !LINK_TO_SECONDARY_PARTITION */

#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#define IMAGE_NS_CODE_SIZE \
            (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)


#define CMSE_VENEER_REGION_SIZE             (0x00000340)

/* Use SRAM1 memory to store Code data */
#define S_ROM_ALIAS_BASE                    (_FLASH_BASE_S)
#define NS_ROM_ALIAS_BASE                   (_FLASH_BASE_NS)


#define S_RAM_ALIAS_BASE                    (_SRAM1_BASE_S)
#define NS_RAM_ALIAS_BASE                   (_SRAM1_BASE_NS)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)                      (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x)                     (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)                      (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x)                     (NS_RAM_ALIAS_BASE + (x))


#define S_IMAGE_PRIMARY_AREA_OFFSET         (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define S_CODE_START                        (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))

#define S_CODE_SIZE                         (IMAGE_S_CODE_SIZE - CMSE_VENEER_REGION_SIZE)
#define S_CODE_LIMIT                        ((S_CODE_START + S_CODE_SIZE) -1)

#define S_DATA_START                        (S_RAM_ALIAS(NS_TOTAL_RAM_SIZE))
#define S_DATA_SIZE                         (S_TOTAL_RAM_SIZE)
#define S_DATA_LIMIT                        (S_DATA_START + S_DATA_SIZE - 1)

/* CMSE Veneers region */
#define CMSE_VENEER_REGION_START            (S_CODE_LIMIT + 1)
/* Non-secure regions */

/* Secure regions , the end of secure regions must be aligned on page size for dual bank 0x800*/
/* Offset and size definition in flash area, used by assemble.py
 * 0x11400+0x33c00= 13000+34000 = 45000*/

#define NS_IMAGE_PRIMARY_AREA_OFFSET        (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START                       (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE                        (IMAGE_NS_CODE_SIZE)
#define NS_CODE_LIMIT                       (NS_CODE_START + NS_CODE_SIZE - 1)
#define NS_DATA_START                       (NS_RAM_ALIAS(0))
#define NS_NO_INIT_DATA_SIZE                (0x100)
#define NS_DATA_SIZE                        (NS_TOTAL_RAM_SIZE)
#define NS_DATA_LIMIT                       (NS_DATA_START + NS_DATA_SIZE - 1)

/* NS partition information is used for GTZC and SAU configuration */
#define NS_PARTITION_START                  (NS_CODE_START)
#define NS_PARTITION_SIZE                   (NS_CODE_SIZE)

/* Secondary partition for new images/ in case of firmware upgrade */
#define SECONDARY_PARTITION_START           (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE            (FLASH_AREA_2_SIZE)

#ifdef BL2

/* Bootloader region protected by hdp */
#define BL2_CODE_START                      (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE                       (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT                      (BL2_CODE_START + BL2_CODE_SIZE - 1)

/* Bootloader region not protected by hdp */
#define BL2_NOHDP_CODE_START                (S_ROM_ALIAS(FLASH_AREA_BL2_NOHDP_OFFSET))
#define BL2_NOHDP_CODE_SIZE                 (FLASH_AREA_BL2_NOHDP_SIZE)
#define BL2_NOHDP_CODE_LIMIT                (BL2_NOHDP_CODE_START + BL2_NOHDP_CODE_SIZE - 1)

/*  keep 256 bytes unsed to place while(1) for non secure to enable */
/*  regression from local tool with non secure attachement
 *  This avoid blocking board in case of hardening error */
#define BL2_DATA_START                      (S_RAM_ALIAS(_SRAM1_SIZE_MAX))
#define BL2_DATA_SIZE                       (BOOT_TFM_SHARED_DATA_BASE - BL2_DATA_START)
#define BL2_DATA_LIMIT                      (BL2_DATA_START + BL2_DATA_SIZE - 1)
#endif /* BL2 */

#endif /* __REGION_DEFS_H__ */
