/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_IPC_H__
#define __SPM_IPC_H__

#include <stdint.h>
#include "spm_partition_defs.h"
#include "tfm_arch.h"
#include "lists.h"
#include "tfm_wait.h"
#include "tfm_secure_api.h"
#include "tfm_thread.h"
#include "psa/service.h"

#define TFM_VERSION_POLICY_RELAXED      0
#define TFM_VERSION_POLICY_STRICT       1

#define TFM_HANDLE_STATUS_IDLE          0
#define TFM_HANDLE_STATUS_ACTIVE        1
#define TFM_HANDLE_STATUS_CONNECT_ERROR 2

#define PART_REGION_ADDR(partition, region) \
    (uint32_t)&REGION_NAME(Image$$, partition, region)

#define TFM_CONN_HANDLE_MAX_NUM         16

/*
 * Set a number limit for stateless handle.
 * Valid handle must be positive, set client handle minimum value to 1.
 */
#define STATIC_HANDLE_NUM_LIMIT         32
#define CLIENT_HANDLE_VALUE_MIN         1

#define STAIC_HANDLE_IDX_BIT_WIDTH      8
#define STAIC_HANDLE_IDX_MASK \
    (uint32_t)((1UL << STAIC_HANDLE_IDX_BIT_WIDTH) - 1)
#define GET_INDEX_FROM_STATIC_HANDLE(handle) \
    (uint32_t)(((handle) & STAIC_HANDLE_IDX_MASK) - 1)

#define STAIC_HANDLE_VER_BIT_WIDTH      8
#define STAIC_HANDLE_VER_OFFSET         8
#define STAIC_HANDLE_VER_MASK \
    (uint32_t)((1UL << STAIC_HANDLE_VER_BIT_WIDTH) - 1)
#define GET_VERSION_FROM_STATIC_HANDLE(handle) \
    (uint32_t)(((handle) >> STAIC_HANDLE_VER_OFFSET) & STAIC_HANDLE_VER_MASK)

#define STAIC_HANDLE_INDICATOR_OFFSET   30
/*
 * A valid static handle must have indicator bit set, have a positive index,
 * 1 <= index <= STATIC_HANDLE_NUM_LIMIT.
 */
#define IS_VALID_STATIC_HANDLE(handle)                      \
    (((handle) & (1UL << STAIC_HANDLE_INDICATOR_OFFSET)) && \
     (GET_INDEX_FROM_STATIC_HANDLE(handle) < STATIC_HANDLE_NUM_LIMIT))

#define SPM_INVALID_PARTITION_IDX     (~0U)

/* Privileged definitions for partition thread mode */
#define TFM_PARTITION_UNPRIVILEGED_MODE 0
#define TFM_PARTITION_PRIVILEGED_MODE   1

#define SPM_PART_FLAG_APP_ROT           0x01
#define SPM_PART_FLAG_PSA_ROT           0x02
#define SPM_PART_FLAG_IPC               0x04

#define TFM_PRIORITY_HIGH               THRD_PRIOR_HIGHEST
#define TFM_PRIORITY_NORMAL             THRD_PRIOR_MEDIUM
#define TFM_PRIORITY_LOW                THRD_PRIOR_LOWEST
#define TFM_PRIORITY(LEVEL)             TFM_PRIORITY_##LEVEL

#define TFM_MSG_MAGIC                   0x15154343

/* Message struct to collect parameter from client */
struct tfm_msg_body_t {
    int32_t magic;
    struct tfm_spm_service_t *service; /* RoT service pointer            */
    struct tfm_event_t ack_evnt;       /* Event for ack reponse          */
    psa_msg_t msg;                     /* PSA message body               */
    psa_invec invec[PSA_MAX_IOVEC];    /* Put in/out vectors in msg body */
    psa_outvec outvec[PSA_MAX_IOVEC];
    psa_outvec *caller_outvec;         /*
                                        * Save caller outvec pointer for
                                        * write length update
                                        */
#ifdef TFM_MULTI_CORE_TOPOLOGY
    const void *caller_data;           /*
                                        * Pointer to the private data of the
                                        * caller. It identifies the NSPE PSA
                                        * client calls in multi-core topology
                                        */
#endif
    struct bi_list_node_t msg_node;    /* For list operators             */
};

