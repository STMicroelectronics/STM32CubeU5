/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

/* FixMe: Use PSA_ERROR_CONNECTION_REFUSED when performing parameter
 *        integrity checks but this will have to be revised
 *        when the full set of error codes mandated by PSA FF
 *        is available.
 */
#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"

/*!
 * \defgroup public_psa Public functions, PSA
 *
 */

/*!@{*/
psa_status_t tfm_crypto_get_generator_capacity(psa_invec in_vec[],
                                               size_t in_len,
                                               psa_outvec out_vec[],
                                               size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if ((in_len != 1) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(size_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    size_t *capacity = out_vec[0].base;
    psa_crypto_generator_t *generator = NULL;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_GENERATOR_OPERATION,
                                         handle,
                                         (void **)&generator);
    if (status != PSA_SUCCESS) {
        *capacity = 0;
        return status;
    }

    return psa_get_generator_capacity(generator, capacity);
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generator_read(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if ((in_len != 1) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    uint8_t *output = out_vec[0].base;
    size_t output_length = out_vec[0].len;
    psa_crypto_generator_t *generator = NULL;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_GENERATOR_OPERATION,
                                         handle,
                                         (void **)&generator);
    if (status != PSA_SUCCESS) {
        return status;
    }

    return psa_generator_read(generator, output, output_length);
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generator_import_key(psa_invec in_vec[],
                                             size_t in_len,
                                             psa_outvec out_vec[],
                                             size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if ((in_len != 2) || (out_len != 0)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (in_vec[1].len != sizeof(size_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    psa_key_handle_t key_handle = iov->key_handle;
    psa_key_type_t type = iov->type;
    size_t bits = *(size_t *)(in_vec[1].base);
    psa_crypto_generator_t *generator = NULL;

    status = tfm_crypto_check_handle_owner(key_handle, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_GENERATOR_OPERATION,
                                         handle,
                                         (void **)&generator);
    if (status != PSA_SUCCESS) {
        return status;
    }

    return psa_generator_import_key(key_handle, type, bits, generator);
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generator_abort(psa_invec in_vec[],
                                        size_t in_len,
                                        psa_outvec out_vec[],
                                        size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if ((in_len != 1) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    psa_crypto_generator_t *generator = NULL;

    /* Init the handle in the operation with the one passed from the iov */
    *handle_out = iov->op_handle;

    /* Look up the corresponding operation context */
    status = tfm_crypto_operation_lookup(TFM_CRYPTO_GENERATOR_OPERATION,
                                         handle,
                                         (void **)&generator);
    if (status != PSA_SUCCESS) {
        /* Operation does not exist, so abort has no effect */
        return PSA_SUCCESS;
    }

    *handle_out = handle;

    status = psa_generator_abort(generator);
    if (status != PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
        return status;
    }

    status = tfm_crypto_operation_release(handle_out);

    return status;
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_key_derivation(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if (!((in_len == 1) || (in_len == 2) || (in_len == 3)) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    psa_key_handle_t key_handle = iov->key_handle;
    psa_algorithm_t alg = iov->alg;
    const uint8_t *salt = NULL;
    size_t salt_length = 0;
    const uint8_t *label = NULL;
    size_t label_length = 0;
    size_t capacity = iov->capacity;
    psa_crypto_generator_t *generator = NULL;

    if (in_len > 1) {
        salt = in_vec[1].base;
        salt_length = in_vec[1].len;
    }

    if (in_len > 2) {
        label = in_vec[2].base;
        label_length = in_vec[2].len;
    }

    status = tfm_crypto_check_handle_owner(key_handle, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Allocate the generator context in the secure world */
    status = tfm_crypto_operation_alloc(TFM_CRYPTO_GENERATOR_OPERATION,
                                        &handle,
                                        (void **)&generator);
    if (status != PSA_SUCCESS) {
        return status;
    }

    *handle_out = handle;

    status = psa_key_derivation(generator, key_handle, alg, salt, salt_length,
                                label, label_length, capacity);
    if (status != PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
    }

    return status;
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_key_agreement(psa_invec in_vec[],
                                      size_t in_len,
                                      psa_outvec out_vec[],
                                      size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    if ((in_len != 2) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(uint32_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    uint32_t handle = iov->op_handle;
    uint32_t *handle_out = out_vec[0].base;
    psa_key_handle_t private_key = iov->key_handle;
    psa_algorithm_t alg = iov->alg;
    const uint8_t *peer_key = in_vec[1].base;
    size_t peer_key_length = in_vec[1].len;
    psa_crypto_generator_t *generator = NULL;

    status = tfm_crypto_check_handle_owner(private_key, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Allocate the generator context in the secure world */
    status = tfm_crypto_operation_alloc(TFM_CRYPTO_GENERATOR_OPERATION,
                                        &handle,
                                        (void **)&generator);
    if (status != PSA_SUCCESS) {
        return status;
    }

    *handle_out = handle;

    status = psa_key_agreement(generator, private_key,
                               peer_key, peer_key_length, alg);
    if (status != PSA_SUCCESS) {
        /* Release the operation context, ignore if the operation fails. */
        (void)tfm_crypto_operation_release(handle_out);
    }

    return status;
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generate_random(psa_invec in_vec[],
                                        size_t in_len,
                                        psa_outvec out_vec[],
                                        size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    if ((in_len != 1) || (out_len != 1)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    uint8_t *output = out_vec[0].base;
    size_t output_size = out_vec[0].len;

    return psa_generate_random(output, output_size);
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generate_key(psa_invec in_vec[],
                                     size_t in_len,
                                     psa_outvec out_vec[],
                                     size_t out_len)
{
#if (TFM_CRYPTO_GENERATOR_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else
    if (!((in_len == 2) || (in_len == 3)) || (out_len != 0)) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (in_vec[1].len != sizeof(size_t))) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_handle_t key_handle = iov->key_handle;
    psa_key_type_t type = iov->type;
    size_t bits = *((size_t *)(in_vec[1].base));
    const void *extra = NULL;
    size_t extra_size = 0;
    psa_status_t status;

    status = tfm_crypto_check_handle_owner(key_handle, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if (in_len == 3) {
        extra = in_vec[2].base;
        extra_size = in_vec[2].len;
    }

    return psa_generate_key(key_handle, type, bits, extra, extra_size);
#endif /* TFM_CRYPTO_GENERATOR_MODULE_DISABLED */
}
/*!@}*/
