/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif
#include "crypto_tests_common.h"

void psa_key_interface_test(const psa_key_type_t key_type,
                            struct test_result_t *ret)
{
    psa_status_t status = PSA_SUCCESS;
    uint32_t i = 0;
    psa_key_handle_t key_handle = 0x0u;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t exported_data[sizeof(data)] = {0};
    size_t exported_data_size = 0;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_EXPORT);
    psa_set_key_type(&key_attributes, key_type);

    status = psa_import_key(&key_attributes, data, sizeof(data),
                            &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        return;
    }

    if (psa_get_key_bits(&retrieved_attributes) != BIT_SIZE_TEST_KEY) {
        TEST_FAIL("The number of key bits is different from expected");
        return;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        return;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    status = psa_export_key(key_handle,
                            exported_data,
                            sizeof(data),
                            &exported_data_size);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error exporting a key");
        return;
    }

    if (exported_data_size != BYTE_SIZE_TEST_KEY) {
        TEST_FAIL("Number of bytes of exported key different from expected");
        return;
    }

    /* Check that the exported key is the same as the imported one */
    for (i=0; i<exported_data_size; i++) {
        if (exported_data[i] != data[i]) {
            TEST_FAIL("Exported key doesn't match the imported key");
            return;
        }
    }

    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_ERROR_INVALID_HANDLE) {
        TEST_FAIL("Key handle should be invalid now");
        return;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    ret->val = TEST_PASSED;
}

void psa_cipher_test(const psa_key_type_t key_type,
                     const psa_algorithm_t alg,
                     struct test_result_t *ret)
{
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_cipher_operation_t handle_dec = psa_cipher_operation_init();
    psa_status_t status = PSA_SUCCESS;
    psa_key_handle_t key_handle;
    const uint8_t data[] = "THIS IS MY KEY1";
    const size_t iv_length = PSA_BLOCK_CIPHER_BLOCK_SIZE(key_type);
    const uint8_t iv[] = "012345678901234";
    const uint8_t plain_text[BYTE_SIZE_CHUNK] = "Sixteen bytes!!";
    uint8_t decrypted_data[ENC_DEC_BUFFER_SIZE] = {0};
    size_t output_length = 0, total_output_length = 0;
    uint8_t encrypted_data[ENC_DEC_BUFFER_SIZE] = {0};
    uint32_t comp_result;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    uint32_t i;

    ret->val = TEST_PASSED;