/**
 * Holds the fields of the partition DB used by the SPM code. The values of
 * these fields are calculated at compile time, and set during initialisation
 * phase.
 */
struct partition_static_t {
    uint32_t psa_ff_ver;                /* PSA-FF version                   */
    uint32_t pid;                       /* Partition ID                     */
    uint32_t flags;                     /* Flags of the partition           */
    uint32_t priority;                  /* Priority of the partition thread */
    sp_entry_point entry;               /* Entry point of the partition     */
    uintptr_t stack_base_addr;          /* Stack base of the partition      */
    size_t stack_size;                  /* Stack size of the partition      */
    uintptr_t heap_base_addr;           /* Heap base of the partition       */
    size_t heap_size;                   /* Heap size of the partition       */
    uintptr_t platform_data;            /* Platform specific data           */
    uint32_t ndeps;                     /* Numbers of depended services     */
    uint32_t *deps;                     /* Pointer to dependency arrays     */
};

/**
 * Holds the fields that define a partition for SPM. The fields are further
 * divided to structures, to keep the related fields close to each other.
 */
struct partition_t {
    const struct partition_static_t *p_static;
    void *p_platform;
    void *p_interrupts;
    void *p_metadata;
    struct tfm_core_thread_t sp_thread;
    struct tfm_event_t event;
    struct bi_list_node_t msg_list;
    uint32_t signals_allowed;
    uint32_t signals_waiting;
    uint32_t signals_asserted;
    /** A list of platform_data pointers */
    const struct tfm_spm_partition_memory_data_t *memory_data;
};

struct spm_partition_db_t {
    uint32_t partition_count;
    struct partition_t *partitions;
};

/* Service database defined by manifest */
struct tfm_spm_service_db_t {
    char *name;                     /* Service name                          */
    uint32_t partition_id;          /* Partition ID which service belong to  */
    psa_signal_t signal;            /* Service signal                        */
    uint32_t sid;                   /* Service identifier                    */
    bool non_secure_client;         /* If can be called by non secure client */
    bool connection_based;          /* 'true' for connection-based service   */
    uint32_t version;               /* Service version                       */
    uint32_t version_policy;        /* Service version policy                */
};

/* RoT Service data */
struct tfm_spm_service_t {
    const struct tfm_spm_service_db_t *service_db;/* Service database pointer */
    struct partition_t *partition;           /*
                                              * Point to secure partition
                                              * data
                                              */
    struct bi_list_node_t handle_list;       /* Service handle list          */
    struct bi_list_node_t list;              /* For list operation           */
};

/* Stateless RoT service tracking array item type. Indexed by static handle */
struct stateless_service_tracking_t {
    uint32_t                 sid;           /* Service ID */
    struct tfm_spm_service_t *p_service;    /* Service instance */
};

/* RoT connection handle list */
struct tfm_conn_handle_t {
    void *rhandle;                      /* Reverse handle value              */
    uint32_t status;                    /*
                                         * Status of handle, three valid
                                         * options:
                                         * TFM_HANDLE_STATUS_ACTIVE,
                                         * TFM_HANDLE_STATUS_IDLE and
                                         * TFM_HANDLE_STATUS_CONNECT_ERROR
                                         */
    int32_t client_id;                  /*
                                         * Partition ID of the sender of the
                                         * message:
                                         *  - secure partition id;
                                         *  - non secure client endpoint id.
                                         */
    struct tfm_msg_body_t internal_msg; /* Internal message for message queue */
    struct tfm_spm_service_t *service;  /* RoT service pointer                */
    struct bi_list_node_t list;         /* list node                          */
};

enum tfm_memory_access_e {
    TFM_MEMORY_ACCESS_RO = 1,
    TFM_MEMORY_ACCESS_RW = 2,
};

/**
 * \brief                   Get the current partition mode.
 *
 * \param[in] partition_flags               Flags of current partition
 *
 * \retval TFM_PARTITION_PRIVILEGED_MODE    Privileged mode
 * \retval TFM_PARTITION_UNPRIVILEGED_MODE  Unprivileged mode
 */
uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_flags);

/**
 * \brief                   Handle an SPM request by a secure service
 * \param[in] svc_ctx       The stacked SVC context
 */
void tfm_spm_request_handler(const struct tfm_state_context_t *svc_ctx);

