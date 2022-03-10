/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_APP_ROT_API__
#define __TFM_APP_ROT_API__

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief TFM App Rot partition platform API version
 */
#define TFM_APP_ROT_API_VERSION_MAJOR (0)
#define TFM_APP_ROT_API_VERSION_MINOR (3)

/*!
 * \enum ttfm_app_rot_err_t
 *
 * \brief app_rot service error types
 *
 */
enum tfm_app_rot_err_t {
    TFM_APP_ROT_ERR_SUCCESS = 0,
    TFM_APP_ROT_ERROR,
    TFM_APP_ROT_ERR_INVALID_PARAM,
    TFM_APP_ROT_ERR_NOT_SUPPORTED,

    /* Following entry is only to ensure the error code of int size */
    TFM_APP_ROT_ERR_FORCE_INT_SIZE = INT_MAX
};

/* ioctl value */
typedef int32_t tfm_app_rot_ioctl_req_t;

/*!
 * \brief Performs a platform-specific service
 *
 * \param[in]  request      Request identifier (valid values vary
 *                          based on the platform)
 * \param[in]  input        Input buffer to the requested service (or NULL)
 * \param[in,out] output    Output buffer to the requested service (or NULL)
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_app_rot_err_t tfm_app_rot_ioctl(tfm_app_rot_ioctl_req_t request,
                                           psa_invec *input,
                                           psa_outvec *output);


#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_API__ */
