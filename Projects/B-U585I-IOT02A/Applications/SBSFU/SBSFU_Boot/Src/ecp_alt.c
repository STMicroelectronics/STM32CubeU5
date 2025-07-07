/**
  * Elliptic curves over GF(p): generic functions
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
  * This file implements STMicroelectronics EC scalar mul and point check
  * with HW services based on mbed TLS API
  */

/*
 * References:
 *
 * SEC1 https://www.secg.org/sec1-v2.pdf
 * GECC = Guide to Elliptic Curve Cryptography - Hankerson, Menezes, Vanstone
 * FIPS 186-3 http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf
 * RFC 4492 for the related TLS structures and constants
 * - https://www.rfc-editor.org/rfc/rfc4492
 * RFC 7748 for the Curve448 and Curve25519 curve definitions
 * - https://www.rfc-editor.org/rfc/rfc7748
 *
 * [Curve25519] https://cr.yp.to/ecdh/curve25519-20060209.pdf
 *
 * [2] CORON, Jean-S'ebastien. Resistance against differential power analysis
 *     for elliptic curve cryptosystems. In : Cryptographic Hardware and
 *     Embedded Systems. Springer Berlin Heidelberg, 1999. p. 292-302.
 *     <http://link.springer.com/chapter/10.1007/3-540-48059-5_25>
 *
 * [3] HEDABOU, Mustapha, PINEL, Pierre, et B'EN'ETEAU, Lucien. A comb method to
 *     render ECC resistant against Side Channel Attacks. IACR Cryptology
 *     ePrint Archive, 2004, vol. 2004, p. 342.
 *     <http://eprint.iacr.org/2004/342.pdf>
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif /* !MBEDTLS_CONFIG_FILE */

/**
  * \brief Function level alternative implementation.
  *
  * The MBEDTLS_ECP_INTERNAL_ALT macro enables alternative implementations to
  * replace certain functions in this module. The alternative implementations are
  * typically hardware accelerators and need to activate the hardware before the
  * computation starts and deactivate it after it finishes. The
  * mbedtls_internal_ecp_init() and mbedtls_internal_ecp_free() functions serve
  * this purpose.
  *
  * To preserve the correct functionality the following conditions must hold:
  *
  * - The alternative implementation must be activated by
  *   mbedtls_internal_ecp_init() before any of the replaceable functions is
  *   called.
  * - mbedtls_internal_ecp_free() must \b only be called when the alternative
  *   implementation is activated.
  * - mbedtls_internal_ecp_init() must \b not be called when the alternative
  *   implementation is activated.
  * - Public functions must not return while the alternative implementation is
  *   activated.
  * - Replaceable functions are guarded by \c MBEDTLS_ECP_XXX_ALT macros and
  *   before calling them an \code if( mbedtls_internal_ecp_grp_capable( grp ) )
  *   \endcode ensures that the alternative implementation supports the current
  *   group.
  */

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_ECP_ALT)

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_calloc     calloc
#define mbedtls_free       free
#endif /* MBEDTLS_PLATFORM_C */

#include "ecp_internal_alt.h"

#define ST_ECP_TIMEOUT     (5000U)

#if defined(MBEDTLS_SELF_TEST)
/*
 * Counts of point addition and doubling, and field multiplications.
 * Used to test resistance of point multiplication to simple timing attacks.
 */
static unsigned long add_count, dbl_count, mul_count;
#endif /* MBEDTLS_SELF_TEST */

#if defined(MBEDTLS_ECP_RESTARTABLE)
/*
 * Maximum number of "basic operations" to be done in a row.
 *
 * Default value 0 means that ECC operations will not yield.
 * Note that regardless of the value of ecp_max_ops, always at
 * least one step is performed before yielding.
 *
 * Setting ecp_max_ops=1 can be suitable for testing purposes
 * as it will interrupt computation at all possible points.
 */
static unsigned ecp_max_ops = 0;

/*
 * Set ecp_max_ops
 */
void mbedtls_ecp_set_max_ops(unsigned max_ops)
{
  ecp_max_ops = max_ops;
}

/*
 * Check if restart is enabled
 */
int mbedtls_ecp_restart_is_enabled(void)
{
  return (ecp_max_ops != 0);
}

/*
 * Restart sub-context for ecp_mul_comb()
 */
struct mbedtls_ecp_restart_mul
{
  mbedtls_ecp_point R;    /* current intermediate result                  */
  size_t i;               /* current index in various loops, 0 outside    */
  mbedtls_ecp_point *T;   /* table for precomputed points                 */
  unsigned char T_size;   /* number of points in table T                  */
  enum                    /* what were we doing last time we returned?    */
  {
    ECP_RSM_INIT = 0,       /* nothing so far, dummy initial state      */
    ECP_RSM_PRE_DBL,        /* precompute 2^n multiples                 */
    ECP_RSM_PRE_NORM_DBL,   /* normalize precomputed 2^n multiples      */
    ECP_RSM_PRE_ADD,        /* precompute remaining points by adding    */
    ECP_RSM_PRE_NORM_ADD,   /* normalize all precomputed points         */
    ECP_RSM_COMB_CORE,      /* ecp_mul_comb_core()                      */
    ECP_RSM_FINAL_NORM,     /* do the final normalization               */
  } STATE;
};

/*
 * Init restart_mul sub-context
 */
static void ecp_restart_rsm_init(mbedtls_ecp_restart_mul_ctx *ctx)
{
  mbedtls_ecp_point_init(&ctx->R);
  ctx->i = 0;
  ctx->T = NULL;
  ctx->T_size = 0;
  ctx->STATE = ECP_RSM_INIT;
}

/*
 * Free the components of a restart_mul sub-context
 */
static void ecp_restart_rsm_free(mbedtls_ecp_restart_mul_ctx *ctx)
{
  unsigned char i;

  if (ctx == NULL)
  {
    return;
  }

  mbedtls_ecp_point_free(&ctx->R);

  if (ctx->T != NULL)
  {
    for (i = 0; i < ctx->T_size; i++)
    {
      mbedtls_ecp_point_free(ctx->T + i);
    }
    mbedtls_free(ctx->T);
  }

  ecp_restart_rsm_init(ctx);
}

/*
 * Restart context for ecp_muladd()
 */
struct mbedtls_ecp_restart_muladd
{
  mbedtls_ecp_point mP;       /* mP value                             */
  mbedtls_ecp_point R;        /* R intermediate result                */
  enum                        /* what should we do next?              */
  {
    ECP_RSMA_MUL1 = 0,      /* first multiplication                 */
    ECP_RSMA_MUL2,          /* second multiplication                */
    ECP_RSMA_ADD,           /* addition                             */
    ECP_RSMA_NORM,          /* normalization                        */
  } STATE;
};

/*
 * Init restart_muladd sub-context
 */
static void ecp_restart_ma_init(mbedtls_ecp_restart_muladd_ctx *ctx)
{
  mbedtls_ecp_point_init(&ctx->mP);
  mbedtls_ecp_point_init(&ctx->R);
  ctx->STATE = ECP_RSMA_MUL1;
}

/*
 * Free the components of a restart_muladd sub-context
 */
static void ecp_restart_ma_free(mbedtls_ecp_restart_muladd_ctx *ctx)
{
  if (ctx == NULL)
  {
    return;
  }

  mbedtls_ecp_point_free(&ctx->mP);
  mbedtls_ecp_point_free(&ctx->R);

  ecp_restart_ma_init(ctx);
}

/*
 * Initialize a restart context
 */
void mbedtls_ecp_restart_init(mbedtls_ecp_restart_ctx *ctx)
{
  ECP_VALIDATE(ctx != NULL);
  ctx->ops_done = 0;
  ctx->depth = 0;
  ctx->rsm = NULL;
  ctx->ma = NULL;
}

/*
 * Free the components of a restart context
 */
void mbedtls_ecp_restart_free(mbedtls_ecp_restart_ctx *ctx)
{
  if (ctx == NULL)
  {
    return;
  }

  ecp_restart_rsm_free(ctx->rsm);
  mbedtls_free(ctx->rsm);

  ecp_restart_ma_free(ctx->ma);
  mbedtls_free(ctx->ma);

  mbedtls_ecp_restart_init(ctx);
}

/*
 * Check if we can do the next step
 */
int mbedtls_ecp_check_budget(const mbedtls_ecp_group *grp,
                             mbedtls_ecp_restart_ctx *rs_ctx,
                             unsigned ops)
{
  if (rs_ctx != NULL && ecp_max_ops != 0)
  {
    /* scale depending on curve size: the chosen reference is 256-bit,
     * and multiplication is quadratic. Round to the closest integer. */
    if (grp->pbits >= 512)
    {
      ops *= 4;
    }
    else if (grp->pbits >= 384)
    {
      ops *= 2;
    }

    /* Avoid infinite loops: always allow first step.
     * Because of that, however, it's not generally true
     * that ops_done <= ecp_max_ops, so the check
     * ops_done > ecp_max_ops below is mandatory. */
    if ((rs_ctx->ops_done != 0) &&
        (rs_ctx->ops_done > ecp_max_ops ||
         ops > ecp_max_ops - rs_ctx->ops_done))
    {
      return MBEDTLS_ERR_ECP_IN_PROGRESS;
    }

    /* update running count */
    rs_ctx->ops_done += ops;
  }

  return 0;
}

/* Call this when entering a function that needs its own sub-context */
#define ECP_RS_ENTER(SUB)   do {                                      \
                                 /* reset ops count for this call if top-level */                    \
                                 if (rs_ctx != NULL && rs_ctx->depth++ == 0)                        \
                                 rs_ctx->ops_done = 0;                                           \
                                                                        \
                                 /* set up our own sub-context if needed */                          \
                                 if (mbedtls_ecp_restart_is_enabled() &&                             \
                                 rs_ctx != NULL && rs_ctx->SUB == NULL)                         \
                                 {                                                                   \
                                   rs_ctx->SUB = mbedtls_calloc(1, sizeof(*rs_ctx->SUB));      \
                                   if (rs_ctx->SUB == NULL)                                       \
                                   return MBEDTLS_ERR_ECP_ALLOC_FAILED;                     \
                                                                      \
                                   ecp_restart_## SUB ##_init(rs_ctx->SUB);                      \
                                 }                                                                   \
                               } while (0)

