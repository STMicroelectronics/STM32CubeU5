#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if(BL2)
    add_library(bl2_crypto_hw STATIC)
endif()
add_library(crypto_service_crypto_hw STATIC)

add_subdirectory(${CRYPTO_HW_ACCELERATOR_TYPE})

################################ BL2 ###########################################

if(BL2)
    target_include_directories(bl2_crypto_hw
        PUBLIC
            interface
    )

    target_compile_definitions(bl2_crypto_hw
        PUBLIC
            CRYPTO_HW_ACCELERATOR
    )

    target_link_libraries(platform_bl2
        PRIVATE
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR_OTP_STATE}>:bl2_crypto_hw>
    )
endif()

############################ Crypto Service ####################################

target_include_directories(crypto_service_crypto_hw
    PUBLIC
        interface
)

target_compile_definitions(crypto_service_crypto_hw
    PUBLIC
        CRYPTO_HW_ACCELERATOR
)

target_link_libraries(platform_s
    PRIVATE
        $<$<STREQUAL:${CRYPTO_HW_ACCELERATOR_OTP_STATE},ENABLED>:crypto_service_crypto_hw>
)
