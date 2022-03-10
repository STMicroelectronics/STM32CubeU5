#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project("Bootloader" VERSION 0.1.0 LANGUAGES C ASM)

add_executable(bl2
    src/security_cnt.c
    src/flash_map.c
    $<$<BOOL:${MCUBOOT_DATA_SHARING}>:src/shared_data.c>
)

add_subdirectory(ext/mcuboot)

set_target_properties(bl2
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(bl2
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/src>
)

target_link_libraries(bl2
    PRIVATE
        tfm_boot_status
)

target_link_options(bl2
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl2.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/bl2.map>
)

add_convert_to_bin_target(bl2)

############################### BOOT HAL # #####################################

add_library(bl2_hal INTERFACE)

target_include_directories(bl2_hal
    INTERFACE
        include
)

############################### MBEDCRYPTO #####################################

add_library(bl2_mbedcrypto_config INTERFACE)

target_compile_definitions(bl2_mbedcrypto_config
    INTERFACE
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA_LEN=${MCUBOOT_SIGNATURE_KEY_LEN}>
        MBEDTLS_CONFIG_FILE="$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ext/mcuboot/config/mcuboot-mbedtls-cfg.h>"
        # Workaround for https://github.com/ARMmbed/mbedtls/issues/1077
        $<$<OR:$<STREQUAL:${CMAKE_SYSTEM_ARCHITECTURE},armv8-m.base>,$<STREQUAL:${CMAKE_SYSTEM_ARCHITECTURE},armv6-m>>:MULADDC_CANNOT_USE_R7>
)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(ENABLE_TESTING OFF)
set(ENABLE_PROGRAMS OFF)
set(MBEDTLS_FATAL_WARNINGS OFF)
set(ENABLE_DOCS OFF)
set(INSTALL_MBEDTLS_HEADERS OFF)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto/install)

set(lib_target bl2_mbedcrypto_libs)
set(mbedcrypto_target bl2_mbedcrypto)
set(mbedtls_target bl2_mbedtls)
set(mbedx509_target bl2_mbedx509)
set(MBEDTLS_TARGET_PREFIX bl2_)

# Mbedcrypto is quite a large lib, and it uses too much memory for it to be
# reasonable to build it in debug info. As a compromise, if `debug` build type
# is selected mbedcrypto will build under `relwithdebinfo` which preserved debug
# symbols whild optimizing space.
set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${MBEDCRYPTO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

if(NOT TARGET bl2_mbedcrypto)
    message(FATAL_ERROR "Target bl2_mbedcrypto does not exist. Have the patches in ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH} ?
    Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
endif()

target_link_libraries(bl2_mbedcrypto
    PUBLIC
        bl2_mbedcrypto_config
)

############################### CODE SHARING ###################################

if (TFM_CODE_SHARING)
    compiler_create_shared_code(bl2 ${CMAKE_CURRENT_SOURCE_DIR}/src/shared_symbol_template.txt)

    if (NOT EXISTS ${MBEDCRYPTO_PATH}/library/code_share.c)
        message(FATAL_ERROR "File ${MBEDCRYPTO_PATH}/library/code_share.c does not exist.
        Have the patch ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/0005-Enable-crypto-code-sharing-between-independent-binar.patch
        been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH}?
        Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
    endif()
endif()
