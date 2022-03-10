/*
 *  NIST SP800-38D compliant GCM implementation
 *
 *  Copyright The Mbed TLS Contributors
 *  Copyright (C) 2021, STMicroelectronics, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file implements ST GCM HW services based on API from mbed TLS
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/gcm.h"

#if defined(MBEDTLS_GCM_C)
#if defined(MBEDTLS_GCM_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

/* Parameter validation macros */
#define GCM_VALIDATE_RET( cond ) \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_GCM_BAD_INPUT )
#define GCM_VALIDATE( cond ) \
    MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/      
#define ST_GCM_TIMEOUT    0xFFU  /* 255 ms timeout for the crypto processor */
#define IV_LENGTH         12U    /* implementations restrict support to 96 bits */
#define SEC_SUCCESS_CONSTANT  0x3AU   /* Secure Success value */

/* Private macro -------------------------------------------------------------*/
/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \
} while( 0 )
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * Initialize a context
 */
void mbedtls_gcm_init( mbedtls_gcm_context *ctx )
{
    GCM_VALIDATE( ctx != NULL );

#if  defined(HW_CRYPTO_DPA_GCM)
   /* Enable SAES clock */
    __HAL_RCC_SAES_CLK_ENABLE();

#endif
    memset( ctx, 0, sizeof( mbedtls_gcm_context ) );
}

#if  defined(HW_CRYPTO_DPA_GCM)
/*
 * Precompute small multiples of H, that is set
 *      HH[i] || HL[i] = H times i,
 * where i is seen as a field element as in [MGV], ie high-order bits
 * correspond to low powers of P. The result is stored in the same way, that
 * is the high-order bit of HH corresponds to P^0 and the low-order bit of HL
 * corresponds to P^127.
 */
static int gcm_gen_table( mbedtls_gcm_context *ctx )
{
    int i, j;
    uint64_t hi, lo;
    uint64_t vl, vh;
    unsigned char h[16];

    memset( h, 0, 16 );

    /* compute Hash_subkey = E(0) */
    if ( HAL_CRYP_Encrypt( &ctx->hcryp_gcm,
                           (uint32_t *)h,
                           16,
                           (uint32_t *)h,
                           ST_GCM_TIMEOUT ) != HAL_OK )
    {
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }

    /* pack h as two 64-bits ints, big-endian */
    GET_UINT32_BE( hi, h,  0  );
    GET_UINT32_BE( lo, h,  4  );
    vh = (uint64_t) hi << 32 | lo;

    GET_UINT32_BE( hi, h,  8  );
    GET_UINT32_BE( lo, h,  12 );
    vl = (uint64_t) hi << 32 | lo;

    /* 8 = 1000 corresponds to 1 in GF(2^128) */
    ctx->HL[8] = vl;
    ctx->HH[8] = vh;

    /* 0 corresponds to 0 in GF(2^128) */
    ctx->HH[0] = 0;
    ctx->HL[0] = 0;

    for( i = 4; i > 0; i >>= 1 )
    {
        uint32_t T = ( vl & 1 ) * 0xe1000000U;
        vl  = ( vh << 63 ) | ( vl >> 1 );
        vh  = ( vh >> 1 ) ^ ( (uint64_t) T << 32);

        ctx->HL[i] = vl;
        ctx->HH[i] = vh;
    }

    for( i = 2; i <= 8; i *= 2 )
    {
        uint64_t *HiL = ctx->HL + i, *HiH = ctx->HH + i;
        vh = *HiH;
        vl = *HiL;
        for( j = 1; j < i; j++ )
        {
            HiH[j] = vh ^ ctx->HH[j];
            HiL[j] = vl ^ ctx->HL[j];
        }
    }

    return( 0 );
}
#endif /* HW_CRYPTO_DPA_GCM */

