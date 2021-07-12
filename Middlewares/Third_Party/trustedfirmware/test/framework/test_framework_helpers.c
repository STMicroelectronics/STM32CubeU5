/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_helpers.h"

#include <stdio.h>

const char *psa_ps_status_to_str(psa_ps_status_t status)
{
    switch (status) {
    case PSA_PS_SUCCESS:
        return "PSA_PS_SUCCESS";
    case PSA_PS_ERROR_WRITE_ONCE:
        return "PSA_PS_ERROR_WRITE_ONCE";
    case PSA_PS_ERROR_FLAGS_NOT_SUPPORTED:
        return "PSA_PS_ERROR_FLAGS_NOT_SUPPORTED";
    case PSA_PS_ERROR_INSUFFICIENT_SPACE:
        return "PSA_PS_ERROR_INSUFFICIENT_SPACE";
    case PSA_PS_ERROR_STORAGE_FAILURE:
        return "PSA_PS_ERROR_STORAGE_FAILURE";
    case PSA_PS_ERROR_UID_NOT_FOUND:
        return "PSA_PS_ERROR_UID_NOT_FOUND";
    case PSA_PS_ERROR_INCORRECT_SIZE:
        return "PSA_PS_ERROR_INCORRECT_SIZE";
    case PSA_PS_ERROR_OFFSET_INVALID:
        return "PSA_PS_ERROR_OFFSET_INVALID";
    case PSA_PS_ERROR_INVALID_ARGUMENT:
        return "PSA_PS_ERROR_INVALID_ARGUMENT";
    case PSA_PS_ERROR_DATA_CORRUPT:
        return "PSA_PS_ERROR_DATA_CORRUPT";
    case PSA_PS_ERROR_AUTH_FAILED:
        return "PSA_PS_ERROR_AUTH_FAILED";
    case PSA_PS_ERROR_OPERATION_FAILED:
        return "PSA_PS_ERROR_OPERATION_FAILED";
    case PSA_PS_ERROR_NOT_SUPPORTED:
        return "PSA_PS_ERROR_NOT_SUPPORTED";
    default:
        return "Unknown error";
    }
}

const char *asset_perms_to_str(uint8_t permissions)
{
    switch (permissions) {
    case 0:
        return "No permissions";
    case 1:
        return "SECURE_ASSET_REFERENCE";
    case 2:
        return "SECURE_ASSET_WRITE";
    case 3:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_WRITE";
    case 4:
        return "SECURE_ASSET_READ";
    case 5:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_READ";
    case 6:
        return "SECURE_ASSET_WRITE | SECURE_ASSET_READ";
    case 7:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_WRITE | "
               "SECURE_ASSET_READ";
    default:
        return "Unknown permissions";
    }
}

void printf_set_color(enum serial_color_t color_id)
{
    printf("\33[3%dm", color_id);
}
