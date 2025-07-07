/**
  * FIPS-180-2 compliant SHA-256 implementation
  *
  * Copyright The Mbed TLS Contributors
  * Copyright (c) 2023 STMicroelectronics.
  * All Rights Reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the "License"); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  * This file implements STMicroelectronics SHA256 with HW services based on mbed TLS API
  */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/sha256.h"
#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ST_SHA256_TIMEOUT     ((uint32_t) 3)
/* #define ST_HW_CONTEXT_SAVING */   /* Allows hash buffers interleaving */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *value, size_t size)
{
  volatile unsigned char *p = (unsigned char *)value;
  while (size--)
  {
    *p++ = 0;
  }
}

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
  mbedtls_zeroize(ctx, sizeof(mbedtls_sha256_context));

  /* Enable HASH clock */
  __HAL_RCC_HASH_CLK_ENABLE();
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
  if (ctx == NULL)
  {
    return;
  }

  mbedtls_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
  *dst = *src;
}

int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{
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

  ctx->is224 = is224;

  /* first block on 17 words */
  ctx->first = ST_SHA256_EXTRA_BYTES;

  ctx->sbuf_len = 0;

#ifdef ST_HW_CONTEXT_SAVING
  /* save hw context */
  HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */

  return 0;
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[ST_SHA256_BLOCK_SIZE])
{
#ifdef ST_HW_CONTEXT_SAVING
  /* restore hw context */
  HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */

  if (ctx->is224 == 0)
  {
    if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash, (uint8_t *) data, ST_SHA256_BLOCK_SIZE) != 0)
    {
      return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
  }
  else
  {
    if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash, (uint8_t *) data, ST_SHA256_BLOCK_SIZE) != 0)
    {
      return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
  }

#ifdef ST_HW_CONTEXT_SAVING
  /* save hw context */
  HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */

  return 0;
}

int mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
  size_t currentlen = ilen;

#ifdef ST_HW_CONTEXT_SAVING
  /* restore hw context */
  HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */

  if (currentlen < (ST_SHA256_BLOCK_SIZE + ctx->first - ctx->sbuf_len))
  {
    /* only store input data in context buffer */
    memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
    ctx->sbuf_len += currentlen;
  }
  else
  {
    /* fill context buffer until ST_SHA256_BLOCK_SIZE bytes, and process it */
    memcpy(ctx->sbuf + ctx->sbuf_len, input, (ST_SHA256_BLOCK_SIZE + ctx->first - ctx->sbuf_len));
    currentlen -= (ST_SHA256_BLOCK_SIZE + ctx->first - ctx->sbuf_len);

    if (ctx->is224 == 0)
    {
      if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash, (uint8_t *)(ctx->sbuf), ST_SHA256_BLOCK_SIZE + ctx->first) != 0)
      {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
      }
    }
    else
    {
      if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash, (uint8_t *)(ctx->sbuf), ST_SHA256_BLOCK_SIZE + ctx->first) != 0)
      {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
      }
    }

    /* Process following input data with size multiple of ST_SHA256_BLOCK_SIZE bytes */
    size_t iter = currentlen / ST_SHA256_BLOCK_SIZE;
    if (iter != 0)
    {
      if (ctx->is224 == 0)
      {
        if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash,
                                     (uint8_t *)(input + ST_SHA256_BLOCK_SIZE + ctx->first - ctx->sbuf_len),
                                     (iter * ST_SHA256_BLOCK_SIZE)) != 0)
        {
          return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
      }
      else
      {
        if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash,
                                     (uint8_t *)(input + ST_SHA256_BLOCK_SIZE + ctx->first - ctx->sbuf_len),
                                     (iter * ST_SHA256_BLOCK_SIZE)) != 0)
        {
          return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
      }
    }

    /* following blocks on 16 words */
    ctx->first = 0;

    /* Store only the remaining input data up to (ST_SHA256_BLOCK_SIZE - 1) bytes */
    ctx->sbuf_len = currentlen % ST_SHA256_BLOCK_SIZE;
    if (ctx->sbuf_len != 0)
    {
      memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
    }
  }

#ifdef ST_HW_CONTEXT_SAVING
  /* save hw context */
  HAL_HASH_ContextSaving(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */
  return 0;
}

int mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char *output)
{
#ifdef ST_HW_CONTEXT_SAVING
  /* restore hw context */
  HAL_HASH_ContextRestoring(&ctx->hhash, ctx->ctx_save_regs);
#endif /* ST_HW_CONTEXT_SAVING */

  /* Last accumulation for pending bytes in sbuf_len, then trig processing and get digest */
  if (ctx->is224 == 0)
  {
    if (HAL_HASHEx_SHA256_Accmlt_End(&ctx->hhash, ctx->sbuf, ctx->sbuf_len, output, ST_SHA256_TIMEOUT) != 0)
    {
      return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
  }
  else
  {
    if (HAL_HASHEx_SHA224_Accmlt_End(&ctx->hhash, ctx->sbuf, ctx->sbuf_len, output, ST_SHA256_TIMEOUT) != 0)
    {
      return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
  }

  ctx->sbuf_len = 0;

  return 0;
}


#endif /* MBEDTLS_SHA256_ALT*/
#endif /* MBEDTLS_SHA256_C */
