/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_API_H__
#define __SPM_API_H__

/* This file contains the apis exported by the SPM to tfm core */
#include "tfm_api.h"
#include "spm_partition_defs.h"
#include "tfm_secure_api.h"
#include <stdbool.h>
#ifdef TFM_PSA_API
#include "tfm_list.h"
#include "tfm_wait.h"
#include "tfm_message_queue.h"
#include "tfm_secure_api.h"
#include "tfm_thread.h"
#endif

#define SPM_INVALID_PARTITION_IDX     (~0U)

/* Privileged definitions for partition thread mode */
#define TFM_PARTITION_PRIVILEGED_MODE   1
#define TFM_PARTITION_UNPRIVILEGED_MODE 0

enum spm_err_t {
    SPM_ERR_OK = 0,
    SPM_ERR_PARTITION_DB_NOT_INIT,
    SPM_ERR_PARTITION_ALREADY_ACTIVE,
    SPM_ERR_PARTITION_NOT_AVAILABLE,
    SPM_ERR_INVALID_PARAMETER,
    SPM_ERR_INVALID_CONFIG,
};

#define SPM_PARTITION_STATE_UNINIT       0
#define SPM_PARTITION_STATE_IDLE         1
#define SPM_PARTITION_STATE_RUNNING      2
#define SPM_PARTITION_STATE_HANDLING_IRQ 3
#define SPM_PARTITION_STATE_SUSPENDED    4
#define SPM_PARTITION_STATE_BLOCKED      5
#define SPM_PARTITION_STATE_CLOSED       6

#define SPM_PART_FLAG_APP_ROT 0x01
#define SPM_PART_FLAG_PSA_ROT 0x02
#define SPM_PART_FLAG_IPC     0x04

#ifndef TFM_PSA_API
/**
 * \brief Holds the iovec parameters that are passed to a service
 *
 * \note The size of the structure is (and have to be) multiple of 8 bytes
 */
struct iovec_args_t {
    psa_invec in_vec[PSA_MAX_IOVEC];   /*!< Array of psa_invec objects */
    size_t in_len;                     /*!< Number psa_invec objects in in_vec
                                        */
    psa_outvec out_vec[PSA_MAX_IOVEC]; /*!< Array of psa_outvec objects */
    size_t out_len;                    /*!< Number psa_outvec objects in out_vec
                                        */
};

/* The size of this struct must be multiple of 4 bytes as it is stacked to an
 * uint32_t[] array
 */
struct interrupted_ctx_stack_frame_t {
    uint32_t partition_state;
};

/* The size of this struct must be multiple of 4 bytes as it is stacked to an
 * uint32_t[] array
 */
struct handler_ctx_stack_frame_t {
    uint32_t partition_state;
    uint32_t caller_partition_idx;
};
#endif /* !define(TFM_PSA_API) */

/**
 * \brief Runtime context information of a partition
 */
struct spm_partition_runtime_data_t {
#ifdef TFM_PSA_API
    struct tfm_event_t signal_evnt;     /* Event signal                      */
    uint32_t signals;                   /* Service signals had been triggered*/
    struct tfm_list_node_t service_list;/* Service list                      */
    struct tfm_thrd_ctx sp_thrd;        /* Thread context                    */
#else /* TFM_PSA_API */
    uint32_t partition_state;
    uint32_t caller_partition_idx;
    int32_t caller_client_id;
    uint32_t stack_ptr;
    uint32_t lr;
    struct iovec_args_t iovec_args;
    psa_outvec *orig_outvec;
    uint32_t *ctx_stack_ptr;
#endif /* TFM_PSA_API */
    uint32_t signal_mask;               /*
                                         * Service signal mask passed by
                                         * psa_wait()
                                         */
};

#ifdef TFM_PSA_API

#define TFM_VERSION_POLICY_RELAXED      0
#define TFM_VERSION_POLICY_STRICT       1

#define TFM_CONN_HANDLE_MAX_NUM         16

/* RoT connection handle list */
struct tfm_conn_handle_t {
    void *rhandle;                      /* Reverse handle value              */
    struct tfm_msg_body_t internal_msg; /* Internal message for message queue */
    struct tfm_spm_service_t *service;  /* RoT service pointer               */
    struct tfm_list_node_t list;        /* list node                         */
};