int mbedtls_gcm_setkey( mbedtls_gcm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    unsigned int i;
    int ret = 0;

    GCM_VALIDATE_RET( ctx != NULL );

    switch (keybits) {
#if  defined(HW_CRYPTO_DPA_GCM)
        case 0:
            /* implicit request for using HUK */
            break;
#endif
        case 128:
            ctx->hcryp_gcm.Init.KeySize = CRYP_KEYSIZE_128B;;
            break;

        case 192:
            ret = MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
            goto exit;

        case 256:
            ctx->hcryp_gcm.Init.KeySize = CRYP_KEYSIZE_256B;
            break;

        default :
            ret = MBEDTLS_ERR_GCM_BAD_INPUT;
            goto exit;
    }

    /* Format and fill AES key  */
    for( i=0; i < (keybits/32); i++ )
        GET_UINT32_BE( ctx->gcm_key[i], key, 4*i );

    ctx->hcryp_gcm.Init.pKey = ctx->gcm_key;
    ctx->hcryp_gcm.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;
    ctx->hcryp_gcm.Init.DataType = CRYP_BYTE_SWAP;
#if  defined(HW_CRYPTO_DPA_GCM)
    ctx->hcryp_gcm.Instance = SAES;
    ctx->hcryp_gcm.Init.Algorithm  = CRYP_AES_ECB;

    if ( 0 == keybits )
    {
        ctx->hcryp_gcm.Init.KeyMode = CRYP_KEYMODE_NORMAL;
        ctx->hcryp_gcm.Init.KeyProtection = CRYP_KEYSEL_HW;
    }
    else
    {
        ctx->hcryp_gcm.Init.KeyMode = CRYP_KEYMODE_NORMAL;
        ctx->hcryp_gcm.Init.KeyProtection = CRYP_KEYSEL_NORMAL;
    }

    /* Deinitializes the CRYP peripheral */
    if (HAL_CRYP_DeInit(&ctx->hcryp_gcm) != HAL_OK)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Enable SAES clock */
    __HAL_RCC_SAES_CLK_ENABLE();
#else
    ctx->hcryp_gcm.Instance = AES;
    ctx->hcryp_gcm.Init.Algorithm  = CRYP_AES_GCM_GMAC;

    /* Deinitializes the CRYP peripheral */
    if (HAL_CRYP_DeInit(&ctx->hcryp_gcm) != HAL_OK)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Enable AES clock */
    __HAL_RCC_AES_CLK_ENABLE();
#endif /* HW_CRYPTO_DPA_GCM */

    if (HAL_CRYP_Init(&ctx->hcryp_gcm) != HAL_OK)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

#if  defined(HW_CRYPTO_DPA_GCM)
    if( ( ret = gcm_gen_table( ctx ) ) != 0 )
        goto exit;
#endif

    /* allow multi-instance of CRYP use: save context for CRYP HW module CR */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

exit :
    return( ret );
}

#if  defined(HW_CRYPTO_DPA_GCM)
/*
 * Shoup's method for multiplication use this table with
 *      last4[x] = x times P^128
 * where x and last4[x] are seen as elements of GF(2^128) as in [MGV]
 */
static const uint64_t last4[16] =
{
    0x0000, 0x1c20, 0x3840, 0x2460,
    0x7080, 0x6ca0, 0x48c0, 0x54e0,
    0xe100, 0xfd20, 0xd940, 0xc560,
    0x9180, 0x8da0, 0xa9c0, 0xb5e0
};

/*
 * Sets output to x times H using the precomputed tables.
 * x and output are seen as elements of GF(2^128) as in [MGV].
 */
static void gcm_mult( mbedtls_gcm_context *ctx, const unsigned char x[16],
                      unsigned char output[16] )
{
    int i = 0;
    unsigned char lo, hi, rem;
    uint64_t zh, zl;

    lo = x[15] & 0xf;

    zh = ctx->HH[lo];
    zl = ctx->HL[lo];

    for( i = 15; i >= 0; i-- )
    {
        lo = x[i] & 0xf;
        hi = ( x[i] >> 4 ) & 0xf;

        if( i != 15 )
        {
            rem = (unsigned char) zl & 0xf;
            zl = ( zh << 60 ) | ( zl >> 4 );
            zh = ( zh >> 4 );
            zh ^= (uint64_t) last4[rem] << 48;
            zh ^= ctx->HH[lo];
            zl ^= ctx->HL[lo];

        }

        rem = (unsigned char) zl & 0xf;
        zl = ( zh << 60 ) | ( zl >> 4 );
        zh = ( zh >> 4 );
        zh ^= (uint64_t) last4[rem] << 48;
        zh ^= ctx->HH[hi];
        zl ^= ctx->HL[hi];
    }

    PUT_UINT32_BE( zh >> 32, output, 0 );
    PUT_UINT32_BE( zh, output, 4 );
    PUT_UINT32_BE( zl >> 32, output, 8 );
    PUT_UINT32_BE( zl, output, 12 );
}
#endif /* HW_CRYPTO_DPA_GCM */

