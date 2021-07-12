/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* All the APIs defined in this file are used for IPC model. */

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include "region_defs.h"
#include "psa/client.h"
#include "psa/service.h"
#include "tfm_utils.h"
#include "tfm_spm_hal.h"
#include "spm_api.h"
#include "spm_db.h"
#include "tfm_core_mem_check.h"
#include "tfm_internal_defines.h"
#include "tfm_wait.h"
#include "tfm_message_queue.h"
#include "tfm_list.h"
#include "tfm_pools.h"
#include "tfm_thread.h"
#include "region_defs.h"
#include "tfm_nspm.h"
#include "tfm_memory_utils.h"
#include "tfm_core_utils.h"
#ifdef FLOW_CONTROL
#include "target_flowcontrol.h"
#endif

#include "secure_fw/services/tfm_service_list.inc"

extern void Error_Handler(void);

/* Extern service variable */
extern struct tfm_spm_service_t service[];
extern const struct tfm_spm_service_db_t service_db[];

/* Extern SPM variable */
extern struct spm_partition_db_t g_spm_partition_db;

/* Extern secure lock variable */
extern int32_t tfm_secure_lock;

/* Pools */
TFM_POOL_DECLARE(conn_handle_pool, sizeof(struct tfm_conn_handle_t),
                 TFM_CONN_HANDLE_MAX_NUM);

/********************** SPM functions for handler mode ***********************/

/* Service handle management functions */
psa_handle_t tfm_spm_create_conn_handle(struct tfm_spm_service_t *service)
{
    struct tfm_conn_handle_t *p_handle;

    TFM_ASSERT(service);

    /* Get buffer for handle list structure from handle pool */
    p_handle = (struct tfm_conn_handle_t *)tfm_pool_alloc(conn_handle_pool);
    if (!p_handle) {
        return PSA_NULL_HANDLE;
    }

    p_handle->service = service;

    /* Add handle node to list for next psa functions */
    tfm_list_add_tail(&service->handle_list, &p_handle->list);

    return (psa_handle_t)p_handle;
}

static struct tfm_conn_handle_t *
    tfm_spm_find_conn_handle_node(struct tfm_spm_service_t *service,
                                  psa_handle_t conn_handle)
{
    TFM_ASSERT(service);

    return (struct tfm_conn_handle_t *)conn_handle;
}

int32_t tfm_spm_free_conn_handle(struct tfm_spm_service_t *service,
                                 psa_handle_t conn_handle)
{
    struct tfm_conn_handle_t *p_handle;

    TFM_ASSERT(service);

    /* There are many handles for each RoT Service */
    p_handle = tfm_spm_find_conn_handle_node(service, conn_handle);
    if (!p_handle) {
        tfm_panic();
    }

    /* Clear magic as the handler is not used anymore */
    p_handle->internal_msg.magic = 0;

    /* Remove node from handle list */
    tfm_list_del_node(&p_handle->list);

    /* Back handle buffer to pool */
    tfm_pool_free(p_handle);
    return IPC_SUCCESS;
}

int32_t tfm_spm_set_rhandle(struct tfm_spm_service_t *service,
                            psa_handle_t conn_handle,
                            void *rhandle)
{
    struct tfm_conn_handle_t *p_handle;

    TFM_ASSERT(service);
    /* Set reverse handle value only be allowed for a connected handle */
    TFM_ASSERT(conn_handle != PSA_NULL_HANDLE);

    /* There are many handles for each RoT Service */
    p_handle = tfm_spm_find_conn_handle_node(service, conn_handle);
    if (!p_handle) {
        tfm_panic();
    }

    p_handle->rhandle = rhandle;
    return IPC_SUCCESS;
}

