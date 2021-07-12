/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "platform/include/tfm_plat_defs.h"
#include "platform/include/tfm_plat_crypto_keys.h"

/*
 * This file contains the hard coded version of the ECDSA P-256 key pair in:
 * platform/common/tfm_initial_attestation_key.pem
 *
 * This key is used to sign the initial attestation token.
 * The key pair is stored in raw format, without any encoding(ASN.1, COSE).
 *
 * This ECDSA P-256 key is the construction of:
 *   - private key:      32 bytes
 *   - public key:
 *       - X-coordinate: 32 bytes
 *       - Y-coordinate: 32 bytes
 *
 * The hash of the raw public key (H(0x04 || X || Y)) is also included, because
 * it is used as an instance ID. It is a unique identifier of the device
 * instance.
 *
 * Instance ID is mapped to:
 *   - UEID in the EAT token
 *
 *   #######  DO NOT USE THIS KEY IN PRODUCTION #######
 */

/* Type of the EC curve which the key belongs to */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const enum ecc_curve_t initial_attestation_curve_type = P_256;

/* Initial attestation private key in raw format, without any encoding.
 * It belongs to the ECDSA P-256 curve.
 * It MUST present on the device-
 */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint8_t initial_attestation_private_key[] =
{
    0xA9, 0xB4, 0x54, 0xB2, 0x6D, 0x6F, 0x90, 0xA4,
    0xEA, 0x31, 0x19, 0x35, 0x64, 0xCB, 0xA9, 0x1F,
    0xEC, 0x6F, 0x9A, 0x00, 0x2A, 0x7D, 0xC0, 0x50,
    0x4B, 0x92, 0xA1, 0x93, 0x71, 0x34, 0x58, 0x5F
};

TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint32_t initial_attestation_private_key_size =
        sizeof(initial_attestation_private_key);

/* Initial attestation x-coordinate of the public key in raw format,
 * without any encoding.
 * It belongs to the ECDSA P-256 curve.
 * It MIGHT be present on the device.
 */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint8_t initial_attestation_public_x_key[] =
{
    0x79, 0xEB, 0xA9, 0x0E, 0x8B, 0xF4, 0x50, 0xA6,
    0x75, 0x15, 0x76, 0xAD, 0x45, 0x99, 0xB0, 0x7A,
    0xDF, 0x93, 0x8D, 0xA3, 0xBB, 0x0B, 0xD1, 0x7D,
    0x00, 0x36, 0xED, 0x49, 0xA2, 0xD0, 0xFC, 0x3F
};

TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint32_t initial_attestation_public_x_key_size =
        sizeof(initial_attestation_public_x_key);

/* Initial attestation y-coordinate of the public key in raw format,
 * without any encoding.
 * It belongs to the ECDSA P-256 curve.
 * It MIGHT be present on the device.
 */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint8_t initial_attestation_public_y_key[] =
{
    0xBF, 0xCD, 0xFA, 0x89, 0x56, 0xB5, 0x68, 0xBF,
    0xDB, 0x86, 0x73, 0xE6, 0x48, 0xD8, 0xB5, 0x8D,
    0x92, 0x99, 0x55, 0xB1, 0x4A, 0x26, 0xC3, 0x08,
    0x0F, 0x34, 0x11, 0x7D, 0x97, 0x1D, 0x68, 0x64
};

TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint32_t initial_attestation_public_y_key_size =
        sizeof(initial_attestation_public_y_key);

/* Hash (SHA256) of initial attestation public key.
 * Byte string representation of ECC public key according to
 * psa_export_public_key() in interface/include/psa/crypto.h:
 * 0x04 || X_coord || Y_coord
 */
TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint8_t initial_attestation_raw_public_key_hash[] =
{
    0xfa, 0x58, 0x75, 0x5f, 0x65, 0x86, 0x27, 0xce,
    0x54, 0x60, 0xf2, 0x9b, 0x75, 0x29, 0x67, 0x13,
    0x24, 0x8c, 0xae, 0x7a, 0xd9, 0xe2, 0x98, 0x4b,
    0x90, 0x28, 0x0e, 0xfc, 0xbc, 0xb5, 0x02, 0x48
};

TFM_LINK_SET_RO_IN_PARTITION_SECTION("TFM_SP_INITIAL_ATTESTATION")
const uint32_t initial_attestation_raw_public_key_hash_size =
        sizeof(initial_attestation_raw_public_key_hash);
