/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include "psa/client.h"
#include "psa/service.h"
#include "tfm_svc.h"
#include "tfm_svcalls.h"
#include "tfm_thread.h"
#include "tfm_wait.h"
#include "tfm_utils.h"
#include "tfm_internal_defines.h"
#include "tfm_message_queue.h"
#include "tfm_spm_hal.h"
#include "tfm_irq_list.h"
#include "tfm_api.h"
#include "tfm_secure_api.h"
#include "tfm_memory_utils.h"
#include "spm_api.h"
#include "tfm_peripherals_def.h"
#include "spm_db.h"
#include "tfm_core_utils.h"
#ifdef __ICCARM__
/* remove IAR warning relative to pointless comparison */
#pragma diag_suppress=Pa084
#endif
void tfm_irq_handler(uint32_t partition_id, psa_signal_t signal,
                     IRQn_Type irq_line);

#include "tfm_secure_irq_handlers_ipc.inc"

/************************* SVC handler for PSA Client APIs *******************/

uint32_t tfm_svcall_psa_framework_version(void)
{
    return PSA_FRAMEWORK_VERSION;
}

uint32_t tfm_svcall_psa_version(uint32_t *args, int32_t ns_caller)
{
    uint32_t sid;
    struct tfm_spm_service_t *service;

    TFM_ASSERT(args != NULL);
    sid = (uint32_t)args[0];
    /*
     * It should return PSA_VERSION_NONE if the RoT Service is not
     * implemented.
     */
    service = tfm_spm_get_service_by_sid(sid);
    if (!service) {
        return PSA_VERSION_NONE;
    }

    /*
     * It should return PSA_VERSION_NONE if the caller is not authorized
     * to access the RoT Service.
     */
    if (ns_caller && !service->service_db->non_secure_client) {
        return PSA_VERSION_NONE;
    }

    return service->service_db->minor_version;
}

psa_handle_t tfm_svcall_psa_connect(uint32_t *args, int32_t ns_caller)
{
    uint32_t sid;
    uint32_t minor_version;
    struct tfm_spm_service_t *service;
    struct tfm_msg_body_t *msg;
    psa_handle_t connect_handle;

    TFM_ASSERT(args != NULL);
    sid = (uint32_t)args[0];
    minor_version = (uint32_t)args[1];

    /* It is a fatal error if the RoT Service does not exist on the platform */
    service = tfm_spm_get_service_by_sid(sid);
    if (!service) {
        tfm_panic();
    }

    /*
     * Create connection handle here since it is possible to return the error
     * code to client when creation fails.
     */
    connect_handle = tfm_spm_create_conn_handle(service);
    if (connect_handle == PSA_NULL_HANDLE) {
        return PSA_ERROR_CONNECTION_BUSY;
    }

    /*
     * It is a fatal error if the caller is not authorized to access the RoT
     * Service.
     */
    if (ns_caller && !service->service_db->non_secure_client) {
        tfm_panic();
    }

    /*
     * It is a fatal error if the version of the RoT Service requested is not
     * supported on the platform.
     */
    if (tfm_spm_check_client_version(service, minor_version) != IPC_SUCCESS) {
        tfm_panic();
    }

    msg = tfm_spm_get_msg_buffer_from_conn_handle(connect_handle);
    if (!msg) {
        /* Have no enough resource to create message */
        return PSA_ERROR_CONNECTION_BUSY;
    }

    /* No input or output needed for connect message */
    tfm_spm_fill_msg(msg, service, connect_handle, PSA_IPC_CONNECT,
                     ns_caller, NULL, 0, NULL, 0, NULL);

    /*
     * Send message and wake up the SP who is waiting on message queue,
     * and scheduler triggered
     */
    tfm_spm_send_event(service, msg);

    return PSA_SUCCESS;
}

