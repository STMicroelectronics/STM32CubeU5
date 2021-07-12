/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>

#include "test_framework.h"
#include "test_framework_integ_test_helper.h"

void integ_test(const char *suite_type,
                struct test_suite_t test_suites[])
{
    uint32_t i;

    printf_set_color(YELLOW);
    printf("\r\n#### Execute test suites for the %s area ####\r\n", suite_type);

    /* Executes test suites */
    for (i = 0; test_suites[i].freg != NULL; i++) {
        if (run_testsuite(&test_suites[i]) != TEST_SUITE_ERR_NO_ERROR) {
            /* End function execution */
            return;
        }
    }

    /* Prints test suites summary */
    printf_set_color(YELLOW);
    printf("\r\n*** %s test suites summary ***\r\n", suite_type);
    for (i = 0; test_suites[i].freg != NULL; i++) {
        printf_set_color(WHITE);
        printf("Test suite '%s' has ", test_suites[i].name);
        if (test_suites[i].val == TEST_PASSED) {
            printf_set_color(GREEN);
            printf(" PASSED\r\n");
        } else {
            printf_set_color(RED);
            printf(" FAILED\r\n");
        }
    }

    printf_set_color(YELLOW);
    printf("\r\n*** End of %s test suites ***\r\n", suite_type);
}
