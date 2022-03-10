#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# A CMake script to weaken identical symbols in the target linked libraries to avoid
# symbol collision at linking time between shared code and other libraries.
# i.e.: Shared cryptographic code between MCUBoot and secure runtime firmware.
#
# INPUT parameters:
#   LIB_LIST          - List of all libraries which are linked to the target, and are using
#                       the shared code.
#   SHARED_CODE_PATH  - The location of the shared code. It could be outside of TF-M repository.
#
# OUTPUTS produced by this script:
#   The libraries might be modified by this script, if they contain the same symbols
#   as the shared code.

# TODO: Library search path is modified manually to include path for platform
#       related libraries.

find_program(OBJCOPY arm-none-eabi-objcopy)
if (OBJCOPY STREQUAL "OBJCOPY-NOTFOUND")
    message(FATAL_ERROR "WeakenSymbols.cmake: mandatory tool 'arm-none-eabi-objcopy' is missing.")
endif()

# Macro to collect all libraries where an *.a file is found
macro(LIBRARY_DIRECTORIES return_list)
    file(GLOB_RECURSE new_list *.a)
    set(dir_list "")
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    set(${return_list} ${dir_list})
endmacro()

# Create a library search path for static libraries
LIBRARY_DIRECTORIES(LIBRARY_PATH)

# Workaround to include directories outside of 'secure_fw' folder for platform
list(APPEND LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/../platform/ext/accelerator/cc312/crypto_service_cc312 # Musca-B1: libcrypto_service_cc312.a
                         ${CMAKE_CURRENT_BINARY_DIR}/../platform/ext/accelerator
                         ${CMAKE_CURRENT_BINARY_DIR}/../platform
)

# When invoking the CMake scripts the original list separator(;) is replaced with space.
# Need to convert back to be able to handle as a list.
string(REPLACE " " ";" _LIB_LIST ${LIB_LIST})

# Want to weaken all shared symbols in one go, so first concatenate them.
# There are libraries which might not contain any of these, but it does
# not cause any issue, the command does not return with error code.
file(STRINGS ${SHARED_CODE_PATH}/shared_symbols_name.txt SHARED_SYMBOL_NAME)
foreach(_SYMBOL IN LISTS SHARED_SYMBOL_NAME)
    list(APPEND ARGUMENT "-W${_SYMBOL}")
endforeach()

# Iterate over each library and set potentially colliding symbols to be weak
foreach(LIB IN LISTS _LIB_LIST)
    find_file(LIB_FULL_PATH "lib${LIB}.a" PATHS ${LIBRARY_PATH} PATH_SUFFIXES Common NO_DEFAULT_PATH)
    if (NOT ${LIB_FULL_PATH} STREQUAL "LIB_FULL_PATH-NOTFOUND")
        execute_process(COMMAND ${OBJCOPY} ${ARGUMENT} ${LIB_FULL_PATH}
                        TIMEOUT 120
                        OUTPUT_VARIABLE _RES
                        ERROR_VARIABLE  _RES
                        RESULT_VARIABLE _STATUS_CODE
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (_STATUS_CODE GREATER 0)
            message(FATAL_ERROR "ERROR: Failed to execute ${OBJCOPY} ${_RES}")
        endif()
    endif()
    unset(LIB_FULL_PATH CACHE)
endforeach()
