#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TFM_PSA_API)
    return()
endif()

cmake_policy(SET CMP0079 NEW)

####################### Non Secure #############################################

add_library(tfm_test_suite_ipc_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_ipc_ns
    PRIVATE
        non_secure/ipc_ns_interface_testsuite.c
)

target_include_directories(tfm_test_suite_ipc_ns
    PUBLIC
        non_secure
)

target_compile_definitions(tfm_test_suite_ipc_ns
    PRIVATE
        DOMAIN_NS=1
)

target_link_libraries(tfm_test_suite_ipc_ns
    PRIVATE
        tfm_test_framework_ns
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_ipc_ns
)

####################### Secure #################################################

if (TEST_S)
    target_sources(tfm_test_suite_ipc_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/ipc_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_ipc_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_link_libraries(tfm_test_suite_ipc_s
        PRIVATE
            tfm_test_framework_s
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_ipc_s
    )
endif()
