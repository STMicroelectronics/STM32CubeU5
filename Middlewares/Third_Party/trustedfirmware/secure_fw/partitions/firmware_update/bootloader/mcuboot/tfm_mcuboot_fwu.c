/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/crypto.h"
#include "log/tfm_log.h"
#include "bootutil_priv.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "flash_map_backend/flash_map_backend.h"
#include "sysflash/sysflash.h"
#include "tfm_bootloader_fwu_abstraction.h"
#include "tfm_fwu_req_mngr.h"
#include "tfm_boot_status.h"
#include "service_api.h"
#include "tfm_memory_utils.h"
#include "tfm_secure_api.h"
#include "flash_layout.h"

#if !defined(MCUBOOT_PRIMARY_ONLY)
#define MAX_IMAGE_INFO_LENGTH  (MCUBOOT_IMAGE_NUMBER *(sizeof(struct image_version) + \
                                  SHARED_DATA_ENTRY_HEADER_SIZE))

#define TFM_MCUBOOT_FWU_INVALID_IMAGE_ID    0xFF
/*
 * \struct fwu_image_info_data
 *
 * \brief Contains the received boot status information from bootloader
 *
 * \details This is a redefinition of \ref tfm_boot_data to allocate the
 *          appropriate, service dependent size of \ref boot_data.
 */
typedef struct fwu_image_info_data_s {
    struct shared_data_tlv_header header;
    uint8_t data[MAX_IMAGE_INFO_LENGTH];
} fwu_image_info_data_t;

typedef struct tfm_fwu_mcuboot_ctx_s {
    /* The flash area corresponding to mcuboot_image_id. */
    const struct flash_area *fap;
    uint8_t mcuboot_image_id;

    /* The size of the downloaded data in the FWU process. */
    size_t loaded_size;
} tfm_fwu_mcuboot_ctx_t;

static tfm_fwu_mcuboot_ctx_t mcuboot_ctx[TFM_FWU_MAX_IMAGES];
static fwu_image_info_data_t boot_shared_data;

static int convert_id_from_bl_to_mcuboot(bl_image_id_t bl_image_id,
                                         uint8_t *mcuboot_image_id)
{
#if (MCUBOOT_APP_IMAGE_NUMBER == 1)
    /* Only full image upgrade is supported in this case. */
    if (bl_image_id != FWU_IMAGE_TYPE_FULL) {
        LOG_MSG("TFM FWU: multi-image is not supported in current mcuboot configuration.");
        return -1;
    }

    /* The image id in mcuboot. 0: the full image. */
    *mcuboot_image_id = 0;
#else
    if (bl_image_id == FWU_IMAGE_TYPE_SECURE) {
        /* The image id in mcuboot. 0: the secure image. */
        *mcuboot_image_id = 0;
    } else if (bl_image_id == FWU_IMAGE_TYPE_NONSECURE) {
        /* The image id in mcuboot. 1: the non-secure image. */
        *mcuboot_image_id = 1;
    }  else {
        LOG_MSG("TFM FWU: invalid image_type: %d", bl_image_id);
        return -1;
    }
#endif
    return 0;
}

static int convert_id_from_mcuboot_to_bl(uint8_t mcuboot_image_id,
                                         bl_image_id_t *bl_image_id)
{
    uint8_t image_type;

    if (bl_image_id == NULL) {
        return -1;
    }

#if (MCUBOOT_APP_IMAGE_NUMBER == 1)
    /* Only full image upgrade is supported in this case. */
    if (mcuboot_image_id != 0) {
        LOG_MSG("TFM FWU: multi-image is not supported in current mcuboot configuration.\n\r");
        return -1;
    }

    /* The image id in mcuboot. 0: the full image. */
    image_type = FWU_IMAGE_TYPE_FULL;
#else
    if (mcuboot_image_id == 0) {
        /* The image id in mcuboot. 0: the secure image. */
        image_type = FWU_IMAGE_TYPE_SECURE;
    } else if (mcuboot_image_id == 1) {
        /* The image id in mcuboot. 1: the non-secure image. */
        image_type = FWU_IMAGE_TYPE_NONSECURE;
    }  else {
        LOG_MSG("TFM FWU: invalid mcuboot image id\n\r: %d",
                mcuboot_image_id);
        return -1;
    }
#endif
    *bl_image_id = image_type;
    return 0;
}


