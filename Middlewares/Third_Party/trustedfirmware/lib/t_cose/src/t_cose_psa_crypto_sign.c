/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "t_cose_crypto.h"
#include "attestation_key.h"
#include "tfm_plat_defs.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_memory_utils.h"
#include "psa/crypto.h"

/* Avoid compiler warning due to unused argument */
#define ARG_UNUSED(arg) (void)(arg)

enum t_cose_err_t
t_cose_crypto_pub_key_sign(int32_t cose_alg_id,
                           int32_t key_select,
                           struct q_useful_buf_c hash_to_sign,
                           struct q_useful_buf signature_buffer,
                           struct q_useful_buf_c *signature)
{
    enum t_cose_err_t cose_ret = T_COSE_SUCCESS;
    enum psa_attest_err_t attest_ret;
    psa_status_t psa_ret;
    const size_t sig_size = t_cose_signature_size(cose_alg_id);
    psa_key_handle_t private_key;

    ARG_UNUSED(key_select);

    if (sig_size > signature_buffer.len) {
        return T_COSE_ERR_SIG_BUFFER_SIZE;
    }

    /* FixMe: Registration of key(s) should not be done by attestation service.
     *        Later Crypto service is going to get the attestation key from
     *        platform layer.
     */
    attest_ret = attest_register_initial_attestation_private_key(&private_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        return T_COSE_ERR_FAIL;
    }

    psa_ret = psa_asymmetric_sign(private_key,
                                  PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                                  hash_to_sign.ptr,
                                  hash_to_sign.len,
                                  signature_buffer.ptr, /* Sig buf */
                                  signature_buffer.len, /* Sig buf size */
                                  &(signature->len));   /* Sig length */

    if (psa_ret != PSA_SUCCESS) {
        cose_ret = T_COSE_ERR_FAIL;
    } else {
        signature->ptr = signature_buffer.ptr;
    }

    attest_ret = attest_unregister_initial_attestation_private_key(private_key);
    if (attest_ret != PSA_ATTEST_ERR_SUCCESS) {
        cose_ret =  T_COSE_ERR_FAIL;
    }

    return cose_ret;
}

#ifdef INCLUDE_TEST_CODE_AND_KEY_ID /* Remove them from release build */
enum t_cose_err_t
t_cose_crypto_get_ec_pub_key(int32_t key_select,
                             struct q_useful_buf_c kid,
                             int32_t *cose_curve_id,
                             struct q_useful_buf buf_to_hold_x_coord,
                             struct q_useful_buf buf_to_hold_y_coord,
                             struct q_useful_buf_c *x_coord,
                             struct q_useful_buf_c *y_coord)
{
    enum tfm_plat_err_t plat_res;
    enum ecc_curve_t cose_curve;
    struct ecc_key_t attest_key = {0};
    uint8_t  key_buf[ECC_P_256_KEY_SIZE];

    ARG_UNUSED(key_select);

    /* Get the initial attestation key */
    plat_res = tfm_plat_get_initial_attest_key(key_buf, sizeof(key_buf),
                                               &attest_key, &cose_curve);

    /* Check the availability of the private key */
    if (plat_res != TFM_PLAT_ERR_SUCCESS ||
        attest_key.pubx_key == NULL ||
        attest_key.puby_key == NULL) {
        return T_COSE_ERR_KEY_BUFFER_SIZE;
    }

    *cose_curve_id = (int32_t)cose_curve;

    *x_coord = q_useful_buf_copy_ptr(buf_to_hold_x_coord,
                                     attest_key.pubx_key,
                                     attest_key.pubx_key_size);

    *y_coord = q_useful_buf_copy_ptr(buf_to_hold_y_coord,
                                     attest_key.puby_key,
                                     attest_key.puby_key_size);

    if(q_useful_buf_c_is_null(*x_coord) || q_useful_buf_c_is_null(*y_coord)) {
        return T_COSE_ERR_KEY_BUFFER_SIZE;
    }

    return T_COSE_SUCCESS;
}
#endif /* INCLUDE_TEST_CODE_AND_KEY_ID */
