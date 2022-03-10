/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "bitops.h"
#include "spm_psa_client_call.h"
#include "psa/lifecycle.h"
#ifdef TFM_PSA_API
#include "spm_ipc.h"
#else
#include "spm_func.h"
#endif
#include "tfm_core_utils.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_platform.h"
#include "ffm/spm_error_base.h"
#include "tfm_rpc.h"
#include "tfm_spm_hal.h"

/*********************** SPM functions for PSA Client APIs *******************/

uint32_t tfm_spm_get_lifecycle_state(void)
{
    /*
     * FixMe: return PSA_LIFECYCLE_UNKNOWN to the caller directly. It will be
     * implemented in the future.
     */
    return PSA_LIFECYCLE_UNKNOWN;
}

uint32_t tfm_spm_psa_framework_version(void)
{
    return tfm_spm_client_psa_framework_version();
}

uint32_t tfm_spm_psa_version(uint32_t *args, bool ns_caller)
{
    uint32_t sid;

    TFM_CORE_ASSERT(args != NULL);
    sid = (uint32_t)args[0];

    return tfm_spm_client_psa_version(sid, ns_caller);
}

psa_status_t tfm_spm_psa_connect(uint32_t *args, bool ns_caller)
{
    uint32_t sid;
    uint32_t version;

    TFM_CORE_ASSERT(args != NULL);
    sid = (uint32_t)args[0];
    version = (uint32_t)args[1];

    return tfm_spm_client_psa_connect(sid, version, ns_caller);
}

psa_status_t tfm_spm_psa_call(uint32_t *args, bool ns_caller, uint32_t lr)
{
    psa_handle_t handle;
    psa_invec *inptr;
    psa_outvec *outptr;
    size_t in_num, out_num;
    struct partition_t *partition = NULL;
    uint32_t privileged;
    int32_t type;
    struct tfm_control_parameter_t ctrl_param;

    TFM_CORE_ASSERT(args != NULL);
    handle = (psa_handle_t)args[0];

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->p_static->flags);

    /*
     * Read parameters from the arguments. It is a PROGRAMMER ERROR if the
     * memory reference for buffer is invalid or not readable.
     */
    if (tfm_memory_check((const void *)args[1],
        sizeof(struct tfm_control_parameter_t), ns_caller,
        TFM_MEMORY_ACCESS_RW, privileged) != SPM_SUCCESS) {
        TFM_PROGRAMMER_ERROR(ns_caller, PSA_ERROR_PROGRAMMER_ERROR);
    }

    spm_memcpy(&ctrl_param, (const void *)args[1], sizeof(ctrl_param));

    type = ctrl_param.type;
    in_num = ctrl_param.in_len;
    out_num = ctrl_param.out_len;
    inptr = (psa_invec *)args[2];
    outptr = (psa_outvec *)args[3];

    /* The request type must be zero or positive. */
    if (type < 0) {
        TFM_PROGRAMMER_ERROR(ns_caller, PSA_ERROR_PROGRAMMER_ERROR);
    }

    return tfm_spm_client_psa_call(handle, type, inptr, in_num, outptr, out_num,
                                   ns_caller, privileged);
}

void tfm_spm_psa_close(uint32_t *args, bool ns_caller)
{
    psa_handle_t handle;

    TFM_CORE_ASSERT(args != NULL);
    handle = args[0];

    tfm_spm_client_psa_close(handle, ns_caller);
}

/********************* SPM functions for PSA Service APIs ********************/

psa_signal_t tfm_spm_psa_wait(uint32_t *args)
{
    psa_signal_t signal_mask;
    uint32_t timeout;
    struct partition_t *partition = NULL;

    TFM_CORE_ASSERT(args != NULL);
    signal_mask = (psa_signal_t)args[0];
    timeout = args[1];

    /*
     * Timeout[30:0] are reserved for future use.
     * SPM must ignore the value of RES.
     */
    timeout &= PSA_TIMEOUT_MASK;

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }

    /*
     * It is a PROGRAMMER ERROR if the signal_mask does not include any assigned
     * signals.
     */
    if ((partition->signals_allowed & signal_mask) == 0) {
        tfm_core_panic();
    }

    /*
     * tfm_event_wait() blocks the caller thread if no signals are available.
     * In this case, the return value of this function is temporary set into
     * runtime context. After new signal(s) are available, the return value
     * is updated with the available signal(s) and blocked thread gets to run.
     */
    if (timeout == PSA_BLOCK &&
        (partition->signals_asserted & signal_mask) == 0) {
        partition->signals_waiting = signal_mask;
        tfm_event_wait(&partition->event);
    }

    return partition->signals_asserted & signal_mask;
}

