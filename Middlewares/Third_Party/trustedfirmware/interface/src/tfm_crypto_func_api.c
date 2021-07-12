/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_veneers.h"
#include "tfm_crypto_defs.h"
#include "psa/crypto.h"
#include "tfm_ns_interface.h"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#define API_DISPATCH(sfn_name, sfn_id)                               \
        tfm_ns_interface_dispatch((veneer_fn)tfm_##sfn_name##_veneer,\
        (uint32_t)in_vec, ARRAY_SIZE(in_vec),                        \
        (uint32_t)out_vec, ARRAY_SIZE(out_vec))

#define API_DISPATCH_NO_OUTVEC(sfn_name, sfn_id)                     \
        tfm_ns_interface_dispatch((veneer_fn)tfm_##sfn_name##_veneer,\
        (uint32_t)in_vec, ARRAY_SIZE(in_vec),                        \
        (uint32_t)NULL, 0)

psa_status_t psa_crypto_init(void)
{
    /* Service init is performed during TFM boot up,
     * so application level initialisation is empty
     */
    return PSA_SUCCESS;
}

psa_status_t psa_allocate_key(psa_key_handle_t *handle)
{
    psa_status_t status;
    const struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ALLOCATE_KEY_SID,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = handle, .len = sizeof(psa_key_handle_t)},
    };

    status = API_DISPATCH(tfm_crypto_allocate_key,
                          TFM_CRYPTO_ALLOCATE_KEY);

    return status;
}

psa_status_t psa_open_key(psa_key_lifetime_t lifetime,
                          psa_key_id_t id,
                          psa_key_handle_t *handle)
{
    (void)lifetime;
    (void)id;
    (void)handle;

    /* TODO: Persistent key APIs are not supported yet */
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_create_key(psa_key_lifetime_t lifetime,
                            psa_key_id_t id,
                            psa_key_handle_t *handle)
{
    (void)lifetime;
    (void)id;
    (void)handle;

    /* TODO: Persistent key APIs are not supported yet */
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_close_key(psa_key_handle_t handle)
{
    (void)handle;

    /* TODO: Persistent key APIs are not supported yet */
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_import_key(psa_key_handle_t handle,
                            psa_key_type_t type,
                            const uint8_t *data,
                            size_t data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_IMPORT_KEY_SID,
        .key_handle = handle,
        .type = type,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = data, .len = data_length}
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_import_key,
                                    TFM_CRYPTO_IMPORT_KEY);

    return status;
}

psa_status_t psa_destroy_key(psa_key_handle_t handle)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_DESTROY_KEY_SID,
        .key_handle = handle,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_destroy_key,
                                    TFM_CRYPTO_DESTROY_KEY);

    return status;
}

psa_status_t psa_get_key_information(psa_key_handle_t handle,
                                     psa_key_type_t *type,
                                     size_t *bits)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GET_KEY_INFORMATION_SID,
        .key_handle = handle,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = type, .len = sizeof(psa_key_type_t)},
        {.base = bits, .len = sizeof(size_t)}
    };

    status = API_DISPATCH(tfm_crypto_get_key_information,
                          TFM_CRYPTO_GET_KEY_INFORMATION);

    return status;
}

psa_status_t psa_export_key(psa_key_handle_t handle,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_EXPORT_KEY_SID,
        .key_handle = handle,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = data, .len = data_size}
    };

    status = API_DISPATCH(tfm_crypto_export_key,
                          TFM_CRYPTO_EXPORT_KEY);

    *data_length = out_vec[0].len;


    return status;
}

psa_status_t psa_export_public_key(psa_key_handle_t handle,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_EXPORT_PUBLIC_KEY_SID,
        .key_handle = handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = data, .len = data_size}
    };

    status = API_DISPATCH(tfm_crypto_export_public_key,
                          TFM_CRYPTO_EXPORT_PUBLIC_KEY);

    *data_length = out_vec[0].len;

    return status;
}

psa_status_t psa_copy_key(psa_key_handle_t source_handle,
                          psa_key_handle_t target_handle,
                          const psa_key_policy_t *constraint)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_COPY_KEY_SID,
        .key_handle = source_handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = &target_handle, .len = sizeof(psa_key_handle_t)},
        {.base = constraint, .len = sizeof(psa_key_policy_t)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_copy_key,
                                    TFM_CRYPTO_COPY_KEY);

    return status;
}

void psa_key_policy_set_usage(psa_key_policy_t *policy,
                              psa_key_usage_t usage,
                              psa_algorithm_t alg)
{
    policy->usage = usage;
    policy->alg = alg;
}

psa_key_usage_t psa_key_policy_get_usage(const psa_key_policy_t *policy)
{
    return policy->usage;
}

psa_algorithm_t psa_key_policy_get_algorithm(const psa_key_policy_t *policy)
{
    return policy->alg;
}

