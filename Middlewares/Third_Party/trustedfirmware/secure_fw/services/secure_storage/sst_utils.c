/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sst_utils.h"

psa_ps_status_t sst_utils_check_contained_in(uint32_t superset_size,
                                             uint32_t subset_offset,
                                             uint32_t subset_size)
{
    /* Check that subset_offset is valid */
    if (subset_offset > superset_size) {
        return PSA_PS_ERROR_OFFSET_INVALID;
    }

    /* Check that subset_offset + subset_size fits in superset_size.
     * The previous check passed, so we know that subset_offset <= superset_size
     * and so the right hand side of the inequality cannot underflow.
     */
    if (subset_size > (superset_size - subset_offset)) {
        return PSA_PS_ERROR_INCORRECT_SIZE;
    }

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_utils_validate_fid(uint32_t fid)
{
    if (fid == SST_INVALID_FID) {
        return PSA_PS_ERROR_UID_NOT_FOUND;
    }

    return PSA_PS_SUCCESS;
}
