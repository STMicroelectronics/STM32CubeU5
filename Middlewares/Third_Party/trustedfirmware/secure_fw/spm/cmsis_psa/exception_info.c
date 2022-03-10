/*
 * Copyright (c) 2021, Nordic Semiconductor ASA. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "tfm_arch.h"
#include "exception_info.h"
#include "tfm_spm_log.h"
#include "tfm_core_utils.h"

struct exception_info_t {
    uint32_t EXC_RETURN;        /* EXC_RETURN value in LR. */
    uint32_t MSP;               /* (Secure) MSP. */
    uint32_t PSP;               /* (Secure) PSP. */
    uint32_t *EXC_FRAME;        /* Exception frame on stack. */
    uint32_t EXC_FRAME_COPY[8]; /* Copy of the basic exception frame. */
    uint32_t xPSR;              /* Program Status Registers. */

#ifdef FAULT_STATUS_PRESENT
    uint32_t CFSR;              /* Configurable Fault Status Register. */
    uint32_t HFSR;              /* Hard Fault Status Register. */
    uint32_t BFAR;              /* Bus Fault address register. */
    uint32_t BFARVALID;         /* Whether BFAR contains a valid address. */
    uint32_t MMFAR;             /* MemManage Fault address register. */
    uint32_t MMARVALID;         /* Whether MMFAR contains a valid address. */
#ifdef TRUSTZONE_PRESENT
    uint32_t SFSR;              /* SecureFault Status Register. */
    uint32_t SFAR;              /* SecureFault Address Register. */
    uint32_t SFARVALID;         /* Whether SFAR contains a valid address. */
#endif

#endif
};

static struct exception_info_t exception_info;

/**
 * \brief Check whether the exception was triggered in thread or handler mode.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             The exception will return to thread mode.
 */
__STATIC_INLINE bool is_return_thread_mode(uint32_t lr)
{
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    return !((lr == EXC_RETURN_HANDLER) || (lr == EXC_RETURN_HANDLER_FPU));
#elif defined(__ARM_ARCH_8M_BASE__) || defined(__ARM_ARCH_8M_MAIN__) \
        || defined(__ARM_ARCH_8_1M_MAIN__)
    return (lr & EXC_RETURN_MODE);
#else
    return !(lr == EXC_RETURN_HANDLER);
#endif
}

/**
 * \brief Check whether the PSP or MSP is used to restore stack frame on
 *        exception return.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             The exception frame is on the PSP
 */
__STATIC_INLINE bool is_return_psp(uint32_t lr)
{
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    return ((lr == EXC_RETURN_THREAD_PSP) || (lr == EXC_RETURN_THREAD_PSP_FPU));
#elif defined(__ARM_ARCH_8M_BASE__) || defined(__ARM_ARCH_8M_MAIN__) \
        || defined(__ARM_ARCH_8_1M_MAIN__)
    /* PSP is used only if SPSEL is set, and we came from thread mode. */
    return ((lr & EXC_RETURN_SPSEL) && is_return_thread_mode(lr));
#else
    return (lr == EXC_RETURN_THREAD_PSP);
#endif
}

/**
 * \brief Get a pointer to the current exception frame
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 * \param[in] msp           The MSP at the start of the exception handler.
 * \param[in] psp           The PSP at the start of the exception handler.
 *
 * \return  A pointer to the current exception frame.
 */
__STATIC_INLINE
uint32_t *get_exception_frame(uint32_t lr, uint32_t msp, uint32_t psp)
{
#if defined(__ARM_ARCH_8M_BASE__) || defined(__ARM_ARCH_8M_MAIN__) \
        || defined(__ARM_ARCH_8_1M_MAIN__)
    bool is_psp = is_return_psp(lr);

    return (uint32_t *)(is_return_secure_stack(lr)
                        ? (is_psp ? psp : msp)
                        : (is_psp ? __TZ_get_PSP_NS() : __TZ_get_MSP_NS()));
#else
    return (uint32_t *)(is_return_psp(lr) ? psp : msp);
#endif
}

static void dump_exception_info_t(bool stack_error,
                                  struct exception_info_t *ctx)
{
    SPMLOG_DBGMSG("Here is some context for the exception:\n");
    SPMLOG_DBGMSGVAL("    EXC_RETURN (LR): ", ctx->EXC_RETURN);
    SPMLOG_DBGMSG("    Exception came from");
#ifdef TRUSTZONE_PRESENT
    if (is_return_secure_stack(ctx->EXC_RETURN)) {
        SPMLOG_DBGMSG(" secure FW in");
    } else {
        SPMLOG_DBGMSG(" non-secure FW in");
    }
#endif

    if (is_return_thread_mode(ctx->EXC_RETURN)) {
        SPMLOG_DBGMSG(" thread mode.\n");
    } else {
        SPMLOG_DBGMSG(" handler mode.\n");
    }
    SPMLOG_DBGMSGVAL("    xPSR:    ", ctx->xPSR);
    SPMLOG_DBGMSGVAL("    MSP:     ", ctx->MSP);
    SPMLOG_DBGMSGVAL("    PSP:     ", ctx->PSP);
#ifdef TRUSTZONE_PRESENT
    SPMLOG_DBGMSGVAL("    MSP_NS:  ", __TZ_get_MSP_NS());
    SPMLOG_DBGMSGVAL("    PSP_NS:  ", __TZ_get_PSP_NS());
#endif

