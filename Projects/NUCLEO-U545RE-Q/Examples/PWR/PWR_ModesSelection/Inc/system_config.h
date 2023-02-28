/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/system_config.h
  * @author  MCD Application Team
  * @brief   Header for system_config.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  System Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_OK    = 0x00, /*!< System status OK             */
  SYSTEM_ERROR = 0x01  /*!< System status ERROR          */

} System_StatusTypeDef;

/** @defgroup  System Low Power Mode Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_LOWPOWER_RUN      = 0x00, /*!< System low power run mode.       */
  SYSTEM_LOWPOWER_STOP1    = 0x01, /*!< System low power stop1 mode.     */
  SYSTEM_LOWPOWER_STOP2    = 0x02, /*!< System low power stop2 mode.     */
  SYSTEM_LOWPOWER_STOP3    = 0x03, /*!< System low power stop3 mode.     */
  SYSTEM_LOWPOWER_STANDBY  = 0x04, /*!< System low power standby mode.   */
  SYSTEM_LOWPOWER_SHUTDOWN = 0x05, /*!< System low power shutdown mode.  */

} System_LowPowerModeTypeDef;

/** @defgroup  System Regulator Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_REGULATOR_LDO     = 0x00, /*!< System regulator LDO.      */
  SYSTEM_REGULATOR_SMPS    = 0x01, /*!< System regulator SMPS.     */
  SYSTEM_REGULATOR_DEFAULT = 0x02  /*!< System regulator DEFAULT.  */

} System_RegulatorTypeDef;

/** @defgroup  RTC State Enumeration Definition.
  * @{
  */
typedef enum
{
  RTC_STATE_DISABLE = 0x00, /*!< RTC state disable.     */
  RTC_STATE_ENABLE  = 0x01, /*!< RTC state enable.      */
  RTC_STATE_DEFAULT = 0x02  /*!< RTC state default.     */

} RTC_StateTypeDef;

/** @defgroup  System Clock Frequency Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_CLOCK_160MH   = 0x00, /*!< System clock frequency 160MH.     */
  SYSTEM_CLOCK_24MH    = 0x01, /*!< System clock frequency 160MH.     */
  SYSTEM_CLOCK_DEFAULT = 0x02  /*!< System clock frequency Default.   */

} System_ClockFrequencyTypeDef;

/** @defgroup  System Run Ram Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_RUN_SRAM_FULL       = 0x00, /*!< System run full SRAM retention.         */
  SYSTEM_RET_RUN_ONLY_SRAM1_FULL = 0x01, /*!< System run only SRAM1 full retention.   */

} System_Run_RamRetentionTypeDef;

/** @defgroup  System Stop1 Ram  Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_STOP1_SRAM_FULL                  = 0x00, /*!< System stop1 full SRAM retention.                 */
  SYSTEM_RET_STOP1_ONLY_ICACHE_FULL_SRAM2_8KB = 0x01, /*!< System stop1 full ICACHE 8KB SRAM2 retention.     */

} System_Stop1_RamRetentionTypeDef;

/** @defgroup  System Stop2 Ram  Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_STOP2_SRAM_FULL                   = 0x00, /*!< System stop2 full SRAM retention.                 */
  SYSTEM_RET_STOP2_ONLY_ICACHE_FULL_SRAM2_8KB  = 0x01, /*!< System stop2 full ICACHE 8KB SRAM2 retention.     */

} System_Stop2_RamRetentionTypeDef;

/** @defgroup  System Stop3 Ram  Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_STOP3_SRAM_FULL                   = 0x00, /*!< System stop3 full SRAM retention.                 */
  SYSTEM_RET_STOP3_ONLY_ICACHE_FULL_SRAM2_8KB  = 0x01, /*!< System stop3 full ICACHE 8KB SRAM2 retention.     */

} System_Stop3_RamRetentionTypeDef;

/** @defgroup  System Standby Ram  Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_STANDBY_ONLY_SRAM2_FULL_BKPRAM  = 0x00, /*!< System standby only SRAM2 full BKPRAM retention.    */
  SYSTEM_RET_STANDBY_ONLY_SRAM2_8KB_BKPRAM   = 0x01, /*!< System standby only 8KB SRAM2 BKPRAM retention.     */
  SYSTEM_RET_STANDBY_SRAM2_NO                = 0x02, /*!< System standby no SRAM2 retention.                  */

} System_Standby_RamRetentionTypeDef;

/** @defgroup System Shutdown Ram  Retention Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_RET_SHUTDOWN_DEFAULT = 0x00, /*!< System shutdown default retention.      */

} System_Shutdown_RamRetentionTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
System_StatusTypeDef system_config(RTC_StateTypeDef state, System_LowPowerModeTypeDef lowpower);
System_StatusTypeDef system_lowpower_config(System_LowPowerModeTypeDef lowpower_mode);
System_StatusTypeDef system_regulator_config(System_RegulatorTypeDef regulator);
System_StatusTypeDef system_rtc_config(RTC_StateTypeDef state);
System_StatusTypeDef system_clock_config(System_ClockFrequencyTypeDef clock_frequency);
System_StatusTypeDef system_ramretention_config(System_LowPowerModeTypeDef lowpower_mode, uint32_t config);

#endif /* SYSTEM_CONFIG_H */
