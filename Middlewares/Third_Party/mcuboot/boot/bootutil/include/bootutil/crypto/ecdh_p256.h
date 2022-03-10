/*
 * This module provides a thin abstraction over some of the crypto
 * primitives to make it easier to swap out the used crypto library.
 *
 * At this point, there are two choices: MCUBOOT_USE_MBED_TLS, or
 * MCUBOOT_USE_TINYCRYPT.  It is a compile error there is not exactly
 * one of these defined.
 */

#ifndef __BOOTUTIL_CRYPTO_ECDH_P256_H_
#define __BOOTUTIL_CRYPTO_ECDH_P256_H_

#include "mcuboot_config/mcuboot_config.h"

#if (defined(MCUBOOT_USE_TINYCRYPT) + \
     defined(MCUBOOT_USE_MBED_TLS)) != 1
    #error "One crypto backend must be defined: either MBEDTLS or TINYCRYPT"
#endif

#if defined(MCUBOOT_USE_MBED_TLS)
    #include "mbedtls/ecdh.h"
    #include "mbedtls/hkdf.h"
    #define BOOTUTIL_CRYPTO_ECDH_P256_HASH_SIZE (4 * 8)
#endif /* MCUBOOT_USE_MBED_TLS */
#if defined(MCUBOOT_USE_TINYCRYPT)
    #include <tinycrypt/ecc_dh.h>
    #include <tinycrypt/constants.h>
    #define BOOTUTIL_CRYPTO_ECDH_P256_HASH_SIZE (4 * 8)
#endif /* MCUBOOT_USE_TINYCRYPT */

#ifdef __cplusplus
extern "C" {
#endif
#if defined(MCUBOOT_USE_MBED_TLS)
typedef mbedtls_ecdh_context bootutil_ecdh_p256_context;
static inline void bootutil_ecdh_p256_init(bootutil_ecdh_p256_context *ctx)
{
  mbedtls_ecdh_init(ctx);  
}
static inline void bootutil_ecdh_p256_drop(bootutil_ecdh_p256_context *ctx)
{
    mbedtls_ecdh_free(ctx);
}


static inline int bootutil_ecdh_p256_shared_secret(bootutil_ecdh_p256_context *ctx, const uint8_t *pk, const uint8_t *sk, uint8_t *z)
{
    int rc = -1;
    size_t olen; /* output len */
    rc = mbedtls_ecdh_setup(ctx,MBEDTLS_ECP_DP_SECP256R1);
    if (rc) {
        return -4;
    }
    rc = mbedtls_ecp_point_read_binary( &ctx->ctx.mbed_ecdh.grp,
            &ctx->ctx.mbed_ecdh.Qp,
            pk, 2*NUM_ECC_BYTES+1);
    if (rc) {
        return -1;
    }
    if (mbedtls_ecp_check_pubkey(&ctx->ctx.mbed_ecdh.grp, &ctx->ctx.mbed_ecdh.Qp)) {
        return -11;
    }
    /* import the private key */
    rc = mbedtls_mpi_read_binary(&ctx->ctx.mbed_ecdh.d, sk, NUM_ECC_BYTES);
    if (rc) {
        return -2;
    }
    /* compute secret */
    rc = mbedtls_ecdh_calc_secret( ctx,
                                   &olen, z,
                                   NUM_ECC_BYTES,
                                   NULL,
                                   NULL);
    if (rc) {
	return -3;
    }
    return 0;
}
#endif
#if defined(MCUBOOT_USE_TINYCRYPT)
typedef uintptr_t bootutil_ecdh_p256_context;
static inline void bootutil_ecdh_p256_init(bootutil_ecdh_p256_context *ctx)
{
    (void)ctx;
}

static inline void bootutil_ecdh_p256_drop(bootutil_ecdh_p256_context *ctx)
{
    (void)ctx;
}

static inline int bootutil_ecdh_p256_shared_secret(bootutil_ecdh_p256_context *ctx, const uint8_t *pk, const uint8_t *sk, uint8_t *z)
{
    int rc;
    (void)ctx;

    rc = uECC_valid_public_key(pk, uECC_secp256r1());
    if (rc != 0) {
        return -1;
    }

    rc = uECC_shared_secret(pk, sk, z, uECC_secp256r1());
    if (rc != TC_CRYPTO_SUCCESS) {
        return -1;
    }
       

    return 0;
}
#endif /* MCUBOOT_USE_TINYCRYPT */

#ifdef __cplusplus
}
#endif

#endif /* __BOOTUTIL_CRYPTO_ECDH_P256_H_ */
