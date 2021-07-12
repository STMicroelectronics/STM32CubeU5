/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* All the APIs defined in this file are used for library model. */

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tfm_utils.h"
#include "tfm_spm_hal.h"
#include "spm_api.h"
#include "spm_db.h"
#include "region_defs.h"
#include "tfm_nspm.h"
#include "tfm_memory_utils.h"
#include "tfm_internal.h"

extern struct spm_partition_db_t g_spm_partition_db;

typedef enum {
    TFM_INIT_FAILURE,
} sp_error_type_t;

/*
 * This function is called when a secure partition causes an error.
 * In case of an error in the error handling, a non-zero value have to be
 * returned.
 */
static void tfm_spm_partition_err_handler(
    const struct spm_partition_desc_t *partition,
    sp_error_type_t err_type,
    int32_t err_code)
{
#ifdef TFM_CORE_DEBUG
    if (err_type == TFM_INIT_FAILURE) {
        printf("Partition init failed for partition id 0x%08X\r\n",
               partition->static_data->partition_id);
    } else {
        printf(
            "Unknown partition error %d (code: %d) for partition id 0x%08X\r\n",
            err_type, err_code, partition->static_data->partition_id);
    }
#else
    (void)err_type;
    (void)err_code;
#endif
    tfm_spm_partition_set_state(partition->static_data->partition_id,
                                SPM_PARTITION_STATE_CLOSED);
}

enum spm_err_t tfm_spm_partition_init(void)
{
    struct spm_partition_desc_t *part;
    struct tfm_sfn_req_s desc;
    int32_t args[4] = {0};
    int32_t fail_cnt = 0;
    uint32_t idx;

    /* Call the init function for each partition */
    for (idx = 0; idx < g_spm_partition_db.partition_count; ++idx) {
        part = &g_spm_partition_db.partitions[idx];
        tfm_spm_hal_configure_default_isolation(part->platform_data);
        if (part->static_data->partition_init == NULL) {
            tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            tfm_spm_partition_set_caller_partition_idx(idx,
                                                       SPM_INVALID_PARTITION_IDX);
        } else {
            int32_t res;

            desc.args = args;
            desc.ns_caller = 0U;
            desc.sfn = (sfn_t)part->static_data->partition_init;
            desc.sp_id = part->static_data->partition_id;
            res = tfm_core_sfn_request(&desc);
            if (res == TFM_SUCCESS) {
                tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            } else {
                tfm_spm_partition_err_handler(part, TFM_INIT_FAILURE, res);
                fail_cnt++;
            }
        }
    }

    tfm_secure_api_init_done();

    if (fail_cnt == 0) {
        return SPM_ERR_OK;
    } else {
        return SPM_ERR_PARTITION_NOT_AVAILABLE;
    }
}

void tfm_spm_partition_push_interrupted_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct interrupted_ctx_stack_frame_t *stack_frame =
        (struct interrupted_ctx_stack_frame_t *)runtime_data->ctx_stack_ptr;

    stack_frame->partition_state = runtime_data->partition_state;
}

void tfm_spm_partition_pop_interrupted_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct interrupted_ctx_stack_frame_t *stack_frame;

    stack_frame = (struct interrupted_ctx_stack_frame_t *)
                      runtime_data->ctx_stack_ptr;
    tfm_spm_partition_set_state(partition_idx, stack_frame->partition_state);
    stack_frame->partition_state = 0;
}

void tfm_spm_partition_push_handler_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct handler_ctx_stack_frame_t *stack_frame =
        (struct handler_ctx_stack_frame_t *)
            runtime_data->ctx_stack_ptr;

    stack_frame->partition_state = runtime_data->partition_state;
    stack_frame->caller_partition_idx = runtime_data->caller_partition_idx;

    runtime_data->ctx_stack_ptr +=
        sizeof(struct handler_ctx_stack_frame_t) / sizeof(uint32_t);
}

void tfm_spm_partition_pop_handler_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct handler_ctx_stack_frame_t *stack_frame;

    runtime_data->ctx_stack_ptr -=
        sizeof(struct handler_ctx_stack_frame_t) / sizeof(uint32_t);

    stack_frame = (struct handler_ctx_stack_frame_t *)
                      runtime_data->ctx_stack_ptr;

    tfm_spm_partition_set_state(partition_idx, stack_frame->partition_state);
    stack_frame->partition_state = 0;
    tfm_spm_partition_set_caller_partition_idx(
        partition_idx, stack_frame->caller_partition_idx);
    stack_frame->caller_partition_idx = 0;
}

void tfm_spm_partition_store_context(uint32_t partition_idx,
        uint32_t stack_ptr, uint32_t lr)
{
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.stack_ptr = stack_ptr;
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.lr = lr;
}

const struct spm_partition_runtime_data_t *
            tfm_spm_partition_get_runtime_data(uint32_t partition_idx)
{
    return &(g_spm_partition_db.partitions[partition_idx].runtime_data);
}

void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.partition_state =
            state;
    if (state == SPM_PARTITION_STATE_RUNNING ||
        state == SPM_PARTITION_STATE_HANDLING_IRQ) {
        g_spm_partition_db.running_partition_idx = partition_idx;
    }
}

void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_partition_idx = caller_partition_idx;
}

void tfm_spm_partition_set_signal_mask(uint32_t partition_idx,
                                       uint32_t signal_mask)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            signal_mask = signal_mask;
}

void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_client_id = caller_client_id;
}

enum spm_err_t tfm_spm_partition_set_iovec(uint32_t partition_idx,
                                           const int32_t *args)
{
    struct spm_partition_runtime_data_t *runtime_data =
            &g_spm_partition_db.partitions[partition_idx].runtime_data;
    size_t i;

    if ((args[1] < 0) || (args[3] < 0)) {
        return SPM_ERR_INVALID_PARAMETER;
    }

    runtime_data->iovec_args.in_len = (size_t)args[1];
    for (i = 0U; i < runtime_data->iovec_args.in_len; ++i) {
        runtime_data->iovec_args.in_vec[i].base =
                                                 ((psa_invec *)args[0])[i].base;
        runtime_data->iovec_args.in_vec[i].len = ((psa_invec *)args[0])[i].len;
    }
    runtime_data->iovec_args.out_len = (size_t)args[3];
    for (i = 0U; i < runtime_data->iovec_args.out_len; ++i) {
        runtime_data->iovec_args.out_vec[i].base =
                                                ((psa_outvec *)args[2])[i].base;
        runtime_data->iovec_args.out_vec[i].len =
                                                 ((psa_outvec *)args[2])[i].len;
    }
    runtime_data->orig_outvec = (psa_outvec *)args[2];

    return SPM_ERR_OK;
}

uint32_t tfm_spm_partition_get_running_partition_idx(void)
{
    return g_spm_partition_db.running_partition_idx;
}

void tfm_spm_partition_cleanup_context(uint32_t partition_idx)
{
    struct spm_partition_desc_t *partition =
            &(g_spm_partition_db.partitions[partition_idx]);
    int32_t i;

    partition->runtime_data.caller_partition_idx = SPM_INVALID_PARTITION_IDX;
    partition->runtime_data.iovec_args.in_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.in_vec[i].base = 0;
        partition->runtime_data.iovec_args.in_vec[i].len = 0;
    }
    partition->runtime_data.iovec_args.out_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.out_vec[i].base = 0;
        partition->runtime_data.iovec_args.out_vec[i].len = 0;
    }
    partition->runtime_data.orig_outvec = 0;
}
