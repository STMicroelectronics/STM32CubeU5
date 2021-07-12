/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "psa/initial_attestation.h"
#include "attestation.h"

#ifdef TFM_PSA_API
#include "psa_manifest/tfm_initial_attestation.h"
#include "tfm_client.h"
#include "psa/service.h"
#include "region_defs.h"

#define IOVEC_LEN(x) (sizeof(x)/sizeof(x[0]))

typedef enum psa_attest_err_t (*attest_func_t)(const psa_msg_t *msg);

int32_t g_attest_caller_id;

static psa_status_t psa_attest_get_token(const psa_msg_t *msg)
{
    enum psa_attest_err_t status = PSA_ATTEST_ERR_SUCCESS;
    uint8_t challenge_buff[PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64];
    uint8_t token_buff[PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE];
    uint32_t bytes_read = 0;
    size_t challenge_size = msg->in_size[0];
    size_t token_size = msg->out_size[0];
    psa_invec in_vec[] = {
        {challenge_buff, challenge_size}
    };
    
    psa_outvec out_vec[] = {
        {token_buff, sizeof(token_buff)}
    };

    if (challenge_size > PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }
    if (token_size < sizeof(token_buff)) {
        out_vec[0].len = token_size;
    }
    /* store the client ID here for later use in service */
    g_attest_caller_id = msg->client_id;

    bytes_read = psa_read(msg->handle, 0,
                          challenge_buff, challenge_size);
    if (bytes_read != challenge_size) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    status = initial_attest_get_token(in_vec, IOVEC_LEN(in_vec),
                                      out_vec, IOVEC_LEN(out_vec));
    if (status == PSA_ATTEST_ERR_SUCCESS) {
        psa_write(msg->handle, 0, out_vec[0].base, out_vec[0].len);
    }

    return status;
}

static enum psa_attest_err_t psa_attest_get_token_size(const psa_msg_t *msg)
{
    enum psa_attest_err_t status = PSA_ATTEST_ERR_SUCCESS;
    uint32_t challenge_size;
    uint32_t token_size;
    uint32_t bytes_read = 0;
    psa_invec in_vec[] = {
        {&challenge_size, msg->in_size[0]}
    };
    psa_outvec out_vec[] = {
        {&token_size, msg->out_size[0]}
    };

    if (msg->in_size[0] != sizeof(challenge_size)
        || msg->out_size[0] != sizeof(token_size)) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    /* store the client ID here for later use in service */
    g_attest_caller_id = msg->client_id;

    bytes_read = psa_read(msg->handle, 0,
                          &challenge_size, msg->in_size[0]);
    if (bytes_read != msg->in_size[0]) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    status = initial_attest_get_token_size(in_vec, IOVEC_LEN(in_vec),
                                           out_vec, IOVEC_LEN(out_vec));
    if (status == PSA_ATTEST_ERR_SUCCESS) {
        psa_write(msg->handle, 0, out_vec[0].base, out_vec[0].len);
    }

    return status;
}

/*
 * Fixme: Temporarily implement abort as infinite loop,
 * will replace it later.
 */
static void tfm_abort(void)
{
    while (1)
        ;
}

static void attest_signal_handle(psa_signal_t signal, attest_func_t pfn)
{
    psa_msg_t msg;
    psa_status_t status;

    status = psa_get(signal, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        status = (psa_status_t)pfn(&msg);
        psa_reply(msg.handle, status);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        tfm_abort();
    }
}
#endif

enum psa_attest_err_t attest_partition_init(void)
{
    enum psa_attest_err_t err = PSA_ATTEST_ERR_SUCCESS;
#ifdef TFM_PSA_API
    psa_signal_t signals;
#endif

    err = attest_init();
#ifdef TFM_PSA_API
    if (err != PSA_ATTEST_ERR_SUCCESS) {
        tfm_abort();
    }

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & PSA_ATTEST_GET_TOKEN_SIG) {
            attest_signal_handle(PSA_ATTEST_GET_TOKEN_SIG,
                             (attest_func_t)psa_attest_get_token);
        } else if (signals & PSA_ATTEST_GET_TOKEN_SIZE_SIG) {
            attest_signal_handle(PSA_ATTEST_GET_TOKEN_SIZE_SIG,
                                 psa_attest_get_token_size);
        } else {
            tfm_abort();
        }
    }
    /* NOTREACHED */
#endif
    return err;
}
