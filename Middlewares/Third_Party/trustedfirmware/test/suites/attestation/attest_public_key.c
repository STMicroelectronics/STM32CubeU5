/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "attest_public_key.h"
#include "psa/initial_attestation.h"
#include "psa/crypto.h"
#include <stdint.h>

/*!
 * \def ECC_CURVE_SECP256R1_PULBIC_KEY_LENGTH
 *
 * \brief Calculates the size of ECC public key is bytes based on the bit size
 *        of the curve
 */
#define ECC_CURVE_SECP256R1_PULBIC_KEY_LENGTH (1 + 2 * PSA_BITS_TO_BYTES(256))

/*!
 *   Byte string representation of ECC public key according to
 *   psa_export_public_key() in interface/include/psa/crypto.h
 */
struct ecc_public_key_t {
    const uint8_t a;
    uint8_t public_key[]; /* X-coordinate || Y-coordinate */
};

/*!
 * Public key part of the example ECC key pair for initial attestation. It is
 * used to validate the signature of the attestation token.
 *
 * Note: This key must be in sync with the corresponding private part,
 *       otherwise attestation test cases will fail.
 *
 * Example key is stored here:
 *  - platform/ext/common/tfm_initial_attestation_key_material.c
 *  - platform/ext/common/tfm_initial_attestation_key.pem
 *
 * Example key type is: NIST P-256 also known as secp256r1
 */
static const struct ecc_public_key_t attest_public_key = {
     /* Constant byte */
     0x04,
     /* X-coordinate */
     {0x79, 0xEB, 0xA9, 0x0E, 0x8B, 0xF4, 0x50, 0xA6,
      0x75, 0x15, 0x76, 0xAD, 0x45, 0x99, 0xB0, 0x7A,
      0xDF, 0x93, 0x8D, 0xA3, 0xBB, 0x0B, 0xD1, 0x7D,
      0x00, 0x36, 0xED, 0x49, 0xA2, 0xD0, 0xFC, 0x3F,
     /* Y-coordinate */
      0xBF, 0xCD, 0xFA, 0x89, 0x56, 0xB5, 0x68, 0xBF,
      0xDB, 0x86, 0x73, 0xE6, 0x48, 0xD8, 0xB5, 0x8D,
      0x92, 0x99, 0x55, 0xB1, 0x4A, 0x26, 0xC3, 0x08,
      0x0F, 0x34, 0x11, 0x7D, 0x97, 0x1D, 0x68, 0x64},
};

/*!
 * \var ecc_curve
 *
 * \brief Key type of the example initial attestation key.
 *
 * Note: This must be updated if example key is replaced with other type.
 */
static psa_ecc_curve_t ecc_curve = PSA_ECC_CURVE_SECP256R1;

/*!
 * \var public_key_registered
 *
 * \brief Indicates whether the public part of the attestation key pair was
 *        registered to Crypto service or not.
 */
static uint32_t public_key_registered = 0;

enum psa_attest_err_t
attest_register_initial_attestation_public_key(psa_key_handle_t *public_key)
{
    psa_key_type_t attest_key_type;
    psa_status_t crypto_res;
    psa_key_policy_t policy = psa_key_policy_init();

    /* Public key should be unregistered at this point */
    if (public_key_registered != 0) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Allocate a transient key for the public key in the Crypto service */
    crypto_res = psa_allocate_key(public_key);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Setup the key policy for public key */
    psa_key_policy_set_usage(&policy,
                             PSA_KEY_USAGE_VERIFY,
                             PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    crypto_res = psa_set_key_policy(*public_key, &policy);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    /* Set key type for public key */
    attest_key_type = PSA_KEY_TYPE_ECC_PUBLIC_KEY(ecc_curve);

    /* Register public key to Crypto service */
    crypto_res = psa_import_key(*public_key,
                                attest_key_type,
                                (const uint8_t *)&attest_public_key,
                                ECC_CURVE_SECP256R1_PULBIC_KEY_LENGTH);

    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }
    public_key_registered = 1;

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_unregister_initial_attestation_public_key(psa_key_handle_t public_key)
{
    psa_status_t crypto_res;

    if (public_key_registered != 1) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    crypto_res = psa_destroy_key(public_key);
    if (crypto_res != PSA_SUCCESS) {
        return PSA_ATTEST_ERR_GENERAL;
    }
    public_key_registered = 0;

    return PSA_ATTEST_ERR_SUCCESS;
}