/* Service database defined by manifest */
struct tfm_spm_service_db_t {
    char *name;                     /* Service name                          */
    uint32_t partition_id;          /* Partition ID which service belong to  */
    psa_signal_t signal;            /* Service signal                        */
    uint32_t sid;                   /* Service identifier                    */
    bool non_secure_client;         /* If can be called by non secure client */
    uint32_t minor_version;         /* Minor version                         */
    uint32_t minor_policy;          /* Minor version policy                  */
};

/* RoT Service data */
struct tfm_spm_service_t {
    const struct tfm_spm_service_db_t *service_db;/* Service database pointer */
    struct spm_partition_desc_t *partition;  /*
                                              * Point to secure partition
                                              * data
                                              */
    struct tfm_list_node_t handle_list;      /* Service handle list          */
    struct tfm_msg_queue_t msg_queue;        /* Message queue                */
    struct tfm_list_node_t list;             /* For list operation           */
};
#endif /* ifdef(TFM_PSA_API) */

/*********************** common definitions ***********************/

/**
 * \brief Returns the index of the partition with the given partition ID.
 *
 * \param[in] partition_id     Partition id
 *
 * \return the partition idx if partition_id is valid,
 *         \ref SPM_INVALID_PARTITION_IDX othervise
 */
uint32_t get_partition_idx(uint32_t partition_id);

/**
 * \brief Get the id of the partition for its index from the db
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Partition ID for that partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_partition_id(uint32_t partition_idx);

/**
 * \brief Get the flags associated with a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Flags associated with the partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_flags(uint32_t partition_idx);

/**
 * \brief Initialize partition database
 *
 * \return Error code \ref spm_err_t
 */
enum spm_err_t tfm_spm_db_init(void);

/**
 * \brief Change the privilege mode for partition thread mode.
 *
 * \param[in] privileged        Privileged mode,
 *                                \ref TFM_PARTITION_PRIVILEGED_MODE
 *                                and \ref TFM_PARTITION_UNPRIVILEGED_MODE
 *
 * \note Barrier instructions are not called by this function, and if
 *       it is called in thread mode, it might be necessary to call
 *       them after this function returns.
 */
void tfm_spm_partition_change_privilege(uint32_t privileged);

/*********************** library definitions ***********************/

#ifndef TFM_PSA_API
/**
 * \brief Save interrupted partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack overflows.
 */
void tfm_spm_partition_push_interrupted_ctx(uint32_t partition_idx);

/**
 * \brief Restores interrupted partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack underflows.
 */
void tfm_spm_partition_pop_interrupted_ctx(uint32_t partition_idx);

/**
 * \brief Save handler partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack overflows.
 */
void tfm_spm_partition_push_handler_ctx(uint32_t partition_idx);

/**
 * \brief Restores handler partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack underflows.
 */
void tfm_spm_partition_pop_handler_ctx(uint32_t partition_idx);

/**
 * \brief Get the current runtime data of a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return The runtime data of the specified partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
const struct spm_partition_runtime_data_t *
             tfm_spm_partition_get_runtime_data(uint32_t partition_idx);

/**
 * \brief Returns the index of the partition that has running state
 *
 * \return The index of the partition with the running state, if there is any
 *         set. 0 otherwise.
 */
uint32_t tfm_spm_partition_get_running_partition_idx(void);

/**
 * \brief Save stack pointer and link register for partition in database
 *
 * \param[in] partition_idx  Partition index
 * \param[in] stack_ptr      Stack pointer to be stored
 * \param[in] lr             Link register to be stored
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_store_context(uint32_t partition_idx,
        uint32_t stack_ptr, uint32_t lr);

/**
 * \brief Set the current state of a partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] state          The state to be set
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note The state has to have the value set of \ref spm_part_state_t.
 */
void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state);

/**
 * \brief Set the caller partition index for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] caller_partition_idx The index of the caller partition
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx);

/**
* \brief Set the caller client ID for a given partition
*
* \param[in] partition_idx        Partition index
* \param[in] caller_client_id     The ID of the calling client
*
* \note This function doesn't check if any of the partition_idxs are valid.
*/
void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id);


/**
 * \brief Set the iovec parameters for the partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] args           The arguments of the secure function
 *
 * args is expected to be of type int32_t[4] where:
 *   args[0] is in_vec
 *   args[1] is in_len
 *   args[2] is out_vec
 *   args[3] is out_len
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function assumes that the iovecs that are passed in args are
 *       valid, and does no sanity check on them at all.
 */