/* Call this when leaving a function that needs its own sub-context */
#define ECP_RS_LEAVE(SUB)   do {                                      \
                                 /* clear our sub-context when not in progress (done or error) */    \
                                 if (rs_ctx != NULL && rs_ctx->SUB != NULL &&                        \
                                 ret != MBEDTLS_ERR_ECP_IN_PROGRESS)                            \
                                 {                                                                   \
                                   ecp_restart_## SUB ##_free(rs_ctx->SUB);                      \
                                   mbedtls_free(rs_ctx->SUB);                                    \
                                   rs_ctx->SUB = NULL;                                             \
                                 }                                                                   \
                                                                        \
                                 if (rs_ctx != NULL)                                                \
                                 rs_ctx->depth--;                                                \
                               } while (0)

#else /* MBEDTLS_ECP_RESTARTABLE */

#define ECP_RS_ENTER(sub)     (void) rs_ctx;
#define ECP_RS_LEAVE(sub)     (void) rs_ctx;

#endif /* MBEDTLS_ECP_RESTARTABLE */

/*
 * List of supported curves:
 *  - internal ID
 *  - TLS NamedCurve ID (RFC 4492 sec. 5.1.1, RFC 7071 sec. 2, RFC 8446 sec. 4.2.7)
 *  - size in bits
 *  - readable name
 *
 * Curves are listed in order: largest curves first, and for a given size,
 * fastest curves first. This provides the default order for the SSL module.
 *
 * Reminder: update profiles in x509_crt.c when adding a new curves!
 */
static const mbedtls_ecp_curve_info ecp_supported_curves[] =
{
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
  { MBEDTLS_ECP_DP_SECP521R1,    25,     521,    "secp521r1"         },
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP512R1_ENABLED)
  { MBEDTLS_ECP_DP_BP512R1,      28,     512,    "brainpoolP512r1"   },
#endif /* MBEDTLS_ECP_DP_BP512R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
  { MBEDTLS_ECP_DP_SECP384R1,    24,     384,    "secp384r1"         },
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP384R1_ENABLED)
  { MBEDTLS_ECP_DP_BP384R1,      27,     384,    "brainpoolP384r1"   },
#endif /* MBEDTLS_ECP_DP_BP384R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
  { MBEDTLS_ECP_DP_SECP256R1,    23,     256,    "secp256r1"         },
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP256K1_ENABLED)
  { MBEDTLS_ECP_DP_SECP256K1,    22,     256,    "secp256k1"         },
#endif /* MBEDTLS_ECP_DP_SECP256K1_ENABLED */
#if defined(MBEDTLS_ECP_DP_BP256R1_ENABLED)
  { MBEDTLS_ECP_DP_BP256R1,      26,     256,    "brainpoolP256r1"   },
#endif /* MBEDTLS_ECP_DP_BP256R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
  { MBEDTLS_ECP_DP_SECP224R1,    21,     224,    "secp224r1"         },
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP224K1_ENABLED)
  { MBEDTLS_ECP_DP_SECP224K1,    20,     224,    "secp224k1"         },
#endif /* MBEDTLS_ECP_DP_SECP224K1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
  { MBEDTLS_ECP_DP_SECP192R1,    19,     192,    "secp192r1"         },
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */
#if defined(MBEDTLS_ECP_DP_SECP192K1_ENABLED)
  { MBEDTLS_ECP_DP_SECP192K1,    18,     192,    "secp192k1"         },
#endif /* MBEDTLS_ECP_DP_SECP192K1_ENABLED */
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
  { MBEDTLS_ECP_DP_CURVE25519,   29,     256,    "x25519"            },
#endif /* MBEDTLS_ECP_DP_CURVE25519_ENABLED */
#if defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
  { MBEDTLS_ECP_DP_CURVE448,     30,     448,    "x448"              },
#endif /* MBEDTLS_ECP_DP_CURVE448_ENABLED */
  { MBEDTLS_ECP_DP_NONE,          0,     0,      NULL                },
};

#define ECP_NB_CURVES   sizeof(ecp_supported_curves) /    \
  sizeof(ecp_supported_curves[0])

static mbedtls_ecp_group_id ecp_supported_grp_id[ECP_NB_CURVES];

/*
 * List of supported curves and associated info
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_list(void)
{
  return ecp_supported_curves;
}

/*
 * List of supported curves, group ID only
 */
const mbedtls_ecp_group_id *mbedtls_ecp_grp_id_list(void)
{
  static int init_done = 0;

  if (!init_done)
  {
    size_t i = 0;
    const mbedtls_ecp_curve_info *curve_info;

    for (curve_info = mbedtls_ecp_curve_list();
         curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
         curve_info++)
    {
      ecp_supported_grp_id[i++] = curve_info->grp_id;
    }
    ecp_supported_grp_id[i] = MBEDTLS_ECP_DP_NONE;

    init_done = 1;
  }

  return ecp_supported_grp_id;
}

/*
 * Get the curve info for the internal identifier
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_grp_id(mbedtls_ecp_group_id grp_id)
{
  const mbedtls_ecp_curve_info *curve_info;

  for (curve_info = mbedtls_ecp_curve_list();
       curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
       curve_info++)
  {
    if (curve_info->grp_id == grp_id)
    {
      return curve_info;
    }
  }

  return NULL;
}

/*
 * Get the curve info from the TLS identifier
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_tls_id(uint16_t tls_id)
{
  const mbedtls_ecp_curve_info *curve_info;

  for (curve_info = mbedtls_ecp_curve_list();
       curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
       curve_info++)
  {
    if (curve_info->tls_id == tls_id)
    {
      return curve_info;
    }
  }

  return NULL;
}

/*
 * Get the curve info from the name
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_name(const char *name)
{
  const mbedtls_ecp_curve_info *curve_info;

  if (name == NULL)
  {
    return NULL;
  }

  for (curve_info = mbedtls_ecp_curve_list();
       curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
       curve_info++)
  {
    if (strcmp(curve_info->name, name) == 0)
    {
      return curve_info;
    }
  }

  return NULL;
}

/*
 * Get the type of a curve
 */
mbedtls_ecp_curve_type mbedtls_ecp_get_type(const mbedtls_ecp_group *grp)
{
  if (grp->G.MBEDTLS_PRIVATE(X).MBEDTLS_PRIVATE(p) == NULL)
  {
    return MBEDTLS_ECP_TYPE_NONE;
  }

  if (grp->G.MBEDTLS_PRIVATE(Y).MBEDTLS_PRIVATE(p) == NULL)
  {
    return MBEDTLS_ECP_TYPE_MONTGOMERY;
  }
  else
  {
    return MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS;
  }
}

/*
 * Initialize (the components of) a point
 */
void mbedtls_ecp_point_init(mbedtls_ecp_point *pt)
{
  mbedtls_mpi_init(&pt->MBEDTLS_PRIVATE(X));
  mbedtls_mpi_init(&pt->MBEDTLS_PRIVATE(Y));
  mbedtls_mpi_init(&pt->MBEDTLS_PRIVATE(Z));
}

/*
 * Initialize (the components of) a group
 *
 * STMicroelectronics edition
 *
 */
void mbedtls_ecp_group_init(mbedtls_ecp_group *grp)
{
  grp->id = MBEDTLS_ECP_DP_NONE;
  mbedtls_mpi_init(&grp->P);
  mbedtls_mpi_init(&grp->A);
  mbedtls_mpi_init(&grp->B);
  mbedtls_ecp_point_init(&grp->G);
  mbedtls_mpi_init(&grp->N);
  grp->pbits = 0;
  grp->nbits = 0;
  grp->h = 0;
  grp->modp = NULL;
  grp->t_pre = NULL;
  grp->t_post = NULL;
  grp->t_data = NULL;
  grp->T = NULL;
  grp->T_size = 0;

  grp->st_modulus_size = 0;
  grp->st_order_size = 0;
  grp->st_p = NULL;
  grp->st_a_sign = 0;
  grp->st_a_abs = NULL;
  grp->st_b = NULL;
  grp->st_gx = NULL;
  grp->st_gy = NULL;
  grp->st_n = NULL;
}

/*
 * Initialize (the components of) a key pair
 */
void mbedtls_ecp_keypair_init(mbedtls_ecp_keypair *key)
{
  mbedtls_ecp_group_init(&key->MBEDTLS_PRIVATE(grp));
  mbedtls_mpi_init(&key->MBEDTLS_PRIVATE(d));
  mbedtls_ecp_point_init(&key->MBEDTLS_PRIVATE(Q));
}

/*
 * Unallocate (the components of) a point
 */
void mbedtls_ecp_point_free(mbedtls_ecp_point *pt)
{
  if (pt == NULL)
  {
    return;
  }

  mbedtls_mpi_free(&(pt->MBEDTLS_PRIVATE(X)));
  mbedtls_mpi_free(&(pt->MBEDTLS_PRIVATE(Y)));
  mbedtls_mpi_free(&(pt->MBEDTLS_PRIVATE(Z)));
}

/*
 * Unallocate (the components of) a group
 *
 * STMicroelectronics edition
 *
 */
void mbedtls_ecp_group_free(mbedtls_ecp_group *grp)
{
  size_t i;

  if (grp == NULL)
  {
    return;
  }

  if (grp->h != 1)
  {
    mbedtls_mpi_free(&grp->P);
    mbedtls_mpi_free(&grp->A);
    mbedtls_mpi_free(&grp->B);
    mbedtls_ecp_point_free(&grp->G);
    mbedtls_mpi_free(&grp->N);
  }

  if (grp->T != NULL)
  {
    for (i = 0; i < grp->T_size; i++)
    {
      mbedtls_ecp_point_free(&grp->T[i]);
    }
    mbedtls_free(grp->T);
  }

  if (grp->st_p != NULL)
  {
    mbedtls_platform_zeroize(grp->st_p, grp->st_modulus_size);
    mbedtls_free(grp->st_p);
  }
  if (grp->st_a_abs != NULL)
  {
    mbedtls_platform_zeroize(grp->st_a_abs, grp->st_modulus_size);
    mbedtls_free(grp->st_a_abs);
  }
  if (grp->st_b != NULL)
  {
    mbedtls_platform_zeroize(grp->st_b, grp->st_modulus_size);
    mbedtls_free(grp->st_b);
  }
  if (grp->st_gx != NULL)
  {
    mbedtls_platform_zeroize(grp->st_gx, grp->st_modulus_size);
    mbedtls_free(grp->st_gx);
  }
  if (grp->st_gy != NULL)
  {
    mbedtls_platform_zeroize(grp->st_gy, grp->st_modulus_size);
    mbedtls_free(grp->st_gy);
  }
  if (grp->st_n != NULL)
  {
    mbedtls_platform_zeroize(grp->st_n, grp->st_order_size);
    mbedtls_free(grp->st_n);
  }

  mbedtls_platform_zeroize(grp, sizeof(mbedtls_ecp_group));
}

