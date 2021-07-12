/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "core_test_api.h"
#include "test/test_services/tfm_core_test/core_test_defs.h"

int32_t tfm_core_test_call(int32_t (*fn_ptr)(),
                           struct tfm_core_test_call_args_t *args)
{
    int32_t (*fn_ptr_to_call)(struct psa_invec*, size_t,
                              struct psa_outvec*, size_t) = fn_ptr;
    return fn_ptr_to_call(args->in_vec, args->in_len,
                          args->out_vec, args->out_len);
}