void *tfm_spm_get_rhandle(struct tfm_spm_service_t *service,
                          psa_handle_t conn_handle)
{
    struct tfm_conn_handle_t *p_handle;

    TFM_ASSERT(service);
    /* Get reverse handle value only be allowed for a connected handle */
    TFM_ASSERT(conn_handle != PSA_NULL_HANDLE);

    /* There are many handles for each RoT Service */
    p_handle = tfm_spm_find_conn_handle_node(service, conn_handle);
    if (!p_handle) {
        tfm_panic();
    }

    return p_handle->rhandle;
}

/* Partition management functions */
struct tfm_spm_service_t *
    tfm_spm_get_service_by_signal(struct spm_partition_desc_t *partition,
                                  psa_signal_t signal)
{
    struct tfm_list_node_t *node, *head;
    struct tfm_spm_service_t *service;

    TFM_ASSERT(partition);

    if (tfm_list_is_empty(&partition->runtime_data.service_list)) {
        tfm_panic();
    }

    head = &partition->runtime_data.service_list;
    TFM_LIST_FOR_EACH(node, head) {
        service = TFM_GET_CONTAINER_PTR(node, struct tfm_spm_service_t, list);
        if (service->service_db->signal == signal) {
            return service;
        }
    }
    return NULL;
}

struct tfm_spm_service_t *tfm_spm_get_service_by_sid(uint32_t sid)
{
    uint32_t i;
    struct tfm_list_node_t *node, *head;
    struct tfm_spm_service_t *service;
    struct spm_partition_desc_t *partition;

    for (i = 0; i < g_spm_partition_db.partition_count; i++) {
        partition = &g_spm_partition_db.partitions[i];
        /* Skip partition without IPC flag */
        if ((tfm_spm_partition_get_flags(i) & SPM_PART_FLAG_IPC) == 0) {
            continue;
        }

        if (tfm_list_is_empty(&partition->runtime_data.service_list)) {
            continue;
        }

        head = &partition->runtime_data.service_list;
        TFM_LIST_FOR_EACH(node, head) {
            service = TFM_GET_CONTAINER_PTR(node, struct tfm_spm_service_t,
                                            list);
            if (service->service_db->sid == sid) {
                return service;
            }
        }
    }
    return NULL;
}

struct tfm_spm_service_t *
    tfm_spm_get_service_by_handle(psa_handle_t conn_handle)
{
    return ((struct tfm_conn_handle_t *)conn_handle)->service;
}

struct spm_partition_desc_t *tfm_spm_get_partition_by_id(int32_t partition_id)
{
    uint32_t idx = get_partition_idx(partition_id);

    if (idx != SPM_INVALID_PARTITION_IDX) {
        return &(g_spm_partition_db.partitions[idx]);
    }
    return NULL;
}

struct spm_partition_desc_t *tfm_spm_get_running_partition(void)
{
    uint32_t spid;

    spid = tfm_spm_partition_get_running_partition_id();

    return tfm_spm_get_partition_by_id(spid);
}

int32_t tfm_spm_check_client_version(struct tfm_spm_service_t *service,
                                     uint32_t minor_version)
{
    TFM_ASSERT(service);

    switch (service->service_db->minor_policy) {
    case TFM_VERSION_POLICY_RELAXED:
        if (minor_version > service->service_db->minor_version) {
            return IPC_ERROR_VERSION;
        }
        break;
    case TFM_VERSION_POLICY_STRICT:
        if (minor_version != service->service_db->minor_version) {
            return IPC_ERROR_VERSION;
        }
        break;
    default:
        return IPC_ERROR_VERSION;
    }
    return IPC_SUCCESS;
}

/* Message functions */
struct tfm_msg_body_t *tfm_spm_get_msg_from_handle(psa_handle_t msg_handle)
{
    /*
     * The message handler passed by the caller is considered invalid in the
     * following cases:
     *   1. Not a valid message handle. (The address of a message is not the
     *      address of a possible handle from the pool
     *   2. Handle not belongs to the caller partition (The handle is either
     *      unused, or owned by anither partition)
     * Check the conditions above
     */
    struct tfm_conn_handle_t *connection_handle_address;
    struct tfm_msg_body_t *msg;
    uint32_t partition_id;

