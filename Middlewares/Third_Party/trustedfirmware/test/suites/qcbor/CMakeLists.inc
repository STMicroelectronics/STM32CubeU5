#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#Definitions to compile the QCBOR module.
#This file assumes it will be included from a project specific cmakefile, and
#will not create a library or executable.
#Inputs:
#	TFM_ROOT_DIR - root directory of the TF-M repo.
#
#Outputs:
#	Will modify include directories to make the source compile.
#	ALL_SRC_C: C source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	ALL_SRC_CXX: C++ source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	ALL_SRC_ASM: assembly source files to be compiled will be added to this list. This shall be added to your add_executable or add_library command.
#	Include directories will be modified by using the include_directories() commands as needed.

#Get the current directory where this file is located.
set(QCBOR_TEST_SUIT_DIR ${CMAKE_CURRENT_LIST_DIR})

#Verify input parameters
if(NOT DEFINED TFM_ROOT_DIR)
	message(FATAL_ERROR "Please set TFM_ROOT_DIR before including this file.")
endif()

#Get the directory where test cases are stored
set(QCBOR_TEST_DIR "${TFM_ROOT_DIR}/lib/ext/qcbor/test")

if (NOT DEFINED ENABLE_QCBOR_TESTS)
	message(FATAL_ERROR "Incomplete build configuration: ENABLE_QCBOR_TESTS is undefined.")
elseif(ENABLE_QCBOR_TESTS)
	list(APPEND ALL_SRC_C_NS
		"${QCBOR_TEST_SUIT_DIR}/non_secure/qcbor_ns_testsuite.c"
		"${QCBOR_TEST_DIR}/float_tests.c"
		"${QCBOR_TEST_DIR}/half_to_double_from_rfc7049.c"
		"${QCBOR_TEST_DIR}/qcbor_decode_tests.c"
		"${QCBOR_TEST_DIR}/qcbor_encode_tests.c"
		"${QCBOR_TEST_DIR}/run_tests.c"
		"${QCBOR_TEST_DIR}/UsefulBuf_Tests.c"
	)

	#Setting include directories
	embedded_include_directories(PATH ${TFM_ROOT_DIR} ABSOLUTE)
	embedded_include_directories(PATH ${TFM_ROOT_DIR}/lib/ext/qcbor/inc ABSOLUTE)
	embedded_include_directories(PATH ${QCBOR_TEST_DIR} ABSOLUTE)
endif()
