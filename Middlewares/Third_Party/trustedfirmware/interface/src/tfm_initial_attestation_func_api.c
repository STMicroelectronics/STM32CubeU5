/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/initial_attestation.h"
#include "tfm_veneers.h"
#include "tfm_ns_interface.h"
#include "psa/client.h"

#define IOVEC_LEN(x) (sizeof(x)/sizeof(x[0]))

enum psa_attest_err_t
psa_initial_attest_get_token(const uint8_t *challenge_obj,
                             uint32_t       challenge_size,
                             uint8_t       *token,
                             uint32_t      *token_size)
{

    int32_t res;

    psa_invec in_vec[] = {
        {challenge_obj, challenge_size}
    };
    psa_outvec out_vec[] = {
        {token, *token_size}
    };

    res = tfm_ns_interface_dispatch(
                               (veneer_fn)tfm_initial_attest_get_token_veneer,
                               (uint32_t)in_vec,  IOVEC_LEN(in_vec),
                               (uint32_t)out_vec, IOVEC_LEN(out_vec));

    if (res == (int32_t)PSA_ATTEST_ERR_SUCCESS) {
        *token_size = out_vec[0].len;
    }

    return (enum psa_attest_err_t)res;
}

enum psa_attest_err_t
psa_initial_attest_get_token_size(uint32_t  challenge_size,
                                  uint32_t *token_size)
{
    psa_invec in_vec[] = {
        {&challenge_size, sizeof(challenge_size)}
    };
    psa_outvec out_vec[] = {
        {token_size, sizeof(uint32_t)}
    };

    return (enum psa_attest_err_t)tfm_ns_interface_dispatch(
                            (veneer_fn)tfm_initial_attest_get_token_size_veneer,
                            (uint32_t)in_vec,  IOVEC_LEN(in_vec),
                            (uint32_t)out_vec, IOVEC_LEN(out_vec));
}
