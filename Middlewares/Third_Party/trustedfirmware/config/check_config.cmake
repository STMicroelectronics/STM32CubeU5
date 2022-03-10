#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

function(tfm_invalid_config)
    if (${ARGV})
        string (REPLACE ";" " " ARGV_STRING "${ARGV}")
        string (REPLACE "STREQUAL"     "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "GREATER"      ">" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "LESS"         "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "VERSION_LESS" "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "EQUAL"        "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "IN_LIST"      "in" ARGV_STRING "${ARGV_STRING}")

        message(FATAL_ERROR "INVALID CONFIG: ${ARGV_STRING}")
    endif()
endfunction()

tfm_invalid_config(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "7.3.1")

set (TFM_L3_PLATFORM_LISTS mps2/an521 musca_b1/sse_200 stm/stm32l562e_dk)

tfm_invalid_config(TFM_ISOLATION_LEVEL LESS 1 OR TFM_ISOLATION_LEVEL GREATER 3)
tfm_invalid_config(TFM_ISOLATION_LEVEL EQUAL 3 AND NOT TFM_PLATFORM IN_LIST TFM_L3_PLATFORM_LISTS)
tfm_invalid_config(TFM_ISOLATION_LEVEL GREATER 1 AND NOT TFM_PSA_API)

tfm_invalid_config(TFM_MULTI_CORE_TOPOLOGY AND NOT TFM_PSA_API)

tfm_invalid_config(TEST_S  AND TEST_PSA_API)
tfm_invalid_config(TEST_NS AND TEST_PSA_API)

tfm_invalid_config((TFM_PARTITION_PROTECTED_STORAGE AND PS_ROLLBACK_PROTECTION) AND NOT TFM_PARTITION_PLATFORM)
tfm_invalid_config(PS_ROLLBACK_PROTECTION AND NOT PS_ENCRYPTION)

tfm_invalid_config(SUITE STREQUAL "IPC" AND NOT TEST_PSA_API STREQUAL "IPC")

tfm_invalid_config(TEST_PSA_API STREQUAL "IPC" AND NOT TFM_PSA_API)
tfm_invalid_config(TEST_PSA_API STREQUAL "CRYPTO" AND NOT TFM_PARTITION_CRYPTO)
tfm_invalid_config(TEST_PSA_API STREQUAL "INITIAL_ATTESTATION" AND NOT TFM_PARTITION_INITIAL_ATTESTATION)
tfm_invalid_config(TEST_PSA_API STREQUAL "INTERNAL_TRUSTED_STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "PROTECTED_STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)

tfm_invalid_config(CRYPTO_HW_ACCELERATOR_OTP_STATE AND NOT CRYPTO_HW_ACCELERATOR)
tfm_invalid_config(CRYPTO_HW_ACCELERATOR_OTP_STATE AND NOT (CRYPTO_HW_ACCELERATOR_OTP_STATE STREQUAL "ENABLED" OR CRYPTO_HW_ACCELERATOR_OTP_STATE STREQUAL "PROVISIONING"))

########################## BL2 #################################################

get_property(MCUBOOT_STRATEGY_LIST CACHE MCUBOOT_UPGRADE_STRATEGY PROPERTY STRINGS)
tfm_invalid_config(NOT MCUBOOT_UPGRADE_STRATEGY IN_LIST MCUBOOT_STRATEGY_LIST)

####################### Code sharing ###########################################

set(TFM_CODE_SHARING_PLATFORM_LISTS mps2/an521 musca_b1/sse_200) # Without crypto hw acceleration
tfm_invalid_config(NOT TFM_CODE_SHARING STREQUAL "OFF" AND NOT TFM_PLATFORM IN_LIST TFM_CODE_SHARING_PLATFORM_LISTS)
tfm_invalid_config(NOT TFM_CODE_SHARING STREQUAL "OFF" AND CRYPTO_HW_ACCELERATOR)
tfm_invalid_config(TFM_CODE_SHARING STREQUAL "OFF" AND TFM_CODE_SHARING_PATH)

####################### SP META Pointer ########################################

tfm_invalid_config(TFM_SP_META_PTR_ENABLE AND NOT TFM_PSA_API)

####################### Firmware Update Parttion ###############################

tfm_invalid_config(TFM_PARTITION_FIRMWARE_UPDATE AND NOT TFM_PARTITION_PLATFORM)
tfm_invalid_config((MCUBOOT_UPGRADE_STRATEGY STREQUAL "DIRECT_XIP" OR MCUBOOT_UPGRADE_STRATEGY STREQUAL "RAM_LOAD") AND TFM_PARTITION_FIRMWARE_UPDATE)
tfm_invalid_config(TFM_PARTITION_FIRMWARE_UPDATE AND NOT MCUBOOT_DATA_SHARING)

########################## FIH #################################################

get_property(TFM_FIH_PROFILE_LIST CACHE TFM_FIH_PROFILE PROPERTY STRINGS)
tfm_invalid_config(NOT TFM_FIH_PROFILE IN_LIST TFM_FIH_PROFILE_LIST)

########################### TF-M audit log #####################################

tfm_invalid_config(TFM_PARTITION_AUDIT_LOG AND TFM_PSA_API)
