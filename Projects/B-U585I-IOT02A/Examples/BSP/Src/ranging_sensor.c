/**
  ******************************************************************************
  * @file    ranging_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the ranging sensor feature in the
  *          b_u585i_iot02_ranging_sensors  driver
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
#include "main.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define POLLING_PERIOD (250U) /* milliseconds */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;

/* Private function prototypes -----------------------------------------------*/
static void print_result(RANGING_SENSOR_Result_t *Result);

/**
  * @brief  Ranging sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Rs_demo(void)
{
  int32_t                   result  = 0;
  uint32_t                  chipId;
  RANGING_SENSOR_Result_t   distance;

  printf("\n******RANGING SENSOR******\n");
  printf("Press user button for next Example\n");
  /* Initialize user button */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE) result--;
  printf("Waiting for Ranging Sensor Initialization ... \n");
  if (BSP_RANGING_SENSOR_Init(VL53L5A1_DEV_CENTER) != BSP_ERROR_NONE) result--;
  if (BSP_RANGING_SENSOR_ReadID(VL53L5A1_DEV_CENTER, &chipId) != BSP_ERROR_NONE) result--;
  if (BSP_RANGING_SENSOR_GetCapabilities(VL53L5A1_DEV_CENTER, &Cap) != BSP_ERROR_NONE) result--;

  Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
  Profile.TimingBudget = 30; /* 5 ms < TimingBudget < 100 ms */
  Profile.Frequency = 5; /* Hz */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  /* set the profile if different from default one */
  if (BSP_RANGING_SENSOR_ConfigProfile(VL53L5A1_DEV_CENTER, &Profile) != BSP_ERROR_NONE) result--;
  if (BSP_RANGING_SENSOR_Start(VL53L5A1_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS) != BSP_ERROR_NONE)  result--;

  /* GetDistance function */
  printf("Distance values:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_RANGING_SENSOR_GetDistance(VL53L5A1_DEV_CENTER, &distance) != BSP_ERROR_NONE) result--;
    print_result(&distance);
    HAL_Delay(POLLING_PERIOD);
  }
  UserButtonPressed = RESET;

  return result;
}


static void print_result(RANGING_SENSOR_Result_t *Result)
{
  int8_t i, j, k, l;
  uint8_t zones_per_line;

  zones_per_line = ((Profile.RangingProfile == RS_PROFILE_8x8_AUTONOMOUS) ||
         (Profile.RangingProfile == RS_PROFILE_8x8_CONTINUOUS)) ? 8 : 4;

  printf("53L5A1 Simple Ranging demo application\n");
  printf("--------------------------------------\n\n");

  printf("Cell Format :\n\n");
  for (l = 0; l < RANGING_SENSOR_NB_TARGET_PER_ZONE; l++)
  {
    printf(" [38;5;10m%20s[0m : %20s\n", "Distance [mm]", "Status");
    if ((Profile.EnableAmbient != 0) || (Profile.EnableSignal != 0))
    {
      printf(" %20s : %20s\n", "Signal [kcps/spad]", "Ambient [kcps/spad]");
    }
  }

  printf("\n\n");

  for (j = 0; j < Result->NumberOfZones; j += zones_per_line)
  {
    for (i = 0; i < zones_per_line; i++) /* number of zones per line */
      printf(" -----------------------------");
    printf("\n");

    for (i = 0; i < zones_per_line; i++)
      printf("|                             ");
    printf("|\n");

    for (l = 0; l < RANGING_SENSOR_NB_TARGET_PER_ZONE; l++)
    {
      /* Print distance and status */
      for (k = (zones_per_line - 1); k >= 0; k--)
      {
        if (Result->ZoneResult[j+k].NumberOfTargets > 0)
          printf("| [38;5;10m%5ld[0m  :  %5ld ",
              (long)Result->ZoneResult[j+k].Distance[l],
              (long)Result->ZoneResult[j+k].Status[l]);
        else
          printf("| %5s  :  %5s ", "X", "X");
      }
      printf("|\n");

      if ((Profile.EnableAmbient != 0) || (Profile.EnableSignal != 0))
      {
        /* Print Signal and Ambient */
        for (k = (zones_per_line - 1); k >= 0; k--)
        {
          if (Result->ZoneResult[j+k].NumberOfTargets > 0)
          {
            if (Profile.EnableSignal != 0)
              printf("| %5ld  :  ", (long)Result->ZoneResult[j+k].Signal[l]);
            else
              printf("| %5s  :  ", "X");

            if (Profile.EnableAmbient != 0)
              printf("%5ld ", (long)Result->ZoneResult[j+k].Ambient[l]);
            else
              printf("%5s ", "X");
          }
          else
            printf("| %5s  :  %5s ", "X", "X");
        }
        printf("|\n");
      }
    }
  }

  for (i = 0; i < zones_per_line; i++)
    printf(" -----------------------------");
  printf("\n");
}



/**
  * @}
  */

/**
  * @}
  */

