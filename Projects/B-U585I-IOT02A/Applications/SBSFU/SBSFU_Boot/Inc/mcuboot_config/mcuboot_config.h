/*
 * Copyright (c) 2018 Open Source Foundries Limited
 * Copyright (c) 2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Original code taken from mcuboot project at:
 * https://github.com/JuulLabs-OSS/mcuboot
 * Git SHA of the original version: ac55554059147fff718015be9f4bd3108123f50a
 */

#ifndef __MCUBOOT_CONFIG_H__
#define __MCUBOOT_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This file is also included by the simulator, but we don't want to
 * define anything here in simulator builds.
 *
 * Instead of using mcuboot_config.h, the simulator adds MCUBOOT_xxx
 * configuration flags to the compiler command lines based on the
 * values of environment variables. However, the file still must
 * exist, or bootutil won't build.
 */
#ifndef __BOOTSIM__
/* FIH config */
#define MCUBOOT_FIH_PROFILE_HIGH    /* in this config random delay is activated at each FIH_CALL */
/*
 * In TF-M most of the configuration flags (e.g. signature type,
 * upgrade mode ...) are handled by the CMake-based buildsystem and
 * added to the compiler command lines.
 */
#define MCUBOOT_FLASH_HOMOGENOUS     /* Defined: flash having the same sector size can use this flag */
/* Available crypto schemes (do not change values, as used in appli postbuild script) */
#define CRYPTO_SCHEME_RSA2048    0x0 /* RSA-2048 signature,
                                        AES-CTR-128 encryption with key RSA-OAEP encrypted */
#define CRYPTO_SCHEME_RSA3072    0x1 /* RSA-3072 signature,
                                        AES-CTR-128 encryption with key RSA-OAEP encrypted */
#define CRYPTO_SCHEME_EC256      0x2 /* ECDSA-256 signature,
                                        AES-CTR-128 encryption with key ECIES-P256 encrypted */
#define CRYPTO_SCHEME_RSA4096    0x3 /* RSA-4096 signature,
                                        AES-CTR-128 encryption with key RSA-OAEP encrypted */
/* Crypto scheme selection : begin */
#define CRYPTO_SCHEME            CRYPTO_SCHEME_RSA2048  /* Select one of available crypto schemes */
/* Crypto scheme selection : end */


#if (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA2048) || (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA3072) || (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA4096)
/* RSA configuration */
#define MCUBOOT_SIGN_RSA
#define MCUBOOT_ENCRYPT_RSA
#if (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA2048)
#define MCUBOOT_SIGN_RSA_LEN     2048
#elif (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA3072)
#define MCUBOOT_SIGN_RSA_LEN     3072
#else
#define MCUBOOT_SIGN_RSA_LEN     4096
#endif /* (CRYPTO_SCHEME == CRYPTO_SCHEME_RSA2048) */
#else
/* ECC config */
#define NUM_ECC_BYTES 32
#define MCUBOOT_SIGN_EC256
#define MCUBOOT_ENCRYPT_EC256
#endif /* crypto scheme selection */

#define MCUBOOT_VALIDATE_PRIMARY_SLOT
#define MCUBOOT_USE_FLASH_AREA_GET_SECTORS

#define MCUBOOT_HW_ROLLBACK_PROT
#define MCUBOOT_ENC_IMAGES           /* Defined: Image encryption enabled. */

/*
 * Cryptographic settings
 */
#define MCUBOOT_USE_MBED_TLS

#include "stm32u5xx_hal.h"

/*
 * Logging
 */
#ifdef TFM_DEV_MODE
#define MCUBOOT_HAVE_LOGGING    1
#endif 

#endif /* !__BOOTSIM__ */

/*
 * Watchdog feeding
 */
#define MCUBOOT_WATCHDOG_FEED()     \
    do {                            \
        /* Do nothing. */           \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* __MCUBOOT_CONFIG_H__ */