    /* FIXME: Special override for the CC312 accelerator. Implemented because
     * there is not yet a generic way to override tests.
     */
#ifdef CRYPTO_HW_ACCELERATOR_CC312
    if (alg == PSA_ALG_CFB) {
        TEST_LOG("%s %s", "The CC312 does not support CFB mode.",
                 "The test execution was SKIPPED.\r\n");
        return;
    }
#endif /* CRYPTO_HW_ACCELERATOR_CC312 */

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import a key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        goto destroy_key;
    }

    status = psa_get_key_attributes(key_handle, &key_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key;
    }

    if (psa_get_key_bits(&key_attributes) != BIT_SIZE_TEST_KEY) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key;
    }

    if (psa_get_key_type(&key_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key;
    }

    psa_reset_key_attributes(&key_attributes);

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
        } else {
            TEST_FAIL("Error setting up cipher operation object");
        }
        goto destroy_key;
    }

    /* Set the IV */
    status = psa_cipher_set_iv(&handle, iv, iv_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV on the cypher operation object");
        status = psa_cipher_abort(&handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    /* Encrypt one chunk of information */
    status = psa_cipher_update(&handle, plain_text, BYTE_SIZE_CHUNK,
                               encrypted_data, ENC_DEC_BUFFER_SIZE,
                               &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error encrypting one chunk of information");
        status = psa_cipher_abort(&handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    if (output_length != BYTE_SIZE_CHUNK) {
        TEST_FAIL("Expected encrypted data length is different from expected");
        status = psa_cipher_abort(&handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    /* Finalise the cipher operation */
    status = psa_cipher_finish(&handle, &encrypted_data[output_length],
                               ENC_DEC_BUFFER_SIZE - output_length,
                               &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        status = psa_cipher_abort(&handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    if (output_length != 0) {
        TEST_FAIL("Unexpected output length after finalisation");
        goto destroy_key;
    }

    /* Setup the decryption object */
    if (alg == PSA_ALG_CFB) {
        /* In CFB mode the object is always in encryption mode */
        status = psa_cipher_encrypt_setup(&handle_dec, key_handle, alg);
    } else {
        status = psa_cipher_decrypt_setup(&handle_dec, key_handle, alg);
    }

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up cipher operation object");
        goto destroy_key;
    }

    /* Set the IV for decryption */
    status = psa_cipher_set_iv(&handle_dec, iv, iv_length);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting the IV for decryption");
        status = psa_cipher_abort(&handle_dec);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    /* Decrypt */
    for (i = 0; i < ENC_DEC_BUFFER_SIZE; i += BYTE_SIZE_CHUNK) {
        status = psa_cipher_update(&handle_dec,
                                   (encrypted_data + i), BYTE_SIZE_CHUNK,
                                   (decrypted_data + total_output_length),
                                   (ENC_DEC_BUFFER_SIZE - total_output_length),
                                   &output_length);

        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error during decryption");
            status = psa_cipher_abort(&handle_dec);
            if (status != PSA_SUCCESS) {
                TEST_FAIL("Error aborting the operation");
            }
            goto destroy_key;
        }

        total_output_length += output_length;
    }

#if DOMAIN_NS == 1U
    /* Check that the plain text matches the decrypted data */
    comp_result = memcmp(plain_text, decrypted_data, sizeof(plain_text));
#else
    comp_result = tfm_memcmp(plain_text, decrypted_data, sizeof(plain_text));
#endif
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        status = psa_cipher_abort(&handle_dec);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    /* Finalise the cipher operation for decryption (destroys decrypted data) */
    status = psa_cipher_finish(&handle_dec, decrypted_data, BYTE_SIZE_CHUNK,
                               &output_length);

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error finalising the cipher operation");
        status = psa_cipher_abort(&handle_dec);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error aborting the operation");
        }
        goto destroy_key;
    }

    total_output_length += output_length;

    /* Check that the decrypted length is equal to the original length */
    if (total_output_length != ENC_DEC_BUFFER_SIZE) {
        TEST_FAIL("After finalising, unexpected decrypted length");
        goto destroy_key;
    }

destroy_key:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }

}

void psa_invalid_cipher_test(const psa_key_type_t key_type,
                             const psa_algorithm_t alg,
                             const size_t key_size,
                             struct test_result_t *ret)
{
    psa_status_t status;
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_key_handle_t key_handle;
    uint8_t data[TEST_MAX_KEY_LENGTH];
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

#if DOMAIN_NS == 1U
    /* Fill the key data */
    (void)memset(data, 'A', key_size);
#else
    (void)tfm_memset(data, 'A', key_size);
#endif

    /* Import a key */
    status = psa_import_key(&key_attributes, data, key_size, &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the encryption object */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Should not successfully setup an invalid cipher");
        (void)psa_destroy_key(key_handle);
        return;
    }

    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_unsupported_hash_test(const psa_algorithm_t alg,
                               struct test_result_t *ret)
{
    psa_status_t status;
    psa_hash_operation_t handle = PSA_HASH_OPERATION_INIT;

    /* Setup the hash object for the unsupported hash algorithm */
    status = psa_hash_setup(&handle, alg);
    if (status != PSA_ERROR_NOT_SUPPORTED) {
        TEST_FAIL("Should not successfully setup an unsupported hash alg");
        return;
    }

    ret->val = TEST_PASSED;
}

/*
 * \brief This is the list of algorithms supported by the current
 *        configuration of the crypto engine used by the crypto
 *        service. In case the crypto engine default capabilities
 *        is changed, this list needs to be updated accordingly
 */
static const psa_algorithm_t hash_alg[] = {
    PSA_ALG_SHA_224,
    PSA_ALG_SHA_256,
    PSA_ALG_SHA_384,
    PSA_ALG_SHA_512,
};

static const uint8_t hash_val[][PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {
    {0x00, 0xD2, 0x90, 0xE2, 0x0E, 0x4E, 0xC1, 0x7E, /*!< SHA-224 */
     0x7A, 0x95, 0xF5, 0x10, 0x5C, 0x76, 0x74, 0x04,
     0x6E, 0xB5, 0x56, 0x5E, 0xE5, 0xE7, 0xBA, 0x15,
     0x6C, 0x23, 0x47, 0xF3},
    {0x6B, 0x22, 0x09, 0x2A, 0x37, 0x1E, 0xF5, 0x14, /*!< SHA-256 */
     0xF7, 0x39, 0x4D, 0xCF, 0xAD, 0x4D, 0x17, 0x46,
     0x66, 0xCB, 0x33, 0xA0, 0x39, 0xD8, 0x41, 0x4E,
     0xF1, 0x2A, 0xD3, 0x4D, 0x69, 0xC3, 0xB5, 0x3E},
    {0x64, 0x79, 0x11, 0xBB, 0x47, 0x4E, 0x47, 0x59, /*!< SHA-384 */
     0x3E, 0x4D, 0xBC, 0x60, 0xA5, 0xF9, 0xBF, 0x9C,
     0xC0, 0xBA, 0x55, 0x0F, 0x93, 0xCA, 0x72, 0xDF,
     0x57, 0x1E, 0x50, 0x56, 0xF9, 0x4A, 0x01, 0xD6,
     0xA5, 0x6F, 0xF7, 0x62, 0x34, 0x4F, 0x48, 0xFD,
     0x9D, 0x15, 0x07, 0x42, 0xB7, 0x72, 0x94, 0xB8},
    {0xB4, 0x1C, 0xA3, 0x6C, 0xA9, 0x67, 0x1D, 0xAD, /*!< SHA-512 */
     0x34, 0x1F, 0xBE, 0x1B, 0x83, 0xC4, 0x40, 0x2A,
     0x47, 0x42, 0x79, 0xBB, 0x21, 0xCA, 0xF0, 0x60,
     0xE4, 0xD2, 0x6E, 0x9B, 0x70, 0x12, 0x34, 0x3F,
     0x55, 0x2C, 0x09, 0x31, 0x0A, 0x5B, 0x40, 0x21,
     0x01, 0xA8, 0x3B, 0x58, 0xE7, 0x48, 0x13, 0x1A,
     0x7E, 0xCD, 0xE1, 0xD2, 0x46, 0x10, 0x58, 0x34,
     0x49, 0x14, 0x4B, 0xAA, 0x89, 0xA9, 0xF5, 0xB1},
};

void psa_hash_test(const psa_algorithm_t alg,
                   struct test_result_t *ret)
{
    const char *msg[] = {"This is my test message, ",
                         "please generate a hash for this."};

    const size_t msg_size[] = {25, 32}; /* Length in bytes of msg[0], msg[1] */
    const uint32_t msg_num = sizeof(msg)/sizeof(msg[0]);
    uint32_t idx;

    psa_status_t status;
    psa_hash_operation_t handle = psa_hash_operation_init();

    /* Setup the hash object for the desired hash*/
    status = psa_hash_setup(&handle, alg);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
            return;
        }

        TEST_FAIL("Error setting up hash operation object");
        return;
    }

    /* Update object with all the chunks of message */
    for (idx=0; idx<msg_num; idx++) {
        status = psa_hash_update(&handle,
                                 (const uint8_t *)msg[idx],msg_size[idx]);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error updating the hash operation object");
            return;
        }
    }

    /* Cycle until idx points to the correct index in the algorithm table */
    for (idx=0; hash_alg[idx] != alg; idx++);

    /* Finalise and verify that the hash is as expected */
    status = psa_hash_verify(&handle, &(hash_val[idx][0]), PSA_HASH_LENGTH(alg));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error verifying the hash operation object");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_unsupported_mac_test(const psa_key_type_t key_type,
                              const psa_algorithm_t alg,
                              struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_handle_t key_handle;
    psa_mac_operation_t handle = PSA_MAC_OPERATION_INIT;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    const uint8_t data[] = "THIS IS MY KEY1";

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    /* Setup the mac object for the unsupported mac algorithm */
    status = psa_mac_verify_setup(&handle, key_handle, alg);
    if (status != PSA_ERROR_NOT_SUPPORTED) {
        TEST_FAIL("Should not successfully setup an unsupported MAC alg");
        /* Do not return, to ensure key is destroyed */
    }

    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
    }
}

