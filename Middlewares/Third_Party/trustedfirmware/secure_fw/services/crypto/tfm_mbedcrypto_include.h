/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_MBEDCRYPTO_INCLUDE_H__
#define __TFM_MBEDCRYPTO_INCLUDE_H__

/* FIXME: The PSA Crypto headers in Mbed Crypto define PSA_SUCCESS and typedef
 * psa_status_t. To prevent redefinition errors in psa/client.h, use the
 * preprocessor to prefix psa_status_t in the Mbed Crypto headers, and then
 * undef psa_status_t and PSA_SUCCESS after the include.
 */
#define psa_status_t mbedcrypto__psa_status_t
/* Include the crypto_spe.h header before including the PSA Crypto header from
 * Mbed Crypto
 */
#include "crypto_spe.h"
#include "mbed-crypto/include/psa/crypto.h"
#undef psa_status_t
#undef PSA_SUCCESS

#endif /* __TFM_MBEDCRYPTO_INCLUDE_H__ */
