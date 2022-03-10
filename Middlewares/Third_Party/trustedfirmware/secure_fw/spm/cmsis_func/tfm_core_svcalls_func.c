/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>

#include "tfm/tfm_core_svc.h"
#include "tfm_secure_api.h"
#include "region_defs.h"
#include "spm_func.h"
#include "spm_partition_defs.h"
#include "tfm_api.h"
#include "tfm_memory_utils.h"
#include "tfm_arch.h"
#include "tfm_peripherals_def.h"
#include "tfm_irq_list.h"
#include "ffm/tfm_boot_data.h"

#ifdef PLATFORM_SVC_HANDLERS
extern int32_t platform_svc_handlers(tfm_svc_number_t svc_num,
                                     uint32_t *svc_args, uint32_t lr);
#endif

/* Include the definitions of the privileged IRQ handlers in case of library
 * model
 */
#include "tfm_secure_irq_handlers.inc"


uint32_t tfm_core_svc_handler(uint32_t *msp, uint32_t *psp, uint32_t exc_return)
{
    uint8_t svc_number = 0;
    uint32_t *svc_args = msp;
    uint32_t retval = exc_return;

    if ((exc_return & EXC_RETURN_MODE) && (exc_return & EXC_RETURN_SPSEL)) {
        /* Use PSP when both EXC_RETURN.MODE and EXC_RETURN.SPSEL are set */
        svc_args = psp;
    } else {
        svc_args = msp;
    }

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
        tfm_core_panic();
    }
    switch (svc_number) {
    case TFM_SVC_SFN_REQUEST:
        retval = tfm_spm_partition_request_svc_handler(svc_args, exc_return);
        break;
    case TFM_SVC_SFN_RETURN:
        retval = tfm_spm_partition_return_handler(exc_return);
        break;
    case TFM_SVC_GET_CALLER_CLIENT_ID:
        tfm_spm_get_caller_client_id_handler(svc_args);
        break;
    case TFM_SVC_SPM_REQUEST:
        tfm_spm_request_handler((struct tfm_state_context_t *)svc_args);
        break;
    case TFM_SVC_DEPRIV_REQ:
        retval = tfm_spm_depriv_req_handler(svc_args, exc_return);
        break;
    case TFM_SVC_DEPRIV_RET:
        retval = tfm_spm_depriv_return_handler(msp, exc_return);
        break;
    case TFM_SVC_PSA_WAIT:
        tfm_spm_psa_wait(svc_args);
        break;
    case TFM_SVC_PSA_EOI:
        tfm_spm_psa_eoi(svc_args);
        break;
    case TFM_SVC_ENABLE_IRQ:
        tfm_spm_enable_irq_handler(svc_args);
        break;
    case TFM_SVC_DISABLE_IRQ:
        tfm_spm_disable_irq_handler(svc_args);
        break;
    case TFM_SVC_GET_BOOT_DATA:
        tfm_core_get_boot_data_handler(svc_args);
        break;
    case TFM_SVC_GET_BOOT_VALUE:
        tfm_core_get_boot_value_handler(svc_args);
        break;

    default:
#ifdef PLATFORM_SVC_HANDLERS
        svc_args[0] = platform_svc_handlers(svc_num, svc_args, exc_return);
#endif
        break;
    }

    return retval;
}

void tfm_access_violation_handler(void)
{
    while (1) {
        ;
    }
}
