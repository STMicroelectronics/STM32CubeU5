/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/lowpower_scenarios.c
  * @author  MCD Application Team
  * @brief   This file provides low power scenarios
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

/* Includes ------------------------------------------------------------------*/
#include "lowpower_scenarios.h"
#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Scenrio_CmdBuff[SCENARIO_CMDBUF_SIZE + 1U] = {DUMMY_PATTERN, 0x00};

/* Private function prototypes -----------------------------------------------*/
static Scenario_StatusTypeDef get_lowpower_cmd(LowPower_ScenarioTypeDef *scenario);
static Scenario_StatusTypeDef get_rtc_state_cmd(LowPower_ScenarioTypeDef *scenario);
static Scenario_StatusTypeDef get_system_frequency_cmd(LowPower_ScenarioTypeDef *scenario);
static Scenario_StatusTypeDef get_regulator_cmd(LowPower_ScenarioTypeDef *scenario);
static Scenario_StatusTypeDef get_ram_retention_cmd(LowPower_ScenarioTypeDef *scenario);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Print the header msg on Hyperterminal.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_header_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Clear scenario variable */
  memset((void *)scenario, 0U, sizeof(LowPower_ScenarioTypeDef));

  /* Print header application */
  Console_Log(empty_line_msg);
  Console_Log(header_limiter_msg);
  Console_Log(header_start_msg);
  Console_Log(header_limiter_msg);
  Console_Log(empty_line_msg);

  return SCENARIO_OK;
}

/**
  * @brief  Print the footer msg on Hyperterminal.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_footer_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Print the configuration msg */
  Console_Log(system_lowpower_config_msg);

  /* Print the low power mode selected */
  if (scenario->lowpower_mode_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->lowpower_mode_str);
  }

  /* Print the rtc state selected */
  if (scenario->rtc_state_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->rtc_state_str);
  }

    /* Print the ram retention selected */
  if (scenario->ram_retention_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->ram_retention_str);
  }

  /* Print the regulator selected */
  if (scenario->regulator_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->regulator_str);
  }

  /* Print the clock frequency selected */
  if (scenario->clock_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->clock_str);
  }

  return SCENARIO_OK;
}