psa_status_t psa_set_key_policy(psa_key_handle_t handle,
                                const psa_key_policy_t *policy)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_SET_KEY_POLICY_SID,
        .key_handle = handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = policy, .len = sizeof(psa_key_policy_t)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_set_key_policy,
                                    TFM_CRYPTO_SET_KEY_POLICY);

    return status;
}

psa_status_t psa_get_key_policy(psa_key_handle_t handle,
                                psa_key_policy_t *policy)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GET_KEY_POLICY_SID,
        .key_handle = handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = policy, .len = sizeof(psa_key_policy_t)},
    };

    status = API_DISPATCH(tfm_crypto_get_key_policy,
                          TFM_CRYPTO_GET_KEY_POLICY);

    return status;
}

psa_status_t psa_get_key_lifetime(psa_key_handle_t handle,
                                  psa_key_lifetime_t *lifetime)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GET_KEY_LIFETIME_SID,
        .key_handle = handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = lifetime, .len = sizeof(psa_key_lifetime_t)},
    };

    status = API_DISPATCH(tfm_crypto_get_key_lifetime,
                          TFM_CRYPTO_GET_KEY_LIFETIME);

    return status;
}

psa_status_t psa_cipher_generate_iv(psa_cipher_operation_t *operation,
                                    unsigned char *iv,
                                    size_t iv_size,
                                    size_t *iv_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_GENERATE_IV_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = iv, .len = iv_size},
    };

    status = API_DISPATCH(tfm_crypto_cipher_generate_iv,
                          TFM_CRYPTO_CIPHER_GENERATE_IV);

    *iv_length = out_vec[1].len;

    return status;
}

psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const unsigned char *iv,
                               size_t iv_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_SET_IV_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = iv, .len = iv_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_set_iv,
                          TFM_CRYPTO_CIPHER_SET_IV);

    return status;
}

psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_ENCRYPT_SETUP_SID,
        .key_handle = handle,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_encrypt_setup,
                          TFM_CRYPTO_CIPHER_ENCRYPT_SETUP);

    return status;
}

psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_handle_t handle,
                                      psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_DECRYPT_SETUP_SID,
        .key_handle = handle,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_decrypt_setup,
                          TFM_CRYPTO_CIPHER_DECRYPT_SETUP);

    return status;
}

psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input,
                               size_t input_length,
                               unsigned char *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = output, .len = output_size}
    };

    status = API_DISPATCH(tfm_crypto_cipher_update,
                          TFM_CRYPTO_CIPHER_UPDATE);

    *output_length = out_vec[1].len;

    return status;
}

psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_cipher_abort,
                          TFM_CRYPTO_CIPHER_ABORT);

    return status;
}

psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output,
                               size_t output_size,
                               size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_CIPHER_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = output, .len = output_size},
    };

    status = API_DISPATCH(tfm_crypto_cipher_finish,
                          TFM_CRYPTO_CIPHER_FINISH);

    *output_length = out_vec[1].len;

    return status;
}

psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_SETUP_SID,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_setup,
                          TFM_CRYPTO_HASH_SETUP);

    return status;
}

psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_update,
                          TFM_CRYPTO_HASH_UPDATE);

    return status;
}

psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = hash, .len = hash_size},
    };

    status = API_DISPATCH(tfm_crypto_hash_finish,
                          TFM_CRYPTO_HASH_FINISH);

    *hash_length = out_vec[1].len;

    return status;
}

psa_status_t psa_hash_verify(psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_VERIFY_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_verify,
                          TFM_CRYPTO_HASH_VERIFY);

    return status;
}

psa_status_t psa_hash_abort(psa_hash_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_abort,
                          TFM_CRYPTO_HASH_ABORT);

    return status;
}

psa_status_t psa_hash_clone(const psa_hash_operation_t *source_operation,
                            psa_hash_operation_t *target_operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_HASH_CLONE_SID,
        .op_handle = source_operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = target_operation, .len = sizeof(psa_hash_operation_t)},
    };

    status = API_DISPATCH(tfm_crypto_hash_clone,
                          TFM_CRYPTO_HASH_CLONE);

    return status;
}

psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                psa_key_handle_t handle,
                                psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_SIGN_SETUP_SID,
        .key_handle = handle,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_sign_setup,
                          TFM_CRYPTO_MAC_SIGN_SETUP);

    return status;
}

psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  psa_key_handle_t handle,
                                  psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_VERIFY_SETUP_SID,
        .key_handle = handle,
        .alg = alg,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_verify_setup,
                          TFM_CRYPTO_MAC_VERIFY_SETUP);

    return status;
}

psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_UPDATE_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_update,
                          TFM_CRYPTO_MAC_UPDATE);

    return status;
}

psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_SIGN_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
        {.base = mac, .len = mac_size},
    };

    status = API_DISPATCH(tfm_crypto_mac_sign_finish,
                          TFM_CRYPTO_MAC_SIGN_FINISH);

    *mac_length = out_vec[1].len;

    return status;
}

psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_VERIFY_FINISH_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = mac, .len = mac_length},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_verify_finish,
                          TFM_CRYPTO_MAC_VERIFY_FINISH);

    return status;
}

psa_status_t psa_mac_abort(psa_mac_operation_t *operation)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_MAC_ABORT_SID,
        .op_handle = operation->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };
    psa_outvec out_vec[] = {
        {.base = &(operation->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_mac_abort,
                          TFM_CRYPTO_MAC_ABORT);

    return status;
}

psa_status_t psa_aead_encrypt(psa_key_handle_t handle,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *plaintext,
                              size_t plaintext_length,
                              uint8_t *ciphertext,
                              size_t ciphertext_size,
                              size_t *ciphertext_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_AEAD_ENCRYPT_SID,
        .key_handle = handle,
        .alg = alg,
        .aead_in = {.nonce = {0}, .nonce_length = nonce_length}
    };

    /* Sanitize the optional input */
    if ((additional_data == NULL) && (additional_data_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    size_t idx = 0;
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = plaintext, .len = plaintext_length},
        {.base = additional_data, .len = additional_data_length},
    };
    psa_outvec out_vec[] = {
        {.base = ciphertext, .len = ciphertext_size},
    };

    if (nonce_length > TFM_CRYPTO_MAX_NONCE_LENGTH) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (nonce != NULL) {
        for (idx = 0; idx < nonce_length; idx++) {
            iov.aead_in.nonce[idx] = nonce[idx];
        }
    }

    status = API_DISPATCH(tfm_crypto_aead_encrypt,
                          TFM_CRYPTO_AEAD_ENCRYPT);

    *ciphertext_length = out_vec[0].len;

    return status;
}

psa_status_t psa_aead_decrypt(psa_key_handle_t handle,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *ciphertext,
                              size_t ciphertext_length,
                              uint8_t *plaintext,
                              size_t plaintext_size,
                              size_t *plaintext_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_AEAD_DECRYPT_SID,
        .key_handle = handle,
        .alg = alg,
        .aead_in = {.nonce = {0}, .nonce_length = nonce_length}
    };

    /* Sanitize the optional input */
    if ((additional_data == NULL) && (additional_data_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    size_t idx = 0;
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = ciphertext, .len = ciphertext_length},
        {.base = additional_data, .len = additional_data_length},
    };
    psa_outvec out_vec[] = {
        {.base = plaintext, .len = plaintext_size},
    };

    if (nonce_length > TFM_CRYPTO_MAX_NONCE_LENGTH) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (nonce != NULL) {
        for (idx = 0; idx < nonce_length; idx++) {
            iov.aead_in.nonce[idx] = nonce[idx];
        }
    }

    status = API_DISPATCH(tfm_crypto_aead_decrypt,
                          TFM_CRYPTO_AEAD_DECRYPT);

    *plaintext_length = out_vec[0].len;

    return status;
}

psa_status_t psa_asymmetric_sign(psa_key_handle_t handle,
                                 psa_algorithm_t alg,
                                 const uint8_t *hash,
                                 size_t hash_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_SIGN_SID,
        .key_handle = handle,
        .alg = alg,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
    };
    psa_outvec out_vec[] = {
        {.base = signature, .len = signature_size},
    };

    status = API_DISPATCH(tfm_crypto_asymmetric_sign,
                          TFM_CRYPTO_ASYMMETRIC_SIGN);

    *signature_length = out_vec[0].len;

    return status;
}

psa_status_t psa_asymmetric_verify(psa_key_handle_t handle,
                                   psa_algorithm_t alg,
                                   const uint8_t *hash,
                                   size_t hash_length,
                                   const uint8_t *signature,
                                   size_t signature_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_VERIFY_SID,
        .key_handle = handle,
        .alg = alg
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = hash, .len = hash_length},
        {.base = signature, .len = signature_length}
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_asymmetric_verify,
                                    TFM_CRYPTO_ASYMMETRIC_VERIFY);

    return status;
}

psa_status_t psa_asymmetric_encrypt(psa_key_handle_t handle,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_ENCRYPT_SID,
        .key_handle = handle,
        .alg = alg
    };

    /* Sanitize the optional input */
    if ((salt == NULL) && (salt_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = salt, .len = salt_length}
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    status = API_DISPATCH(tfm_crypto_asymmetric_encrypt,
                          TFM_CRYPTO_ASYMMETRIC_ENCRYPT);

    *output_length = out_vec[0].len;

    return status;
}