int mbedtls_gcm_starts( mbedtls_gcm_context *ctx,
                int mode,
                const unsigned char *iv,
                size_t iv_len,
                const unsigned char *add,
                size_t add_len )
{
    size_t i;
#if  defined(HW_CRYPTO_DPA_GCM)
    const unsigned char *p;
    size_t use_len;
#else
    static uint32_t iv_32B[4];
#endif

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( mode != MBEDTLS_GCM_ENCRYPT || mode != MBEDTLS_GCM_DECRYPT);
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );

    /* IV and AD are limited to 2^64 bits, so 2^61 bytes */
    /* IV is not allowed to be zero length */
    if( iv_len == 0 ||
      ( (uint64_t) iv_len  ) >> 61 != 0 ||
      ( (uint64_t) add_len ) >> 61 != 0 )
    {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    /* HW implementation restrict support to the length of 96 bits */
    if( IV_LENGTH != iv_len )
    {
        return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
    }

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    if ( HAL_CRYP_Init( &ctx->hcryp_gcm ) != HAL_OK )
    {
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }

    ctx->mode = mode;
    ctx->len = 0;

#if  defined(HW_CRYPTO_DPA_GCM)
    ctx->add_len = 0;

    /* generate pre-counter block (Y = IV || 0exp(31) || 1) */
    memcpy( ctx->y, iv, iv_len );
    ctx->y[15] = 1;

    /* compute Single Block = E(Y)*/
    /* keep it in internal context for final computation (Authentication Tag) */
    if ( HAL_CRYP_Encrypt( &ctx->hcryp_gcm,
                           (uint32_t *)ctx->y,
                           16,
                           (uint32_t *)ctx->base_ectr,
                           ST_GCM_TIMEOUT ) != HAL_OK )
    {
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }

    ctx->add_len = add_len;
    p = add;
    while( add_len > 0 )
    {
        use_len = ( add_len < 16 ) ? add_len : 16;

        for( i = 0; i < use_len; i++ )
            ctx->buf[i] ^= p[i];

        gcm_mult( ctx, ctx->buf, ctx->buf );

        add_len -= use_len;
        p += use_len;
    }
#else
    /* Set IV with invert endianness */
    for( i=0; i < 3; i++ )
        GET_UINT32_BE( iv_32B[i], iv, 4*i );

    /* counter value must be set to 2 when processing the first block of payload */
    iv_32B[3] = 0x00000002;

    ctx->hcryp_gcm.Init.pInitVect = iv_32B;

    if (add_len != 0)
    {
      ctx->hcryp_gcm.Init.Header = (uint32_t *)add;
      /* header buffer in byte length */
      ctx->hcryp_gcm.Init.HeaderSize = (uint32_t)add_len;
    }
    else
    {
      ctx->hcryp_gcm.Init.Header = NULL;
      ctx->hcryp_gcm.Init.HeaderSize = 0;
    }

    /* Additional Authentication Data in bytes unit */
    ctx->hcryp_gcm.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_BYTE;

    /* Do not Allow IV reconfiguration at every gcm update */
    ctx->hcryp_gcm.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ONCE;

    /* reconfigure the CRYP */
    if ( HAL_CRYP_SetConfig( &ctx->hcryp_gcm, &ctx->hcryp_gcm.Init ) != HAL_OK )
    {
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }
#endif /* HW_CRYPTO_DPA_GCM */

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

    return( 0 );
}

int mbedtls_gcm_update( mbedtls_gcm_context *ctx,
                size_t length,
                const unsigned char *input,
                unsigned char *output )
{
#if  defined(HW_CRYPTO_DPA_GCM)
    unsigned char ectr[16];
    size_t i;
    const unsigned char *p;
    unsigned char *out_p = output;
    size_t use_len;
#endif

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );

    if( output > input && (size_t) ( output - input ) < length )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

    /* Total length is restricted to 2^39 - 256 bits, ie 2^36 - 2^5 bytes
     * Also check for possible overflow */
    if( ctx->len + length < ctx->len ||
        (uint64_t) ctx->len + length > 0xFFFFFFFE0ull )
    {
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
    }

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    ctx->len += length;

#if  defined(HW_CRYPTO_DPA_GCM)
    p = input;
    while( length > 0 )
    {
        use_len = ( length < 16 ) ? length : 16;

        /* increment counter */
        for( i = 16; i > 12; i-- )
        {
            ++ctx->y[i - 1];
            if( ctx->y[i - 1] != 0 )
                break;
        }

        if ( HAL_CRYP_Encrypt( &ctx->hcryp_gcm,
                           (uint32_t *)ctx->y,
                           16,
                           (uint32_t *)ectr,
                           ST_GCM_TIMEOUT ) != HAL_OK )
        {
            return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
        }

        for( i = 0; i < use_len; i++ )
        {
            if( ctx->mode == MBEDTLS_GCM_DECRYPT )
                ctx->buf[i] ^= p[i];
            out_p[i] = ectr[i] ^ p[i];
            if( ctx->mode == MBEDTLS_GCM_ENCRYPT )
                ctx->buf[i] ^= out_p[i];
        }

        gcm_mult( ctx, ctx->buf, ctx->buf );

        length -= use_len;
        p += use_len;
        out_p += use_len;
    }