psa_status_t tfm_svcall_psa_call(uint32_t *args, int32_t ns_caller, uint32_t lr)
{
    psa_handle_t handle;
    psa_invec *inptr, invecs[PSA_MAX_IOVEC];
    psa_outvec *outptr, outvecs[PSA_MAX_IOVEC];
    size_t in_num, out_num;
    struct tfm_spm_service_t *service;
    struct tfm_msg_body_t *msg;
    int i;
    struct spm_partition_desc_t *partition = NULL;
    uint32_t privileged;
    int32_t type;

    TFM_ASSERT(args != NULL);
    handle = (psa_handle_t)args[0];
    type = (int32_t)args[1];

    if (type < 0) {
        tfm_panic();
    }
    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->static_data->partition_flags);

    if (!ns_caller) {
        inptr = (psa_invec *)args[2];
        in_num = (size_t)args[3];
        /*
         * 5th and 6th parameter is pushed at stack top before SVC, then PE
         * hardware stacks the execution context. The size of the context
         * depends on various settings:
         * - if FP is not used, 5th and 6th parameters are at 8th and 9th
         *   position counting from SP;
         * - if FP is used and FPCCR_S.TS is 0, 5th and 6th parameters are at
         *   26th and 27th position counting from SP;
         * - if FP is used and FPCCR_S.TS is 1, 5th and 6th parameters are at
         *   42th and 43th position counting from SP.
         */
         if (!is_stack_alloc_fp_space(lr)) {
            outptr = (psa_outvec *)args[8];
            out_num = (size_t)args[9];
#if defined (__FPU_USED) && (__FPU_USED == 1U)
        } else if (FPU->FPCCR & FPU_FPCCR_TS_Msk) {
            outptr = (psa_outvec *)args[42];
            out_num = (size_t)args[43];
#endif
        } else {
            outptr = (psa_outvec *)args[26];
            out_num = (size_t)args[27];
        }
    } else {
        /*
         * FixMe: From non-secure caller, vec and len are composed into a new
         * struct parameter. Need to extract them.
         */
        /*
         * Read parameters from the arguments. It is a fatal error if the
         * memory reference for buffer is invalid or not readable.
         */
        if (tfm_memory_check((const void *)args[2], sizeof(uint32_t),
            ns_caller, TFM_MEMORY_ACCESS_RO, privileged) != IPC_SUCCESS) {
            tfm_panic();
        }
        if (tfm_memory_check((const void *)args[3], sizeof(uint32_t),
            ns_caller, TFM_MEMORY_ACCESS_RO, privileged) != IPC_SUCCESS) {
            tfm_panic();
        }

        inptr = (psa_invec *)((psa_invec *)args[2])->base;
        in_num = ((psa_invec *)args[2])->len;
        outptr = (psa_outvec *)((psa_invec *)args[3])->base;
        out_num = ((psa_invec *)args[3])->len;
    }

    /* It is a fatal error if in_len + out_len > PSA_MAX_IOVEC. */
    if ((in_num > PSA_MAX_IOVEC) ||
        (out_num > PSA_MAX_IOVEC) ||
        (in_num + out_num > PSA_MAX_IOVEC)) {
        tfm_panic();
    }

    /* It is a fatal error if an invalid handle was passed. */
    service = tfm_spm_get_service_by_handle(handle);
    if (!service) {
        /* FixMe: Need to implement one mechanism to resolve this failure. */
        tfm_panic();
    }

    /*
     * Read client invecs from the wrap input vector. It is a fatal error
     * if the memory reference for the wrap input vector is invalid or not
     * readable.
     */
    if (tfm_memory_check(inptr, in_num * sizeof(psa_invec), ns_caller,
        TFM_MEMORY_ACCESS_RO, privileged) != IPC_SUCCESS) {
        tfm_panic();
    }
    /*
     * Read client outvecs from the wrap output vector and will update the
     * actual length later. It is a fatal error if the memory reference for
     * the wrap output vector is invalid or not read-write.
     */
    if (tfm_memory_check(outptr, out_num * sizeof(psa_outvec), ns_caller,
        TFM_MEMORY_ACCESS_RW, privileged) != IPC_SUCCESS) {
        tfm_panic();
    }

    tfm_core_util_memset(invecs, 0, sizeof(invecs));
    tfm_core_util_memset(outvecs, 0, sizeof(outvecs));

    /* Copy the address out to avoid TOCTOU attacks. */
    tfm_core_util_memcpy(invecs, inptr, in_num * sizeof(psa_invec));
    tfm_core_util_memcpy(outvecs, outptr, out_num * sizeof(psa_outvec));

    /*
     * For client input vector, it is a fatal error if the provided payload
     * memory reference was invalid or not readable.
     */
    for (i = 0; i < in_num; i++) {
        if (tfm_memory_check(invecs[i].base, invecs[i].len, ns_caller,
            TFM_MEMORY_ACCESS_RO, privileged) != IPC_SUCCESS) {
            tfm_panic();
        }
    }
    /*
     * For client output vector, it is a fatal error if the provided payload
     * memory reference was invalid or not read-write.
     */
    for (i = 0; i < out_num; i++) {
        if (tfm_memory_check(outvecs[i].base, outvecs[i].len,
            ns_caller, TFM_MEMORY_ACCESS_RW, privileged) != IPC_SUCCESS) {
            tfm_panic();
        }
    }

    /*
     * FixMe: Need to check if the message is unrecognized by the RoT
     * Service or incorrectly formatted.
     */
    msg = tfm_spm_get_msg_buffer_from_conn_handle(handle);
    if (!msg) {
        /* FixMe: Need to implement one mechanism to resolve this failure. */
        tfm_panic();
    }

    tfm_spm_fill_msg(msg, service, handle, type, ns_caller, invecs,
                     in_num, outvecs, out_num, outptr);

    /*
     * Send message and wake up the SP who is waiting on message queue,
     * and scheduler triggered
     */
    if (tfm_spm_send_event(service, msg) != IPC_SUCCESS) {
        /* FixMe: Need to refine failure process here. */
        tfm_panic();
    }
    return PSA_SUCCESS;
}

