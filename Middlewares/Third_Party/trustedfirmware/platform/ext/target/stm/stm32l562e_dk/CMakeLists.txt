#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2021 STMicroelectronics. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)

set(STM32L563E_DK_DIR ${CMAKE_CURRENT_LIST_DIR})
set(STM_COMMON_DIR ${STM32L563E_DK_DIR}/../common)

#def for stm32l5xx
include(${STM_COMMON_DIR}/stm32l5xx/CMakeLists.txt)
#========================= Platform defs ===============================#

# Specify the location of platform specific build dependencies.
target_sources(tfm_s
    PRIVATE
        ${STM_COMMON_DIR}/stm32l5xx/Device/Source/startup_stm32l5xx_s.c
)

if(NS)
    target_sources(tfm_ns
        PRIVATE
            ${STM_COMMON_DIR}/stm32l5xx/Device/Source/startup_stm32l5xx_ns.c
    )
endif()

if(BL2)
    target_sources(bl2
        PRIVATE
            ${STM_COMMON_DIR}/stm32l5xx/Device/Source/startup_stm32l5xx_bl2.c
            ${STM_COMMON_DIR}/stm32l5xx/bl2/low_level_ospi_device.c
            ${STM_COMMON_DIR}/stm32l5xx/CMSIS_Driver/low_level_ospi_flash.c
            ${STM_COMMON_DIR}/stm32l5xx/stm32l5xx_hal/Src/stm32l5xx_hal_ospi.c
            ${STM_COMMON_DIR}/Components/mx25lm51245g/mx25lm51245g.c
            ${STM32L563E_DK_DIR}/src/stm32l562e_discovery_ospi.c
    )
    target_include_directories(bl2
        PRIVATE
            ${STM_COMMON_DIR}/Components/mx25lm51245g
    )
endif()

install(FILES ${STM32L563E_DK_DIR}/include/flash_layout.h
              ${STM32L563E_DK_DIR}/include/region_defs.h
        DESTINATION ${CMAKE_BINARY_DIR} )
set (BL2_FILE_TO_PREPROCESS ${CMAKE_CURRENT_BINARY_DIR}/image_macros_to_preprocess_bl2.c)
file(WRITE ${BL2_FILE_TO_PREPROCESS} ${BL2_PREPROCESSING})
install(FILES ${BL2_FILE_TO_PREPROCESS}
        DESTINATION ${CMAKE_BINARY_DIR} )
