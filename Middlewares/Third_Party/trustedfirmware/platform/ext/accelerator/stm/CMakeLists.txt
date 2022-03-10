#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2021 STMicroelectronics. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
# Allow linking to things 'upwards' in the directory tree (in this case bl2 / tfm_partition_crypto)
cmake_policy(SET CMP0079 NEW)
# Allow relative paths
cmake_policy(SET CMP0076 NEW)

if(NOT DEFINED PLATFORM_DIR)
    set(PLATFORM_DIR ../../.. CACHE PATH "Path to plaform dir")
endif()
target_compile_definitions(platform_bl2
    PRIVATE
        CRYPTO_HW_ACCELERATOR
)

################################ BL2 ###########################################
if(BL2)

    set(STM_LIB_TARGET bl2_crypto_hw)

    target_compile_definitions(bl2_mbedcrypto
        PUBLIC
            CRYPTO_HW_ACCELERATOR
    )

    target_compile_options(bl2_mbedcrypto
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
            $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
    )
endif()
include(${PLATFORM_DIR}/ext/target/${TFM_PLATFORM}/accelerator/CMakeLists.txt)

set(STM_LIB_TARGET crypto_service_crypto_hw)

target_link_libraries(crypto_service_mbedcrypto
    PUBLIC
        crypto_service_crypto_hw
)

target_compile_definitions(crypto_service_mbedcrypto
    PUBLIC
        CRYPTO_HW_ACCELERATOR

)

target_compile_options(crypto_service_mbedcrypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)
