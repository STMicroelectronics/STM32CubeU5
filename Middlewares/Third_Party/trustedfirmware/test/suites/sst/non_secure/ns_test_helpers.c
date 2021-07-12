/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ns_test_helpers.h"



void tfm_sst_run_test(const char *thread_name, struct test_result_t *ret,
                      test_func_t *test_func)
{
    test_func(ret);
}
