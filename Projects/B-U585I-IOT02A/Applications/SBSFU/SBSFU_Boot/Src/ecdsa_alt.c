/**
  * Elliptic curve DSA sign and verify functions
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
  * This file implements ST ECDSA sign and verify HW services based on mbed TLS API
  */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/ecdsa.h"
#include <string.h>

#if defined(MBEDTLS_ECDSA_C)
#if defined(MBEDTLS_ECP_ALT)
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"
#include "stm32u5xx_hal.h"
#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
#include "boot_hal_imagevalid.h"
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ST_ECDSA_TIMEOUT     (5000U)
#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
#define PKA_ECDSA_VERIF_OUT_SIGNATURE_R                ((0x0578UL - PKA_RAM_OFFSET)>>2)   /*!< Output result */
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if !defined(MBEDTLS_ECP_ALT)
#error "MBEDTLS_ECP_ALT must be defined, if MBEDTLS_ECDSA_SIGN_ALT is"
#endif /* !MBEDTLS_ECP_ALT */

#if defined(MBEDTLS_ECDSA_SIGN_ALT) || defined(MBEDTLS_ECDSA_VERIFY_ALT)
/**
  * @brief      Prepare the digest for respecting digest length = Prime order length
  * @param[in]  P_pDigestIn        Input digest
  * @param[in]  P_DigestInLength   Input digest length
  * @param[out] P_pDigestOut       Output digest
  * @param[in]  P_DigestOutLength  Output digest length
  * @param[out] P_pPrimeOrder      Prime order of the selected curve
  * @retval     none
  */
