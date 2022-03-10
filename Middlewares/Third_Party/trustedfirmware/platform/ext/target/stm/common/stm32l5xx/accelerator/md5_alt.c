/*
 *  RFC 1321 compliant MD5 implementation
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
 *  This file implements STMicroelectronics MD5 with HW services based on mbed TLS API
 */
/*
 *  The MD5 algorithm was designed by Ron Rivest in 1991.
 *
 *  http://www.ietf.org/rfc/rfc1321.txt
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/md5.h"

#if defined(MBEDTLS_MD5_C)
#if defined(MBEDTLS_MD5_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ST_MD5_TIMEOUT     ((uint32_t) 3)

/* Private macro -------------------------------------------------------------*/
#define MD5_VALIDATE_RET(cond)                             \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_MD5_BAD_INPUT_DATA )
#define MD5_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

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

void mbedtls_md5_init(mbedtls_md5_context *ctx)
{
    MD5_VALIDATE( ctx != NULL );

    mbedtls_zeroize(ctx, sizeof(mbedtls_md5_context));

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();
}

void mbedtls_md5_free(mbedtls_md5_context *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    mbedtls_zeroize(ctx, sizeof(mbedtls_md5_context));
}

void mbedtls_md5_clone(mbedtls_md5_context *dst,
                       const mbedtls_md5_context *src)
{
    MD5_VALIDATE( dst != NULL );
    MD5_VALIDATE( src != NULL );

    *dst = *src;
}

int mbedtls_md5_starts_ret(mbedtls_md5_context *ctx)
{
    MD5_VALIDATE_RET( ctx != NULL );

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
    ctx->first = ST_MD5_EXTRA_BYTES;

    ctx->sbuf_len = 0;

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_internal_md5_process( mbedtls_md5_context *ctx, const unsigned char data[ST_MD5_BLOCK_SIZE] )
{
    MD5_VALIDATE_RET( ctx != NULL );
    MD5_VALIDATE_RET( (const unsigned char *)data != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    if (HAL_HASH_MD5_Accmlt(&ctx->hhash, (uint8_t *) data, ST_MD5_BLOCK_SIZE) != 0)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_md5_update_ret(mbedtls_md5_context *ctx, const unsigned char *input, size_t ilen)
{
    size_t currentlen = ilen;

    MD5_VALIDATE_RET( ctx != NULL );
    MD5_VALIDATE_RET( ilen == 0 || input != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    if (currentlen < (ST_MD5_BLOCK_SIZE + ctx->first - ctx->sbuf_len))
    {
        /* only store input data in context buffer */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
        ctx->sbuf_len += currentlen;
    }
    else
    {
        /* fill context buffer until ST_MD5_BLOCK_SIZE bytes, and process it */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, (ST_MD5_BLOCK_SIZE + ctx->first - ctx->sbuf_len));
        currentlen -= (ST_MD5_BLOCK_SIZE + ctx->first - ctx->sbuf_len);

        if (HAL_HASH_MD5_Accmlt(&ctx->hhash, (uint8_t *)(ctx->sbuf), ST_MD5_BLOCK_SIZE + ctx->first) != 0)
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        /* Process following input data with size multiple of ST_MD5_BLOCK_SIZE bytes */
        size_t iter = currentlen / ST_MD5_BLOCK_SIZE;
        if (iter != 0)
        {
            if (HAL_HASH_MD5_Accmlt(&ctx->hhash, (uint8_t *)(input + ST_MD5_BLOCK_SIZE + ctx->first - ctx->sbuf_len), (iter * ST_MD5_BLOCK_SIZE)) != 0)
            {
                return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            }
        }

        /* following blocks on 16 words */
        ctx->first = 0;

        /* Store only the remaining input data up to (ST_MD5_BLOCK_SIZE - 1) bytes */
        ctx->sbuf_len = currentlen % ST_MD5_BLOCK_SIZE;
        if (ctx->sbuf_len != 0)
        {
            memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
        }
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);

    return 0;
}

int mbedtls_md5_finish_ret(mbedtls_md5_context *ctx, unsigned char output[32])
{
    MD5_VALIDATE_RET( ctx != NULL );
    MD5_VALIDATE_RET( (unsigned char *)output != NULL );

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);

    /* Last accumulation for pending bytes in sbuf_len, then trig processing and get digest */
    if (HAL_HASH_MD5_Accmlt_End(&ctx->hhash, ctx->sbuf, ctx->sbuf_len, output, ST_MD5_TIMEOUT) != 0)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    ctx->sbuf_len = 0;

    return 0;
}

#endif /* MBEDTLS_MD5_ALT*/
#endif /* MBEDTLS_MD5_C */