psa_status_t psa_asymmetric_decrypt(psa_key_handle_t handle,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_ASYMMETRIC_DECRYPT_SID,
        .key_handle = handle,
        .alg = alg
    };

    /* Sanitize the optional input */
    if ((salt == NULL) && (salt_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = input, .len = input_length},
        {.base = salt, .len = salt_length}
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    status = API_DISPATCH(tfm_crypto_asymmetric_decrypt,
                          TFM_CRYPTO_ASYMMETRIC_DECRYPT);

    *output_length = out_vec[0].len;

    return status;
}

psa_status_t psa_get_generator_capacity(const psa_crypto_generator_t *generator,
                                        size_t *capacity)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GET_GENERATOR_CAPACITY_SID,
        .op_handle = generator->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = capacity, .len = sizeof(size_t)},
    };

    status = API_DISPATCH(tfm_crypto_get_generator_capacity,
                          TFM_CRYPTO_GET_GENERATOR_CAPACITY);

    return status;
}

psa_status_t psa_generator_read(psa_crypto_generator_t *generator,
                                uint8_t *output,
                                size_t output_length)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATOR_READ_SID,
        .op_handle = generator->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_length},
    };

    status = API_DISPATCH(tfm_crypto_generator_read,
                          TFM_CRYPTO_GENERATOR_READ);

    return status;
}

psa_status_t psa_generator_import_key(psa_key_handle_t handle,
                                      psa_key_type_t type,
                                      size_t bits,
                                      psa_crypto_generator_t *generator)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATOR_IMPORT_KEY_SID,
        .key_handle = handle,
        .type = type,
        .op_handle = generator->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = &bits, .len = sizeof(size_t)},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_generator_import_key,
                                    TFM_CRYPTO_GENERATOR_IMPORT_KEY);

    return status;
}

psa_status_t psa_generator_abort(psa_crypto_generator_t *generator)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATOR_ABORT_SID,
        .op_handle = generator->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = &(generator->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_generator_abort,
                          TFM_CRYPTO_GENERATOR_ABORT);

    return status;
}

psa_status_t psa_key_derivation(psa_crypto_generator_t *generator,
                                psa_key_handle_t handle,
                                psa_algorithm_t alg,
                                const uint8_t *salt,
                                size_t salt_length,
                                const uint8_t *label,
                                size_t label_length,
                                size_t capacity)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_DERIVATION_SID,
        .key_handle = handle,
        .alg = alg,
        .op_handle = generator->handle,
        .capacity = capacity,
    };

    /* Sanitize the optional input */
    if ((salt == NULL) && (salt_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((label == NULL) && (label_length != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = salt, .len = salt_length},
        {.base = label, .len = label_length},
    };

    psa_outvec out_vec[] = {
        {.base = &(generator->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_key_derivation,
                          TFM_CRYPTO_KEY_DERIVATION);

    return status;
}

psa_status_t psa_key_agreement(psa_crypto_generator_t *generator,
                               psa_key_handle_t private_key,
                               const uint8_t *peer_key,
                               size_t peer_key_length,
                               psa_algorithm_t alg)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_KEY_AGREEMENT_SID,
        .key_handle = private_key,
        .alg = alg,
        .op_handle = generator->handle,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = peer_key, .len = peer_key_length},
    };

    psa_outvec out_vec[] = {
        {.base = &(generator->handle), .len = sizeof(uint32_t)},
    };

    status = API_DISPATCH(tfm_crypto_key_agreement,
                          TFM_CRYPTO_KEY_AGREEMENT);

    return status;
}

psa_status_t psa_generate_random(uint8_t *output,
                                 size_t output_size)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATE_RANDOM_SID,
    };

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
    };

    psa_outvec out_vec[] = {
        {.base = output, .len = output_size},
    };

    if (output_size == 0) {
        return PSA_SUCCESS;
    }

    status = API_DISPATCH(tfm_crypto_generate_random,
                          TFM_CRYPTO_GENERATE_RANDOM);

    return status;
}

psa_status_t psa_generate_key(psa_key_handle_t handle,
                              psa_key_type_t type,
                              size_t bits,
                              const void *extra,
                              size_t extra_size)
{
    psa_status_t status;
    struct tfm_crypto_pack_iovec iov = {
        .sfn_id = TFM_CRYPTO_GENERATE_KEY_SID,
        .key_handle = handle,
        .type = type,
    };

    /* Sanitize the optional input */
    if ((extra == NULL) && (extra_size != 0)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_crypto_pack_iovec)},
        {.base = &bits, .len = sizeof(size_t)},
        {.base = extra, .len = extra_size},
    };

    status = API_DISPATCH_NO_OUTVEC(tfm_crypto_generate_key,
                                    TFM_CRYPTO_GENERATE_KEY);

    return status;
}
