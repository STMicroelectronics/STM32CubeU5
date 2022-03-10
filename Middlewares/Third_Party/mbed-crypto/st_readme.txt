
  @verbatim
  ******************************************************************************
  *
  *         Portions COPYRIGHT 2016-2019 STMicroelectronics, All Rights Reserved
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

### 14-December-2021 ###
========================
    + remove compilation warning

### 03-November-2021 ###
========================
    + fix some stsafe issues

### 11-October-2021 ###
========================
    + use crypto lib from mbedtls-2.25.0

### 18-May-2021 ###
========================
    + Add LICENSE.txt

### 20-October-2020 ###
========================
    + HUK support for STM32U5, under switch USE_HUK

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