static const uint8_t hmac_val[][PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {
    {0xc1, 0x9f, 0x19, 0xac, 0x05, 0x65, 0x5f, 0x02, /*!< SHA-224 */
     0x1b, 0x64, 0x32, 0xd9, 0xb1, 0x49, 0xba, 0x75,
     0x05, 0x60, 0x52, 0x4e, 0x78, 0xfa, 0x61, 0xc9,
     0x37, 0x5d, 0x7f, 0x58},
    {0x94, 0x37, 0xbe, 0xb5, 0x7f, 0x7c, 0x5c, 0xb0, /*!< SHA-256 */
     0x0a, 0x92, 0x4d, 0xd3, 0xba, 0x7e, 0xb1, 0x1a,
     0xdb, 0xa2, 0x25, 0xb2, 0x82, 0x8e, 0xdf, 0xbb,
     0x61, 0xbf, 0x91, 0x1d, 0x28, 0x23, 0x4a, 0x04},
    {0x94, 0x21, 0x9b, 0xc3, 0xd5, 0xed, 0xe6, 0xee, /*!< SHA-384 */
     0x42, 0x10, 0x5a, 0x58, 0xa4, 0x4d, 0x67, 0x87,
     0x16, 0xa2, 0xa7, 0x6c, 0x2e, 0xc5, 0x85, 0xb7,
     0x6a, 0x4c, 0x90, 0xb2, 0x73, 0xee, 0x58, 0x3c,
     0x59, 0x16, 0x67, 0xf3, 0x6f, 0x30, 0x99, 0x1c,
     0x2a, 0xf7, 0xb1, 0x5f, 0x45, 0x83, 0xf5, 0x9f},
    {0x8f, 0x76, 0xef, 0x12, 0x0b, 0x92, 0xc2, 0x06, /*!< SHA-512 */
     0xce, 0x01, 0x18, 0x75, 0x84, 0x96, 0xd9, 0x6f,
     0x23, 0x88, 0xd4, 0xf8, 0xcf, 0x79, 0xf8, 0xcf,
     0x27, 0x12, 0x9f, 0xa6, 0x7e, 0x87, 0x9a, 0x68,
     0xee, 0xe2, 0xe7, 0x1d, 0x4b, 0xf2, 0x87, 0xc0,
     0x05, 0x6a, 0xbd, 0x7f, 0x9d, 0xff, 0xaa, 0xf3,
     0x9a, 0x1c, 0xb7, 0xb7, 0xbd, 0x03, 0x61, 0xa3,
     0xa9, 0x6a, 0x5d, 0xb2, 0x81, 0xe1, 0x6f, 0x1f},
};

static const uint8_t long_key_hmac_val[PSA_HASH_LENGTH(PSA_ALG_SHA_224)] = {
    0x47, 0xa3, 0x42, 0xb1, 0x2f, 0x52, 0xd3, 0x8f, /*!< SHA-224 */
    0x1e, 0x02, 0x4a, 0x46, 0x73, 0x0b, 0x77, 0xc1,
    0x5e, 0x93, 0x31, 0xa9, 0x3e, 0xc2, 0x81, 0xb5,
    0x3d, 0x07, 0x6f, 0x31
};

void psa_mac_test(const psa_algorithm_t alg,
                  uint8_t use_long_key,
                  struct test_result_t *ret)
{
    const char *msg[] = {"This is my test message, ",
                         "please generate a hmac for this."};
    const size_t msg_size[] = {25, 32}; /* Length in bytes of msg[0], msg[1] */
    const uint32_t msg_num = sizeof(msg)/sizeof(msg[0]);
    uint32_t idx;

    psa_key_handle_t key_handle;
    const uint8_t data[] = "THIS IS MY KEY1";
    const uint8_t long_data[] = "THIS IS MY UNCOMMONLY LONG KEY1";
    psa_key_type_t key_type = PSA_KEY_TYPE_HMAC;
    size_t bit_size_test_key = 0;
    psa_status_t status;
    psa_mac_operation_t handle = psa_mac_operation_init();
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_VERIFY_MESSAGE;

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import key */
    if (use_long_key == 1) {
        status = psa_import_key(&key_attributes,
                                long_data,
                                sizeof(long_data),
                                &key_handle);
    } else {
        status = psa_import_key(&key_attributes,
                                data,
                                sizeof(data),
                                &key_handle);
    }

    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    if (use_long_key == 1) {
        bit_size_test_key = BIT_SIZE_TEST_LONG_KEY;
    } else {
        bit_size_test_key = BIT_SIZE_TEST_KEY;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key_mac;
    }

    if (psa_get_key_bits(&retrieved_attributes) != bit_size_test_key) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key_mac;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key_mac;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    /* Setup the mac object for hmac */
    status = psa_mac_verify_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error setting up mac operation object");
        goto destroy_key_mac;
    }

    /* Update object with all the chunks of message */
    for (idx=0; idx<msg_num; idx++) {
        status = psa_mac_update(&handle,
                                (const uint8_t *)msg[idx],
                                msg_size[idx]);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Error during mac operation");
            goto destroy_key_mac;
        }
    }

    /* Cycle until idx points to the correct index in the algorithm table */
    for (idx=0; hash_alg[idx] != PSA_ALG_HMAC_GET_HASH(alg); idx++);

    /* Finalise and verify the mac value */
    if (use_long_key == 1) {
        status = psa_mac_verify_finish(
                                     &handle,
                                     &(long_key_hmac_val[0]),
                                     PSA_HASH_LENGTH(PSA_ALG_HMAC_GET_HASH(alg)));
    } else {
        status = psa_mac_verify_finish(
                                     &handle,
                                     &(hmac_val[idx][0]),
                                     PSA_HASH_LENGTH(PSA_ALG_HMAC_GET_HASH(alg)));
    }
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error during finalising the mac operation");
        goto destroy_key_mac;
    }

