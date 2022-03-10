#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_executable(tfm_s)
add_library(secure_fw INTERFACE)
add_library(tfm_secure_api INTERFACE)
add_library(tfm_partitions INTERFACE)
# Lots of seperate things need to know which partitions are enabled, so this
# meta-target is provided so the related compile definitions can be collected in
# such a way that they don't cause issues with linking to the full spm (which is
# the other place these could be collected) Actual definitions are placed in the
# directories of the partitions
add_library(tfm_partition_defs INTERFACE)

add_subdirectory(partitions/lib/sprt)
add_subdirectory(partitions/audit_logging)
add_subdirectory(partitions/crypto)
add_subdirectory(partitions/initial_attestation)
add_subdirectory(partitions/protected_storage)
add_subdirectory(partitions/internal_trusted_storage)
add_subdirectory(partitions/platform)
add_subdirectory(partitions/psa_proxy)
add_subdirectory(partitions/firmware_update)
add_subdirectory(partitions/tfm_ffm11_partition)
add_subdirectory(spm)

target_include_directories(secure_fw
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/partitions>
)

target_link_libraries(secure_fw
    INTERFACE
        tfm_spm
        tfm_partitions
)

target_link_libraries(tfm_s
    PRIVATE
        secure_fw
        platform_s
        psa_interface
        tfm_sprt
)

set_target_properties(tfm_s
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_options(tfm_s
    PRIVATE
        --entry=Reset_Handler
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/tfm_s.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/tfm_s.map>
)

add_convert_to_bin_target(tfm_s)

############################ Secure API ########################################

target_include_directories(tfm_secure_api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(tfm_secure_api
    INTERFACE
        tfm_arch
)

set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/interface/src/psa/psa_client.c
    ${CMAKE_SOURCE_DIR}/interface/src/psa/psa_service.c
    PROPERTIES
    COMPILE_FLAGS $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
    COMPILE_FLAGS $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

############################# Secure veneers ###################################

if(NOT (TFM_PSA_API AND TFM_MULTI_CORE_TOPOLOGY))
    add_library(tfm_s_veneers STATIC)

    target_sources(tfm_s_veneers
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
    )

    # Since s_veneers.o doesn't exist when this is evaluated by cmake we need to
    # explicity specify what language it will use.
    set_target_properties(tfm_s_veneers
        PROPERTIES
            LINKER_LANGUAGE C
    )

    # Pretend we have a command to generate the veneers, when in reality all
    # that's needed is the dependency on tfm_s. This is required for the ninja
    # build system
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
        COMMAND
        DEPENDS tfm_s
    )

    target_link_options(tfm_s
        PRIVATE
            ${LINKER_VENEER_OUTPUT_FLAG}${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
    )
endif()

############################### CODE SHARING ###################################
if (TFM_CODE_SHARING)
    set(LIB_LIST mbedcrypto
                 crypto_service_cc312
                 platform_s
                 tfm_psa_rot_partition_crypto
                 tfm_psa_rot_partition_audit
                 tfm_psa_rot_partition_attestation
                 tfm_app_rot_partition_ps
                 tfm_psa_rot_partition_its
                 tfm_psa_rot_partition_platform
                 platform_s
                 tfm_sprt
                 tfm_spm
    )
    if (TFM_CODE_SHARING_PATH)
        compiler_link_shared_code(tfm_s
                                  ${TFM_CODE_SHARING_PATH} # Path to shared code
                                  EXTERNAL_TARGET # Not produced by tf-m build
                                  "${LIB_LIST}"
        )
    else()
        compiler_link_shared_code(tfm_s
                                  ${CMAKE_CURRENT_BINARY_DIR}/../bl2
                                  bl2
                                  "${LIB_LIST}"
        )
    endif()
endif()
