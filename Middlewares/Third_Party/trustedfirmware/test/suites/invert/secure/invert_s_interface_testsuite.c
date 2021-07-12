/*
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "invert_s_tests.h"
#include "tfm_api.h"
#include <stdio.h>
#include <string.h>

/* Define test suite for asset manager tests */
/* List of tests */
static void tfm_invert_test_1001(struct test_result_t *ret);
static void tfm_invert_test_1002(struct test_result_t *ret);

static struct test_t invert_veeners_tests[] = {
    {&tfm_invert_test_1001, "TFM_INVERT_TEST_1001",
     "Invert with valid buffer", {0} },
    {&tfm_invert_test_1002, "TFM_INVERT_TEST_1002",
     "Invert with invalid buffer", {0} },
};

void register_testsuite_s_invert_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(invert_veeners_tests) /
                 sizeof(invert_veeners_tests[0]));

    set_testsuite("Invert secure interface tests (TFM_INVERT_TEST_1XXX)",
                  invert_veeners_tests, list_size, p_test_suite);
}

/**
 * \brief Tests invert function with valid length
 */
static void tfm_invert_test_1001(struct test_result_t *ret)
{
    TEST_LOG("This test is DEPRECATED and the test execution was SKIPPED\r\n");
    ret->val = TEST_PASSED;
}

/**
 * \brief Tests invert function with invalid length
 */
static void tfm_invert_test_1002(struct test_result_t *ret)
{
    TEST_LOG("This test is DEPRECATED and the test execution was SKIPPED\r\n");
    ret->val = TEST_PASSED;
}
