#-------------------------------------------------------------------------------
# Copyright (c) 2018-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT DEFINED REGRESSION)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: REGRESSION not defined, Include this file from a Config*.cmake")
elseif(NOT DEFINED CORE_TEST)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: CORE_TEST not defined, Include this file from a Config*.cmake")
elseif(NOT DEFINED TFM_LVL)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: TFM_LVL not defined, Include this file from a Config*.cmake")
elseif(NOT DEFINED CORE_IPC)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: CORE_IPC not deinfed. Include this file from a Config*.cmake")
endif()

if(NOT DEFINED COMPILER)
	message(FATAL_ERROR "ERROR: COMPILER is not set in command line")
elseif((NOT ${COMPILER} STREQUAL "ARMCLANG") AND (NOT ${COMPILER} STREQUAL "GNUARM"))
	message(FATAL_ERROR "ERROR: Compiler \"${COMPILER}\" is not supported.")
endif()

#Configure the default build type
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (i.e. Debug)")

if(CORE_IPC)
	if (TFM_LVL EQUAL 3)
		message(FATAL_ERROR "ERROR: Invalid isolation level!")
	endif()
else()
	if(NOT TFM_LVL EQUAL 1)
		message(FATAL_ERROR "ERROR: Invalid isolation level!")
	endif()
endif()

#BL2 bootloader (MCUBoot) related settings
include(${CMAKE_CURRENT_LIST_DIR}/bl2/ext/mcuboot/MCUBootConfig.cmake)

set(BUILD_CMSIS_CORE Off)
set(BUILD_RETARGET Off)
set(BUILD_NATIVE_DRIVERS Off)
set(BUILD_TIME Off)
set(BUILD_STARTUP Off)
set(BUILD_TARGET_CFG Off)
set(BUILD_TARGET_HARDWARE_KEYS Off)
set(BUILD_TARGET_NV_COUNTERS Off)
set(BUILD_CMSIS_DRIVERS Off)
set(BUILD_UART_STDOUT Off)
set(BUILD_FLASH Off)
set(BUILD_BOOT_SEED Off)
set(BUILD_DEVICE_ID Off)
set(BUILD_PLAT_TEST Off)
set(BUILD_BOOT_HAL Off)
if(NOT DEFINED PLATFORM_CMAKE_FILE)
	message (FATAL_ERROR "Platform specific CMake is not defined. Please set PLATFORM_CMAKE_FILE.")
elseif(NOT EXISTS ${PLATFORM_CMAKE_FILE})
	message (FATAL_ERROR "Platform specific CMake \"${PLATFORM_CMAKE_FILE}\" file does not exist. Please fix value of PLATFORM_CMAKE_FILE.")
else()
	include(${PLATFORM_CMAKE_FILE})
endif()

# Select the corresponding CPU type and configuration according to current
# building status in multi-core scenario.
# The updated configuration will be used in following compiler setting.
if (DEFINED TFM_MULTI_CORE_TOPOLOGY AND TFM_MULTI_CORE_TOPOLOGY)
	include("Common/MultiCore")

	if (NOT DEFINED TFM_BUILD_IN_SPE)
		message(FATAL_ERROR "Flag of building in SPE is not specified. Please set TFM_BUILD_IN_SPE.")
	else()
		select_arm_cpu_type(${TFM_BUILD_IN_SPE})
	endif()
endif()

if (DEFINED TFM_MULTI_CORE_TOPOLOGY AND TFM_MULTI_CORE_TOPOLOGY)
	# CMSE is unnecessary in multi-core scenarios.
	# TODO: Need further discussion about if CMSE is required when an Armv8-M
	# core acts as secure core in multi-core scenario.
	set (CMSE_FLAGS "")
	set (ARM_FEATURE_CMSE 0)
else()
	set (CMSE_FLAGS "-mcmse")
	set (ARM_FEATURE_CMSE 3)
endif()

