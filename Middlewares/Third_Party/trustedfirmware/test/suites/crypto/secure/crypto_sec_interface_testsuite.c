/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_helpers.h"
#include "tfm_secure_client_2_api.h"
#include "tfm_api.h"
#include "../crypto_tests_common.h"

/* List of tests */
static void tfm_crypto_test_5001(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CBC
static void tfm_crypto_test_5002(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CBC */
#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_5003(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
#ifdef TFM_CRYPTO_TEST_ALG_CTR
static void tfm_crypto_test_5005(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CTR */
static void tfm_crypto_test_5007(struct test_result_t *ret);
static void tfm_crypto_test_5008(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_5009(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
static void tfm_crypto_test_5010(struct test_result_t *ret);
static void tfm_crypto_test_5011(struct test_result_t *ret);
static void tfm_crypto_test_5012(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_5013(struct test_result_t *ret);
static void tfm_crypto_test_5014(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
static void tfm_crypto_test_5019(struct test_result_t *ret);
static void tfm_crypto_test_5020(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_5021(struct test_result_t *ret);
static void tfm_crypto_test_5022(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
static void tfm_crypto_test_5024(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_5030(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
#ifdef TFM_CRYPTO_TEST_ALG_GCM
static void tfm_crypto_test_5031(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_GCM */
static void tfm_crypto_test_5032(struct test_result_t *ret);
static void tfm_crypto_test_5033(struct test_result_t *ret);
static void tfm_crypto_test_5034(struct test_result_t *ret);
static void tfm_crypto_test_5035(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_5036(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
static void tfm_crypto_test_5037(struct test_result_t *ret);
static void tfm_crypto_test_5038(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_HKDF
static void tfm_crypto_test_5039(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_HKDF */

static struct test_t crypto_tests[] = {
    {&tfm_crypto_test_5001, "TFM_CRYPTO_TEST_5001",
     "Secure Key management interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CBC
    {&tfm_crypto_test_5002, "TFM_CRYPTO_TEST_5002",
     "Secure Symmetric encryption (AES-128-CBC) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CBC */
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    {&tfm_crypto_test_5003, "TFM_CRYPTO_TEST_5003",
     "Secure Symmetric encryption (AES-128-CFB) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
#ifdef TFM_CRYPTO_TEST_ALG_CTR
    {&tfm_crypto_test_5005, "TFM_CRYPTO_TEST_5005",
     "Secure Symmetric encryption (AES-128-CTR) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CTR */
    {&tfm_crypto_test_5007, "TFM_CRYPTO_TEST_5007",
     "Secure Symmetric encryption invalid cipher", {TEST_PASSED} },
    {&tfm_crypto_test_5008, "TFM_CRYPTO_TEST_5008",
     "Secure Symmetric encryption invalid cipher (AES-152)", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    {&tfm_crypto_test_5009, "TFM_CRYPTO_TEST_5009",
     "Secure Symmetric encryption invalid cipher (HMAC-128-CFB)", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
    {&tfm_crypto_test_5010, "TFM_CRYPTO_TEST_5010",
     "Secure Unsupported Hash (SHA-1) interface", {TEST_PASSED} },
    {&tfm_crypto_test_5011, "TFM_CRYPTO_TEST_5011",
     "Secure Hash (SHA-224) interface", {TEST_PASSED} },
    {&tfm_crypto_test_5012, "TFM_CRYPTO_TEST_5012",
     "Secure Hash (SHA-256) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
    {&tfm_crypto_test_5013, "TFM_CRYPTO_TEST_5013",
     "Secure Hash (SHA-384) interface", {TEST_PASSED} },
    {&tfm_crypto_test_5014, "TFM_CRYPTO_TEST_5014",
     "Secure Hash (SHA-512) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
    {&tfm_crypto_test_5019, "TFM_CRYPTO_TEST_5019",
     "Secure Unsupported HMAC (SHA-1) interface", {TEST_PASSED} },
    {&tfm_crypto_test_5020, "TFM_CRYPTO_TEST_5020",
     "Secure HMAC (SHA-256) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
    {&tfm_crypto_test_5021, "TFM_CRYPTO_TEST_5021",
     "Secure HMAC (SHA-384) interface", {TEST_PASSED} },
    {&tfm_crypto_test_5022, "TFM_CRYPTO_TEST_5022",
     "Secure HMAC (SHA-512) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
    {&tfm_crypto_test_5024, "TFM_CRYPTO_TEST_5024",
     "Secure HMAC with long key (SHA-224) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    {&tfm_crypto_test_5030, "TFM_CRYPTO_TEST_5030",
     "Secure AEAD (AES-128-CCM) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
#ifdef TFM_CRYPTO_TEST_ALG_GCM
    {&tfm_crypto_test_5031, "TFM_CRYPTO_TEST_5031",
     "Secure AEAD (AES-128-GCM) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_GCM */
    {&tfm_crypto_test_5032, "TFM_CRYPTO_TEST_5032",
     "Secure key policy interface", {TEST_PASSED} },
    {&tfm_crypto_test_5033, "TFM_CRYPTO_TEST_5033",
     "Secure key policy check permissions", {TEST_PASSED} },
    {&tfm_crypto_test_5034, "TFM_CRYPTO_TEST_5034",
     "Secure persistent key interface", {TEST_PASSED} },
    {&tfm_crypto_test_5035, "TFM_CRYPTO_TEST_5035",
     "Key access control", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    {&tfm_crypto_test_5036, "TFM_CRYPTO_TEST_6036",
     "Secure AEAD interface with truncated auth tag (AES-128-CCM-8)",
     {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
    {&tfm_crypto_test_5037, "TFM_CRYPTO_TEST_5037",
     "Secure TLS 1.2 PRF key derivation", {TEST_PASSED} },
    {&tfm_crypto_test_5038, "TFM_CRYPTO_TEST_5038",
     "Secure TLS-1.2 PSK-to-MasterSecret key derivation", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_HKDF
    {&tfm_crypto_test_5039, "TFM_CRYPTO_TEST_5039",
     "Secure HKDF key derivation", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_HKDF */
};

void register_testsuite_s_crypto_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size = (sizeof(crypto_tests) / sizeof(crypto_tests[0]));

    set_testsuite("Crypto secure interface tests (TFM_CRYPTO_TEST_5XXX)",
                  crypto_tests, list_size, p_test_suite);
}

/**
 * \brief Secure interface test for Crypto
 *
 * \details The scope of this set of tests is to functionally verify
 *          the interfaces specified by psa/crypto.h are working
 *          as expected. This is not meant to cover all possible
 *          scenarios and corner cases.
 *
 */
static void tfm_crypto_test_5001(struct test_result_t *ret)
{
    psa_key_interface_test(PSA_KEY_TYPE_AES, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CBC
static void tfm_crypto_test_5002(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CBC_NO_PADDING, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CBC */

#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_5003(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CFB, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CFB */

#ifdef TFM_CRYPTO_TEST_ALG_CTR
static void tfm_crypto_test_5005(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CTR, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CTR */

static void tfm_crypto_test_5007(struct test_result_t *ret)
{
    psa_invalid_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_HMAC(PSA_ALG_SHA_256),
                            16, ret);
}

static void tfm_crypto_test_5008(struct test_result_t *ret)
{
    psa_invalid_key_length_test(ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_5009(struct test_result_t *ret)
{
    /* HMAC is not a block cipher */
    psa_invalid_cipher_test(PSA_KEY_TYPE_HMAC, PSA_ALG_CFB, 16, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CFB */

static void tfm_crypto_test_5010(struct test_result_t *ret)
{
    psa_unsupported_hash_test(PSA_ALG_SHA_1, ret);
}

static void tfm_crypto_test_5011(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_224, ret);
}

static void tfm_crypto_test_5012(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_256, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_5013(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_384, ret);
}

static void tfm_crypto_test_5014(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_512, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */

static void tfm_crypto_test_5019(struct test_result_t *ret)
{
    psa_unsupported_mac_test(PSA_KEY_TYPE_HMAC, PSA_ALG_HMAC(PSA_ALG_SHA_1),
                             ret);
}

static void tfm_crypto_test_5020(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_256), 0, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_5021(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_384), 0, ret);
}

static void tfm_crypto_test_5022(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_512), 0, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */

static void tfm_crypto_test_5024(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_224), 1, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_5030(struct test_result_t *ret)
{
    psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_CCM, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CCM */

#ifdef TFM_CRYPTO_TEST_ALG_GCM
static void tfm_crypto_test_5031(struct test_result_t *ret)
{
    psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_GCM, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_GCM */

static void tfm_crypto_test_5032(struct test_result_t *ret)
{
    psa_policy_key_interface_test(ret);
}

static void tfm_crypto_test_5033(struct test_result_t *ret)
{
    psa_policy_invalid_policy_usage_test(ret);
}

static void tfm_crypto_test_5034(struct test_result_t *ret)
{
    psa_persistent_key_test(1, ret);
}

/**
 * \brief Tests key access control based on partition ID
 *
 * \param[out] ret  Test result
 */
static void tfm_crypto_test_5035(struct test_result_t *ret)
{
    psa_status_t status;
    psa_key_handle_t key_handle;
    const uint8_t data[] = "THIS IS MY KEY1";
    psa_key_attributes_t key_attributes = psa_key_attributes_init();

    /* Set key sage and type */
    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_EXPORT);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);

    status = psa_import_key(&key_attributes, data, sizeof(data),
                            &key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to import key");
        return;
    }

    /* Attempt to destroy the key handle from the Secure Client 2 partition */
    status = tfm_secure_client_2_call_test(
                                      TFM_SECURE_CLIENT_2_ID_CRYPTO_ACCESS_CTRL,
                                      &key_handle, sizeof(key_handle));
    if (status != PSA_ERROR_NOT_PERMITTED) {
        TEST_FAIL("Should not be able to destroy key from another partition");
        return;
    }

    /* Destroy the key */
    status = psa_destroy_key(key_handle);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Error destroying a key");
    }
    return;
}

#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_5036(struct test_result_t *ret)
{
    psa_algorithm_t alg = PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM,
                                                       TRUNCATED_AUTH_TAG_LEN);

    psa_aead_test(PSA_KEY_TYPE_AES, alg, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_GCM */

static void tfm_crypto_test_5037(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_TLS12_PRF(PSA_ALG_SHA_256), ret);
}

static void tfm_crypto_test_5038(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_TLS12_PSK_TO_MS(PSA_ALG_SHA_256), ret);
}

#ifdef TFM_CRYPTO_TEST_HKDF
static void tfm_crypto_test_5039(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_HKDF(PSA_ALG_SHA_256), ret);
}
#endif /* TFM_CRYPTO_TEST_HKDF */
