#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_TOOLCHAIN_FILE                  ${CMAKE_SOURCE_DIR}/toolchain_GNUARM.cmake CACHE FILEPATH    "Path to TFM compiler toolchain file")
set(TFM_PLATFORM                        ""          CACHE STRING    "Platform to build TF-M for. Must be either a relative path from [TF-M]/platform/ext/target, or an absolute path.")
set(CROSS_COMPILE                       arm-none-eabi CACHE STRING  "Cross-compilation triplet")

set(BL2                                 ON          CACHE BOOL      "Whether to build BL2")
set(NS                                  ON          CACHE BOOL      "Whether to build NS app")

set(TEST_S                              OFF         CACHE BOOL      "Whether to build S regression tests")
set(TEST_NS                             OFF         CACHE BOOL      "Whether to build NS regression tests")
set(TEST_PSA_API                        ""          CACHE STRING    "Which (if any) of the PSA API tests should be compiled")

set(TFM_PSA_API                         OFF         CACHE BOOL      "Use PSA api (IPC mode) instead of secure library mode")
set(TFM_ISOLATION_LEVEL                 1           CACHE STRING    "Isolation level")
set(TFM_PROFILE                         ""          CACHE STRING    "Profile to use")
set(TFM_FIH_PROFILE                     OFF         CACHE STRING    "Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]")

set(TFM_NS_CLIENT_IDENTIFICATION        OFF         CACHE BOOL      "Enable NS client identification")

set(TFM_EXTRA_CONFIG_PATH               ""          CACHE PATH      "Path to extra cmake config file")
set(TFM_EXTRA_MANIFEST_LIST_PATH        ""          CACHE PATH      "Path to extra manifest file, used to declare extra partitions. Appended to standard TFM manifest")
set(TFM_EXTRA_GENERATED_FILE_LIST_PATH  ""          CACHE PATH      "Path to extra generated file list. Appended to stardard TFM generated file list.")

set(TFM_SPM_LOG_LEVEL                   TFM_SPM_LOG_LEVEL_INFO          CACHE STRING    "Set default SPM log level as INFO level")
set(TFM_PARTITION_LOG_LEVEL             TFM_PARTITION_LOG_LEVEL_INFO    CACHE STRING    "Set default Secure Partition log level as INFO level")

set(TFM_CODE_SHARING                    OFF         CACHE PATH      "Enable code sharing between MCUboot and secure firmware")
set(TFM_CODE_SHARING_PATH               ""          CACHE PATH      "Path to repo which shares code with secure firmware")

set(TFM_INSTALL_PATH                    ${CMAKE_BINARY_DIR}/install CACHE PATH "Path to which to install TF-M files")

set(TFM_CODE_COVERAGE                   OFF         CACHE BOOL      "Whether to build the binary for lcov tools by adding -g")

set(TFM_SP_META_PTR_ENABLE              OFF         CACHE BOOL      "Use Partition Metadata Pointer")

set(TFM_PXN_ENABLE                      OFF         CACHE BOOL      "Use Privileged execute never (PXN)")

set(TFM_EXCEPTION_INFO_DUMP             OFF         CACHE BOOL      "On fatal errors in the secure firmware, capture info about the exception. Print the info if the SPM log level is sufficient.")

########################## BL2 #################################################

