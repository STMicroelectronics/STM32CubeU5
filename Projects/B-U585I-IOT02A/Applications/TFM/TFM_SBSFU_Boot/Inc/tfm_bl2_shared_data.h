/**
  ******************************************************************************
  * @file    tfm_bl2_shared_data.h
  * @author  MCD Application Team
  * @brief   Header for tfm_bl2_shared_data.c module
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
#ifndef TFM_BL2_SHARED_DATA_H
#define TFM_BL2_SHARED_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/** @addtogroup TFM_BL2_SHARED_DATA
 * * @{
  */
/** @defgroup TFM_BL2_SHARED_DATA define
  * @{
  */
/* size of element stored in RTC for exchange with TFM */
#define BL2_RTC_SHARED_DATA_SIZE 		32
#define BL2_RTC_CLEANUP_VALUE			0xDADABEEF
/**
  * @}
  */
/** @defgroupTFM_BL2_SHARED_DATA  Interface
  * @{
  */
/** @defgroup TFM_SECURITY_Configuration Security Configuration
  * @{
  */


/**
  * @}
  */

/** @defgroup TFM_BL2_SHARED_DATA__Exported_Constants Exported Constants
  * @{
  */

/** @defgroup TFM_BL2_SHARED_DATA__Constants_BOOL SFU Bool definition
  * @{
  */

/**
  * @}
  */

/** @defgroup TFM_BL2_SHARED_DATA_Constants_State TFM Functional State definition
  * @{
  */

/**
  * @}
  */

/** @defgroup TFM_BL2_SHARED_DATA_Exported_Functions Exported Functions
  * @{
  */
void TFM_BL2_CopySharedData(void);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* TFM_BL2_SHARED_DATA_H */
