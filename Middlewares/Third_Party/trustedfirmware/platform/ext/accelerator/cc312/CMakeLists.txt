#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
# Allow linking to things 'upwards' in the directory tree (in this case bl2 / tfm_psa_rot_partition_crypto)
cmake_policy(SET CMP0079 NEW)
# Allow relative paths
cmake_policy(SET CMP0076 NEW)

if(NOT DEFINED CC312_PATH)
    set(CC312_PATH ../../../../lib/ext/cryptocell-312-runtime CACHE PATH "Path to CC312 lib")
endif()

target_compile_definitions(platform_bl2
    PRIVATE
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR_OTP_STATE}>:CRYPTO_HW_ACCELERATOR_OTP_${CRYPTO_HW_ACCELERATOR_OTP_STATE}>
        CRYPTO_HW_ACCELERATOR
)

################################ BL2 ###########################################
if(BL2)
    target_sources(bl2_crypto_hw
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/cc312.c
            $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_OTP_STATE},PROVISIONING>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_provisioning.c>
    )

    target_include_directories(bl2_crypto_hw
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(bl2_crypto_hw
        PUBLIC
            bl2_cc312
        PRIVATE
            bl2_mbedcrypto
            platform_bl2
            $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_OTP_STATE},PROVISIONING>:cc312_cdmpu>
    )

    set(CC312_LIB_TARGET bl2_cc312)
    set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
    add_subdirectory(${CC312_PATH} ${CMAKE_CURRENT_BINARY_DIR}/bl2_cc312)
    set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

    target_sources(bl2_cc312
        PUBLIC
            $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
    )

    # Adding two targets as link-time dependencies of each other seems bad, but
    # in reality it just means that they'll share headers and compile defs.
    target_link_libraries(bl2_cc312
        PRIVATE
            bl2_mbedcrypto
        PUBLIC
            platform_bl2
    )
    target_link_libraries(bl2_mbedcrypto
        PRIVATE
            bl2_cc312
        PUBLIC
            bl2_crypto_hw
    )

    target_link_libraries(cc312_cdmpu
        INTERFACE
            bl2_mbedcrypto
    )

    target_compile_definitions(bl2_mbedcrypto
        PUBLIC
            CRYPTO_HW_ACCELERATOR
            MBEDTLS_ECDH_LEGACY_CONTEXT
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR_OTP_STATE}>:CRYPTO_HW_ACCELERATOR_OTP_${CRYPTO_HW_ACCELERATOR_OTP_STATE}>
    )

    target_compile_options(bl2_mbedcrypto
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
            $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
    )

    #Include platform specific defs (registers etc)
    target_include_directories(bl2_cc312
        BEFORE
        PUBLIC
            ${CC312_PATH}/shared/hw/include/${TFM_PLATFORM}
    )

    target_include_directories(cc312_cdmpu
        BEFORE
        INTERFACE
            ${CC312_PATH}/shared/hw/include/${TFM_PLATFORM}
    )
endif()

############################ Crypto Service ####################################

target_sources(crypto_service_crypto_hw
    PRIVATE
        cc312.c
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
)

target_include_directories(crypto_service_crypto_hw
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(crypto_service_crypto_hw
    PUBLIC
        crypto_service_cc312
    PRIVATE
        crypto_service_mbedcrypto
        platform_s
)

target_link_libraries(platform_s
    PRIVATE
        $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_OTP_STATE},ENABLED>:crypto_service_crypto_hw>
        $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_OTP_STATE},ENABLED>:crypto_service_cc312>
)

set(CC312_LIB_TARGET crypto_service_cc312)
set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${CC312_PATH} ${CMAKE_CURRENT_BINARY_DIR}/crypto_service_cc312)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

target_sources(crypto_service_cc312
    PUBLIC
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:${CMAKE_CURRENT_SOURCE_DIR}/cc312_log.c>
)

# Adding two targets as link-time dependencies of each other seems bad, but
# in reality it just means that they'll share headers and compile defs.
target_link_libraries(crypto_service_cc312
    PRIVATE
        crypto_service_mbedcrypto
    PUBLIC
        platform_s
)
target_link_libraries(crypto_service_mbedcrypto
    PRIVATE
        crypto_service_cc312
    PUBLIC
        crypto_service_crypto_hw
)

target_compile_definitions(crypto_service_mbedcrypto
    PUBLIC
        CRYPTO_HW_ACCELERATOR
        MBEDTLS_ECDH_LEGACY_CONTEXT
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR_OTP_STATE}>:CRYPTO_HW_ACCELERATOR_OTP_${CRYPTO_HW_ACCELERATOR_OTP_STATE}>
)

target_compile_options(crypto_service_mbedcrypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

#Include platform specific defs (registers etc)
target_include_directories(crypto_service_cc312
    BEFORE
    PUBLIC
    ${CC312_PATH}/shared/hw/include/${TFM_PLATFORM}
)
