
  @verbatim
  ******************************************************************************
  *
  *         Portions COPYRIGHT 2016-2019 STMicroelectronics, All Rights Reserved
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          trustedfirmware for integration with STM32Cube solution.
  ******************************************************************************
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  ******************************************************************************
  @endverbatim

### 18-October-2024 ###
========================
    + Remove test FWU 012
    
### 12-September-2024 ###
========================
    + Add TFM_PARTITION_QCBOR flag to disable QCBOR tests

### 11-March-2024 ###
========================
    Alignment with mbed-tls-v2.28.8_20240419 :
      + Remove deprecated definitions in interface/include/psa/crypto_compat.h
      + Replace deprecated definitions by new ones in interface/include/psa/crypto_sizes.h
          -> PSA_HASH_SIZE                         => PSA_HASH_LENGTH
          -> PSA_ALG_TLS12_PSK_TO_MS_MAX_PSK_LEN   => PSA_TLS12_PSK_TO_MS_PSK_MAX_SIZE
          -> PSA_MAX_BLOCK_CIPHER_BLOCK_SIZE       => PSA_TLS12_PSK_TO_MS_PSK_MAX_SIZE
          -> PSA_MAC_FINAL_SIZE                    => PSA_MAC_LENGTH
          -> PSA_KEY_EXPORT_MAX_SIZE               => PSA_EXPORT_KEY_OUTPUT_SIZE

      + Add new PSA_KEY_USAGE_SIGN_MESSAGE definition in interface/include/psa/crypto_values.h
      + Remove deprecated callbacks in interface/src/tfm_crypto_ipc.c
          -> psa_asymmetric_sign   (psa_sign_hash)
          -> psa_asymmetric_verify (psa_verify_hash)

      + Remove compatibility switching to new _HASH definition : 
          -> PSA_KEY_USAGE_VERIFY => PSA_KEY_USAGE_VERIFY_HASH
          -> PSA_KEY_USAGE_SIGN   => PSA_KEY_USAGE_SIGN_HASH
          -> PSA_HASH_SIZE        => PSA_HASH_LENGTH

      + Update implementation with _MESSAGE new definition :
          -> PSA_KEY_USAGE_VERIFY_MESSAGE / PSA_KEY_USAGE_VERIFY_HASH / PSA_HASH_LENGTH

### 01-January-2024 ###
========================
    + Synchronize secure_fw/partitions/crypto/crypto_spe.h with mbed-crypto/tests/include/spe/crypto_spe.h (mbed-tls-v2.28.5_20240108)

### 16-December-2022 ###
========================
    + Fix Keil and STM32CubeIDE compilation errors and warnings

### 05-September-2022 ###
========================
    + Backport advisory TFMV-3 (to avoid memory leakage)

### 18-August-2022 ###
========================
    + Backport advisory TFMV-5 (psa_fwu_write() may cause buffer overflow)
    + Add FIH latency on tfm memory check

### 07-July-2022 ###
========================
    + Update printf() implementation

### 26-January-2022 ###
========================
    + FOSS Audit Neutral license and removal of external targets
    + fix crypto issues
    + fix PS object data size

### 14-December-2021 ###
========================
    + stsafe: add pairing
    + fix stsafe issue
    + remove compilation warning

### 02-November-2021 ###
========================
    + add support of STSAFE through SE PSA driver started from crypto partition
	(under compilation switch PSA_USE_SE_ST)

### 11-October-2021 ###
========================
    + use tfm 1.3.0 release
    + add support of firmware update partition
    + add support of data provisionning

### 28-May-2021 ###
========================
    Add Add NS_DATA_START_2 region

### 18-May-2021 ###
========================
    Add LICENSE.txt

### 1-March-2021 ###
==========================
    Fix compile errors/warnings with STM32CubeIDE and Keil

### 29-January-2021 ###
==========================
    Avoid IAR specific preprocessor define for linker

