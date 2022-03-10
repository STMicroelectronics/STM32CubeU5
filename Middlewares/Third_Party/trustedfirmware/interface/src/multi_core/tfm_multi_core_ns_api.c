/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_api.h"
#include "tfm_multi_core_api.h"

__attribute__((weak))
enum tfm_status_e tfm_ns_interface_init(void)
{
    return TFM_SUCCESS;
}

int32_t tfm_ns_wait_for_s_cpu_ready(void)
{
    return tfm_platform_ns_wait_for_s_cpu_ready();
}
