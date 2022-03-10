#------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(mcuboot_config INTERFACE)

set(MCUBOOT_KEY_ENC "${MCUBOOT_PATH}/enc-rsa2048-pub.pem" CACHE FILEPATH "Path to key with which to encrypt binary")

add_subdirectory("${MCUBOOT_PATH}/boot/bootutil" bootutil)

target_include_directories(bootutil
    PUBLIC
        include
)

target_link_libraries(bootutil
    PUBLIC
        mcuboot_config
        platform_bl2
        bl2_mbedcrypto
)

target_include_directories(mcuboot_config
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for mcuboot_config.h only
)

target_include_directories(bl2
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(bl2
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/bl2_main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/keys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_map_extended.c
        ${CMAKE_CURRENT_SOURCE_DIR}/flash_map_legacy.c
)

target_link_libraries(bl2
    PUBLIC
        bootutil
)

set(MCUBOOT_ALLOWED_LOG_LEVELS OFF ERROR WARNING INFO DEBUG)
list(FIND MCUBOOT_ALLOWED_LOG_LEVELS ${MCUBOOT_LOG_LEVEL} LOG_LEVEL_ID)

configure_file(include/mcuboot_config/mcuboot_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/mcuboot_config/mcuboot_config.h
               @ONLY)

############################### IMAGE SIGNING ##################################

find_package(Python3)

set(FLASH_AREA_NUM 0)
if (MCUBOOT_IMAGE_NUMBER GREATER 1)
    configure_file(signing_layout.c.in signing_layout_s.c @ONLY)
    add_library(signing_layout_s OBJECT ${CMAKE_CURRENT_BINARY_DIR}/signing_layout_s.c)
else()
    # Imgtool script requires the s_ns sufix. Since only one sigining layout is
    # used in this mode the signing_layout_s target's source file is renamed.
    configure_file(signing_layout.c.in signing_layout_s_ns.c @ONLY)
    add_library(signing_layout_s OBJECT ${CMAKE_CURRENT_BINARY_DIR}/signing_layout_s_ns.c)
endif()

target_compile_options(signing_layout_s
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-E\;-xc>
        $<$<C_COMPILER_ID:ARMClang>:-E\;-xc>
        $<$<C_COMPILER_ID:IAR>:--preprocess=ns\;$<TARGET_OBJECTS:signing_layout_s>>
)
target_compile_definitions(signing_layout_s
    PRIVATE
        $<$<BOOL:${BL2}>:BL2>
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
)
target_link_libraries(signing_layout_s
    PRIVATE
        platform_bl2
)

if(NS)
    add_custom_target(tfm_s_ns_bin
        SOURCES tfm_s_ns.bin
    )
    add_custom_command(OUTPUT tfm_s_ns.bin
        DEPENDS $<TARGET_FILE_DIR:tfm_s>/tfm_s.bin
        DEPENDS $<TARGET_FILE_DIR:tfm_ns>/tfm_ns.bin
        DEPENDS tfm_s_bin tfm_ns_bin
        DEPENDS signing_layout_s

        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/assemble.py
            --layout $<TARGET_OBJECTS:signing_layout_s>
            -s $<TARGET_FILE_DIR:tfm_s>/tfm_s.bin
            -n $<TARGET_FILE_DIR:tfm_ns>/tfm_ns.bin
            -o tfm_s_ns.bin
        COMMAND ${CMAKE_COMMAND} -E copy tfm_s_ns.bin $<TARGET_FILE_DIR:bl2>
    )
endif()

add_custom_target(tfm_s_signed_bin
    SOURCES tfm_s_signed.bin
)
add_custom_command(OUTPUT tfm_s_signed.bin
    DEPENDS $<TARGET_FILE_DIR:tfm_s>/tfm_s.bin
    DEPENDS tfm_s_bin signing_layout_s
    WORKING_DIRECTORY ${MCUBOOT_PATH}/scripts

    #Sign secure binary image with provided secret key
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/wrapper/wrapper.py
        -v ${MCUBOOT_IMAGE_VERSION_S}
        --layout $<TARGET_OBJECTS:signing_layout_s>
        -k ${MCUBOOT_KEY_S}
        --public-key-format $<IF:$<BOOL:${MCUBOOT_HW_KEY}>,full,hash>
        --align 1
        --pad
        --pad-header
        -H 0x400
        -s ${MCUBOOT_SECURITY_COUNTER_S}
        -d \"\(1,${MCUBOOT_NS_IMAGE_MIN_VER}\)\"
        $<$<STREQUAL:${MCUBOOT_UPGRADE_STRATEGY},OVERWRITE_ONLY>:--overwrite-only>
        $<$<BOOL:${MCUBOOT_ENC_IMAGES}>:-E${MCUBOOT_KEY_ENC}>
        $<TARGET_FILE_DIR:tfm_s>/tfm_s.bin
        ${CMAKE_CURRENT_BINARY_DIR}/tfm_s_signed.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/tfm_s_signed.bin $<TARGET_FILE_DIR:bl2>
)

set(FLASH_AREA_NUM 1)
configure_file(signing_layout.c.in signing_layout_ns.c @ONLY)

add_library(signing_layout_ns OBJECT ${CMAKE_CURRENT_BINARY_DIR}/signing_layout_ns.c)
target_compile_options(signing_layout_ns
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-E\;-xc>
        $<$<C_COMPILER_ID:ARMClang>:-E\;-xc>
        $<$<C_COMPILER_ID:IAR>:--preprocess=ns\;$<TARGET_OBJECTS:signing_layout_ns>>
)
target_compile_definitions(signing_layout_ns
    PRIVATE
        $<$<BOOL:${BL2}>:BL2>
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
)
target_link_libraries(signing_layout_ns
    PRIVATE
        platform_bl2
)

if(NS)
    add_custom_target(tfm_ns_signed_bin
        SOURCES tfm_ns_signed.bin
    )
    add_custom_command(OUTPUT tfm_ns_signed.bin
        DEPENDS $<TARGET_FILE_DIR:tfm_ns>/tfm_ns.bin
        DEPENDS tfm_ns_bin signing_layout_ns
        WORKING_DIRECTORY ${MCUBOOT_PATH}/scripts

        #Sign non-secure binary image with provided secret key
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/wrapper/wrapper.py
            -v ${MCUBOOT_IMAGE_VERSION_NS}
            --layout $<TARGET_OBJECTS:signing_layout_ns>
            -k ${MCUBOOT_KEY_NS}
            --public-key-format $<IF:$<BOOL:${MCUBOOT_HW_KEY}>,full,hash>
            --align 1
            --pad
            --pad-header
            -H 0x400
            -s ${MCUBOOT_SECURITY_COUNTER_NS}
            -d \"\(0, ${MCUBOOT_S_IMAGE_MIN_VER}\)\"
            $<TARGET_FILE_DIR:tfm_ns>/tfm_ns.bin
            $<$<STREQUAL:${MCUBOOT_UPGRADE_STRATEGY},OVERWRITE_ONLY>:--overwrite-only>
            $<$<BOOL:${MCUBOOT_ENC_IMAGES}>:-E${MCUBOOT_KEY_ENC}>
            ${CMAKE_CURRENT_BINARY_DIR}/tfm_ns_signed.bin
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/tfm_ns_signed.bin $<TARGET_FILE_DIR:bl2>
    )
endif()

if(NS)
    add_custom_target(tfm_s_ns_signed_bin
        SOURCES tfm_s_ns_signed.bin
    )
    if (MCUBOOT_IMAGE_NUMBER GREATER 1)
        add_custom_command(OUTPUT tfm_s_ns_signed.bin
            DEPENDS tfm_s_signed_bin $<TARGET_FILE_DIR:tfm_s>/tfm_s.bin
            DEPENDS tfm_ns_signed_bin $<TARGET_FILE_DIR:tfm_ns>/tfm_ns.bin
            DEPENDS signing_layout_s

            # Create concatenated binary image from the two independently signed
            # binary file. This only uses the local assemble.py script (not from
            # upstream mcuboot) because that script is geared towards zephyr
            # support
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/assemble.py
                --layout $<TARGET_OBJECTS:signing_layout_s>
                -s $<TARGET_FILE_DIR:bl2>/tfm_s_signed.bin
                -n $<TARGET_FILE_DIR:bl2>/tfm_ns_signed.bin
                -o tfm_s_ns_signed.bin
            COMMAND ${CMAKE_COMMAND} -E copy tfm_s_ns_signed.bin $<TARGET_FILE_DIR:bl2>
        )
    else()
        add_custom_command(OUTPUT tfm_s_ns_signed.bin
            DEPENDS tfm_s_ns_bin tfm_s_ns.bin
            DEPENDS signing_layout_s

        # Use the non-secure key to sign the combined image if FORWARD_PROT_MSG is set.
        # In such a configuration there is a subsystem with higher privileges controlling the
        # the boot process and current implementation requires to use the non-secure key here.
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/wrapper/wrapper.py
            -v ${MCUBOOT_IMAGE_VERSION_S}
            --layout $<TARGET_OBJECTS:signing_layout_s>
            -k $<IF:$<BOOL:${FORWARD_PROT_MSG}>,${MCUBOOT_KEY_NS},${MCUBOOT_KEY_S}>
            --public-key-format $<IF:$<BOOL:${MCUBOOT_HW_KEY}>,full,hash>
            --align 1
            --pad
            --pad-header
            -H 0x400
            -s ${MCUBOOT_SECURITY_COUNTER_S}
            $<$<STREQUAL:${MCUBOOT_UPGRADE_STRATEGY},OVERWRITE_ONLY>:--overwrite-only>
            $<$<BOOL:${MCUBOOT_ENC_IMAGES}>:-E${MCUBOOT_KEY_ENC}>
            tfm_s_ns.bin
            ${CMAKE_CURRENT_BINARY_DIR}/tfm_s_ns_signed.bin
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/tfm_s_ns_signed.bin $<TARGET_FILE_DIR:bl2>
        )
    endif()
endif()

add_custom_target(signed_images
    ALL
    DEPENDS $<IF:$<BOOL:${NS}>,tfm_s_ns_signed_bin,tfm_s_signed_bin>
)