enum spm_err_t tfm_spm_partition_set_iovec(uint32_t partition_idx,
                                           const int32_t *args);

/**
 * \brief Execute partition init function
 *
 * \return Error code \ref spm_err_t
 */
enum spm_err_t tfm_spm_partition_init(void);

/**
 * \brief Clears the context info from the database for a partition.
 *
 * \param[in] partition_idx     Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_cleanup_context(uint32_t partition_idx);

/**
 * \brief Set the signal mask for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] signal_mask          The signal mask to be set for the partition
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_signal_mask(uint32_t partition_idx,
                                       uint32_t signal_mask);
#endif /* !defined(TFM_PSA_API) */

#ifdef TFM_PSA_API
/*************************** IPC definitions **************************/

/**
 * \brief Get bottom of stack region for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Stack region bottom value
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_stack_bottom(uint32_t partition_idx);

/**
 * \brief Get top of stack region for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Stack region top value
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_stack_top(uint32_t partition_idx);

/**
 * \brief   Get the running partition ID.
 *
 * \return  Returns the partition ID
 */
uint32_t tfm_spm_partition_get_running_partition_id(void);

/**
 * \brief                   Get the current partition mode.
 *
 * \param[in] partition_flags               Flags of current partition
 *
 * \retval TFM_PARTITION_PRIVILEGED_MODE    Privileged mode
 * \retval TFM_PARTITION_UNPRIVILEGED_MODE  Unprivileged mode
 */
uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_flags);

/******************** Service handle management functions ********************/

/**
 * \brief                   Create connection handle for client connect
 *
 * \param[in] service       Target service context pointer
 *
 * \retval PSA_NULL_HANDLE  Create failed \ref PSA_NULL_HANDLE
 * \retval >0               Service handle created, \ref psa_handle_t
 */
psa_handle_t tfm_spm_create_conn_handle(struct tfm_spm_service_t *service);

/**
 * \brief                   Free connection handle which not used anymore.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find service by handle
 */
int32_t tfm_spm_free_conn_handle(struct tfm_spm_service_t *service,
                                 psa_handle_t conn_handle);

/**
 * \brief                   Set reverse handle value for connection.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 * \param[in] rhandle       rhandle need to save
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find handle node
 */
int32_t tfm_spm_set_rhandle(struct tfm_spm_service_t *service,
                            psa_handle_t conn_handle,
                            void *rhandle);

/**
 * \brief                   Get reverse handle value from connection hanlde.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle(), \ref psa_handle_t
 *
 * \retval void *           Success
 * \retval "Does not return"  Panic for those:
 *                              service pointer are NULL
 *                              hanlde is \ref PSA_NULL_HANDLE
 *                              handle node does not be found
 */
void *tfm_spm_get_rhandle(struct tfm_spm_service_t *service,
                          psa_handle_t conn_handle);

/******************** Partition management functions *************************/

/**
 * \brief                   Get current running partition context.
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Return the parttion context pointer
 *                          \ref spm_partition_desc_t structures
 */
struct spm_partition_desc_t *tfm_spm_get_running_partition(void);

/**
 * \brief                   Get the service context by signal.
 *
 * \param[in] partition     Partition context pointer
 *                          \ref spm_partition_desc_t structures
 * \param[in] signal        Signal associated with inputs to the Secure
 *                          Partition, \ref psa_signal_t
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref tfm_spm_service_t structures
 */
struct tfm_spm_service_t *
    tfm_spm_get_service_by_signal(struct spm_partition_desc_t *partition,
                                  psa_signal_t signal);

/**
 * \brief                   Get the service context by service ID.
 *
 * \param[in] sid           RoT Service identity
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref tfm_spm_service_t structures
 */
struct tfm_spm_service_t *tfm_spm_get_service_by_sid(uint32_t sid);

/**
 * \brief                   Get the service context by connection handle.
 *
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle()
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref tfm_spm_service_t structures
 */
struct tfm_spm_service_t *
    tfm_spm_get_service_by_handle(psa_handle_t conn_handle);

/**
 * \brief                   Get the partition context by partition ID.
 *
 * \param[in] partition_id  Partition identity
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target partition context pointer,
 *                          \ref spm_partition_desc_t structures
 */
struct spm_partition_desc_t *
    tfm_spm_get_partition_by_id(int32_t partition_id);

/************************ Message functions **********************************/

