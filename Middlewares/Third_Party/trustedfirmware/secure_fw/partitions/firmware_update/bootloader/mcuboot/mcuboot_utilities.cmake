#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(tfm_fwu_mcuboot_util INTERFACE)

target_sources(tfm_fwu_mcuboot_util
    INTERFACE
        ${MCUBOOT_PATH}/boot/bootutil/src/bootutil_misc.c
        ${CMAKE_SOURCE_DIR}/bl2/src/flash_map.c
        ${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/flash_map_extended.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bootloader/mcuboot/tfm_mcuboot_fwu.c
)

target_include_directories(tfm_fwu_mcuboot_util
    INTERFACE
        ${CMAKE_BINARY_DIR}/bl2/ext/mcuboot
        ${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/include
        ${MCUBOOT_PATH}/boot/bootutil/include
        ${MCUBOOT_PATH}/boot/bootutil/src
        ${CMAKE_CURRENT_SOURCE_DIR}/bootloader
)

target_link_libraries(tfm_fwu_mcuboot_util
    INTERFACE
        platform_region_defs
)

target_link_libraries(tfm_psa_rot_partition_fwu
    PRIVATE
        tfm_fwu_mcuboot_util
)

target_compile_definitions(tfm_psa_rot_partition_fwu
    PRIVATE
        MCUBOOT_${MCUBOOT_UPGRADE_STRATEGY}
)
