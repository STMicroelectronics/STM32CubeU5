/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#include "tfm_secure_api.h"
#include "secure_fw/spm/spm_partition_defs.h"

/******** TFM_SP_STORAGE ********/
psa_status_t tfm_sst_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_sst_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_sst_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_sst_remove_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_sst_get_support_req(psa_invec *, size_t, psa_outvec *, size_t);

/******** TFM_SP_ITS ********/
psa_status_t tfm_its_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_remove_req(psa_invec *, size_t, psa_outvec *, size_t);

#ifdef TFM_PARTITION_AUDIT_LOG
/******** TFM_SP_AUDIT_LOG ********/
psa_status_t audit_core_retrieve_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_add_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_record_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_delete_record(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_AUDIT_LOG */

/******** TFM_SP_CRYPTO ********/
psa_status_t tfm_crypto_allocate_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_import_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_destroy_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_get_key_information(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_export_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_export_public_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_copy_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_set_key_policy(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_get_key_policy(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_get_key_lifetime(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_generate_iv(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_set_iv(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_encrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_decrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_verify(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_clone(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_sign_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_verify_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_sign_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_verify_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_encrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_decrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_sign(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_verify(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_encrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_decrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_get_generator_capacity(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generator_read(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generator_import_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generator_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_agreement(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generate_random(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generate_key(psa_invec *, size_t, psa_outvec *, size_t);

#ifdef TFM_PARTITION_PLATFORM
/******** TFM_SP_PLATFORM ********/
psa_status_t platform_sp_system_reset(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_ioctl(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_PLATFORM */

/******** TFM_SP_INITIAL_ATTESTATION ********/
psa_status_t initial_attest_get_token(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t initial_attest_get_token_size(psa_invec *, size_t, psa_outvec *, size_t);

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST ********/
psa_status_t spm_core_test_sfn(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_init_success(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_direct_recursion(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST_2 ********/
psa_status_t spm_core_test_2_slave_service(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_sfn_invert(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_check_caller_client_id(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_get_every_second_byte(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_prepare_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_execute_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_TEST_PARTITION ********/
psa_status_t tfm_secure_client_service_sfn_run_tests(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
/******** TFM_IRQ_TEST_1 ********/
psa_status_t spm_irq_test_1_prepare_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_irq_test_1_execute_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
/******** TFM_APP_ROT ********/
psa_status_t tfm_app_rot_ioctl_sig(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_APP_ROT */


#define TFM_VENEER_FUNCTION(partition_name, sfn_name) \
    __tfm_secure_gateway_attributes__ \
    psa_status_t tfm_##sfn_name##_veneer(psa_invec *in_vec, \
                                         size_t in_len, \
                                         psa_outvec *out_vec, \
                                         size_t out_len) \
    { \
        TFM_CORE_IOVEC_SFN_REQUEST(partition_name##_ID, \
                                   (void *) sfn_name, \
                                   in_vec, in_len, out_vec, out_len); \
    }

/******** TFM_SP_STORAGE ********/
TFM_VENEER_FUNCTION(TFM_SP_STORAGE, tfm_sst_set_req)
TFM_VENEER_FUNCTION(TFM_SP_STORAGE, tfm_sst_get_req)
TFM_VENEER_FUNCTION(TFM_SP_STORAGE, tfm_sst_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_STORAGE, tfm_sst_remove_req)
TFM_VENEER_FUNCTION(TFM_SP_STORAGE, tfm_sst_get_support_req)

/******** TFM_SP_ITS ********/
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_set_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_remove_req)

#ifdef TFM_PARTITION_AUDIT_LOG
/******** TFM_SP_AUDIT_LOG ********/
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_retrieve_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_add_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_get_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_get_record_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_delete_record)
#endif /* TFM_PARTITION_AUDIT_LOG */

/******** TFM_SP_CRYPTO ********/
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_allocate_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_import_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_destroy_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_get_key_information)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_export_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_export_public_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_copy_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_set_key_policy)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_get_key_policy)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_get_key_lifetime)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_generate_iv)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_set_iv)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_encrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_decrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_verify)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_clone)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_sign_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_verify_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_sign_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_verify_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_encrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_decrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_sign)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_verify)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_encrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_decrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_get_generator_capacity)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generator_read)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generator_import_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generator_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_agreement)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generate_random)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generate_key)

#ifdef TFM_PARTITION_PLATFORM
/******** TFM_SP_PLATFORM ********/
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_system_reset)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_ioctl)
#endif /* TFM_PARTITION_PLATFORM */

/******** TFM_SP_INITIAL_ATTESTATION ********/
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, initial_attest_get_token)
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, initial_attest_get_token_size)

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn_init_success)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn_direct_recursion)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST_2 ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_slave_service)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_sfn_invert)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_check_caller_client_id)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_get_every_second_byte)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_prepare_test_scenario)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_execute_test_scenario)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_TEST_PARTITION ********/
TFM_VENEER_FUNCTION(TFM_SP_SECURE_TEST_PARTITION, tfm_secure_client_service_sfn_run_tests)
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
/******** TFM_IRQ_TEST_1 ********/
TFM_VENEER_FUNCTION(TFM_IRQ_TEST_1, spm_irq_test_1_prepare_test_scenario)
TFM_VENEER_FUNCTION(TFM_IRQ_TEST_1, spm_irq_test_1_execute_test_scenario)
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_APP_ROT
/******** TFM_APP_ROT ********/
TFM_VENEER_FUNCTION(TFM_APP_ROT, tfm_app_rot_ioctl_sig)
#endif /* TFM_PARTITION_APP_ROT */