    msg = (struct tfm_msg_body_t *)msg_handle;

    connection_handle_address =
        TFM_GET_CONTAINER_PTR(msg, struct tfm_conn_handle_t, internal_msg);

    if (is_valid_chunk_data_in_pool(
        conn_handle_pool, (uint8_t *)connection_handle_address) != 1) {
        return NULL;
    }

    /*
     * Check that the magic number is correct. This proves that the message
     * structure contains an active message.
     */
    if (msg->magic != TFM_MSG_MAGIC) {
        return NULL;
    }

    /* Check that the running partition owns the message */
    partition_id = tfm_spm_partition_get_running_partition_id();
    if (partition_id != msg->service->partition->static_data->partition_id) {
        return NULL;
    }

    /*
     * FixMe: For condition 1 it should be checked whether the message belongs
     * to the service. Skipping this check isn't a security risk as even if the
     * message belongs to another service, the handle belongs to the calling
     * partition.
     */

    return msg;
}

struct tfm_msg_body_t *
    tfm_spm_get_msg_buffer_from_conn_handle(psa_handle_t conn_handle)
{
    TFM_ASSERT(conn_handle != PSA_NULL_HANDLE);

    return &(((struct tfm_conn_handle_t *)conn_handle)->internal_msg);
}

void tfm_spm_fill_msg(struct tfm_msg_body_t *msg,
                      struct tfm_spm_service_t *service,
                      psa_handle_t handle,
                      int32_t type, int32_t ns_caller,
                      psa_invec *invec, size_t in_len,
                      psa_outvec *outvec, size_t out_len,
                      psa_outvec *caller_outvec)
{
    uint32_t i;

    TFM_ASSERT(msg);
    TFM_ASSERT(service);
    TFM_ASSERT(!(invec == NULL && in_len != 0));
    TFM_ASSERT(!(outvec == NULL && out_len != 0));
    TFM_ASSERT(in_len <= PSA_MAX_IOVEC);
    TFM_ASSERT(out_len <= PSA_MAX_IOVEC);
    TFM_ASSERT(in_len + out_len <= PSA_MAX_IOVEC);

    /* Clear message buffer before using it */
    tfm_core_util_memset(msg, 0, sizeof(struct tfm_msg_body_t));

    tfm_event_init(&msg->ack_evnt);
    msg->magic = TFM_MSG_MAGIC;
    msg->service = service;
    msg->handle = handle;
    msg->caller_outvec = caller_outvec;
    /* Get current partition id */
    if (ns_caller) {
        msg->msg.client_id = tfm_nspm_get_current_client_id();
    } else {
        msg->msg.client_id = tfm_spm_partition_get_running_partition_id();
    }

    /* Copy contents */
    msg->msg.type = type;

    for (i = 0; i < in_len; i++) {
        msg->msg.in_size[i] = invec[i].len;
        msg->invec[i].base = invec[i].base;
    }

    for (i = 0; i < out_len; i++) {
        msg->msg.out_size[i] = outvec[i].len;
        msg->outvec[i].base = outvec[i].base;
        /* Out len is used to record the writed number, set 0 here again */
        msg->outvec[i].len = 0;
    }

    /* Use message address as handle */
    msg->msg.handle = (psa_handle_t)msg;

    /* For connected handle, set rhandle to every message */
    if (handle != PSA_NULL_HANDLE) {
        msg->msg.rhandle = tfm_spm_get_rhandle(service, handle);
    }
}

int32_t tfm_spm_send_event(struct tfm_spm_service_t *service,
                           struct tfm_msg_body_t *msg)
{
    struct spm_partition_runtime_data_t *p_runtime_data =
                                            &service->partition->runtime_data;

