
#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
if (NOT TFM_PARTITION_FFM11)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_ffm11 STATIC)

target_sources(tfm_app_rot_partition_ffm11
    PRIVATE
        tfm_ffm11_partition.c
)

# The generated sources
target_sources(tfm_app_rot_partition_ffm11
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/tfm_ffm11_partition/auto_generated/intermedia_tfm_ffm11_partition.c
)

target_include_directories(tfm_app_rot_partition_ffm11
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/tfm_ffm11_partition
)

target_link_libraries(tfm_app_rot_partition_ffm11
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_sprt
)

############################ Partition Defs ####################################
target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_ffm11
)
target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_FFM11
)