if(${COMPILER} STREQUAL "ARMCLANG")
	#Use any ARMCLANG version found on PATH. Note: Only versions supported by the
	#build system will work. A file cmake/Common/CompilerArmClangXY.cmake
	#must be present with a matching version.
	include("Common/FindArmClang")
	include("Common/${ARMCLANG_MODULE}")

	set (COMMON_COMPILE_FLAGS -fshort-enums -fshort-wchar -funsigned-char -mfpu=none ${CMSE_FLAGS} -ffunction-sections -fdata-sections)
	##Shared compiler settings.
	function(config_setting_shared_compiler_flags tgt)
		embedded_set_target_compile_flags(TARGET ${tgt} LANGUAGE C FLAGS -xc -std=c99 ${COMMON_COMPILE_FLAGS} -Wall -Werror)
	endfunction()

	##Shared linker settings.
	function(config_setting_shared_linker_flags tgt)
		embedded_set_target_link_flags(TARGET ${tgt} FLAGS --strict --map --symbols --xref --entry=Reset_Handler --remove --info=summarysizes,sizes,totals,unused,veneers)
	endfunction()
elseif(${COMPILER} STREQUAL "GNUARM")
	#Use any GNUARM version found on PATH. Note: Only versions supported by the
	#build system will work. A file cmake/Common/CompilerGNUARMXY.cmake
	#must be present with a matching version.
	include("Common/FindGNUARM")
	include("Common/${GNUARM_MODULE}")

	set (COMMON_COMPILE_FLAGS -fshort-enums -fshort-wchar -funsigned-char -msoft-float ${CMSE_FLAGS} -ffunction-sections -fdata-sections --specs=nano.specs)
	##Shared compiler and linker settings.
	function(config_setting_shared_compiler_flags tgt)
		embedded_set_target_compile_flags(TARGET ${tgt} LANGUAGE C FLAGS -xc -std=c99 ${COMMON_COMPILE_FLAGS} -Wall -Werror -Wno-format -Wno-return-type -Wno-unused-but-set-variable)
	endfunction()

	##Shared linker settings.
	function(config_setting_shared_linker_flags tgt)
		#--no-wchar-size-warning flag is added because TF-M sources are compiled
		#with short wchars, however the standard library is compiled with normal
		#wchar, and this generates linker time warnings. TF-M code does not use
		#wchar, so the warning can be suppressed.
		embedded_set_target_link_flags(TARGET ${tgt} FLAGS -Wl,-check-sections,-fatal-warnings,--gc-sections,--no-wchar-size-warning,--print-memory-usage --entry=Reset_Handler --specs=nano.specs)
	endfunction()
endif()

#Create a string from the compile flags list, so that it can be used later
#in this file to set mbedtls and BL2 flags
list_to_string(COMMON_COMPILE_FLAGS_STR ${COMMON_COMPILE_FLAGS})

#Settings which shall be set for all projects the same way based
# on the variables above.
set (TFM_PARTITION_TEST_CORE OFF)
set (TFM_PARTITION_TEST_CORE_IPC OFF)
set (CORE_TEST_POSITIVE OFF)
set (CORE_TEST_INTERACTIVE OFF)
set (REFERENCE_PLATFORM OFF)
set (TFM_PARTITION_TEST_SECURE_SERVICES OFF)
set (SERVICES_TEST_ENABLED OFF)
set (TEST_FRAMEWORK_S  OFF)
set (TEST_FRAMEWORK_NS OFF)
set (TFM_PSA_API OFF)
set (TFM_LEGACY_API ON)

option(TFM_PARTITION_AUDIT_LOG "Enable the TF-M Audit Log partition" ON)
option(TFM_PARTITION_PLATFORM "Enable the TF-M Platform partition" ON)

if(${TARGET_PLATFORM} STREQUAL "AN521" OR ${TARGET_PLATFORM} STREQUAL "AN519" OR ${TARGET_PLATFORM} STREQUAL "AN539")
	set (REFERENCE_PLATFORM ON)
endif()

# Option to demonstrate usage of secure-only peripheral
set (SECURE_UART1 OFF)

if (REGRESSION)
	set(SERVICES_TEST_ENABLED ON)
endif()

if (CORE_IPC)
	set(TFM_PSA_API ON)

	# Disable IPC Test by default if the config or platform doesn't explicitly
	# require it
	if (NOT DEFINED IPC_TEST)
		set(IPC_TEST OFF)
	endif()
else()
	set(IPC_TEST OFF)
endif()

if (TFM_PSA_API)
	add_definitions(-DTFM_PSA_API)
endif()

