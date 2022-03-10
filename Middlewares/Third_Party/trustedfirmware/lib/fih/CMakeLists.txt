#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_fih STATIC)

target_sources(tfm_fih
    PRIVATE
        src/fih.c
)

target_include_directories(tfm_fih
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(tfm_fih
    PRIVATE
        platform_s
)

target_compile_definitions(tfm_fih
    PUBLIC
        TFM_FIH_PROFILE_${TFM_FIH_PROFILE}
        $<$<NOT:$<STREQUAL:${TFM_FIH_PROFILE},OFF>>:TFM_FIH_PROFILE_ON>
)