/*
 * Unallocate (the components of) a key pair
 */
void mbedtls_ecp_keypair_free(mbedtls_ecp_keypair *key)
{
  if (key == NULL)
  {
    return;
  }

  mbedtls_ecp_group_free(&key->MBEDTLS_PRIVATE(grp));
  mbedtls_mpi_free(&key->MBEDTLS_PRIVATE(d));
  mbedtls_ecp_point_free(&key->MBEDTLS_PRIVATE(Q));
}

/*
 * Copy the contents of a point
 */
int mbedtls_ecp_copy(mbedtls_ecp_point *P, const mbedtls_ecp_point *Q)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&P->MBEDTLS_PRIVATE(X), &Q->MBEDTLS_PRIVATE(X)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&P->MBEDTLS_PRIVATE(Y), &Q->MBEDTLS_PRIVATE(Y)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&P->MBEDTLS_PRIVATE(Z), &Q->MBEDTLS_PRIVATE(Z)));

cleanup:
  return ret;
}

/*
 * Copy the contents of a group object
 */
int mbedtls_ecp_group_copy(mbedtls_ecp_group *dst, const mbedtls_ecp_group *src)
{
  return mbedtls_ecp_group_load(dst, src->id);
}

/*
 * Set point to zero
 */
int mbedtls_ecp_set_zero(mbedtls_ecp_point *pt)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->MBEDTLS_PRIVATE(X), 1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->MBEDTLS_PRIVATE(Y), 1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->MBEDTLS_PRIVATE(Z), 0));

cleanup:
  return ret;
}

/*
 * Tell if a point is zero
 */
int mbedtls_ecp_is_zero(mbedtls_ecp_point *pt)
{
  return mbedtls_mpi_cmp_int(&pt->MBEDTLS_PRIVATE(Z), 0) == 0;
}

/*
 * Compare two points lazily
 */
int mbedtls_ecp_point_cmp(const mbedtls_ecp_point *P,
                          const mbedtls_ecp_point *Q)
{
  if (mbedtls_mpi_cmp_mpi(&P->MBEDTLS_PRIVATE(X), &Q->MBEDTLS_PRIVATE(X)) == 0 &&
      mbedtls_mpi_cmp_mpi(&P->MBEDTLS_PRIVATE(Y), &Q->MBEDTLS_PRIVATE(Y)) == 0 &&
      mbedtls_mpi_cmp_mpi(&P->MBEDTLS_PRIVATE(Z), &Q->MBEDTLS_PRIVATE(Z)) == 0)
  {
    return 0;
  }

  return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
}

/*
 * Import a non-zero point from ASCII strings
 */
int mbedtls_ecp_point_read_string(mbedtls_ecp_point *P, int radix,
                                  const char *x, const char *y)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&P->MBEDTLS_PRIVATE(X), radix, x));
  MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&P->MBEDTLS_PRIVATE(Y), radix, y));
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&P->MBEDTLS_PRIVATE(Z), 1));

cleanup:
  return ret;
}

/*
 * Export a point into unsigned binary data (SEC1 2.3.3 and RFC7748)
 */
int mbedtls_ecp_point_write_binary(const mbedtls_ecp_group *grp,
                                   const mbedtls_ecp_point *P,
                                   int format, size_t *olen,
                                   unsigned char *buf, size_t buflen)
{
  int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
  size_t plen;

  plen = mbedtls_mpi_size(&grp->P);

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  (void) format; /* Montgomery curves always use the same point format */
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    *olen = plen;
    if (buflen < *olen)
    {
      return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary_le(&P->MBEDTLS_PRIVATE(X), buf, plen));
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    /*
     * Common case: P == 0
     */
    if (mbedtls_mpi_cmp_int(&P->MBEDTLS_PRIVATE(Z), 0) == 0)
    {
      if (buflen < 1)
      {
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
      }

      buf[0] = 0x00;
      *olen = 1;

      return 0;
    }

    if (format == MBEDTLS_ECP_PF_UNCOMPRESSED)
    {
      *olen = 2 * plen + 1;

      if (buflen < *olen)
      {
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
      }

      buf[0] = 0x04;
      MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&P->MBEDTLS_PRIVATE(X), buf + 1, plen));
      MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&P->MBEDTLS_PRIVATE(Y), buf + 1 + plen, plen));
    }
    else if (format == MBEDTLS_ECP_PF_COMPRESSED)
    {
      *olen = plen + 1;

      if (buflen < *olen)
      {
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
      }

      buf[0] = 0x02 + mbedtls_mpi_get_bit(&P->MBEDTLS_PRIVATE(Y), 0);
      MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&P->MBEDTLS_PRIVATE(X), buf + 1, plen));
    }
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

cleanup:
  return ret;
}

/*
 * Import a point from unsigned binary data (SEC1 2.3.4 and RFC7748)
 */
int mbedtls_ecp_point_read_binary(const mbedtls_ecp_group *grp,
                                  mbedtls_ecp_point *pt,
                                  const unsigned char *buf, size_t ilen)
{
  int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
  size_t plen;

  if (ilen < 1)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  plen = mbedtls_mpi_size(&grp->P);

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    if (plen != ilen)
    {
      return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary_le(&pt->MBEDTLS_PRIVATE(X), buf, plen));
    mbedtls_mpi_free(&pt->Y);

    if (grp->id == MBEDTLS_ECP_DP_CURVE25519)
    {
      /* Set most significant bit to 0 as prescribed in RFC7748 5 */
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&pt->MBEDTLS_PRIVATE(X), plen * 8 - 1, 0));
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->Z, 1));
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    if (buf[0] == 0x00)
    {
      if (ilen == 1)
      {
        return mbedtls_ecp_set_zero(pt);
      }
      else
      {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
      }
    }

    if (buf[0] != 0x04)
    {
      return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }

    if (ilen != 2 * plen + 1)
    {
      return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&pt->MBEDTLS_PRIVATE(X), buf + 1, plen));
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&pt->MBEDTLS_PRIVATE(Y),
                                            buf + 1 + plen, plen));
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->MBEDTLS_PRIVATE(Z), 1));
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

cleanup:
  return ret;
}

/*
 * Import a point from a TLS ECPoint record (RFC 4492)
 *      struct {
 *          opaque point <1..2^8-1>;
 *      } ECPoint;
 */
int mbedtls_ecp_tls_read_point(const mbedtls_ecp_group *grp,
                               mbedtls_ecp_point *pt,
                               const unsigned char **buf, size_t buf_len)
{
  unsigned char data_len = 0;
  const unsigned char *buf_start;

  /*
   * We must have at least two bytes (1 for length, at least one for data)
   */
  if (buf_len < 2)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  data_len = *(*buf)++;
  if (data_len < 1 || data_len > buf_len - 1)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /*
   * Save buffer start for read_binary and update buf
   */
  buf_start = *buf;
  *buf += data_len;

  return mbedtls_ecp_point_read_binary(grp, pt, buf_start, data_len);
}

/*
 * Export a point as a TLS ECPoint record (RFC 4492)
 *      struct {
 *          opaque point <1..2^8-1>;
 *      } ECPoint;
 */
int mbedtls_ecp_tls_write_point(const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt,
                                int format, size_t *olen,
                                unsigned char *buf, size_t blen)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  /*
   * buffer length must be at least one, for our length byte
   */
  if (blen < 1)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  if ((ret = mbedtls_ecp_point_write_binary(grp, pt, format,
                                            olen, buf + 1, blen - 1)) != 0)
  {
    return ret;
  }

  /*
   * write length to the first byte and update total length
   */
  buf[0] = (unsigned char) * olen;
  ++*olen;

  return 0;
}

/*
 * Set a group from an ECParameters record (RFC 4492)
 */
int mbedtls_ecp_tls_read_group(mbedtls_ecp_group *grp,
                               const unsigned char **buf, size_t len)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_ecp_group_id grp_id;

  if ((ret = mbedtls_ecp_tls_read_group_id(&grp_id, buf, len)) != 0)
  {
    return ret;
  }

  return mbedtls_ecp_group_load(grp, grp_id);
}

/*
 * Read a group id from an ECParameters record (RFC 4492) and convert it to
 * mbedtls_ecp_group_id.
 */
int mbedtls_ecp_tls_read_group_id(mbedtls_ecp_group_id *grp,
                                  const unsigned char **buf, size_t len)
{
  uint16_t tls_id;
  const mbedtls_ecp_curve_info *curve_info;

  /*
   * We expect at least three bytes (see below)
   */
  if (len < 3)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /*
   * First byte is curve_type; only named_curve is handled
   */
  if (*(*buf)++ != MBEDTLS_ECP_TLS_NAMED_CURVE)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /*
   * Next two bytes are the namedcurve value
   */
  tls_id = *(*buf)++;
  tls_id <<= 8;
  tls_id |= *(*buf)++;

  if ((curve_info = mbedtls_ecp_curve_info_from_tls_id(tls_id)) == NULL)
  {
    return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
  }

  *grp = curve_info->grp_id;

  return 0;
}

/*
 * Write the ECParameters record corresponding to a group (RFC 4492)
 */
int mbedtls_ecp_tls_write_group(const mbedtls_ecp_group *grp, size_t *olen,
                                unsigned char *buf, size_t blen)
{
  const mbedtls_ecp_curve_info *curve_info;

  if ((curve_info = mbedtls_ecp_curve_info_from_grp_id(grp->id)) == NULL)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /*
   * We are going to write 3 bytes (see below)
   */
  *olen = 3;
  if (blen < *olen)
  {
    return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
  }

  /*
   * First byte is curve_type, always named_curve
   */
  *buf++ = MBEDTLS_ECP_TLS_NAMED_CURVE;

  /*
   * Next two bytes are the namedcurve value
   */
  buf[0] = curve_info->tls_id >> 8;
  buf[1] = curve_info->tls_id & 0xFF;

  return 0;
}

/*
 * Wrapper around fast quasi-modp functions, with fall-back to mbedtls_mpi_mod_mpi.
 * See the documentation of struct mbedtls_ecp_group.
 *
 * This function is in the critial loop for mbedtls_ecp_mul, so pay attention to perf.
 */
