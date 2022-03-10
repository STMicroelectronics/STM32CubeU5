#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project("CC312 Runtime Library" LANGUAGES C ASM)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

if (NOT DEFINED CC312_LIB_TARGET)
    set(CC312_LIB_TARGET cc312)
endif()
if (NOT DEFINED CC312_PAL_TARGET)
    set(CC312_PAL_TARGET cc312_pal)
endif()
if (NOT DEFINED CC312_CDMPU_TARGET)
    set(CC312_CDMPU_TARGET cc312_cdmpu)
endif()

add_library(${CC312_LIB_TARGET} STATIC)
# The extra libraries don't change much, so if we've already got one just ignore
# it. it's still possible to create multiple targets by altering the name
# variable.
if(NOT TARGET ${CC312_PAL_TARGET})
    add_library(${CC312_PAL_TARGET} INTERFACE)
endif()
if(NOT TARGET ${CC312_CDMPU_TARGET})
    add_library(${CC312_CDMPU_TARGET} INTERFACE)
endif()

add_subdirectory(codesafe)
add_subdirectory(host)
add_subdirectory(shared)

########################## LIB #################################################

target_compile_definitions(${CC312_LIB_TARGET}
    PRIVATE
        DLLI_MAX_BUFF_SIZE=0x10000
        USE_MBEDTLS_CRYPTOCELL
        CC_MNG_MIN_BACKUP_SIZE_IN_BYTES=512
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:CC_PAL_MAX_LOG_LEVEL=3>
    PUBLIC
        CC_IOT
)
target_link_libraries(${CC312_LIB_TARGET}
    PRIVATE
        ${CC312_PAL_TARGET}
)

########################## PAL #################################################

target_compile_definitions(${CC312_PAL_TARGET}
    INTERFACE
        CC_TRNG_MODE=0
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:CC_PAL_MAX_LOG_LEVEL=3>
        CC_IOT
)

########################## CDMPU ###############################################

target_link_libraries(${CC312_CDMPU_TARGET}
    INTERFACE
        ${CC312_PAL_TARGET}
)

target_compile_definitions(${CC312_CDMPU_TARGET}
    INTERFACE
        DLLI_MAX_BUFF_SIZE=0x10000
        USE_MBEDTLS_CRYPTOCELL
        CC_MNG_MIN_BACKUP_SIZE_IN_BYTES=512
        CC_IOT
)