    TFM_ASSERT(service);
    TFM_ASSERT(msg);

    /* Enqueue message to service message queue */
    if (tfm_msg_enqueue(&service->msg_queue, msg) != IPC_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

    /* Messages put. Update signals */
    p_runtime_data->signals |= service->service_db->signal;

    tfm_event_wake(&p_runtime_data->signal_evnt, (p_runtime_data->signals &
                                                  p_runtime_data->signal_mask));

    tfm_event_wait(&msg->ack_evnt);

    return IPC_SUCCESS;
}

uint32_t tfm_spm_partition_get_stack_bottom(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data->stack_bottom;
}

uint32_t tfm_spm_partition_get_stack_top(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].memory_data->stack_top;
}

uint32_t tfm_spm_partition_get_running_partition_id(void)
{
    struct tfm_thrd_ctx *pth = tfm_thrd_curr_thread();
    struct spm_partition_desc_t *partition;
    struct spm_partition_runtime_data_t *r_data;

    r_data = TFM_GET_CONTAINER_PTR(pth, struct spm_partition_runtime_data_t,
                                   sp_thrd);
    partition = TFM_GET_CONTAINER_PTR(r_data, struct spm_partition_desc_t,
                                      runtime_data);
    return partition->static_data->partition_id;
}

static struct tfm_thrd_ctx *
    tfm_spm_partition_get_thread_info(uint32_t partition_idx)
{
    return &g_spm_partition_db.partitions[partition_idx].runtime_data.sp_thrd;
}

static tfm_thrd_func_t
    tfm_spm_partition_get_init_func(uint32_t partition_idx)
{
    return (tfm_thrd_func_t)(g_spm_partition_db.partitions[partition_idx].
                             static_data->partition_init);
}

static uint32_t tfm_spm_partition_get_priority(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].static_data->
                    partition_priority;
}

int32_t tfm_memory_check(const void *buffer, size_t len, int32_t ns_caller,
                         enum tfm_memory_access_e access,
                         uint32_t privileged)
{
    enum tfm_status_e err;

    /* If len is zero, this indicates an empty buffer and base is ignored */
    if (len == 0) {
        return IPC_SUCCESS;
    }

    if (!buffer) {
        return IPC_ERROR_BAD_PARAMETERS;
    }

    if ((uintptr_t)buffer > (UINTPTR_MAX - len)) {
        return IPC_ERROR_MEMORY_CHECK;
    }

    if (access == TFM_MEMORY_ACCESS_RW) {
        err = tfm_core_has_write_access_to_region(buffer, len, ns_caller,
                                                  privileged);
    } else {
        err = tfm_core_has_read_access_to_region(buffer, len, ns_caller,
                                                 privileged);
    }
    if (err == TFM_SUCCESS) {
        return IPC_SUCCESS;
    }

    return IPC_ERROR_MEMORY_CHECK;
}

uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_flags)
{
    if (partition_flags & SPM_PART_FLAG_PSA_ROT) {
        return TFM_PARTITION_PRIVILEGED_MODE;
    } else {
        return TFM_PARTITION_UNPRIVILEGED_MODE;
    }
}

/********************** SPM functions for thread mode ************************/

