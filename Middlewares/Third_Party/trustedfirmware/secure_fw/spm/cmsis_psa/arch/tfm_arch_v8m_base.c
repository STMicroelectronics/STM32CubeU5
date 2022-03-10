/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "spm_ipc.h"
#include "tfm_hal_device_header.h"
#include "tfm_arch.h"
#include "exception_info.h"
#include "tfm_secure_api.h"
#include "tfm/tfm_core_svc.h"

#if !defined(__ARM_ARCH_8M_BASE__)
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
        "push    {r4-r7}                    \n"
        "mov     r4, r8                     \n"
        "mov     r5, r9                     \n"
        "mov     r6, r10                    \n"
        "mov     r7, r11                    \n"
        "push    {r4-r7}                    \n"
        "mov     r0, sp                     \n"
        "bl      tfm_pendsv_do_schedule     \n"
        "pop     {r4-r7}                    \n"
        "mov     r8, r4                     \n"
        "mov     r9, r5                     \n"
        "mov     r10, r6                    \n"
        "mov     r11, r7                    \n"
        "pop     {r4-r7}                    \n"
        "pop     {r0-r3}                    \n"
        "mov     lr, r3                     \n"
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
 * \brief Overwrites default Hard fault handler.
 *
 * In case of a baseline implementation fault conditions that would generate a
 * SecureFault in a mainline implementation instead generate a Secure HardFault.
 */
__attribute__((naked)) void HardFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_HARDFAULT);

    /* In a baseline implementation there is no way, to find out whether this is
     * a hard fault triggered directly, or another fault that has been
     * escalated.
     */
    /* A HardFault may indicate corruption of secure state, so it is essential
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

/* There are no coprocessors in Armv8-M Baseline implementations */
void tfm_arch_config_extensions(void)
{
}

/* There is no FPCA in baseline. */
void tfm_arch_clear_fp_status(void)
{
}
