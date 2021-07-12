/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  its_flash_fs.h
 *
 * \brief Internal Trusted Storage service filesystem abstraction APIs.
 *        The purpose of this abstraction is to have the ability to plug-in
 *        other filesystems or filesystem proxies (supplicant).
 */

#ifndef __ITS_FLASH_FS_H__
#define __ITS_FLASH_FS_H__

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct its_file_info_t
 *
 * \brief Structure to store the file information.
 */
struct its_file_info_t {
    size_t size_current; /*!< The current size of the flash file data */
    size_t size_max;     /*!< The maximum size of the flash file data in
                          * bytes.
                          */
    uint32_t flags;      /*!< Flags set when the file was created */
};

/**
 * \brief Prepares the filesystem to accept operations on the files.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_prepare(void);

/**
 * \brief Wipes all files from the filesystem.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_wipe_all(void);

/**
 * \brief Checks if a file exists in the filesystem.
 *
 * \param[in] fid  File ID
 *
 * \return Returns PSA_SUCCESS if the file exists. If file does not exist, it
 *         returns PSA_ERROR_DOES_NOT_EXIST. Otherwise, it returns error code as
 *         specified in \ref psa_status_t.
 */
psa_status_t its_flash_fs_file_exist(const uint8_t *fid);

/**
 * \brief Creates a file in the filesystem.
 *
 * \param[in] fid        File ID
 * \param[in] max_size   Size of the file to be created
 * \param[in] data_size  Size of the incoming buffer. This parameter is set to 0
 *                       when the file is empty after the creation.
 * \param[in] flags      Flags of the file
 * \param[in] data       Pointer to buffer containing the initial data.
 *                       This parameter is set to NULL when the file is empty
 *                       after the creation.
 *
 * \return Returns PSA_SUCCESS if the file has been created correctly. If the
 *         fid is in use, it returns PSA_ERROR_INVALID_ARGUMENT. Otherwise, it
 *         returns error code as specified in \ref psa_status_t.
 */
psa_status_t its_flash_fs_file_create(const uint8_t *fid,
                                      size_t max_size,
                                      size_t data_size,
                                      uint32_t flags,
                                      const uint8_t *data);

/**
 * \brief Gets the file information referenced by the file ID.
 *
 * \param[in]  fid   File ID
 * \param[out] info  Pointer to the information structure to store the
 *                   file information values \ref its_file_info_t
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_get_info(const uint8_t *fid,
                                        struct its_file_info_t *info);

/**
 * \brief Writes data to an existing file.
 *
 * \param[in] fid     File ID
 * \param[in] size    Size of the incoming buffer
 * \param[in] offset  Offset in the file
 * \param[in] data    Pointer to buffer containing data to be written
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_write(const uint8_t *fid,
                                     size_t size,
                                     size_t offset,
                                     const uint8_t *data);

/**
 * \brief Reads data from an existing file.
 *
 * \param[in]  fid          File ID
 * \param[in]  size         Size to be read
 * \param[in]  offset       Offset in the file
 * \param[out] data         Pointer to buffer to store the data
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_read(const uint8_t *fid,
                                    size_t size,
                                    size_t offset,
                                    uint8_t *data);

/**
 * \brief Deletes file referenced by the file ID.
 *
 * \param[in] fid  File ID
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_delete(const uint8_t *fid);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_FS_H__ */
