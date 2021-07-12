/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_API_H__
#define __TFM_CRYPTO_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_api.h"
#include "tfm_crypto_defs.h"
#include "psa/crypto.h"
#ifdef TFM_PSA_API
#include "psa/service.h"

/**
 * \brief This define is a function pointer type to the Uniform Signature API
 *        prototype.
 */
typedef psa_status_t (*tfm_crypto_us_t)(psa_invec[],size_t,psa_outvec[],size_t);
#endif

#define UNIFORM_SIGNATURE_API(api_name) \
    psa_status_t api_name(psa_invec[], size_t, psa_outvec[], size_t)

/**
 * \brief List of possible operation types supported by the TFM based
 *        implementation. This type is needed by the operation allocation,
 *        lookup and release functions.
 *
 */
enum tfm_crypto_operation_type {
    TFM_CRYPTO_OPERATION_NONE = 0,
    TFM_CRYPTO_CIPHER_OPERATION = 1,
    TFM_CRYPTO_MAC_OPERATION = 2,
    TFM_CRYPTO_HASH_OPERATION = 3,
    TFM_CRYPTO_GENERATOR_OPERATION = 4,

    /* Used to force the enum size */
    TFM_CRYPTO_OPERATION_TYPE_MAX = INT_MAX
};

/**
 * \brief Initialise the service
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_init(void);

/**
 * \brief Initialise the Alloc module
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_init_alloc(void);

/**
 * \brief Returns the ID of the caller
 *
 * \param[out] id Pointer to hold the ID of the caller
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_get_caller_id(int32_t *id);

/**
 * \brief Checks that the requested handle belongs to the requesting
 *        partition
 *
 * \param[in]  handle Handle given as input
 * \param[out] index  Optionally, pointer to hold the internal index
 *                    corresponding to the input handle. Valid only
 *                    on PSA_SUCCESS, it's returned only if the input
 *                    parameter is not NULL.
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_check_handle_owner(psa_key_handle_t handle,
                                           uint32_t *index);

#if defined(USE_HUK)
/**
 * \brief Checks that the requested handle can be used on HUK
 *
 * \param[in]  handle       Handle given as input
 * \param[in]  data         Buffer containing the key data.
 * \param[in]  data_length  Size of the \p data buffer in bytes.
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_check_handle_usage(psa_key_handle_t handle,
                                           const uint8_t *data,
                                           const size_t data_length);
#endif /* USE_HUK */
/**
 * \brief Allocate an operation context in the backend
 *
 * \param[in]  type   Type of the operation context to allocate
 * \param[out] handle Pointer to hold the allocated handle
 * \param[out  ctx    Double pointer to the corresponding context
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_alloc(enum tfm_crypto_operation_type type,
                                        uint32_t *handle,
                                        void **ctx);
/**
 * \brief Release an operation context in the backend
 *
 * \param[in] handle Pointer to the handle of the context to release
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_release(uint32_t *handle);
/**
 * \brief Look up an operation context in the backend for the corresponding
 *        frontend operation
 *
 * \param[in]  type   Type of the operation context to look up
 * \param[in]  handle Handle of the context to lookup
 * \param[out] ctx    Double pointer to the corresponding context
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_lookup(enum tfm_crypto_operation_type type,
                                         uint32_t handle,
                                         void **ctx);

#define LIST_TFM_CRYPTO_UNIFORM_SIGNATURE_API \
    X(tfm_crypto_allocate_key)                \
    X(tfm_crypto_import_key)                  \
    X(tfm_crypto_destroy_key)                 \
    X(tfm_crypto_get_key_information)         \
    X(tfm_crypto_export_key)                  \
    X(tfm_crypto_export_public_key)           \
    X(tfm_crypto_copy_key)                    \
    X(tfm_crypto_set_key_policy)              \
    X(tfm_crypto_get_key_policy)              \
    X(tfm_crypto_get_key_lifetime)            \
    X(tfm_crypto_cipher_generate_iv)          \
    X(tfm_crypto_cipher_set_iv)               \
    X(tfm_crypto_cipher_encrypt_setup)        \
    X(tfm_crypto_cipher_decrypt_setup)        \
    X(tfm_crypto_cipher_update)               \
    X(tfm_crypto_cipher_abort)                \
    X(tfm_crypto_cipher_finish)               \
    X(tfm_crypto_hash_setup)                  \
    X(tfm_crypto_hash_update)                 \
    X(tfm_crypto_hash_finish)                 \
    X(tfm_crypto_hash_verify)                 \
    X(tfm_crypto_hash_abort)                  \
    X(tfm_crypto_hash_clone)                  \
    X(tfm_crypto_mac_sign_setup)              \
    X(tfm_crypto_mac_verify_setup)            \
    X(tfm_crypto_mac_update)                  \
    X(tfm_crypto_mac_sign_finish)             \
    X(tfm_crypto_mac_verify_finish)           \
    X(tfm_crypto_mac_abort)                   \
    X(tfm_crypto_aead_encrypt)                \
    X(tfm_crypto_aead_decrypt)                \
    X(tfm_crypto_asymmetric_sign)             \
    X(tfm_crypto_asymmetric_verify)           \
    X(tfm_crypto_asymmetric_encrypt)          \
    X(tfm_crypto_asymmetric_decrypt)          \
    X(tfm_crypto_get_generator_capacity)      \
    X(tfm_crypto_generator_read)              \
    X(tfm_crypto_generator_import_key)        \
    X(tfm_crypto_generator_abort)             \
    X(tfm_crypto_key_derivation)              \
    X(tfm_crypto_key_agreement)               \
    X(tfm_crypto_generate_random)             \
    X(tfm_crypto_generate_key)                \

#define X(api_name) UNIFORM_SIGNATURE_API(api_name);
LIST_TFM_CRYPTO_UNIFORM_SIGNATURE_API
#undef X

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_API_H__ */