void tfm_svcall_psa_close(uint32_t *args, int32_t ns_caller)
{
    psa_handle_t handle;
    struct tfm_spm_service_t *service;
    struct tfm_msg_body_t *msg;

    TFM_ASSERT(args != NULL);
    handle = args[0];
    /* It will have no effect if called with the NULL handle */
    if (handle == PSA_NULL_HANDLE) {
        return;
    }

    /*
     * It is a fatal error if an invalid handle was provided that is not the
     * null handle..
     */
    service = tfm_spm_get_service_by_handle(handle);
    if (!service) {
        /* FixMe: Need to implement one mechanism to resolve this failure. */
        tfm_panic();
    }

    msg = tfm_spm_get_msg_buffer_from_conn_handle(handle);
    if (!msg) {
        /* FixMe: Need to implement one mechanism to resolve this failure. */
        tfm_panic();
    }

    /* No input or output needed for close message */
    tfm_spm_fill_msg(msg, service, handle, PSA_IPC_DISCONNECT, ns_caller,
                     NULL, 0, NULL, 0, NULL);

    /*
     * Send message and wake up the SP who is waiting on message queue,
     * and scheduler triggered
     */
    tfm_spm_send_event(service, msg);
}

/*********************** SVC handler for PSA Service APIs ********************/

/**
 * \brief SVC handler for \ref psa_wait.
 *
 * \param[in] args              Include all input arguments:
 *                              signal_mask, timeout.
 *
 * \retval >0                   At least one signal is asserted.
 * \retval 0                    No signals are asserted. This is only seen when
 *                              a polling timeout is used.
 */
static psa_signal_t tfm_svcall_psa_wait(uint32_t *args)
{
    psa_signal_t signal_mask;
    uint32_t timeout;
    struct spm_partition_desc_t *partition = NULL;

    TFM_ASSERT(args != NULL);
    signal_mask = (psa_signal_t)args[0];
    timeout = args[1];

    /*
     * Timeout[30:0] are reserved for future use.
     * SPM must ignore the value of RES.
     */
    timeout &= PSA_TIMEOUT_MASK;

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }

    /*
     * Expected signals are included in signal wait mask, ignored signals
     * should not be set and affect caller thread status. Save this mask for
     * further checking while signals are ready to be set.
     */
    partition->runtime_data.signal_mask = signal_mask;

    /*
     * tfm_event_wait() blocks the caller thread if no signals are available.
     * In this case, the return value of this function is temporary set into
     * runtime context. After new signal(s) are available, the return value
     * is updated with the available signal(s) and blocked thread gets to run.
     */
    if (timeout == PSA_BLOCK &&
        (partition->runtime_data.signals & signal_mask) == 0) {
        tfm_event_wait(&partition->runtime_data.signal_evnt);
    }

    return partition->runtime_data.signals & signal_mask;
}

