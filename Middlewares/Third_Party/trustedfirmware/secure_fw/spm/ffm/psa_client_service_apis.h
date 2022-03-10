/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_CLIENT_SERVICE_APIS_H__
#define __PSA_CLIENT_SERVICE_APIS_H__

/* Svcall for PSA Client APIs */

/*
 * \brief This function get the current PSA RoT lifecycle state.
 *
 * \return state                The current security lifecycle state of the PSA
 *                              RoT. The PSA state and implementation state are
 *                              encoded as follows:
 * \arg                           state[15:8] – PSA lifecycle state
 * \arg                           state[7:0] – IMPLEMENTATION DEFINED state
 */
uint32_t tfm_spm_get_lifecycle_state(void);

/**
 * \brief SVC handler for \ref psa_framework_version.
 *
 * \return version              The version of the PSA Framework implementation
 *                              that is providing the runtime services to the
 *                              caller.
 */
uint32_t tfm_spm_psa_framework_version(void);

/**
 * \brief SVC handler for \ref psa_version.
 *
 * \param[in] args              Include all input arguments: sid.
 * \param[in] ns_caller         If 'true', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval PSA_VERSION_NONE     The RoT Service is not implemented, or the
 *                              caller is not permitted to access the service.
 * \retval > 0                  The version of the implemented RoT Service.
 */
uint32_t tfm_spm_psa_version(uint32_t *args, bool ns_caller);

/**
 * \brief SVC handler for \ref psa_connect.
 *
 * \param[in] args              Include all input arguments:
 *                              sid, version.
 * \param[in] ns_caller         If 'true', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval PSA_SUCCESS          Success.
 * \retval PSA_ERROR_CONNECTION_REFUSED The SPM or RoT Service has refused the
 *                              connection.
 * \retval PSA_ERROR_CONNECTION_BUSY The SPM or RoT Service cannot make the
 *                              connection at the moment.
 * \retval "Does not return"    The RoT Service ID and version are not
 *                              supported, or the caller is not permitted to
 *                              access the service.
 */
psa_status_t tfm_spm_psa_connect(uint32_t *args, bool ns_caller);

/**
 * \brief SVC handler for \ref psa_call.
 *
 * \param[in] args              Include all input arguments:
 *                              handle, in_vec, in_len, out_vec, out_len.
 * \param[in] ns_caller         If 'true', call from non-secure client.
 *                              Or from secure client.
 * \param[in] lr                EXC_RETURN value of the SVC.
 *
 * \retval >=0                  RoT Service-specific status value.
 * \retval <0                   RoT Service-specific error code.
 * \retval PSA_ERROR_PROGRAMMER_ERROR The connection has been terminated by the
 *                              RoT Service. The call is a PROGRAMMER ERROR if
 *                              one or more of the following are true:
 * \arg                           An invalid handle was passed.
 * \arg                           The connection is already handling a request.
 * \arg                           type < 0.
 * \arg                           An invalid memory reference was provided.
 * \arg                           in_len + out_len > PSA_MAX_IOVEC.
 * \arg                           The message is unrecognized by the RoT
 *                                Service or incorrectly formatted.
 */
psa_status_t tfm_spm_psa_call(uint32_t *args, bool ns_caller, uint32_t lr);

/**
 * \brief SVC handler for \ref psa_close.
 *
 * \param[in] args              Include all input arguments: handle.
 * \param[in] ns_caller         If 'true', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           An invalid handle was provided that is not
 *                                the null handle.
 * \arg                           The connection is handling a request.
 */
void tfm_spm_psa_close(uint32_t *args, bool ns_caller);

/* Svcall for PSA Service APIs */

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
psa_signal_t tfm_spm_psa_wait(uint32_t *args);

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
 * \arg                           signal does not correspond to an RoT Service.
 * \arg                           The RoT Service signal is not currently
 *                                asserted.
 * \arg                           The msg pointer provided is not a valid memory
 *                                reference.
 */
psa_status_t tfm_spm_psa_get(uint32_t *args);

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
void tfm_spm_psa_set_rhandle(uint32_t *args);

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
size_t tfm_spm_psa_read(uint32_t *args);

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
size_t tfm_spm_psa_skip(uint32_t *args);

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
void tfm_spm_psa_write(uint32_t *args);

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
void tfm_spm_psa_reply(uint32_t *args);

/**
 * \brief SVC handler for \ref psa_notify.
 *
 * \param[in] args              Include all input arguments: partition_id.
 *
 * \retval void                 Success.
 * \retval "Does not return"    partition_id does not correspond to a Secure
 *                              Partition.
 */
void tfm_spm_psa_notify(uint32_t *args);

/**
 * \brief SVC handler for \ref psa_clear.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The Secure Partition's doorbell signal is not
 *                              currently asserted.
 */
void tfm_spm_psa_clear(void);

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
void tfm_spm_psa_eoi(uint32_t *args);

/**
 * \brief Terminate execution within the calling Secure Partition and will not
 *        return.
 *
 * \retval "Does not return"
 */
void tfm_spm_psa_panic(void);

/**
 * \brief SVC handler for \ref psa_irq_enable.
 *
 * \param[in] args            Include all input arguments: irq_signal.
 *
 * \retval void
 * \retval "Does not return"  The call is invalid, if one or more of the
 *                            following are true:
 *                            - irq_signal does not belong to the calling
 *                              partition.
 *                            - irq_signal indicates more than one signal.
 */
void tfm_spm_irq_enable(uint32_t *args);

/**
 * \brief SVC handler for \ref psa_irq_disable.
 *
 * \param[in] args            Include all input arguments: irq_signal.
 *
 * \retval 0                  The interrupt was disabled prior to this call
 *         1                  The interrupt was enabled prior to this call
 * \retval "Does not return"  The call is invalid, if one or more of the
 *                            following are true:
 *                            - irq_signal does not belong to the calling
 *                              partition.
 *                            - irq_signal indicates more than one signal.
 */
psa_irq_status_t tfm_spm_irq_disable(uint32_t *args);

#endif /* __PSA_CLIENT_SERVICE_APIS_H__ */
