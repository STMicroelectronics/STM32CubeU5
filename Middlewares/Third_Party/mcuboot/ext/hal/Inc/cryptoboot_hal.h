/**
  ******************************************************************************
  * @file    crypto_boot.h
  * @author  MCD Application Team
  * @brief   This file contains crypto declarations in mcuboot to map onto HAL
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRYPTOBOOT_HAL_H
#define __CRYPTOBOOT_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcuboot_config/mcuboot_config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ERR_AES_INVALID_KEY_LENGTH    -1
#define ERR_PLATFORM_HW_ACCEL_FAILED  -2
#define ERR_BAD_INPUT_DATA            -3
#define ERR_ECP_VERIFY_FAILED         -4
/* Exported macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
#define INPUT_VALIDATE_RET( cond )  \
    do {                                            \
        if( !(cond) )                               \
        {                                           \
            return( ERR_BAD_INPUT_DATA );           \
        }                                           \
    } while( 0 )

#define INPUT_VALIDATE( cond )  \
    do {                                            \
        if( !(cond) )                               \
        {                                           \
            return;           \
        }                                           \
    } while( 0 )

#define CHK(f)       \
    do                           \
    {                            \
        if( ( ret = (f) ) != 0 ) \
            goto cleanup;        \
    } while( 0 )
/* Private constants ---------------------------------------------------------*/
static const uint8_t P_256_absA[] __attribute__((aligned(4))) =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03
};

static const uint8_t P_256_b[] __attribute__((aligned(4))) =
{
  0x5A, 0xC6, 0x35, 0xD8, 0xAA, 0x3A, 0x93, 0xE7, 0xB3, 0xEB, 0xBD, 0x55, 0x76,
  0x98, 0x86, 0xBC, 0x65, 0x1D, 0x06, 0xB0, 0xCC, 0x53, 0xB0, 0xF6, 0x3B, 0xCE,
  0x3C, 0x3E, 0x27, 0xD2, 0x60, 0x4B
};
static const uint32_t P_256_a_sign = 1;
static const uint8_t P_256_p[] __attribute__((aligned(4))) =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
static const uint8_t P_256_n[] __attribute__((aligned(4))) =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84, 0xF3, 0xB9,
  0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51
};
static const uint8_t P_256_Gx[] __attribute__((aligned(4))) =
{
  0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5, 0x63,
  0xA4, 0x40, 0xF2, 0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0, 0xF4, 0xA1,
  0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96
};
static const uint8_t P_256_Gy[] __attribute__((aligned(4))) =
{
  0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A, 0x7C,
  0x0F, 0x9E, 0x16, 0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE, 0xCB, 0xB6,
  0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5
};
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


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

#endif /* __CRYPTOBOOT_HAL_H */