static int ecp_modp(mbedtls_mpi *N, const mbedtls_ecp_group *grp)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  if (grp->modp == NULL)
  {
    return mbedtls_mpi_mod_mpi(N, N, &grp->P);
  }

  /* N->s < 0 is a much faster test, which fails only if N is 0 */
  if ((N->MBEDTLS_PRIVATE(s) < 0 && mbedtls_mpi_cmp_int(N, 0) != 0) ||
      mbedtls_mpi_bitlen(N) > 2 * grp->pbits)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  MBEDTLS_MPI_CHK(grp->modp(N));

  /* N->s < 0 is a much faster test, which fails only if N is 0 */
  while (N->MBEDTLS_PRIVATE(s) < 0 && mbedtls_mpi_cmp_int(N, 0) != 0)
  {
    MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(N, N, &grp->P));
  }

  while (mbedtls_mpi_cmp_mpi(N, &grp->P) >= 0)
  {
    /* we known P, N and the result are positive */
    MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(N, N, &grp->P));
  }

cleanup:
  return ret;
}

/*
 * Fast mod-p functions expect their argument to be in the 0..p^2 range.
 *
 * In order to guarantee that, we need to ensure that operands of
 * mbedtls_mpi_mul_mpi are in the 0..p range. So, after each operation we will
 * bring the result back to this range.
 *
 * The following macros are shortcuts for doing that.
 */

/*
 * Reduce a mbedtls_mpi mod p in-place, general case, to use after mbedtls_mpi_mul_mpi
 */
#if defined(MBEDTLS_SELF_TEST)
#define INC_MUL_COUNT   mul_count++;
#else
#define INC_MUL_COUNT
#endif /* MBEDTLS_SELF_TEST */

#define MOD_MUL(N)                                                    \
  do                                                                  \
  {                                                                   \
    MBEDTLS_MPI_CHK(ecp_modp(&(N), grp));                       \
    INC_MUL_COUNT                                                   \
  } while (0)

static inline int mbedtls_mpi_mul_mod(const mbedtls_ecp_group *grp,
                                      mbedtls_mpi *X,
                                      const mbedtls_mpi *A,
                                      const mbedtls_mpi *B)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(X, A, B));
  MOD_MUL(*X);
cleanup:
  return ret;
}

/*
 * Reduce a mbedtls_mpi mod p in-place, to use after mbedtls_mpi_sub_mpi
 * N->s < 0 is a very fast test, which fails only if N is 0
 */
#define MOD_SUB(N)                                                    \
  while ((N).MBEDTLS_PRIVATE(s) < 0 && mbedtls_mpi_cmp_int(&(N), 0) != 0)           \
    MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&(N), &(N), &grp->P))

static inline int mbedtls_mpi_sub_mod(const mbedtls_ecp_group *grp,
                                      mbedtls_mpi *X,
                                      const mbedtls_mpi *A,
                                      const mbedtls_mpi *B)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(X, A, B));
  MOD_SUB(*X);
cleanup:
  return ret;
}

/*
 * Reduce a mbedtls_mpi mod p in-place, to use after mbedtls_mpi_add_mpi and mbedtls_mpi_mul_int.
 * We known P, N and the result are positive, so sub_abs is correct, and
 * a bit faster.
 */
#define MOD_ADD(N)                                                    \
  while (mbedtls_mpi_cmp_mpi(&(N), &grp->P) >= 0)                  \
    MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(&(N), &(N), &grp->P))

static inline int mbedtls_mpi_add_mod(const mbedtls_ecp_group *grp,
                                      mbedtls_mpi *X,
                                      const mbedtls_mpi *A,
                                      const mbedtls_mpi *B)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(X, A, B));
  MOD_ADD(*X);
cleanup:
  return ret;
}

static inline int mbedtls_mpi_shift_l_mod(const mbedtls_ecp_group *grp,
                                          mbedtls_mpi *X,
                                          size_t count)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(X, count));
  MOD_ADD(*X);
cleanup:
  return ret;
}

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
/*
 * For curves in short Weierstrass form, we do all the internal operations in
 * Jacobian coordinates.
 *
 * For multiplication, we'll use a comb method with countermeasures against
 * SPA, hence timing attacks.
 */

/*
 * Normalize jacobian coordinates so that Z == 0 || Z == 1  (GECC 3.2.1)
 * Cost: 1N := 1I + 3M + 1S
 */
static int ecp_normalize_jac(const mbedtls_ecp_group *grp, mbedtls_ecp_point *pt)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_mpi Zi, ZZi;

  if (mbedtls_mpi_cmp_int(&pt->MBEDTLS_PRIVATE(Z), 0) == 0)
  {
    return 0;
  }

#if defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_normalize_jac(grp, pt);
  }
#endif /* MBEDTLS_ECP_NORMALIZE_JAC_ALT */

  mbedtls_mpi_init(&Zi);
  mbedtls_mpi_init(&ZZi);

  /*
   * X = X / Z^2  mod p
   */
  MBEDTLS_MPI_CHK(mbedtls_mpi_inv_mod(&Zi,      &pt->MBEDTLS_PRIVATE(Z),     &grp->P));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &ZZi,     &Zi,        &Zi));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &pt->MBEDTLS_PRIVATE(X),   &pt->MBEDTLS_PRIVATE(X),     &ZZi));

  /*
   * Y = Y / Z^3  mod p
   */
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &pt->MBEDTLS_PRIVATE(Y),   &pt->MBEDTLS_PRIVATE(Y),     &ZZi));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &pt->MBEDTLS_PRIVATE(Y),   &pt->MBEDTLS_PRIVATE(Y),     &Zi));

  /*
   * Z = 1
   */
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->MBEDTLS_PRIVATE(Z), 1));

cleanup:

  mbedtls_mpi_free(&Zi);
  mbedtls_mpi_free(&ZZi);

  return ret;
}

/*
 * Point doubling R = 2 P, Jacobian coordinates
 *
 * Based on http://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian.html#doubling-dbl-1998-cmo-2 .
 *
 * We follow the variable naming fairly closely. The formula variations that trade a MUL for a SQR
 * (plus a few ADDs) aren't useful as our bignum implementation doesn't distinguish squaring.
 *
 * Standard optimizations are applied when curve parameter A is one of { 0, -3 }.
 *
 * Cost: 1D := 3M + 4S          (A ==  0)
 *             4M + 4S          (A == -3)
 *             3M + 6S + 1a     otherwise
 */
static int ecp_double_jac(const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                          const mbedtls_ecp_point *P)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_mpi M, S, T, U;

#if defined(MBEDTLS_SELF_TEST)
  dbl_count++;
#endif /* MBEDTLS_SELF_TEST */

#if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_double_jac(grp, R, P);
  }
#endif /* MBEDTLS_ECP_DOUBLE_JAC_ALT */

  mbedtls_mpi_init(&M);
  mbedtls_mpi_init(&S);
  mbedtls_mpi_init(&T);
  mbedtls_mpi_init(&U);

  /* Special case for A = -3 */
  if (grp->A.MBEDTLS_PRIVATE(p) == NULL)
  {
    /* M = 3(X + Z^2)(X - Z^2) */
    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &P->MBEDTLS_PRIVATE(Z),  &P->MBEDTLS_PRIVATE(Z)));
    MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &T,  &P->MBEDTLS_PRIVATE(X),  &S));
    MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &U,  &P->MBEDTLS_PRIVATE(X),  &S));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &T,     &U));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&M,  &S,     3));
    MOD_ADD(M);
  }
  else
  {
    /* M = 3.X^2 */
    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &P->MBEDTLS_PRIVATE(X),  &P->MBEDTLS_PRIVATE(X)));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&M,  &S,     3));
    MOD_ADD(M);

    /* Optimize away for "koblitz" curves with A = 0 */
    if (mbedtls_mpi_cmp_int(&grp->A, 0) != 0)
    {
      /* M += A.Z^4 */
      MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &P->MBEDTLS_PRIVATE(Z),  &P->MBEDTLS_PRIVATE(Z)));
      MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T,  &S,     &S));
      MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &T,     &grp->A));
      MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &M,  &M,     &S));
    }
  }

  /* S = 4.X.Y^2 */
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T,  &P->MBEDTLS_PRIVATE(Y),  &P->MBEDTLS_PRIVATE(Y)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l_mod(grp, &T,  1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &P->MBEDTLS_PRIVATE(X),  &T));
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l_mod(grp, &S,  1));

  /* U = 8.Y^4 */
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &U,  &T,     &T));
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l_mod(grp, &U,  1));

  /* T = M^2 - 2.S */
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T,  &M,     &M));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &T,  &T,     &S));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &T,  &T,     &S));

  /* S = M(S - T) - U */
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &S,  &S,     &T));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S,  &S,     &M));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &S,  &S,     &U));

  /* U = 2.Y.Z */
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &U,  &P->MBEDTLS_PRIVATE(Y),  &P->MBEDTLS_PRIVATE(Z)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l_mod(grp, &U,  1));

  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(X), &T));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(Y), &S));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(Z), &U));

cleanup:
  mbedtls_mpi_free(&M);
  mbedtls_mpi_free(&S);
  mbedtls_mpi_free(&T);
  mbedtls_mpi_free(&U);

  return ret;
}

/*
 * Addition: R = P + Q, mixed affine-Jacobian coordinates (GECC 3.22)
 *
 * The coordinates of Q must be normalized (= affine),
 * but those of P don't need to. R is not normalized.
 *
 * Special cases: (1) P or Q is zero, (2) R is zero, (3) P == Q.
 * None of these cases can happen as intermediate step in ecp_mul_comb():
 * - at each step, P, Q and R are multiples of the base point, the factor
 *   being less than its order, so none of them is zero;
 * - Q is an odd multiple of the base point, P an even multiple,
 *   due to the choice of precomputed points in the modified comb method.
 * So branches for these cases do not leak secret information.
 *
 * We accept Q->Z being unset (saving memory in tables) as meaning 1.
 *
 * Cost: 1A := 8M + 3S
 */
static int ecp_add_mixed(const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                         const mbedtls_ecp_point *P, const mbedtls_ecp_point *Q)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_mpi T1, T2, T3, T4, X, Y, Z;

#if defined(MBEDTLS_SELF_TEST)
  add_count++;
#endif /* MBEDTLS_SELF_TEST */

#if defined(MBEDTLS_ECP_ADD_MIXED_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_add_mixed(grp, R, P, Q);
  }
