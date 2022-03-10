/**
  ******************************************************************************
  * @file    boot_hal_hash_ref.h
  * @author  MCD Application Team
  * @brief   Header for hash ref services in boot_hal.c module
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
#ifndef BOOT_HAL_HASH_REF_H
#define BOOT_HAL_HASH_REF_H

#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_LEN   (0x20U)

extern uint8_t ImageValidEnable;
extern uint8_t ImageValidHashUpdate;
extern uint8_t ImageValidHashRef[];

int boot_hash_ref_store(void);
int boot_hash_ref_load(void);
int boot_hash_ref_set(uint8_t *hash_ref, uint8_t size, uint8_t image_index);
int boot_hash_ref_get(uint8_t *hash_ref, uint8_t size, uint8_t image_index);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_HASH_REF_H */
