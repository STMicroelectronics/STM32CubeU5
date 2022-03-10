#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_FIRMWARE_UPDATE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_fwu STATIC)

target_include_directories(tfm_psa_rot_partition_fwu
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update
)

target_sources(tfm_psa_rot_partition_fwu
    PRIVATE
        tfm_fwu_req_mngr.c
        tfm_fwu.c
        $<$<BOOL:${TFM_PSA_API}>:
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/firmware_update/auto_generated/intermedia_tfm_firmware_update.c>
        )

# Include the bootloader specific configuration.
if ((NOT TFM_FWU_BOOTLOADER_LIB) OR (NOT EXISTS ${TFM_FWU_BOOTLOADER_LIB}))
    message(FATAL_ERROR "TFM_FWU_BOOTLOADER_LIB invalid")
endif()

include(${TFM_FWU_BOOTLOADER_LIB})

target_link_libraries(tfm_psa_rot_partition_fwu
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_sprt
)

target_compile_definitions(tfm_psa_rot_partition_fwu
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:TFM_PSA_API>
)

############################ Secure API ########################################

target_sources(tfm_secure_api
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_fwu_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_fwu_secure_api.c
    PROPERTIES
        COMPILE_FLAGS -Wno-implicit-function-declaration
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_fwu
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
    TFM_PARTITION_FIRMWARE_UPDATE
)
