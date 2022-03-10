/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tfm_arch.h"
#include "tfm_assert.h"
#include "tfm_spe_mailbox.h"
#include "tfm_spm_hal.h"
#include "tfm_spm_log.h"

#define DEFAULT_NS_CLIENT_ID            (-1)

int32_t tfm_nspm_get_current_client_id(void)
{
    return DEFAULT_NS_CLIENT_ID;
}

void tfm_nspm_thread_entry(void)
{
#ifdef TFM_CORE_DEBUG
    /* Boot up non-secure core */
    SPMLOG_DBGMSG("Enabling non-secure core...");
#endif

    tfm_spm_hal_boot_ns_cpu(tfm_spm_hal_get_ns_VTOR());
    tfm_spm_hal_wait_for_ns_cpu_ready();

    tfm_mailbox_init();

    /*
     * TODO
     * The infinite-loop can be replaced with platform-specific low-power sleep
     * and resume operation.
     * It requires privileged access to platform specific hardware.
     */
    while (1) {
        __WFI();
    }

    /* NOTREACHED */
    TFM_ASSERT(false);
}
