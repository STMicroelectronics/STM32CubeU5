#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_core_test STATIC)

target_sources(tfm_psa_rot_partition_core_test
    PRIVATE
        tfm_ss_core_test.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_core_test
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/test_services/tfm_core_test/auto_generated/intermedia_tfm_ss_core_test.c>
)

target_include_directories(tfm_psa_rot_partition_core_test
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/test_services/tfm_core_test
)

target_link_libraries(tfm_psa_rot_partition_core_test
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_arch
        tfm_test_framework_s
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_psa_rot_partition_core_test
)

target_include_directories(tfm_test_framework_s
    INTERFACE
        .
)

# header files included by other libraries
add_library(tfm_psa_rot_partition_core_test_defs INTERFACE)

target_include_directories(tfm_psa_rot_partition_core_test_defs
    INTERFACE
        .
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_core_test
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_TEST_CORE
)