/*
 * Get the flash area of the image mcuboot_image_id.
 */
static bool get_flash_image_index(uint8_t mcuboot_image_id, uint8_t *index)
{
    for (uint8_t i = 0; i < TFM_FWU_MAX_IMAGES; i++) {
        if (mcuboot_ctx[i].mcuboot_image_id == mcuboot_image_id) {
            *index = i;
            return true;
        }
    }
    return false;
}

static bool get_next_free_index(uint8_t *index)
{
    for (uint8_t i = 0; i < TFM_FWU_MAX_IMAGES; i++) {
        if (mcuboot_ctx[i].fap == NULL) {
            *index = i;
            return true;
        }
    }
    return false;
}

static int fwu_bootloader_get_shared_data(void)
{
    return tfm_core_get_boot_data(TLV_MAJOR_FWU,
                                  (struct tfm_boot_data *)&boot_shared_data,
                                  sizeof(boot_shared_data));
}

psa_status_t fwu_bootloader_init(void)
{
    if (fwu_bootloader_get_shared_data() != TFM_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t fwu_bootloader_staging_area_init(bl_image_id_t bootloader_image_id)
{
    uint8_t mcuboot_image_id = 0;
    uint8_t index;
    const struct flash_area *fap;

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (flash_area_open(FLASH_AREA_IMAGE_SECONDARY(mcuboot_image_id),
                        &fap) != 0) {
        LOG_MSG("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (get_flash_image_index(mcuboot_image_id, &index) ||
        get_next_free_index(&index)) {
        mcuboot_ctx[index].mcuboot_image_id = mcuboot_image_id;
        mcuboot_ctx[index].fap = fap;

        /* Reset the loaded_size. */
        mcuboot_ctx[index].loaded_size = 0;
    } else {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    if (flash_area_erase(fap, 0, fap->fa_size) != 0) {
        LOG_MSG("TFM FWU: erasing flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t fwu_bootloader_load_image(bl_image_id_t bootloader_image_id,
                                       size_t block_offset,
                                       const void *block,
                                       size_t block_size)
{
    uint8_t mcuboot_image_id = 0;
    uint8_t index;
    const struct flash_area *fap;

    if (block == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
        fap = mcuboot_ctx[index].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (flash_area_write(fap, block_offset, block, block_size) != 0) {
        LOG_MSG("TFM FWU: write flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* The overflow check has been done in flash_area_write. */
    mcuboot_ctx[index].loaded_size += block_size;
    return PSA_SUCCESS;
}

static bool check_image_dependency(uint8_t mcuboot_image_id,
                                uint8_t *dependency,
                                psa_image_version_t *version)
{
    bool found = false;

    if ((dependency == NULL || version == NULL)) {
        return found;
    }

    /* Currently only single image update is supported. So no dependency is
     * required.
     */
    /* TODO: Add the dependency check to support multiple image update.*/
    *dependency = TFM_MCUBOOT_FWU_INVALID_IMAGE_ID;
    *version = (psa_image_version_t){.iv_major = 0, .iv_minor = 0,
                           .iv_revision = 0, .iv_build_num = 0};

    return found;
}

psa_status_t fwu_bootloader_install_image(bl_image_id_t bootloader_image_id,
                                          bl_image_id_t *dependency,
                                       psa_image_version_t *dependency_version)
{
    uint8_t mcuboot_image_id = 0;
    uint8_t dependency_mcuboot;
    bl_image_id_t dependency_bl;
    psa_image_version_t version;
    const struct flash_area *fap;
    uint8_t index;

    if ((dependency == NULL || dependency_version == NULL)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
        fap = mcuboot_ctx[index].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (check_image_dependency(mcuboot_image_id,
                               &dependency_mcuboot,
                               &version)) {
        if (convert_id_from_mcuboot_to_bl(dependency_mcuboot,
                                          &dependency_bl) != 0) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        *dependency = dependency_bl;
        *dependency_version = version;

        /* PSA_ERROR_DEPENDENCY_NEEDED indicates that a dependency is
         * required. See the function description in
         * tfm_bootloader_fwu_abstraction.h.
         */
        return PSA_ERROR_DEPENDENCY_NEEDED;
    } else {
        /* Write the magic in the image trailer so that this image will be set
         * taken as a candidate.
         */
        if (boot_write_magic(fap) != 0) {
            return PSA_ERROR_GENERIC_ERROR;
        } else {
            /* System reboot is always required. */
            return PSA_SUCCESS_REBOOT;
        }
    }
}

psa_status_t fwu_bootloader_mark_image_accepted(void)
{
    /* As DIRECT_XIP, RAM_LOAD and OVERWRITE_ONLY do not support image revert.
     * So there is nothing to do in these three upgrade strategies. Image
     * revert is only supported in SWAP upgrade strategy. In this case, the
     * image need to be set as a permanent image, so that the next time reboot
     * up, the image will still be the running image, otherwise, the image will
     * be reverted and the original image will be chosen as the running image.
     */
#if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_RAM_LOAD) && \
    !defined(MCUBOOT_OVERWRITE_ONLY)
    if (boot_set_confirmed() != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }
#endif
    return PSA_SUCCESS;
}

psa_status_t fwu_bootloader_abort(bl_image_id_t bootloader_image_id)
{
    uint8_t mcuboot_image_id = 0;
    const struct flash_area *fap;
    uint8_t index;

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
        fap = mcuboot_ctx[index].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    flash_area_erase(fap, 0, fap->fa_size);
    flash_area_close(fap);
    mcuboot_ctx[index].fap = NULL;
    mcuboot_ctx[index].loaded_size = 0;
    return PSA_SUCCESS;
}


static psa_status_t util_img_hash(const struct flash_area *fap,
                                 size_t data_size,
                                 uint8_t *hash_result,
                                 size_t buf_size,
                                 size_t *hash_size)
{
    psa_hash_operation_t handle = psa_hash_operation_init();
    psa_status_t status;
    uint8_t tmpbuf[BOOT_TMPBUF_SZ];
    uint32_t tmp_buf_sz = BOOT_TMPBUF_SZ;
    uint32_t blk_sz;
    uint32_t off;

    /* Setup the hash object for the desired hash. */
    status = psa_hash_setup(&handle, PSA_ALG_SHA_256);
    if (status != PSA_SUCCESS) {
        return status;
    }

    for (off = 0; off < data_size; off += blk_sz) {
        blk_sz = data_size - off;
        if (blk_sz > tmp_buf_sz) {
            blk_sz = tmp_buf_sz;
        }

        if (flash_area_read(fap, off, tmpbuf, blk_sz)) {
            return PSA_ERROR_GENERIC_ERROR;
        }
        status = psa_hash_update(&handle, tmpbuf, blk_sz);
        if (status != PSA_SUCCESS) {
            return status;
        }
    }

    status = psa_hash_finish(&handle, hash_result, buf_size, hash_size);

    return status;
}

static psa_status_t get_secondary_image_info(uint8_t image_id,
                                             psa_image_info_t *info)
{
    const struct flash_area *fap = NULL;
    struct image_header hdr = {0};
    uint8_t hash[PSA_FWU_MAX_DIGEST_SIZE] = {0};
    size_t hash_size = 0;
    psa_status_t ret = PSA_SUCCESS;
    uint8_t index;
    size_t data_size;

    if (info == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((flash_area_open(FLASH_AREA_IMAGE_SECONDARY(image_id),
                            &fap)) != 0) {
        LOG_MSG("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (flash_area_read(fap, 0, &hdr, sizeof(hdr)) != 0) {
        flash_area_close(fap);
        LOG_MSG("TFM FWU: reading flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }


    /* Check if the image is in a FWU process. */
    if (get_flash_image_index(image_id, &index)) {

        /* Calculate hash on the downloaded data. */
        data_size = mcuboot_ctx[index].loaded_size;
    } else {
        /* Check if a valid image exists on the staging area.
         * If a valid image exists, the FWU partition has no information on the
         * image size downloaded as the image is copied from the running slot
         * during the reboot. So in this case when calculating the image hash,
         * the range starts from the image header till the end of the protected
         * TLV area.
         */
        if (hdr.ih_magic == IMAGE_MAGIC) {
            info->version.iv_major = hdr.ih_ver.iv_major;
            info->version.iv_minor = hdr.ih_ver.iv_minor;
            info->version.iv_revision = hdr.ih_ver.iv_revision;
            info->version.iv_build_num = hdr.ih_ver.iv_build_num;
            LOG_MSG("version= %d., %d., %d.,+ %d\n\r",
                    info->version.iv_major,
                    info->version.iv_minor,
                    info->version.iv_revision,
                    info->version.iv_build_num);

            /* Calculate hash on from the image header to the protected TLV. */
            data_size = hdr.ih_hdr_size + hdr.ih_img_size +
                        hdr.ih_protect_tlv_size;
        } else {
            /* No image in the staging area. */
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    }

    if (util_img_hash(fap, data_size, hash, (size_t)PSA_FWU_MAX_DIGEST_SIZE,
                      &hash_size) == PSA_SUCCESS) {
        tfm_memcpy(info->digest, hash, hash_size);
        ret = PSA_SUCCESS;
    } else {
        ret = PSA_ERROR_GENERIC_ERROR;
    }

    /* The actual image state will be filled in the tfm_fwu_req_mngr.c where
     * the image state is maintained.
     */
    info->state = PSA_IMAGE_UNDEFINED;
    flash_area_close(fap);
    return ret;
}

psa_status_t fwu_bootloader_get_image_info(bl_image_id_t bootloader_image_id,
                                  bool active_image,
                                  psa_image_info_t *info)
{
    struct image_version image_ver = { 0 };
    struct shared_data_tlv_entry tlv_entry;
    uint8_t *tlv_end;
    uint8_t *tlv_curr;
    bool found = false;
    uint8_t mcuboot_image_id = 0;

    if (info == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    memset(info, 0, sizeof(psa_image_info_t));
    memset(info->digest, TFM_IMAGE_INFO_INVALID_DIGEST, sizeof(info->digest));

    if (active_image) {
        /* Set the image state as accepted. */
        /* TODO: check the image state by reading the image_ok flag in SWAP
         * case.
         */
        info->state = PSA_IMAGE_INSTALLED;

        /* When getting the primary image information, read it from the
         * shared memory.
         */
        if (boot_shared_data.header.tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        tlv_end = (uint8_t *)&boot_shared_data +
                  boot_shared_data.header.tlv_tot_len;
        tlv_curr = boot_shared_data.data;

        while (tlv_curr < tlv_end) {
            (void)memcpy(&tlv_entry, tlv_curr, SHARED_DATA_ENTRY_HEADER_SIZE);
            if ((GET_FWU_CLAIM(tlv_entry.tlv_type) == SW_VERSION) &&
               (GET_FWU_MODULE(tlv_entry.tlv_type) == mcuboot_image_id)) {
                if (tlv_entry.tlv_len != sizeof(struct image_version)) {
                    return PSA_ERROR_GENERIC_ERROR;
                }
                memcpy(&image_ver,
                    tlv_curr + SHARED_DATA_ENTRY_HEADER_SIZE,
                    tlv_entry.tlv_len);
                found = true;
                break;
            }
            tlv_curr += SHARED_DATA_ENTRY_HEADER_SIZE + tlv_entry.tlv_len;
        }
        if (found) {
            info->version.iv_major = image_ver.iv_major;
            info->version.iv_minor = image_ver.iv_minor;
            info->version.iv_revision = image_ver.iv_revision;
            info->version.iv_build_num = image_ver.iv_build_num;

            /* The image in the primary slot is verified by the bootloader.
             * The image digest in the primary slot should not be exposed to
             * nonsecure.
             */
            return PSA_SUCCESS;
        } else {
            return PSA_ERROR_GENERIC_ERROR;
        }
    } else {
        return get_secondary_image_info(mcuboot_image_id, info);
    }
}
#endif