#endif /* MBEDTLS_ECP_ADD_MIXED_ALT */

  /*
   * Trivial cases: P == 0 or Q == 0 (case 1)
   */
  if (mbedtls_mpi_cmp_int(&P->MBEDTLS_PRIVATE(Z), 0) == 0)
  {
    return mbedtls_ecp_copy(R, Q);
  }

  if (Q->MBEDTLS_PRIVATE(Z).MBEDTLS_PRIVATE(p) != NULL && mbedtls_mpi_cmp_int(&Q->MBEDTLS_PRIVATE(Z), 0) == 0)
  {
    return mbedtls_ecp_copy(R, P);
  }

  /*
   * Make sure Q coordinates are normalized
   */
  if (Q->MBEDTLS_PRIVATE(Z).MBEDTLS_PRIVATE(p) != NULL && mbedtls_mpi_cmp_int(&Q->MBEDTLS_PRIVATE(Z), 1) != 0)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  mbedtls_mpi_init(&T1);
  mbedtls_mpi_init(&T2);
  mbedtls_mpi_init(&T3);
  mbedtls_mpi_init(&T4);
  mbedtls_mpi_init(&X);
  mbedtls_mpi_init(&Y);
  mbedtls_mpi_init(&Z);

  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T1,  &P->MBEDTLS_PRIVATE(Z),  &P->MBEDTLS_PRIVATE(Z)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T2,  &T1,    &P->MBEDTLS_PRIVATE(Z)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T1,  &T1,    &Q->MBEDTLS_PRIVATE(X)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T2,  &T2,    &Q->MBEDTLS_PRIVATE(Y)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &T1,  &T1,    &P->MBEDTLS_PRIVATE(X)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &T2,  &T2,    &P->MBEDTLS_PRIVATE(Y)));

  /* Special cases (2) and (3) */
  if (mbedtls_mpi_cmp_int(&T1, 0) == 0)
  {
    if (mbedtls_mpi_cmp_int(&T2, 0) == 0)
    {
      ret = ecp_double_jac(grp, R, P);
      goto cleanup;
    }
    else
    {
      ret = mbedtls_ecp_set_zero(R);
      goto cleanup;
    }
  }

  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &Z,   &P->MBEDTLS_PRIVATE(Z),  &T1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T3,  &T1,    &T1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T4,  &T3,    &T1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T3,  &T3,    &P->MBEDTLS_PRIVATE(X)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&T1, &T3));
  MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l_mod(grp, &T1,  1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &X,   &T2,    &T2));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &X,   &X,     &T1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &X,   &X,     &T4));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &T3,  &T3,    &X));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T3,  &T3,    &T2));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &T4,  &T4,    &P->MBEDTLS_PRIVATE(Y)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &Y,   &T3,    &T4));

  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(X), &X));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(Y), &Y));
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&R->MBEDTLS_PRIVATE(Z), &Z));

cleanup:

  mbedtls_mpi_free(&T1);
  mbedtls_mpi_free(&T2);
  mbedtls_mpi_free(&T3);
  mbedtls_mpi_free(&T4);
  mbedtls_mpi_free(&X);
  mbedtls_mpi_free(&Y);
  mbedtls_mpi_free(&Z);

  return ret;
}

/*
 * Check and define parameters used by the comb method (see below for details)
 */
#if MBEDTLS_ECP_WINDOW_SIZE < 2 || MBEDTLS_ECP_WINDOW_SIZE > 7
#error "MBEDTLS_ECP_WINDOW_SIZE out of bounds"
#endif /* MBEDTLS_ECP_WINDOW_SIZE < 2 or MBEDTLS_ECP_WINDOW_SIZE > 7 */

/*
 * Multiplication using the comb method - for curves in short Weierstrass form
 *
 * This function is mainly responsible for administrative work:
 * - managing the restart context if enabled
 * - managing the table of precomputed points (passed between the below two
 *   functions): allocation, computation, ownership transfer, freeing.
 *
 * It delegates the actual arithmetic work to:
 *      ecp_precompute_comb() and ecp_mul_comb_with_precomp()
 *
 * See comments on ecp_comb_recode_core() regarding the computation strategy.
 *
 * STMicroelectronics edition
 *
 */
static int ecp_mul_comb(mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                        const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng,
                        mbedtls_ecp_restart_ctx *rs_ctx)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  size_t olen;
  size_t scalarMulSize;
  uint8_t *P_binary;
  uint8_t *m_binary = NULL;
  uint8_t *R_binary = NULL;
  PKA_HandleTypeDef hpka = {0};
  PKA_ECCMulInTypeDef ECC_MulIn = {0};
  PKA_ECCMulOutTypeDef ECC_MulOut;

  /* Set HW peripheral input parameter: scalar m size */
  scalarMulSize = mbedtls_mpi_size(m);
  ECC_MulIn.scalarMulSize = scalarMulSize;

  /* Set HW peripheral Input parameter: curve coefs */
  ECC_MulIn.modulusSize = grp->st_modulus_size;
  ECC_MulIn.coefSign    = grp->st_a_sign;
  ECC_MulIn.coefA       = grp->st_a_abs;
  ECC_MulIn.modulus     = grp->st_p;
  ECC_MulIn.coefB       = grp->st_b;
  ECC_MulIn.primeOrder  = grp->st_n;
  /* Set HW peripheral input parameter: coordinates of P point */
  P_binary = mbedtls_calloc(2U * grp->st_modulus_size + 1U, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((P_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_ecp_point_write_binary(grp, P, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, P_binary,
                                                 2U * grp->st_modulus_size + 1U));

  ECC_MulIn.pointX = P_binary + 1U;
  ECC_MulIn.pointY = P_binary + grp->st_modulus_size + 1U;

  /* Set HW peripheral input parameter: scalar m */
  m_binary = mbedtls_calloc(scalarMulSize, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((m_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(m, m_binary, scalarMulSize));
  ECC_MulIn.scalarMul = m_binary;

  /* Enable HW peripheral clock */
  __HAL_RCC_PKA_CLK_ENABLE();

  /* Initialize HW peripheral */
  hpka.Instance = PKA;
  MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Reset PKA RAM */
  HAL_PKA_RAMReset(&hpka);

  /* Launch the scalar multiplication */
  MBEDTLS_MPI_CHK((HAL_PKA_ECCMul(&hpka, &ECC_MulIn,
                                  ST_ECP_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Allocate memory space for scalar multiplication result */
  R_binary = mbedtls_calloc(2U * grp->st_modulus_size + 1U, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((R_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  ECC_MulOut.ptX = R_binary + 1U;
  ECC_MulOut.ptY = R_binary + grp->st_modulus_size + 1U;

  /* Get the scalar multiplication result */
  HAL_PKA_ECCMul_GetResult(&hpka, &ECC_MulOut);

  /* Convert the scalar multiplication result into ecp point format */
  R_binary[0] = 0x04U;
  MBEDTLS_MPI_CHK(mbedtls_ecp_point_read_binary(grp, R, R_binary, 2U * grp->st_modulus_size + 1U));

cleanup:
  /* De-initialize HW peripheral */
  HAL_PKA_DeInit(&hpka);

  /* Disable HW peripheral clock */
  __HAL_RCC_PKA_CLK_DISABLE();

  /* Free memory */
  if (P_binary != NULL)
  {
    mbedtls_platform_zeroize(P_binary, 2U * grp->st_modulus_size + 1U);
    mbedtls_free(P_binary);
  }

  if (m_binary != NULL)
  {
    mbedtls_platform_zeroize(m_binary, scalarMulSize);
    mbedtls_free(m_binary);
  }

  if (R_binary != NULL)
  {
    mbedtls_platform_zeroize(R_binary, 2U * grp->st_modulus_size + 1U);
    mbedtls_free(R_binary);
  }

  return ret;
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
/*
 * For Montgomery curves, we do all the internal arithmetic in projective
 * coordinates. Import/export of points uses only the x coordinates, which is
 * internally represented as X / Z.
 *
 * For scalar multiplication, we'll use a Montgomery ladder.
 */

/*
 * Normalize Montgomery x/z coordinates: X = X/Z, Z = 1
 * Cost: 1M + 1I
 */
static int ecp_normalize_mxz(const mbedtls_ecp_group *grp, mbedtls_ecp_point *P)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

#if defined(MBEDTLS_ECP_NORMALIZE_MXZ_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_normalize_mxz(grp, P);
  }
#endif /* MBEDTLS_ECP_NORMALIZE_MXZ_ALT */

  MBEDTLS_MPI_CHK(mbedtls_mpi_inv_mod(&P->Z, &P->Z, &grp->P));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &P->MBEDTLS_PRIVATE(X), &P->MBEDTLS_PRIVATE(X), &P->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&P->Z, 1));

cleanup:
  return ret;
}

/*
 * Randomize projective x/z coordinates:
 * (X, Z) -> (l X, l Z) for random l
 * This is sort of the reverse operation of ecp_normalize_mxz().
 *
 * This countermeasure was first suggested in [2].
 * Cost: 2M
 */
static int ecp_randomize_mxz(const mbedtls_ecp_group *grp, mbedtls_ecp_point *P,
                             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_mpi l;
  size_t p_size = 0;
  int count = 0;

#if defined(MBEDTLS_ECP_RANDOMIZE_MXZ_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_randomize_mxz(grp, P, f_rng, p_rng);
  }
#endif /* MBEDTLS_ECP_RANDOMIZE_MXZ_ALT */

  p_size = (grp->pbits + 7) / 8;
  mbedtls_mpi_init(&l);

  /* Generate l such that 1 < l < p */
  do
  {
    MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(&l, p_size, f_rng, p_rng));

    while (mbedtls_mpi_cmp_mpi(&l, &grp->P) >= 0)
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&l, 1));
    }

    if (count++ > 10)
    {
      ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
      goto cleanup;
    }
  } while (mbedtls_mpi_cmp_int(&l, 1) <= 0);

  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &P->MBEDTLS_PRIVATE(X), &P->MBEDTLS_PRIVATE(X), &l));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &P->Z, &P->Z, &l));

cleanup:
  mbedtls_mpi_free(&l);

  return ret;
}

/*
 * Double-and-add: R = 2P, S = P + Q, with d = X(P - Q),
 * for Montgomery curves in x/z coordinates.
 *
 * http://www.hyperelliptic.org/EFD/g1p/auto-code/montgom/xz/ladder/mladd-1987-m.op3
 * with
 * d =  X1
 * P = (X2, Z2)
 * Q = (X3, Z3)
 * R = (X4, Z4)
 * S = (X5, Z5)
 * and eliminating temporary variables tO, ..., t4.
 *
 * Cost: 5M + 4S
 */