destroy_key_mac:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying the key");
    }
}

void psa_aead_test(const psa_key_type_t key_type,
                   const psa_algorithm_t alg,
                   struct test_result_t *ret)
{
    psa_key_handle_t key_handle;
    const size_t nonce_length = 12;
    const uint8_t nonce[] = "01234567890";
    const uint8_t plain_text[BYTE_SIZE_CHUNK] = "Sixteen bytes!!";
    const uint8_t associated_data[ASSOCIATED_DATA_SIZE] =
                                                      "This is associated data";
    uint8_t encrypted_data[ENC_DEC_BUFFER_SIZE] = {0};
    size_t encrypted_data_length = 0, decrypted_data_length = 0;
    uint8_t decrypted_data[ENC_DEC_BUFFER_SIZE] = {0};
    psa_status_t status;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint32_t comp_result;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_attributes_t retrieved_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);

    ret->val = TEST_PASSED;

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, key_type);

    /* Import a key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error importing a key");
        return;
    }

    status = psa_get_key_attributes(key_handle, &retrieved_attributes);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error getting key metadata");
        goto destroy_key_aead;
    }

    if (psa_get_key_bits(&retrieved_attributes) != BIT_SIZE_TEST_KEY) {
        TEST_FAIL("The number of key bits is different from expected");
        goto destroy_key_aead;
    }

    if (psa_get_key_type(&retrieved_attributes) != key_type) {
        TEST_FAIL("The type of the key is different from expected");
        goto destroy_key_aead;
    }

    psa_reset_key_attributes(&retrieved_attributes);

    /* Perform AEAD encryption */
    status = psa_aead_encrypt(key_handle, alg, nonce, nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              plain_text,
                              sizeof(plain_text),
                              encrypted_data,
                              sizeof(encrypted_data),
                              &encrypted_data_length);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
            goto destroy_key_aead;
        }

        TEST_FAIL("Error performing AEAD encryption");
        goto destroy_key_aead;
    }

    if (encrypted_data_length
        != PSA_AEAD_ENCRYPT_OUTPUT_SIZE(alg, sizeof(plain_text))) {
        TEST_FAIL("Encrypted data length is different than expected");
        goto destroy_key_aead;
    }

    /* Perform AEAD decryption */
    status = psa_aead_decrypt(key_handle, alg, nonce, nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              encrypted_data,
                              encrypted_data_length,
                              decrypted_data,
                              sizeof(decrypted_data),
                              &decrypted_data_length);

    if (status != PSA_SUCCESS) {
        if (status == PSA_ERROR_NOT_SUPPORTED) {
            TEST_FAIL("Algorithm NOT SUPPORTED by the implementation");
        } else {
            TEST_FAIL("Error performing AEAD decryption");
        }

        goto destroy_key_aead;
    }

    if (sizeof(plain_text) != decrypted_data_length) {
        TEST_FAIL("Decrypted data length is different from plain text");
        goto destroy_key_aead;
    }

