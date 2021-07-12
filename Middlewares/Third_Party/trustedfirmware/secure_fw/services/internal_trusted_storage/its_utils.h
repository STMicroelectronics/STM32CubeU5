/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ITS_UTILS_H__
#define __ITS_UTILS_H__

#include <stddef.h>
#include <stdint.h>

#include "flash_layout.h"
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ITS_FILE_ID_SIZE 12
#define ITS_DEFAULT_EMPTY_BUFF_VAL 0

/**
 * \brief Macro to check, at compilation time, if data fits in data buffer
 *
 * \param[in] err_msg        Error message which will be displayed in first
 *                           instance if the error is triggered
 * \param[in] data_size      Data size to check if it fits
 * \param[in] data_buf_size  Size of the data buffer
 *
 * \return  Triggers a compilation error if data_size is bigger than
 *          data_buf_size. The compilation error should be
 *          "... error: 'err_msg' declared as an array with a negative size"
 */
#define ITS_UTILS_BOUND_CHECK(err_msg, data_size, data_buf_size) \
typedef char err_msg[(data_size <= data_buf_size)*2 - 1]

/**
 * \brief Evaluates to the minimum of the two parameters.
 */
#define ITS_UTILS_MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
 * \brief Macro to get the number of bytes aligned with the
 *        ITS_FLASH_PROGRAM_UNIT.
 *
 * \param[in] nbr_bytes  Number of bytes
 *
 * \return Return number of bytes aligned with ITS_FLASH_PROGRAM_UNIT
 */
#define GET_ALIGNED_FLASH_BYTES(nbr_bytes) \
 ((nbr_bytes + (ITS_FLASH_PROGRAM_UNIT - 1)) & ~((ITS_FLASH_PROGRAM_UNIT - 1)))

/**
 * \brief Checks if a subset region is fully contained within a superset region.
 *
 * \param[in] superset_size  Size of superset region
 * \param[in] subset_offset  Offset of start of subset region from start of
 *                           superset region
 * \param[in] subset_size    Size of subset region
 *
 * \return Returns error code as specified in \ref psa_status_t
 *
 * \retval PSA_SUCCESS                 The subset is contained within the
 *                                     superset
 * \retval PSA_ERROR_INVALID_ARGUMENT  Otherwise
 */
psa_status_t its_utils_check_contained_in(size_t superset_size,
                                          size_t subset_offset,
                                          size_t subset_size);

/**
 * \brief Validates file ID
 *
 * \param[in] fid  File ID
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_utils_validate_fid(const uint8_t *fid);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_UTILS_H__ */
