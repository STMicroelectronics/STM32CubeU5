/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "flash_layout.h"
#include "test_framework_integ_test.h"
#include "test_framework_integ_test_helper.h"
#include "test_framework.h"

/* Service specific includes */
#if defined(TFM_PARTITION_PROTECTED_STORAGE) || defined(FORWARD_PROT_MSG)
#include "ps_tests.h"
#endif
#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) || defined(FORWARD_PROT_MSG)
#include "its_s_tests.h"
#endif
#if defined(TFM_PARTITION_INITIAL_ATTESTATION) || defined(FORWARD_PROT_MSG)
#include "attest_s_tests.h"
#endif
#if defined(TFM_PARTITION_CRYPTO) || defined(FORWARD_PROT_MSG)
#include "crypto_s_tests.h"
#endif
#if defined(TFM_PARTITION_FIRMWARE_UPDATE)
#include "fwu_s_tests.h"
#endif
#if defined(TFM_PARTITION_PLATFORM) || defined(FORWARD_PROT_MSG)
#include "platform_s_tests.h"
#endif
#ifdef TFM_PSA_API
#include "ipc_s_tests.h"
#else
#ifdef TFM_PARTITION_AUDIT_LOG
#include "audit_s_tests.h"
#endif
#endif /* TFM_PSA_API */

static struct test_suite_t test_suites[] = {
#if defined(TFM_PARTITION_PROTECTED_STORAGE) || defined(FORWARD_PROT_MSG)
    {&register_testsuite_s_psa_ps_interface, 0, 0, 0},
    {&register_testsuite_s_psa_ps_reliability, 0, 0, 0},

#ifdef PS_TEST_NV_COUNTERS
    {&register_testsuite_s_rollback_protection, 0, 0, 0},
#endif
#endif

#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) || defined(FORWARD_PROT_MSG)
    /* Secure ITS test cases */
    {&register_testsuite_s_psa_its_interface, 0, 0, 0},
    {&register_testsuite_s_psa_its_reliability, 0, 0, 0},
#endif

#if defined(TFM_PARTITION_CRYPTO) || defined(FORWARD_PROT_MSG)
    /* Crypto test cases */
    {&register_testsuite_s_crypto_interface, 0, 0, 0},
#endif

#if defined(TFM_PARTITION_INITIAL_ATTESTATION) || defined(FORWARD_PROT_MSG)
    /* Secure initial attestation service test cases */
    {&register_testsuite_s_attestation_interface, 0, 0, 0},
#endif

#if defined(TFM_PARTITION_PLATFORM) || defined(FORWARD_PROT_MSG)
    /* Secure platform service test cases */
    {&register_testsuite_s_platform_interface, 0, 0, 0},
#endif

#ifdef TFM_PARTITION_AUDIT_LOG
    /* Secure Audit Logging test cases */
    {&register_testsuite_s_audit_interface, 0, 0, 0},
#endif

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    /* Non-secure Firmware Update test cases */
    {&register_testsuite_s_psa_fwu_interface, 0, 0, 0},
#endif

#ifdef TFM_PSA_API
/*  Not activated  flag TFM_PARTITION_FFM11 not used  */
#if 0
    /* Secure IPC test cases */
    {&register_testsuite_s_ipc_interface, 0, 0, 0},
#endif
#endif
    /* End of test suites */
    {0, 0, 0, 0}
};

static void setup_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no setup
     */
}

static void tear_down_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no tear down
     */
}

enum test_suite_err_t start_integ_test(void)
{
    enum test_suite_err_t retval;

    setup_integ_test();
    retval = integ_test("Secure", test_suites);
    tear_down_integ_test();
    return retval;
}
