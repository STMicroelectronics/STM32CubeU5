/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SECURE_UTILITIES_H__
#define __SECURE_UTILITIES_H__

#include <stdbool.h>
#include "cmsis_compiler.h"
#include "tfm_svc.h"
#include "tfm_core_log.h"

#define EXC_NUM_THREAD_MODE     (0)
#define EXC_NUM_SVCALL          (11)
#define EXC_NUM_PENDSV          (14)
#define EXC_NUM_SYSTICK         (15)

#define SEC_HDLR_STR            "[Sec Handler] "
#define SEC_THRD_STR            "[Sec Thread] "

#ifdef TFM_CORE_DEBUG
#define LOG_MSG_HDLR(MSG)                           \
        do {                                        \
            tfm_core_log_puts(SEC_HDLR_STR);        \
            tfm_core_log_puts(MSG);                 \
            tfm_core_log_puts("\r\n");              \
        } while (0)
#else
/* FixMe: redirect to secure log area */
#define LOG_MSG_HDLR(MSG)                           \
        do {                                        \
            tfm_core_log_puts(SEC_HDLR_STR);        \
            tfm_core_log_puts(MSG);                 \
            tfm_core_log_puts("\r\n");              \
        } while (0)
#endif

#define TFM_CORE_MSG(MSG)                                            \
        do {                                                         \
            /* FixMe: call safe function "tfm_core_log_put_buf" */   \
            tfm_core_log_puts("\033[1;34m"SEC_THRD_STR);             \
            tfm_core_log_puts(MSG);                                  \
            tfm_core_log_puts("\033[0m\r\n");                        \
        } while (0)

#ifdef TFM_CORE_DEBUG
#define TFM_LVL_MSG(LEVEL)                          \
        do {                                        \
            tfm_core_log_puts("TFM level is: ");    \
            tfm_core_log_hex(LEVEL);                \
            tfm_core_log_puts("\r\n");              \
        } while (0)
#endif

#define LOG_MSG_THR(MSG)                                             \
            __ASM volatile("MOV r0, %0\n"                            \
                           "SVC %1\n"                                \
                           : : "r" (MSG), "I" (TFM_SVC_PRINT))

#define LOG_MSG(MSG)                                \
            do {                                    \
                if (__get_active_exc_num()) {       \
                    LOG_MSG_HDLR(MSG);              \
                } else {                            \
                    LOG_MSG_THR(MSG);               \
                }                                   \
            } while (0)

#ifdef TFM_CORE_DEBUG
#define ERROR_MSG(MSG)                              \
        do {                                        \
            tfm_core_log_puts(SEC_HDLR_STR);        \
            tfm_core_log_puts(MSG);                 \
            tfm_core_log_puts("\r\n");              \
        } while (0)
#else
/* FixMe: redirect to secure log area */
#define ERROR_MSG(MSG)                              \
        do {                                        \
            tfm_core_log_puts(SEC_HDLR_STR);        \
            tfm_core_log_puts(MSG);                 \
            tfm_core_log_puts("\r\n");              \
        } while (0)
#endif

int32_t tfm_bitcount(uint32_t n);

bool tfm_is_one_bit_set(uint32_t n);

#endif /* __SECURE_UTILITIES_H__ */
