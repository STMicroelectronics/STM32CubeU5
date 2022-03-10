#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

if (NOT TFM_PARTITION_PROTECTED_STORAGE)
    return()
endif()

add_library(tfm_psa_rot_partition_ps_test STATIC)

target_sources(tfm_psa_rot_partition_ps_test
    PRIVATE
        ./tfm_ps_test_service.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_ps_test
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/test_services/tfm_ps_test_service/auto_generated/intermedia_tfm_ps_test_service.c>
)

target_include_directories(tfm_psa_rot_partition_ps_test
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/test_services/tfm_ps_test_service
)

target_link_libraries(tfm_psa_rot_partition_ps_test
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_app_rot_partition_ps
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_psa_rot_partition_ps_test
)

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_ps_test_service_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_ps_test_service_api.c
    PROPERTIES
        COMPILE_FLAGS -Wno-implicit-function-declaration
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_ps_test
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_TEST_PS
)
