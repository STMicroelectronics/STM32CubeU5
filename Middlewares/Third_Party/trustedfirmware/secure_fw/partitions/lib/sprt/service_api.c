/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis_compiler.h"
#include "service_api.h"
#include "tfm/tfm_core_svc.h"

#ifdef __ICCARM__
/* remove IAR warning relative to non void function not returning value */
#pragma diag_suppress=Pe940
#endif

__attribute__((naked))
int32_t tfm_core_get_boot_data(uint8_t major_type,
                               struct tfm_boot_data *boot_status,
                               uint32_t len)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_GET_BOOT_DATA));
}

__attribute__((naked))
int32_t tfm_core_get_boot_value(uint16_t tlv_value,
                               void *ptr,
                               uint32_t len)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_GET_BOOT_VALUE));
}

