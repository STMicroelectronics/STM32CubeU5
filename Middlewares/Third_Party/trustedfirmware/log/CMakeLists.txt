#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# For multi-core projects, the NS app can be run on a different CPU to the
# Secure code. To facilitate this, we once again reload the compiler to load the
# setting for the NS CPU. Cmake settings are directory scoped so this affects
# anything loaded from or declared in this dir.
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
endif()

############################# Ns Log ###########################################
add_library(tfm_ns_log STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_ns_log
    PRIVATE
        tfm_log_raw.c
)

target_include_directories(tfm_ns_log
    PUBLIC
        .
)

target_link_libraries(tfm_ns_log
    PRIVATE
        platform_ns
)

################################ S Log #########################################
add_library(tfm_s_log STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_s_log
    PRIVATE
        ${CMAKE_SOURCE_DIR}/interface/src/log/tfm_log_raw.c
)

target_include_directories(tfm_s_log
    PUBLIC
        ${CMAKE_SOURCE_DIR}/interface/include/
        ${CMAKE_SOURCE_DIR}/interface/include/log
)

target_link_libraries(tfm_s_log
    PRIVATE
        platform_s
)
