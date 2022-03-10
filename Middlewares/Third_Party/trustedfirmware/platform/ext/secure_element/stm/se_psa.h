/**
  ******************************************************************************
  * @file    se_psa.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @brief   SE_PSA interface include file.
  *          Specifies device specific defines, macros, types.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SE_PSA_H__
#define __SE_PSA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "psa/crypto_se_driver.h"
#include "crypto.h"
#include "se_stsafea_api.h"
#include "se_psa_id.h"


/** This macro checks if the result of an `expression` is equal to an
  *  `expected` value and sets a `status` variable of type `psa_status_t` to
  *  `PSA_SUCCESS`. If they are not equal, the `status` is set to
  *  `psa_error instead`, the error details are printed, and the code jumps
  *  to the `exit` label. */
#define ASSERT_STATUS(expression, expected, psa_error)              \
  do                                                              \
  {                                                               \
    SE_API_Status_t ASSERT_result = (expression);                   \
    SE_API_Status_t ASSERT_expected = (expected);                   \
    if ((ASSERT_result) != (ASSERT_expected))                   \
    {                                                           \
      printf("assertion failed in %s:%d "                     \
             "(actual=%d expected=%d)\n", __func__, __LINE__, \
             ASSERT_result, ASSERT_expected);                 \
      status = (psa_error);                                   \
      goto exit;                                              \
    }                                                           \
    status = PSA_SUCCESS;                                       \
  } while(0)

/** Check if an ATCA operation is successful, translate the error otherwise. */
#define ASSERT_SUCCESS(expression) ASSERT_STATUS(expression, SE_OK, \
                                                 se_st_to_psa_error(ASSERT_result))

/** Does the same as the macro above, but without the error translation and for
  *  the PSA return code - PSA_SUCCESS.*/
#define ASSERT_SUCCESS_PSA(expression) ASSERT_STATUS(expression, PSA_SUCCESS, \
                                                     ASSERT_result)
#define SE_ST_256_PRIV_KEY_SIZE 32
#define SE_ST_384_PRIV_KEY_SIZE 48

#define SE_TO_PSA_CURVE(__curve__) (((__curve__ == SE_KEY_TYPE_NIST_256)\
                                                     || (__curve__ == SE_KEY_TYPE_NIST_384)) ? PSA_ECC_FAMILY_SECP_R1 :\
                                                     PSA_ECC_FAMILY_BRAINPOOL_P_R1)


typedef struct
{
uint32_t lifetime;
psa_key_id_t id;
uint32_t alg;
uint32_t usage;
size_t bits;
uint16_t type;
}  psa_se_st_key_attribute_t;

typedef struct
{
uint16_t nb_key;
psa_se_st_key_attribute_t *keys;
}       psa_se_st_key_map_t;

typedef struct
{
size_t bits;
uint32_t lifetime;
psa_key_id_t id;
}  psa_se_st_mapping_attribute_t;

extern const psa_drv_se_t psa_se_st;


/* Exported functions --------------------------------------------------------*/
/**
 * \brief          This function registers the Secure Element keys.
 *
 *                 Keys can be a physical
 *                 or  logical  (they are called function keys).
 *
 *                 For instance, function key can embed the memory mapping
 *                 of the regions available in the Secure Element
 */
psa_status_t register_se_keys(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SE_PSA_H__ */


