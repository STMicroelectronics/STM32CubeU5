/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "tfm_hal_device_header.h"
#include "region_defs.h"
#include "tfm_arch.h"
#include "tfm_memory_utils.h"
#include "tfm_core_utils.h"
#include "exception_info.h"
#include "tfm_secure_api.h"
#include "spm_ipc.h"
#include "tfm/tfm_core_svc.h"

#if !defined(__ARM_ARCH_8M_MAIN__) && !defined(__ARM_ARCH_8_1M_MAIN__)
#error "Unsupported ARM Architecture."
#endif

/*
 * Stack status at PendSV entry:
 *
 *                                            [ R0 - R3  ]<- PSP
 *                                            [ R12      ]
 *                                            [ LR_of_RA ]
 *                       MSP->[ ........ ]    [ RA       ]
 *                            [ ........ ]    [ XPSR     ]
 *                                            [ ........ ]
 *                                            [ ........ ]
 *
 * Stack status before calling pendsv_do_schedule():
 *
 *                       MSP->[ R4 - R11 ]
 *                            [ PSP      ]--->[ R0 - R3  ]
 *                            [ PSP Limit]    [ R12      ]
 *                            [ R2(dummy)]    [ LR_of_RA ]
 *                            [ LR       ]    [ RA       ]
 *                            [ ........ ]    [ XPSR     ]
 *                            [ ........ ]    [ ........ ]
 *                                            [ ........ ]
 *
 * tfm_pendsv_do_schedule() updates stacked context into current thread and
 * replace stacked context with context of next thread.
 *
 * Scheduler does not support handler mode thread so take PSP/PSP_LIMIT as
 * thread SP/SP_LIMIT. R2 holds dummy data due to stack operation is 8 bytes
 * aligned.
 */
#if defined(__ICCARM__)
#pragma required = tfm_pendsv_do_schedule
#endif

__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
        "mrs     r0, psp                    \n"
        "mrs     r1, psplim                 \n"
        "push    {r0, r1, r2, lr}           \n"
        "push    {r4-r11}                   \n"
        "mov     r0, sp                     \n"
        "bl      tfm_pendsv_do_schedule     \n"
        "pop     {r4-r11}                   \n"
        "pop     {r0, r1, r2, lr}           \n"
        "msr     psp, r0                    \n"
        "msr     psplim, r1                 \n"
        "bx      lr                         \n"
    );
}

void tfm_arch_init_actx(struct tfm_arch_ctx_t *p_actx,
                        uint32_t sp, uint32_t sp_limit)
{
    p_actx->sp = sp;
    p_actx->sp_limit = sp_limit;
    p_actx->lr = EXC_RETURN_THREAD_S_PSP;
}

/**
 * \brief Overwrites default Secure fault handler.
 */
__attribute__((naked)) void SecureFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_SECUREFAULT);

    /* A SecureFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

#if defined(__ICCARM__)
uint32_t tfm_core_svc_handler(uint32_t *msp, uint32_t *psp, uint32_t exc_return);
#pragma required = tfm_core_svc_handler
#endif

__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r0, MSP                        \n"
    "MRS     r1, PSP                        \n"
    "MOV     r2, lr                         \n"
    "BL      tfm_core_svc_handler           \n"
    "BX      r0                             \n"
    );
}

/* Reserved for future usage */
__attribute__((naked)) void HardFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_HARDFAULT);

    /* A HardFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void MemManage_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_MEMFAULT);

    /* A MemManage fault may indicate corruption of secure state, so it is
     * essential that Non-secure code does not regain control after one is
     * raised. Returning from this exception could allow a pending NS exception
     * to be taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void BusFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_BUSFAULT);

    /* A BusFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void UsageFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_USAGEFAULT);
    __ASM volatile("b    .");
}

void tfm_arch_set_secure_exception_priorities(void)
{
    uint32_t VECTKEY;
    SCB_Type *scb = SCB;
    uint32_t AIRCR;

    /* Set PRIS flag in AIRCR */
    AIRCR = scb->AIRCR;
    VECTKEY = (~AIRCR & SCB_AIRCR_VECTKEYSTAT_Msk);
    scb->AIRCR = SCB_AIRCR_PRIS_Msk |
                 VECTKEY |
                 (AIRCR & ~SCB_AIRCR_VECTKEY_Msk);
    /* Set fault priority to less than 0x80 (with AIRCR.PRIS set) to prevent
     * Non-secure from pre-empting faults that may indicate corruption of Secure
     * state.
     */
    NVIC_SetPriority(MemoryManagement_IRQn, 0);
    NVIC_SetPriority(BusFault_IRQn, 0);
    NVIC_SetPriority(SecureFault_IRQn, 0);

    NVIC_SetPriority(SVCall_IRQn, 0);
#ifdef TFM_MULTI_CORE_TOPOLOGY
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
#else
    /*
     * Set secure PendSV priority to the lowest in SECURE state.
     *
     * IMPORTANT NOTE:
     *
     * Although the priority of the secure PendSV must be the lowest possible
     * among other interrupts in the Secure state, it must be ensured that
     * PendSV is not preempted nor masked by Non-Secure interrupts to ensure
     * the integrity of the Secure operation.
     * When AIRCR.PRIS is set, the Non-Secure execution can act on
     * FAULTMASK_NS, PRIMASK_NS or BASEPRI_NS register to boost its priority
     * number up to the value 0x80.
     * For this reason, set the priority of the PendSV interrupt to the next
     * priority level configurable on the platform, just below 0x80.
     */
    NVIC_SetPriority(PendSV_IRQn, (1 << (__NVIC_PRIO_BITS - 1)) - 1);
#endif
}

void tfm_arch_config_extensions(void)
{
#if defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)
    /* Configure Secure access to the FPU only if the secure image is being
     * built with the FPU in use. This avoids introducing extra interrupt
     * latency when the FPU is not used by the SPE.
     */
#if defined (__FPU_USED) && (__FPU_USED == 1U)
    /* Enable Secure privileged and unprivilged access to the FP Extension */
    SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                  | (3U << 11U*2U);  /* enable CP11 full access */

    /* If the SPE will ever use the floating-point registers for sensitive data,
     * then FPCCR.TS, FPCCR.CLRONRET and FPCCR.CLRONRETS must be set at
     * initialisation and not changed again afterwards.
     */
    FPU->FPCCR |= FPU_FPCCR_TS_Msk
                  | FPU_FPCCR_CLRONRET_Msk
                  | FPU_FPCCR_CLRONRETS_Msk;
#endif

    /* Permit Non-secure access to the Floating-point Extension.
     * Note: It is still necessary to set CPACR_NS to enable the FP Extension in
     * the NSPE. This configuration is left to NS privileged software.
     */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;

#if defined(__ARM_ARCH_8_1M_MAIN__)
    SCB->CCR |= SCB_CCR_TRD_Msk;
#endif
#endif
}

__attribute__((naked, noinline)) void tfm_arch_clear_fp_status(void)
{
    __ASM volatile(
                   "mrs  r0, control         \n"
                   "bics r0, r0, #4          \n"
                   "msr  control, r0         \n"
                   "isb                      \n"
                   "bx   lr                  \n"
                  );
}