/**
 * \brief SVC handler for \ref psa_get.
 *
 * \param[in] args              Include all input arguments: signal, msg.
 *
 * \retval PSA_SUCCESS          Success, *msg will contain the delivered
 *                              message.
 * \retval PSA_ERROR_DOES_NOT_EXIST Message could not be delivered.
 * \retval "Does not return"    The call is invalid because one or more of the
 *                              following are true:
 * \arg                           signal has more than a single bit set.
 * \arg                           signal does not correspond to a RoT Service.
 * \arg                           The RoT Service signal is not currently
 *                                asserted.
 * \arg                           The msg pointer provided is not a valid memory
 *                                reference.
 */
static psa_status_t tfm_svcall_psa_get(uint32_t *args)
{
    psa_signal_t signal;
    psa_msg_t *msg = NULL;
    struct tfm_spm_service_t *service = NULL;
    struct tfm_msg_body_t *tmp_msg = NULL;
    struct spm_partition_desc_t *partition = NULL;
    uint32_t privileged;

    TFM_ASSERT(args != NULL);
    signal = (psa_signal_t)args[0];
    msg = (psa_msg_t *)args[1];

    /*
     * Only one message could be retrieved every time for psa_get(). It is a
     * fatal error if the input signal has more than a signal bit set.
     */
    if (tfm_bitcount(signal) != 1) {
        tfm_panic();
    }

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->static_data->partition_flags);

    /*
     * Write the message to the service buffer. It is a fatal error if the
     * input msg pointer is not a valid memory reference or not read-write.
     */
    if (tfm_memory_check(msg, sizeof(psa_msg_t), false, TFM_MEMORY_ACCESS_RW,
        privileged) != IPC_SUCCESS) {
        tfm_panic();
    }

    /*
     * It is a fatal error if the caller call psa_get() when no message has
     * been set. The caller must call this function after a RoT Service signal
     * is returned by psa_wait().
     */
    if (partition->runtime_data.signals == 0) {
        tfm_panic();
    }

    /*
     * It is a fatal error if the RoT Service signal is not currently asserted.
     */
    if ((partition->runtime_data.signals & signal) == 0) {
        tfm_panic();
    }

    /*
     * Get Rot service by signal from partition. It is a fatal error if geting
     * failed which mean the input signal is not correspond to a RoT service.
     */
    service = tfm_spm_get_service_by_signal(partition, signal);
    if (!service) {
        tfm_panic();
    }

    tmp_msg = tfm_msg_dequeue(&service->msg_queue);
    if (!tmp_msg) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    tfm_core_util_memcpy(msg, &tmp_msg->msg, sizeof(psa_msg_t));

    /*
     * There may be mutiple messages for this RoT Service signal, do not clear
     * its mask until no remaining message.
     */
    if (tfm_msg_queue_is_empty(&service->msg_queue)) {
        partition->runtime_data.signals &= ~signal;
    }

    return PSA_SUCCESS;
}

/**
 * \brief SVC handler for \ref psa_set_rhandle.
 *
 * \param[in] args              Include all input arguments:
 *                              msg_handle, rhandle.
 *
 * \retval void                 Success, rhandle will be provided with all
 *                              subsequent messages delivered on this
 *                              connection.
 * \retval "Does not return"    msg_handle is invalid.
 */
static void tfm_svcall_psa_set_rhandle(uint32_t *args)
{
    psa_handle_t msg_handle;
    void *rhandle = NULL;
    struct tfm_msg_body_t *msg = NULL;

    TFM_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    rhandle = (void *)args[1];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_panic();
    }

    msg->msg.rhandle = rhandle;

    /* Store reverse handle for following client calls. */
    tfm_spm_set_rhandle(msg->service, msg->handle, rhandle);
}

/**
 * \brief SVC handler for \ref psa_read.
 *
 * \param[in] args              Include all input arguments:
 *                              msg_handle, invec_idx, buffer, num_bytes.
 *
 * \retval >0                   Number of bytes copied.
 * \retval 0                    There was no remaining data in this input
 *                              vector.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           msg_handle is invalid.
 * \arg                           msg_handle does not refer to a request
 *                                message.
 * \arg                           invec_idx is equal to or greater than
 *                                \ref PSA_MAX_IOVEC.
 * \arg                           the memory reference for buffer is invalid or
 *                                not writable.
 */
