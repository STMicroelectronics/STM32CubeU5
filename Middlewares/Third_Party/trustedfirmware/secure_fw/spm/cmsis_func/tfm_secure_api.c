/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "spm_func.h"
#include "tfm_secure_api.h"

void tfm_secure_api_error_handler(void)
{
    ERROR_MSG("Security violation when calling secure API");
    tfm_core_panic();
}

int32_t tfm_core_partition_request(uint32_t id, bool is_ns, void *fn,
            int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4)
{
    int32_t args[4] = {arg1, arg2, arg3, arg4};
    struct tfm_sfn_req_s desc, *desc_ptr = &desc;

    desc.sp_id = id;
    desc.sfn = (sfn_t) fn;
    desc.args = args;
    desc.ns_caller = is_ns;

    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE) {
        /* The veneer of a secure service had been called from Handler mode.
         * This violates TF-M's programming model, and is considered an
         * unrecoverable error.
         */
        tfm_core_panic();
    } else {
        if (desc.ns_caller) {
            return tfm_core_sfn_request(desc_ptr);
        } else {
            return tfm_spm_sfn_request_thread_mode(desc_ptr);
        }
    }
    return TFM_ERROR_GENERIC;
}
