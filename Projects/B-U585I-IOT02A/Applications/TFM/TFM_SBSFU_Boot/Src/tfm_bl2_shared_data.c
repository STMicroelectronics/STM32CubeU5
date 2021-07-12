/**
  ******************************************************************************
  * @file    tfm_bl2_shared_data.c
  * @author  MCD Application Team
  * @brief   This files recopies data shared between BOOT and TFM Appli
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
/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "region_defs.h"
#include "tfm_boot_status.h"
#include "tfm_bl2_shared_data.h"
#include "low_level_rng.h"
#ifdef TFM_DEV_MODE
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_ERROR
#else
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_OFF
#endif /* TFM_DEV_MODE */
#include "bootutil/bootutil_log.h"
#include "boot_record.h"
#include "mbedtls/entropy_poll.h"
#include "platform/include/region.h"
#include "platform/include/tfm_plat_boot_seed.h"
#include "platform/include/tfm_attest_hal.h"
#include "platform/include/tfm_plat_crypto_keys.h"
#include "platform/include/tfm_plat_device_id.h"
#include "bootutil/sha256.h"
#include "bootutil/sign_key.h"


/* Macros to pick linker symbols */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

#if defined(__ICCARM__)
#define CODE_START (void *)(BL2_CODE_START)
#else
REGION_DECLARE(Image$$, ER_CODE, $$Base);
#define CODE_START &REGION_NAME(Image$$, ER_CODE, $$Base)
#endif /* __ICCARM__ */
#if defined (__ARMCC_VERSION)
#define LOAD_NAME(a,b,c,d) a##b##c##d
#define LOAD_DECLARE(a,b,c,d) extern uint32_t LOAD_NAME(a, b, c,d)
LOAD_DECLARE(Load$$, LR$$, LR_CODE, $$Limit);
#define CODE_LIMIT &LOAD_NAME(Load$$, LR$$, LR_CODE, $$Limit)
#else
REGION_DECLARE(Image$$, HDP_CODE, $$Limit);
#define CODE_LIMIT &REGION_NAME(Image$$, HDP_CODE, $$Limit)
#endif /*__ARMCC_VERSION */
/**
  * @}
  */
static void ComputeImplementationId(uint8_t *hash_result);

extern const uint8_t huk_value[];
extern const uint32_t huk_size;
extern const struct bootutil_key bootutil_priv_keys[];
extern const int bootutil_priv_key_cnt;

/** @defgroup TFM_BL2_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Recopy bl2 shared data
  * @param  None
  * @note   The boot seed is generated and recopied in BL2 shared data.
  * @note   tamper backup register is set privileged and secured.
  * @note   Attestation private key is recopied in tamper back up register.
  * @note   Attestation public key and hash is recopied in BL2 shared data.
  * @retval None
  */

