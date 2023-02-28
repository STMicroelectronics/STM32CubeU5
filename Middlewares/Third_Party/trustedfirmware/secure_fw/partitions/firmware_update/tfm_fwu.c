/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "log/tfm_log.h"
#include "tfm_platform_api.h"
#include "tfm_fwu.h"
#include "flash_layout.h"

#if !defined(MCUBOOT_PRIMARY_ONLY)
psa_status_t tfm_internal_fwu_initialize(psa_image_id_t image_id)
{
    uint8_t image_type = (uint8_t)FWU_IMAGE_ID_GET_TYPE(image_id);
    uint8_t slot_id = (uint8_t)FWU_IMAGE_ID_GET_SLOT(image_id);

    /* Check the image slot, the target should be the staging slot. */
    if (slot_id != FWU_IMAGE_ID_SLOT_STAGE) {
        LOG_MSG("TFM FWU: invalid slot_id: %d", slot_id);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return fwu_bootloader_staging_area_init(image_type);
}

psa_status_t tfm_internal_fwu_write(psa_image_id_t image_id,
                           size_t block_offset,
                           const void *block,
                           size_t block_size)
{
    uint8_t image_type = (uint8_t)FWU_IMAGE_ID_GET_TYPE(image_id);
    uint8_t slot_id = (uint8_t)FWU_IMAGE_ID_GET_SLOT(image_id);

    if ((block == NULL) || (slot_id != FWU_IMAGE_ID_SLOT_STAGE)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return fwu_bootloader_load_image(image_type,
                                     block_offset,
                                     block,
                                     block_size);
}

psa_status_t tfm_internal_fwu_install(psa_image_id_t image_id,
                                      psa_image_id_t *dependency,
                                      psa_image_version_t *dependency_version)
{
    uint8_t image_type = (uint8_t)FWU_IMAGE_ID_GET_TYPE(image_id);
    uint8_t slot_id = (uint8_t)FWU_IMAGE_ID_GET_SLOT(image_id);
    bl_image_id_t dependency_bl;
    psa_image_version_t version;
    psa_status_t result;

    /* Check the image slot, the target should be the staging slot. */
    if (slot_id != FWU_IMAGE_ID_SLOT_STAGE) {
        LOG_MSG("TFM FWU: invalid slot_id: %d", slot_id);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    result = fwu_bootloader_install_image(image_type,
                                          &dependency_bl,
                                          &version);
    if (result == PSA_ERROR_DEPENDENCY_NEEDED) {
        if (dependency == NULL || dependency_version == NULL) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        *dependency = (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                             dependency_bl,
                                                             0);
        *dependency_version = version;
    }

    return result;
}

psa_status_t tfm_internal_fwu_abort(psa_image_id_t image_id)
{
    uint8_t image_type = (uint8_t)FWU_IMAGE_ID_GET_TYPE(image_id);
    uint8_t slot_id = (uint8_t)FWU_IMAGE_ID_GET_SLOT(image_id);

    if (slot_id != FWU_IMAGE_ID_SLOT_STAGE) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return fwu_bootloader_abort(image_type);
}

/* The image version of the given image. */
psa_status_t tfm_internal_fwu_query(psa_image_id_t image_id,
                           psa_image_info_t *info)
{
    uint8_t image_type = (uint8_t)FWU_IMAGE_ID_GET_TYPE(image_id);
    uint8_t slot_id = (uint8_t)FWU_IMAGE_ID_GET_SLOT(image_id);
    bool active_image = 0;

    if (slot_id == FWU_IMAGE_ID_SLOT_STAGE) {
        active_image = false;
    } else if (slot_id == FWU_IMAGE_ID_SLOT_ACTIVE) {
        active_image = true;
    } else {
        LOG_MSG("TFM FWU: invalid slot_id: %d", slot_id);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return fwu_bootloader_get_image_info(image_type, active_image, info);
}

void tfm_internal_fwu_request_reboot(void)
{
    tfm_platform_system_reset();
}

psa_status_t tfm_internal_fwu_accept(void)
{
    return fwu_bootloader_mark_image_accepted();
}

#endif
