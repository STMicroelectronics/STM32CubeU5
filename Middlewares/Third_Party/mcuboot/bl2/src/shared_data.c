/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <string.h>
#include "bootutil/boot_record.h"
#include "bootutil/boot_status.h"
#include "bootutil/image.h"
#include "flash_map/flash_map.h"
#include "sysflash/sysflash.h"
#include "mcuboot_config/mcuboot_config.h"
#ifdef TFM_MEASURED_BOOT_API
#include "boot_hal.h"
#include "boot_measurement.h"
#include "bootutil/crypto/sha.h"
#include "bootutil_priv.h"
#include "psa/crypto.h"
#if defined(MCUBOOT_SIGN_EC384)
#define MCUBOOT_HASH_ALG    PSA_ALG_SHA_384
#define MCUBOOT_HASH_SIZE   (48)
#else
#define MCUBOOT_HASH_ALG    PSA_ALG_SHA_256
#define MCUBOOT_HASH_SIZE   (32)
#endif /* MCUBOOT_SIGN_EC384 */
#ifdef MCUBOOT_HW_KEY
#if defined(MCUBOOT_SIGN_RSA)
#define SIG_BUF_SIZE (MCUBOOT_SIGN_RSA_LEN / 8)
#define SIG_EXTRA_BYTES     (24) /* Few extra bytes for encoding and for public exponent. */
#elif defined(MCUBOOT_SIGN_EC256)
#define SIG_BUF_SIZE        (64) /* Curve byte (32) * 2 for EC-256 */
#define SIG_EXTRA_BYTES     (32)
#elif defined(MCUBOOT_SIGN_EC384)
#define SIG_BUF_SIZE        (96) /* Curve byte (48) * 2 for EC-384 */
#define SIG_EXTRA_BYTES     (48)
#endif /* MCUBOOT_SIGN_RSA */
#endif /* MCUBOOT_HW_KEY */
#endif /* TFM_MEASURED_BOOT_API */
/* Firmware Update specific macros */
#define TLV_MAJOR_FWU   0x2
#define MODULE_MASK     0x3F           /* 6 bit */
#define CLAIM_MASK      0x3F           /* 6 bit */
#define SET_FWU_MINOR(sw_module, claim)  \
                     ((uint16_t)((sw_module & MODULE_MASK) << 6) | \
                      (uint16_t)(claim & CLAIM_MASK))
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
extern int boot_add_data_to_shared_area(uint8_t        major_type,
                                        uint16_t       minor_type,
                                        size_t         size,
                                        const uint8_t *data);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */
#ifdef TFM_MEASURED_BOOT_API
/**
 * Collect boot measurement and available associated metadata from the
 * TLV area of an image.
 *
 * @param[in]  hdr        Pointer to the image header stored in RAM.
 * @param[in]  fap        Pointer to the flash area where image is stored.
 * @param[out] metadata   Pointer to measurement metadata structure.
 * @param[out] measurement_buf          Buffer to store the boot measurement.
 * @param[in]  measurement_buf_size     As an input value it indicates the size
 *                                      of the measurement buffer in bytes.
 *
 * @return                0 on success; nonzero on failure.
 *
 */
static int collect_image_measurement_and_metadata(
                                    const struct image_header *hdr,
                                    const struct flash_area *fap,
                                    struct boot_measurement_metadata *metadata,
                                    uint8_t *measurement_buf,
                                    size_t   measurement_buf_size)
{
    struct image_tlv_iter it;
    uint32_t off;
    uint16_t len;
    uint16_t type;
#ifdef MCUBOOT_HW_KEY
    /* Few extra bytes for encoding and for public exponent. */
    uint8_t key_buf[SIG_BUF_SIZE + SIG_EXTRA_BYTES];
    bootutil_sha_context sha_ctx;
#endif
    int rc;
    /* Copy the software version information from the image header. */
    metadata->sw_version.major = hdr->ih_ver.iv_major;
    metadata->sw_version.minor = hdr->ih_ver.iv_minor;
    metadata->sw_version.revision = hdr->ih_ver.iv_revision;
    metadata->sw_version.build_num = hdr->ih_ver.iv_build_num;
    /* Traverse through all of the TLVs for the required items. */
    rc = bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ANY, false);
    if (rc) {
        return rc;
    }
    while (true) {
        rc = bootutil_tlv_iter_next(&it, &off, &len, &type);
        if (rc < 0) {
            return -1;
        } else if (rc > 0) {
            break;
        }
        if (type == EXPECTED_HASH_TLV) {
            /* Retrieve the image hash (measurement value) from the TLV area. */
            if (len > measurement_buf_size) {
                return -1;
            }
            rc = LOAD_IMAGE_DATA(hdr, fap, off, measurement_buf, len);
            if (rc) {
                return -1;
            }
#ifdef MCUBOOT_HW_KEY
        } else if (type == IMAGE_TLV_PUBKEY) {
            /* Retrieve the signer ID (hash of PUBKEY) from the TLV area. */
            if (len > sizeof(key_buf)) {
                /* Something is wrong with the public key, proceed without
                 * the signer ID.
                 */
                continue;
            }
            rc = LOAD_IMAGE_DATA(hdr, fap, off, key_buf, len);
            if (rc) {
                /* Proceed without this piece of data. */
                continue;
            }
            /* Calculate the hash of the public key (signer ID). */
            bootutil_sha_init(&sha_ctx);
            bootutil_sha_update(&sha_ctx, key_buf, len);
            bootutil_sha_finish(&sha_ctx, metadata->signer_id);
#else
        } else if (type == IMAGE_TLV_KEYHASH) {
            /* Retrieve the signer ID (hash of PUBKEY) from the TLV area. */
            if (len != MCUBOOT_HASH_SIZE) {
                /* Something is wrong with the key hash, proceed without
                 * the signer ID.
                 */
                continue;
            }
            rc = LOAD_IMAGE_DATA(hdr, fap, off,
                                 metadata->signer_id, MCUBOOT_HASH_SIZE);
            if (rc) {
                /* Proceed without this piece of data. */
                continue;
            }
#endif /* MCUBOOT_HW_KEY */
            metadata->signer_id_size = MCUBOOT_HASH_SIZE;
        }
    }
    return 0;
}
#endif /* TFM_MEASURED_BOOT_API */
/**
 * Add application specific data to the shared memory area between the
 * bootloader and runtime SW.
 *
 * @param[in]  hdr           Pointer to the image header stored in RAM.
 * @param[in]  fap           Pointer to the flash area where image is stored.
 *
 * @return                0 on success; nonzero on failure.
 */
