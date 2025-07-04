
  @verbatim
  ******************************************************************************
  *
  * Portions Copyright (C) 2016-2024 STMicroelectronics, All Rights Reserved
  * Copyright (C) 2006-2024, ARM Limited, All Rights Reserved
  *
  * @file    st_readme.txt
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          Mbed-TLS for integration with STM32Cube solution.
  ******************************************************************************
  *
  * Original licensing conditions
  * as issued by SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  @endverbatim

  @Note Mbed-TLS release
   For additional information on the Mbed-TLS release, consult the changelog file.

### 14-April-2025 ###
========================
    + Move to Mbed-TLS V3.6.3 to get fix for CVE-2025-27809 and CVE-2025-27810.
    + Keep ST patchs from Mbed TLS V3.6.2
    + Update st_readme.txt

### 04-February-2025 ###
========================
    + Replace the Mbed TLS contribution file with the ST contribution file.
    + Replace the Mbed TLS security file with the ST security file (PSIRT).
    + Solve warning in pk.c : 'key_bits' may be used uninitialized.
    + Update st_readme.txt.

### 24-January-2025 ###
========================
    + Add CODE_OF_CONDUCT.md
    + Renamed README.md to MBEDTLS_README.md
    + Add ST README.md
    + Add HUK feature to allow cipher operation using the hardware unique key on STM32
    + Set MBEDTLS_PRIVATE for pk in mbedtls_pk_get_ec_group_id
    + Update st_readme.txt

### 25-November-2024 ###
========================
    + Move to Mbed-TLS V3.6.2 to get fix for CVE-2024-49195
    + Keep ST patchs from Mbed TLS V3.6.1
    + Update st_readme.txt

### 10-september-2024 ###
========================
    + Move to Mbed-TLS V3.6.1
    + Remove dual license, STMicroelectronics provides the Mbed-TLS middleware
      under only the Apache-2.0 license.
    + FIx IDE warnings :
      Add MBEDTLS_RSA_C in psa_crypto_driver_wrappers.h file.
      Add MBEDTLS_MD_C in md.c and config_adjust_psa_from_legacy.h files.
      Add AT_LEAST_ONE_BUILTIN_KDF, PSA_WANT_ALG_GCM, and PSA_WANT_ALG_CCM to psa_crypto.c file.
      Add MBEDTLS_PK_HAVE_ECC_KEYS to pk.c file.
    + Enable constant time API to be used by RSA ALT in constant_time files.
    + Add double signature check, with single signature computation in ecdsa.c file.
    + Improve self-test for nist_kw and gcm.
    + Add STM32 Key Wrap Engine opaque driver to Mbed TLS and improve Jinja driver templates.
    + Export rsa_rsassa_pkcs1_v15_encode and mgf_mask functions to be used by Key Wrap driver.
    + Replace include psa/error by mbedtls/error in psa_crypto_se.c and psa_crypto_storage.c files.
    + Add ITS ALT support in psa_crypto_storage.c file.
    + Update st_readme.txt

### 29-March-2024 ###
========================
    + Move to Mbed-TLS V3.6.0 LTS
    + Remove dual license, STMicroelectronics provides the Mbed-TLS middleware
      under only the Apache-2.0 license.
    + Update st_readme.txt

### 07-February-2024 ###
========================
    + Move to Mbed-TLS V2.28.7
    + Remove dual license, STMicroelectronics provides the Mbed-TLS middleware
      under only the Apache-2.0 license.
    + Fixed IAR Warning[Pe546]: transfer of control bypasses initialization
      variable : padding
    + Update st_readme.txt


### 08-December-2023 ###
========================
    + Add ST Copyright
    + Update st_readme.txt
    + Add support when using STM32 Secure Element.
    + Add support when using STM32 HUK.
    + Add double signature check, with single signature computation.
    + Fix inclusion path : replaced "psa/crypto.h" by "crypto.h" when building
      TFM project.
    + Fix warning : enumerated type mixed with another type (mbedtls_md_type_t).
    + Execlude macro when building project for TFM to avoid Warning[Pa181]:
      incompatible redefinition of macros : "PSA_KEY_EXPORT_MAX_SIZE",
      “PSA_MAX_BLOCK_CIPHER_BLOCK_SIZE”, “PSA_HASH_SIZE”,  “PSA_MAC_FINAL_SIZE”
      & “PSA_ALG_TLS12_PSK_TO_MS_MAX_PSK_LEN”.

### 27-November-2023 ###
========================
    + Add LICENSE.txt
    + update st_readme.txt
    + psa_crypto_aead.c & pas_crypto_driver_wrappers.c :
      add MBEDTLS_PSA_BUILTIN_AEAD flag to avoid Keil Error: L6218E: Undefined
      symbol.
    + psa_crypto.c : Add AT_LEAST_ONE_BUILTIN_KDF flag to avoid IAR
      Warning[Pe111].
    + pkcs5.c : moved padding variable declaration to the begining of
      mbedtls_pkcs5_pbes2_ext to avoid IAR warning.
    + gcm.c & nist_kw.c : improved Mbed TLS Self-test to skip AES-192 key size
      test when the alternative implementation does not support it.
    + crypto_spe.h : add function definition to avoid duplication of symbols
      between TF-M and Mbed Crypto.

### 15-November-2023 ###
========================
    Move to Mbed-TLS v2.28.5

### 17-August-2023 ###
========================
    Move to Mbed-TLS v2.28.4
 
### 25-May-2023 ###
========================
    Move to Mbed-TLS v2.28.3

### 13-May-2022 ###
========================
    + backport mbedtls-2.28.0
    ++ alignment with TFM v1.3.0
    +++ remove unsupported functions : psa_mac_compute, psa_mac_verify, psa_cipher_encrypt, psa_cipher_decrypt
    +++ fix double inclusion path porting on crypto.h
    +++ use deprecating define PSA_KEY_USAGE_SIGN_HASH (instead of PSA_KEY_USAGE_SIGN_MESSAGE)
    +++ use deprecating define PSA_KEY_USAGE_VERIFY_HASH, (instead of PSA_KEY_USAGE_VERIFY_MESSAGE,)
    +++ Allow creating a read-only key (for secure element)

    ++ miscellaneous warnings

    ++ backport customizations
    +++ HUK support for STM32U5, under switch USE_HUK
    +++ double signature check
    +++ using of vendor keys for secure element)

### 18-May-2021 ###
========================
    + Add LICENSE.txt

### 12-June-2020 ###
========================
    + pkparse.c fix warning when MBEDTLS_PEM_PARSE_C, MBEDTLS_PKCS12_C & MBEDTLS_PKCS5_C are not defined

### 8-June-2020 ###
========================
    + Update this file.

### 1-June-2020 ###
========================
    + Fix switch MCUBOOT_DOUBLE_SIGN_VERIF name.

### 14-May-2020 ###
========================
    + Introduce switch to allow double signature check with single signature
      computation.

### 07-November-2019 ###
========================
    use  mbedcrypto-1.1.0

### 28-June-2019 ###
========================
    use  mbedcrypto-1.0.0
