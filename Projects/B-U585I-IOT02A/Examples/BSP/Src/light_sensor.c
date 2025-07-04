/**
  ******************************************************************************
  * @file    light_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the light sensor feature in the
  *          b_u585i_iot02_light_sensors  driver
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

/** @addtogroup STM32U5xx_BSP_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined (USE_B_U585I_IOT02A_U585AI_REVD)
static float IntegrationTime_800[4]     = {.0036, .0072, .0288, .0576};
static float IntegrationTime_400[4]     = {.0072, .0144, .0576, .1152};
static float IntegrationTime_200[4]     = {.0144, .0288, .1152, .2304};
static float IntegrationTime_100[4]     = {.0288, .0576, .2304, .4608};
static float IntegrationTime_50[4]      = {.0576, .1152, .4608, .9216};
static float IntegrationTime_25[4]      = {.1152, .2304, .9216, 1.8432};
#else
static float IntegrationTime_800[3]     = {0.00426, 0.00852, 0.01704};
static float IntegrationTime_400[3]     = {0.00852, 0.01704, 0.03408};
static float IntegrationTime_200[3]     = {0.01704, 0.03408, 0.06816};
static float IntegrationTime_100[3]     = {0.03408, 0.06816, 0.13632};
static float IntegrationTime_50[3]      = {0.06816, 0.13632, 0.27264};
#endif /* USE_B_U585I_IOT02A_U585AI_REVD */

int32_t  result  = 0;
LIGHT_SENSOR_Capabilities_t Capabilities;
uint32_t sensorId;
uint32_t pResult[2];/* pResult[0]for ALS channel value,pResult[1] for White channel */
uint32_t ALS_Lux ;
uint32_t WHITE_lux;
/* Private function prototypes -----------------------------------------------*/
static uint32_t LIGHT_SENSOR_LuxCompensation(uint32_t Value);
static int32_t  LIGHT_SENSOR_ConvertToLUX(uint32_t Instance, uint32_t Value, uint32_t *LuxLevel);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Light sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */

int32_t Ls_demo(void)
{

  printf("\n******LIGHT SENSORS EXAMPLE******\n");

  /* Initialize function */
  if (BSP_LIGHT_SENSOR_Init(0) != BSP_ERROR_NONE) result--;

  if (BSP_LIGHT_SENSOR_GetCapabilities(0, &Capabilities) != BSP_ERROR_NONE) result--;

  /* Read id function */
  if (BSP_LIGHT_SENSOR_ReadID(0, &sensorId) != BSP_ERROR_NONE) result--;

  /* STOP the Sensor */
  if (BSP_LIGHT_SENSOR_Stop(0)) result--;

  /* Set Sensor Gain */
  if (BSP_LIGHT_SENSOR_SetGain(0,LIGHT_SENSOR_ALS_CHANNEL, LIGHT_SENSOR_GAIN_1)!= BSP_ERROR_NONE) result--;

  /* Set Sensor Exposure Time (Integreation Time) */
  if ( BSP_LIGHT_SENSOR_SetExposureTime(0, LIGHT_SENSOR_EXPOSURE_TIME_100)!= BSP_ERROR_NONE) result--;

  HAL_Delay(100);
   /* Start the Sensor */
  if (BSP_LIGHT_SENSOR_Start(0,LIGHT_SENSOR_MODE_CONTINUOUS)) result--;

  /*Sensor Get channel values */
  if ( BSP_LIGHT_SENSOR_GetValues(0,&pResult[0])!= BSP_ERROR_NONE) result--;

  /* Convert ALS Channel Value to Lux */
  LIGHT_SENSOR_ConvertToLUX(0, pResult[0], &ALS_Lux);

  if(ALS_Lux > 10000)
  {
    ALS_Lux = LIGHT_SENSOR_LuxCompensation(ALS_Lux);
  }

  /* Convert White Channel Value to Lux */
  LIGHT_SENSOR_ConvertToLUX(0, pResult[1], &WHITE_lux);

  if(WHITE_lux > 10000)
  {
    WHITE_lux = LIGHT_SENSOR_LuxCompensation(WHITE_lux);
  }

  printf ("\n Ambiant Light channel value %u , %u lux \n",pResult[0],ALS_Lux);
  printf ("\n White Light channel value %u , %u lux \n",pResult[1],WHITE_lux);

  return result;

}
/**
  * @brief  This function compensates for lux values over 10000.
  *          Illumination values higher than 10000 lx show non-linearity
  *          value light Level
  * @param  Value  light level in lux > 10000
  * @retval Light Level in lux
  */
