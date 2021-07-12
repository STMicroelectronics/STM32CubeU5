/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_SPM_DB_INC__
#define __TFM_SPM_DB_INC__

#include "spm_api.h"

/**************************************************************************/
/** IRQ count per partition */
/**************************************************************************/
#define TFM_PARTITION_TFM_SP_STORAGE_IRQ_COUNT 0

#define TFM_PARTITION_TFM_SP_ITS_IRQ_COUNT 0

#ifdef TFM_PARTITION_AUDIT_LOG
#define TFM_PARTITION_TFM_SP_AUDIT_LOG_IRQ_COUNT 0
#endif /* TFM_PARTITION_AUDIT_LOG */

#define TFM_PARTITION_TFM_SP_CRYPTO_IRQ_COUNT 0

#ifdef TFM_PARTITION_PLATFORM
#define TFM_PARTITION_TFM_SP_PLATFORM_IRQ_COUNT 2
#endif /* TFM_PARTITION_PLATFORM */

#define TFM_PARTITION_TFM_SP_INITIAL_ATTESTATION_IRQ_COUNT 0

#ifdef TFM_PARTITION_TEST_CORE
#define TFM_PARTITION_TFM_SP_CORE_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
#define TFM_PARTITION_TFM_SP_CORE_TEST_2_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#define TFM_PARTITION_TFM_SP_SECURE_TEST_PARTITION_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#define TFM_PARTITION_TFM_SP_IPC_SERVICE_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#define TFM_PARTITION_TFM_SP_IPC_CLIENT_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
#define TFM_PARTITION_TFM_IRQ_TEST_1_IRQ_COUNT 1
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
#define TFM_PARTITION_TFM_APP_ROT_IRQ_COUNT 0
#endif /* TFM_PARTITION_APP_ROT */

/**************************************************************************/
/** Declarations of partition init functions */
/**************************************************************************/
#ifdef TFM_PSA_API
extern psa_status_t tfm_nspm_thread_entry(void);
#endif

extern int32_t tfm_sst_req_mngr_init(void);

extern int32_t tfm_its_req_mngr_init(void);

#ifdef TFM_PARTITION_AUDIT_LOG
extern int32_t audit_core_init(void);
#endif /* TFM_PARTITION_AUDIT_LOG */

extern int32_t tfm_crypto_init(void);

#ifdef TFM_PARTITION_PLATFORM
extern int32_t platform_sp_init(void);
#endif /* TFM_PARTITION_PLATFORM */

extern int32_t attest_partition_init(void);

#ifdef TFM_PARTITION_TEST_CORE
extern int32_t core_test_init(void);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
extern int32_t core_test_2_init(void);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
extern int32_t tfm_secure_client_service_init(void);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
extern int32_t ipc_service_test_main(void);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
extern int32_t ipc_client_test_main(void);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
extern int32_t tfm_irq_test_1_init(void);
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
extern int32_t app_rot_init(void);
#endif /* TFM_PARTITION_APP_ROT */

/**************************************************************************/
/** Memory region declarations */
/**************************************************************************/
#ifdef TFM_PSA_API
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);

REGION_DECLARE(Image$$, TFM_SP_STORAGE, $$Base);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_STORAGE, _STACK$$ZI$$Limit);

REGION_DECLARE(Image$$, TFM_SP_ITS, $$Base);
REGION_DECLARE(Image$$, TFM_SP_ITS, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_ITS, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_ITS, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_ITS, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_ITS, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_ITS, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_ITS, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_ITS, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_ITS, _STACK$$ZI$$Limit);

#ifdef TFM_PARTITION_AUDIT_LOG
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, $$Base);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_AUDIT_LOG, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_AUDIT_LOG */

REGION_DECLARE(Image$$, TFM_SP_CRYPTO, $$Base);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CRYPTO, _STACK$$ZI$$Limit);

#ifdef TFM_PARTITION_PLATFORM
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, $$Base);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_PLATFORM, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_PLATFORM */

REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, $$Base);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_INITIAL_ATTESTATION, _STACK$$ZI$$Limit);

#ifdef TFM_PARTITION_TEST_CORE
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, $$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, $$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_CORE_TEST_2, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, $$Base);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_SECURE_TEST_PARTITION, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, $$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_SERVICE_TEST, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, $$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, $$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_IPC_CLIENT_TEST, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, $$Base);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, $$Limit);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_IRQ_TEST_1, _STACK$$ZI$$Limit);
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
REGION_DECLARE(Image$$, TFM_APP_ROT, $$Base);
REGION_DECLARE(Image$$, TFM_APP_ROT, $$Limit);
REGION_DECLARE(Image$$, TFM_APP_ROT, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_APP_ROT, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_ROT, _DATA$$RW$$Base);
REGION_DECLARE(Image$$, TFM_APP_ROT, _DATA$$RW$$Limit);
REGION_DECLARE(Image$$, TFM_APP_ROT, _DATA$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_APP_ROT, _DATA$$ZI$$Limit);
REGION_DECLARE(Image$$, TFM_APP_ROT, _STACK$$ZI$$Base);
REGION_DECLARE(Image$$, TFM_APP_ROT, _STACK$$ZI$$Limit);
#endif /* TFM_PARTITION_APP_ROT */

