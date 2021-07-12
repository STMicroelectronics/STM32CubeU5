#-------------------------------------------------------------------------------
# Copyright (c) 2018-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.7)

#Tell cmake where our modules can be found
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../cmake)

#Some project global settings
set (CRYPTO_DIR "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(TFM_ROOT_DIR "${CRYPTO_DIR}/../../.." ABSOLUTE)

#Include common stuff to control cmake.
include("Common/BuildSys")

#Start an embedded project.
embedded_project_start(CONFIG "${TFM_ROOT_DIR}/configs/ConfigDefault.cmake")
project(tfm_crypto LANGUAGES ASM C)
embedded_project_fixup()

#Get the definition of what files we need to build
set (ENABLE_CRYPTO ON)
#The backend of the service is based on Mbed Crypto
set (CRYPTO_ENGINE_MBEDTLS ON)

if (CRYPTO_ENGINE_MBEDTLS)
  #Define location of Mbed Crypto source, build, and installation directory.
  get_filename_component(MBEDTLS_SOURCE_DIR "${TFM_ROOT_DIR}/../mbed-crypto" ABSOLUTE)
  set (MBEDTLS_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto")
  set (MBEDTLS_INSTALL_DIR ${MBEDTLS_BINARY_DIR}/mbedcrypto_install)
  set (MBEDTLS_TARGET_NAME "mbedcrypto_lib")
endif()

include(CMakeLists.inc)

#Create a list of the C defines
list(APPEND TFM_CRYPTO_C_DEFINES_LIST __ARM_FEATURE_CMSE=${ARM_FEATURE_CMSE} __thumb2__ TFM_LVL=${TFM_LVL})

if (CRYPTO_ENGINE_MBEDTLS)
  list(APPEND TFM_CRYPTO_C_DEFINES_LIST TFM_CRYPTO_ENGINE_MBEDTLS MBEDTLS_CONFIG_FILE="platform/ext/common/tfm_mbedcrypto_config.h")
endif()

#Add module configuration parameters in case they are provided during CMake configuration step
list(APPEND TFM_CRYPTO_C_DEFINES_LIST TFM_CRYPTO_KEY_MODULE_DISABLED=${CRYPTO_KEY_MODULE_DISABLED}
                                      TFM_CRYPTO_AEAD_MODULE_DISABLED=${CRYPTO_AEAD_MODULE_DISABLED}
                                      TFM_CRYPTO_MAC_MODULE_DISABLED=${CRYPTO_MAC_MODULE_DISABLED}
                                      TFM_CRYPTO_CIPHER_MODULE_DISABLED=${CRYPTO_CIPHER_MODULE_DISABLED}
                                      TFM_CRYPTO_HASH_MODULE_DISABLED=${CRYPTO_HASH_MODULE_DISABLED}
                                      TFM_CRYPTO_GENERATOR_MODULE_DISABLED=${CRYPTO_GENERATOR_MODULE_DISABLED}
                                      TFM_CRYPTO_ASYMMETRIC_MODULE_DISABLED=${CRYPTO_ASYMMETRIC_MODULE_DISABLED})
if (DEFINED CRYPTO_ENGINE_BUF_SIZE)
  list(APPEND TFM_CRYPTO_C_DEFINES_LIST TFM_CRYPTO_ENGINE_BUF_SIZE=${CRYPTO_ENGINE_BUF_SIZE})
endif()
if (DEFINED CRYPTO_CONC_OPER_NUM)
  list(APPEND TFM_CRYPTO_C_DEFINES_LIST TFM_CRYPTO_CONC_OPER_NUM=${CRYPTO_CONC_OPER_NUM})
endif()
if (TFM_PSA_API AND DEFINED CRYPTO_IOVEC_BUFFER_SIZE)
  list(APPEND TFM_CRYPTO_C_DEFINES_LIST TFM_CRYPTO_IOVEC_BUFFER_SIZE=${CRYPTO_IOVEC_BUFFER_SIZE})
endif()

if (CRYPTO_ENGINE_MBEDTLS)
  #Set Mbed Crypto compiler flags
  set(MBEDTLS_C_FLAGS ${MBEDTLS_C_FLAGS_SERVICES})
  string(APPEND MBEDTLS_C_FLAGS " -DMBEDTLS_CONFIG_FILE=\\\\\\\"tfm_mbedcrypto_config.h\\\\\\\""
                                " -I${CMAKE_CURRENT_LIST_DIR}")

  #Build Mbed Crypto as external project.
  #This ensures Mbed Crypto is built with exactly defined settings.
  #Mbed Crypto will be used from its install location
  include(${TFM_ROOT_DIR}/BuildMbedCrypto.cmake)
endif()

#Specify what we build (for the crypto service, build as a static library)
add_library(tfm_crypto STATIC ${ALL_SRC_ASM} ${ALL_SRC_C})
embedded_set_target_compile_defines(TARGET tfm_crypto LANGUAGE C DEFINES ${TFM_CRYPTO_C_DEFINES_LIST})
if (CRYPTO_ENGINE_MBEDTLS)
  #Add a dependency on the Mbed Crypto install target.
  add_dependencies(tfm_crypto ${MBEDTLS_TARGET_NAME}_install)
  #Ask the compiler to merge the Mbed Crypto and crypto service libraries.
  compiler_merge_library(DEST tfm_crypto LIBS "${MBEDTLS_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX_C}mbedcrypto${CMAKE_STATIC_LIBRARY_SUFFIX_C}")
endif()

#Set common compiler and linker flags
config_setting_shared_compiler_flags(tfm_crypto)
config_setting_shared_linker_flags(tfm_crypto)

embedded_project_end(tfm_crypto)