/**
 * \brief   Get the running partition ID.
 *
 * \return  Returns the partition ID
 */
uint32_t tfm_spm_partition_get_running_partition_id(void);

/******************** Service handle management functions ********************/

/**
 * \brief                   Create connection handle for client connect
 *
 * \param[in] service       Target service context pointer
 * \param[in] client_id     Partition ID of the sender of the message
 *
 * \retval NULL             Create failed
 * \retval "Not NULL"       Service handle created
 */
struct tfm_conn_handle_t *tfm_spm_create_conn_handle(
                                        struct tfm_spm_service_t *service,
                                        int32_t client_id);

/**
 * \brief                   Validate connection handle for client connect
 *
 * \param[in] conn_handle   Handle to be validated
 * \param[in] client_id     Partition ID of the sender of the message
 *
 * \retval SPM_SUCCESS        Success
 * \retval SPM_ERROR_GENERIC  Invalid handle
 */
int32_t tfm_spm_validate_conn_handle(
                                    const struct tfm_conn_handle_t *conn_handle,
                                    int32_t client_id);

/**
 * \brief                   Free connection handle which not used anymore.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle()
 *
 * \retval SPM_SUCCESS      Success
 * \retval SPM_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find service by handle
 */
int32_t tfm_spm_free_conn_handle(struct tfm_spm_service_t *service,
                                 struct tfm_conn_handle_t *conn_handle);

/******************** Partition management functions *************************/

/**
 * \brief                   Get the msg context by signal.
 *
 * \param[in] partition     Partition context pointer
 *                          \ref partition_t structures
 * \param[in] signal        Signal associated with inputs to the Secure
 *                          Partition, \ref psa_signal_t
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target service context pointer,
 *                          \ref tfm_msg_body_t structures
 */
struct tfm_msg_body_t *tfm_spm_get_msg_by_signal(struct partition_t *partition,
                                                 psa_signal_t signal);

/**
 * \brief                   Get current running partition context.
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Return the parttion context pointer
 *                          \ref partition_t structures
 */
struct partition_t *tfm_spm_get_running_partition(void);

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
 tfm_spm_get_msg_buffer_from_conn_handle(struct tfm_conn_handle_t *conn_handle);

/**
 * \brief                   Fill the message for PSA client call.
 *
 * \param[in] msg           Service Message Queue buffer pointer
 * \param[in] service       Target service context pointer, which can be
 *                          obtained by partition management functions
 * \prarm[in] handle        Connect handle return by psa_connect().
 * \param[in] type          Message type, PSA_IPC_CONNECT, PSA_IPC_CALL or
 *                          PSA_IPC_DISCONNECT
 * \param[in] client_id     Partition ID of the sender of the message
 * \param[in] invec         Array of input \ref psa_invec structures
 * \param[in] in_len        Number of input \ref psa_invec structures
 * \param[in] outvec        Array of output \ref psa_outvec structures
 * \param[in] out_len       Number of output \ref psa_outvec structures
 * \param[in] caller_outvec Array of caller output \ref psa_outvec structures
 */