#if DOMAIN_NS == 1U
    /* Check that the decrypted data is the same as the original data */
    comp_result = memcmp(plain_text, decrypted_data, sizeof(plain_text));
#else
    comp_result = tfm_memcmp(plain_text, decrypted_data, sizeof(plain_text));
#endif
    if (comp_result != 0) {
        TEST_FAIL("Decrypted data doesn't match with plain text");
        goto destroy_key_aead;
    }

destroy_key_aead:
    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
}

/*
 * The list of available AES cipher/AEAD mode for test.
 * Not all the modes can be available in some use cases and configurations.
 */
static const psa_algorithm_t test_aes_mode_array[] = {
#ifdef TFM_CRYPTO_TEST_ALG_CBC
    PSA_ALG_CBC_NO_PADDING,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    PSA_ALG_CCM,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    PSA_ALG_CFB,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CTR
    PSA_ALG_CTR,
#endif
#ifdef TFM_CRYPTO_TEST_ALG_GCM
    PSA_ALG_GCM,
#endif
    /* In case no AES algorithm is available */
    PSA_ALG_VENDOR_FLAG,
};

/* Number of available AES cipher modes */
#define NR_TEST_AES_MODE        (sizeof(test_aes_mode_array) / \
                                 sizeof(test_aes_mode_array[0]) - 1)

