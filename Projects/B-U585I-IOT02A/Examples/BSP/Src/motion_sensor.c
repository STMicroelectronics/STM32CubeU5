/**
  ******************************************************************************
  * @file    motion_sensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the motion sensor feature in the
  *          b_u585i_iot02_motion_sensors  driver
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
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Motion sensor demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Ms_demo(void)
{
  int32_t  result  = 0;
  MOTION_SENSOR_Capabilities_t Capabilities;
  BSP_MOTION_SENSOR_Axes_t         axes;
  BSP_MOTION_SENSOR_AxesRaw_t      axesRaw;
  uint8_t chipId;
  float sensitivity, outputDataRate;
  int32_t fullScale;

  printf("\n******MOTION SENSORS EXAMPLE******\n");
  printf("Press user button for next step\n");

  /* Initialize user button */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE) result--;

  /* --------------- */
  /* TEST of IIS2MDC */
  /* --------------- */

  /*************************/
  /* Test of Accelerometer */
  /*************************/
  printf("TEST OF IIS2MDC ACCLEROMETER\n");

  /* Initialize function */
  if (BSP_MOTION_SENSOR_Init(0, MOTION_ACCELERO) != BSP_ERROR_NONE) result--;

  if (BSP_MOTION_SENSOR_GetCapabilities(0, &Capabilities) != BSP_ERROR_NONE) result--;

  /* Read id function */
  if (BSP_MOTION_SENSOR_ReadID(0, &chipId) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_MOTION_SENSOR_Enable(0, MOTION_ACCELERO) != BSP_ERROR_NONE) result--;

  /* GetFullScale and SetFullScale functions */
  if (BSP_MOTION_SENSOR_SetFullScale(0, MOTION_ACCELERO, 16) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetFullScale(0, MOTION_ACCELERO, &fullScale) != BSP_ERROR_NONE) result--;
  if (fullScale != 16) result--;

  if (BSP_MOTION_SENSOR_SetFullScale(0, MOTION_ACCELERO, 2) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetFullScale(0, MOTION_ACCELERO, &fullScale) != BSP_ERROR_NONE) result--;
  if (fullScale != 2) result--;

  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_MOTION_SENSOR_SetOutputDataRate(0, MOTION_ACCELERO, 3333.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(0, MOTION_ACCELERO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 6667.0f) result--;

  if (BSP_MOTION_SENSOR_SetOutputDataRate(0, MOTION_ACCELERO, 208.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(0, MOTION_ACCELERO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 208.0f) result--;

  /* GetSensitivity function */
  if (BSP_MOTION_SENSOR_GetSensitivity(0, MOTION_ACCELERO, &sensitivity) != BSP_ERROR_NONE) result--;

  /* GetAxes function */
  printf("Accelerometer axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxes(0, MOTION_ACCELERO, &axes) != BSP_ERROR_NONE) result--;
    printf("X = %d, Y = %d, , Z = %d\n", axes.xval, axes.yval, axes.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* GetAxesRaw function */
  printf("Accelerometer raw axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxesRaw(0, MOTION_ACCELERO, &axesRaw) != BSP_ERROR_NONE) result--;
    printf("Xraw = %d, Yraw = %d, , Zraw = %d\n", axesRaw.xval, axesRaw.yval, axesRaw.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_MOTION_SENSOR_Disable(0, MOTION_ACCELERO) != BSP_ERROR_NONE) result--;

  /* De-initialize function */
  if (BSP_MOTION_SENSOR_DeInit(0) != BSP_ERROR_NONE) result--;

  /*********************/
  /* Test of Gyroscope */
  /*********************/
  printf("TEST OF IIS2MDC GYROSCOPE\n");

  /* Initialize function */
  if (BSP_MOTION_SENSOR_Init(0, MOTION_GYRO) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_MOTION_SENSOR_Enable(0, MOTION_GYRO) != BSP_ERROR_NONE) result--;

  /* GetFullScale and SetFullScale functions */
  if (BSP_MOTION_SENSOR_SetFullScale(0, MOTION_GYRO, 2000) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetFullScale(0, MOTION_GYRO, &fullScale) != BSP_ERROR_NONE) result--;
  if (fullScale != 2000) result--;

  if (BSP_MOTION_SENSOR_SetFullScale(0, MOTION_GYRO, 500) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetFullScale(0, MOTION_GYRO, &fullScale) != BSP_ERROR_NONE) result--;
  if (fullScale != 500) result--;

  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_MOTION_SENSOR_SetOutputDataRate(0, MOTION_GYRO, 3332.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(0, MOTION_GYRO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 3332.0f) result--;

  if (BSP_MOTION_SENSOR_SetOutputDataRate(0, MOTION_GYRO, 208.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(0, MOTION_GYRO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 208.0f) result--;

  /* GetSensitivity function */
  if (BSP_MOTION_SENSOR_GetSensitivity(0, MOTION_GYRO, &sensitivity) != BSP_ERROR_NONE) result--;

  /* GetAxes function */
  printf("Gyroscope axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxes(0, MOTION_GYRO, &axes) != BSP_ERROR_NONE) result--;
    printf("X = %d, Y = %d, , Z = %d\n", axes.xval, axes.yval, axes.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* GetAxesRaw function */
  printf("Gyroscope raw axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxesRaw(0, MOTION_GYRO, &axesRaw) != BSP_ERROR_NONE) result--;
    printf("Xraw = %d, Yraw = %d, , Zraw = %d\n", axesRaw.xval, axesRaw.yval, axesRaw.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_MOTION_SENSOR_Disable(0, MOTION_GYRO) != BSP_ERROR_NONE) result--;

  /* De-initialize function */
  if (BSP_MOTION_SENSOR_DeInit(0) != BSP_ERROR_NONE) result--;

  /* --------------- */
  /* TEST of ISM330DHCX */
  /* --------------- */

  /************************/
  /* Test of Magnetometer */
  /************************/
  printf("TEST OF ISM330DHCX MAGNETOMETER\n");

  /* Initialize function */
  if (BSP_MOTION_SENSOR_Init(1, MOTION_MAGNETO) != BSP_ERROR_NONE) result--;

  if (BSP_MOTION_SENSOR_GetCapabilities(1, &Capabilities) != BSP_ERROR_NONE) result--;

  /* Read id function */
  if (BSP_MOTION_SENSOR_ReadID(1, &chipId) != BSP_ERROR_NONE) result--;

  /* Enable function */
  if (BSP_MOTION_SENSOR_Enable(1, MOTION_MAGNETO) != BSP_ERROR_NONE) result--;

  /* GetFullScale functions */
  if (BSP_MOTION_SENSOR_GetFullScale(1, MOTION_MAGNETO, &fullScale) != BSP_ERROR_NONE) result--;
  if (fullScale != 50) result--;

  /* GetOutputDataRate and SetOutputDataRate functions */
  if (BSP_MOTION_SENSOR_SetOutputDataRate(1, MOTION_MAGNETO, 80.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(1, MOTION_MAGNETO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 100.0f) result--;

  if (BSP_MOTION_SENSOR_SetOutputDataRate(1, MOTION_MAGNETO, 20.0f) != BSP_ERROR_NONE) result--;
  if (BSP_MOTION_SENSOR_GetOutputDataRate(1, MOTION_MAGNETO, &outputDataRate) != BSP_ERROR_NONE) result--;
  if (outputDataRate != 20.0f) result--;

  /* GetSensitivity function */
  if (BSP_MOTION_SENSOR_GetSensitivity(1, MOTION_MAGNETO, &sensitivity) != BSP_ERROR_NONE) result--;

  /* GetAxes function */
  printf("Magnetometer axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxes(1, MOTION_MAGNETO, &axes) != BSP_ERROR_NONE) result--;
    printf("X = %d, Y = %d, , Z = %d\n", axes.xval, axes.yval, axes.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* GetAxesRaw function */
  printf("Magnetometer raw axes:\n");
  while (UserButtonPressed == 0)
  {
    if (BSP_MOTION_SENSOR_GetAxesRaw(1, MOTION_MAGNETO, &axesRaw) != BSP_ERROR_NONE) result--;
    printf("Xraw = %d, Yraw = %d, , Zraw = %d\n", axesRaw.xval, axesRaw.yval, axesRaw.zval);
    HAL_Delay(500);
  }
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Disable function */
  if (BSP_MOTION_SENSOR_Disable(1, MOTION_MAGNETO) != BSP_ERROR_NONE) result--;

  /* De-initialize function */
  if (BSP_MOTION_SENSOR_DeInit(1) != BSP_ERROR_NONE) result--;

  /***************/
  /* End of example */
  /***************/

  /* De-Initialize User Button */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE) result--;

  printf("Motion Sensors Example Ended\n");
  return result;
}


/**
  * @}
  */

/**
  * @}
  */

