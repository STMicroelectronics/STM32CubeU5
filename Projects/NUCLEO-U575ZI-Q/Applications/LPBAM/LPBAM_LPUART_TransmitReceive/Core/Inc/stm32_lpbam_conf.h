/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_conf.h
  * @author  MCD Application Team
  * @brief   lpbam configuration file.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32_LPBAM_CONF_H
#define STM32_LPBAM_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
/**
  * @brief Include device header file
  */
#include "stm32u5xx_hal.h"

/* ############################################## Module Selection ################################################## */
/**
  * @brief This is the list of modules to be used in the LPBAM utilities
  */

/*#define LPBAM_ADC_MODULE_ENABLED   */
/*#define LPBAM_COMP_MODULE_ENABLED   */
/*#define LPBAM_DAC_MODULE_ENABLED   */
/*#define LPBAM_LPDMA_MODULE_ENABLED   */
/*#define LPBAM_LPGPIO_MODULE_ENABLED   */
/*#define LPBAM_I2C_MODULE_ENABLED   */
/*#define LPBAM_LPTIM_MODULE_ENABLED   */
/*#define LPBAM_OPAMP_MODULE_ENABLED   */
/*#define LPBAM_RTC_MODULE_ENABLED   */
/*#define LPBAM_SPI_MODULE_ENABLED   */
#define LPBAM_UART_MODULE_ENABLED
/*#define LPBAM_VREFBUF_MODULE_ENABLED   */
#define LPBAM_COMMON_MODULE_ENABLED

#ifdef __cplusplus
}
#endif

#endif /* STM32_LPBAM_CONF_H */
