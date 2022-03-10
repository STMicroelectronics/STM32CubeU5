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
#define MBEDTLS_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_ENTROPY_HARDWARE_ALT

#ifdef MBEDTLS_SHA1_C
#define MBEDTLS_SHA1_ALT
#endif /* MBEDTLS_SHA1_C */

#ifdef MBEDTLS_SHA256_C
#define MBEDTLS_SHA256_ALT
#endif /* MBEDTLS_SHA256_C */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
