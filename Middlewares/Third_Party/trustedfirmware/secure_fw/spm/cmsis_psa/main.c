/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fih.h"
#include "ffm/tfm_boot_data.h"
#include "region.h"
#include "spm_ipc.h"
#include "tfm_hal_platform.h"
#include "tfm_hal_isolation.h"
#include "tfm_irq_list.h"
#include "tfm_nspm.h"
#include "tfm_spm_hal.h"
#include "tfm_spm_log.h"
#include "tfm_version.h"
#include "uart_stdout.h"

/*
 * Avoids the semihosting issue
 * FixMe: describe 'semihosting issue'
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#elif (TFM_LVL != 1) && (TFM_LVL != 2) && (TFM_LVL != 3)
#error Invalid TFM_LVL value. Only TFM_LVL 1, 2 and 3 are supported in IPC model!
#endif

REGION_DECLARE(Image$$, ARM_LIB_STACK_MSP,  $$ZI$$Base);

static fih_int tfm_core_init(void)
{
    size_t i;
    enum tfm_hal_status_t hal_status = TFM_HAL_ERROR_GENERIC;
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
    enum irq_target_state_t irq_target_state = TFM_IRQ_TARGET_STATE_SECURE;
#ifdef TFM_FIH_PROFILE_ON
    fih_int fih_rc = FIH_FAILURE;
#endif

    /* init uart to be able to print trace in early SPM init */
    stdio_init();

    /* Enables fault handlers */
    plat_err = tfm_spm_hal_enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    /* Configures the system reset request properties */
    plat_err = tfm_spm_hal_system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    /* Configures debug authentication */
#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_spm_hal_init_debug, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }
#else /* TFM_FIH_PROFILE_ON */
    plat_err = tfm_spm_hal_init_debug();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }
#endif /* TFM_FIH_PROFILE_ON */

    /*
     * Access to any peripheral should be performed after programming
     * the necessary security components such as PPC/SAU.
     */
#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_hal_set_up_static_boundaries, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }
#else /* TFM_FIH_PROFILE_ON */
    hal_status = tfm_hal_set_up_static_boundaries();
    if (hal_status != TFM_HAL_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }
#endif /* TFM_FIH_PROFILE_ON */

#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_spm_hal_verify_isolation_hw, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
        tfm_core_panic();
    }
#endif

    /* Performs platform specific initialization */
    hal_status = tfm_hal_platform_init();
    if (hal_status != TFM_HAL_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    /* Configures architecture */
    tfm_arch_config_extensions();

    SPMLOG_INFMSG("\033[1;34m[Sec Thread] Secure image initializing!\033[0m\r\n");

    SPMLOG_DBGMSGVAL("TF-M isolation level is: ", TFM_LVL);

    tfm_core_validate_boot_data();

    configure_ns_code();

    /* Configures all interrupts to retarget NS state, except for
     * secure peripherals
     */
    plat_err = tfm_spm_hal_nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    for (i = 0; i < tfm_core_irq_signals_count; ++i) {
        plat_err = tfm_spm_hal_set_secure_irq_priority(
                                          tfm_core_irq_signals[i].irq_line,
                                          tfm_core_irq_signals[i].irq_priority);
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
        irq_target_state = tfm_spm_hal_set_irq_target_state(
                                          tfm_core_irq_signals[i].irq_line,
                                          TFM_IRQ_TARGET_STATE_SECURE);
        if (irq_target_state != TFM_IRQ_TARGET_STATE_SECURE) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
    }

    /* Enable secure peripherals interrupts */
    plat_err = tfm_spm_hal_nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    FIH_RET(fih_int_encode(TFM_SUCCESS));
}

int main(void)
{
    fih_int fih_rc = FIH_FAILURE;

    /* set Main Stack Pointer limit */
    tfm_arch_init_secure_msp((uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK_MSP,
                                               $$ZI$$Base));

    fih_delay_init();

    FIH_CALL(tfm_core_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_SUCCESS))) {
        tfm_core_panic();
    }

    /* All isolation should have been set up at this point */
    FIH_LABEL_CRITICAL_POINT();

    /* Print the TF-M version */
    SPMLOG_INFMSG("\033[1;34mBooting TFM v"VERSION_FULLSTR"\033[0m\r\n");

    /*
     * Prioritise secure exceptions to avoid NS being able to pre-empt
     * secure SVC or SecureFault. Do it before PSA API initialization.
     */
    tfm_arch_set_secure_exception_priorities();

    /* Move to handler mode for further SPM initialization. */
    tfm_core_handler_mode();
}