static void ecc_hal_prepare_digest(const uint8_t *P_pDigestIn, uint8_t P_DigestInLength,
                                   uint8_t *P_pDigestOut, uint8_t P_DigestOutLength, uint8_t *P_pPrimeOrder)
{

  uint8_t i;                                            /* Counter */
  uint8_t propagation = 0;                              /* For internal use */

  /* Prepare P_pDigestIn for being equal to P_DigestOutLength */
  if (P_DigestInLength < P_DigestOutLength)
  {
    /* Padding of P_pDigestIn at the beginning */
    for (i = 0; i < (P_DigestOutLength - P_DigestInLength); i++)
    {
      P_pDigestOut[i] = 0x00;
    }
    /* Copy the digest */
    (void) memcpy(&P_pDigestOut[P_DigestOutLength - P_DigestInLength], P_pDigestIn, P_DigestInLength);
  }
  else if (P_DigestInLength == P_DigestOutLength)
  {
    if (memcmp(P_pDigestIn, P_pPrimeOrder, P_DigestOutLength) < 0) /* P_pDigestIn < P_pPrimeOrder */
    {
      /* Use digest as provide by user */
      (void) memcpy(P_pDigestOut, P_pDigestIn, P_DigestInLength);
    }
    else /* P_pDigestIn => P_pPrimeOrder */
    {
      /* Copy P_pDigestIn in final digest, temporary operation */
      (void) memcpy(P_pDigestOut, P_pDigestIn, P_DigestInLength);

      /* Reduce digest. digest = digest - prime order */
      for (i = (P_DigestOutLength - 1u); i >= 1u; i--)
      {
        /* Manage the restrains */
        if (propagation == 1u)
        {
          if (P_pDigestOut[i] == 0x00u)
          {
            propagation = 1; /* Keep the restrains */
            P_pDigestOut[i] = 0xFFu;
          }
          else
          {
            propagation = 0u;
            P_pDigestOut[i] = P_pDigestOut[i] - 1u;
          }
        }
        if (P_pDigestOut[i] >= P_pPrimeOrder[i])
        {
          /* P_pDigestIn byte bigger than P_pPrimeOrder, easy to subtract */
          P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
        }
        else
        {
          /* P_pDigestIn byte is smaller than P_pPrimeOrder */
          P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
          propagation = 1;
        }
      }
      /* Case of last byte outside the loop for misra */
      if (propagation == 1u)
      {
        P_pDigestOut[0] = P_pDigestOut[0] - 1u;
      }
      /* P_pDigestOut is obligatory bigger than P_pPrimeOrder */
      P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
    } /* P_pDigestIn => P_pPrimeOrder */
  }
  else /* P_DigestInLength > P_DigestOutLength */
  {
    /* Keep only first P_DigestOutLength bit of the digest */
    (void) memcpy(P_pDigestOut, P_pDigestIn, P_DigestOutLength);

    if (memcmp(P_pDigestOut, P_pPrimeOrder, P_DigestOutLength) < 0) /* P_pDigestOut < P_pPrimeOrder */
    {
      /* Nothing to do */
    }
    else /* P_pDigestOut >=  P_pPrimeOrder */
    {
      /* Reduce digest. digest = digest - prime order */
      for (i = (P_DigestOutLength - 1u); i >= 1u; i--)
      {
        /* Manage the restrains */
        if (propagation == 1u)
        {
          if (P_pDigestOut[i] == 0x00u)
          {
            propagation = 1u; /* Keep the restrains */
            P_pDigestOut[i] = 0xFFu;
          }
          else
          {
            propagation = 0u;
            P_pDigestOut[i] = P_pDigestOut[i] - 1u;
          }
        }
        if (P_pDigestOut[i] >= P_pPrimeOrder[i])
        {
          /* P_pDigestIn byte bigger than P_pPrimeOrder, easy to subtract */
          P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
        }
        else
        {
          /* P_pDigestIn byte is smaller than P_pPrimeOrder */
          P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
          propagation = 1;
        }
        /* Case of last byte outside the loop for misra */
        if (propagation == 1u)
        {
          P_pDigestOut[0] = P_pDigestOut[0] - 1u;
        }
        /* P_pDigestOut is obligatory bigger than P_pPrimeOrder */
        P_pDigestOut[i] = P_pDigestOut[i] - P_pPrimeOrder[i];
      }
    }  /* P_pDigestOut >=  P_pPrimeOrder */
  } /* end digest preparation */
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT || MBEDTLS_ECDSA_VERIFY_ALT */

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                       const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                       int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  uint8_t *d_binary;
  uint8_t *k_binary = NULL;

  mbedtls_mpi k;
  PKA_HandleTypeDef hpka = {0};
  PKA_ECDSASignInTypeDef ECDSA_SignIn = {0};
  PKA_ECDSASignOutTypeDef ECDSA_SignOut = {0};

  uint8_t a_digest[66] ; /* Local digest after rework input digest, 66 is the maximum order size supported */

  /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
  if (! mbedtls_ecdsa_can_do(grp->id) || grp->N.MBEDTLS_PRIVATE(p) == NULL)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /* Make sure d is in range 1..n-1 */
  if (mbedtls_mpi_cmp_int(d, 1) < 0 || mbedtls_mpi_cmp_mpi(d, &grp->N) >= 0)
  {
    return MBEDTLS_ERR_ECP_INVALID_KEY;
  }

  /* Set HW peripheral Input parameter: curve coefs */
  ECDSA_SignIn.primeOrderSize = grp->st_order_size;
  ECDSA_SignIn.modulusSize    = grp->st_modulus_size;
  ECDSA_SignIn.modulus        = grp->st_p;
  ECDSA_SignIn.coefSign       = grp->st_a_sign;
  ECDSA_SignIn.coef           = grp->st_a_abs;
  ECDSA_SignIn.coefB          = grp->st_b;
  ECDSA_SignIn.basePointX     = grp->st_gx;
  ECDSA_SignIn.basePointY     = grp->st_gy;
  ECDSA_SignIn.primeOrder     = grp->st_n;

  /* Prepare the digest */
  ecc_hal_prepare_digest((uint8_t *)buf, (uint8_t)blen, a_digest, (uint8_t)grp->st_order_size, grp->st_n);
  /* Set HW peripheral input parameter: hash content buffer to be signed */
  ECDSA_SignIn.hash = a_digest;

  /* Set HW peripheral input parameter: private signing key */
  d_binary = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((d_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(d, d_binary, grp->st_order_size));

  ECDSA_SignIn.privateKey = d_binary;

  /* Set HW peripheral input parameter: random integer */
  mbedtls_mpi_init(&k);

  MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(grp, &k, f_rng, p_rng));

  k_binary = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((k_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&k, k_binary, grp->st_order_size));

  ECDSA_SignIn.integer = k_binary;

  /* Enable HW peripheral clock */
  __HAL_RCC_PKA_CLK_ENABLE();

  /* Initialize HW peripheral */
  hpka.Instance = PKA;
  MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  HAL_PKA_RAMReset(&hpka);

  /* Launch the signature */
  MBEDTLS_MPI_CHK((HAL_PKA_ECDSASign(&hpka, &ECDSA_SignIn,
                                     ST_ECDSA_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Allocate memory space for signature */
  ECDSA_SignOut.RSign = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((ECDSA_SignOut.RSign == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  ECDSA_SignOut.SSign = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((ECDSA_SignOut.SSign == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  /* Get the signature into allocated space */
  HAL_PKA_ECDSASign_GetResult(&hpka, &ECDSA_SignOut, NULL);

  /* Convert the signature into mpi format */
  MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(r, ECDSA_SignOut.RSign, grp->st_order_size));

  MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(s, ECDSA_SignOut.SSign, grp->st_order_size));

cleanup:
  /* De-initialize HW peripheral */
  HAL_PKA_DeInit(&hpka);

  /* Disable HW peripheral clock */
  __HAL_RCC_PKA_CLK_DISABLE();

  /* Free memory */
  mbedtls_mpi_free(&k);

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
#endif /* !MBEDTLS_ECP_ALT */

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
/**
  * @brief  Check PKA signature with a constant time execution.
  * @param  hpka PKA handle
  * @param  in Input information
  * @retval IMAGE_VALID if equal, IMAGE_INVALID otherwise.
  */
static int CheckPKASignature(PKA_HandleTypeDef *hpka, PKA_ECDSAVerifInTypeDef *in)
{
  __IO uint8_t result = 0;
  uint32_t i;
  uint32_t j;
  uint8_t *p_sign_PKA = (uint8_t *) &hpka->Instance->RAM[PKA_ECDSA_VERIF_OUT_SIGNATURE_R];
  uint8_t *pSign = (uint8_t *)in->RSign;
  uint32_t Size =  in->primeOrderSize;

  /* Signature comparison LSB vs MSB */
  for (i = 0U, j = Size - 1U; i < Size; i++, j--)
  {
    result |= pSign[i] ^ IMAGE_VALID ^ p_sign_PKA[j];
  }

  /* Loop fully executed ==> no basic HW attack */
  /* Any other unexpected result */
  if ((i != Size) || (result != IMAGE_VALID))
  {
    result = IMAGE_INVALID;
  }

  return result;
}
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify(mbedtls_ecp_group *grp,
                         const unsigned char *buf, size_t blen,
                         const mbedtls_ecp_point *Q,
                         const mbedtls_mpi *r,
                         const mbedtls_mpi *s)
{
  int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
  size_t olen = 0;
  uint8_t *Q_binary;
  uint8_t *r_binary = NULL;
  uint8_t *s_binary = NULL;
  PKA_HandleTypeDef hpka = {0};
  PKA_ECDSAVerifInTypeDef ECDSA_VerifyIn = {0};
  uint8_t a_digest[66] = {0}; /* Local digest after rework input digest, 66 is the maximum order size supported */

  /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
  if (grp->G.MBEDTLS_PRIVATE(Y).MBEDTLS_PRIVATE(p) == NULL)
  {
    return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
  }

  /* Make sure r and s are in range 1..n-1 */
  if (mbedtls_mpi_cmp_int(r, 1) < 0 || mbedtls_mpi_cmp_mpi(r, &grp->N) >= 0 ||
      mbedtls_mpi_cmp_int(s, 1) < 0 || mbedtls_mpi_cmp_mpi(s, &grp->N) >= 0)
  {
    return MBEDTLS_ERR_ECP_VERIFY_FAILED;
  }

  /* Set HW peripheral Input parameter: curve coefs */
  ECDSA_VerifyIn.primeOrderSize = grp->st_order_size;
  ECDSA_VerifyIn.modulusSize    = grp->st_modulus_size;
  ECDSA_VerifyIn.modulus        = grp->st_p;
  ECDSA_VerifyIn.coefSign       = grp->st_a_sign;
  ECDSA_VerifyIn.coef           = grp->st_a_abs;
  ECDSA_VerifyIn.basePointX     = grp->st_gx;
  ECDSA_VerifyIn.basePointY     = grp->st_gy;
  ECDSA_VerifyIn.primeOrder     = grp->st_n;

  /* Prepare the digest */
  ecc_hal_prepare_digest((uint8_t *)buf, (uint8_t)blen, a_digest, (uint8_t)grp->st_order_size, grp->st_n);
  /* Set HW peripheral input parameter: hash content buffer that was signed */
  ECDSA_VerifyIn.hash = a_digest;

  /* Set HW peripheral input parameter: public key */
  Q_binary = mbedtls_calloc((2U * grp->st_modulus_size) + 1U, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((Q_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_ecp_point_write_binary(grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, Q_binary,
                                                 (2U * grp->st_modulus_size) + 1U));

  ECDSA_VerifyIn.pPubKeyCurvePtX = Q_binary + 1U;
  ECDSA_VerifyIn.pPubKeyCurvePtY = Q_binary + grp->st_modulus_size + 1U;

  /* Set HW peripheral input parameter: signature to be verified */
  r_binary = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((r_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  s_binary = mbedtls_calloc(grp->st_order_size, sizeof(uint8_t));
  MBEDTLS_MPI_CHK((s_binary == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

  MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(r, r_binary, grp->st_order_size));
  ECDSA_VerifyIn.RSign = r_binary;

  MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(s, s_binary, grp->st_order_size));
  ECDSA_VerifyIn.SSign = s_binary;

  /* Enable HW peripheral clock */
  __HAL_RCC_PKA_CLK_ENABLE();

  /* Initialize HW peripheral */
  hpka.Instance = PKA;
  MBEDTLS_MPI_CHK((HAL_PKA_Init(&hpka) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  HAL_PKA_RAMReset(&hpka);

  /* Launch the signature verification */
  MBEDTLS_MPI_CHK((HAL_PKA_ECDSAVerif(&hpka, &ECDSA_VerifyIn,
                                      ST_ECDSA_TIMEOUT) != HAL_OK) ? MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED : 0);

  /* Check the result */
  MBEDTLS_MPI_CHK((HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != 1U) ? MBEDTLS_ERR_ECP_VERIFY_FAILED : 0);

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
  /* Double the signature verification (using another way) to resist to basic HW attacks.
   * The second verification is applicable to final signature check on primary slot images
   * only (condition: ImageValidEnable).
   * It is performed in 2 steps:
   * 1- save signature status in global variable ImageValidStatus[]
   *    Return value of HAL api (0 failed, 1 passed) is mul with IMAGE_VALID to avoid
   *    value 1 for success: IMAGE_VALID for success.
   * 2- verify saved signature status later in boot process
   */
  if (ImageValidEnable == 1)
  {
    /* Check ImageValidIndex is in expected range MCUBOOT_IMAGE_NUMBER */
    MBEDTLS_MPI_CHK((ImageValidIndex >= MCUBOOT_IMAGE_NUMBER) ? MBEDTLS_ERR_ECP_VERIFY_FAILED : 0);

    ImageValidStatus[ImageValidIndex++] = CheckPKASignature(&hpka, &ECDSA_VerifyIn);
  }

#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */
cleanup:
  /* De-initialize HW peripheral */
  HAL_PKA_DeInit(&hpka);

  /* Disable HW peripheral clock */
  __HAL_RCC_PKA_CLK_DISABLE();

  /* Free memory */
  if (Q_binary != NULL)
  {
    mbedtls_platform_zeroize(Q_binary, (2U * grp->st_modulus_size) + 1U);
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



#endif /* MBEDTLS_ECP_ALT */
#endif /* MBEDTLS_ECDSA_C */
