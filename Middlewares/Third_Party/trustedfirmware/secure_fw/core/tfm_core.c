/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "region_defs.h"
#include "tfm_core.h"
#include "tfm_internal.h"
#include "tfm_api.h"
#include "tfm_arch.h"
#include "tfm_core_topology.h"
#include "tfm_nspm.h"
#include "platform/include/region.h"
#include "platform/include/tfm_spm_hal.h"
#include "uart_stdout.h"
#include "secure_utilities.h"
#include "secure_fw/spm/spm_api.h"
#include "secure_fw/include/tfm_spm_services_api.h"
#include "tfm_irq_list.h"
#include "tfm_utils.h"
#ifdef TFM_PSA_API
#include "psa/client.h"
#include "psa/service.h"
#include "tfm_thread.h"
#include "tfm_wait.h"
#include "tfm_message_queue.h"
#else
#include "spm_api.h"
#endif

/*
 * Avoids the semihosting issue
 * FixMe: describe 'semihosting issue'
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

#ifdef TFM_PSA_API
#if (TFM_LVL != 1) && (TFM_LVL != 2)
#error Only TFM_LVL 1 and 2 are supported for IPC model!
#endif
#else
#if (TFM_LVL != 1)
#error Only TFM_LVL 1 is supported for library model!
#endif
#endif

/* Macros to pick linker symbols and allow to form the partition data base */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

REGION_DECLARE(Image$$, ARM_LIB_STACK_MSP,  $$ZI$$Base);

int32_t tfm_core_init(void)
{
    size_t i;
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
    enum irq_target_state_t irq_target_state = TFM_IRQ_TARGET_STATE_SECURE;

    /* Enables fault handlers */
    plat_err = tfm_spm_hal_enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    /* Configures the system reset request properties */
    plat_err = tfm_spm_hal_system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    /* Configures debug authentication */
    plat_err = tfm_spm_hal_init_debug();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    __enable_irq();

    stdio_init();
    LOG_MSG("Secure image initializing!");

#ifdef TFM_CORE_DEBUG
    TFM_LVL_MSG(TFM_LVL);
#endif

    tfm_core_validate_boot_data();

    plat_err = tfm_spm_hal_init_isolation_hw();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    configure_ns_code();

    /* Configures all interrupts to retarget NS state, except for
     * secure peripherals
     */
    plat_err = tfm_spm_hal_nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    for (i = 0; i < tfm_core_irq_signals_count; ++i) {
        plat_err = tfm_spm_hal_set_secure_irq_priority(
                                           tfm_core_irq_signals[i].irq_line,
                                          tfm_core_irq_signals[i].irq_priority);
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            return TFM_ERROR_GENERIC;
        }
        irq_target_state = tfm_spm_hal_set_irq_target_state(
                                          tfm_core_irq_signals[i].irq_line,
                                          TFM_IRQ_TARGET_STATE_SECURE);
        if (irq_target_state != TFM_IRQ_TARGET_STATE_SECURE) {
            return TFM_ERROR_GENERIC;
        }
    }

    /* Enable secure peripherals interrupts */
    plat_err = tfm_spm_hal_nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    return TFM_SUCCESS;
}

static int32_t tfm_core_set_secure_exception_priorities(void)
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;

    tfm_arch_prioritize_secure_exception();

    /* Explicitly set Secure SVC priority to highest */
    plat_err = tfm_spm_hal_set_secure_irq_priority(SVCall_IRQn, 0);
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }

    tfm_core_topology_set_pendsv_priority();

    return TFM_SUCCESS;
}

#ifndef TFM_PSA_API
void tfm_core_spm_request_handler(const struct tfm_state_context_t *svc_ctx)
{
    uint32_t *res_ptr = (uint32_t *)&svc_ctx->r0;
    uint32_t running_partition_flags = 0;
    uint32_t running_partition_idx;

    /* Check permissions on request type basis */

    switch (svc_ctx->r0) {
    case TFM_SPM_REQUEST_RESET_VOTE:
        running_partition_idx =
        tfm_spm_partition_get_running_partition_idx();
        running_partition_flags = tfm_spm_partition_get_flags(
                                                         running_partition_idx);

        /* Currently only PSA Root of Trust services are allowed to make Reset
         * vote request
         */
        if ((running_partition_flags & SPM_PART_FLAG_PSA_ROT) == 0) {
            *res_ptr = (uint32_t)TFM_ERROR_GENERIC;
        }

        /* FixMe: this is a placeholder for checks to be performed before
         * allowing execution of reset
         */
        *res_ptr = (uint32_t)TFM_SUCCESS;
        break;
    default:
        *res_ptr = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
    }
}
#endif /* TFM_PSA_API */

int main(void)
{
    /* set Main Stack Pointer limit */
    tfm_arch_set_msplim((uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK_MSP,
                                               $$ZI$$Base));

    if (tfm_core_init() != TFM_SUCCESS) {
        tfm_panic();
    }

    if (tfm_spm_db_init() != SPM_ERR_OK) {
        tfm_panic();
    }

#if TFM_LVL != 1
    if (tfm_spm_hal_setup_isolation_hw() != TFM_PLAT_ERR_SUCCESS) {
        tfm_panic();
    }
#endif /* TFM_LVL != 1 */

#ifndef TFM_PSA_API
    tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_RUNNING);

    REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base)[];
    uint32_t psp_stack_bottom =
                      (uint32_t)REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base);

    tfm_arch_set_psplim(psp_stack_bottom);

    if (tfm_spm_partition_init() != SPM_ERR_OK) {
        /* Certain systems might refuse to boot altogether if partitions fail
         * to initialize. This is a placeholder for such an error handler
         */
    }

    /*
     * Prioritise secure exceptions to avoid NS being able to pre-empt
     * secure SVC or SecureFault. Do it before PSA API initialization.
     */
    if (tfm_core_set_secure_exception_priorities() != TFM_SUCCESS) {
        tfm_panic();
    }

    /* We close the TFM_SP_CORE_ID partition, because its only purpose is
     * to be able to pass the state checks for the tests started from secure.
     */
    tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_CLOSED);
    tfm_spm_partition_set_state(TFM_SP_NON_SECURE_ID,
                                SPM_PARTITION_STATE_RUNNING);

#ifdef TFM_CORE_DEBUG
    /* Jumps to non-secure code */
    LOG_MSG("Jumping to non-secure code...");
#endif

    jump_to_ns_code();
#else /* !defined(TFM_PSA_API) */
    /*
     * Prioritise secure exceptions to avoid NS being able to pre-empt
     * secure SVC or SecureFault. Do it before PSA API initialization.
     */
    if (tfm_core_set_secure_exception_priorities() != TFM_SUCCESS) {
        tfm_panic();
    }
    tfm_spm_init();
#endif /* !defined(TFM_PSA_API) */
}