#else
    if( ctx->mode == MBEDTLS_GCM_DECRYPT )
    {
         if (HAL_CRYP_Decrypt(&ctx->hcryp_gcm,
                              (uint32_t *)input,
                              length,
                              (uint32_t *)output,
                              ST_GCM_TIMEOUT) != HAL_OK)
         {
            return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
         }
    }
    else
    {
         if (HAL_CRYP_Encrypt(&ctx->hcryp_gcm,
                              (uint32_t *)input,
                              length,
                              (uint32_t *)output,
                              ST_GCM_TIMEOUT) != HAL_OK)
         {
            return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
         }
    }
#endif /* HW_CRYPTO_DPA_GCM */

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

    return( 0 );
}

int mbedtls_gcm_finish( mbedtls_gcm_context *ctx,
                unsigned char *tag,
                size_t tag_len )
{
#if  defined(HW_CRYPTO_DPA_GCM)
    unsigned char work_buf[16];
    size_t i;
    uint64_t orig_len;
    uint64_t orig_add_len;
#else
    __ALIGN_BEGIN uint8_t mac[16]      __ALIGN_END; /* temporary mac */
#endif

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( tag != NULL );

    if( tag_len > 16 || tag_len < 4 )
        return( MBEDTLS_ERR_GCM_BAD_INPUT );

#if  defined(HW_CRYPTO_DPA_GCM)
    orig_len = ctx->len * 8;
    orig_add_len = ctx->add_len * 8;

    memcpy( tag, ctx->base_ectr, tag_len );

    if( orig_len || orig_add_len )
    {
        memset( work_buf, 0x00, 16 );

        PUT_UINT32_BE( ( orig_add_len >> 32 ), work_buf, 0  );
        PUT_UINT32_BE( ( orig_add_len       ), work_buf, 4  );
        PUT_UINT32_BE( ( orig_len     >> 32 ), work_buf, 8  );
        PUT_UINT32_BE( ( orig_len           ), work_buf, 12 );

        for( i = 0; i < 16; i++ )
            ctx->buf[i] ^= work_buf[i];

        gcm_mult( ctx, ctx->buf, ctx->buf );

        for( i = 0; i < tag_len; i++ )
            tag[i] ^= ctx->buf[i];
    }
#else
    /* implementation restrict support to a 16 bytes tag buffer */
    if( tag_len != 16 )
        return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    /* Tag has a variable length */
    memset(mac, 0, sizeof(mac));

    /* Generate the authentication TAG */
    if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&ctx->hcryp_gcm,
                                          (uint32_t *)mac,
                                          ST_GCM_TIMEOUT)!= HAL_OK)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    memcpy( tag, mac, tag_len );

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;
#endif /* HW_CRYPTO_DPA_GCM */

    return( 0 );
}

int mbedtls_gcm_crypt_and_tag( mbedtls_gcm_context *ctx,
                       int mode,
                       size_t length,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t tag_len,
                       unsigned char *tag )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );
    GCM_VALIDATE_RET( tag != NULL );

    if( ( ret = mbedtls_gcm_starts( ctx, mode, iv, iv_len, add, add_len ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_gcm_update( ctx, length, input, output ) ) != 0 )
        return( ret );

    if( ( ret = mbedtls_gcm_finish( ctx, tag, tag_len ) ) != 0 )
        return( ret );

    return( 0 );
}

int mbedtls_gcm_auth_decrypt( mbedtls_gcm_context *ctx,
                      size_t length,
                      const unsigned char *iv,
                      size_t iv_len,
                      const unsigned char *add,
                      size_t add_len,
                      const unsigned char *tag,
                      size_t tag_len,
                      const unsigned char *input,
                      unsigned char *output )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char check_tag[16];
    size_t i, j;
    unsigned char diff = 0;
    volatile unsigned char inv_diff = 0xFF;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );

    if( ( ret = mbedtls_gcm_crypt_and_tag( ctx, MBEDTLS_GCM_DECRYPT, length,
                                   iv, iv_len, add, add_len,
                                   input, output, tag_len, check_tag ) ) != 0 )
    {
        return( ret );
    }

    j = tag_len;

    /* Check tag in "constant-time" */
    for( i = 0; i < tag_len; i++ )
    {
        diff |= tag[i] ^ SEC_SUCCESS_CONSTANT ^ check_tag[i];
        inv_diff &= tag[i] ^ SEC_SUCCESS_CONSTANT ^ check_tag[i];
        j--;
    }

    /* Check loop is executed ("not skipped" due to attacks) */
    if( ( i != tag_len ) || ( j != 0 ) || ( 0 == tag_len) )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_GCM_AUTH_FAILED );
    }

    if( ( diff != SEC_SUCCESS_CONSTANT ) || ( inv_diff != SEC_SUCCESS_CONSTANT ) )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_GCM_AUTH_FAILED );
    }

    return( (diff ^ inv_diff) + j );
}

void mbedtls_gcm_free( mbedtls_gcm_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_gcm_context ) );
}

#endif /* MBEDTLS_GCM_ALT */
#endif /* MBEDTLS_GCM_C */
