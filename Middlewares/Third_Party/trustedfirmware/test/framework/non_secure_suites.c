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
#include "test/suites/sst/non_secure/sst_ns_tests.h"
#include "test/suites/its/non_secure/its_ns_tests.h"
#include "test/suites/audit/non_secure/audit_ns_tests.h"
#include "test/suites/crypto/non_secure/crypto_ns_tests.h"
#include "test/suites/attestation/non_secure/attestation_ns_tests.h"
#include "test/suites/qcbor/non_secure/qcbor_ns_tests.h"
#include "test/suites/invert/non_secure/invert_ns_tests.h"
#include "test/suites/core/non_secure/core_ns_tests.h"
#include "test/suites/ipc/non_secure/ipc_ns_tests.h"

static struct test_suite_t test_suites[] = {
#ifdef SERVICES_TEST_NS
    /* List test cases which are compliant with level 1 isolation */

    {&register_testsuite_ns_psa_ps_interface, 0, 0, 0},

    /* Non-secure ITS test cases */
    {&register_testsuite_ns_psa_its_interface, 0, 0, 0},

    /* Non-secure Crypto test cases */
    {&register_testsuite_ns_crypto_interface, 0, 0, 0},

    /* Non-secure initial attestation service test cases */
    {&register_testsuite_ns_attestation_interface, 0, 0, 0},

    /* Non-secure QCBOR library test cases */
    {&register_testsuite_ns_qcbor, 0, 0, 0},

#ifndef TFM_PSA_API
    /*
     * FixMe: skip below test cases temporary since target service is not
     * IPC compatible yet.
     */
#ifdef ENABLE_AUDIT_LOGGING_SERVICE_TESTS
    /* Non-secure Audit Logging test cases */
    {&register_testsuite_ns_audit_interface, 0, 0, 0},
#endif

#endif

#ifdef TFM_PARTITION_TEST_CORE
    /* Non-secure invert test cases */
    /* Note: since this is sample code, only run if test services are enabled */
    {&register_testsuite_ns_invert_interface, 0, 0, 0},
#endif
#endif /* SERVICES_TEST_NS */

#ifdef CORE_TEST_POSITIVE
#if !defined(TFM_PSA_API) || (TFM_LVL == 1)
    /* Non-secure core test cases */
    {&register_testsuite_ns_core_positive, 0, 0, 0},
#endif
#endif

#ifdef CORE_TEST_INTERACTIVE
    /* Non-secure interactive test cases */
    {&register_testsuite_ns_core_interactive, 0, 0, 0},
#endif

#ifdef ENABLE_IPC_TEST
    /* Non-secure IPC test cases */
    {&register_testsuite_ns_ipc_interface, 0, 0, 0},
#endif
    /* End of test suites */
    {0, 0, 0, 0}
};

void start_integ_test(void)
{
    integ_test("Non-secure", test_suites);
}

/* Service stand-in for NS tests. To be called from a non-secure context */
void tfm_non_secure_client_run_tests(void)
{
    start_integ_test();
}
