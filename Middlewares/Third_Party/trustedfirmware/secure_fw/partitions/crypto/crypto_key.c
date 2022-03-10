/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "tfm_mbedcrypto_include.h"

#include "psa_manifest/pid.h"
#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"
#include "tfm_crypto_private.h"
#include <stdbool.h>

#ifndef TFM_CRYPTO_MAX_KEY_HANDLES
#define TFM_CRYPTO_MAX_KEY_HANDLES (32)
#endif
struct tfm_crypto_handle_owner_s {
    int32_t owner;           /*!< Owner of the allocated handle */
    psa_key_id_t key;        /*!< Allocated key */
    uint8_t in_use;          /*!< Flag to indicate if this in use */
};

#ifndef TFM_CRYPTO_KEY_MODULE_DISABLED
static struct tfm_crypto_handle_owner_s
                                 handle_owner[TFM_CRYPTO_MAX_KEY_HANDLES] = {0};
#endif

/*!
 * \defgroup public Public functions
 *
 */
/*!@{*/
psa_status_t tfm_crypto_key_attributes_from_client(
                    const struct psa_client_key_attributes_s *client_key_attr,
                    int32_t client_id,
                    psa_key_attributes_t *key_attributes)
{
    if (client_key_attr == NULL || key_attributes == NULL) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    *key_attributes = psa_key_attributes_init();

    /* Copy core key attributes from the client core key attributes */
    key_attributes->core.type = client_key_attr->type;
    key_attributes->core.lifetime = client_key_attr->lifetime;
    key_attributes->core.policy.usage = client_key_attr->usage;
    key_attributes->core.policy.alg = client_key_attr->alg;
    key_attributes->core.bits = client_key_attr->bits;

    /* Use the client key id as the key_id and its partition id as the owner */
    key_attributes->core.id.key_id = client_key_attr->id;
    key_attributes->core.id.owner = client_id;

    return PSA_SUCCESS;
}

psa_status_t tfm_crypto_key_attributes_to_client(
                        const psa_key_attributes_t *key_attributes,
                        struct psa_client_key_attributes_s *client_key_attr)
{
    if (client_key_attr == NULL || key_attributes == NULL) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    struct psa_client_key_attributes_s v = PSA_CLIENT_KEY_ATTRIBUTES_INIT;
    *client_key_attr = v;

    /* Copy core key attributes from the client core key attributes */
    client_key_attr->type = key_attributes->core.type;
    client_key_attr->lifetime = key_attributes->core.lifetime;
    client_key_attr->usage = key_attributes->core.policy.usage;
    client_key_attr->alg = key_attributes->core.policy.alg;
    client_key_attr->bits = key_attributes->core.bits;

    /* Return the key_id as the client key id, do not return the owner */
    client_key_attr->id = key_attributes->core.id.key_id;

    return PSA_SUCCESS;
}

psa_status_t tfm_crypto_check_handle_owner(psa_key_id_t key,
                                           uint32_t *index)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int32_t partition_id = 0;
    uint32_t i = 0;
    psa_status_t status;

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use && handle_owner[i].key == key) {
            if (handle_owner[i].owner == partition_id) {
                if (index != NULL) {
                    *index = i;
                }
                return PSA_SUCCESS;
            } else {
                return PSA_ERROR_NOT_PERMITTED;
            }
        }
    }

    return PSA_ERROR_INVALID_HANDLE;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_encode_id_and_owner(psa_key_id_t key_id,
                                            mbedtls_svc_key_id_t *enc_key_ptr)
{
    int32_t partition_id = 0;
    psa_status_t status = tfm_crypto_get_caller_id(&partition_id);

    if (status != PSA_SUCCESS) {
        return status;
    }

    /* If Null Pointer, return PSA_ERROR_PROGRAMMER_ERROR */
    if (enc_key_ptr == NULL) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* Use the client key id as the key_id and its partition id as the owner */
    *enc_key_ptr = mbedtls_svc_key_id_make(partition_id, key_id);

    return PSA_SUCCESS;
}

