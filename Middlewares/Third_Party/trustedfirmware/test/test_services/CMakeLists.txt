#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Since the main test directory is a NS dir, this subdir is specifically made a
# secure directory as it build the secure side test services
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload.cmake)
    tfm_toolchain_reload_compiler()
endif()

if (TEST_S)
    # Secure tests are declared in this dir so that it can use the secure CPU
    # definitions
    add_library(tfm_s_tests INTERFACE)
    if (TFM_PARTITION_CRYPTO OR FORWARD_PROT_MSG)
        add_library(tfm_test_suite_crypto_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TFM_PARTITION_INITIAL_ATTESTATION OR FORWARD_PROT_MSG)
        add_library(tfm_test_suite_attestation_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE OR FORWARD_PROT_MSG)
        add_library(tfm_test_suite_its_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if (TFM_PARTITION_PROTECTED_STORAGE OR FORWARD_PROT_MSG)
        add_library(tfm_test_suite_ps_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if(NOT TFM_PSA_API AND TFM_PARTITION_AUDIT_LOG)
        add_library(tfm_test_suite_audit_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if(TFM_PARTITION_PLATFORM OR FORWARD_PROT_MSG)
        add_library(tfm_test_suite_platform_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if(TFM_PSA_API)
        add_library(tfm_test_suite_ipc_s STATIC EXCLUDE_FROM_ALL)
    endif()
    if(TFM_PARTITION_FIRMWARE_UPDATE)
        add_library(tfm_test_suite_fwu_s STATIC EXCLUDE_FROM_ALL)
    endif()
endif()

add_subdirectory(tfm_core_test)
add_subdirectory(tfm_core_test_2)
if(TFM_IRQ_TEST)
    add_subdirectory(tfm_irq_test_service_1)
endif()
add_subdirectory(tfm_ps_test_service)
if (TEST_S)
    add_subdirectory(tfm_secure_client_service)
    add_subdirectory(tfm_secure_client_2)
endif()
if(TFM_PSA_API)
    add_subdirectory(tfm_ipc_service)
    add_subdirectory(tfm_ipc_client)
endif()
