/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_integ_test.h"
#include "test_framework_integ_test_helper.h"
#include "test_framework.h"

/* Service specific includes */
#include "test/suites/sst/secure/sst_tests.h"
#include "test/suites/its/secure/its_s_tests.h"
#include "test/suites/audit/secure/audit_s_tests.h"
#include "test/suites/attestation/secure/attestation_s_tests.h"
#include "test/suites/invert/secure/invert_s_tests.h"
#include "test/suites/crypto/secure/crypto_s_tests.h"
#include "test/suites/ipc/secure/ipc_s_tests.h"

static struct test_suite_t test_suites[] = {
#ifdef SERVICES_TEST_S
    /* List test cases which are compliant with level 1 isolation */

    {&register_testsuite_s_psa_ps_interface, 0, 0, 0},
    {&register_testsuite_s_psa_ps_reliability, 0, 0, 0},

#ifdef SST_TEST_NV_COUNTERS
    {&register_testsuite_s_rollback_protection, 0, 0, 0},
#endif

    /* Secure ITS test cases */
    {&register_testsuite_s_psa_its_interface, 0, 0, 0},
    {&register_testsuite_s_psa_its_reliability, 0, 0, 0},

    /* Crypto test cases */
    {&register_testsuite_s_crypto_interface, 0, 0, 0},

    /* Secure initial attestation service test cases */
    {&register_testsuite_s_attestation_interface, 0, 0, 0},

#ifndef TFM_PSA_API
    /*
     * FixMe: since the following partitions haven't implement the IPC model,
     * they will block the process. Skip them in IPC model.
     */
#ifdef ENABLE_AUDIT_LOGGING_SERVICE_TESTS
    /* Secure Audit Logging test cases */
    {&register_testsuite_s_audit_interface, 0, 0, 0},
#endif

#endif

#ifdef TFM_PARTITION_TEST_CORE
    /* Secure invert test cases */
    /* Note: since this is sample code, only run if test services are enabled */
    {&register_testsuite_s_invert_interface, 0, 0, 0},
#endif /* TFM_PARTITION_TEST_CORE*/

#ifdef ENABLE_IPC_TEST
    /* Secure IPC test cases */
    {&register_testsuite_s_ipc_interface, 0, 0, 0},
#endif
#endif /* SERVICES_TEST_S */
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

void start_integ_test(void)
{
    setup_integ_test();
    integ_test("Secure", test_suites);
    tear_down_integ_test();
}