void psa_invalid_key_length_test(struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle;
    const uint8_t data[19] = {0};
    size_t mode_number = NR_TEST_AES_MODE;

    if (mode_number < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&key_attributes, test_aes_mode_array[0]);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* AES does not support 152-bit keys */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Should not successfully import with an invalid key length");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_policy_key_interface_test(struct test_result_t *ret)
{
    psa_algorithm_t alg = test_aes_mode_array[0];
    psa_algorithm_t alg_out;
    psa_key_lifetime_t lifetime = PSA_KEY_LIFETIME_VOLATILE;
    psa_key_lifetime_t lifetime_out;
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_usage_t usage = PSA_KEY_USAGE_EXPORT;
    psa_key_usage_t usage_out;
    size_t mode_number = NR_TEST_AES_MODE;

    if (mode_number < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Verify that initialised policy forbids all usage */
    usage_out = psa_get_key_usage_flags(&key_attributes);
    if (usage_out != 0) {
        TEST_FAIL("Unexpected usage value");
        return;
    }

    alg_out = psa_get_key_algorithm(&key_attributes);
    if (alg_out != 0) {
        TEST_FAIL("Unexpected algorithm value");
        return;
    }

    /* Set the key policy values */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);

    /* Check that the key policy has the correct usage */
    usage_out = psa_get_key_usage_flags(&key_attributes);
    if (usage_out != usage) {
        TEST_FAIL("Unexpected usage value");
        return;
    }

    /* Check that the key policy has the correct algorithm */
    alg_out = psa_get_key_algorithm(&key_attributes);
    if (alg_out != alg) {
        TEST_FAIL("Unexpected algorithm value");
        return;
    }

    /* Check the key handle has the correct key lifetime */
    lifetime_out = psa_get_key_lifetime(&key_attributes);

    if (lifetime_out != lifetime) {
        TEST_FAIL("Unexpected key lifetime value");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_policy_invalid_policy_usage_test(struct test_result_t *ret)
{
    psa_status_t status;
    psa_algorithm_t alg, not_permit_alg;
    psa_cipher_operation_t handle = psa_cipher_operation_init();
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle;
    psa_key_usage_t usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    size_t data_len;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t data_out[sizeof(data)];
    uint8_t i, j;
    size_t mode_number = NR_TEST_AES_MODE;

    ret->val = TEST_PASSED;

    if (mode_number < 2) {
        TEST_LOG("Two cipher modes are required. Skip this test case\r\n");
        return;
    }

    /*
     * Search for two modes for test. Both modes should be Cipher algorithms.
     * Otherwise, cipher setup may fail before policy permission check.
     */
    for (i = 0; i < (mode_number - 1); i++) {
        if (PSA_ALG_IS_CIPHER(test_aes_mode_array[i])) {
            alg = test_aes_mode_array[i];
            break;
        }
    }

    for (j = i + 1; j < mode_number; j++) {
        if (PSA_ALG_IS_CIPHER(test_aes_mode_array[j])) {
            not_permit_alg = test_aes_mode_array[j];
            break;
        }
    }

    if (j == NR_TEST_AES_MODE) {
        TEST_LOG("Unable to find two Cipher algs. Skip this test case.\r\n");
        return;
    }

    /* Setup the key policy */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* Import a key to the key handle for which policy has been set */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import a key");
        return;
    }

    /* Setup a cipher permitted by the key policy */
    status = psa_cipher_encrypt_setup(&handle, key_handle, alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to setup cipher operation");
        goto destroy_key;
    }

    status = psa_cipher_abort(&handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to abort cipher operation");
        goto destroy_key;
    }

    /* Attempt to setup a cipher with an alg not permitted by the policy */
    status = psa_cipher_encrypt_setup(&handle, key_handle, not_permit_alg);
    if (status != PSA_ERROR_NOT_PERMITTED) {
        TEST_FAIL("Was able to setup cipher operation with wrong alg");
        goto destroy_key;
    }

    /* Attempt to export the key, which is forbidden by the key policy */
    status = psa_export_key(key_handle, data_out, sizeof(data_out), &data_len);
    if (status != PSA_ERROR_NOT_PERMITTED) {
        TEST_FAIL("Should not be able to export key without correct usage");
        goto destroy_key;
    }

destroy_key:
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to destroy key");
    }
}

void psa_persistent_key_test(psa_key_id_t key_id, struct test_result_t *ret)
{
    psa_status_t status;
    int comp_result;
    psa_key_handle_t key_handle;
    psa_algorithm_t alg = test_aes_mode_array[0];
    psa_key_usage_t usage = PSA_KEY_USAGE_EXPORT;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    size_t data_len;
    const uint8_t data[] = "THIS IS MY KEY1";
    uint8_t data_out[sizeof(data)] = {0};
    size_t mode_number = NR_TEST_AES_MODE;

    if (mode_number < 1) {
        TEST_FAIL("A cipher mode in AES is required in current test case");
        return;
    }

    /* Setup the key attributes with a key ID to create a persistent key */
    psa_set_key_id(&key_attributes, key_id);
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, alg);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    /* Import key data to create the persistent key */
    status = psa_import_key(&key_attributes, data, sizeof(data), &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import a key");
        return;
    }

    /* Close the persistent key handle */
    status = psa_close_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to close a persistent key handle");
        return;
    }

    /* Open the previsously-created persistent key */
    status = psa_open_key(key_id, &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to open a persistent key");
        return;
    }

    /* Export the persistent key */
    status = psa_export_key(key_handle, data_out, sizeof(data_out), &data_len);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to export a persistent key");
        return;
    }

    if (data_len != sizeof(data)) {
        TEST_FAIL("Number of bytes of exported key different from expected");
        return;
    }

    /* Check that the exported key is the same as the imported one */
