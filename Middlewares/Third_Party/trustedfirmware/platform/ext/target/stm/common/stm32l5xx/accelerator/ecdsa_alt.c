/*
 *  Elliptic curve DSA sign and verify functions
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
 *  This file implements ST ECDSA sign and verify HW services based on mbed TLS API
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/ecdsa.h"

#if defined(MBEDTLS_ECDSA_C)
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "stm32l5xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ST_ECDSA_TIMEOUT     (5000U)

/* Private macro -------------------------------------------------------------*/
/* Parameter validation macros based on platform_util.h */
#define ECDSA_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECDSA_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if defined(MBEDTLS_ECDSA_SIGN_ALT)

#if !defined(MBEDTLS_ECP_ALT)
#error "MBEDTLS_ECP_ALT must be defined, if MBEDTLS_ECDSA_SIGN_ALT is"
#endif

/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = 0;
    uint8_t *d_binary;
    uint8_t *k_binary = NULL;

    mbedtls_mpi k;
    PKA_HandleTypeDef hpka = {0};
    PKA_ECDSASignInTypeDef ECDSA_SignIn = {0};
    PKA_ECDSASignOutTypeDef ECDSA_SignOut;

    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( grp->G.Y.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure d is in range 1..n-1 */
    if( mbedtls_mpi_cmp_int( d, 1 ) < 0 || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return ( MBEDTLS_ERR_ECP_INVALID_KEY);

    /* Set HW peripheral Input parameter: curve coefs */
    ECDSA_SignIn.primeOrderSize = grp->st_order_size;
    ECDSA_SignIn.modulusSize    = grp->st_modulus_size;
    ECDSA_SignIn.modulus        = grp->st_p;
    ECDSA_SignIn.coefSign       = grp->st_a_sign;
    ECDSA_SignIn.coef           = grp->st_a_abs;
    ECDSA_SignIn.basePointX     = grp->st_gx;
    ECDSA_SignIn.basePointY     = grp->st_gy;
    ECDSA_SignIn.primeOrder     = grp->st_n;

    /* Set HW peripheral input parameter: hash content buffer to be signed */
    ECDSA_SignIn.hash = buf;

    /* Set HW peripheral input parameter: private signing key */
    d_binary = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((d_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, d_binary, grp->st_order_size ) );

    ECDSA_SignIn.privateKey = d_binary;

    /* Set HW peripheral input parameter: random integer */
    mbedtls_mpi_init( &k );

    MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, &k, f_rng, p_rng ) );

    k_binary = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((k_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &k, k_binary, grp->st_order_size ) );

    ECDSA_SignIn.integer = k_binary;

    /* Enable HW peripheral clock */
    __HAL_RCC_PKA_CLK_ENABLE();

    /* Initialize HW peripheral */
    hpka.Instance = PKA;
    MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

    HAL_PKA_RAMReset(&hpka);

    /* Launch the signature */
    MBEDTLS_MPI_CHK((HAL_PKA_ECDSASign(&hpka, &ECDSA_SignIn, ST_ECDSA_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

    /* Allocate memory space for signature */
    ECDSA_SignOut.RSign = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((ECDSA_SignOut.RSign == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    ECDSA_SignOut.SSign = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((ECDSA_SignOut.SSign == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    /* Get the signature into allocated space */
    HAL_PKA_ECDSASign_GetResult(&hpka, &ECDSA_SignOut, NULL);

    /* Convert the signature into mpi format */
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( r, ECDSA_SignOut.RSign, grp->st_order_size ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( s, ECDSA_SignOut.SSign, grp->st_order_size ) );

cleanup:
    /* De-initialize HW peripheral */
    HAL_PKA_DeInit(&hpka);

    /* Disable HW peripheral clock */
    __HAL_RCC_PKA_CLK_DISABLE();

    /* Free memory */
    mbedtls_mpi_free( &k );

    if (d_binary != NULL)
    {
        mbedtls_platform_zeroize(d_binary, grp->st_order_size);
        mbedtls_free(d_binary);
    }

    if (k_binary != NULL)
    {
        mbedtls_platform_zeroize(k_binary, grp->st_order_size);
        mbedtls_free(k_binary);
    }

    if (ECDSA_SignOut.RSign != NULL)
    {
        mbedtls_platform_zeroize(ECDSA_SignOut.RSign, grp->st_order_size);
        mbedtls_free(ECDSA_SignOut.RSign);
    }

    if (ECDSA_SignOut.SSign != NULL)
    {
        mbedtls_platform_zeroize(ECDSA_SignOut.SSign, grp->st_order_size);
        mbedtls_free(ECDSA_SignOut.SSign);
    }

    return ret;
}

#endif /* MBEDTLS_ECDSA_SIGN_ALT*/

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)

#if !defined(MBEDTLS_ECP_ALT)
#error "MBEDTLS_ECP_ALT must be defined, if MBEDTLS_ECDSA_VERIFY_ALT is"
#endif


/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                          const unsigned char *buf, size_t blen,
                          const mbedtls_ecp_point *Q,
                          const mbedtls_mpi *r,
                          const mbedtls_mpi *s)
{
    int ret = 0;
    size_t olen;
    uint8_t *Q_binary;
    uint8_t *r_binary = NULL;
    uint8_t *s_binary = NULL;
    PKA_HandleTypeDef hpka = {0};
    PKA_ECDSAVerifInTypeDef ECDSA_VerifyIn;

    /* Check parameters */
    ECDSA_VALIDATE_RET( grp != NULL );
    ECDSA_VALIDATE_RET( Q   != NULL );
    ECDSA_VALIDATE_RET( r   != NULL );
    ECDSA_VALIDATE_RET( s   != NULL );
    ECDSA_VALIDATE_RET( buf != NULL || blen == 0 );

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( grp->G.Y.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Make sure r and s are in range 1..n-1 */
    if( mbedtls_mpi_cmp_int( r, 1 ) < 0 || mbedtls_mpi_cmp_mpi( r, &grp->N ) >= 0 ||
        mbedtls_mpi_cmp_int( s, 1 ) < 0 || mbedtls_mpi_cmp_mpi( s, &grp->N ) >= 0 )
        return( MBEDTLS_ERR_ECP_VERIFY_FAILED );

    /* Set HW peripheral Input parameter: curve coefs */
    ECDSA_VerifyIn.primeOrderSize = grp->st_order_size;
    ECDSA_VerifyIn.modulusSize    = grp->st_modulus_size;
    ECDSA_VerifyIn.modulus        = grp->st_p;
    ECDSA_VerifyIn.coefSign       = grp->st_a_sign;
    ECDSA_VerifyIn.coef           = grp->st_a_abs;
    ECDSA_VerifyIn.basePointX     = grp->st_gx;
    ECDSA_VerifyIn.basePointY     = grp->st_gy;
    ECDSA_VerifyIn.primeOrder     = grp->st_n;

    /* Set HW peripheral input parameter: hash content buffer that was signed */
    ECDSA_VerifyIn.hash = buf;

    /* Set HW peripheral input parameter: public key */
    Q_binary = mbedtls_calloc( ( 2U * grp->st_modulus_size ) + 1U, sizeof( uint8_t ) );
    MBEDTLS_MPI_CHK((Q_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, Q_binary, ( 2U * grp->st_modulus_size ) + 1U ) );

    ECDSA_VerifyIn.pPubKeyCurvePtX = Q_binary + 1U;
    ECDSA_VerifyIn.pPubKeyCurvePtY = Q_binary + grp->st_modulus_size + 1U;

    /* Set HW peripheral input parameter: signature to be verified */
    r_binary = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((r_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    s_binary = mbedtls_calloc(grp->st_order_size, sizeof( uint8_t ));
    MBEDTLS_MPI_CHK((s_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( r, r_binary, grp->st_order_size ) );
    ECDSA_VerifyIn.RSign = r_binary;

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( s, s_binary, grp->st_order_size ) );
    ECDSA_VerifyIn.SSign = s_binary;

    /* Enable HW peripheral clock */
    __HAL_RCC_PKA_CLK_ENABLE();

    /* Initialize HW peripheral */
    hpka.Instance = PKA;
    MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

    HAL_PKA_RAMReset(&hpka);

    /* Launch the signature verification */
    MBEDTLS_MPI_CHK((HAL_PKA_ECDSAVerif(&hpka, &ECDSA_VerifyIn, ST_ECDSA_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

    /* Check the result */
    MBEDTLS_MPI_CHK((HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != 1U) ? MBEDTLS_ERR_ECP_VERIFY_FAILED : 0);

cleanup:
    /* De-initialize HW peripheral */
    HAL_PKA_DeInit(&hpka);

    /* Disable HW peripheral clock */
    __HAL_RCC_PKA_CLK_DISABLE();

    /* Free memory */
    if (Q_binary != NULL)
    {
        mbedtls_platform_zeroize(Q_binary, ( 2U * grp->st_modulus_size ) + 1U );
        mbedtls_free(Q_binary);
    }

    if (r_binary != NULL)
    {
        mbedtls_platform_zeroize(r_binary, grp->st_order_size);
        mbedtls_free(r_binary);
    }

    if (s_binary != NULL)
    {
        mbedtls_platform_zeroize(s_binary, grp->st_order_size);
        mbedtls_free(s_binary);
    }

    return ret;
}

#endif /* MBEDTLS_ECDSA_VERIFY_ALT*/

#endif /* MBEDTLS_ECDSA_C */
