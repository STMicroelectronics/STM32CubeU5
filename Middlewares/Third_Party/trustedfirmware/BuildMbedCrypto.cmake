#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Mbed Crypto can be built in the same way Mbed TLS is built
include(${TFM_ROOT_DIR}/BuildMbedtls.cmake)

# After building the install target, rename the installed include/psa directory
# to include/mbedcrypto/psa to avoid name clash with the PSA Crypto headers in
# TF-M.
add_custom_command(TARGET ${MBEDTLS_TARGET_NAME}_install
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${MBEDTLS_INSTALL_DIR}/include/psa
                           ${MBEDTLS_INSTALL_DIR}/include/mbedcrypto/psa
                   COMMAND ${CMAKE_COMMAND} -E remove_directory
                           ${MBEDTLS_INSTALL_DIR}/include/psa)