static size_t tfm_svcall_psa_read(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t invec_idx;
    void *buffer = NULL;
    size_t num_bytes;
    size_t bytes;
    struct tfm_msg_body_t *msg = NULL;
    uint32_t privileged;
    struct spm_partition_desc_t *partition = NULL;

    TFM_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    invec_idx = args[1];
    buffer = (void *)args[2];
    num_bytes = (size_t)args[3];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_panic();
    }

    partition = msg->service->partition;
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->static_data->partition_flags);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_panic();
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
        TFM_MEMORY_ACCESS_RW, privileged) != IPC_SUCCESS) {
        tfm_panic();
    }

    bytes = num_bytes > msg->msg.in_size[invec_idx] ?
                        msg->msg.in_size[invec_idx] : num_bytes;

    tfm_core_util_memcpy(buffer, msg->invec[invec_idx].base, bytes);

    /* There maybe some remaining data */
    msg->invec[invec_idx].base =(char *)msg->invec[invec_idx].base+bytes;
    msg->msg.in_size[invec_idx] -= bytes;

    return bytes;
}

/**
 * \brief SVC handler for \ref psa_skip.
 *
 * \param[in] args              Include all input arguments:
 *                              msg_handle, invec_idx, num_bytes.
 *
 * \retval >0                   Number of bytes skipped.
 * \retval 0                    There was no remaining data in this input
 *                              vector.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           msg_handle is invalid.
 * \arg                           msg_handle does not refer to a request
 *                                message.
 * \arg                           invec_idx is equal to or greater than
 *                                \ref PSA_MAX_IOVEC.
 */
static size_t tfm_svcall_psa_skip(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t invec_idx;
    size_t num_bytes;
    struct tfm_msg_body_t *msg = NULL;

    TFM_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    invec_idx = args[1];
    num_bytes = (size_t)args[2];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_panic();
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
    msg->invec[invec_idx].base = (char *)msg->invec[invec_idx].base+ num_bytes;
    msg->msg.in_size[invec_idx] -= num_bytes;

    return num_bytes;
}

/**
 * \brief SVC handler for \ref psa_write.
 *
 * \param[in] args              Include all input arguments:
 *                              msg_handle, outvec_idx, buffer, num_bytes.
 *
 * \retval void                 Success
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           msg_handle is invalid.
 * \arg                           msg_handle does not refer to a request
 *                                message.
 * \arg                           outvec_idx is equal to or greater than
 *                                \ref PSA_MAX_IOVEC.
 * \arg                           The memory reference for buffer is invalid.
 * \arg                           The call attempts to write data past the end
 *                                of the client output vector.
 */
static void tfm_svcall_psa_write(uint32_t *args)
{
    psa_handle_t msg_handle;
    uint32_t outvec_idx;
    void *buffer = NULL;
    size_t num_bytes;
    struct tfm_msg_body_t *msg = NULL;
    uint32_t privileged;
    struct spm_partition_desc_t *partition = NULL;

    TFM_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    outvec_idx = args[1];
    buffer = (void *)args[2];
    num_bytes = (size_t)args[3];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_panic();
    }

    partition = msg->service->partition;
    privileged = tfm_spm_partition_get_privileged_mode(
        partition->static_data->partition_flags);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (msg->msg.type < PSA_IPC_CALL) {
        tfm_panic();
    }

    /*
     * It is a fatal error if outvec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (outvec_idx >= PSA_MAX_IOVEC) {
        tfm_panic();
    }

    /*
     * It is a fatal error if the call attempts to write data past the end of
     * the client output vector
     */
    if (num_bytes > msg->msg.out_size[outvec_idx] -
        msg->outvec[outvec_idx].len) {
        tfm_panic();
    }

    /*
     * Copy the service buffer to client outvecs. It is a fatal error
     * if the memory reference for buffer is invalid or not readable.
     */
    if (tfm_memory_check(buffer, num_bytes, false,
        TFM_MEMORY_ACCESS_RO, privileged) != IPC_SUCCESS) {
        tfm_panic();
    }

    tfm_core_util_memcpy((char *)msg->outvec[outvec_idx].base +
                         msg->outvec[outvec_idx].len, buffer, num_bytes);

    /* Update the write number */
    msg->outvec[outvec_idx].len += num_bytes;
}

static void update_caller_outvec_len(struct tfm_msg_body_t *msg)
{
    int32_t i = 0;

    /*
     * FixeMe: abstract these part into dedicated functions to avoid
     * accessing thread context in psa layer
     */
    TFM_ASSERT(msg->ack_evnt.owner->status == THRD_STAT_BLOCK);

    while (msg->msg.out_size[i] != 0) {
        TFM_ASSERT(msg->caller_outvec[i].base == msg->outvec[i].base);
        msg->caller_outvec[i].len = msg->outvec[i].len;
        i++;
    }
}
/**
 * \brief SVC handler for \ref psa_reply.
 *
 * \param[in] args              Include all input arguments:
 *                              msg_handle, status.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                         msg_handle is invalid.
 * \arg                         An invalid status code is specified for the
 *                              type of message.
 */