if (TFM_LEGACY_API)
	add_definitions(-DTFM_LEGACY_API)
endif()

if (SERVICES_TEST_ENABLED)
	set(SERVICE_TEST_S ON)
	set(SERVICE_TEST_NS ON)
endif()

if (CORE_TEST)
	set(CORE_TEST_POSITIVE ON)
	set(CORE_TEST_INTERACTIVE OFF)
endif()

if (CORE_TEST_INTERACTIVE)
	add_definitions(-DCORE_TEST_INTERACTIVE)
	set(TEST_FRAMEWORK_NS ON)
	set(TFM_PARTITION_TEST_CORE ON)
endif()

if (CORE_TEST_POSITIVE)
	add_definitions(-DCORE_TEST_POSITIVE)
	set(TEST_FRAMEWORK_NS ON)
	set(TFM_PARTITION_TEST_CORE ON)
endif()

if (TFM_PARTITION_TEST_CORE)
	# If the platform or the topology doesn't specify whether IRQ test is
	# supported, enable it by default.
	if (NOT DEFINED TFM_ENABLE_IRQ_TEST)
		set(TFM_ENABLE_IRQ_TEST ON)
	endif()

	if (TFM_ENABLE_IRQ_TEST)
		add_definitions(-DTFM_ENABLE_IRQ_TEST)
	endif()
else()
	set(TFM_ENABLE_IRQ_TEST OFF)
endif()

if (IPC_TEST)
	add_definitions(-DENABLE_IPC_TEST)
	set(TEST_FRAMEWORK_NS ON)
	set(TFM_PARTITION_TEST_CORE_IPC ON)
endif()

if (SERVICE_TEST_S)
	add_definitions(-DSERVICES_TEST_S)
	set(TEST_FRAMEWORK_S ON)
endif()

if (SERVICE_TEST_NS)
	add_definitions(-DSERVICES_TEST_NS)
	set(TEST_FRAMEWORK_NS ON)
endif()

if (TEST_FRAMEWORK_S)
	add_definitions(-DTEST_FRAMEWORK_S)
	# The secure client partition is required to run secure tests
	set(TFM_PARTITION_TEST_SECURE_SERVICES ON)
endif()

if (TEST_FRAMEWORK_NS)
	add_definitions(-DTEST_FRAMEWORK_NS)
endif()

if (CORE_IPC)
	set(TFM_PARTITION_AUDIT_LOG OFF)
	set(TFM_PARTITION_PLATFORM OFF)
endif()

if (TFM_PARTITION_AUDIT_LOG)
	add_definitions(-DTFM_PARTITION_AUDIT_LOG)
endif()

if (TFM_PARTITION_PLATFORM)
	add_definitions(-DTFM_PARTITION_PLATFORM)
endif()

if (TFM_PARTITION_TEST_CORE)
	add_definitions(-DTFM_PARTITION_TEST_CORE)
endif()

if (TFM_PARTITION_TEST_CORE_IPC)
	add_definitions(-DTFM_PARTITION_TEST_CORE_IPC)
endif()

if (TFM_PARTITION_TEST_SECURE_SERVICES)
	add_definitions(-DTFM_PARTITION_TEST_SECURE_SERVICES)
endif()

if (PSA_API_TEST)
	add_definitions(-DPSA_API_TEST_NS)
	set(PSA_API_TEST_NS ON)
	if (NOT DEFINED PSA_API_TEST_CRYPTO)
		set(PSA_API_TEST_CRYPTO OFF)
	endif()
	if (NOT DEFINED PSA_API_TEST_INTERNAL_TRUSTED_STORAGE)
		set(PSA_API_TEST_INTERNAL_TRUSTED_STORAGE OFF)
	endif()
	if (NOT DEFINED PSA_API_TEST_SECURE_STORAGE)
		set(PSA_API_TEST_SECURE_STORAGE OFF)
	endif()
	if (NOT DEFINED PSA_API_TEST_ATTESTATION)
		set(PSA_API_TEST_ATTESTATION OFF)
	endif()
endif()

# This flag indicates if the non-secure OS is capable of identify the non-secure clients
# which call the secure services. It is diabled in IPC model.
if (NOT DEFINED TFM_NS_CLIENT_IDENTIFICATION)
	if (TFM_PSA_API)
		set(TFM_NS_CLIENT_IDENTIFICATION OFF)
	else()
		set(TFM_NS_CLIENT_IDENTIFICATION ON)
	endif()
