#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

set(TFM_VERSION 1.3.0)

############################ CONFIGURATION #####################################

if (IS_ABSOLUTE "${TFM_PLATFORM}")
    file(RELATIVE_PATH TFM_PLATFORM_RELATIVE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target"
        ${TFM_PLATFORM})
    set(TFM_PLATFORM "${TFM_PLATFORM_RELATIVE_PATH}" CACHE STRING "Target platform set as an absolute path." FORCE)
endif()

# Some compiler flags depend on the CPU / platform config. This include should
# be run before the toolchain file so the compiler can be configured properly.
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload.cmake)
    Message(FATAL_ERROR "Unsupported TFM_PLATFORM ${TFM_PLATFORM}")
else()
    include(platform/ext/target/${TFM_PLATFORM}/preload.cmake)
endif()

if(TFM_SYSTEM_FP)
    message(FATAL_ERROR "Hardware FPU is currently not supported in TF-M")
endif()
if(TFM_SYSTEM_MVE)
    message(FATAL_ERROR "Hardware MVE is currently not supported in TF-M")
endif()
if(TFM_SYSTEM_DSP)
    message(FATAL_ERROR "Hardware DSP is currently not supported in TF-M")
endif()

include(config/set_config.cmake)

if(NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" AND
   NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
    Message(FATAL_ERROR "Unsupported generator ${CMAKE_GENERATOR}. Hint: Try -G\"Unix Makefiles\"")
endif()

# The default build type is release. If debug symbols are needed then
# -DCMAKE_BUILD_TYPE=debug should be used (likewise with other build types)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

############################### Compiler configuration #########################

#Legacy compat option - load CMAKE_TOOLCHAIN_FILE as a TFM_TOOLCHAIN_FILE
if (CMAKE_TOOLCHAIN_FILE)
    message(DEPRECATION "SETTING CMAKE_TOOLCHAIN_FILE is deprecated. It has been replaced with TFM_TOOLCHAIN_FILE.")
    message(DEPRECATION "INTERPRETING -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} as -DTFM_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    set(TFM_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE})
    unset(CMAKE_TOOLCHAIN_FILE)
endif()

if (NOT IS_ABSOLUTE ${TFM_TOOLCHAIN_FILE})
    message(FATAL_ERROR "SETTING CMAKE_TOOLCHAIN_FILE no longer accepts relative paths. Please supply an absolute path or instead use TFM_TOOLCHAIN_FILE (which does accept relative paths)")
endif()

include(${TFM_TOOLCHAIN_FILE})
set(CMAKE_PROJECT_INCLUDE_BEFORE ${CMAKE_SOURCE_DIR}/cmake/disable_compiler_detection.cmake)

project("Trusted Firmware M" VERSION ${TFM_VERSION} LANGUAGES C ASM)
tfm_toolchain_reload_compiler()

############################ Config Check ######################################

include(${CMAKE_SOURCE_DIR}/config/check_config.cmake)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/check_config.cmake)
    include(platform/ext/target/${TFM_PLATFORM}/check_config.cmake)
endif()

################################################################################

add_subdirectory(lib/ext)
add_subdirectory(lib/fih)
add_subdirectory(tools)
add_subdirectory(docs)
if(NS)
    # Set to ${TFM_TEST_REPO_PATH}/app by default
    add_subdirectory(${TFM_APP_PATH} ${CMAKE_CURRENT_BINARY_DIR}/app)
    add_subdirectory(${TFM_NS_LOG_PATH} ${CMAKE_CURRENT_BINARY_DIR}/ns_log)
endif()
add_subdirectory(secure_fw)
add_subdirectory(interface)
if(BL2)
    add_subdirectory(bl2)
endif()
add_subdirectory(platform)

if(NS AND (TEST_S OR TEST_NS))
    # Set to ${TFM_TEST_REPO_PATH}/test by default
    add_subdirectory(${TFM_TEST_PATH} ${CMAKE_CURRENT_BINARY_DIR}/test)
endif()

include(cmake/install.cmake)

if(CRYPTO_HW_ACCELERATOR)
    add_subdirectory(platform/ext/accelerator)
endif()
