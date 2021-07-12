/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_ARCH_H__
#define __TFM_ARCH_H__

/* This header file collects the architecture related operations. */

#include <inttypes.h>
#include "cmsis.h"
#include "cmsis_compiler.h"

#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__)
#include "tfm_arch_v8m.h"
#elif defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
      defined(__ARM_ARCH_7EM__)
#include "tfm_arch_v6m_v7m.h"
#else
#error "Unsupported ARM Architecture."
#endif

#define XPSR_T32            0x01000000

/* General core state context */
struct tfm_state_context_t {
    uint32_t    r0;
    uint32_t    r1;
    uint32_t    r2;
    uint32_t    r3;
    uint32_t    r12;
    uint32_t    lr;
    uint32_t    ra;
    uint32_t    xpsr;
};

/* Architecture specific thread context extension */
struct tfm_state_context_ext;

#define TFM_STATE_1ST_ARG(ctx)     \
                    (((struct tfm_state_context_t *)(ctx)->ctxb.sp)->r0)
#define TFM_STATE_RET_VAL(ctx)     \
                    (((struct tfm_state_context_t *)(ctx)->ctxb.sp)->r0)

__attribute__ ((always_inline))
__STATIC_INLINE void tfm_arch_trigger_pendsv(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/**
 * \brief Get Link Register
 * \details Returns the value of the Link Register (LR)
 * \return LR value
 */
__attribute__ ((always_inline)) __STATIC_INLINE uint32_t __get_LR(void)
{
    register uint32_t result;

    __ASM volatile ("MOV %0, LR\n" : "=r" (result));
    return result;
}

__attribute__ ((always_inline))
__STATIC_INLINE uint32_t __get_active_exc_num(void)
{
    IPSR_Type IPSR;

    /* if non-zero, exception is active. NOT banked S/NS */
    IPSR.w = __get_IPSR();
    return IPSR.b.ISR;
}

__attribute__ ((always_inline))
__STATIC_INLINE void __set_CONTROL_SPSEL(uint32_t SPSEL)
{
    CONTROL_Type ctrl;

    ctrl.w = __get_CONTROL();
    ctrl.b.SPSEL = SPSEL;
    __set_CONTROL(ctrl.w);
    __ISB();
}

/*
 * Initialize CPU architecture specific thread context extension
 */
void tfm_arch_initialize_ctx_ext(struct tfm_state_context_ext *p_ctxb,
                                 uint32_t sp, uint32_t sp_limit);

/*
 * Prioritize Secure exceptions
 */
void tfm_arch_prioritize_secure_exception(void);

#endif
