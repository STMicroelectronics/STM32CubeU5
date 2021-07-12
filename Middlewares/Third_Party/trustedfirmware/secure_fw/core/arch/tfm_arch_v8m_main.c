/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "cmsis.h"
#include "region_defs.h"
#include "secure_utilities.h"
#include "tfm_arch.h"
#include "tfm_memory_utils.h"
#include "tfm_secure_api.h"
#include "spm_api.h"
#include "tfm_svc.h"

#if !defined(__ARM_ARCH_8M_MAIN__)
#error "Unsupported ARM Architecture."
#endif
extern void Error_Handler(void);
struct tfm_fault_context_s {
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t ReturnAddress;
    uint32_t RETPSR;
} tfm_fault_context;

extern uint32_t SVCHandler_main(uint32_t *svc_args, uint32_t lr, uint32_t *msp);

#ifdef TFM_PSA_API
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
#pragma required=tfm_pendsv_do_schedule
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

void tfm_arch_initialize_ctx_ext(struct tfm_state_context_ext *p_ctxb,
                                 uint32_t sp, uint32_t sp_limit)
{
    p_ctxb->sp = sp;
    p_ctxb->sp_limit = sp_limit;
    p_ctxb->lr = INIT_LR_UNPRIVILEGED;
}
#else
__attribute__((section("SFN"), naked))
int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
        "PUSH   {r4-r12, lr}                \n"
        "SVC    %[SVC_REQ]                  \n"
        "MOV    r4,  #0                     \n"
        "MOV    r5,  #0                     \n"
        "MOV    r6,  #0                     \n"
        "MOV    r7,  #0                     \n"
        "MOV    r8,  #0                     \n"
        "MOV    r9,  #0                     \n"
        "MOV    r10, #0                     \n"
        "MOV    r11, #0                     \n"
        "BLX    lr                          \n"
        "SVC    %[SVC_RET]                  \n"
        "POP    {r4-r12, pc}                \n"
        : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST),
            [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
        : "r0");
}

__attribute__((section("SFN"), naked))
void priv_irq_handler_main(uint32_t partition_id,
                                                  uint32_t unpriv_handler,
                                                  uint32_t irq_signal,
                                                  uint32_t irq_line)
{
    __ASM(
          /* Save the callee saved registers*/
          "PUSH   {r4-r12, lr}              \n"
          /* Request SVC to configure environment for the unpriv IRQ handler */
          "SVC    %[SVC_REQ]                \n"
          /* clear the callee saved registers to prevent information leak */
          "MOV    r4, #0                    \n"
          "MOV    r5, #0                    \n"
          "MOV    r6, #0                    \n"
          "MOV    r7, #0                    \n"
          "MOV    r8, #0                    \n"
          "MOV    r9, #0                    \n"
          "MOV    r10, #0                   \n"
          "MOV    r11, #0                   \n"
          /* Branch to the unprivileged handler */
          "BLX    lr                        \n"
          /* Request SVC to reconfigure the environment of the interrupted
           * partition
           */
          "SVC    %[SVC_RET]                \n"
            /* restore callee saved registers and return */
          "POP    {r4-r12, pc}              \n"
          : : [SVC_REQ] "I" (TFM_SVC_DEPRIV_REQ)
            , [SVC_RET] "I" (TFM_SVC_DEPRIV_RET)
          : "r0");
}
#endif

/**
 * \brief Overwrites default Secure fault handler.
 */
void SecureFault_Handler(void)
{
    /* figure out context from which we landed in fault handler */
    uint32_t lr = __get_LR();
    uint32_t sp;

    if (lr & EXC_RETURN_SECURE_STACK) {
        if (lr & EXC_RETURN_STACK_PROCESS) {
            sp = __get_PSP();
        } else {
            sp = __get_MSP();
        }
    } else {
        if (lr & EXC_RETURN_STACK_PROCESS) {
            sp =  __TZ_get_PSP_NS();
        } else {
            sp = __TZ_get_MSP_NS();
        }
    }

    /* Only save the context if sp is valid */
    if ((sp >=  S_DATA_START &&
         sp <=  (S_DATA_LIMIT - sizeof(tfm_fault_context)) + 1) ||
#if defined(NS_DATA_START_2) && defined(NS_DATA_LIMIT_2)
        (sp >= NS_DATA_START_2 &&
         sp <= (NS_DATA_LIMIT_2 - sizeof(tfm_fault_context)) + 1) ||
#endif
        (sp >= NS_DATA_START &&
         sp <= (NS_DATA_LIMIT - sizeof(tfm_fault_context)) + 1)) {
        tfm_memcpy(&tfm_fault_context,
                   (const void *)sp,
                   sizeof(tfm_fault_context));
    }

    LOG_MSG("Oops... Secure fault!!! You're not going anywhere!");
    Error_Handler();
}
#if 1

#if defined(__ICCARM__)
uint32_t tfm_core_svc_handler(uint32_t *svc_args, uint32_t exc_return);

#pragma required=tfm_core_svc_handler
#endif
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r2, MSP                        \n"
    /* Check store SP in thread mode to r0 */
    "TST     lr, #4                         \n"
    "ITE     EQ                             \n"
    "MOVEQ   r0, r2                         \n"
    "MRSNE   r0, PSP                        \n"
    "MOV     r1, lr                         \n"
    "BL      tfm_core_svc_handler           \n"
    "BX      r0                             \n"
    );
}
#endif
/* Reserved for future usage */
__attribute__((naked)) void MemManage_Handler(void)
{
    __ASM volatile("b    .");
}

__attribute__((naked)) void BusFault_Handler(void)
{
    __ASM volatile("b    .");
}
__attribute__((naked)) void UsageFault_Handler(void)
{
    __ASM volatile("b    .");
}

void tfm_arch_prioritize_secure_exception(void)
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
}
