/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "tfm_api.h"
#include "tfm_internal.h"
#include "tfm_svc.h"
#include "tfm_utils.h"
#include "tfm_svcalls.h"

extern void tfm_psa_ipc_request_handler(const uint32_t svc_args[]);

uint32_t tfm_core_svc_handler(uint32_t *svc_args, uint32_t exc_return)
{
    uint8_t svc_number = 0;
    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, r14 (lr), the return address and xPSR
     * First argument (r0) is svc_args[0]
     */
    if (is_return_secure_stack(exc_return)) {
        /* SV called directly from secure context. Check instruction for
         * svc_number
         */
        svc_number = ((uint8_t *)svc_args[6])[-2];
    } else {
        /* Secure SV executing with NS return.
         * NS cannot directly trigger S SVC so this should not happen. This is
         * an unrecoverable error.
         */
        tfm_panic();
    }
    switch (svc_number) {
    case TFM_SVC_IPC_REQUEST:
        tfm_psa_ipc_request_handler(svc_args);
        break;
    case TFM_SVC_PRINT:
        TFM_CORE_MSG((char *)svc_args[0]);
        break;
    case TFM_SVC_GET_BOOT_DATA:
        tfm_core_get_boot_data_handler(svc_args);
        break;
	case TFM_SVC_GET_BOOT_VALUE:
        tfm_core_get_boot_value_handler(svc_args);
        break;
    default:
        svc_args[0] = SVC_Handler_IPC((tfm_svc_number_t)svc_number, svc_args, exc_return);
        break;
    }

    return exc_return;
}

void tfm_access_violation_handler(void)
{
    while (1) {
        ;
    }
}