#if DOMAIN_NS == 1U
    comp_result = memcmp(data_out, data, sizeof(data));
#else
    comp_result = tfm_memcmp(data_out, data, sizeof(data));
#endif
    if (comp_result != 0) {
        TEST_FAIL("Exported key does not match the imported key");
        return;
    }

    /* Destroy the persistent key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to destroy a persistent key");
        return;
    }

    ret->val = TEST_PASSED;
}

#define KEY_DERIVE_OUTPUT_LEN          32
#define KEY_DERIV_SECRET_LEN           16
#define KEY_DERIV_LABEL_INFO_LEN       8
#define KEY_DERIV_SEED_SALT_LEN        8

static uint8_t key_deriv_secret[KEY_DERIV_SECRET_LEN];
static uint8_t key_deriv_label_info[KEY_DERIV_LABEL_INFO_LEN];
static uint8_t key_deriv_seed_salt[KEY_DERIV_SEED_SALT_LEN];

void psa_key_derivation_test(psa_algorithm_t deriv_alg,
                             struct test_result_t *ret)
{
    psa_key_handle_t input_handle = 0, output_handle = 0;
    psa_key_attributes_t input_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_attributes_t output_key_attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_derivation_operation_t deriv_ops;
    psa_status_t status;
    uint8_t counter = 0xA5;

    /* Prepare the parameters */