static void tfm_svcall_psa_reply(uint32_t *args)
{
    psa_handle_t msg_handle;
    psa_status_t status;
    struct tfm_spm_service_t *service = NULL;
    struct tfm_msg_body_t *msg = NULL;
    int32_t ret = PSA_SUCCESS;

    TFM_ASSERT(args != NULL);
    msg_handle = (psa_handle_t)args[0];
    status = (psa_status_t)args[1];

    /* It is a fatal error if message handle is invalid */
    msg = tfm_spm_get_msg_from_handle(msg_handle);
    if (!msg) {
        tfm_panic();
    }

    /*
     * RoT Service information is needed in this function, stored it in message
     * body structure. Only two parameters are passed in this function: handle
     * and status, so it is useful and simply to do like this.
     */
    service = msg->service;
    if (!service) {
        tfm_panic();
    }

    /*
     * Three type of message are passed in this function: CONNECT, CALL,
     * DISCONNECT. It needs to process differently for each type.
     */
    switch (msg->msg.type) {
    case PSA_IPC_CONNECT:
        /*
         * Reply to PSA_IPC_CONNECT message. Connect handle is returned if the
         * input status is PSA_SUCCESS. Others return values are based on the
         * input status.
         */
        if (status == PSA_SUCCESS) {
            ret = msg->handle;
        } else if (status == PSA_ERROR_CONNECTION_REFUSED) {
            ret = PSA_ERROR_CONNECTION_REFUSED;
        } else if (status == PSA_ERROR_CONNECTION_BUSY) {
            ret = PSA_ERROR_CONNECTION_BUSY;
        } else {
            tfm_panic();
        }
        break;
    case PSA_IPC_DISCONNECT:
        /* Service handle is not used anymore */
        tfm_spm_free_conn_handle(service, msg->handle);

        /*
         * If the message type is PSA_IPC_DISCONNECT, then the status code is
         * ignored
         */
        break;
    default:
        if (msg->msg.type >= PSA_IPC_CALL) {
            /* Reply to a request message. Return values are based on status */
            if (status == PSA_SUCCESS) {
                ret = PSA_SUCCESS;
            } else if ((status >= (INT32_MIN + 1)) &&
                       (status <= (INT32_MIN + 127))) {
                tfm_panic();
            } else if ((status >= (INT32_MIN + 128)) && (status <= -1)) {
                ret = status;
            } else if ((status >= 1) && (status <= INT32_MAX)) {
                ret = status;
            } else {
                tfm_panic();
            }

            /*
             * The total number of bytes written to a single parameter must be
             * reported to the client by updating the len member of the
             * psa_outvec structure for the parameter before returning from
             * psa_call().
             */
            update_caller_outvec_len(msg);
        } else {
            tfm_panic();
        }
    }

    tfm_event_wake(&msg->ack_evnt, ret);
}

/**
 * \param[in] partition_id      The ID of the partition to be notified.
 * \param[in] signal            The signal that the partition is to be notified
 *                              with.
 *
 * \retval void                 Success.
 * \retval "Does not return"    If partition_id is invalid.
 */
static void notify_with_signal(int32_t partition_id, psa_signal_t signal)
{
    struct spm_partition_desc_t *partition = NULL;

    /*
     * The value of partition_id must be greater than zero as the target of
     * notification must be a Secure Partition, providing a Non-secure
     * Partition ID is a fatal error.
     */
    if (!TFM_CLIENT_ID_IS_S(partition_id)) {
        tfm_panic();
    }

    /*
     * It is a fatal error if partition_id does not correspond to a Secure
     * Partition.
     */
    partition = tfm_spm_get_partition_by_id(partition_id);
    if (!partition) {
        tfm_panic();
    }

    partition->runtime_data.signals |= signal;

    /*
     * The target partition may be blocked with waiting for signals after
     * called psa_wait(). Set the return value with the available signals
     * before wake it up with tfm_event_signal().
     */
    tfm_event_wake(&partition->runtime_data.signal_evnt,
                   partition->runtime_data.signals &
                   partition->runtime_data.signal_mask);
}