static uint32_t LIGHT_SENSOR_LuxCompensation(uint32_t Value)
{

  /* Polynomial is pulled from the datasheet */
  float compLux = (.00000000000060135 * (pow(Value, 4))) -
                      (.0000000093924 * (pow(Value, 3))) +
                      (.000081488 * (pow(Value,2))) +
                      (1.0023 * Value);
  return (uint32_t)compLux;
}

/** @brief  This function determines which conversion value
  *         to use by using the bit representation of the gain as an index to look up
  *         the conversion value in the correct integration time array. It then converts
  *         the value and returns it
  * @param  Instance ambiant light sensor instance. Could be only 0.
  * @param  value     Raw values to be converted in lux
  * @param  LuxLevel  Pointer to light value
  * @retval Light Level in lux
  */
static int32_t LIGHT_SENSOR_ConvertToLUX(uint32_t Instance, uint32_t Value, uint32_t *LuxLevel)
{
#if defined (USE_B_U585I_IOT02A_U585AI_REVD)
  int32_t ret = VEML6030_OK;
#else
  int32_t ret = VEML3235_OK;
#endif /* USE_B_U585I_IOT02A_U585AI_REVD */
  float luxConv = 0;
  uint8_t convPos = 0;
  uint32_t pGain;
  uint32_t pExposureTime;

  if (BSP_LIGHT_SENSOR_GetGain(Instance,LIGHT_SENSOR_ALS_CHANNEL,&pGain)!= BSP_ERROR_NONE)
  {
  }
  else if (BSP_LIGHT_SENSOR_GetExposureTime(Instance,&pExposureTime)!= BSP_ERROR_NONE)
  {
  }
  else
  {
#if defined (USE_B_U585I_IOT02A_U585AI_REVD)
    if (pGain == VEML6030_CONF_GAIN_2)
    {
      convPos = 0;
    }
    else if (pGain == VEML6030_CONF_GAIN_1)
    {
      convPos = 1;
    }
    else if (pGain == VEML6030_CONF_GAIN_1_4)
    {
      convPos = 2;
    }
    else if (pGain == VEML6030_CONF_GAIN_1_8)
    {
      convPos = 3;
    }
#else
    if (pGain == VEML3235_CONF_GAIN_1)
    {
      convPos = 2;
    }
    else if (pGain == VEML3235_CONF_GAIN_2)
    {
      convPos = 1;
    }
    else if (pGain == VEML3235_CONF_GAIN_4)
    {
      convPos = 0;
    }
#endif /* USE_B_U585I_IOT02A_U585AI_REVD */
    else
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  if(ret == BSP_ERROR_NONE)
  {
#if defined (USE_B_U585I_IOT02A_U585AI_REVD)
    if(pExposureTime == VEML6030_CONF_IT800)
    {
      luxConv = IntegrationTime_800[convPos];
    }
    else if(pExposureTime == VEML6030_CONF_IT400)
    {
      luxConv = IntegrationTime_400[convPos];
    }
    else if(pExposureTime == VEML6030_CONF_IT200)
    {
      luxConv = IntegrationTime_200[convPos];
    }
    else if(pExposureTime == VEML6030_CONF_IT100)
    {
      luxConv = IntegrationTime_100[convPos];
    }
    else if(pExposureTime == VEML6030_CONF_IT50)
    {
      luxConv = IntegrationTime_50[convPos];
    }
    else if(pExposureTime == VEML6030_CONF_IT25)
    {
      luxConv = IntegrationTime_25[convPos];
    }
    else
    {
      ret = VEML6030_INVALID_PARAM;
    }
#else
    if(pExposureTime == VEML3235_CONF_IT800)
    {
      luxConv = IntegrationTime_800[convPos];
    }
    else if(pExposureTime == VEML3235_CONF_IT400)
    {
      luxConv = IntegrationTime_400[convPos];
    }
    else if(pExposureTime == VEML3235_CONF_IT200)
    {
      luxConv = IntegrationTime_200[convPos];
    }
    else if(pExposureTime == VEML3235_CONF_IT100)
    {
      luxConv = IntegrationTime_100[convPos];
    }
    else if(pExposureTime == VEML3235_CONF_IT50)
    {
      luxConv = IntegrationTime_50[convPos];
    }
    else
    {
      ret = VEML3235_INVALID_PARAM;
    }
#endif /* USE_B_U585I_IOT02A_U585AI_REVD */
  }
  *LuxLevel =(uint32_t)(luxConv * Value);

  return ret;
}
/**
  * @}
  */

/**
  * @}
  */