#if DOMAIN_NS == 1U
    memset(key_deriv_secret, counter, KEY_DERIV_SECRET_LEN);
    memset(key_deriv_label_info, counter++, KEY_DERIV_LABEL_INFO_LEN);
    memset(key_deriv_seed_salt, counter++, KEY_DERIV_SEED_SALT_LEN);
#else
    tfm_memset(key_deriv_secret, counter, KEY_DERIV_SECRET_LEN);
    tfm_memset(key_deriv_label_info, counter++, KEY_DERIV_LABEL_INFO_LEN);
    tfm_memset(key_deriv_seed_salt, counter++, KEY_DERIV_SEED_SALT_LEN);
#endif

    deriv_ops = psa_key_derivation_operation_init();

    psa_set_key_usage_flags(&input_key_attr, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&input_key_attr, deriv_alg);
    psa_set_key_type(&input_key_attr, PSA_KEY_TYPE_DERIVE);

    /* Force to use HMAC-SHA256 as HMAC operation so far */
    status = psa_import_key(&input_key_attr, key_deriv_secret,
                            KEY_DERIV_SECRET_LEN, &input_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import secret");
        return;
    }

    status = psa_key_derivation_setup(&deriv_ops, deriv_alg);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to setup derivation operation");
        goto destroy_key;
    }

    if (PSA_ALG_IS_TLS12_PRF(deriv_alg) ||
        PSA_ALG_IS_TLS12_PSK_TO_MS(deriv_alg)) {
        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_SEED,
                                                key_deriv_seed_salt,
                                                KEY_DERIV_SEED_SALT_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input seed");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_key(&deriv_ops,
                                              PSA_KEY_DERIVATION_INPUT_SECRET,
                                              input_handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input key");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_LABEL,
                                                key_deriv_label_info,
                                                KEY_DERIV_LABEL_INFO_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input label");
            goto deriv_abort;
        }
    } else if (PSA_ALG_IS_HKDF(deriv_alg)) {
        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_SALT,
                                                key_deriv_seed_salt,
                                                KEY_DERIV_SEED_SALT_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input salt");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_key(&deriv_ops,
                                              PSA_KEY_DERIVATION_INPUT_SECRET,
                                              input_handle);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input key");
            goto deriv_abort;
        }

        status = psa_key_derivation_input_bytes(&deriv_ops,
                                                PSA_KEY_DERIVATION_INPUT_INFO,
                                                key_deriv_label_info,
                                                KEY_DERIV_LABEL_INFO_LEN);
        if (status != PSA_SUCCESS) {
            TEST_FAIL("Failed to input info");
            goto deriv_abort;
        }
    } else {
        TEST_FAIL("Unsupported derivation algorithm");
        goto deriv_abort;
    }

    if (NR_TEST_AES_MODE < 1) {
        TEST_LOG("No AES algorithm to verify. Output raw data instead");
        psa_set_key_type(&output_key_attr, PSA_KEY_TYPE_RAW_DATA);
    } else {
        psa_set_key_usage_flags(&output_key_attr, PSA_KEY_USAGE_ENCRYPT);
        psa_set_key_algorithm(&output_key_attr, test_aes_mode_array[0]);
        psa_set_key_type(&output_key_attr, PSA_KEY_TYPE_AES);
    }
    psa_set_key_bits(&output_key_attr,
                     PSA_BYTES_TO_BITS(KEY_DERIVE_OUTPUT_LEN));

    status = psa_key_derivation_output_key(&output_key_attr, &deriv_ops,
                                           &output_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to output key");
        goto deriv_abort;
    }

    ret->val = TEST_PASSED;

deriv_abort:
    psa_key_derivation_abort(&deriv_ops);
destroy_key:
    psa_destroy_key(input_handle);
    if (output_handle) {
        psa_destroy_key(output_handle);
    }

    return;
}