void tfm_spm_fill_msg(struct tfm_msg_body_t *msg,
                      struct tfm_spm_service_t *service,
                      psa_handle_t handle,
                      int32_t type, int32_t client_id,
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
 */
void tfm_spm_send_event(struct tfm_spm_service_t *service,
                        struct tfm_msg_body_t *msg);

/**
 * \brief                   Check the client version according to
 *                          version policy
 *
 * \param[in] service       Target service context pointer, which can be get
 *                          by partition management functions
 * \param[in] version       Client support version
 *
 * \retval SPM_SUCCESS      Success
 * \retval SPM_ERROR_BAD_PARAMETERS Bad parameters input
 * \retval SPM_ERROR_VERSION Check failed
 */
int32_t tfm_spm_check_client_version(struct tfm_spm_service_t *service,
                                     uint32_t version);

/**
 * \brief                   Check the client access authorization
 *
 * \param[in] sid           Target RoT Service identity
 * \param[in] service       Target service context pointer, which can be get
 *                          by partition management functions
 * \param[in] ns_caller     Whether from NS caller
 *
 * \retval SPM_SUCCESS      Success
 * \retval SPM_ERROR_GENERIC Authorization check failed
 */
int32_t tfm_spm_check_authorization(uint32_t sid,
                                    struct tfm_spm_service_t *service,
                                    bool ns_caller);

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
 * \retval SPM_SUCCESS               Success
 * \retval SPM_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval SPM_ERROR_MEMORY_CHECK    Check failed
 */
int32_t tfm_memory_check(const void *buffer, size_t len, bool ns_caller,
                         enum tfm_memory_access_e access,
                         uint32_t privileged);

/*
 * PendSV specified function.
 *
 * Parameters :
 *  p_actx        -    Architecture context storage pointer
 *
 * Notes:
 *  This is a staging API. Scheduler should be called in SPM finally and
 *  this function will be obsoleted later.
 */
void tfm_pendsv_do_schedule(struct tfm_arch_ctx_t *p_actx);

/**
 * \brief                      SPM initialization implementation
 *
 * \details                    This function must be called under handler mode.
 * \retval                     This function returns an EXC_RETURN value. Other
 *                             faults would panic the execution and never
 *                             returned.
 */
uint32_t tfm_spm_init(void);

#if !defined(__ARM_ARCH_8_1M_MAIN__)
/**
 * \brief Validate the whether NS caller re-enter.
 *
 * \param[in] p_cur_sp          Pointer to current partition.
 * \param[in] p_ctx             Pointer to current stack context.
 * \param[in] exc_return        EXC_RETURN value.
 * \param[in] ns_caller         If 'true', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval void                 Success.
 *
 * Notes:
 *  For architecture v8.1m and later, will use hardware re-entrant detection.
 *  Otherwise will use the software solution to validate the caller.
 */
void tfm_spm_validate_caller(struct partition_t *p_cur_sp, uint32_t *p_ctx,
                             uint32_t exc_return, bool ns_caller);
#else
/**
 * In v8.1 mainline, will use hardware re-entrant detection instead.
 */
__STATIC_INLINE
void tfm_spm_validate_caller(struct partition_t *p_cur_sp, uint32_t *p_ctx,
                             uint32_t exc_return, bool ns_caller)
{
    (void)p_cur_sp;
    (void)p_ctx;
    (void)exc_return;
    (void)ns_caller;
    return;
}
#endif


/**
 * \brief Converts a handle instance into a corresponded user handle.
 */
psa_handle_t tfm_spm_to_user_handle(struct tfm_conn_handle_t *handle_instance);

/**
 * \brief Converts a user handle into a corresponded handle instance.
 */
struct tfm_conn_handle_t *tfm_spm_to_handle_instance(psa_handle_t user_handle);

/**
 * \brief Move to handler mode by a SVC for specific purpose
 */
void tfm_core_handler_mode(void);

/**
 * \brief                   Set reverse handle value for connection.
 *
 * \param[in] service       Target service context pointer
 * \param[in] conn_handle   Connection handle created by
 *                          tfm_spm_create_conn_handle()
 * \param[in] rhandle       rhandle need to save
 *
 * \retval SPM_SUCCESS      Success
 * \retval SPM_ERROR_BAD_PARAMETERS  Bad parameters input
 * \retval "Does not return"  Panic for not find handle node
 */
int32_t tfm_spm_set_rhandle(struct tfm_spm_service_t *service,
                            struct tfm_conn_handle_t *conn_handle,
                            void *rhandle);

void update_caller_outvec_len(struct tfm_msg_body_t *msg);

/**
 * \brief   notify the partition with the signal.
 *
 * \param[in] partition_id      The ID of the partition to be notified.
 * \param[in] signal            The signal that the partition is to be notified
 *                              with.
 *
 * \retval void                 Success.
 * \retval "Does not return"    If partition_id is invalid.
 */
void notify_with_signal(int32_t partition_id, psa_signal_t signal);

/**
 * \brief Return the IRQ line number associated with a signal
 *
 * \param[in]      partition_id    The ID of the partition in which we look for
 *                                 the signal.
 * \param[in]      signal          The signal to query for.
 *
 * \retval None-negative value  The irq line associated with signal
 * \retval Negative value       if one of more the following are true:
 *                              - the \ref signal indicates more than one signal
 *                              - the \ref signal does not belong to the
 *                                partition.
 */
int32_t get_irq_line_for_signal(int32_t partition_id, psa_signal_t signal);

#endif /* __SPM_IPC_H__ */
