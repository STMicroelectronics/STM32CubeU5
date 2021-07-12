/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file crypto_spe.h
 *
 * \brief When Mbed Crypto is built with the MBEDTLS_PSA_CRYPTO_SPM option
 *        enabled, this header is included by all .c files in Mbed Crypto that
 *        use PSA Crypto function names. This avoids duplication of symbols
 *        between TF-M and Mbed Crypto.
 *
 * \note  This file should be included before including any PSA Crypto headers
 *        from Mbed Crypto.
 */

#ifndef CRYPTO_SPE_H
#define CRYPTO_SPE_H

#define PSA_FUNCTION_NAME(x) mbedcrypto__ ## x

#define psa_crypto_init \
        PSA_FUNCTION_NAME(psa_crypto_init)
#define psa_key_policy_init \
        PSA_FUNCTION_NAME(psa_key_policy_init)
#define psa_key_policy_set_usage \
        PSA_FUNCTION_NAME(psa_key_policy_set_usage)
#define psa_key_policy_get_usage \
        PSA_FUNCTION_NAME(psa_key_policy_get_usage)
#define psa_key_policy_get_algorithm \
        PSA_FUNCTION_NAME(psa_key_policy_get_algorithm)
#define psa_set_key_policy \
        PSA_FUNCTION_NAME(psa_set_key_policy)
#define psa_get_key_policy \
        PSA_FUNCTION_NAME(psa_get_key_policy)
#define psa_get_key_lifetime \
        PSA_FUNCTION_NAME(psa_get_key_lifetime)
#define psa_allocate_key \
        PSA_FUNCTION_NAME(psa_allocate_key)
#define psa_open_key \
        PSA_FUNCTION_NAME(psa_open_key)
#define psa_create_key \
        PSA_FUNCTION_NAME(psa_create_key)
#define psa_close_key \
        PSA_FUNCTION_NAME(psa_close_key)
#define psa_import_key \
        PSA_FUNCTION_NAME(psa_import_key)
#define psa_destroy_key \
        PSA_FUNCTION_NAME(psa_destroy_key)
#define psa_get_key_information \
        PSA_FUNCTION_NAME(psa_get_key_information)
#define psa_export_key \
        PSA_FUNCTION_NAME(psa_export_key)
#define psa_export_public_key \
        PSA_FUNCTION_NAME(psa_export_public_key)
#define psa_copy_key \
        PSA_FUNCTION_NAME(psa_copy_key)
#define psa_hash_operation_init \
        PSA_FUNCTION_NAME(psa_hash_operation_init)
#define psa_hash_setup \
        PSA_FUNCTION_NAME(psa_hash_setup)
#define psa_hash_update \
        PSA_FUNCTION_NAME(psa_hash_update)
#define psa_hash_finish \
        PSA_FUNCTION_NAME(psa_hash_finish)
#define psa_hash_verify \
        PSA_FUNCTION_NAME(psa_hash_verify)
#define psa_hash_abort \
        PSA_FUNCTION_NAME(psa_hash_abort)
#define psa_hash_clone \
        PSA_FUNCTION_NAME(psa_hash_clone)
#define psa_mac_operation_init \
        PSA_FUNCTION_NAME(psa_mac_operation_init)
#define psa_mac_sign_setup \
        PSA_FUNCTION_NAME(psa_mac_sign_setup)
#define psa_mac_verify_setup \
        PSA_FUNCTION_NAME(psa_mac_verify_setup)
#define psa_mac_update \
        PSA_FUNCTION_NAME(psa_mac_update)
#define psa_mac_sign_finish \
        PSA_FUNCTION_NAME(psa_mac_sign_finish)
#define psa_mac_verify_finish \
        PSA_FUNCTION_NAME(psa_mac_verify_finish)
#define psa_mac_abort \
        PSA_FUNCTION_NAME(psa_mac_abort)
#define psa_cipher_operation_init \
        PSA_FUNCTION_NAME(psa_cipher_operation_init)
#define psa_cipher_encrypt_setup \
        PSA_FUNCTION_NAME(psa_cipher_encrypt_setup)
#define psa_cipher_decrypt_setup \
        PSA_FUNCTION_NAME(psa_cipher_decrypt_setup)
#define psa_cipher_generate_iv \
        PSA_FUNCTION_NAME(psa_cipher_generate_iv)
#define psa_cipher_set_iv \
        PSA_FUNCTION_NAME(psa_cipher_set_iv)
#define psa_cipher_update \
        PSA_FUNCTION_NAME(psa_cipher_update)
#define psa_cipher_finish \
        PSA_FUNCTION_NAME(psa_cipher_finish)
#define psa_cipher_abort \
        PSA_FUNCTION_NAME(psa_cipher_abort)
#define psa_aead_encrypt \
        PSA_FUNCTION_NAME(psa_aead_encrypt)
#define psa_aead_decrypt \
        PSA_FUNCTION_NAME(psa_aead_decrypt)
#define psa_asymmetric_sign \
        PSA_FUNCTION_NAME(psa_asymmetric_sign)
#define psa_asymmetric_verify \
        PSA_FUNCTION_NAME(psa_asymmetric_verify)
#define psa_asymmetric_encrypt \
        PSA_FUNCTION_NAME(psa_asymmetric_encrypt)
#define psa_asymmetric_decrypt \
        PSA_FUNCTION_NAME(psa_asymmetric_decrypt)
#define psa_crypto_generator_init \
        PSA_FUNCTION_NAME(psa_crypto_generator_init)
#define psa_get_generator_capacity \
        PSA_FUNCTION_NAME(psa_get_generator_capacity)
#define psa_generator_read \
        PSA_FUNCTION_NAME(psa_generator_read)
#define psa_generator_import_key \
        PSA_FUNCTION_NAME(psa_generator_import_key)
#define psa_generator_abort \
        PSA_FUNCTION_NAME(psa_generator_abort)
#define psa_key_derivation \
        PSA_FUNCTION_NAME(psa_key_derivation)
#define psa_key_agreement \
        PSA_FUNCTION_NAME(psa_key_agreement)
#define psa_generate_random \
        PSA_FUNCTION_NAME(psa_generate_random)
#define psa_generate_key \
        PSA_FUNCTION_NAME(psa_generate_key)

#endif /* CRYPTO_SPE_H */
