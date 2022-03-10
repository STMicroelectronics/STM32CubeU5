/**
  ******************************************************************************
  * @file    se_psa_id.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @brief   SE_PSA ID include file.
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

#ifndef __SE_PSA_ID_H__
#define __SE_PSA_ID_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION
#include "psa/crypto.h"
#endif

/* PSA LOCATION ID for primary Secure Element */

#define PSA_SE_ST_LOCATION    (1)

/* SE PSA internal ID */
#define SE_ST_PRIV_SLOT_0     0 /* preprovisionned slot in SE non-volatile area */
#define SE_ST_PRIV_SLOT_1     1 /* slot in SE non-volatile area */
#define SE_ST_PRIV_SLOT_FF    2 /* slot in SE volatile area */
#define SE_ST_SERIAL_NUMBER   3 /* SE Unique Serial Number */
#define SE_ST_MAPPING         4 /* SE storage format */
#define SE_ST_KEY_MAPPING     5 /* KEYs format */
#define SE_ST_START_REGION    6 /* SE region number */
#define SE_ST_MEMORY_REGION_ID(__i__) (SE_ST_START_REGION + __i__)

/* SE PSA default lifetime for read only persistent data  */
#define PSA_SE_ST_LIFETIME_READ_ONLY PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(\
    PSA_KEY_PERSISTENCE_READ_ONLY,PSA_SE_ST_LOCATION)

/* SE PSA default lifetime for default persistent data  */
#define PSA_SE_ST_LIFETIME_DEFAULT PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(\
    PSA_KEY_PERSISTENCE_DEFAULT,PSA_SE_ST_LOCATION)

#define PSA_SE_ST_LIFETIME_VOLATILE PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(\
    PSA_KEY_PERSISTENCE_VOLATILE,PSA_SE_ST_LOCATION)

/* convert PSA ID to SE ST ID */
#define PSA_ID_TO_SE_ST_ID(__i__) (__i__ - PSA_KEY_ID_VENDOR_MIN)
/* convert SE ST ID to PSA ID */
#define SE_ST_ID_TO_PSA_ID(__i__) (PSA_KEY_ID_VENDOR_MIN + __i__)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SE_PSA_ID_H__ */

