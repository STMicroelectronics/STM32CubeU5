/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_ARCH_V6M_V7M_H__
#define __TFM_ARCH_V6M_V7M_H__

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_compiler.h"
#include "utilities.h"

#if !TFM_MULTI_CORE_TOPOLOGY
#error "Armv6-M/Armv7-M can only support multi-core TF-M now."
#endif

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#define EXC_RETURN_FPU_FRAME_BASIC              (1 << 4)
#endif

/* Initial EXC_RETURN value in LR when a thread is loaded at the first time */
#define EXC_RETURN_THREAD_S_PSP                 0xFFFFFFFD

/* Exception return behavior */

/* stack pointer used to restore context: 0=MSP 1=PSP. */
#define EXC_RETURN_SPSEL    (1UL << 2)
/* processor mode for return: 0=Handler mode 1=Thread mod. */
#define EXC_RETURN_MODE     (1UL << 3)

struct tfm_arch_ctx_t {
    uint32_t    r8;
    uint32_t    r9;
    uint32_t    r10;
    uint32_t    r11;
    uint32_t    r4;
    uint32_t    r5;
    uint32_t    r6;
    uint32_t    r7;
    uint32_t    sp;
    uint32_t    lr;
};

/**
 * \brief Check whether Secure or Non-secure stack is used to restore stack
 *        frame on exception return.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             Always return to Secure stack on secure core in
 *                          multi-core topology.
 */
__STATIC_INLINE bool is_return_secure_stack(uint32_t lr)
{
    (void)lr;

    return true;
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
/**
 * \brief Check whether the stack frame for this exception has space allocated
 *        for Floating Point(FP) state information.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             The stack allocates space for FP information
 * \retval false            The stack doesn't allocate space for FP information
 */
__STATIC_INLINE bool is_stack_alloc_fp_space(uint32_t lr)
{
    return (lr & EXC_RETURN_FPU_FRAME_BASIC) ? false : true;
}
#elif defined(__ARM_ARCH_6M__)
/**
 * \brief Check whether the stack frame for this exception has space allocated
 *        for Floating Point(FP) state information.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval false            The stack doesn't allocate space for FP information
 */
__STATIC_INLINE bool is_stack_alloc_fp_space(uint32_t lr)
{
    (void)lr;

    return false;
}
#endif

/**
 * \brief Set PSP limit value.
 *
 * \param[in] psplim        PSP limit value to be written.
 */
__STATIC_INLINE void tfm_arch_set_psplim(uint32_t psplim)
{
    /*
     * Defined as an empty function now.
     * The PSP limit value can be used in more strict memory check.
     */
    (void)psplim;
}

/**
 * \brief Seal the thread stack.
 *
 * \param[in] stk        Thread stack address.
 *
 * \retval stack         Updated thread stack address.
 */
__STATIC_INLINE uintptr_t tfm_arch_seal_thread_stack(uintptr_t stk)
{
    TFM_CORE_ASSERT((stk & 0x7) == 0);
    return stk;
}

/**
 * \brief Update architecture context value into hardware
 *
 * \param[in] p_actx        Pointer of context data
 */
__STATIC_INLINE void tfm_arch_update_ctx(struct tfm_arch_ctx_t *p_actx)
{
    __set_PSP(p_actx->sp);
}

/**
 * \brief Secure the MSP
 *
 * \param[in] msplim        MSP limit value to be written.
 */
__STATIC_INLINE void tfm_arch_init_secure_msp(uint32_t msplim)
{
    /*
     * Defined as an empty function now.
     * The MSP limit value can be used in more strict memory check.
     */
    (void)msplim;
}

/**
 * \brief Whether in privileged level
 *
 * \retval true             If current execution runs in privileged level.
 * \retval false            If current execution runs in unprivileged level.
 */
__STATIC_INLINE bool tfm_arch_is_priv(void)
{
    CONTROL_Type ctrl;

    /* If in Handler mode */
    if (__get_IPSR()) {
        return true;
    }

    /* If in privileged Thread mode */
    ctrl.w = __get_CONTROL();
    if (!ctrl.b.nPRIV) {
        return true;
    }

    return false;
}

#endif
