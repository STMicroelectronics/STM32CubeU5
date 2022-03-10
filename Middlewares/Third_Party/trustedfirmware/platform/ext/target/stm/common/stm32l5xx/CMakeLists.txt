#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2021 STMicroelectronics. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

#========================= Platform region defs ===============================#

target_include_directories(platform_region_defs
    INTERFACE
        include
        ${CMAKE_CURRENT_SOURCE_DIR}/boards
)

target_compile_definitions(platform_region_defs
    INTERFACE
        $<$<OR:$<BOOL:${TEST_NS}>,$<BOOL:${TEST_S}>>:FLASH_LAYOUT_FOR_TEST>
)

#========================= Platform common defs ===============================#
target_add_scatter_file(tfm_s
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:ARMClang>>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_s.sct>
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
    $<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:IAR>>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_s.icf>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:ARMClang>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/armclang/tfm_isolation_l3.sct>
    $<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/gcc/tfm_isolation_l3.ld>
)

if(NS)
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:GNU>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/gcc/stm32l5xx_ns.ld>
        $<$<C_COMPILER_ID:ARMClang>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/armclang/stm32l5xx_ns.sct>
        $<$<C_COMPILER_ID:IAR>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/iar/stm32l5xx_ns.icf>
    )
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            CMSIS_5_RTX_V8MMN
    )
endif()

if(BL2)
    target_add_scatter_file(bl2
        $<$<C_COMPILER_ID:GNU>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/gcc/stm32l5xx_bl2.ld>
        $<$<C_COMPILER_ID:ARMClang>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/armclang/stm32l5xx_bl2.sct>
        $<$<C_COMPILER_ID:IAR>:${STM_COMMON_DIR}/stm32l5xx/Device/Source/iar/stm32l5xx_bl2.icf>
    )
endif()

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Inc
)

target_sources(platform_s
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/system_stm32l5xx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/tick.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/mpu_armv8m_drv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/spm_hal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/tfm_hal_isolation.c
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/target_cfg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/secure/low_level_device.c
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver/low_level_flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver/low_level_com.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_flash_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_dma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rcc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rcc_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_gtzc.c
        ${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${CMAKE_CURRENT_SOURCE_DIR}/secure/tfm_platform_system>
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rng.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rng_ex.c
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_hash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_hash_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pka.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/low_level_rng.c
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CMSE_FLAG}
)

# GNU ld has some strange behaviour to do with weak functions, and does not deal
# well with strong functions in static libraries overriding weak functions in
# object files. For this reason, the file mailbox_ipc_intr is linked directly to
# the ns binary as an object file. This issue does not affect armclang, but the
# fix is compatible.
target_sources(tfm_s
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/tick.c
)

#========================= Platform Non-Secure ================================#

target_include_directories(platform_ns
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Inc
)

target_sources(platform_ns
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/Templates/system_stm32l5xx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver/low_level_com.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_dma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rcc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart_ex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_cortex.c
)

#========================= Platform BL2 =======================================#

if(BL2)
    target_include_directories(platform_bl2
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver
            ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver
            ${CMAKE_CURRENT_SOURCE_DIR}/Device/Include
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Inc
            ${MCUBOOT_PATH}/boot/bootutil/include
            $<TARGET_PROPERTY:bl2,BINARY_DIR>/ext/mcuboot
            ${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/include
    )

    target_sources(platform_bl2
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Device/Source/Templates/system_stm32l5xx.c
            ${CMAKE_CURRENT_SOURCE_DIR}/bl2/boot_hal.c
            ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/mpu_armv8m_drv.c
            ${CMAKE_CURRENT_SOURCE_DIR}/bl2/tfm_low_level_security.c
            ${CMAKE_CURRENT_SOURCE_DIR}/bl2/low_level_device.c
            ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver/low_level_flash.c
            ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_Driver/low_level_com.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_flash.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_flash_ex.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr_ex.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_dma.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pwr.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_rcc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_cortex.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_gpio.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_uart_ex.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_gtzc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_hash.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_hash_ex.c
            ${CMAKE_CURRENT_SOURCE_DIR}/stm32l5xx_hal/Src/stm32l5xx_hal_pka.c
    )

    target_compile_options(platform_bl2
        PUBLIC
            ${COMPILER_CMSE_FLAG}
    )

    # GNU ld has some strange behaviour to do with weak functions, and does not deal
    # well with strong functions in static libraries overriding weak functions in
    # object files. For this reason, the file mailbox_ipc_intr is linked directly to
    # the ns binary as an object file. This issue does not affect armclang, but the
    # fix is compatible.
    target_sources(bl2
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Native_Driver/tick.c
    )
endif()

install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/stm_tool.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../bl2/ext/mcuboot/scripts/macro_parser.py
        DESTINATION ${CMAKE_BINARY_DIR}/scripts/ )

