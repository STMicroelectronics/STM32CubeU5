/*
 *  FIPS-180-1 compliant SHA-1 implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019, STMicroelectronics, All Rights Reserved
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
 *  This file implements STMicroelectronics SHA1 with HW services based on mbed TLS API
 */
/*
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/sha1.h"

#if defined(MBEDTLS_SHA1_C)
#if defined(MBEDTLS_SHA1_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ST_SHA1_TIMEOUT     ((uint32_t) 3)

/* Private macro -------------------------------------------------------------*/
#define SHA1_VALIDATE_RET(cond)                             \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA1_BAD_INPUT_DATA )
#define SHA1_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char *)v;
    while (n--)
    {
        *p++ = 0;
    }
}

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    SHA1_VALIDATE( ctx != NULL );

    mbedtls_zeroize(ctx, sizeof(mbedtls_sha1_context));

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    mbedtls_zeroize(ctx, sizeof(mbedtls_sha1_context));
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                          const mbedtls_sha1_context *src)
{
    SHA1_VALIDATE( dst != NULL );
    SHA1_VALIDATE( src != NULL );

    *dst = *src;
}

int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx)
{
    SHA1_VALIDATE_RET( ctx != NULL );

    /* HASH Configuration */
    if (HAL_HASH_DeInit(&ctx->hhash) != HAL_OK)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    ctx->hhash.Init.DataType = HASH_DATATYPE_8B;
    if (HAL_HASH_Init(&ctx->hhash) != HAL_OK)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* first block on 17 words */
    ctx->first = ST_SHA1_EXTRA_BYTES;

    ctx->sbuf_len = 0;

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[ST_SHA1_BLOCK_SIZE] )
{
    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( (const unsigned char *)data != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    if (HAL_HASH_SHA1_Accmlt(&ctx->hhash, (uint8_t *) data, ST_SHA1_BLOCK_SIZE) != 0)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    size_t currentlen = ilen;

    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( ilen == 0 || input != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    if (currentlen < (ST_SHA1_BLOCK_SIZE + ctx->first - ctx->sbuf_len))
    {
        /* only store input data in context buffer */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
        ctx->sbuf_len += currentlen;
    }
    else
    {
        /* fill context buffer until ST_SHA1_BLOCK_SIZE bytes, and process it */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, (ST_SHA1_BLOCK_SIZE + ctx->first - ctx->sbuf_len));
        currentlen -= (ST_SHA1_BLOCK_SIZE + ctx->first - ctx->sbuf_len);

        if (HAL_HASH_SHA1_Accmlt(&ctx->hhash, (uint8_t *)(ctx->sbuf), ST_SHA1_BLOCK_SIZE + ctx->first) != 0)
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        /* Process following input data with size multiple of ST_SHA1_BLOCK_SIZE bytes */
        size_t iter = currentlen / ST_SHA1_BLOCK_SIZE;
        if (iter != 0)
        {
            if (HAL_HASH_SHA1_Accmlt(&ctx->hhash, (uint8_t *)(input + ST_SHA1_BLOCK_SIZE + ctx->first - ctx->sbuf_len), (iter * ST_SHA1_BLOCK_SIZE)) != 0)
            {
                return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            }
        }

        /* following blocks on 16 words */
        ctx->first = 0;

        /* Store only the remaining input data up to (ST_SHA1_BLOCK_SIZE - 1) bytes */
        ctx->sbuf_len = currentlen % ST_SHA1_BLOCK_SIZE;
        if (ctx->sbuf_len != 0)
        {
            memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
        }
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx, unsigned char output[32])
{
    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( (unsigned char *)output != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    /* Last accumulation for pending bytes in sbuf_len, then trig processing and get digest */
    if (HAL_HASH_SHA1_Accmlt_End(&ctx->hhash, ctx->sbuf, ctx->sbuf_len, output, ST_SHA1_TIMEOUT) != 0)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    ctx->sbuf_len = 0;

    return 0;
}

#endif /* MBEDTLS_SHA1_ALT*/
#endif /* MBEDTLS_SHA1_C */