### 11-December-2020 ###
==========================
    Initial Attestation: fix identity missing for eat
    Modification to check iat
    EAT: fix implementation regression
    APP_ROT_PARTITION removable with modification in flash_layout.h
    Fix instance ID lenght computation (33 bytes)
    Attest: Fix out_vec len if request is bigger than token_buff
    Bl2: Clean ram twice before jumper in secure appli
    Initial attestation: compute hash eat public and remove bl2 data TLV not used
    Remove key to avoid confusion (now in project folder)
    Align expected hw_version field in EAT token response

### 20-October-2020 ###
==========================
    Add flow control check
    SST with HW HUK
    fix: app rot test protection interface
    U5 Secure IRQ generated file
    U5 Secure IRQ support: GTZC , TAMPER
    trustedfirmware patch for regression test
    Put AppRoT and Platform partitions files under corresponding compile switchs
    U5/L5 AppRot test, U5 non secure RAM3/RAM4 support
    Add region.h file for the support of TFM secure appli startup in C
    app rot generated file
    Create app_rot generic code
    ITS / SST : support Programm unit 16 bytes
    AppRot : update after manifest parsing
    Add App Rot support
    Add PsaRot platform partition

### 26-June-2020 ###
==========================
    checkiat.sh : evolution to support other board
    boot data: add enum for IAS private key

### 22-June-2020 ###
==========================
    Fix perso area offset in iat-verifier tool
    Add shebang in checkiat.sh script

### 16-June-2020 ###
==========================
    Remove duplicated msp setting as done in jumper

### 04-June-2020 ###
==========================
    Fix compile warnings
    BL2: Robustness improvement: resistance to basic hw attack
    BL2: Rename of bl2_main() to main()
    BL2: Put boot_jump_to_next_image() function is specific NoHDP section
    BL2: imgtool: add command do_flash : to update hardening.sh, regression.sh, TFM_Update.sh
    BL2: Add support of second flash device (ospi external flash) in mcu boot
    BL2: Remove __CC_ARM switch introduced for ".BL2_NoHdp_Code" section
    BL2: update directory  from TF-Mv1.0
    BL2: Adaptation for SBSFU_Boot
    BL2: add MCUBOOT_PRIMARY_ONLY
    BL2: modif for MCUBOOT_PRIMARY_ONLY and MCUBOOT_EXT_LOADER
    BL2: bl2_main : mbedtls min size to stay in SRAM2
    BL2: add MCUBOOT_IMAGE_NUMBER=1
    BL2: remove mcuboot_config.h
    BL2: Reduce BL2_MBEDTLS_MEM_BUF_LEN, to get BL2 RAM fitting in SRAM2
    qcbor: Fix long long conversion to double compiler warning in MDK-ARM
    cose: Fix crypto_struct.h path for t_cose

### 10-February-2020 ###
==========================
    Fix compile warnings
    BL2: Fix boot install when no valid image in primary slot
    fix to have nonsecure build independant of project installation
    iat_verifier: byte array displayed in hexadecimal and add build ass command
    add script to compute cbor and boot.binary without provision
    add script to help initial attestation check
    Fix TFM_SBSFU_Boot binary path in checkiat.sh script

### 02-December-2019 ###
==========================
    use  TF-Mv1.0-RC2
	BL2: fix key for secure /non secure authentication
	BL2 / TFM core : modification for IAR build
	TFM core : add interface to set non secure mpu

### 03-September-2019 ###
==========================
    TFM Crypto: Modify mbed-Crypto build config to use hardware for Random Generation
	BL2 Shared Data: Area is move on top of SRAM in dedicated place(no overlay) 
	TFM core: Add function to get boot value from BL2 Shared Data
    BL2:Add a weak function call in bl2_main to set BL2 Shared Data specific to STM32 
	BL2 Shared Data: Create TLV minor definition for TLV MAJOR Core to support BL2 Shared Data
	specific to STM32
	TFM EAT : fix to pass non regression test

### 08-July-2019 ###
========================
    use  TF-Mv1.0-RC1
	Modify mbed-crypto include to fix build issue
	Add windows executeable scripts in bl2/ext/mcuboot/scripts
	Remove gpio platform service not relevant for STM32 platform
	Add bl2_main function to allow STM32 specific main
	Add weak function to jump in install application for STM32 customization
	remove directory  platform/ext/target
