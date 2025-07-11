/**
  * \file aes_alt.h
  *
  * \brief   This file contains AES definitions and functions.
  *
  *          The Advanced Encryption Standard (AES) specifies a FIPS-approved
  *          cryptographic algorithm that can be used to protect electronic
  *          data.
  *
  *          The AES algorithm is a symmetric block cipher that can
  *          encrypt and decrypt information. For more information, see
  *          <em>FIPS Publication 197: Advanced Encryption Standard</em> and
  *          <em>ISO/IEC 18033-2:2006: Information technology -- Security
  *          techniques -- Encryption algorithms -- Part 2: Asymmetric
  *          ciphers</em>.
  *
  *          The AES-XTS block mode is standardized by NIST SP 800-38E
  *          <https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38e.pdf>
  *          and described in detail by IEEE P1619
  *          <https://ieeexplore.ieee.org/servlet/opac?punumber=4375278>.
  */

/**
  * Copyright The Mbed TLS Contributors
  * Copyright (c) 2023 STMicroelectronics.
  * All Rights Reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the "License"); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  * This file implements ST AES HW services based on API from mbed TLS
  */

#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#if defined(MBEDTLS_AES_ALT)
#include "stm32u5xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * \brief          AES context structure
  */
typedef struct
{
  uint32_t aes_key[8];           /* Decryption key */
  uint32_t Algorithm;            /* Algorithm set (or not) in driver */
  CRYP_HandleTypeDef hcryp_aes;  /* HW driver handle */
  uint32_t ctx_save_cr;          /* Saved HW context for multi-instance */
}
mbedtls_aes_context;

#if defined(MBEDTLS_CIPHER_MODE_XTS)
/**
  * \brief The AES XTS context-type definition.
  */
typedef struct mbedtls_aes_xts_context
{
  mbedtls_aes_context crypt; /*!< The AES context to use for AES block
                                      encryption or decryption. */
  mbedtls_aes_context tweak; /*!< The AES context used for tweak computation. */
}
mbedtls_aes_xts_context;
#endif /* MBEDTLS_CIPHER_MODE_XTS */

int mbedtls_aes_wrap(mbedtls_aes_context *ctx,
                     uint32_t *input,
                     uint32_t *output);


#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_ALT_H */