    SPMLOG_DBGMSGVAL("    Exception frame at: ", (uint32_t)ctx->EXC_FRAME);
    if (stack_error) {
        SPMLOG_DBGMSG(
            "       (Note that the exception frame may be corrupted for this type of error.)\n");
    }
    SPMLOG_DBGMSGVAL("        R0:   ", ctx->EXC_FRAME_COPY[0]);
    SPMLOG_DBGMSGVAL("        R1:   ", ctx->EXC_FRAME_COPY[1]);
    SPMLOG_DBGMSGVAL("        R2:   ", ctx->EXC_FRAME_COPY[2]);
    SPMLOG_DBGMSGVAL("        R3:   ", ctx->EXC_FRAME_COPY[3]);
    SPMLOG_DBGMSGVAL("        R12:  ", ctx->EXC_FRAME_COPY[4]);
    SPMLOG_DBGMSGVAL("        LR:   ", ctx->EXC_FRAME_COPY[5]);
    SPMLOG_DBGMSGVAL("        PC:   ", ctx->EXC_FRAME_COPY[6]);
    SPMLOG_DBGMSGVAL("        xPSR: ", ctx->EXC_FRAME_COPY[7]);

#ifdef FAULT_STATUS_PRESENT
    SPMLOG_DBGMSGVAL("    CFSR:  ", ctx->CFSR);
    SPMLOG_DBGMSGVAL("    BFSR:  ",
                    (ctx->CFSR & SCB_CFSR_BUSFAULTSR_Msk) >> SCB_CFSR_BUSFAULTSR_Pos);
    if (ctx->BFARVALID) {
        SPMLOG_DBGMSGVAL("    BFAR: ", ctx->BFAR);
    } else {
        SPMLOG_DBGMSG("    BFAR:  Not Valid\n");
    }
    SPMLOG_DBGMSGVAL("    MMFSR: ",
                    (ctx->CFSR & SCB_CFSR_MEMFAULTSR_Msk) >> SCB_CFSR_MEMFAULTSR_Pos);
    if (ctx->MMARVALID) {
        SPMLOG_DBGMSGVAL("    MMFAR: ", ctx->MMFAR);
    } else {
        SPMLOG_DBGMSG("    MMFAR: Not Valid\n");
    }
    SPMLOG_DBGMSGVAL("    UFSR:  ",
                    (ctx->CFSR & SCB_CFSR_USGFAULTSR_Msk) >> SCB_CFSR_USGFAULTSR_Pos);
    SPMLOG_DBGMSGVAL("    HFSR:  ", ctx->HFSR);
#ifdef TRUSTZONE_PRESENT
    SPMLOG_DBGMSGVAL("    SFSR:  ", ctx->SFSR);
    if (ctx->SFARVALID) {
        SPMLOG_DBGMSGVAL("    SFAR: ", ctx->SFAR);
    } else {
        SPMLOG_DBGMSG("    SFAR: Not Valid\n");
    }
#endif

#endif
}

static void dump_error(uint32_t error_type)
{
    bool stack_error = false;

    SPMLOG_DBGMSG("FATAL ERROR: ");
    switch (error_type) {
    case EXCEPTION_TYPE_SECUREFAULT:
        SPMLOG_DBGMSG("SecureFault\n");
        break;
    case EXCEPTION_TYPE_HARDFAULT:
        SPMLOG_DBGMSG("HardFault\n");
        break;
    case EXCEPTION_TYPE_MEMFAULT:
        SPMLOG_DBGMSG("MemManage fault\n");
        stack_error = true;
        break;
    case EXCEPTION_TYPE_BUSFAULT:
        SPMLOG_DBGMSG("BusFault\n");
        stack_error = true;
        break;
    case EXCEPTION_TYPE_USAGEFAULT:
        SPMLOG_DBGMSG("UsageFault\n");
        stack_error = true;
        break;
    default:
        SPMLOG_DBGMSG("Unknown\n");
        break;
    }
    dump_exception_info_t(stack_error, &exception_info);
}

void store_and_dump_context(uint32_t LR_in, uint32_t MSP_in, uint32_t PSP_in,
                            uint32_t exception_type)
{
    struct exception_info_t *ctx = &exception_info;

    ctx->xPSR = __get_xPSR();
    ctx->EXC_RETURN = LR_in;
    ctx->MSP = MSP_in;
    ctx->PSP = PSP_in;
    ctx->EXC_FRAME = get_exception_frame(ctx->EXC_RETURN, ctx->MSP, ctx->PSP);
    spm_memcpy(ctx->EXC_FRAME_COPY, ctx->EXC_FRAME,
               sizeof(ctx->EXC_FRAME_COPY));

#ifdef FAULT_STATUS_PRESENT
    ctx->CFSR = SCB->CFSR;
    ctx->HFSR = SCB->HFSR;
    ctx->BFAR = SCB->BFAR;
    ctx->BFARVALID = ctx->CFSR & SCB_CFSR_BFARVALID_Msk;
    ctx->MMFAR = SCB->MMFAR;
    ctx->MMARVALID = ctx->CFSR & SCB_CFSR_MMARVALID_Msk;
    SCB->CFSR = ctx->CFSR; /* Clear bits. CFSR is write-one-to-clear. */
    SCB->HFSR = ctx->HFSR; /* Clear bits. HFSR is write-one-to-clear. */
#ifdef TRUSTZONE_PRESENT
    ctx->SFSR = SAU->SFSR;
    ctx->SFAR = SAU->SFAR;
    ctx->SFARVALID = ctx->SFSR & SAU_SFSR_SFARVALID_Msk;
    SAU->SFSR = ctx->SFSR; /* Clear bits. SFSR is write-one-to-clear. */
#endif
#endif

    dump_error(exception_type);
}
