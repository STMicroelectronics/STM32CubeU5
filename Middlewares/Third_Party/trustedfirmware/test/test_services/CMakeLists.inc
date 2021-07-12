#-------------------------------------------------------------------------------
# Copyright (c) 2017-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#Definitions to compile the "secure services" module.
#This file assumes it will be included from a project specific cmakefile, and
#will not create a library or executable.
#Inputs:
#	TFM_ROOT_DIR - root directory of the TF-M repository.
#
#Outputs:
#	Will modify include directories to make the source compile.
#	ALL_SRC_C: C source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	ALL_SRC_CXX: C++ source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	ALL_SRC_ASM: assembly source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	Include directories will be modified by using the include_directories() commands as needed.

#Get the current directory where this file is located.
set(CORE_TEST_DIR ${CMAKE_CURRENT_LIST_DIR})

#Check input variables
if (NOT DEFINED TFM_ROOT_DIR)
	message(FATAL_ERROR "Please set TFM_ROOT_DIR before including this file.")
endif()

if (NOT DEFINED ENABLE_CORE_TESTS)
	message(FATAL_ERROR "Incomplete build configuration: ENABLE_CORE_TESTS is undefined. ")
elseif(ENABLE_CORE_TESTS)
	list(APPEND ALL_SRC_C_S "${CORE_TEST_DIR}/tfm_core_test/tfm_ss_core_test.c")
endif()

if (NOT DEFINED ENABLE_CORE_TESTS_2)
	message(FATAL_ERROR "Incomplete build configuration: ENABLE_CORE_TESTS_2 is undefined. ")
elseif(ENABLE_CORE_TESTS_2)
	list(APPEND ALL_SRC_C_S "${CORE_TEST_DIR}/tfm_core_test_2/tfm_ss_core_test_2.c")
endif()

if (NOT DEFINED ENABLE_IRQ_TEST_SERVICES)
	message(FATAL_ERROR "Incomplete build configuration: ENABLE_IRQ_TEST_SERVICES is undefined. ")
elseif(ENABLE_IRQ_TEST_SERVICES)
	list(APPEND ALL_SRC_C_S
		"${CORE_TEST_DIR}/tfm_irq_test_service_1/tfm_irq_test_service_1.c")
endif()

if (NOT DEFINED TFM_PARTITION_TEST_SECURE_SERVICES)
	message(FATAL_ERROR "Incomplete build configuration: TFM_PARTITION_TEST_SECURE_SERVICES is undefined. ")
elseif (TFM_PARTITION_TEST_SECURE_SERVICES)
	list(APPEND ALL_SRC_C_S "${CORE_TEST_DIR}/tfm_secure_client_service/tfm_secure_client_service.c")

	list(APPEND ALL_SRC_C_NS "${CORE_TEST_DIR}/tfm_secure_client_service/tfm_secure_client_service_api.c")
endif()

if (NOT DEFINED TFM_PARTITION_TEST_CORE_IPC)
	message(FATAL_ERROR "Incomplete build configuration: TFM_PARTITION_TEST_CORE_IPC is undefined. ")
elseif (TFM_PARTITION_TEST_CORE_IPC)
	list(APPEND ALL_SRC_C_S "${CORE_TEST_DIR}/tfm_ipc_service/tfm_ipc_service_test.c"
		"${CORE_TEST_DIR}/tfm_ipc_client/tfm_ipc_client_test.c"
		)
endif()

embedded_include_directories(PATH ${TFM_ROOT_DIR} ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/interface/include ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/platform/include ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/secure_fw/core/include ABSOLUTE)

set(BUILD_CMSIS_CORE Off)
set(BUILD_RETARGET Off)
set(BUILD_NATIVE_DRIVERS Off)
set(BUILD_STARTUP Off)
set(BUILD_TARGET_CFG Off)
set(BUILD_TARGET_HARDWARE_KEYS Off)
set(BUILD_TARGET_NV_COUNTERS Off)
set(BUILD_CMSIS_DRIVERS Off)
set(BUILD_UART_STDOUT Off)
set(BUILD_FLASH Off)
set(BUILD_BOOT_SEED Off)
set(BUILD_DEVICE_ID On)
if (CORE_TEST_POSITIVE)
	set(BUILD_PLAT_TEST On)
	set(BUILD_TIME On)
else()
	set(BUILD_PLAT_TEST Off)
	set(BUILD_TIME Off)
endif()
if(NOT DEFINED PLATFORM_CMAKE_FILE)
	message (FATAL_ERROR "Platform specific CMake is not defined. Please set PLATFORM_CMAKE_FILE.")
elseif(NOT EXISTS ${PLATFORM_CMAKE_FILE})
	message (FATAL_ERROR "Platform specific CMake \"${PLATFORM_CMAKE_FILE}\" file does not exist. Please fix value of PLATFORM_CMAKE_FILE.")
else()
	include(${PLATFORM_CMAKE_FILE})
endif()
