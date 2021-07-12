/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "t_cose_crypto.h"
#include "psa/crypto.h"
#include "attest_public_key.h"

/* Avoid compiler warning due to unused argument */
#define ARG_UNUSED(arg) (void)(arg)

enum t_cose_err_t
t_cose_crypto_pub_key_verify(int32_t cose_alg_id,
                             int32_t key_select,
                             struct q_useful_buf_c key_id,
                             struct q_useful_buf_c hash_to_verify,
                             struct q_useful_buf_c signature)
{
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    enum psa_attest_err_t attest_ret;
    psa_status_t psa_ret;
    psa_key_handle_t public_key;

    ARG_UNUSED(cose_alg_id);
    ARG_UNUSED(key_select);

    attest_ret = attest_register_initial_attestation_public_key(&public_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return T_COSE_ERR_FAIL;
    }

    psa_ret = psa_asymmetric_verify(public_key,
                                    PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                                    hash_to_verify.ptr,
                                    hash_to_verify.len,
                                    signature.ptr,
                                    signature.len);
    if (psa_ret != PSA_SUCCESS) {
        cose_ret = T_COSE_ERR_SIG_VERIFY;
    }

    attest_ret = attest_unregister_initial_attestation_public_key(public_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        cose_ret = T_COSE_ERR_FAIL;
    }

    return cose_ret;
}
