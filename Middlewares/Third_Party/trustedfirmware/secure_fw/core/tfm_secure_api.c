/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "secure_utilities.h"
#include "tfm_secure_api.h"

/* This is the "Big Lock" on the secure side, to guarantee single entry
 * to SPE
 */
int32_t tfm_secure_lock;

int32_t tfm_bitcount(uint32_t n)
{
    uint8_t count = 0;
    while (n) {
        count++;
        n &= n-1;
    }
    return (int32_t)count;
}

bool tfm_is_one_bit_set(uint32_t n)
{
    return ((n && !(n & (n-1))) ? true : false);
}

enum tfm_status_e check_address_range(const void *p, size_t s,
                                      uintptr_t region_start,
                                      uintptr_t region_limit)
{
    int32_t range_in_region;

    /* Check for overflow in the range parameters */
    if ((uintptr_t)p > UINTPTR_MAX - s) {
        return TFM_ERROR_GENERIC;
    }

    /* We trust the region parameters, and don't check for overflow */

    /* Calculate the result */
    range_in_region = ((uintptr_t)p >= region_start) &&
        ((uintptr_t)((char *) p + s - 1) <= region_limit);
    if (range_in_region) {
        return TFM_SUCCESS;
    } else {
        return TFM_ERROR_GENERIC;
    }
}

void tfm_secure_api_error_handler(void)
{
    ERROR_MSG("Security violation when calling secure API");
    tfm_panic();
}
