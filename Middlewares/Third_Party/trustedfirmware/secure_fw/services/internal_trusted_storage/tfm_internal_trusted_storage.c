/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_internal_trusted_storage.h"
#include "flash_fs/its_flash_fs.h"
#include "tfm_memory_utils.h"
#include "tfm_its_defs.h"
#include "its_utils.h"

static uint8_t g_fid[ITS_FILE_ID_SIZE];
static struct its_file_info_t g_file_info;

/**
 * \brief Maps a pair of client id and uid to a file id.
 *
 * \param[in]  client_id  Identifier of the asset's owner (client)
 * \param[in]  uid        Identifier for the data
 * \param[out] fid        Identifier of the file
 */
static void tfm_its_get_fid(int32_t client_id,
                            psa_storage_uid_t uid,
                            uint8_t *fid)
{
    tfm_memcpy(fid, (const void *)&client_id, sizeof(client_id));
    tfm_memcpy(fid + sizeof(client_id), (const void *)&uid, sizeof(uid));
}

psa_status_t tfm_its_init(void)
{
    psa_status_t status;

    status = its_flash_fs_prepare();
#ifdef ITS_CREATE_FLASH_LAYOUT
    /* If ITS_CREATE_FLASH_LAYOUT is set, it indicates that it is required to
     * create a ITS flash layout. ITS service will generate an empty and valid
     * ITS flash layout to store assets. It will erase all data located in the
     * assigned ITS memory area before generating the ITS layout.
     * This flag is required to be set if the ITS memory area is located in
     * non-persistent memory.
     * This flag can be set if the ITS memory area is located in persistent
     * memory without a previous valid ITS flash layout in it. That is the case
     * when it is the first time in the device life that the ITS service is
     * executed.
     */
     if (status != PSA_SUCCESS) {
        /* Remove all data in the ITS memory area and create a valid ITS flash
         * layout in that area.
         */
        status = its_flash_fs_wipe_all();
        if (status != PSA_SUCCESS) {
            return status;
        }

        /* Attempt to initialise again */
        status = its_flash_fs_prepare();
    }
#endif /* ITS_CREATE_FLASH_LAYOUT */

    return status;
}

psa_status_t tfm_its_set(int32_t client_id,
                         psa_storage_uid_t uid,
                         size_t data_length,
                         const void *p_data,
                         psa_storage_create_flags_t create_flags)
{
    psa_status_t status;

    /* Check that the UID is valid */
    if (uid == TFM_ITS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check that the create_flags does not contain any unsupported flags */
    if (create_flags & ~(PSA_STORAGE_FLAG_WRITE_ONCE |
                         PSA_STORAGE_FLAG_NO_CONFIDENTIALITY |
                         PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Set file id */
    tfm_its_get_fid(client_id, uid, g_fid);

    /* Read file info */
    status = its_flash_fs_file_get_info(g_fid, &g_file_info);
    if (status == PSA_SUCCESS) {
        /* If the object exists and has the write once flag set, then it
         * cannot be modified. Otherwise it needs to be removed.
         */
        if (g_file_info.flags & PSA_STORAGE_FLAG_WRITE_ONCE) {
            return PSA_ERROR_NOT_PERMITTED;
        } else {
            status = its_flash_fs_file_delete(g_fid);
            if (status != PSA_SUCCESS) {
                return status;
            }
        }
    } else if (status != PSA_ERROR_DOES_NOT_EXIST) {
        /* If the file does not exist, then do nothing.
         * If other error occurred, return it
         */
        return status;
    }

    /* Create the file in the file system */
    return its_flash_fs_file_create(g_fid,
                                    data_length,
                                    data_length,
                                    (uint32_t)create_flags,
                                    (const uint8_t *)p_data);
}

psa_status_t tfm_its_get(int32_t client_id,
                         psa_storage_uid_t uid,
                         size_t data_offset,
                         size_t data_size,
                         void *p_data,
                         size_t *p_data_length)
{
    psa_status_t status;

    /* Check that the UID is valid */
    if (uid == TFM_ITS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set file id */
    tfm_its_get_fid(client_id, uid, g_fid);

    /* Read file info */
    status = its_flash_fs_file_get_info(g_fid, &g_file_info);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Boundary check the incoming request */
    if (data_offset > g_file_info.size_current) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Copy the object data only from within the file boundary */
    data_size = ITS_UTILS_MIN(data_size,
                              g_file_info.size_current - data_offset);

    /* Read object data if any */
    status = its_flash_fs_file_read(g_fid, data_size, data_offset, p_data);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Update the size of the data placed in p_data */
    *p_data_length = data_size;

    return PSA_SUCCESS;
}

psa_status_t tfm_its_get_info(int32_t client_id, psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info)
{
    psa_status_t status;

    /* Check that the UID is valid */
    if (uid == TFM_ITS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set file id */
    tfm_its_get_fid(client_id, uid, g_fid);

    /* Read file info */
    status = its_flash_fs_file_get_info(g_fid, &g_file_info);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Copy file info to the PSA info struct */
    p_info->capacity = g_file_info.size_current;
    p_info->size = g_file_info.size_current;
    p_info->flags = g_file_info.flags;

    return PSA_SUCCESS;
}

psa_status_t tfm_its_remove(int32_t client_id, psa_storage_uid_t uid)
{
    psa_status_t status;

    /* Check that the UID is valid */
    if (uid == TFM_ITS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Set file id */
    tfm_its_get_fid(client_id, uid, g_fid);

    status = its_flash_fs_file_get_info(g_fid, &g_file_info);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* If the object exists and has the write once flag set, then it
     * cannot be deleted.
     */
    if (g_file_info.flags & PSA_STORAGE_FLAG_WRITE_ONCE) {
        return PSA_ERROR_NOT_PERMITTED;
    }

    /* Delete old file from the persistent area */
    return its_flash_fs_file_delete(g_fid);
}
