/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SST_FLASH_FS_H__
#define __SST_FLASH_FS_H__

/**
 * \file  sst_flash_fs.h
 *
 * \brief Secure storage service filesystem abstraction APIs.
 *        The purpose of this abstraction is to have the ability to plug-in
 *        other filesystems or filesystem proxys (supplicant).
 */

#include <stdint.h>
#include "psa/protected_storage.h"

/*!
 * \struct sst_file_info_t
 *
 * \brief Structure to store the file information.
 */
struct sst_file_info_t {
    uint32_t size_current; /*!< The current size of the flash file data */
    uint32_t size_max;     /*!< The maximum size of the flash file data in
                            *   bytes.
                            */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Prepares the filesystem to accept operations on the files.
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_prepare(void);

/**
 * \brief Wipes all files from the filesystem.
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_wipe_all(void);

/**
 * \brief Checks if a file exists in the filesystem.
 *
 * \param[in] fid  File ID
 *
 * \return Returns PSA_PS_SUCCESS if the file exists. If file does not
 *         exist, it returns PSA_PS_ERROR_UID_NOT_FOUND. Otherwise, it returns
 *         error code as specified in \ref psa_ps_status_t.
 */
psa_ps_status_t sst_flash_fs_file_exist(uint32_t fid);

/**
 * \brief Creates a file in the filesystem.
 *
 * \param[in] fid        File ID
 * \param[in] max_size   Size of the file to be created
 * \param[in] data_size  Size of the incoming buffer. This parameter is set to 0
 *                       when the file is empty after the creation.
 * \param[in] data       Pointer to buffer containing the initial data.
 *                       This parameter is set to NULL when the file is empty
 *                       after the creation.
 *
 * \return Returns PSA_PS_SUCCESS if the file has been created correctly.
 *         If fid is in used, it returns PSA_PS_ERROR_INVALID_ARGUMENT.
 *         Otherwise, it returns error code as specified in
 *         \ref psa_ps_status_t.
 */
psa_ps_status_t sst_flash_fs_file_create(uint32_t fid,
                                         uint32_t max_size,
                                         uint32_t data_size,
                                         const uint8_t *data);

/**
 * \brief Gets the file information referenced by the file ID.
 *
 * \param[in]  fid   File ID
 * \param[out] info  Pointer to the information structure to store the
 *                   file information values \ref sst_file_info_t
 *
 * \return Returns error code specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_file_get_info(uint32_t fid,
                                           struct sst_file_info_t *info);

/**
 * \brief Writes data to an existing file.
 *
 * \param[in] fid     File ID
 * \param[in] size    Size of the incoming buffer
 * \param[in] offset  Offset in the file
 * \param[in] data    Pointer to buffer containing data to be written
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_file_write(uint32_t fid,
                                        uint32_t size,
                                        uint32_t offset,
                                        const uint8_t *data);

/**
 * \brief Reads data from an existing file.
 *
 * \param[in]  fid     File ID
 * \param[in]  size    Size to be read
 * \param[in]  offset  Offset in the file
 * \param[out] data    Pointer to buffer to store the data
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_file_read(uint32_t fid,
                                       uint32_t size,
                                       uint32_t offset,
                                       uint8_t *data);

/**
 * \brief Deletes file referenced by the file ID.
 *
 * \param[in] fid  File ID
 *
 * \return Returns error code as specified in \ref psa_ps_status_t
 */
psa_ps_status_t sst_flash_fs_file_delete(uint32_t fid);

#ifdef __cplusplus
}
#endif

#endif /* __SST_FLASH_FS_H__ */
