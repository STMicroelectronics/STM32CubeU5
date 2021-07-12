#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#Definitions to compile the "internal_trusted_storage" module.
#This file assumes it will be included from a project specific cmakefile, and
#will not create a library or executable.
#Inputs:
#    TFM_ROOT_DIR - root directory of the TF-M repository.
#Outputs:
#    Will modify include directories to make the source compile.
#    ALL_SRC_C: C source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#    ALL_SRC_CXX: C++ source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#    ALL_SRC_ASM: assembly source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#    Include directories will be modified by using the include_directories() commands as needed.

#Get the current directory where this file is located.
set(INTERNAL_TRUSTED_STORAGE_DIR ${CMAKE_CURRENT_LIST_DIR})

#Check input variables
if (NOT DEFINED TFM_ROOT_DIR)
    message(FATAL_ERROR "Please set TFM_ROOT_DIR before including this file.")
endif()

if (NOT DEFINED ITS_CREATE_FLASH_LAYOUT)
    message(FATAL_ERROR "Incomplete build configuration: ITS_CREATE_FLASH_LAYOUT is undefined. ")
endif()

if (NOT DEFINED ITS_VALIDATE_METADATA_FROM_FLASH)
    message(FATAL_ERROR "Incomplete build configuration: ITS_VALIDATE_METADATA_FROM_FLASH is undefined. ")
endif()

if (NOT DEFINED ITS_RAM_FS)
    message(FATAL_ERROR "Incomplete build configuration: ITS_RAM_FS is undefined. ")
endif()

set(INTERNAL_TRUSTED_STORAGE_C_SRC
    "${INTERNAL_TRUSTED_STORAGE_DIR}/tfm_its_secure_api.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/tfm_its_req_mngr.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/tfm_internal_trusted_storage.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/its_utils.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/flash/its_flash.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/flash_fs/its_flash_fs.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/flash_fs/its_flash_fs_dblock.c"
    "${INTERNAL_TRUSTED_STORAGE_DIR}/flash_fs/its_flash_fs_mblock.c"
)

if (ITS_VALIDATE_METADATA_FROM_FLASH)
    set_property(SOURCE ${INTERNAL_TRUSTED_STORAGE_C_SRC} APPEND PROPERTY COMPILE_DEFINITIONS ITS_VALIDATE_METADATA_FROM_FLASH)
endif()

if (ITS_CREATE_FLASH_LAYOUT)
    set_property(SOURCE ${INTERNAL_TRUSTED_STORAGE_C_SRC} APPEND PROPERTY COMPILE_DEFINITIONS ITS_CREATE_FLASH_LAYOUT)
endif()

if (ITS_RAM_FS)
    set_property(SOURCE ${INTERNAL_TRUSTED_STORAGE_C_SRC} APPEND PROPERTY COMPILE_DEFINITIONS ITS_RAM_FS)
endif()

#Append all our source files to global lists.
list(APPEND ALL_SRC_C ${INTERNAL_TRUSTED_STORAGE_C_SRC})
unset(INTERNAL_TRUSTED_STORAGE_C_SRC)

#Inform the user about ITS service features selected based on the ITS service CMake flags
message("The ITS service compile configuration is as follows:")
message("- ITS_VALIDATE_METADATA_FROM_FLASH: " ${ITS_VALIDATE_METADATA_FROM_FLASH})
message("- ITS_CREATE_FLASH_LAYOUT: " ${ITS_CREATE_FLASH_LAYOUT})
message("- ITS_RAM_FS: " ${ITS_RAM_FS})

#Setting include directories
embedded_include_directories(PATH ${TFM_ROOT_DIR} ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/interface/include ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/platform/ext/driver ABSOLUTE)
set(BUILD_CMSIS_CORE Off)
set(BUILD_RETARGET Off)
set(BUILD_NATIVE_DRIVERS Off)
set(BUILD_STARTUP Off)
set(BUILD_TARGET_CFG Off)
set(BUILD_TARGET_HARDWARE_KEYS Off)
set(BUILD_TARGET_NV_COUNTERS Off)
set(BUILD_CMSIS_DRIVERS Off)
set(BUILD_TIME Off)
set(BUILD_UART_STDOUT Off)
set(BUILD_FLASH Off)
set(BUILD_BOOT_SEED Off)
set(BUILD_DEVICE_ID Off)
if (NOT DEFINED PLATFORM_CMAKE_FILE)
    message (FATAL_ERROR "Platform specific CMake is not defined. Please set PLATFORM_CMAKE_FILE.")
elseif(NOT EXISTS ${PLATFORM_CMAKE_FILE})
    message (FATAL_ERROR "Platform specific CMake \"${PLATFORM_CMAKE_FILE}\" file does not exist. Please fix value of PLATFORM_CMAKE_FILE.")
else()
    include(${PLATFORM_CMAKE_FILE})
endif()
