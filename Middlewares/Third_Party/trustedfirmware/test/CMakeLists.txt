#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

add_library(tfm_ns_tests INTERFACE)
add_library(tfm_test_framework_ns INTERFACE)
add_library(tfm_test_framework_s INTERFACE)

# For multi-core projects, the NS app can be run on a different CPU to the
# Secure code. To facilitate this, we once again reload the compiler to load the
# setting for the NS CPU. Cmake settings are directory scoped so this affects
# anything loaded from or declared in this dir.
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    tfm_toolchain_reload_compiler()
endif()

add_subdirectory(test_services)

add_subdirectory(suites/attestation)
add_subdirectory(suites/audit)
add_subdirectory(suites/core)
add_subdirectory(suites/crypto)
add_subdirectory(suites/its)
add_subdirectory(suites/qcbor)
add_subdirectory(suites/ps)
add_subdirectory(suites/t_cose)
add_subdirectory(suites/platform)
add_subdirectory(suites/fwu)

if(TFM_PSA_API)
    add_subdirectory(suites/ipc)
endif()
if(TFM_MULTI_CORE_TOPOLOGY)
    add_subdirectory(suites/multi_core)
endif()

add_library(tfm_test_framework_common INTERFACE)

target_sources(tfm_test_framework_common
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework.c
    ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_helpers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_integ_test_helper.c
)

target_include_directories(tfm_test_framework_common
    INTERFACE
        framework
)

target_link_libraries(tfm_test_framework_ns
    INTERFACE
        tfm_test_framework_common
        tfm_api_ns
        tfm_ns_interface
        tfm_ns_log
)

target_sources(tfm_ns_tests
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/non_secure_suites.c
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_framework_ns
        tfm_partition_defs
)

target_compile_definitions(tfm_ns_tests
    INTERFACE
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        $<$<BOOL:${TFM_INTERACTIVE_TEST}>:CORE_TEST_INTERACTIVE>
)

####################### Secure #################################################

target_link_libraries(tfm_test_framework_s
    INTERFACE
        psa_interface
        tfm_test_framework_common
        tfm_s_log
)

if (TEST_S)
    target_sources(tfm_s_tests
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/framework/secure_suites.c
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_framework_s
            tfm_partition_defs
            tfm_spm
    )

    target_compile_definitions(tfm_s_tests
        INTERFACE
            $<$<BOOL:${PS_TEST_NV_COUNTERS}>:PS_TEST_NV_COUNTERS>
    )
endif()
