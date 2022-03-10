#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# A CMake script which is meant to filter some wanted symbols based on a template file
# from another text file, which contains all the extracted symbols from an executable.
#
# INPUT parameters:
#   SHARED_SYMBOL_TEMPLATE - Text file contains wanted symbol name templates to be shared (i.e.: mbedtls_)
#   ALL_SYMBOLS        - Text file, which contains all the extracted symbols from an executable.
#                        Produced in previous step of the code sharing process.
#
# OUTPUTS produced by this script:
#   - shared_symbols_addr.txt  List of the name, type and absolute address of symbols which
#                              match with the patterns in the symbol template file
#   - shared_symbols_name.txt  List of only the names of symbols which match with the
#                              patterns in the symbol template file

file(STRINGS ${SHARED_SYMBOL_TEMPLATE} SHARED_SYMBOL_TEMPLATE)
file(STRINGS ${ALL_SYMBOLS}        ALL_SYMBOLS)

# In 'arm-none-eabi-nm' and 'armclang --symdefs' output 'T' indicates the global
# symbols which can be shared between independently linked executables.
# 'D' is necessary to share two specific function pointers: 'mbedtls_calloc_func'
# and 'mbedtls_free_func'. These also need the 'static' keyword removed in mbed-crypto
set(_SHARED_SYMBOL_TYPES "T;D")

foreach(_SYMBOL_TEMPLATE IN LISTS SHARED_SYMBOL_TEMPLATE)
    string(SUBSTRING _SYMBOL_TEMPLATE 0 1 FIRST_CHAR)
    if (NOT _SYMBOL_TEMPLATE STREQUAL "" AND NOT FIRST_CHAR STREQUAL "#")
        foreach(_ONE_SYMBOL IN LISTS ALL_SYMBOLS)
            foreach(_TEXT IN LISTS _SHARED_SYMBOL_TYPES)
                string(FIND ${_ONE_SYMBOL} "${_TEXT} ${_SYMBOL_TEMPLATE}" POSITION)
                if (NOT POSITION EQUAL -1)
                    # Get matching symbol name and its address
                    list(APPEND SHARED_SYMBOL_ADDR_LIST "${_ONE_SYMBOL}")

                    # Get matching symbol name
                    string(SUBSTRING ${_ONE_SYMBOL} ${POSITION} 200 _ONE_SYMBOL_NAME)
                    string(REPLACE "${_TEXT} " "" _ONE_SYMBOL_NAME ${_ONE_SYMBOL_NAME})
                    list(APPEND SHARED_SYMBOL_NAME_LIST "${_ONE_SYMBOL_NAME}")
                endif()
            endforeach()
        endforeach()
    endif()
endforeach()

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shared_symbols_addr.txt "#<SYMDEFS>#\n")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shared_symbols_name.txt "")

foreach(_SYMBOL IN LISTS SHARED_SYMBOL_ADDR_LIST)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/shared_symbols_addr.txt "${_SYMBOL}\n")
endforeach()

foreach(_SYMBOL IN LISTS SHARED_SYMBOL_NAME_LIST)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/shared_symbols_name.txt "${_SYMBOL}\n")
endforeach()
