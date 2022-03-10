/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <string.h>

#include "crypto_types.h"
#include "tfm_plat_crypto_keys.h"

/* FIXME: Functions in this file should be implemented by platform vendor. For
 * the security of the storage system, it is critical to use a hardware unique
 * key. For the security of the attestation, it is critical to use a unique key
 * pair and keep the private key is secret.
 */

#define TFM_KEY_LEN_BYTES  16

static const uint8_t sample_tfm_key[TFM_KEY_LEN_BYTES] =
             {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
              0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

#ifdef SYMMETRIC_INITIAL_ATTESTATION
extern const psa_algorithm_t tfm_attest_hmac_sign_alg;
extern const uint8_t initial_attestation_hmac_sha256_key[];
extern const size_t initial_attestation_hmac_sha256_key_size;
extern const char *initial_attestation_kid;
#else /* SYMMETRIC_INITIAL_ATTESTATION */
extern const psa_ecc_family_t initial_attestation_curve_type;
extern const uint8_t  initial_attestation_private_key[];
extern const uint32_t initial_attestation_private_key_size;
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

/**
 * \brief Copy the key to the destination buffer
 *
 * \param[out]  p_dst  Pointer to buffer where to store the key
 * \param[in]   p_src  Pointer to the key
 * \param[in]   size   Length of the key
 */
static inline void copy_key(uint8_t *p_dst, const uint8_t *p_src, size_t size)
{
    uint32_t i;

    for (i = size; i > 0; i--) {
        *p_dst = *p_src;
        p_src++;
        p_dst++;
    }
}

enum tfm_plat_err_t tfm_plat_get_huk_derived_key(const uint8_t *label,
                                                 size_t label_size,
                                                 const uint8_t *context,
                                                 size_t context_size,
                                                 uint8_t *key,
                                                 size_t key_size)
{
    (void)label;
    (void)label_size;
    (void)context;
    (void)context_size;

    if (key_size > TFM_KEY_LEN_BYTES) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* FIXME: Do key derivation */
    copy_key(key, sample_tfm_key, key_size);

    return TFM_PLAT_ERR_SUCCESS;
}

#ifdef SYMMETRIC_INITIAL_ATTESTATION
enum tfm_plat_err_t tfm_plat_get_symmetric_iak(uint8_t *key_buf,
                                               size_t buf_len,
                                               size_t *key_len,
                                               psa_algorithm_t *key_alg)
{
    if (!key_buf || !key_len || !key_alg) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (buf_len < initial_attestation_hmac_sha256_key_size) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    /*
     * Actual implementation may derive a key with other input, other than
     * directly providing provisioned symmetric initial attestation key.
     */
    copy_key(key_buf, initial_attestation_hmac_sha256_key,
             initial_attestation_hmac_sha256_key_size);

    *key_alg = tfm_attest_hmac_sign_alg;
    *key_len = initial_attestation_hmac_sha256_key_size;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_get_symmetric_iak_id(void *kid_buf,
                                                  size_t buf_len,
                                                  size_t *kid_len)
{
    /* kid is string in this example. '\0' is ignore. */
    size_t len = strlen(initial_attestation_kid);

    if (!kid_buf || !kid_len || (buf_len < len)) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    copy_key(kid_buf, (const uint8_t *)initial_attestation_kid, len);
    *kid_len = len;

    return TFM_PLAT_ERR_SUCCESS;
}
#else /* SYMMETRIC_INITIAL_ATTESTATION */
enum tfm_plat_err_t
tfm_plat_get_initial_attest_key(uint8_t          *key_buf,
                                uint32_t          size,
                                struct ecc_key_t *ecc_key,
                                psa_ecc_family_t *curve_type)
{
    uint8_t *key_dst;
    const uint8_t *key_src;
    uint32_t key_size;
    uint32_t full_key_size = initial_attestation_private_key_size;

    if (size < full_key_size) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Set the EC curve type which the key belongs to */
    *curve_type = initial_attestation_curve_type;

    /* Copy the private key to the buffer, it MUST be present */
    key_dst  = key_buf;
    key_src  = initial_attestation_private_key;
    key_size = initial_attestation_private_key_size;
    copy_key(key_dst, key_src, key_size);
    ecc_key->priv_key = key_dst;
    ecc_key->priv_key_size = key_size;

    ecc_key->pubx_key = NULL;
    ecc_key->pubx_key_size = 0;
    ecc_key->puby_key = NULL;
    ecc_key->puby_key_size = 0;

    return TFM_PLAT_ERR_SUCCESS;
}
#endif /* SYMMETRIC_INITIAL_ATTESTATION */
