/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/lowpower_scenarios.h
  * @author  MCD Application Team
  * @brief   Header for lowpower_scenarios.c module
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
#ifndef LOWPOWER_SCENARIOS_H
#define LOWPOWER_SCENARIOS_H

/* Includes ------------------------------------------------------------------*/
#include "system_config.h"
#include "console.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  Scenario Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SCENARIO_OK    = 0x00, /*!< Scenario state OK            */
  SCENARIO_ERROR = 0x01  /*!< Scenario state ERROR         */

} Scenario_StatusTypeDef;

/**
  * @brief  System Configuration Structure Definition.
  */
typedef struct
{
  System_LowPowerModeTypeDef   lowpower_mode;     /*!< Specifies system low power mode configuration.  */
  char* lowpower_mode_str;
  RTC_StateTypeDef             rtc_state;         /*!< Specifies rtc state configuration.              */
  char* rtc_state_str;
  System_RegulatorTypeDef      regulator;         /*!< Specifies system regulator configuration.       */
  char* regulator_str;
  System_ClockFrequencyTypeDef clock;             /*!< Specifies system clock frequency configuration. */
  char* clock_str;
  uint32_t                     ram_retention;     /*!< Specifies ram retention configuration.          */
  char* ram_retention_str;

} LowPower_ScenarioTypeDef;

/* Exported constants --------------------------------------------------------*/
#define SCENARIO_CMDBUF_SIZE (0x01U)
#define DUMMY_PATTERN        (0xFFU)

/* Header buffer */
static const char header_limiter_msg[] = " ******************************************************************************************************************** \r\n";
static const char header_start_msg[]   = " *                                 Low power mode measure current consumption example                               * \r\n";

/* Configuration selection buffer */
static const char lowpower_mode_select_msg[] = " Select system low power mode index:                \r\n";
static const char regulator_select_msg[]     = " Select system low power mode regulator index:      \r\n";
static const char rtc_state_select_msg[]     = " Select rtc state during low power mode index:      \r\n";
static const char clock_select_msg[]         = " Select clock frquency during low power mode index: \r\n";
static const char ram_retention_select_msg[] = " Select ram retention during low power mode index:  \r\n";

/* Low power buffer */
static const char *const lowpower_mode_msg[] =
{
  " Low power mode : Run      \r\n",
  " Low power mode : Stop1    \r\n",
  " Low power mode : Stop2    \r\n",
  " Low power mode : Stop3    \r\n",
  " Low power mode : Standby  \r\n",
  " Low power mode : Shutdown \r\n"
};

/* RTC buffer */
static const char *const rtc_state_msg[] =
{
  " RTC : Disabled \r\n",
  " RTC : Enabled  \r\n"
};

/* Regulator buffer */
static const char *const regulator_msg[] =
{
  " Regulator : LDO  \r\n",
  " Regulator : SMPS \r\n"
};

/* Clock frequency buffer */
static const char *const clock_frequency_msg[] =
{
  " Clock frequency : 160Mhz (Voltage scaling range 1) \r\n",
  " Clock frequency : 24Mhz  (Voltage scaling range 4) \r\n"
};

/* Run ram retention buffer */
static const char *const run_ram_retention_msg[] =
{
  " SRAM Retention : Full SRAM retention             \r\n",
  " SRAM Retention : Only SRAM2 retention (192KB)   \r\n"
};

/* Stop1 ram retention buffer */
static const char *const stop1_ram_retention_msg[] =
{
  " SRAM Retention : Full SRAM retention                                \r\n",
  " SRAM Retention : Full ICACHE retention + 8KB SRAM2 retention (16KB) \r\n"
};

/* Stop2 ram retention buffer */
static const char *const stop2_ram_retention_msg[] =
{
  " SRAM Retention : Full SRAM retention                                \r\n",
  " SRAM Retention : Full ICACHE retention + 8KB SRAM2 retention (16KB) \r\n"
};

/* Stop3 ram retention buffer */
static const char *const stop3_ram_retention_msg[] =
{
  " SRAM Retention : Full SRAM retention                                \r\n",
  " SRAM Retention : Full ICACHE retention + 8KB SRAM2 retention (16KB) \r\n"
};

/* Standby ram retention buffer */
static const char *const standby_ram_retention_msg[] =
{
  " SRAM Retention : Full SRAM2 retention + BKPRAM retention  (66KB)     \r\n",
  " SRAM Retention : Only 8KB SRAM2 retention + BKPRAM retention (10KB) \r\n",
  " SRAM Retention : No SRAM2 retention                                 \r\n"
};


/* Footer buffer */
static const char system_lowpower_config_msg[] = " Your system low power configuration is as follow : \r\n";
static const char start_measure_msg[]          = " ***************************           You can start measuring power consumption           ************************** \r\n";

/* Index  buffer */
static const char index0_msg[] = "   0   -->";
static const char index1_msg[] = "   1   -->";
static const char index2_msg[] = "   2   -->";
static const char index3_msg[] = "   3   -->";
static const char index4_msg[] = "   4   -->";
static const char index5_msg[] = "   5   -->";

/* Empty line_space buffer */
static const char empty_line_msg[]  = "\r\n";
static const char empty_space_msg[] = "  ";

/* Cursor buffer */
static const char user_cursor_msg[] = "=> ";

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
Scenario_StatusTypeDef lowpower_header_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_footer_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_get_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_config_scenario(LowPower_ScenarioTypeDef *scenario);

#endif /* LOWPOWER_SCENARIOS_H */