static int ecp_double_add_mxz(const mbedtls_ecp_group *grp,
                              mbedtls_ecp_point *R, mbedtls_ecp_point *S,
                              const mbedtls_ecp_point *P, const mbedtls_ecp_point *Q,
                              const mbedtls_mpi *d)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_mpi A, AA, B, BB, E, C, D, DA, CB;

#if defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT)
  if (mbedtls_internal_ecp_grp_capable(grp))
  {
    return mbedtls_internal_ecp_double_add_mxz(grp, R, S, P, Q, d);
  }
#endif /* MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT */

  mbedtls_mpi_init(&A);
  mbedtls_mpi_init(&AA);
  mbedtls_mpi_init(&B);
  mbedtls_mpi_init(&BB);
  mbedtls_mpi_init(&E);
  mbedtls_mpi_init(&C);
  mbedtls_mpi_init(&D);
  mbedtls_mpi_init(&DA);
  mbedtls_mpi_init(&CB);

  MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &A,    &P->MBEDTLS_PRIVATE(X),   &P->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &AA,   &A,      &A));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &B,    &P->MBEDTLS_PRIVATE(X),   &P->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &BB,   &B,      &B));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &E,    &AA,     &BB));
  MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &C,    &Q->MBEDTLS_PRIVATE(X),   &Q->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &D,    &Q->MBEDTLS_PRIVATE(X),   &Q->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &DA,   &D,      &A));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &CB,   &C,      &B));
  MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &S->MBEDTLS_PRIVATE(X), &DA,     &CB));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S->MBEDTLS_PRIVATE(X), &S->MBEDTLS_PRIVATE(X),   &S->MBEDTLS_PRIVATE(X)));
  MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mod(grp, &S->Z, &DA,     &CB));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S->Z, &S->Z,   &S->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &S->Z, d,       &S->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &R->MBEDTLS_PRIVATE(X), &AA,     &BB));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &R->Z, &grp->A, &E));
  MBEDTLS_MPI_CHK(mbedtls_mpi_add_mod(grp, &R->Z, &BB,     &R->Z));
  MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mod(grp, &R->Z, &E,      &R->Z));

cleanup:
  mbedtls_mpi_free(&A);
  mbedtls_mpi_free(&AA);
  mbedtls_mpi_free(&B);
  mbedtls_mpi_free(&BB);
  mbedtls_mpi_free(&E);
  mbedtls_mpi_free(&C);
  mbedtls_mpi_free(&D);
  mbedtls_mpi_free(&DA);
  mbedtls_mpi_free(&CB);

  return ret;
}

/*
 * Multiplication with Montgomery ladder in x/z coordinates,
 * for curves in Montgomery form
 */
static int ecp_mul_mxz(mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                       const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  size_t i;
  unsigned char b;
  mbedtls_ecp_point RP;
  mbedtls_mpi PX;

  mbedtls_ecp_point_init(&RP);
  mbedtls_mpi_init(&PX);

  /* Save PX and read from P before writing to R, in case P == R */
  MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&PX, &P->X));
  MBEDTLS_MPI_CHK(mbedtls_ecp_copy(&RP, P));

  /* Set R to zero in modified x/z coordinates */
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&R->MBEDTLS_PRIVATE(X), 1));
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&R->Z, 0));
  mbedtls_mpi_free(&R->Y);

  /* RP.X might be slightly larger than P, so reduce it */
  MOD_ADD(RP.X);

  /* Randomize coordinates of the starting point */
  if (f_rng != NULL)
  {
    MBEDTLS_MPI_CHK(ecp_randomize_mxz(grp, &RP, f_rng, p_rng));
  }

  /* Loop invariant: R = result so far, RP = R + P */
  i = mbedtls_mpi_bitlen(m);   /* one past the (zero-based) most significant bit */
  while (i-- > 0)
  {
    b = mbedtls_mpi_get_bit(m, i);
    /*
     *  if (b) R = 2R + P else R = 2R,
     * which is:
     *  if (b) double_add( RP, R, RP, R )
     *  else   double_add( R, RP, R, RP )
     * but using safe conditional swaps to avoid leaks
     */
    MBEDTLS_MPI_CHK(mbedtls_mpi_safe_cond_swap(&R->X, &RP.X, b));
    MBEDTLS_MPI_CHK(mbedtls_mpi_safe_cond_swap(&R->Z, &RP.Z, b));
    MBEDTLS_MPI_CHK(ecp_double_add_mxz(grp, R, &RP, R, &RP, &PX));
    MBEDTLS_MPI_CHK(mbedtls_mpi_safe_cond_swap(&R->X, &RP.X, b));
    MBEDTLS_MPI_CHK(mbedtls_mpi_safe_cond_swap(&R->Z, &RP.Z, b));
  }

  MBEDTLS_MPI_CHK(ecp_normalize_mxz(grp, R));

cleanup:
  mbedtls_ecp_point_free(&RP);
  mbedtls_mpi_free(&PX);

  return ret;
}

#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

/*
 * Restartable multiplication R = m * P
 */
int mbedtls_ecp_mul_restartable(mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                                const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                                mbedtls_ecp_restart_ctx *rs_ctx)
{
  int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  char is_grp_capable = 0;
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
  /* reset ops count for this call if top-level */
  if (rs_ctx != NULL && rs_ctx->depth++ == 0)
  {
    rs_ctx->ops_done = 0;
  }
#else
  (void) rs_ctx;
#endif /* MBEDTLS_ECP_RESTARTABLE */

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  if ((is_grp_capable = mbedtls_internal_ecp_grp_capable(grp)))
  {
    MBEDTLS_MPI_CHK(mbedtls_internal_ecp_init(grp));
  }
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
  /* skip argument check when restarting */
  if (rs_ctx == NULL || rs_ctx->rsm == NULL)
#endif /* MBEDTLS_ECP_RESTARTABLE */
  {
    /* check_privkey is free */
    MBEDTLS_ECP_BUDGET(MBEDTLS_ECP_OPS_CHK);

    /* Common sanity checks */
    MBEDTLS_MPI_CHK(mbedtls_ecp_check_privkey(grp, m));
    MBEDTLS_MPI_CHK(mbedtls_ecp_check_pubkey(grp, P));
  }

  ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    MBEDTLS_MPI_CHK(ecp_mul_mxz(grp, R, m, P, f_rng, p_rng));
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    MBEDTLS_MPI_CHK(ecp_mul_comb(grp, R, m, P, f_rng, p_rng, rs_ctx));
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

cleanup:

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  if (is_grp_capable)
  {
    mbedtls_internal_ecp_free(grp);
  }
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL)
  {
    rs_ctx->depth--;
  }
#endif /* MBEDTLS_ECP_RESTARTABLE */

  return ret;
}

/*
 * Multiplication R = m * P
 */
int mbedtls_ecp_mul(mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                    const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
  return mbedtls_ecp_mul_restartable(grp, R, m, P, f_rng, p_rng, NULL);
}

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
/*
 * Check that an affine point is valid as a public key,
 * short weierstrass curves (SEC1 3.2.3.1)
 *
 * STMicroelectronics edition
 *
 */