int boot_save_shared_data(const struct image_header *hdr,
                          const struct flash_area *fap)
{
    const struct flash_area *temp_fap;
    uint8_t mcuboot_image_id = 0;
    uint8_t i;
    int32_t rc;
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    struct image_version image_ver;
    uint16_t fwu_minor;
#endif
#ifdef TFM_MEASURED_BOOT_API
    enum boot_measurement_slot_t slot_id;
    uint8_t image_hash[MCUBOOT_HASH_SIZE];
    struct boot_measurement_metadata metadata = {
        .measurement_type = MCUBOOT_HASH_ALG,
        .signer_id = { 0 },
        .signer_id_size = 0,
        .sw_type = "",
        .sw_version = { 0 },
    };
#endif /* TFM_MEASURED_BOOT_API */
    if (hdr == NULL || fap == NULL) {
        return -1;
    }
    /* Look for the given flash area to determine the image ID. */
    for (i = 0; i < MCUBOOT_IMAGE_NUMBER; i++) {
        if (flash_area_open(FLASH_AREA_IMAGE_PRIMARY(i), &temp_fap) == 0 &&
            fap == temp_fap) {
            mcuboot_image_id = i;
            break;
        }
#if defined(MCUBOOT_DIRECT_XIP) || defined(MCUBOOT_RAM_LOAD)
        else if (flash_area_open(FLASH_AREA_IMAGE_SECONDARY(i), &temp_fap) == 0 &&
                 fap == temp_fap) {
            mcuboot_image_id = i;
            break;
        }
#endif
    }
    if (i == MCUBOOT_IMAGE_NUMBER) {
        return -1;
    }
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    image_ver = hdr->ih_ver;
    /* Currently hardcode it to 0 which indicates the full image. */
    fwu_minor = SET_FWU_MINOR(mcuboot_image_id, SW_VERSION);
    rc = boot_add_data_to_shared_area(TLV_MAJOR_FWU,
                                      fwu_minor,
                                      sizeof(image_ver),
                                      (const uint8_t *)&image_ver);
    if (rc) {
        return rc;
    }
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */
#ifdef TFM_MEASURED_BOOT_API
    /* Determine the index of the measurement slot. */
    slot_id = BOOT_MEASUREMENT_SLOT_RT_0 + mcuboot_image_id;
    switch (slot_id) {
    case BOOT_MEASUREMENT_SLOT_RT_0:
        if (sizeof(metadata.sw_type) < sizeof("RT_0")) {
            return 1;
        }
        memcpy(metadata.sw_type, "RT_0", sizeof("RT_0"));
        break;
    case BOOT_MEASUREMENT_SLOT_RT_1:
        if (sizeof(metadata.sw_type) < sizeof("RT_1")) {
            return 1;
        }
        memcpy(metadata.sw_type, "RT_1", sizeof("RT_1"));
        break;
    case BOOT_MEASUREMENT_SLOT_RT_2:
        if (sizeof(metadata.sw_type) < sizeof("RT_2")) {
            return 1;
        }
        memcpy(metadata.sw_type, "RT_2", sizeof("RT_2"));
        break;
    default:
        /* Proceed without this piece of data. */
        break;
    }
    rc = collect_image_measurement_and_metadata(hdr, fap,
                                                &metadata,
                                                image_hash,
                                                sizeof(image_hash));
    if (rc) {
        return rc;
    }
    /* Save the boot measurement(s) about the runtime image(s).
     * If there are multiple images, the measurement slot will be extended
     * with the subsequent measurements.
     */
    rc = boot_store_measurement((uint8_t)slot_id, image_hash,
                                sizeof(image_hash), &metadata, false);
    if (rc) {
        return rc;
    }
#endif /* TFM_MEASURED_BOOT_API */
    return 0;
}
