/**
  ******************************************************************************
  * @file    platform_ioctl.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Test Protections functionalities.
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
#ifndef PLATFORM_IOCTL_H
#define PLATFORM_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

/* #define TFM_TEST_PRIV_PROTECTION */

/** @defgroup  PLATFORM_IOCTL_Exported_Constant Exported Constant
  * @{
  */

typedef enum
{
    PLATFORM_IOTCL_FWSEC_CONFIRM = 0,
    PLATFORM_IOTCL_DATASEC_CONFIRM,
#ifdef TFM_TEST_PRIV_PROTECTION
    PLATFORM_IOCTL_TEST_READ,
    PLATFORM_IOCTL_TEST_WRITE,
    PLATFORM_IOCTL_TEST_WRITE_EXEC,
#endif /* TFM_TEST_PRIV_PROTECTION */
} plaform_ioctl_val;

/** @defgroup  PLATFORM_IOCTL_Exported_Function Exported Function
  * @{
  */
#ifdef TFM_TEST_PRIV_PROTECTION
extern __IO uint32_t priv_test_table;
extern void priv_test_func(void);
#endif
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_IOCTL_H */

