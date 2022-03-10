#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if((NOT TFM_PARTITION_INITIAL_ATTESTATION AND NOT FORWARD_PROT_MSG)
   OR SYMMETRIC_INITIAL_ATTESTATION)
    return()
endif()

####################### Non Secure #############################################

add_library(tfm_test_suite_t_cose_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_t_cose_ns
    PRIVATE
        non_secure/t_cose_ns_testsuite.c
)

target_include_directories(tfm_test_suite_t_cose_ns
    PUBLIC
        non_secure
)

target_compile_definitions(tfm_test_suite_t_cose_ns
    PRIVATE
        DOMAIN_NS=1
)

target_link_libraries(tfm_test_suite_t_cose_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_t_cose_test
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_t_cose_ns
)
