/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "psa/lifecycle.h"
#include "tfm/tfm_core_svc.h"

#ifdef __ICCARM__
/* remove IAR warning relative to non void function not returning value */
#pragma diag_suppress=Pe940
#endif

__attribute__((naked))
uint32_t psa_rot_lifecycle_state(void)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_LIFECYCLE));
}