if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    set(PREPROCESSOR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/gcc/preprocess.sh)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL ARMClang)
    set(PREPROCESSOR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/armclang/preprocess.sh)
else()
    set(PREPROCESSOR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/iar/preprocess.sh)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/regression.sh
         ${PREPROCESSOR_FILE}
         ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/TFM_UPDATE.sh
         ${CMAKE_CURRENT_SOURCE_DIR}/boards/scripts/postbuild.sh
         PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
         DESTINATION ${CMAKE_BINARY_DIR} )

if (${TEST_S} OR ${TEST_NS})
    #the define FLASH_LAYOUT_FOR_TEST
    #increase SECURE and NON SECURE IMAGE and modify the  content of  low_level_device.c from BL2 (low_level_flash.c)
    set(FLAGS_FOR_BL2_PREPROCESSING
        "#define FLASH_LAYOUT_FOR_TEST\n"
        "#define BL2\n"
         )
    add_definitions("-DFLASH_LAYOUT_FOR_TEST")
else()
    set(FLAGS_FOR_BL2_PREPROCESSING
        "#define BL2\n"
       )
endif()

set(CONTENT_FOR_BL2_PREPROCESSING
    "#include \"flash_layout.h\"\n\n"
    "enum bl2_image_attributes {\n"
    "\tRE_IMAGE_FLASH_ADDRESS_SECURE = (FLASH_BASE_ADDRESS+FLASH_AREA_0_OFFSET),\n"
    "\tRE_IMAGE_FLASH_ADDRESS_NON_SECURE = (FLASH_BASE_ADDRESS+FLASH_AREA_1_OFFSET),\n"
    "\tRE_BL2_PERSO_ADDRESS = (FLASH_BASE_ADDRESS+FLASH_AREA_BL2_OFFSET),\n"
    "\tRE_BL2_BOOT_ADDRESS = (FLASH_BASE_ADDRESS+FLASH_AREA_BL2_OFFSET),\n"
    "\tRE_BL2_WRP_START = (FLASH_AREA_BL2_OFFSET),\n"
    "\tRE_BL2_WRP_END = (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_SIZE-0x1),\n"
    "\tRE_BL2_HDP_END = (FLASH_AREA_BL2_NOHDP_OFFSET-0x1),\n"
    "#if defined(EXTERNAL_FLASH)\n"
    "\tRE_IMAGE_FLASH_SECURE_UPDATE = (OSPI_FLASH_BASE_ADDRESS+FLASH_AREA_2_OFFSET),\n"
    "\tRE_IMAGE_FLASH_NON_SECURE_UPDATE = (OSPI_FLASH_BASE_ADDRESS+FLASH_AREA_3_OFFSET),\n"
    "\tRE_IMAGE_FLASH_UNUSED = (FLASH_BASE_ADDRESS+FLASH_AREA_1_OFFSET+FLASH_AREA_1_SIZE),\n"
    "#else\n"
    "\tRE_IMAGE_FLASH_SECURE_UPDATE = (FLASH_BASE_ADDRESS+FLASH_AREA_2_OFFSET),\n"
    "\tRE_IMAGE_FLASH_NON_SECURE_UPDATE = (FLASH_BASE_ADDRESS+FLASH_AREA_3_OFFSET),\n"
    "\tRE_IMAGE_FLASH_UNUSED = (FLASH_BASE_ADDRESS+FLASH_AREA_3_OFFSET+FLASH_AREA_3_SIZE),\n"
    "#endif\n"
    "\tRE_IMAGE_FLASH_SCRATCH = (FLASH_BASE_ADDRESS+FLASH_AREA_SCRATCH_OFFSET),\n"
    "\tRE_IMAGE_FLASH_NV_COUNTERS = (FLASH_BASE_ADDRESS+FLASH_NV_COUNTERS_AREA_OFFSET),\n"
    "\tRE_IMAGE_FLASH_NV_PS = (FLASH_BASE_ADDRESS+FLASH_PS_AREA_OFFSET),\n"
    "\tRE_IMAGE_FLASH_NV_ITS = (FLASH_BASE_ADDRESS+FLASH_ITS_AREA_OFFSET)\n}\;"
)

string(CONCAT BL2_PREPROCESSING ${FLAGS_FOR_BL2_PREPROCESSING}
       ${CONTENT_FOR_BL2_PREPROCESSING})
