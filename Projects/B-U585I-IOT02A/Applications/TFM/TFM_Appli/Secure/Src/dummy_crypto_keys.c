/*
 * Copyright (c) 2017-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/include/tfm_plat_crypto_keys.h"
#include "tfm_secure_api.h"
#include "tfm_boot_status.h"
#include "service_api.h"
#include "region_defs.h"
#include <string.h>
#include <stddef.h>

/* FIXME: Functions in this file should be implemented by platform vendor. For
 * the security of the storage system, it is critical to use a hardware unique
 * key. For the security of the attestation, it is critical to use a unique key
 * pair and keep the private key is secret.
 */

#define TFM_HUK_KEY_LEN_BYTES  16
#define TFM_IAS_KEY_LEN_BYTES  32

#define TFM_PROVISION_HUK_LEN_BYTES 32

/**
  * \brief Recopy HUK key
  *
  * \param[out]  key    Pointer to buffer where to store the key
  * \param[in]   size   Length of the key
  *
  */

enum tfm_plat_err_t tfm_plat_get_huk_derived_key(const uint8_t *label,
                                                 size_t label_size,
                                                 const uint8_t *context,
                                                 size_t context_size,
                                                 uint8_t *key,
                                                 size_t key_size)
{
#if !defined(USE_HUK)
  enum tfm_status_e tfm_res;
  uint8_t full_huk[TFM_PROVISION_HUK_LEN_BYTES];
  uint32_t i;
  if (key_size > TFM_HUK_KEY_LEN_BYTES)
  {
    return TFM_PLAT_ERR_SYSTEM_ERR;
  }

  tfm_res = (enum tfm_status_e)tfm_core_get_boot_value(SET_TLV_TYPE(TLV_MAJOR_PS, TLV_MINOR_CORE_HUK),full_huk, sizeof(full_huk));
  if (tfm_res != TFM_SUCCESS)
  {
    return  TFM_PLAT_ERR_SYSTEM_ERR;
  }
  for (i = 0; i < key_size ; i++)
  {
      key[i]  = full_huk[i];
      full_huk[i]=0;
  }
#endif /* !defined(USE_HUK) */
  return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_get_initial_attest_key(uint8_t *key_buf,
                                uint32_t size,
                                struct ecc_key_t *ecc_key,
                                psa_ecc_family_t *curve_type)
{
#if (MCUBOOT_S_DATA_IMAGE_NUMBER != 1)
  enum tfm_status_e tfm_res;
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER != 1) */
  uint8_t *key_dst;
  uint32_t key_size;
  uint32_t full_key_size = 3 * TFM_IAS_KEY_LEN_BYTES;
  if (size < full_key_size)
  {
    return TFM_PLAT_ERR_SYSTEM_ERR;
  }

  /* Set the EC curve type which the key belongs to */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  *curve_type = PSA_ECC_CURVE_SECP256R1;
#else
  /* EC curve is in BL2 shared area */
  tfm_res = (enum tfm_status_e)tfm_core_get_boot_value(SET_TLV_TYPE(TLV_MAJOR_CORE, TLV_MINOR_CORE_IAS_KEY_TYPE),
                                    curve_type, sizeof(*curve_type));
  if (tfm_res != TFM_SUCCESS)
  {
    return  TFM_PLAT_ERR_SYSTEM_ERR;
  }
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */

  key_dst = key_buf;

  key_size = TFM_IAS_KEY_LEN_BYTES;
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  /* Initial attestation key is in Secure Data Image */
  memcpy(key_dst, (void *)(FLASH_BASE_S + S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + S_DATA_IMAGE_IAT_PRIV_KEY_OFFSET), key_size);
#else
  /* Initial attestation key is in BL2 shared area */
  tfm_res = (enum tfm_status_e)tfm_core_get_boot_value(SET_TLV_TYPE(TLV_MAJOR_CORE, TLV_MINOR_CORE_IAS_KEY_PRIV),
                                    key_dst, key_size);
  if (tfm_res != TFM_SUCCESS)
  {
    return  TFM_PLAT_ERR_SYSTEM_ERR;
  }
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
  /*   */
  ecc_key->priv_key = key_dst;
  ecc_key->priv_key_size = key_size;

  ecc_key->pubx_key = NULL;
  ecc_key->pubx_key_size = 0;
  ecc_key->puby_key = NULL;
  ecc_key->puby_key_size = 0;

  return TFM_PLAT_ERR_SUCCESS;
}