/**
 * \brief SVC handler for \ref psa_notify.
 *
 * \param[in] args              Include all input arguments: partition_id.
 *
 * \retval void                 Success.
 * \retval "Does not return"    partition_id does not correspond to a Secure
 *                              Partition.
 */
static void tfm_svcall_psa_notify(uint32_t *args)
{
#if 0
    int32_t partition_id;

    TFM_ASSERT(args != NULL);
    partition_id = (int32_t)args[0];
    return notify_with_signal(partition_id, PSA_DOORBELL);
#endif
}

/**
 * \brief assert signal for a given IRQ line.
 *
 * \param[in] partition_id      The ID of the partition which handles this IRQ
 * \param[in] signal            The signal associated with this IRQ
 * \param[in] irq_line          The number of the IRQ line
 *
 * \retval void                 Success.
 * \retval "Does not return"    Partition ID is invalid
 */
void tfm_irq_handler(uint32_t partition_id, psa_signal_t signal,
                     IRQn_Type irq_line)
{
    tfm_spm_hal_disable_irq(irq_line);
    notify_with_signal(partition_id, signal);
}

/**
 * \brief SVC handler for \ref psa_clear.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The Secure Partition's doorbell signal is not
 *                              currently asserted.
 */
static void tfm_svcall_psa_clear(uint32_t *args)
{
    struct spm_partition_desc_t *partition = NULL;

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }

    /*
     * It is a fatal error if the Secure Partition's doorbell signal is not
     * currently asserted.
     */
    if ((partition->runtime_data.signals & PSA_DOORBELL) == 0) {
        tfm_panic();
    }
    partition->runtime_data.signals &= ~PSA_DOORBELL;
}

/**
 * \brief Return the IRQ line number associated with a signal
 *
 * \param[in]      partition_id    The ID of the partition in which we look for
 *                                 the signal.
 * \param[in]      signal          The signal we do the query for.
 * \param[out]     irq_line        The irq line associated with signal
 *
 * \retval IPC_SUCCESS          Execution successful, irq_line contains a valid
 *                              value.
 * \retval IPC_ERROR_GENERIC    There was an error finding the IRQ line for the
 *                              signal. irq_line is unchanged.
 */
static int32_t get_irq_line_for_signal(int32_t partition_id,
                                       psa_signal_t signal,
                                       IRQn_Type *irq_line)
{
    size_t i;

    for (i = 0; i < tfm_core_irq_signals_count; ++i) {
        if (tfm_core_irq_signals[i].partition_id == partition_id &&
            tfm_core_irq_signals[i].signal_value == signal) {
            *irq_line = tfm_core_irq_signals[i].irq_line;
            return IPC_SUCCESS;
        }
    }
    return IPC_ERROR_GENERIC;
}

/*
 * FIXME: tfm_svcall_psa_eoi, tfm_core_enable_irq_handler and
 * tfm_core_disable_irq_handler function has an implementation in
 * tfm_secure_api.c for the library model.
 * The two implementations should be merged as part of restructuring common code
 * among library and IPC model.
 */
/**
 * \brief SVC handler for \ref psa_eoi.
 *
 * \param[in] args              Include all input arguments: irq_signal.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           irq_signal is not an interrupt signal.
 * \arg                           irq_signal indicates more than one signal.
 * \arg                           irq_signal is not currently asserted.
 */
static void tfm_svcall_psa_eoi(uint32_t *args)
{
    psa_signal_t irq_signal;
    IRQn_Type irq_line = (IRQn_Type) 0;
    int32_t ret;
    struct spm_partition_desc_t *partition = NULL;

    TFM_ASSERT(args != NULL);
    irq_signal = (psa_signal_t)args[0];

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }

    ret = get_irq_line_for_signal(partition->static_data->partition_id,
                                  irq_signal, &irq_line);
    /* It is a fatal error if passed signal is not an interrupt signal. */
    if (ret != IPC_SUCCESS) {
        tfm_panic();
    }

    /* It is a fatal error if passed signal indicates more than one signals. */
    if (!tfm_is_one_bit_set(irq_signal)) {
        tfm_panic();
    }

    /* It is a fatal error if passed signal is not currently asserted */
    if ((partition->runtime_data.signals & irq_signal) == 0) {
        tfm_panic();
    }

    partition->runtime_data.signals &= ~irq_signal;

    tfm_spm_hal_clear_pending_irq(irq_line);
    tfm_spm_hal_enable_irq(irq_line);
}

