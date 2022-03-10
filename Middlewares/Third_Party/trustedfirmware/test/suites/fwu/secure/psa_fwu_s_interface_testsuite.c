/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#include "fwu_s_tests.h"
#include "psa/update.h"
#include "test_framework_helpers.h"
#include "../fwu_tests_common.h"
#include "tfm_memory_utils.h"

static struct test_t psa_fwu_s_tests[] = {
    {&tfm_fwu_test_common_001, "TFM_FWU_TEST_1001",
     "Write, install and abort interface", {TEST_PASSED} },
#if defined TFM_FWU_TEST_WRITE_WITH_NULL
    {&tfm_fwu_test_common_002, "TFM_FWU_TEST_1002",
     "Write interface with NULL block pointer", {TEST_PASSED} },
#endif
    {&tfm_fwu_test_common_003, "TFM_FWU_TEST_1003",
     "Write interface with image_offset + block_size overflow", {TEST_PASSED} },
    {&tfm_fwu_test_common_004, "TFM_FWU_TEST_1004",
     "Install interface with NULL dependency uuid", {TEST_PASSED} },
    {&tfm_fwu_test_common_005, "TFM_FWU_TEST_1005",
     "Install interface with NULL dependency version", {TEST_PASSED} },
    {&tfm_fwu_test_common_006, "TFM_FWU_TEST_1006",
     "Install before write", {TEST_PASSED} },
    {&tfm_fwu_test_common_007, "TFM_FWU_TEST_1007",
     "Install after abort", {TEST_PASSED} },
    {&tfm_fwu_test_common_008, "TFM_FWU_TEST_1008",
     "Abort interface with no image is being installing", {TEST_PASSED} },
    {&tfm_fwu_test_common_009, "TFM_FWU_TEST_1009",
     "Query interface with the active image", {TEST_PASSED} },
#if defined TFM_FWU_TEST_QUERY_WITH_NULL
    {&tfm_fwu_test_common_010, "TFM_FWU_TEST_1010",
     "Query interface with NULL info", {TEST_PASSED} },
#endif
    {&tfm_fwu_test_common_011, "TFM_FWU_TEST_1011",
     "Query interface invald image id", {TEST_PASSED} },
    {&tfm_fwu_test_common_012, "TFM_FWU_TEST_1012",
     "Accept interface", {TEST_PASSED} },
#ifdef TFM_FWU_TEST_REQUEST_REBOOT
    {&tfm_fwu_test_common_013, "TFM_FWU_TEST_1013",
     "Reboot interface", {TEST_PASSED} },
#endif
};

void register_testsuite_s_psa_fwu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(psa_fwu_s_tests) / sizeof(psa_fwu_s_tests[0]));

    set_testsuite("PSA firmware update interface tests"
                  "(TFM_FWU_TEST_1XXX)",
                  psa_fwu_s_tests, list_size, p_test_suite);
}