psa_status_t tfm_spm_psa_get(uint32_t *args)
{
    psa_signal_t signal;
    psa_msg_t *msg = NULL;
    struct tfm_msg_body_t *tmp_msg = NULL;
    struct partition_t *partition = NULL;
    uint32_t privileged;

    TFM_CORE_ASSERT(args != NULL);
    signal = (psa_signal_t)args[0];
    msg = (psa_msg_t *)args[1];

    /*
     * Only one message could be retrieved every time for psa_get(). It is a
     * fatal error if the input signal has more than a signal bit set.
     */
    if (!IS_ONLY_ONE_BIT_IN_UINT32(signal)) {
        tfm_core_panic();
    }

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->p_static->flags);

    /*
     * Write the message to the service buffer. It is a fatal error if the
     * input msg pointer is not a valid memory reference or not read-write.
     */
    if (tfm_memory_check(msg, sizeof(psa_msg_t), false, TFM_MEMORY_ACCESS_RW,
        privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the caller call psa_get() when no message has
     * been set. The caller must call this function after an RoT Service signal
     * is returned by psa_wait().
     */
    if (partition->signals_asserted == 0) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the RoT Service signal is not currently asserted.
     */
    if ((partition->signals_asserted & signal) == 0) {
        tfm_core_panic();
    }

    /*
     * Get message by signal from partition. It is a fatal error if getting
     * failed, which means the input signal is not correspond to an RoT service.
     */
    tmp_msg = tfm_spm_get_msg_by_signal(partition, signal);
    if (!tmp_msg) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    (TFM_GET_CONTAINER_PTR(tmp_msg,
                           struct tfm_conn_handle_t,
                           internal_msg))->status = TFM_HANDLE_STATUS_ACTIVE;

    spm_memcpy(msg, &tmp_msg->msg, sizeof(psa_msg_t));

    return PSA_SUCCESS;
}

void tfm_spm_psa_set_rhandle(uint32_t *args)
{
    psa_handle_t msg_handle;
    void *rhandle = NULL;
    struct tfm_msg_body_t *msg = NULL;
    struct tfm_conn_handle_t *conn_handle;

    TFM_CORE_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    rhandle = (void *)args[1];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_core_panic();
    }

    /* It is a PROGRAMMER ERROR if a stateless service sets rhandle. */
    if (!msg->service->service_db->connection_based) {
        tfm_core_panic();
    }

    msg->msg.rhandle = rhandle;
    conn_handle = tfm_spm_to_handle_instance(msg_handle);

    /* Store reverse handle for following client calls. */
    tfm_spm_set_rhandle(msg->service, conn_handle, rhandle);
}