void tfm_svcall_enable_irq(uint32_t *args)
{
    struct tfm_state_context_t *svc_ctx = (struct tfm_state_context_t *)args;
    psa_signal_t irq_signal = svc_ctx->r0;
    IRQn_Type irq_line = (IRQn_Type) 0;
    int32_t ret;
    struct spm_partition_desc_t *partition = NULL;

    /* It is a fatal error if passed signal indicates more than one signals. */
    if (!tfm_is_one_bit_set(irq_signal)) {
        tfm_panic();
    }

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }

    ret = get_irq_line_for_signal(partition->static_data->partition_id,
                                  irq_signal, &irq_line);
    /* It is a fatal error if passed signal is not an interrupt signal. */
    if (ret != IPC_SUCCESS) {
        tfm_panic();
    }

    tfm_spm_hal_enable_irq(irq_line);
}

void tfm_svcall_disable_irq(uint32_t *args)
{
    struct tfm_state_context_t *svc_ctx = (struct tfm_state_context_t *)args;
    psa_signal_t irq_signal = svc_ctx->r0;
    IRQn_Type irq_line = (IRQn_Type) 0;
    int32_t ret;
    struct spm_partition_desc_t *partition = NULL;

    /* It is a fatal error if passed signal indicates more than one signals. */
    if (!tfm_is_one_bit_set(irq_signal)) {
        tfm_panic();
    }

    partition = tfm_spm_get_running_partition();
    if (!partition) {
        tfm_panic();
    }

    ret = get_irq_line_for_signal(partition->static_data->partition_id,
                                  irq_signal, &irq_line);
    /* It is a fatal error if passed signal is not an interrupt signal. */
    if (ret != IPC_SUCCESS) {
        tfm_panic();
    }

    tfm_spm_hal_disable_irq(irq_line);
}

int32_t SVC_Handler_IPC(tfm_svc_number_t svc_num, uint32_t *ctx, uint32_t lr)
{
    switch (svc_num) {
    case TFM_SVC_EXIT_THRD:
        tfm_svcall_thrd_exit();
        break;
    case TFM_SVC_PSA_FRAMEWORK_VERSION:
        return tfm_svcall_psa_framework_version();
    case TFM_SVC_PSA_VERSION:
        return tfm_svcall_psa_version(ctx, 0);
    case TFM_SVC_PSA_CONNECT:
        return tfm_svcall_psa_connect(ctx, 0);
    case TFM_SVC_PSA_CALL:
        return tfm_svcall_psa_call(ctx, 0, lr);
    case TFM_SVC_PSA_CLOSE:
        tfm_svcall_psa_close(ctx, 0);
        break;
    case TFM_SVC_PSA_WAIT:
        return tfm_svcall_psa_wait(ctx);
    case TFM_SVC_PSA_GET:
        return tfm_svcall_psa_get(ctx);
    case TFM_SVC_PSA_SET_RHANDLE:
        tfm_svcall_psa_set_rhandle(ctx);
        break;
    case TFM_SVC_PSA_READ:
        return tfm_svcall_psa_read(ctx);
    case TFM_SVC_PSA_SKIP:
        return tfm_svcall_psa_skip(ctx);
    case TFM_SVC_PSA_WRITE:
        tfm_svcall_psa_write(ctx);
        break;
    case TFM_SVC_PSA_REPLY:
        tfm_svcall_psa_reply(ctx);
        break;
    case TFM_SVC_PSA_NOTIFY:
        tfm_svcall_psa_notify(ctx);
        break;
    case TFM_SVC_PSA_CLEAR:
        tfm_svcall_psa_clear(ctx);
        break;
    case TFM_SVC_PSA_EOI:
        tfm_svcall_psa_eoi(ctx);
        break;
    case TFM_SVC_ENABLE_IRQ:
        tfm_svcall_enable_irq(ctx);
        break;
    case TFM_SVC_DISABLE_IRQ:
        tfm_svcall_disable_irq(ctx);
        break;

    default:
        LOG_MSG("Unknown SVC number requested!");
        return PSA_ERROR_GENERIC_ERROR;
    }
    return PSA_SUCCESS;
}
