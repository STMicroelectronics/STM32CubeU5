#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# A CMake script to strip that part of an executable which is not meant to be
# shared among distinct binaries (code reuse). Only used by GNUARM tool chain.
#
# INPUT parameters:
#   SHARED_SYMBOLS_FILE  -  File which contains the list of shared symbols.
#   EXECUTABLE_TO_STRIP  -  A copy of the original executable, which contains the sharable code.
#                           From this copy of the executable the unshared code and symbols
#                           are removed.
#
# OUTPUTS produced by this script:
#   - EXECUTABLE_TO_STRIP  -  Output file (stripped) has the same name as input file.

find_program(GNUARM_STRIP arm-none-eabi-strip)
if (GNUARM_STRIP STREQUAL "GNUARM_STRIP-NOTFOUND")
    message(FATAL_ERROR "StripUnsharedCode.cmake: mandatory tool '${GNUARM_STRIP}' is missing.")
endif()

# Want to strip all unwanted symbols in one go, so concatenate those which must be kept
file(STRINGS ${SHARED_SYMBOLS_FILE} SHARED_SYMBOL_NAME)
foreach(_SYMBOL IN LISTS SHARED_SYMBOL_NAME)
    list(APPEND ARGUMENT "-K${_SYMBOL}")
endforeach()

execute_process(COMMAND ${GNUARM_STRIP} ${ARGUMENT} ${EXECUTABLE_TO_STRIP}
                TIMEOUT 120
                OUTPUT_VARIABLE _RES
                ERROR_VARIABLE  _RES
                RESULT_VARIABLE _STATUS_CODE
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if (_STATUS_CODE GREATER 0)
    message(FATAL_ERROR "ERROR: Failed to execute ${GNUARM_STRIP} ${_RES}")
endif()