/**
  * @brief  Check the configuration selected.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_get_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Get system low power mode command */
  if (get_lowpower_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Get rtc state command */
  if (get_rtc_state_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Get ram retention command */
  if (get_ram_retention_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Get system regulator command */
  if (get_regulator_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Get system frequency command (VOS is deduced from system frequency value) */
  if (get_system_frequency_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  return SCENARIO_OK;
}

/**
* @brief  Configure low power mode.
* @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
* @retval Scenario Status.
*/
Scenario_StatusTypeDef lowpower_config_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Configure ram retention */
  if (system_ramretention_config(scenario->lowpower_mode, (uint32_t)scenario->ram_retention) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure system frequency and voltage scaling */
  if (system_clock_config(scenario->clock) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure system regulator */
  if (system_regulator_config(scenario->regulator) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure rtc state */
  if (system_rtc_config(scenario->rtc_state) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Print measuring msg */
  Console_Log(empty_line_msg);
  Console_Log(start_measure_msg);

  /* Configure system */
  if (system_config(scenario->rtc_state, scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure enter low power mode */
  if (system_lowpower_config(scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  return SCENARIO_OK;
}

/**
  * @brief  Get system low power mode command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_lowpower_cmd(LowPower_ScenarioTypeDef *scenario)
{
  uint32_t cmd;

  /* Print low power mode selection msg */
  Console_Log(lowpower_mode_select_msg);
  /* Print index msg */
  Console_Log(index0_msg);
  /* Print system low power mode */
  Console_Log(lowpower_mode_msg[0U]);
  Console_Log(index1_msg);
  Console_Log(lowpower_mode_msg[1U]);
  Console_Log(index2_msg);
  Console_Log(lowpower_mode_msg[2U]);
  Console_Log(index3_msg);
  Console_Log(lowpower_mode_msg[3U]);
  Console_Log(index4_msg);
  Console_Log(lowpower_mode_msg[4U]);
  Console_Log(index5_msg);
  Console_Log(lowpower_mode_msg[5U]);
  /* Print cursor */
  Console_Log(user_cursor_msg);

  /* Repeat until getting valid command */
  do
  {
    /* Receive system low power mode command */
    if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
    {
      return SCENARIO_ERROR;
    }
  }
  while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_5));

  /* Print index selected */
  Console_Log((char *)Scenrio_CmdBuff);
  Console_Log(empty_line_msg);
  Console_Log(empty_line_msg);

  /* Convert char to an integer */
  cmd = atoi((char *)Scenrio_CmdBuff);
  /* Get system low power mode */
  scenario->lowpower_mode     = (System_LowPowerModeTypeDef)cmd;
  scenario->lowpower_mode_str = (char *)lowpower_mode_msg[cmd];

  /* Initialize command buffer */
  Scenrio_CmdBuff[0U] = DUMMY_PATTERN;

  return SCENARIO_OK;
}

/**
  * @brief  Get rtc state command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_rtc_state_cmd(LowPower_ScenarioTypeDef *scenario)
{
  uint32_t cmd;

  /* RTC state configuration does not concern run mode */
  if (scenario->lowpower_mode != SYSTEM_LOWPOWER_RUN)
  {
    /* Print rtc state selection msg */
    Console_Log(rtc_state_select_msg);
    /* Print index msg */
    Console_Log(index0_msg);
    /* Print rtc state */
    Console_Log(rtc_state_msg[0U]);
    Console_Log(index1_msg);
    Console_Log(rtc_state_msg[1U]);
    /* Print cursor */
    Console_Log(user_cursor_msg);

    /* Repeat until getting valid command */
    do
    {
      /* Receive rtc state command */
      if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
      {
        return SCENARIO_ERROR;
      }
    }
    while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

    /* Print index selected */
    Console_Log((char *)Scenrio_CmdBuff);
    Console_Log(empty_line_msg);
    Console_Log(empty_line_msg);

    /* Convert char to an integer */
    cmd = atoi((char *)Scenrio_CmdBuff);
    /* Get rtc state */
    scenario->rtc_state     = (RTC_StateTypeDef)cmd;
    scenario->rtc_state_str = (char *)rtc_state_msg[cmd];
  }
  else
  {
    /* Get rtc state default */
    scenario->rtc_state = RTC_STATE_DEFAULT;
  }

  /* Initialize command buffer */
  Scenrio_CmdBuff[0U] = DUMMY_PATTERN;

  return SCENARIO_OK;
}

/**
  * @brief  Get system regulator command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_regulator_cmd(LowPower_ScenarioTypeDef *scenario)
{
  uint32_t cmd;

  /* Regulator configuration does not concern shutdown and standby with no SRAM2 retention */
  if ((scenario->lowpower_mode != SYSTEM_LOWPOWER_SHUTDOWN) &&
      (scenario->ram_retention != SYSTEM_RET_STANDBY_SRAM2_NO ))

  {
    /* Print regulator selection msg */
    Console_Log(regulator_select_msg);
    /* Print index msg */
    Console_Log(index0_msg);
    /* Print system regulator msg */
    Console_Log(regulator_msg[0U]);
    Console_Log(index1_msg);
    Console_Log(regulator_msg[1U]);
    /* Print cursor */
    Console_Log(user_cursor_msg);

    /* Repeat until getting valid command */
    do
    {
      /* Receive system regulator command */
      if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
      {
        return SCENARIO_ERROR;
      }
    }
    while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

    /* Print index selected */
    Console_Log((char *)Scenrio_CmdBuff);
    Console_Log(empty_line_msg);
    Console_Log(empty_line_msg);

    /* Convert char to an integer */
    cmd = atoi((char *)Scenrio_CmdBuff);
    /* Get system regulator */
    scenario->regulator     = (System_RegulatorTypeDef)cmd;
    scenario->regulator_str = (char *)regulator_msg[cmd];
  }
  else
  {
    /* Get system regulator default */
    scenario->regulator = SYSTEM_REGULATOR_DEFAULT;
  }

  /* Initialize command buffer */
  Scenrio_CmdBuff[0U] = DUMMY_PATTERN;

  return SCENARIO_OK;
}

/**
  * @brief  Get system clock frequency command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_system_frequency_cmd(LowPower_ScenarioTypeDef *scenario)
{
  uint32_t cmd;

  /* System clock frequency concern only run mode */
  if (scenario->lowpower_mode == SYSTEM_LOWPOWER_RUN)
  {
    /* Print system clock frequency selection msg */
    Console_Log(clock_select_msg);
    /* Print index msg */
    Console_Log(index0_msg);
    /* Print system clock frequency msg */
    Console_Log(clock_frequency_msg[0U]);
    Console_Log(index1_msg);
    Console_Log(clock_frequency_msg[1U]);
    /* Print cursor */
    Console_Log(user_cursor_msg);

    /* Repeat until getting valid command */
    do
    {
      /* Receive system clock frequency command */
      if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
      {
        return SCENARIO_ERROR;
      }
    }
    while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

    /* Print index selected */
    Console_Log((char *)Scenrio_CmdBuff);
    Console_Log(empty_line_msg);
    Console_Log(empty_line_msg);

    /* Convert char to an integer */
    cmd = atoi((char *)Scenrio_CmdBuff);
    /* Get system clock frequency */
    scenario->clock     = (System_ClockFrequencyTypeDef)cmd;
    scenario->clock_str = (char *)clock_frequency_msg[cmd];
  }
  else
  {
    /* Get system clock frequency default */
    scenario->clock = SYSTEM_CLOCK_DEFAULT;
  }

  /* Initialize command buffer */
  Scenrio_CmdBuff[0U] = DUMMY_PATTERN;

  return SCENARIO_OK;
}

/**
  * @brief  Get ram retention command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_ram_retention_cmd(LowPower_ScenarioTypeDef *scenario)
{
  Scenario_StatusTypeDef status = SCENARIO_OK;
  uint32_t cmd;

  /* Check system low power mode selected */
  switch (scenario->lowpower_mode)
  {
  case SYSTEM_LOWPOWER_RUN:
    {
      /* Print ram retention selection msg */
      Console_Log(ram_retention_select_msg);
      /* Print index msg */
      Console_Log(index0_msg);
      /* Print run ram retention msg */
      Console_Log(run_ram_retention_msg[0U]);
      Console_Log(index1_msg);
      Console_Log(run_ram_retention_msg[1U]);
      /* Print cursor */
      Console_Log(user_cursor_msg);

      /* Repeat until getting valid command */
      do
      {
        /* Receive run ram retention command */
        if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
        {
          return SCENARIO_ERROR;
        }
      }
      while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

      /* Print index selected */
      Console_Log((char *)Scenrio_CmdBuff);
      Console_Log(empty_line_msg);
      Console_Log(empty_line_msg);

      /* Convert char to an integer */
      cmd = atoi((char *)Scenrio_CmdBuff);
      /* Get run ram retention */
      scenario->ram_retention     = (System_Run_RamRetentionTypeDef)cmd;
      scenario->ram_retention_str = (char *)run_ram_retention_msg[cmd];

      break;
    }

  case SYSTEM_LOWPOWER_STOP1:
    {
      /* Print ram retention selection msg */
      Console_Log(ram_retention_select_msg);
      /* Print index msg */
      Console_Log(index0_msg);
      /* Print stop1 ram retention msg */
      Console_Log(stop1_ram_retention_msg[0U]);
      Console_Log(index1_msg);
      Console_Log(stop1_ram_retention_msg[1U]);
      /* Print cursor */
      Console_Log(user_cursor_msg);

      /* Repeat until getting valid command */
      do
      {
        /* Receive stop1 ram retention command */
        if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
        {
          return SCENARIO_ERROR;
        }
      }
      while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

      /* Print index selected */
      Console_Log((char *)Scenrio_CmdBuff);
      Console_Log(empty_line_msg);
      Console_Log(empty_line_msg);

      /* Convert char to an integer */
      cmd = atoi((char *)Scenrio_CmdBuff);
      /* Get stop1 ram retention */
      scenario->ram_retention     = (System_Stop1_RamRetentionTypeDef)cmd;
      scenario->ram_retention_str = (char *)stop1_ram_retention_msg[cmd];

      break;
    }

  case SYSTEM_LOWPOWER_STOP2:
    {
      /* Print ram retention selection msg */
      Console_Log(ram_retention_select_msg);
      /* Print index msg */
      Console_Log(index0_msg);
      /* Print stop2 ram retention msg */
      Console_Log(stop2_ram_retention_msg[0U]);
      Console_Log(index1_msg);
      Console_Log(stop2_ram_retention_msg[1U]);
      /* Print cursor */
      Console_Log(user_cursor_msg);

      /* Repeat until getting valid command */
      do
      {
        /* Receive stop2 ram retention command */
        if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
        {
          return SCENARIO_ERROR;
        }
      }
      while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

      /* Print index selected */
      Console_Log((char *)Scenrio_CmdBuff);
      Console_Log(empty_line_msg);
      Console_Log(empty_line_msg);

      /* Convert char to an integer */
      cmd = atoi((char *)Scenrio_CmdBuff);
      /* Get stop2 ram retention */
      scenario->ram_retention     = (System_Stop2_RamRetentionTypeDef)cmd;
      scenario->ram_retention_str = (char *)stop2_ram_retention_msg[cmd];

      break;
    }

  case SYSTEM_LOWPOWER_STOP3:
    {
      /* Print ram retention selection msg */
      Console_Log(ram_retention_select_msg);
      /* Print index msg */
      Console_Log(index0_msg);
      /* Print stop3 ram retention msg */
      Console_Log(stop3_ram_retention_msg[0U]);
      Console_Log(index1_msg);
      Console_Log(stop3_ram_retention_msg[1U]);
      /* Print cursor */
      Console_Log(user_cursor_msg);

      /* Repeat until getting valid command */
      do
      {
        /* Receive stop3 ram retention command */
        if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
        {
          return SCENARIO_ERROR;
        }
      }
      while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_1));

      /* Print index selected */
      Console_Log((char *)Scenrio_CmdBuff);
      Console_Log(empty_line_msg);
      Console_Log(empty_line_msg);

      /* Convert char to an integer */
      cmd = atoi((char *)Scenrio_CmdBuff);
      /* Get stop3 ram retention */
      scenario->ram_retention     = (System_Stop3_RamRetentionTypeDef)cmd;
      scenario->ram_retention_str = (char *)stop3_ram_retention_msg[cmd];

      break;
    }

  case SYSTEM_LOWPOWER_STANDBY:
    {
      /* Print ram retention selection msg */
      Console_Log(ram_retention_select_msg);
      /* Print index msg */
      Console_Log(index0_msg);
      /* Print standby ram retention msg */
      Console_Log(standby_ram_retention_msg[0U]);
      Console_Log(index1_msg);
      Console_Log(standby_ram_retention_msg[1U]);
      Console_Log(index2_msg);
      Console_Log(standby_ram_retention_msg[2U]);
      /* Print cursor */
      Console_Log(user_cursor_msg);

      /* Repeat until getting valid command */
      do
      {
        /* Receive standby ram retention command */
        if (Console_GetCommand(Scenrio_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
        {
          return SCENARIO_ERROR;
        }
      }
      while ((Scenrio_CmdBuff[0U] < CODE_ASCII_0) || (Scenrio_CmdBuff[0U] > CODE_ASCII_2));

      /* Print index selected */
      Console_Log((char *)Scenrio_CmdBuff);
      Console_Log(empty_line_msg);
      Console_Log(empty_line_msg);

      /* Convert char to an integer */
      cmd = atoi((char *)Scenrio_CmdBuff);
      /* Get standby ram retention */
      scenario->ram_retention     = (System_Standby_RamRetentionTypeDef)cmd;
      scenario->ram_retention_str = (char *)standby_ram_retention_msg[cmd];

      break;
    }

  case SYSTEM_LOWPOWER_SHUTDOWN:
    {
      /* Get shutdown ram retention default */
      scenario->ram_retention = SYSTEM_RET_SHUTDOWN_DEFAULT;

      break;
    }

    /* Return error if low power command is not ok */
  default:
    {
      status = SCENARIO_ERROR;

      break;
    }
  }

  /* Initialize command buffer */
  Scenrio_CmdBuff[0U] = DUMMY_PATTERN;

  return status;
}