void tfm_spm_init(void)
{
    uint32_t i, num;
    struct spm_partition_desc_t *partition;
    struct tfm_thrd_ctx *pth, this_thrd;

    tfm_pool_init(conn_handle_pool,
                  POOL_BUFFER_SIZE(conn_handle_pool),
                  sizeof(struct tfm_conn_handle_t),
                  TFM_CONN_HANDLE_MAX_NUM);

    /* Init partition first for it will be used when init service */
    for (i = 0; i < g_spm_partition_db.partition_count; i++) {
        partition = &g_spm_partition_db.partitions[i];
        tfm_spm_hal_configure_default_isolation(partition->platform_data);
        if ((tfm_spm_partition_get_flags(i) & SPM_PART_FLAG_IPC) == 0) {
            continue;
        }

        tfm_event_init(&partition->runtime_data.signal_evnt);
        tfm_list_init(&partition->runtime_data.service_list);

        pth = tfm_spm_partition_get_thread_info(i);
        if (!pth) {
            tfm_panic();
        }

        tfm_thrd_init(pth,
                      tfm_spm_partition_get_init_func(i),
                      NULL,
                      (uintptr_t)tfm_spm_partition_get_stack_top(i),
                      (uintptr_t)tfm_spm_partition_get_stack_bottom(i));

        pth->prior = tfm_spm_partition_get_priority(i);

        /* Kick off */
        if (tfm_thrd_start(pth) != THRD_SUCCESS) {
            tfm_panic();
        }
    }

    /* Init Service */
    num = sizeof(service) / sizeof(struct tfm_spm_service_t);
    for (i = 0; i < num; i++) {
        service[i].service_db = &service_db[i];
        partition =
            tfm_spm_get_partition_by_id(service[i].service_db->partition_id);
        if (!partition) {
            tfm_panic();
        }
        service[i].partition = partition;
        tfm_list_init(&service[i].handle_list);
        tfm_list_add_tail(&partition->runtime_data.service_list,
                          &service[i].list);
    }

#ifdef FLOW_CONTROL
    /* check flow control to resist to basic HW attacks */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_1);
#endif

    /*
     * All threads initialized, start the scheduler.
     *
     * NOTE:
     * Here is the booting privileged thread mode, and will never
     * return to this place after scheduler is started. The start
     * function has to save current runtime context to act as a
     * 'current thread' to avoid repeating NULL 'current thread'
     * checking while context switching. This saved context is worthy
     * of being saved somewhere if there are potential usage purpose.
     * Let's save this context in a local variable 'this_thrd' at
     * current since there is no usage for it.
     * Also set tfm_nspm_thread_entry as pfn for this thread to
     * use in detecting NS/S thread scheduling changes.
     */
    this_thrd.pfn = (tfm_thrd_func_t)tfm_nspm_thread_entry;
    tfm_thrd_start_scheduler(&this_thrd);
}

void tfm_pendsv_do_schedule(struct tfm_state_context_ext *ctxb)
{
#if TFM_LVL == 2
    struct spm_partition_desc_t *p_next_partition;
    struct spm_partition_runtime_data_t *r_data;
    uint32_t is_privileged;
#endif
    struct tfm_thrd_ctx *pth_next = tfm_thrd_next_thread();
    struct tfm_thrd_ctx *pth_curr = tfm_thrd_curr_thread();

    if (pth_next != NULL && pth_curr != pth_next) {
#if TFM_LVL == 2
        r_data = TFM_GET_CONTAINER_PTR(pth_next,
                                       struct spm_partition_runtime_data_t,
                                       sp_thrd);
        p_next_partition = TFM_GET_CONTAINER_PTR(r_data,
                                                 struct spm_partition_desc_t,
                                                 runtime_data);

        if (p_next_partition->static_data->partition_flags &
            SPM_PART_FLAG_PSA_ROT) {
            is_privileged = TFM_PARTITION_PRIVILEGED_MODE;
        } else {
            is_privileged = TFM_PARTITION_UNPRIVILEGED_MODE;
        }

        tfm_spm_partition_change_privilege(is_privileged);
#endif
        /* Increase the secure lock, if we enter secure from non-secure */
        if ((void *)pth_curr->pfn == (void *)tfm_nspm_thread_entry) {
            ++tfm_secure_lock;
        }
        /* Decrease the secure lock, if we return from secure to non-secure */
        if ((void *)pth_next->pfn == (void *)tfm_nspm_thread_entry) {
            --tfm_secure_lock;
        }

        tfm_thrd_context_switch(ctxb, pth_curr, pth_next);
    }
}
