/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CORE_SVC_H__
#define __TFM_CORE_SVC_H__

#include "tfm_hal_device_header.h"
#include "tfm_spm_log.h"

typedef enum {
    TFM_SVC_SFN_REQUEST = 0,
    TFM_SVC_SFN_RETURN,
    TFM_SVC_GET_CALLER_CLIENT_ID,
    TFM_SVC_SPM_REQUEST,
    TFM_SVC_GET_BOOT_DATA,
    TFM_SVC_DEPRIV_REQ,
    TFM_SVC_DEPRIV_RET,
#ifndef TFM_PSA_API
    TFM_SVC_ENABLE_IRQ,
    TFM_SVC_DISABLE_IRQ,
#endif
    TFM_SVC_PSA_WAIT,
    TFM_SVC_PSA_EOI,
    TFM_SVC_HANDLER_MODE,
#ifdef TFM_PSA_API
    /* PSA Client SVC */
    TFM_SVC_PSA_FRAMEWORK_VERSION,
    TFM_SVC_PSA_VERSION,
    TFM_SVC_PSA_CONNECT,
    TFM_SVC_PSA_CALL,
    TFM_SVC_PSA_CLOSE,
    /* PSA Service SVC */
    TFM_SVC_PSA_GET,
    TFM_SVC_PSA_SET_RHANDLE,
    TFM_SVC_PSA_READ,
    TFM_SVC_PSA_SKIP,
    TFM_SVC_PSA_WRITE,
    TFM_SVC_PSA_REPLY,
    TFM_SVC_PSA_NOTIFY,
    TFM_SVC_PSA_CLEAR,
    TFM_SVC_PSA_PANIC,
    TFM_SVC_PSA_LIFECYCLE,
#endif
#if (TFM_SPM_LOG_LEVEL > TFM_SPM_LOG_LEVEL_SILENCE)
    TFM_SVC_OUTPUT_UNPRIV_STRING,
#endif
    /* Secure Partition API for interrupt control */
    TFM_SVC_PSA_IRQ_ENABLE,
    TFM_SVC_PSA_IRQ_DISABLE,
    TFM_SVC_GET_BOOT_VALUE, /* additional boot data service */

    TFM_SVC_PLATFORM_BASE = 50 /* leave room for additional Core handlers */
} tfm_svc_number_t;

#define SVC(code) __ASM volatile("svc %0" : : "I" (code))

#endif /* __TFM_CORE_SVC_H__ */