/**
 * \brief                   Get message context by message handle.
 *
 * \param[in] msg_handle    Message handle which is a reference generated
 *                          by the SPM to a specific message.
 *
 * \return                  The message body context pointer
 *                          \ref tfm_msg_body_t structures
 */
struct tfm_msg_body_t *tfm_spm_get_msg_from_handle(psa_handle_t msg_handle);

/**
 * \brief                   Get message context by connect handle.
 *
 * \param[in] conn_handle   Service connect handle.
 *
 * \return                  The message body context pointer
 *                          \ref msg_body_t structures
 */
struct tfm_msg_body_t *
    tfm_spm_get_msg_buffer_from_conn_handle(psa_handle_t conn_handle);

/**
 * \brief                   Fill the message for PSA client call.
 *
 * \param[in] msg           Service Message Queue buffer pointer
 * \param[in] service       Target service context pointer, which can be
 *                          obtained by partition management functions
 * \prarm[in] handle        Connect handle return by psa_connect().
 * \param[in] type          Message type, PSA_IPC_CONNECT, PSA_IPC_CALL or
 *                          PSA_IPC_DISCONNECT
 * \param[in] ns_caller     Whether from NS caller
 * \param[in] invec         Array of input \ref psa_invec structures
 * \param[in] in_len        Number of input \ref psa_invec structures
 * \param[in] outvec        Array of output \ref psa_outvec structures
 * \param[in] out_len       Number of output \ref psa_outvec structures
 * \param[in] caller_outvec Array of caller output \ref psa_outvec structures
 */
void tfm_spm_fill_msg(struct tfm_msg_body_t *msg,
                      struct tfm_spm_service_t *service,
                      psa_handle_t handle,
                      int32_t type, int32_t ns_caller,
                      psa_invec *invec, size_t in_len,
                      psa_outvec *outvec, size_t out_len,
                      psa_outvec *caller_outvec);

/**
 * \brief                   Send message and wake up the SP who is waiting on
 *                          message queue, block the current thread and
 *                          scheduler triggered
 *
 * \param[in] service       Target service context pointer, which can be
 *                          obtained by partition management functions
 * \param[in] msg           message created by tfm_spm_create_msg()
 *                          \ref tfm_msg_body_t structures
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS Bad parameters input
 * \retval IPC_ERROR_GENERIC Failed to enqueue message to service message queue
 */
int32_t tfm_spm_send_event(struct tfm_spm_service_t *service,
                           struct tfm_msg_body_t *msg);

/**
 * \brief                   Check the client minor version according to
 *                          version policy
 *
 * \param[in] service       Target service context pointer, which can be get
 *                          by partition management functions
 * \param[in] minor_version Client support minor version
 *
 * \retval IPC_SUCCESS      Success
 * \retval IPC_ERROR_BAD_PARAMETERS Bad parameters input
 * \retval IPC_ERROR_VERSION Check failed
 */
int32_t tfm_spm_check_client_version(struct tfm_spm_service_t *service,
                                     uint32_t minor_version);

/**
 * \brief                      Check the memory reference is valid.
 *
 * \param[in] buffer           Pointer of memory reference
 * \param[in] len              Length of memory reference in bytes
 * \param[in] ns_caller        From non-secure caller
 * \param[in] access           Type of access specified by the
 *                             \ref tfm_memory_access_e
 * \param[in] privileged       Privileged mode or unprivileged mode:
 *                             \ref TFM_PARTITION_UNPRIVILEGED_MODE
 *                             \ref TFM_PARTITION_PRIVILEGED_MODE
 *
 * \retval IPC_SUCCESS               Success
 * \retval IPC_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval IPC_ERROR_MEMORY_CHECK    Check failed
 */
int32_t tfm_memory_check(const void *buffer, size_t len, int32_t ns_caller,
                         enum tfm_memory_access_e access,
                         uint32_t privileged);

/* This function should be called before schedule function */
void tfm_spm_init(void);

/*
 * PendSV specified function.
 *
 * Parameters :
 *  ctxb        -    State context storage pointer
 *
 * Notes:
 *  This is a staging API. Scheduler should be called in SPM finally and
 *  this function will be obsoleted later.
 */
void tfm_pendsv_do_schedule(struct tfm_state_context_ext *ctxb);

#endif /* ifdef(TFM_PSA_API) */

#endif /*__SPM_API_H__ */
