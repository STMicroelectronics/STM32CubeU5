#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_sprt STATIC)

target_include_directories(tfm_sprt
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(tfm_sprt
    PRIVATE
        ./crt_memcmp.c
        ./crt_memcpy.c
        ./crt_memmove.c
        ./crt_memset.c
        ./service_api.c
        $<$<NOT:$<STREQUAL:${TFM_PARTITION_LOG_LEVEL},TFM_PARTITION_LOG_LEVEL_SILENCE>>:./tfm_sp_log_raw.c>
        $<$<NOT:$<STREQUAL:${TFM_PARTITION_LOG_LEVEL},TFM_PARTITION_LOG_LEVEL_SILENCE>>:${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_sp_logdev_periph.c>
        $<$<BOOL:${TFM_SP_META_PTR_ENABLE}>:./sprt_partition_metadata_indicator.c>
)

target_link_libraries(tfm_sprt
    platform_s
    psa_interface
    tfm_boot_status
    tfm_secure_api
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_LOG_LEVEL=${TFM_PARTITION_LOG_LEVEL}
)
