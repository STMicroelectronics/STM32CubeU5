
  @verbatim
  ******************************************************************************
  *
  *         Portions COPYRIGHT 2016-2022 STMicroelectronics, All Rights Reserved
  *         Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          mbed-crypto for integration with STM32Cube solution.
  ******************************************************************************
  *
  * original licensing conditions
  * as issued by SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  @endverbatim

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