void TFM_BL2_CopySharedData(void)
{
  /*  default value */
  enum shared_memory_err_t res;
  enum tfm_security_lifecycle_t lifecycle = TFM_SLC_SECURED;
  /* if 1 for TFM regression test */
  uint8_t boot_seed[BOOT_SEED_SIZE];
  uint8_t implementation_id[IMPLEMENTATION_ID_MAX_SIZE];
  size_t len = sizeof(boot_seed);
  mbedtls_hardware_poll(NULL, boot_seed, sizeof(boot_seed), &len);
  if (len != sizeof(boot_seed))
  {
    BOOT_LOG_ERR("BL2 BOOT SEED generation Failed !!");
    Error_Handler();
  }
  if (bootutil_priv_key_cnt < 3)
  {
    BOOT_LOG_ERR("No HUK in bootutil_priv_keys!!");
    Error_Handler();
  }
  res = boot_add_data_to_shared_area(TLV_MAJOR_SST,
                                     TLV_MINOR_CORE_HUK,
                                     *(bootutil_priv_keys[2].len),
                                     bootutil_priv_keys[2].key);
  if (res)
  {
    BOOT_LOG_ERR("BL2 HUK set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }
  BOOT_LOG_INF("BL2 HUK %x%x..%x%x set to BL2 SHARED DATA",
               *((uint32_t *)(&bootutil_priv_keys[2].key[0])),
               *((uint32_t *)(&bootutil_priv_keys[2].key[4])),
               *((uint32_t *)(&bootutil_priv_keys[2].key[24])),
               *((uint32_t *)(&bootutil_priv_keys[2].key[28]))
              );

  /* Add BOOT SEED to the shared data */
  res = boot_add_data_to_shared_area(TLV_MAJOR_IAS,
                                     TLV_MINOR_IAS_BOOT_SEED,
                                     sizeof(boot_seed),
                                     (const uint8_t *)boot_seed);
  if (res)
  {
    BOOT_LOG_ERR("BL2 SEED set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }
  BOOT_LOG_INF("BL2 SEED %x%x..%x%x set to BL2 SHARED DATA",
               *((uint32_t *)(&boot_seed[0])),
               *((uint32_t *)(&boot_seed[4])),
               *((uint32_t *)(&boot_seed[BOOT_SEED_SIZE-8])),
               *((uint32_t *)(&boot_seed[BOOT_SEED_SIZE-4]))
              );
  /* Add Hardcoded Lifecycle value */
  res = boot_add_data_to_shared_area(TLV_MAJOR_IAS,
                                     TLV_MINOR_IAS_SLC,
                                     sizeof(lifecycle),
                                     (const uint8_t *)&lifecycle);
  if (res)
  {
    BOOT_LOG_ERR("BL2 LFCYCLE set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }
  ComputeImplementationId(implementation_id);
  res = boot_add_data_to_shared_area(TLV_MAJOR_CORE,
                                     TLV_MINOR_CORE_IAS_IMPLEMENTATION_ID,
                                     sizeof(implementation_id),
                                     (const uint8_t *)&implementation_id);
  if (res)
  {
    BOOT_LOG_ERR("BL2 IMP_ID set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }

  if (bootutil_priv_key_cnt < 2)
  {
    BOOT_LOG_ERR("No EAT KEY in bootutil_priv_keys!!");
    Error_Handler();
  }
  res = boot_add_data_to_shared_area(TLV_MAJOR_CORE,
                                     TLV_MINOR_CORE_IAS_KEY_TYPE,
                                     *(bootutil_priv_keys[1].len),
                                     bootutil_priv_keys[1].key);
  if (res)
  {
    BOOT_LOG_ERR("BL2 EAT_TYPE set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }
  res = boot_add_data_to_shared_area(TLV_MAJOR_CORE,
                                     TLV_MINOR_CORE_IAS_KEY_PRIV,
                                     *(bootutil_priv_keys[0].len),
                                     bootutil_priv_keys[0].key);
  if (res)
  {
    BOOT_LOG_ERR("BL2 EAT_PRIV set BL2 to SHARED DATA Failed !!");
    Error_Handler();
  }
}
/**
  * @brief  Compute Implementation ID
  * @param  destination buffer
  * @param  len of destination buffer
  * @retval None
  */

static void ComputeImplementationId(uint8_t *hash_result)
{
  bootutil_sha256_context sha256_ctx;
  BOOT_LOG_INF("Code %x %x", (uint32_t)CODE_START
               , (uint32_t)CODE_LIMIT);
  bootutil_sha256_init(&sha256_ctx);
  /* compute write protect area set as excuteable */
  bootutil_sha256_update(&sha256_ctx,
                         CODE_START,
                         (uint32_t)CODE_LIMIT - (uint32_t)CODE_START);

  bootutil_sha256_finish(&sha256_ctx, hash_result);
  BOOT_LOG_INF("hash TFM_SBSFU_Boot  %x%x%x%x .. %x%x%x%x",
               hash_result[0], hash_result[1], hash_result[2], hash_result[3],
               hash_result[28], hash_result[29], hash_result[30], hash_result[31]);

}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