#endif /* defined(TFM_PSA_API) */

#ifndef TFM_PSA_API
/**************************************************************************/
/** Context stacks for IRQ handling */
/**************************************************************************/
/* The max size of the context stack can be calculated as a function of the IRQ
 * count of the secure partition:
 *
 * max_stack_size = intr_ctx_size + (IRQ_CNT * (intr_ctx_size + hndl_ctx_size))
 *
 * where:
 *   intr_ctx: Frame pushed when the partition is interrupted
 *   hndl_ctx: Frame pushed when the partition is handling an interrupt
 */
static uint32_t ns_interrupt_ctx_stack[
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t)];

static uint32_t tfm_core_interrupt_ctx_stack[
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t)];

static uint32_t ctx_stack_TFM_SP_STORAGE[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_STORAGE_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];

static uint32_t ctx_stack_TFM_SP_ITS[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_ITS_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];

#ifdef TFM_PARTITION_AUDIT_LOG
static uint32_t ctx_stack_TFM_SP_AUDIT_LOG[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_AUDIT_LOG_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_AUDIT_LOG */

static uint32_t ctx_stack_TFM_SP_CRYPTO[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CRYPTO_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];

#ifdef TFM_PARTITION_PLATFORM
static uint32_t ctx_stack_TFM_SP_PLATFORM[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_PLATFORM_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_PLATFORM */

static uint32_t ctx_stack_TFM_SP_INITIAL_ATTESTATION[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_INITIAL_ATTESTATION_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];

#ifdef TFM_PARTITION_TEST_CORE
static uint32_t ctx_stack_TFM_SP_CORE_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CORE_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
static uint32_t ctx_stack_TFM_SP_CORE_TEST_2[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CORE_TEST_2_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
static uint32_t ctx_stack_TFM_SP_SECURE_TEST_PARTITION[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_SECURE_TEST_PARTITION_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
static uint32_t ctx_stack_TFM_SP_IPC_SERVICE_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_IPC_SERVICE_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
static uint32_t ctx_stack_TFM_SP_IPC_CLIENT_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_IPC_CLIENT_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
static uint32_t ctx_stack_TFM_IRQ_TEST_1[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_IRQ_TEST_1_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
static uint32_t ctx_stack_TFM_APP_ROT[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_APP_ROT_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_APP_ROT */


uint32_t *ctx_stack_list[] =
{
    ns_interrupt_ctx_stack,
    tfm_core_interrupt_ctx_stack,
    ctx_stack_TFM_SP_STORAGE,
    ctx_stack_TFM_SP_ITS,
#ifdef TFM_PARTITION_AUDIT_LOG
    ctx_stack_TFM_SP_AUDIT_LOG,
#endif /* TFM_PARTITION_AUDIT_LOG */
    ctx_stack_TFM_SP_CRYPTO,
#ifdef TFM_PARTITION_PLATFORM
    ctx_stack_TFM_SP_PLATFORM,
#endif /* TFM_PARTITION_PLATFORM */
    ctx_stack_TFM_SP_INITIAL_ATTESTATION,
#ifdef TFM_PARTITION_TEST_CORE
    ctx_stack_TFM_SP_CORE_TEST,
#endif /* TFM_PARTITION_TEST_CORE */
#ifdef TFM_PARTITION_TEST_CORE
    ctx_stack_TFM_SP_CORE_TEST_2,
#endif /* TFM_PARTITION_TEST_CORE */
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    ctx_stack_TFM_SP_SECURE_TEST_PARTITION,
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */
#ifdef TFM_PARTITION_TEST_CORE_IPC
    ctx_stack_TFM_SP_IPC_SERVICE_TEST,
#endif /* TFM_PARTITION_TEST_CORE_IPC */
#ifdef TFM_PARTITION_TEST_CORE_IPC
    ctx_stack_TFM_SP_IPC_CLIENT_TEST,
#endif /* TFM_PARTITION_TEST_CORE_IPC */
#ifdef TFM_ENABLE_IRQ_TEST
    ctx_stack_TFM_IRQ_TEST_1,
#endif /* TFM_ENABLE_IRQ_TEST */
#ifdef TFM_PARTITION_APP_ROT
    ctx_stack_TFM_APP_ROT,
#endif /* TFM_PARTITION_APP_ROT */
};
#endif /* !defined(TFM_PSA_API) */

/**************************************************************************/
/** The static data of the partition list */
/**************************************************************************/
const struct spm_partition_static_data_t static_data_list[] =
{
    {
        .partition_id         = TFM_SP_NON_SECURE_ID,
#ifdef TFM_PSA_API
        .partition_flags      = SPM_PART_FLAG_APP_ROT | SPM_PART_FLAG_IPC,
        .partition_priority   = TFM_PRIORITY_LOW,
        .partition_init       = tfm_nspm_thread_entry,
#else
        .partition_flags      = 0,
#endif
    },

#ifndef TFM_PSA_API
    {
        .partition_id         = TFM_SP_CORE_ID,
        .partition_flags      = SPM_PART_FLAG_APP_ROT | SPM_PART_FLAG_PSA_ROT,
    },
#endif

    {
        .partition_id         = TFM_SP_STORAGE_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = tfm_sst_req_mngr_init,
    },

    {
        .partition_id         = TFM_SP_ITS_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = tfm_its_req_mngr_init,
    },

#ifdef TFM_PARTITION_AUDIT_LOG
    {
        .partition_id         = TFM_SP_AUDIT_LOG_ID,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = audit_core_init,
    },
#endif /* TFM_PARTITION_AUDIT_LOG */

    {
        .partition_id         = TFM_SP_CRYPTO_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = tfm_crypto_init,
    },

#ifdef TFM_PARTITION_PLATFORM
    {
        .partition_id         = TFM_SP_PLATFORM_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = platform_sp_init,
    },
#endif /* TFM_PARTITION_PLATFORM */

    {
        .partition_id         = TFM_SP_INITIAL_ATTESTATION_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = attest_partition_init,
    },

#ifdef TFM_PARTITION_TEST_CORE
    {
        .partition_id         = TFM_SP_CORE_TEST_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = core_test_init,
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
    {
        .partition_id         = TFM_SP_CORE_TEST_2_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = core_test_2_init,
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
        .partition_id         = TFM_SP_SECURE_TEST_PARTITION_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = tfm_secure_client_service_init,
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .partition_id         = TFM_SP_IPC_SERVICE_TEST_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(HIGH),
        .partition_init       = ipc_service_test_main,
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .partition_id         = TFM_SP_IPC_CLIENT_TEST_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = ipc_client_test_main,
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    {
        .partition_id         = TFM_IRQ_TEST_1_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = tfm_irq_test_1_init,
    },
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
    {
        .partition_id         = TFM_APP_ROT_ID,
        .partition_flags      = SPM_PART_FLAG_IPC
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_priority   = TFM_PRIORITY(NORMAL),
        .partition_init       = app_rot_init,
    },
#endif /* TFM_PARTITION_APP_ROT */

};

/**************************************************************************/
/** The platform data of the partition list */
/**************************************************************************/
const struct tfm_spm_partition_platform_data_t *platform_data_list[] =
{
    NULL,

#ifndef TFM_PSA_API
    NULL,
#endif

    NULL,

    NULL,

#ifdef TFM_PARTITION_AUDIT_LOG
    /* FIXME: Only adding the first mmio region */
#ifdef AUDIT_UART_REDIRECTION
    TFM_PERIPHERAL_UART1,
#else /* AUDIT_UART_REDIRECTION */
    NULL,
#endif /* AUDIT_UART_REDIRECTION */
#endif /* TFM_PARTITION_AUDIT_LOG */

    NULL,

#ifdef TFM_PARTITION_PLATFORM
    NULL,
#endif /* TFM_PARTITION_PLATFORM */

    NULL,

#ifdef TFM_PARTITION_TEST_CORE
    /* FIXME: Only adding the first mmio region */
    NULL,
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
    NULL,
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    /* FIXME: Only adding the first mmio region */
    TFM_PERIPHERAL_STD_UART,
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    NULL,
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    NULL,
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    /* FIXME: Only adding the first mmio region */
    TFM_PERIPHERAL_TIMER0,
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
    NULL,
#endif /* TFM_PARTITION_APP_ROT */

};

/**************************************************************************/
/** The memory data of the partition list */
/**************************************************************************/
#ifdef TFM_PSA_API
const struct tfm_spm_partition_memory_data_t memory_data_list[] =
{
    {
        .stack_bottom         = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Limit),
        .rw_start             = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Base),
    },
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_STORAGE, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_STORAGE, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_STORAGE, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_STORAGE, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_STORAGE, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_STORAGE, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_STORAGE, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_STORAGE, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_STORAGE, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_STORAGE, _STACK$$ZI$$Limit),
    },

    {
        .code_start           = PART_REGION_ADDR(TFM_SP_ITS, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_ITS, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_ITS, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_ITS, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_ITS, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_ITS, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_ITS, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_ITS, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_ITS, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_ITS, _STACK$$ZI$$Limit),
    },

#ifdef TFM_PARTITION_AUDIT_LOG
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_AUDIT_LOG, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_AUDIT_LOG */

    {
        .code_start           = PART_REGION_ADDR(TFM_SP_CRYPTO, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_CRYPTO, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_CRYPTO, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_CRYPTO, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_CRYPTO, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_CRYPTO, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_CRYPTO, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_CRYPTO, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_CRYPTO, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_CRYPTO, _STACK$$ZI$$Limit),
    },

#ifdef TFM_PARTITION_PLATFORM
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_PLATFORM, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_PLATFORM, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_PLATFORM, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_PLATFORM, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_PLATFORM, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_PLATFORM, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_PLATFORM, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_PLATFORM, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_PLATFORM, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_PLATFORM, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_PLATFORM */

    {
        .code_start           = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_INITIAL_ATTESTATION, _STACK$$ZI$$Limit),
    },

#ifdef TFM_PARTITION_TEST_CORE
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_CORE_TEST, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_CORE_TEST, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_CORE_TEST, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_CORE_TEST, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_CORE_TEST_2, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_SECURE_TEST_PARTITION, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_IPC_SERVICE_TEST, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .code_start           = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_SP_IPC_CLIENT_TEST, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    {
        .code_start           = PART_REGION_ADDR(TFM_IRQ_TEST_1, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_IRQ_TEST_1, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_IRQ_TEST_1, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_IRQ_TEST_1, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_IRQ_TEST_1, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_IRQ_TEST_1, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_IRQ_TEST_1, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_IRQ_TEST_1, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_IRQ_TEST_1, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_IRQ_TEST_1, _STACK$$ZI$$Limit),
    },
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
    {
        .code_start           = PART_REGION_ADDR(TFM_APP_ROT, $$Base),
        .code_limit           = PART_REGION_ADDR(TFM_APP_ROT, $$Limit),
        .ro_start             = PART_REGION_ADDR(TFM_APP_ROT, $$RO$$Base),
        .ro_limit             = PART_REGION_ADDR(TFM_APP_ROT, $$RO$$Limit),
        .rw_start             = PART_REGION_ADDR(TFM_APP_ROT, _DATA$$RW$$Base),
        .rw_limit             = PART_REGION_ADDR(TFM_APP_ROT, _DATA$$RW$$Limit),
        .zi_start             = PART_REGION_ADDR(TFM_APP_ROT, _DATA$$ZI$$Base),
        .zi_limit             = PART_REGION_ADDR(TFM_APP_ROT, _DATA$$ZI$$Limit),
        .stack_bottom         = PART_REGION_ADDR(TFM_APP_ROT, _STACK$$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(TFM_APP_ROT, _STACK$$ZI$$Limit),
    },
#endif /* TFM_PARTITION_APP_ROT */

};
#endif /* defined(TFM_PSA_API) */

/**************************************************************************/
/** The partition list for the DB */
/**************************************************************************/
static struct spm_partition_desc_t partition_list [] =
{
#ifdef TFM_PSA_API
    {{{0}}}, /* placeholder for Non-secure internal partition */
#else
    {{0}}, /* placeholder for Non-secure internal partition */
#endif
#ifndef TFM_PSA_API
    {{0}}, /* placeholder for TF-M Core internal partition */
#endif /* !ifndefined(TFM_PSA_API) */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_STORAGE */
    /* -----------------------------------------------------------------------*/
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_ITS */
    /* -----------------------------------------------------------------------*/
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_AUDIT_LOG */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_AUDIT_LOG
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_AUDIT_LOG */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CRYPTO */
    /* -----------------------------------------------------------------------*/
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PLATFORM */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PLATFORM
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_PLATFORM */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_INITIAL_ATTESTATION */
    /* -----------------------------------------------------------------------*/
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_TEST_CORE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST_2 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_TEST_CORE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SECURE_TEST_PARTITION */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_SERVICE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_CLIENT_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_IRQ_TEST_1 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_ENABLE_IRQ_TEST
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_ENABLE_IRQ_TEST */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_APP_ROT */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_APP_ROT
    {
    /* Runtime data */
#ifdef TFM_PSA_API
        .runtime_data             = {{0}},
#else
        .runtime_data             = {0},
#endif
        .static_data              = NULL,
        .platform_data            = NULL,

    },
#endif /* TFM_PARTITION_APP_ROT */

};

struct spm_partition_db_t g_spm_partition_db = {
    .is_init = 0,
    .partition_count = sizeof(partition_list) / sizeof(partition_list[0]),
#ifndef TFM_PSA_API
    .running_partition_idx = 0,
#endif
    .partitions = partition_list,
};

#endif /* __TFM_SPM_DB_INC__ */
