/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 * Copyright (c) 2021 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBEDTLS_ACCELERATOR_CONF_H
#define MBEDTLS_ACCELERATOR_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* RNG Config */
#undef MBEDTLS_TEST_NULL_ENTROPY
#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#undef MBEDTLS_ECP_NIST_OPTIM
#define MBEDTLS_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_C

#define MBEDTLS_ENTROPY_HARDWARE_ALT


/* Main Config */

#ifdef MBEDTLS_ECDSA_C
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT

#ifndef CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING
#define MBEDTLS_ECDSA_GENKEY_ALT
#endif
#endif /* MBEDTLS_ECDSA_C */

#ifdef MBEDTLS_RSA_C
#define MBEDTLS_RSA_ALT
#endif /* MBEDTLS_RSA_C */
/* stm hardware */
#ifdef MBEDTLS_SHA1_C
#define MBEDTLS_SHA1_ALT
#endif /* MBEDTLS_SHA1_C */

/* stm hardware */
#ifdef MBEDTLS_SHA256_C
#define MBEDTLS_SHA256_ALT
#endif /* MBEDTLS_SHA256_C */

#if defined(MBEDTLS_ECP_C) && defined(MBEDTLS_MD_C)
#define MBEDTLS_ECP_ALT
#define MBEDTLS_MD5_ALT
#endif /* MBEDTLS_ECP_C && MBEDTLS_MD_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
