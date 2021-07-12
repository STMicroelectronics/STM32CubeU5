/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTESTATION_KEY_H__
#define __ATTESTATION_KEY_H__

#include "psa/initial_attestation.h"
#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Get the initial attestation private key from platform layer and
 *        register it to Crypto service to sign the token.
 *
 * \param[out] key_handle_private  Pointer to the key handle allocated for the
 *                                 private key
 *
 * Private key MUST be present on the device, otherwise token cannot be signed.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was registered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be registered.
 */
enum psa_attest_err_t attest_register_initial_attestation_private_key(
                                          psa_key_handle_t *key_handle_private);

/**
 * \brief Unregister the initial attestation private key from Crypto service
 *        to do not occupy key slot.
 *
 * \param[in] key_handle_private  Key handle associated to the private key
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS   Key(s) was unregistered.
 * \retval  PSA_ATTEST_ERR_GENERAL   Key(s) could not be unregistered.
 */
enum psa_attest_err_t attest_unregister_initial_attestation_private_key(
                                           psa_key_handle_t key_handle_private);

/*!
 * \brief function to calculate instance id.
 *
 * \retval  PSA_ATTEST_ERR_CLAIM_UNAVAILABLE   public key anvailable.
 */
enum psa_attest_err_t attest_calc_instance_id(uint32_t *size,
                                              uint8_t  *buf);
#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_KEY_H__ */