set(MCUBOOT_IMAGE_NUMBER                2           CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each seperately")
set(MCUBOOT_EXECUTION_SLOT              1           CACHE STRING    "Slot from which to execute the image, used for XIP mode")
set(MCUBOOT_LOG_LEVEL                   "INFO"      CACHE STRING    "Level of logging to use for MCUboot [OFF, ERROR, WARNING, INFO, DEBUG]")
set(MCUBOOT_HW_KEY                      ON          CACHE BOOL      "Whether to embed the entire public key in the image metadata instead of the hash only")
set(MCUBOOT_UPGRADE_STRATEGY            "OVERWRITE_ONLY" CACHE STRING "Upgrade strategy for images")
set(MCUBOOT_MEASURED_BOOT               ON          CACHE BOOL      "Add boot measurement values to boot status. Used for initial attestation token")
set(MCUBOOT_HW_ROLLBACK_PROT            ON          CACHE BOOL      "Enable security counter validation against non-volatile HW counters")
set(MCUBOOT_ENC_IMAGES                  OFF         CACHE BOOL      "Enable encrypted image upgrade support")
set(MCUBOOT_ENCRYPT_RSA                 OFF         CACHE BOOL      "Use RSA for encrypted image upgrade support")
set(MCUBOOT_FIH_PROFILE                 OFF         CACHE STRING    "Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]")

# Note - If either SIGNATURE_TYPE or KEY_LEN are changed, the entries for KEY_S
# and KEY_NS will either have to be updated manually or removed from the cache.
# `cmake .. -UMCUBOOT_KEY_S -UMCUBOOT_KEY_NS`. Once removed from the cache it
# will be set to default again.
set(MCUBOOT_SIGNATURE_TYPE              "RSA"       CACHE STRING    "Algorithm to use for signature validation")
set(MCUBOOT_SIGNATURE_KEY_LEN           3072        CACHE STRING    "Key length to use for signature validation")
set(MCUBOOT_KEY_S                       "${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}.pem" CACHE FILEPATH "Path to key with which to sign secure binary")
set(MCUBOOT_KEY_NS                      "${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}_1.pem" CACHE FILEPATH "Path to key with which to sign non-secure binary")

set(MCUBOOT_IMAGE_VERSION_S             ${TFM_VERSION} CACHE STRING "Version number of S image")
set(MCUBOOT_IMAGE_VERSION_NS            0.0.0       CACHE STRING    "Version number of NS image")
set(MCUBOOT_SECURITY_COUNTER_S          "auto"      CACHE STRING    "Security counter for S image. auto sets it to IMAGE_VERSION_S")
set(MCUBOOT_SECURITY_COUNTER_NS         "auto"      CACHE STRING    "Security counter for NS image. auto sets it to IMAGE_VERSION_NS")
set(MCUBOOT_S_IMAGE_MIN_VER             0.0.0+0     CACHE STRING    "Minimum version of secure image required by the non-secure image for upgrade to this non-secure image. If MCUBOOT_IMAGE_NUMBER == 1 this option has no effect")
set(MCUBOOT_NS_IMAGE_MIN_VER            0.0.0+0     CACHE STRING    "Minimum version of non-secure image required by the secure image for upgrade to this secure image. If MCUBOOT_IMAGE_NUMBER == 1 this option has no effect")

############################ Platform ##########################################

set(TFM_MULTI_CORE_TOPOLOGY             OFF         CACHE BOOL      "Whether to build for a dual-cpu architecture")
set(NUM_MAILBOX_QUEUE_SLOT              1           CACHE BOOL      "Number of mailbox queue slots")

set(DEBUG_AUTHENTICATION                CHIP_DEFAULT CACHE STRING   "Debug authentication setting. [CHIP_DEFAULT, NONE, NS_ONLY, FULL")
set(SECURE_UART1                        OFF         CACHE BOOL      "Enable secure UART1")

set(CRYPTO_HW_ACCELERATOR               OFF         CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_HW_ACCELERATOR_OTP_STATE     OFF         CACHE STRING    "Whether to enable the crypto hardware accelerator OTP memory on supported platforms (Set to PROVISIONING to enable OTP provisioning)")

set(PLATFORM_DUMMY_ATTEST_HAL           TRUE        CACHE BOOL      "Use dummy attest hal implementation. Should not be used in production.")
set(PLATFORM_DUMMY_NV_COUNTERS          TRUE        CACHE BOOL      "Use dummy nv counter implementation. Should not be used in production.")
set(PLATFORM_DUMMY_CRYPTO_KEYS          TRUE        CACHE BOOL      "Use dummy crypto keys. Should not be used in production.")
set(PLATFORM_DUMMY_ROTPK                TRUE        CACHE BOOL      "Use dummy root of trust public key. Dummy key is the public key for the default keys in bl2. Should not be used in production.")
set(PLATFORM_DUMMY_IAK                  TRUE        CACHE BOOL      "Use dummy initial attestation_key. Should not be used in production.")
set(PLATFORM_DEFAULT_UART_STDOUT        TRUE        CACHE BOOL      "Use default uart stdout implementation.")

############################ Partitions ########################################

set(TFM_PARTITION_PROTECTED_STORAGE     ON          CACHE BOOL      "Enable Protected Storage partition")
set(PS_CREATE_FLASH_LAYOUT              ON          CACHE BOOL      "Create flash FS if it doesn't exist for Protected Storage partition")
set(PS_ENCRYPTION                       ON          CACHE BOOL      "Enable encryption for Protected Storage partition")
set(PS_RAM_FS                           OFF         CACHE BOOL      "Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition")
set(PS_ROLLBACK_PROTECTION              ON          CACHE BOOL      "Enable rollback protection for Protected Storage partition")
set(PS_VALIDATE_METADATA_FROM_FLASH     ON          CACHE BOOL      "Validate filesystem metadata every time it is read from flash")
set(PS_MAX_ASSET_SIZE                   "2048"      CACHE STRING    "The maximum asset size to be stored in the Protected Storage area")
set(PS_NUM_ASSETS                       "10"        CACHE STRING    "The maximum number of assets to be stored in the Protected Storage area")
set(PS_CRYPTO_AEAD_ALG                  PSA_ALG_GCM CACHE STRING    "The AEAD algorithm to use for authenticated encryption in Protected Storage")

set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
set(ITS_CREATE_FLASH_LAYOUT             ON          CACHE BOOL      "Create flash FS if it doesn't exist for Internal Trusted Storage partition")
set(ITS_RAM_FS                          OFF         CACHE BOOL      "Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition")
set(ITS_VALIDATE_METADATA_FROM_FLASH    ON          CACHE BOOL      "Validate filesystem metadata every time it is read from flash")
set(ITS_MAX_ASSET_SIZE                  "512"       CACHE STRING    "The maximum asset size to be stored in the Internal Trusted Storage area")
set(ITS_NUM_ASSETS                      "10"        CACHE STRING    "The maximum number of assets to be stored in the Internal Trusted Storage area")
set(ITS_BUF_SIZE                        ""          CACHE STRING    "Size of the ITS internal data transfer buffer (defaults to ITS_MAX_ASSET_SIZE if not set)")

set(TFM_PARTITION_CRYPTO                ON          CACHE BOOL      "Enable Crypto partition")
# CRYPTO_ENGINE_BUF_SIZE needs to be >8KB for EC signing by attest module.
set(CRYPTO_ENGINE_BUF_SIZE              0x2080      CACHE STRING    "Heap size for the crypto backend")
set(CRYPTO_CONC_OPER_NUM                8           CACHE STRING    "The max number of concurrent operations that can be active (allocated) at any time in Crypto")
set(CRYPTO_KEY_MODULE_DISABLED          FALSE       CACHE BOOL      "Disable PSA Crypto Key module")
set(CRYPTO_AEAD_MODULE_DISABLED         FALSE       CACHE BOOL      "Disable PSA Crypto AEAD module")
set(CRYPTO_MAC_MODULE_DISABLED          FALSE       CACHE BOOL      "Disable PSA Crypto MAC module")
set(CRYPTO_HASH_MODULE_DISABLED         FALSE       CACHE BOOL      "Disable PSA Crypto Hash module")
set(CRYPTO_CIPHER_MODULE_DISABLED       FALSE       CACHE BOOL      "Disable PSA Crypto Cipher module")
set(CRYPTO_GENERATOR_MODULE_DISABLED    FALSE       CACHE BOOL      "Disable PSA Crypto Key Derivation module")
set(CRYPTO_ASYMMETRIC_MODULE_DISABLED   FALSE       CACHE BOOL      "Disable PSA Crypto Asymmetric key module")
set(CRYPTO_KEY_DERIVATION_MODULE_DISABLED FALSE     CACHE BOOL      "Disable PSA Crypto key derivation module")
set(CRYPTO_IOVEC_BUFFER_SIZE            5120        CACHE STRING    "Default size of the internal scratch buffer used for PSA FF IOVec allocations")

set(TFM_PARTITION_INITIAL_ATTESTATION   ON          CACHE BOOL      "Enable Initial Attestation partition")
set(SYMMETRIC_INITIAL_ATTESTATION       OFF         CACHE BOOL      "Use symmetric crypto for inital attestation")
set(ATTEST_INCLUDE_OPTIONAL_CLAIMS      ON          CACHE BOOL      "Include optional claims in initial attestation token")
set(ATTEST_INCLUDE_COSE_KEY_ID          OFF         CACHE BOOL      "Include COSE key-id in initial attestation token")

set(TFM_PARTITION_PLATFORM              ON          CACHE BOOL      "Enable Platform partition")

set(TFM_PARTITION_AUDIT_LOG             ON          CACHE BOOL      "Enable Audit Log partition")

set(FORWARD_PROT_MSG                    OFF         CACHE BOOL      "Whether to forward all PSA RoT messages to a Secure Enclave")
set(TFM_PARTITION_FIRMWARE_UPDATE       OFF         CACHE BOOL      "Enable firmware update partition")
set(TFM_FWU_BOOTLOADER_LIB             ${CMAKE_SOURCE_DIR}/secure_fw/partitions/firmware_update/bootloader/mcuboot/mcuboot_utilities.cmake CACHE FILEPATH    "Bootloader configure file for Firmware Update partition")

################################## Tests #######################################

set(TFM_INTERACTIVE_TEST                OFF         CACHE BOOL      "Enable interactive tests")
set(TFM_IRQ_TEST                        OFF         CACHE BOOL      "Enable IRQ tests")
set(TFM_PERIPH_ACCESS_TEST              OFF         CACHE BOOL      "Enable peripheral access tests")

set(PS_TEST_NV_COUNTERS                 ON          CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")

set(TFM_CRYPTO_TEST_ALG_CBC             ON          CACHE BOOL      "Test CBC cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CCM             ON          CACHE BOOL      "Test CCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CFB             ON          CACHE BOOL      "Test CFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CTR             ON          CACHE BOOL      "Test CTR cryptography mode")
set(TFM_CRYPTO_TEST_ALG_GCM             ON          CACHE BOOL      "Test GCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_SHA_512         ON          CACHE BOOL      "Test SHA-512 cryptography algorithm")
set(TFM_CRYPTO_TEST_HKDF                ON          CACHE BOOL      "Test SHA-512 cryptography algorithm")

set(TFM_FWU_TEST_REQUEST_REBOOT         OFF         CACHE BOOL      "Test psa_fwu_request_reboot")
set(TFM_FWU_TEST_WRITE_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_write with data block NULL")
set(TFM_FWU_TEST_QUERY_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_query with info NULL")

################################## Dependencies ################################

set(MBEDCRYPTO_PATH                     "DOWNLOAD"  CACHE PATH      "Path to Mbed Crypto (or DOWNLOAD to fetch automatically")
set(MBEDCRYPTO_VERSION                  "mbedtls-2.25.0" CACHE STRING "The version of Mbed Crypto to use")
set(MBEDCRYPTO_GIT_REMOTE               "https://github.com/ARMmbed/mbedtls.git" CACHE STRING "The URL (or path) to retrieve MbedTLS from.")
set(MBEDCRYPTO_BUILD_TYPE               "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type of Mbed Crypto library")
set(TFM_MBEDCRYPTO_CONFIG_PATH          "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_default.h" CACHE PATH "Config to use for Mbed Crypto")
set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH "" CACHE PATH "Config to append to standard Mbed Crypto config, used by platforms to cnfigure feature support")

set(TFM_TEST_REPO_PATH                  "DOWNLOAD"  CACHE PATH      "Path to TFM-TEST repo (or DOWNLOAD to fetch automatically")
set(TFM_TEST_REPO_VERSION               "TF-Mv1.3.0-RC2"   CACHE STRING    "The version of tf-m-tests to use")
set(CMSIS_5_PATH                        "DOWNLOAD"  CACHE PATH      "Path to CMSIS_5 (or DOWNLOAD to fetch automatically")

set(MCUBOOT_PATH                        "DOWNLOAD"  CACHE PATH      "Path to MCUboot (or DOWNLOAD to fetch automatically")
set(MCUBOOT_VERSION                     "v1.7.2"   CACHE STRING    "The version of MCUboot to use")

set(PSA_ARCH_TESTS_PATH                 "DOWNLOAD"  CACHE PATH      "Path to PSA arch tests (or DOWNLOAD to fetch automatically")
set(PSA_ARCH_TESTS_VERSION              "b0635d9"  CACHE STRING    "The version of PSA arch tests to use")

################################################################################
################################################################################

# Specifying the accepted values for certain configuration options to facilitate
# their later validation.

########################## BL2 #################################################

set_property(CACHE MCUBOOT_UPGRADE_STRATEGY PROPERTY STRINGS "OVERWRITE_ONLY;SWAP;DIRECT_XIP;RAM_LOAD")

########################## FIH #################################################

set_property(CACHE TFM_FIH_PROFILE PROPERTY STRINGS "OFF;LOW;MEDIUM;HIGH")