psa_status_t tfm_crypto_check_key_storage(uint32_t *index)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint32_t i;

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            *index = i;
            return PSA_SUCCESS;
        }
    }

    return PSA_ERROR_INSUFFICIENT_MEMORY;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_set_key_storage(uint32_t index,
                                        psa_key_id_t key_handle)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    psa_status_t status;
    int32_t partition_id;

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    handle_owner[index].owner = partition_id;
    handle_owner[index].key = key_handle;
    handle_owner[index].in_use = TFM_CRYPTO_IN_USE;

    return PSA_SUCCESS;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_set_key_domain_parameters(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    /* FixMe: To be implemented */
    return PSA_ERROR_NOT_SUPPORTED;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_get_key_domain_parameters(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    /* FixMe: To be implemented */
    return PSA_ERROR_NOT_SUPPORTED;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_import_key(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 2, 3, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (in_vec[1].len != sizeof(struct psa_client_key_attributes_s)) ||
        (out_vec[0].len != sizeof(psa_key_id_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct psa_client_key_attributes_s *client_key_attr = in_vec[1].base;
    const uint8_t *data = in_vec[2].base;
    size_t data_length = in_vec[2].len;
    psa_key_id_t *psa_key = out_vec[0].base;

    psa_status_t status;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    uint32_t i = 0;
    mbedtls_svc_key_id_t encoded_key;
    int32_t partition_id = 0;
    bool empty_found = false;

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            empty_found = true;
            break;
        }
    }

    if (!empty_found) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

#if defined(USE_HUK)
    if ((data_length == 0) && (data == NULL))
    {
        if (handle_owner[i].owner != TFM_SP_PS) {
            return PSA_ERROR_NOT_PERMITTED;
        }
    }
#endif
    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                   partition_id,
                                                   &key_attributes);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_import_key(&key_attributes, data, data_length, &encoded_key);
    /* Update the imported key id */
    *psa_key = encoded_key.key_id;

    if (status == PSA_SUCCESS) {
        handle_owner[i].owner = partition_id;
        handle_owner[i].key = *psa_key;
        handle_owner[i].in_use = TFM_CRYPTO_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_open_key(psa_invec in_vec[],
                                 size_t in_len,
                                 psa_outvec out_vec[],
                                 size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 2, 2, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (in_vec[1].len != sizeof(psa_key_id_t)) ||
        (out_vec[0].len != sizeof(psa_key_id_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    psa_key_id_t client_key_id = *((psa_key_id_t *)in_vec[1].base);
    psa_key_id_t *key = out_vec[0].base;
    psa_status_t status;
    mbedtls_svc_key_id_t encoded_key;
    int32_t partition_id;
    uint32_t i;

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            break;
        }
    }

    if (i == TFM_CRYPTO_MAX_KEY_HANDLES) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Use the client key id as the key_id and its partition id as the owner */
    encoded_key = mbedtls_svc_key_id_make(partition_id, client_key_id);

    status = psa_open_key(encoded_key, &encoded_key);
    *key = encoded_key.key_id;

    if (status == PSA_SUCCESS) {
        handle_owner[i].owner = partition_id;
        handle_owner[i].key = *key;
        handle_owner[i].in_use = TFM_CRYPTO_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_close_key(psa_invec in_vec[],
                                  size_t in_len,
                                  psa_outvec out_vec[],
                                  size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    (void)out_vec;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 0, 0);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    psa_key_id_t key = iov->key_id;
    uint32_t index;
    mbedtls_svc_key_id_t encoded_key;

    psa_status_t status = tfm_crypto_check_handle_owner(key, &index);
    if (status != PSA_SUCCESS) {
        return status;
    }

    encoded_key = mbedtls_svc_key_id_make(handle_owner[index].owner, key);
    status = psa_close_key(encoded_key);

    if (status == PSA_SUCCESS) {
        handle_owner[index].owner = 0;
        handle_owner[index].key = 0;
        handle_owner[index].in_use = TFM_CRYPTO_NOT_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_destroy_key(psa_invec in_vec[],
                                    size_t in_len,
                                    psa_outvec out_vec[],
                                    size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    (void)out_vec;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 0, 0);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_id_t key = iov->key_id;
    uint32_t index;
    mbedtls_svc_key_id_t encoded_key;

    psa_status_t status = tfm_crypto_check_handle_owner(key, &index);
    if (status != PSA_SUCCESS) {
        return status;
    }

    encoded_key = mbedtls_svc_key_id_make(handle_owner[index].owner, key);

    status = psa_destroy_key(encoded_key);
    if (status == PSA_SUCCESS) {
        handle_owner[index].owner = 0;
        handle_owner[index].key = 0;
        handle_owner[index].in_use = TFM_CRYPTO_NOT_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_get_key_attributes(psa_invec in_vec[],
                                           size_t in_len,
                                           psa_outvec out_vec[],
                                           size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(struct psa_client_key_attributes_s))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    psa_key_id_t key = iov->key_id;
    struct psa_client_key_attributes_s *client_key_attr = out_vec[0].base;
    psa_status_t status;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    mbedtls_svc_key_id_t encoded_key;

    status = tfm_crypto_check_handle_owner(key, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_encode_id_and_owner(key, &encoded_key);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_get_key_attributes(encoded_key, &key_attributes);
    if (status == PSA_SUCCESS) {
        status = tfm_crypto_key_attributes_to_client(&key_attributes,
                                                     client_key_attr);
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_reset_key_attributes(psa_invec in_vec[],
                                             size_t in_len,
                                             psa_outvec out_vec[],
                                             size_t out_len)
{
#if (TFM_CRYPTO_KEY_MODULE_DISABLED != 0)
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(struct psa_client_key_attributes_s))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    struct psa_client_key_attributes_s *client_key_attr = out_vec[0].base;
    psa_status_t status;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    int32_t partition_id;

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                   partition_id,
                                                   &key_attributes);
    if (status != PSA_SUCCESS) {
        return status;
    }

    psa_reset_key_attributes(&key_attributes);

    return tfm_crypto_key_attributes_to_client(&key_attributes,
                                               client_key_attr);
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_export_key(psa_invec in_vec[],
                                   size_t in_len,
                                   psa_outvec out_vec[],
                                   size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 0, 1);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    psa_key_id_t key = iov->key_id;
    uint8_t *data = out_vec[0].base;
    size_t data_size = out_vec[0].len;
    mbedtls_svc_key_id_t encoded_key;
    uint32_t index;

    psa_status_t status = tfm_crypto_check_handle_owner(key, &index);

    if (status != PSA_SUCCESS) {
        return status;
    }

    encoded_key = mbedtls_svc_key_id_make(handle_owner[index].owner, key);
    return psa_export_key(encoded_key, data, data_size,
                          &(out_vec[0].len));
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_export_public_key(psa_invec in_vec[],
                                          size_t in_len,
                                          psa_outvec out_vec[],
                                          size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 0, 1);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_id_t key = iov->key_id;
    uint8_t *data = out_vec[0].base;
    size_t data_size = out_vec[0].len;
    mbedtls_svc_key_id_t encoded_key;
    uint32_t index;

    psa_status_t status = tfm_crypto_check_handle_owner(key, &index);

    if (status != PSA_SUCCESS) {
        return status;
    }

    encoded_key = mbedtls_svc_key_id_make(handle_owner[index].owner, key);

    return psa_export_public_key(encoded_key, data, data_size,
                                 &(out_vec[0].len));
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_purge_key(psa_invec in_vec[],
                                  size_t in_len,
                                  psa_outvec out_vec[],
                                  size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else
    (void)out_vec;

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 1, 1, out_len, 0, 0);

    if (in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_key_id_t key = iov->key_id;
    uint32_t index;
    mbedtls_svc_key_id_t encoded_key;

    psa_status_t status = tfm_crypto_check_handle_owner(key, &index);

    if (status != PSA_SUCCESS) {
        return status;
    }

    encoded_key = mbedtls_svc_key_id_make(handle_owner[index].owner, key);

    status = psa_purge_key(encoded_key);
    if (status == PSA_SUCCESS) {
        handle_owner[index].owner = 0;
        handle_owner[index].key = 0;
        handle_owner[index].in_use = TFM_CRYPTO_NOT_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_copy_key(psa_invec in_vec[],
                                 size_t in_len,
                                 psa_outvec out_vec[],
                                 size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 2, 2, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (out_vec[0].len != sizeof(psa_key_id_t)) ||
        (in_vec[1].len != sizeof(struct psa_client_key_attributes_s))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;

    psa_key_id_t source_key_id = iov->key_id;
    psa_key_id_t *target_key_id = out_vec[0].base;
    const struct psa_client_key_attributes_s *client_key_attr = in_vec[1].base;
    psa_status_t status;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    uint32_t i = 0;
    int32_t partition_id = 0;
    bool empty_found = false;
    mbedtls_svc_key_id_t target_key;
    mbedtls_svc_key_id_t encoded_key;

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            empty_found = true;
            break;
        }
    }

    if (!empty_found) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                   partition_id,
                                                   &key_attributes);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_check_handle_owner(source_key_id, NULL);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_encode_id_and_owner(source_key_id, &encoded_key);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_copy_key(encoded_key, &key_attributes, &target_key);
    *target_key_id = target_key.key_id;
    if (status == PSA_SUCCESS) {
        handle_owner[i].owner = partition_id;
        handle_owner[i].key = *target_key_id;
        handle_owner[i].in_use = TFM_CRYPTO_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}

psa_status_t tfm_crypto_generate_key(psa_invec in_vec[],
                                     size_t in_len,
                                     psa_outvec out_vec[],
                                     size_t out_len)
{
#ifdef TFM_CRYPTO_KEY_MODULE_DISABLED
    return PSA_ERROR_NOT_SUPPORTED;
#else

    CRYPTO_IN_OUT_LEN_VALIDATE(in_len, 2, 2, out_len, 1, 1);

    if ((in_vec[0].len != sizeof(struct tfm_crypto_pack_iovec)) ||
        (in_vec[1].len != sizeof(struct psa_client_key_attributes_s)) ||
        (out_vec[0].len != sizeof(psa_key_id_t))) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    psa_key_id_t *key_handle = out_vec[0].base;
    const struct psa_client_key_attributes_s *client_key_attr = in_vec[1].base;
    psa_status_t status;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    uint32_t i = 0;
    int32_t partition_id = 0;
    bool empty_found = false;
    mbedtls_svc_key_id_t encoded_key;

    for (i = 0; i < TFM_CRYPTO_MAX_KEY_HANDLES; i++) {
        if (handle_owner[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            empty_found = true;
            break;
        }
    }

    if (!empty_found) {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                   partition_id,
                                                   &key_attributes);
    if (status != PSA_SUCCESS) {
        return status;
    }

    status = psa_generate_key(&key_attributes, &encoded_key);
    *key_handle = encoded_key.key_id;

    if (status == PSA_SUCCESS) {
        handle_owner[i].owner = partition_id;
        handle_owner[i].key = *key_handle;
        handle_owner[i].in_use = TFM_CRYPTO_IN_USE;
    }

    return status;
#endif /* TFM_CRYPTO_KEY_MODULE_DISABLED */
}
/*!@}*/
