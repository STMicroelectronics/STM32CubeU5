#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)
find_package(Python3)

############################### Manifest declaration ###########################

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tfm_manifest_list.yaml
               ${CMAKE_CURRENT_BINARY_DIR}/tfm_manifest_list.yaml)

set(MANIFEST_LISTS ${CMAKE_CURRENT_BINARY_DIR}/tfm_manifest_list.yaml)
set(MANIFEST_LISTS ${MANIFEST_LISTS} ${TFM_EXTRA_MANIFEST_LIST_PATH})

if ("${TEST_PSA_API}" STREQUAL "IPC")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tfm_psa_ff_test_manifest_list.yaml
                   ${CMAKE_CURRENT_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)

    set(MANIFEST_LISTS ${MANIFEST_LISTS} ${CMAKE_CURRENT_BINARY_DIR}/tfm_psa_ff_test_manifest_list.yaml)
endif()

############################### File list declaration ##########################

set(GENERATED_FILE_LISTS ${CMAKE_CURRENT_SOURCE_DIR}/tfm_generated_file_list.yaml)
set(GENERATED_FILE_LISTS ${GENERATED_FILE_LISTS} ${TFM_EXTRA_GENERATED_FILE_LIST_PATH})

############################### Dependency generation ##########################

function(parse_field_from_yaml files field output_variable)
    set(${output_variable} "" PARENT_SCOPE)
    foreach(yaml_file ${files})
        # Load the lines that refer to the key we selected
        file(STRINGS ${yaml_file} temp_variable REGEX " *\"${field}\":")
        # Take only the value of the key
        list(TRANSFORM temp_variable REPLACE " *\"${field}\": *" ";")
        # Remove all commas
        list(TRANSFORM temp_variable REPLACE "," "")
        # Remove all quote marks
        list(TRANSFORM temp_variable REPLACE "\"" "")
        set(${output_variable} ${${output_variable}} ${temp_variable} PARENT_SCOPE)
    endforeach()
endfunction()

parse_field_from_yaml("${GENERATED_FILE_LISTS}" template TEMPLATE_FILES)
# Replace relative paths with absolute paths
list(TRANSFORM TEMPLATE_FILES REPLACE "^([^/\\][^:].*)" "${CMAKE_SOURCE_DIR}/\\1")

parse_field_from_yaml("${GENERATED_FILE_LISTS}" output OUTPUT_FILES)
# Replace relative paths with absolute paths
list(TRANSFORM OUTPUT_FILES REPLACE "^([^/\\][^:].*)" "${CMAKE_BINARY_DIR}/generated/\\1")

parse_field_from_yaml("${MANIFEST_LISTS}" manifest MANIFEST_FILES)
# Replace relative paths with absolute paths
list(TRANSFORM MANIFEST_FILES REPLACE "^([^/\\][^:].*)" "${CMAKE_SOURCE_DIR}/\\1")

############################### Command declaration ############################

# Workaround for heap support
if ("${TEST_PSA_API}" STREQUAL "IPC")
    execute_process(
        WORKING_DIRECTORY ${PSA_ARCH_TESTS_PATH}/api-tests
        COMMAND ${Python3_EXECUTABLE} tools/scripts/manifest_update.py
    )
endif()

add_custom_target(tfm_generated_files
    SOURCES ${OUTPUT_FILES}
)

add_custom_command(OUTPUT ${OUTPUT_FILES}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tfm_parse_manifest_list.py
                                  -m ${MANIFEST_LISTS}
                                  -f ${GENERATED_FILE_LISTS}
                                  -o ${CMAKE_BINARY_DIR}/generated
    DEPENDS ${TEMPLATE_FILES} ${MANIFEST_FILES}
    DEPENDS ${MANIFEST_LISTS}
)

# The files need to be generated before cmake will allow them to be used as
# sources. Due to issue with custom_command scoping the easiest way to do this
# is to run the script at cmake-time.
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tfm_parse_manifest_list.py
                                  -m ${MANIFEST_LISTS}
                                  -f ${GENERATED_FILE_LISTS}
                                  -o ${CMAKE_BINARY_DIR}/generated
    RESULT_VARIABLE RET
)

if(NOT RET EQUAL 0)
    message(FATAL_ERROR "File generation failed")
endif()