static int ecp_check_pubkey_sw(const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  size_t olen;
  uint8_t *pt_binary;
  PKA_HandleTypeDef hpka = {0};
  PKA_PointCheckInTypeDef ECC_PointCheck = {0};
  PKA_MontgomeryParamInTypeDef inp = {0};
  uint8_t *pt_montgomery = NULL;

  /* pt coordinates must be normalized for our checks */
  if( mbedtls_mpi_cmp_int( &pt->MBEDTLS_PRIVATE(X), 0 ) < 0 ||
      mbedtls_mpi_cmp_int( &pt->MBEDTLS_PRIVATE(Y), 0 ) < 0 ||
      mbedtls_mpi_cmp_mpi( &pt->MBEDTLS_PRIVATE(X), &grp->P ) >= 0 ||
      mbedtls_mpi_cmp_mpi( &pt->MBEDTLS_PRIVATE(Y), &grp->P ) >= 0 )
  {	
    return MBEDTLS_ERR_ECP_INVALID_KEY;
  }

  /* Set HW peripheral Input parameter: curve coefs */
  ECC_PointCheck.modulusSize = grp->st_modulus_size;
  ECC_PointCheck.modulus     = grp->st_p;
  ECC_PointCheck.coefSign    = grp->st_a_sign;
  ECC_PointCheck.coefA       = grp->st_a_abs;
  ECC_PointCheck.coefB       = grp->st_b;

  /* Set HW peripheral input parameter: coordinates of point to check */
  pt_binary = mbedtls_calloc(( 2U * grp->st_modulus_size ) + 1U, sizeof( uint8_t ));
  MBEDTLS_MPI_CHK((pt_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  pt_montgomery = mbedtls_calloc(grp->st_modulus_size, sizeof( uint8_t ));
  MBEDTLS_MPI_CHK((pt_montgomery == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, pt, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, pt_binary, ( 2U * grp->st_modulus_size ) + 1U ) );

  ECC_PointCheck.pointX = pt_binary + 1U;
  ECC_PointCheck.pointY = pt_binary + grp->st_modulus_size + 1U;

  /* Enable HW peripheral clock */
  __HAL_RCC_PKA_CLK_ENABLE();

  /* Initialize HW peripheral */
  hpka.Instance = PKA;
  MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Reset PKA RAM */
  HAL_PKA_RAMReset(&hpka);

  /* Set Montgomery R2 input parameters */
  inp.size = grp->st_modulus_size;
  inp.pOp1 = grp->st_p;

  /* Launch the processing */
  MBEDTLS_MPI_CHK((HAL_PKA_MontgomeryParam(&hpka, &inp, ST_ECP_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Get Montgomery R2 parameters */
  HAL_PKA_MontgomeryParam_GetResult(&hpka, (uint32_t*)pt_montgomery);
  ECC_PointCheck.pMontgomeryParam = (uint32_t*)pt_montgomery;

  /* Launch the point check */
  MBEDTLS_MPI_CHK((HAL_PKA_PointCheck(&hpka, &ECC_PointCheck, ST_ECP_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Get the result of the point check */
  if( HAL_PKA_PointCheck_IsOnCurve(&hpka) != 1U)
  {
	ret = MBEDTLS_ERR_ECP_INVALID_KEY;
  }

cleanup:
  /* De-initialize HW peripheral */
  HAL_PKA_DeInit(&hpka);

  /* Disable HW peripheral clock */
  __HAL_RCC_PKA_CLK_DISABLE();

  /* Free memory */
  if (pt_binary != NULL)
  {
    mbedtls_platform_zeroize(pt_binary, (2U * grp->st_modulus_size) + 1U);
    mbedtls_free(pt_binary);
  }

  if (pt_montgomery != NULL)
  {
    mbedtls_platform_zeroize(pt_montgomery, grp->st_modulus_size);
    mbedtls_free(pt_montgomery);
  }

  return ret;
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
/*
 * R = m * P with shortcuts for m == 1 and m == -1
 * NOT constant-time - ONLY for short Weierstrass!
 */
static int mbedtls_ecp_mul_shortcuts(mbedtls_ecp_group *grp,
                                     mbedtls_ecp_point *R,
                                     const mbedtls_mpi *m,
                                     const mbedtls_ecp_point *P,
                                     mbedtls_ecp_restart_ctx *rs_ctx)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  if (mbedtls_mpi_cmp_int(m, 1) == 0)
  {
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
  }
  else if (mbedtls_mpi_cmp_int(m, -1) == 0)
  {
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
    if (mbedtls_mpi_cmp_int(&R->MBEDTLS_PRIVATE(Y), 0) != 0)
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(&R->MBEDTLS_PRIVATE(Y), &grp->P, &R->MBEDTLS_PRIVATE(Y)));
    }
  }
  else
  {
    MBEDTLS_MPI_CHK(mbedtls_ecp_mul_restartable(grp, R, m, P,
                                                NULL, NULL, rs_ctx));
  }

cleanup:
  return ret;
}

/*
 * Restartable linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd_restartable(
  mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
  const mbedtls_mpi *m, const mbedtls_ecp_point *P,
  const mbedtls_mpi *n, const mbedtls_ecp_point *Q,
  mbedtls_ecp_restart_ctx *rs_ctx)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_ecp_point mP;
  mbedtls_ecp_point *pmP = &mP;
  mbedtls_ecp_point *pR = R;
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  char is_grp_capable = 0;
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

  if (mbedtls_ecp_get_type(grp) != MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
  }

  mbedtls_ecp_point_init(&mP);

  ECP_RS_ENTER(ma);

#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL && rs_ctx->ma != NULL)
  {
    /* redirect intermediate results to restart context */
    pmP = &rs_ctx->ma->mP;
    pR  = &rs_ctx->ma->R;

    /* jump to next operation */
    if (rs_ctx->ma->STATE == ECP_RSMA_MUL2)
    {
      goto mul2;
    }
    if (rs_ctx->ma->STATE == ECP_RSMA_ADD)
    {
      goto add;
    }
    if (rs_ctx->ma->STATE == ECP_RSMA_NORM)
    {
      goto norm;
    }
  }
#endif /* MBEDTLS_ECP_RESTARTABLE */

  MBEDTLS_MPI_CHK(mbedtls_ecp_mul_shortcuts(grp, pmP, m, P, rs_ctx));
#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL && rs_ctx->ma != NULL)
  {
    rs_ctx->ma->STATE = ECP_RSMA_MUL2;
  }

mul2:
#endif /* MBEDTLS_ECP_RESTARTABLE */
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul_shortcuts(grp, pR,  n, Q, rs_ctx));

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  if ((is_grp_capable = mbedtls_internal_ecp_grp_capable(grp)))
  {
    MBEDTLS_MPI_CHK(mbedtls_internal_ecp_init(grp));
  }
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL && rs_ctx->ma != NULL)
  {
    rs_ctx->ma->STATE = ECP_RSMA_ADD;
  }

add:
#endif /* MBEDTLS_ECP_RESTARTABLE */
  MBEDTLS_ECP_BUDGET(MBEDTLS_ECP_OPS_ADD);
  MBEDTLS_MPI_CHK(ecp_add_mixed(grp, pR, pmP, pR));
#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL && rs_ctx->ma != NULL)
  {
    rs_ctx->ma->STATE = ECP_RSMA_NORM;
  }

norm:
#endif /* MBEDTLS_ECP_RESTARTABLE */
  MBEDTLS_ECP_BUDGET(MBEDTLS_ECP_OPS_INV);
  MBEDTLS_MPI_CHK(ecp_normalize_jac(grp, pR));

#if defined(MBEDTLS_ECP_RESTARTABLE)
  if (rs_ctx != NULL && rs_ctx->ma != NULL)
  {
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, pR));
  }
#endif /* MBEDTLS_ECP_RESTARTABLE */

cleanup:
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
  if (is_grp_capable)
  {
    mbedtls_internal_ecp_free(grp);
  }
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

  mbedtls_ecp_point_free(&mP);

  ECP_RS_LEAVE(ma);

  return ret;
}

/*
 * Linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd(mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                       const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                       const mbedtls_mpi *n, const mbedtls_ecp_point *Q)
{
  return mbedtls_ecp_muladd_restartable(grp, R, m, P, n, Q, NULL);
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
/*
 * Check validity of a public key for Montgomery curves with x-only schemes
 */
static int ecp_check_pubkey_mx(const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt)
{
  /* [Curve25519 p. 5] Just check X is the correct number of bytes */
  /* Allow any public value, if it's too big then we'll just reduce it mod p
   * (RFC 7748 sec. 5 para. 3). */
  if (mbedtls_mpi_size(&pt->X) > (grp->nbits + 7) / 8)
  {
    return MBEDTLS_ERR_ECP_INVALID_KEY;
  }

  return 0;
}
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

/*
 * Check that a point is valid as a public key
 */
int mbedtls_ecp_check_pubkey(const mbedtls_ecp_group *grp,
                             const mbedtls_ecp_point *pt)
{
  /* Must use affine coordinates */
  if (mbedtls_mpi_cmp_int(&pt->MBEDTLS_PRIVATE(Z), 1) != 0)
  {
    return MBEDTLS_ERR_ECP_INVALID_KEY;
  }

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    return ecp_check_pubkey_mx(grp, pt);
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    return ecp_check_pubkey_sw(grp, pt);
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */
  return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
}

/*
 * Check that an mbedtls_mpi is valid as a private key
 */
int mbedtls_ecp_check_privkey(const mbedtls_ecp_group *grp,
                              const mbedtls_mpi *d)
{
#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    /* see RFC 7748 sec. 5 para. 5 */
    if (mbedtls_mpi_get_bit(d, 0) != 0 ||
        mbedtls_mpi_get_bit(d, 1) != 0 ||
        mbedtls_mpi_bitlen(d) - 1 != grp->nbits)    /* mbedtls_mpi_bitlen is one-based! */
    {
      return MBEDTLS_ERR_ECP_INVALID_KEY;
    }

    /* see [Curve25519] page 5 */
    if (grp->nbits == (ECP_CURVE25519_KEY_SIZE * 8 - 2) && mbedtls_mpi_get_bit(d, 2) != 0)
    {
      return MBEDTLS_ERR_ECP_INVALID_KEY;
    }

    return 0;
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    /* see SEC1 3.2 */
    if (mbedtls_mpi_cmp_int(d, 1) < 0 ||
        mbedtls_mpi_cmp_mpi(d, &grp->N) >= 0)
    {
      return MBEDTLS_ERR_ECP_INVALID_KEY;
    }
    else
    {
      return 0;
    }
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

  return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
}

/*
 * Generate a private key
 */
int mbedtls_ecp_gen_privkey(const mbedtls_ecp_group *grp,
                            mbedtls_mpi *d,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng)
{
  int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  size_t n_size;

  n_size = (grp->nbits + 7) / 8;

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    /* [M225] page 5 */
    size_t b;

    do
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(d, n_size, f_rng, p_rng));
    } while (mbedtls_mpi_bitlen(d) == 0);

    /* Make sure the most significant bit is nbits */
    b = mbedtls_mpi_bitlen(d) - 1;   /* mbedtls_mpi_bitlen is one-based */
    if (b > grp->nbits)
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(d, b - grp->nbits));
    }
    else
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(d, grp->nbits, 1));

    /* Make sure the last two bits are unset for Curve448, three bits for
       Curve25519 */
    MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(d, 0, 0));
    MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(d, 1, 0));
    if (grp->nbits == (ECP_CURVE25519_KEY_SIZE * 8 - 2))
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(d, 2, 0));
    }
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(grp) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    /* SEC1 3.2.1: Generate d such that 1 <= n < N */
    int count = 0;
    unsigned cmp = 0;

    /*
     * Match the procedure given in RFC 6979 (deterministic ECDSA):
     * - use the same byte ordering;
     * - keep the leftmost nbits bits of the generated octet string;
     * - try until result is in the desired range.
     * This also avoids any biais, which is especially important for ECDSA.
     */
    do
    {
      MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(d, n_size, f_rng, p_rng));
      MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(d, 8 * n_size - grp->nbits));

      /*
       * Each try has at worst a probability 1/2 of failing (the msb has
       * a probability 1/2 of being 0, and then the result will be < N),
       * so after 30 tries failure probability is a most 2**(-30).
       *
       * For most curves, 1 try is enough with overwhelming probability,
       * since N starts with a lot of 1s in binary, but some curves
       * such as secp224k1 are actually very close to the worst case.
       */
      if (++count > 30)
      {
        ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
        goto cleanup;
      }

      ret = mbedtls_mpi_lt_mpi_ct(d, &grp->N, &cmp);
      if (ret != 0)
      {
        goto cleanup;
      }
    } while (mbedtls_mpi_cmp_int(d, 1) < 0 || cmp != 1);
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

cleanup:
  return ret;
}

/*
 * Generate a key pair with configurable base point
 */
int mbedtls_ecp_gen_keypair_base(mbedtls_ecp_group *grp,
                                 const mbedtls_ecp_point *G,
                                 mbedtls_mpi *d, mbedtls_ecp_point *Q,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(grp, d, f_rng, p_rng));
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul(grp, Q, d, G, f_rng, p_rng));

cleanup:
  return ret;
}

/*
 * Generate key pair, wrapper for conventional base point
 */
int mbedtls_ecp_gen_keypair(mbedtls_ecp_group *grp,
                            mbedtls_mpi *d, mbedtls_ecp_point *Q,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng)
{
  return mbedtls_ecp_gen_keypair_base(grp, &grp->G, d, Q, f_rng, p_rng);
}

/*
 * Generate a key pair, prettier wrapper
 */
