#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_qcbor STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor
    PRIVATE
        src/ieee754.c
        src/qcbor_encode.c
        src/qcbor_decode.c
        src/UsefulBuf.c
        util/qcbor_util.c
)

target_include_directories(tfm_qcbor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/util>
)

target_link_libraries(tfm_qcbor
    PRIVATE
        tfm_attestation_defs
        tfm_t_cose
)

############################ qcbor test ########################################

add_library(tfm_qcbor_test STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_test
    PRIVATE
        test/float_tests.c
        test/half_to_double_from_rfc7049.c
        test/qcbor_decode_tests.c
        test/qcbor_encode_tests.c
        test/run_tests.c
        test/UsefulBuf_Tests.c
)

target_include_directories(tfm_qcbor_test
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(tfm_qcbor_test
    PRIVATE
        tfm_qcbor
)