size_t tfm_spm_psa_read(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t invec_idx;
    void *buffer = NULL;
    size_t num_bytes;
    size_t bytes;
    struct tfm_msg_body_t *msg = NULL;
    uint32_t privileged;
    struct partition_t *partition = NULL;

    TFM_CORE_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    invec_idx = args[1];
    buffer = (void *)args[2];
    num_bytes = (size_t)args[3];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_core_panic();
    }

    partition = msg->service->partition;
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->p_static->flags);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* There was no remaining data in this input vector */
    if (msg->msg.in_size[invec_idx] == 0) {
        return 0;
    }

    /*
     * Copy the client data to the service buffer. It is a fatal error
     * if the memory reference for buffer is invalid or not read-write.
     */
    if (tfm_memory_check(buffer, num_bytes, false,
        TFM_MEMORY_ACCESS_RW, privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    bytes = num_bytes > msg->msg.in_size[invec_idx] ?
                        msg->msg.in_size[invec_idx] : num_bytes;

    spm_memcpy(buffer, msg->invec[invec_idx].base, bytes);

    /* There maybe some remaining data */
    msg->invec[invec_idx].base = (char *)msg->invec[invec_idx].base + bytes;
    msg->msg.in_size[invec_idx] -= bytes;

    return bytes;
}

size_t tfm_spm_psa_skip(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t invec_idx;
    size_t num_bytes;
    struct tfm_msg_body_t *msg = NULL;

    TFM_CORE_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    invec_idx = args[1];
    num_bytes = (size_t)args[2];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* There was no remaining data in this input vector */
    if (msg->msg.in_size[invec_idx] == 0) {
        return 0;
    }

    /*
     * If num_bytes is greater than the remaining size of the input vector then
     * the remaining size of the input vector is used.
     */
    if (num_bytes > msg->msg.in_size[invec_idx]) {
        num_bytes = msg->msg.in_size[invec_idx];
    }

    /* There maybe some remaining data */
    msg->invec[invec_idx].base = (char *)msg->invec[invec_idx].base +
                                 num_bytes;
    msg->msg.in_size[invec_idx] -= num_bytes;

    return num_bytes;
}

void tfm_spm_psa_write(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t outvec_idx;
    void *buffer = NULL;
    size_t num_bytes;
    struct tfm_msg_body_t *msg = NULL;
    uint32_t privileged;
    struct partition_t *partition = NULL;

    TFM_CORE_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    outvec_idx = args[1];
    buffer = (void *)args[2];
    num_bytes = (size_t)args[3];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_core_panic();
    }

    partition = msg->service->partition;
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->p_static->flags);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if outvec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (outvec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the call attempts to write data past the end of
     * the client output vector
     */
    if (num_bytes > msg->msg.out_size[outvec_idx] -
        msg->outvec[outvec_idx].len) {
        tfm_core_panic();
    }

    /*
     * Copy the service buffer to client outvecs. It is a fatal error
     * if the memory reference for buffer is invalid or not readable.
     */
    if (tfm_memory_check(buffer, num_bytes, false,
        TFM_MEMORY_ACCESS_RO, privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    spm_memcpy((char *)msg->outvec[outvec_idx].base +
               msg->outvec[outvec_idx].len, buffer, num_bytes);

    /* Update the write number */
    msg->outvec[outvec_idx].len += num_bytes;
}

void tfm_spm_psa_reply(uint32_t *args)
{
    psa_handle_t msg_handle;
    psa_status_t status;
    struct tfm_spm_service_t *service = NULL;
    struct tfm_msg_body_t *msg = NULL;
    int32_t ret = PSA_SUCCESS;
    struct tfm_conn_handle_t *conn_handle;

    TFM_CORE_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    status = (psa_status_t)args[1];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_core_panic();
    }

    /*
     * RoT Service information is needed in this function, stored it in message
     * body structure. Only two parameters are passed in this function: handle
     * and status, so it is useful and simply to do like this.
     */
    service = msg->service;
    if (!service) {
        tfm_core_panic();
    }

    /*
     * Three type of message are passed in this function: CONNECTION, REQUEST,
     * DISCONNECTION. It needs to process differently for each type.
     */
    conn_handle = tfm_spm_to_handle_instance(msg_handle);
    switch (msg->msg.type) {
    case PSA_IPC_CONNECT:
        /*
         * Reply to PSA_IPC_CONNECT message. Connect handle is returned if the
         * input status is PSA_SUCCESS. Others return values are based on the
         * input status.
         */
        if (status == PSA_SUCCESS) {
            ret = msg_handle;
        } else if (status == PSA_ERROR_CONNECTION_REFUSED) {
            /* Refuse the client connection, indicating a permanent error. */
            tfm_spm_free_conn_handle(service, conn_handle);
            ret = PSA_ERROR_CONNECTION_REFUSED;
        } else if (status == PSA_ERROR_CONNECTION_BUSY) {
            /* Fail the client connection, indicating a transient error. */
            ret = PSA_ERROR_CONNECTION_BUSY;
        } else {
            tfm_core_panic();
        }
        break;
    case PSA_IPC_DISCONNECT:
        /* Service handle is not used anymore */
        tfm_spm_free_conn_handle(service, conn_handle);

        /*
         * If the message type is PSA_IPC_DISCONNECT, then the status code is
         * ignored
         */
        break;
    default:
        if (msg->msg.type >= PSA_IPC_CALL) {
            /* Reply to a request message. Return values are based on status */
            ret = status;
            /*
             * The total number of bytes written to a single parameter must be
             * reported to the client by updating the len member of the
             * psa_outvec structure for the parameter before returning from
             * psa_call().
             */
            update_caller_outvec_len(msg);
            if (!service->service_db->connection_based) {
                tfm_spm_free_conn_handle(service, conn_handle);
            }
        } else {
            tfm_core_panic();
        }
    }

    if (ret == PSA_ERROR_PROGRAMMER_ERROR) {
        /*
         * If the source of the programmer error is a Secure Partition, the SPM
         * must panic the Secure Partition in response to a PROGRAMMER ERROR.
         */
        if (TFM_CLIENT_ID_IS_NS(msg->msg.client_id)) {
            conn_handle->status = TFM_HANDLE_STATUS_CONNECT_ERROR;
        } else {
            tfm_core_panic();
        }
    } else {
        conn_handle->status = TFM_HANDLE_STATUS_IDLE;
    }

    if (is_tfm_rpc_msg(msg)) {
        tfm_rpc_client_call_reply(msg, ret);
    } else {
        tfm_event_wake(&msg->ack_evnt, ret);
    }
}

void tfm_spm_psa_notify(uint32_t *args)
{
    int32_t partition_id;

    TFM_CORE_ASSERT(args != NULL);
    partition_id = (int32_t)args[0];

    notify_with_signal(partition_id, PSA_DOORBELL);
}

void tfm_spm_psa_clear(void)
{
    struct partition_t *partition = NULL;

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the Secure Partition's doorbell signal is not
     * currently asserted.
     */
    if ((partition->signals_asserted & PSA_DOORBELL) == 0) {
        tfm_core_panic();
    }
    partition->signals_asserted &= ~PSA_DOORBELL;
}

void tfm_spm_psa_eoi(uint32_t *args)
{
    psa_signal_t irq_signal;
    int32_t irq_line = 0;
    struct partition_t *partition = NULL;

    TFM_CORE_ASSERT(args != NULL);
    irq_signal = (psa_signal_t)args[0];

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }

    irq_line = get_irq_line_for_signal(partition->p_static->pid, irq_signal);
    /* It is a fatal error if passed signal is not an interrupt signal. */
    if (irq_line < 0) {
        tfm_core_panic();
    }

    /* It is a fatal error if passed signal is not currently asserted */
    if ((partition->signals_asserted & irq_signal) == 0) {
        tfm_core_panic();
    }

    partition->signals_asserted &= ~irq_signal;

    tfm_spm_hal_clear_pending_irq((IRQn_Type)irq_line);
    tfm_spm_hal_enable_irq((IRQn_Type)irq_line);
}

void tfm_spm_psa_panic(void)
{
    /*
     * PSA FF recommends that the SPM causes the system to restart when a secure
     * partition panics.
     */
    tfm_hal_system_reset();
}

void tfm_spm_irq_enable(uint32_t *args)
{
    struct partition_t *partition;
    psa_signal_t irq_signal;
    int32_t irq_line;

    irq_signal = (psa_signal_t)args[0];

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }

    irq_line = get_irq_line_for_signal(partition->p_static->pid, irq_signal);
    if (irq_line < 0) {
        tfm_core_panic();
    }

    tfm_spm_hal_enable_irq((IRQn_Type)irq_line);
}

psa_irq_status_t tfm_spm_irq_disable(uint32_t *args)
{
    struct partition_t *partition;
    psa_signal_t irq_signal;
    int32_t irq_line;

    irq_signal = (psa_signal_t)args[0];

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_core_panic();
    }

    irq_line = get_irq_line_for_signal(partition->p_static->pid, irq_signal);
    if (irq_line < 0) {
        tfm_core_panic();
    }

    tfm_spm_hal_disable_irq((IRQn_Type)irq_line);

    return 1;
}