int mbedtls_ecp_gen_key(mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                        int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

  if ((ret = mbedtls_ecp_group_load(&key->MBEDTLS_PRIVATE(grp), grp_id)) != 0)
  {
    return ret;
  }

  return mbedtls_ecp_gen_keypair(&key->MBEDTLS_PRIVATE(grp), &key->MBEDTLS_PRIVATE(d), &key->MBEDTLS_PRIVATE(Q), f_rng,
                                 p_rng);
}

#define ECP_CURVE25519_KEY_SIZE 32
/*
 * Read a private key.
 */
int mbedtls_ecp_read_key(mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                         const unsigned char *buf, size_t buflen)
{
  int ret = 0;

  if ((ret = mbedtls_ecp_group_load(&key->MBEDTLS_PRIVATE(grp), grp_id)) != 0)
  {
    return ret;
  }

  ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(&key->grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    /*
     * If it is Curve25519 curve then mask the key as mandated by RFC7748
     */
    if (grp_id == MBEDTLS_ECP_DP_CURVE25519)
    {
      if (buflen != ECP_CURVE25519_KEY_SIZE)
      {
        return MBEDTLS_ERR_ECP_INVALID_KEY;
      }

      MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary_le(&key->d, buf, buflen));

      /* Set the three least significant bits to 0 */
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 0, 0));
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 1, 0));
      MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&key->d, 2, 0));

      /* Set the most significant bit to 0 */
      MBEDTLS_MPI_CHK(
        mbedtls_mpi_set_bit(&key->d,
                            ECP_CURVE25519_KEY_SIZE * 8 - 1, 0)
      );

      /* Set the second most significant bit to 1 */
      MBEDTLS_MPI_CHK(
        mbedtls_mpi_set_bit(&key->d,
                            ECP_CURVE25519_KEY_SIZE * 8 - 2, 1)
      );
    }
    else
    {
      ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
  }

#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(&key->MBEDTLS_PRIVATE(grp)) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&key->MBEDTLS_PRIVATE(d), buf, buflen));

    MBEDTLS_MPI_CHK(mbedtls_ecp_check_privkey(&key->MBEDTLS_PRIVATE(grp), &key->MBEDTLS_PRIVATE(d)));
  }

#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */
cleanup:

  if (ret != 0)
  {
    mbedtls_mpi_free(&key->MBEDTLS_PRIVATE(d));
  }

  return ret;
}

/*
 * Write a private key.
 */
#if !defined(MBEDTLS_DEPRECATED_REMOVED)
int mbedtls_ecp_write_key(mbedtls_ecp_keypair *key,
                          unsigned char *buf, size_t buflen)
{
  int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(&key->grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
  {
    if (key->grp.id == MBEDTLS_ECP_DP_CURVE25519)
    {
      if (buflen < ECP_CURVE25519_KEY_SIZE)
      {
        return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
      }

      MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary_le(&key->d, buf, buflen));
    }
    else
    {
      ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
  }

#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(&key->MBEDTLS_PRIVATE(grp)) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
  {
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&key->MBEDTLS_PRIVATE(d), buf, buflen));
  }

#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */
cleanup:

  return ret;
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */

int mbedtls_ecp_write_key_ext(const mbedtls_ecp_keypair *key,
                              size_t *olen, unsigned char *buf, size_t buflen)
{
  size_t len = (key->MBEDTLS_PRIVATE(grp).nbits + 7) / 8;
  if (len > buflen)
  {
    /* For robustness, ensure *olen <= buflen even on error. */
    *olen = 0;
    return MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL;
  }
  *olen = len;

  /* Private key not set */
  if (key->MBEDTLS_PRIVATE(d).MBEDTLS_PRIVATE(n) == 0)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
  if (mbedtls_ecp_get_type(&key->MBEDTLS_PRIVATE(grp) == MBEDTLS_ECP_TYPE_MONTGOMERY)
{
  return mbedtls_mpi_write_binary_le(&key->d, buf, len);
  }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
  if (mbedtls_ecp_get_type(&key->MBEDTLS_PRIVATE(grp)) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS)
{
  return mbedtls_mpi_write_binary(&key->MBEDTLS_PRIVATE(d), buf, len);
  }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

  /* Private key set but no recognized curve type? This shouldn't happen. */
  return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
}

/*
 * Write a public key.
 */
int mbedtls_ecp_write_public_key(const mbedtls_ecp_keypair *key,
                                 int format, size_t *olen,
                                 unsigned char *buf, size_t buflen)
{
  return mbedtls_ecp_point_write_binary(&key->MBEDTLS_PRIVATE(grp), &key->MBEDTLS_PRIVATE(Q),
                                        format, olen, buf, buflen);
}

/*
 * Check a public-private key pair
 */
int mbedtls_ecp_check_pub_priv(
  const mbedtls_ecp_keypair *pub, const mbedtls_ecp_keypair *prv,
  int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  mbedtls_ecp_point Q;
  mbedtls_ecp_group grp;

  if (pub->MBEDTLS_PRIVATE(grp).id == MBEDTLS_ECP_DP_NONE ||
      pub->MBEDTLS_PRIVATE(grp).id != prv->MBEDTLS_PRIVATE(grp).id ||
      mbedtls_mpi_cmp_mpi(&pub->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X)) ||
      mbedtls_mpi_cmp_mpi(&pub->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y)) ||
      mbedtls_mpi_cmp_mpi(&pub->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Z), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Z)))
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  mbedtls_ecp_point_init(&Q);
  mbedtls_ecp_group_init(&grp);

  /* mbedtls_ecp_mul() needs a non-const group... */
  mbedtls_ecp_group_copy(&grp, &prv->MBEDTLS_PRIVATE(grp));

  /* Also checks d is valid */
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &Q, &prv->MBEDTLS_PRIVATE(d), &prv->MBEDTLS_PRIVATE(grp).G, f_rng, p_rng));

  if (mbedtls_mpi_cmp_mpi(&Q.MBEDTLS_PRIVATE(X), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X)) ||
      mbedtls_mpi_cmp_mpi(&Q.MBEDTLS_PRIVATE(Y), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y)) ||
      mbedtls_mpi_cmp_mpi(&Q.MBEDTLS_PRIVATE(Z), &prv->MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Z)))
  {
    ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    goto cleanup;
  }

cleanup:
  mbedtls_ecp_point_free(&Q);
  mbedtls_ecp_group_free(&grp);

  return ret;
}

#if defined(MBEDTLS_SELF_TEST)

/*
 * Checkup routine
 */
int mbedtls_ecp_self_test(int verbose)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  size_t i;
  mbedtls_ecp_group grp;
  mbedtls_ecp_point R, P;
  mbedtls_mpi m;
  unsigned long add_c_prev, dbl_c_prev, mul_c_prev;
  /* exponents especially adapted for secp192r1 */
  const char *exponents[] =
  {
    "000000000000000000000000000000000000000000000001", /* one */
    "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22830", /* N - 1 */
    "5EA6F389A38B8BC81E767753B15AA5569E1782E30ABE7D25", /* random */
    "400000000000000000000000000000000000000000000000", /* one and zeros */
    "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", /* all ones */
    "555555555555555555555555555555555555555555555555", /* 101010... */
  };

  mbedtls_ecp_group_init(&grp);
  mbedtls_ecp_point_init(&R);
  mbedtls_ecp_point_init(&P);
  mbedtls_mpi_init(&m);

  /* Use secp192r1 if available, or any available curve */
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
  MBEDTLS_MPI_CHK(mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP192R1));
#else
  MBEDTLS_MPI_CHK(mbedtls_ecp_group_load(&grp, mbedtls_ecp_curve_list()->grp_id));
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

  if (verbose != 0)
  {
    mbedtls_printf("  ECP test #1 (constant op_count, base point G): ");
  }

  /* Do a dummy multiplication first to trigger precomputation */
  MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&m, 2));
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &P, &m, &grp.G, NULL, NULL));

  add_count = 0;
  dbl_count = 0;
  mul_count = 0;
  MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&m, 16, exponents[0]));
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &R, &m, &grp.G, NULL, NULL));

  for (i = 1; i < sizeof(exponents) / sizeof(exponents[0]); i++)
  {
    add_c_prev = add_count;
    dbl_c_prev = dbl_count;
    mul_c_prev = mul_count;
    add_count = 0;
    dbl_count = 0;
    mul_count = 0;

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&m, 16, exponents[i]));
    MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &R, &m, &grp.G, NULL, NULL));

    if (add_count != add_c_prev ||
        dbl_count != dbl_c_prev ||
        mul_count != mul_c_prev)
    {
      if (verbose != 0)
      {
        mbedtls_printf("failed (%u)\n", (unsigned int) i);
      }

      ret = 1;
      goto cleanup;
    }
  }

  if (verbose != 0)
  {
    mbedtls_printf("passed\n");
  }

  if (verbose != 0)
  {
    mbedtls_printf("  ECP test #2 (constant op_count, other point): ");
  }
  /* We computed P = 2G last time, use it */

  add_count = 0;
  dbl_count = 0;
  mul_count = 0;
  MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&m, 16, exponents[0]));
  MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &R, &m, &P, NULL, NULL));

  for (i = 1; i < sizeof(exponents) / sizeof(exponents[0]); i++)
  {
    add_c_prev = add_count;
    dbl_c_prev = dbl_count;
    mul_c_prev = mul_count;
    add_count = 0;
    dbl_count = 0;
    mul_count = 0;

    MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&m, 16, exponents[i]));
    MBEDTLS_MPI_CHK(mbedtls_ecp_mul(&grp, &R, &m, &P, NULL, NULL));

    if (add_count != add_c_prev ||
        dbl_count != dbl_c_prev ||
        mul_count != mul_c_prev)
    {
      if (verbose != 0)
      {
        mbedtls_printf("failed (%u)\n", (unsigned int) i);
      }

      ret = 1;
      goto cleanup;
    }
  }

  if (verbose != 0)
  {
    mbedtls_printf("passed\n");
  }

cleanup:

  if (ret < 0 && verbose != 0)
  {
    mbedtls_printf("Unexpected error, return code = %08X\n", ret);
  }

  mbedtls_ecp_group_free(&grp);
  mbedtls_ecp_point_free(&R);
  mbedtls_ecp_point_free(&P);
  mbedtls_mpi_free(&m);

  if (verbose != 0)
  {
    mbedtls_printf("\n");
  }

  return ret;
}

#endif /* MBEDTLS_SELF_TEST */

#endif /* MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
