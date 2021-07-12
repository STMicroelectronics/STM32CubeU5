/**
  ******************************************************************************
  * @file    boot_hal_cfg.h
  * @author  MCD Application Team
  * @brief   File fixing configuration flag specific for STM32U5xx platform
  *
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
#ifndef BOOT_HAL_CFG_H
#define BOOT_HAL_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* RTC clock */
#define  RTC_CLOCK_SOURCE_LSI
#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00F9
#endif
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

/* ICache */
#define TFM_ICACHE_ENABLE /*!< Instruction cache enable */

/* Static protections */
#define TFM_WRP_PROTECT_ENABLE  /*!< Write Protection  */
#define TFM_HDP_PROTECT_ENABLE /*!< HDP protection   */
#define TFM_SECURE_USER_SRAM2_ERASE_AT_RESET /*!< SRAM2 clear at Reset  */

#ifdef TFM_DEV_MODE
#define TFM_OB_RDP_LEVEL_VALUE OB_RDP_LEVEL_1 /*!< RDP level */
#else
#define TFM_OB_RDP_LEVEL_VALUE OB_RDP_LEVEL_2 /*!< RDP level */
#endif /* TFM_DEV_MODE */

#define NO_TAMPER            (0)                /*!< No tamper activated */
#define INTERNAL_TAMPER_ONLY (1)                /*!< Only Internal tamper activated */
#define ALL_TAMPER           (2)                /*!< Internal and External tamper activated */
#define TFM_TAMPER_ENABLE ALL_TAMPER            /*!< TAMPER configuration flag  */

#ifdef TFM_DEV_MODE
#define TFM_OB_BOOT_LOCK 0 /*!< BOOT Lock expected value  */
#define TFM_ENABLE_SET_OB /*!< Option bytes are set by TFM_SBSFU_Boot when not correctly set  */
#define TFM_ERROR_HANDLER_NON_SECURE /*!< Error handler is in Non Secure , this allows regression without jumping   */
#else
#define TFM_WRP_LOCK_ENABLE /*!< Write Protection Lock */
#define TFM_OB_BOOT_LOCK 1 /*!< BOOT Lock expected value  */
#define TFM_NSBOOT_CHECK_ENABLE  /*!<  NSBOOTADD0 and NSBOOTADD1 must be set to TFM_SBSFU_Boot Vector  */
#if  (TFM_OB_RDP_LEVEL_VALUE == OB_RDP_LEVEL_0_5)
#error "RDP 0.5 is not allowed in production mode (TFM_DEV_MODE undefined)"
#endif /* (TFM_OB_RDP_LEVEL_VALUE == OB_RDP_LEVEL_0_5) */
#endif /* TFM_DEV_MODE */

/* Run time protections */
#define TFM_FLASH_PRIVONLY_ENABLE  /*!< Flash Command in Privileged only  */
#define TFM_BOOT_MPU_PROTECTION    /*!< TFM_SBSFU_Boot uses MPU to prevent execution outside of TFM_SBSFU_Boot code  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TFM_SUCCESS = 0U,
  TFM_FAILED
} TFM_ErrorStatus;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
#ifndef TFM_ERROR_HANDLER_NON_SECURE
void Error_Handler_rdp(void);
#else
#define Error_Handler_rdp Error_Handler
#endif
#endif /* GENERATOR_RDP_PASSWORD_AVAILABLE */
