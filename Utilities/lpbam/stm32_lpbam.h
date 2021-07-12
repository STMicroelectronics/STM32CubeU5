/**
  **********************************************************************************************************************
  * @file    stm32_lpbam.h
  * @author  MCD Application Team
  * @brief   This file includes lpbam header file resources.
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32_LPBAM_H
#define STM32_LPBAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_lpbam_types.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup LPBAM
  * @brief      LPBAM module driver
  * @{
  */

/**
  * @brief Include module's header file
  */
#ifdef LPBAM_ADC_MODULE_ENABLED
#include "stm32_lpbam_adc.h"
#endif /* LPBAM_ADC_MODULE_ENABLED */

#ifdef LPBAM_COMMON_MODULE_ENABLED
#include "stm32_lpbam_common.h"
#endif /* LPBAM_COMMON_MODULE_ENABLED */

#ifdef LPBAM_COMP_MODULE_ENABLED
#include "stm32_lpbam_comp.h"
#endif /* LPBAM_COMP_MODULE_ENABLED */

#ifdef LPBAM_DAC_MODULE_ENABLED
#include "stm32_lpbam_dac.h"
#endif /* LPBAM_DAC_MODULE_ENABLED */

#ifdef LPBAM_DMA_MODULE_ENABLED
#include "stm32_lpbam_dma.h"
#endif /* LPBAM_DMA_MODULE_ENABLED */

#ifdef LPBAM_GPIO_MODULE_ENABLED
#include "stm32_lpbam_gpio.h"
#endif /* LPBAM_GPIO_MODULE_ENABLED */

#ifdef LPBAM_I2C_MODULE_ENABLED
#include "stm32_lpbam_i2c.h"
#endif /* LPBAM_I2C_MODULE_ENABLED */

#ifdef LPBAM_LPTIM_MODULE_ENABLED
#include "stm32_lpbam_lptim.h"
#endif /* LPBAM_LPTIM_MODULE_ENABLED */

#ifdef LPBAM_OPAMP_MODULE_ENABLED
#include "stm32_lpbam_opamp.h"
#endif /* LPBAM_OPAMP_MODULE_ENABLED */

#ifdef LPBAM_SPI_MODULE_ENABLED
#include "stm32_lpbam_spi.h"
#endif /* LPBAM_SPI_MODULE_ENABLED */

#ifdef LPBAM_UART_MODULE_ENABLED
#include "stm32_lpbam_uart.h"
#endif /* LPBAM_UART_MODULE_ENABLED */

#ifdef LPBAM_VREFBUF_MODULE_ENABLED
#include "stm32_lpbam_vrefbuf.h"
#endif /* LPBAM_VREFBUF_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32_LPBAM_H */