endif()

if (BL2)
	# Add MCUBOOT_IMAGE_NUMBER definition to the compiler command line.
	add_definitions(-DMCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER})

	if (${MCUBOOT_UPGRADE_STRATEGY} STREQUAL "NO_SWAP")
		set(LINK_TO_BOTH_MEMORY_REGION ON)
	endif()
endif()

##Set Mbed TLS compiler flags and variables for audit log and crypto
set(MBEDTLS_C_FLAGS_SERVICES "-D__ARM_FEATURE_CMSE=${ARM_FEATURE_CMSE} -D__thumb2__ ${COMMON_COMPILE_FLAGS_STR} -I${CMAKE_CURRENT_LIST_DIR}/platform/ext/common")

#Default TF-M secure storage flags.
#These flags values can be overwritten by setting them in platform/ext/<TARGET_NAME>.cmake
#Documentation about these flags can be found in docs/user_guides/services/tfm_sst_integration_guide.rst
if (NOT DEFINED SST_ENCRYPTION)
	set (SST_ENCRYPTION ON)
endif()

if (NOT DEFINED SST_ROLLBACK_PROTECTION)
	set (SST_ROLLBACK_PROTECTION OFF)
endif()

if (NOT DEFINED SST_CREATE_FLASH_LAYOUT)
	set (SST_CREATE_FLASH_LAYOUT OFF)
endif()

if (NOT DEFINED SST_VALIDATE_METADATA_FROM_FLASH)
	set (SST_VALIDATE_METADATA_FROM_FLASH ON)
endif()

if (NOT DEFINED SST_RAM_FS)
	if (REGRESSION)
		set (SST_RAM_FS ON)
	else()
		set (SST_RAM_FS OFF)
	endif()
endif()

if (NOT DEFINED SST_TEST_NV_COUNTERS)
	if (REGRESSION AND (TFM_LVL EQUAL 1))
		set(SST_TEST_NV_COUNTERS ON)
	else()
		set(SST_TEST_NV_COUNTERS OFF)
	endif()
endif()

#Default TF-M internal trusted storage flags.
#These flags values can be overwritten by setting them in platform/ext/<TARGET_NAME>.cmake
#Documentation about these flags can be found in the TF-M ITS integration guide
option(ITS_CREATE_FLASH_LAYOUT "Create an empty ITS Flash Layout" OFF)

if (NOT DEFINED ITS_VALIDATE_METADATA_FROM_FLASH)
	set (ITS_VALIDATE_METADATA_FROM_FLASH ON)
endif()

if (NOT DEFINED ITS_RAM_FS)
	if (REGRESSION)
		set (ITS_RAM_FS ON)
	else()
		set (ITS_RAM_FS OFF)
	endif()
endif()

if (NOT DEFINED MBEDTLS_DEBUG)
	set(MBEDTLS_DEBUG OFF)
endif()

#Default TF-M initial-attestation service flags.
#Documentation about these flags can be found in docs/user_guides/services/tfm_attestation_integration_guide.rst
if (NOT DEFINED ATTEST_INCLUDE_OPTIONAL_CLAIMS)
	set(ATTEST_INCLUDE_OPTIONAL_CLAIMS ON)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ATTEST_INCLUDE_TEST_CODE_AND_KEY_ID ON)
else()
	set(ATTEST_INCLUDE_TEST_CODE_AND_KEY_ID OFF)
endif()

##Set mbedTLS compiler flags for BL2 bootloader
set(MBEDTLS_C_FLAGS_BL2 "-D__ARM_FEATURE_CMSE=${ARM_FEATURE_CMSE} -D__thumb2__ ${COMMON_COMPILE_FLAGS_STR} -DMBEDTLS_CONFIG_FILE=\\\\\\\"config-boot.h\\\\\\\" -I${CMAKE_CURRENT_LIST_DIR}/bl2/ext/mcuboot/include")
if (MCUBOOT_SIGNATURE_TYPE STREQUAL "RSA-3072")
	string(APPEND MBEDTLS_C_FLAGS_BL2 " -DMCUBOOT_SIGN_RSA_LEN=3072")
endif()
