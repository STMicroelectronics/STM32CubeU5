/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_comp.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM COMP defines
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

/* Define to prevent recursive inclusion ---------------------------------------------------------------------------- */
#ifndef STM32_PLATFORM_LPBAM_COMP_H
#define STM32_PLATFORM_LPBAM_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMP_Platform
  * @brief      COMP Platform LPBAM module driver
  * @{
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMP_Platform_Exported_Constants LPBAM COMP Platform Exported Constants
  * @brief    LPBAM COMP Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_COMP_OutputPolarity LPBAM COMP Output Polarity
  * @{
  */
#define LPBAM_COMP_OUTPUTPOL_NONINVERTED (0x0000U) /*!< LPBAM COMP output level is not inverted (comparator output is
                                                        high when the input plus is at a higher voltage than the input
                                                        minus)                                                        */
#define LPBAM_COMP_OUTPUTPOL_INVERTED    (0x8000U) /*!< LPBAM COMP output level is inverted (comparator output is low
                                                        when the input plus is at a higher voltage than the input
                                                        minus)                                                        */
/**
  * @}
  */

/** @defgroup LPBAM_COMP_InputPlus LPBAM COMP Input Plus (Non-Inverting Input)
  * @{
  */
#define LPBAM_COMP_INPUT_PLUS_IO1 (0x0000U) /*!< LPBAM comparator input plus connected to IO1 (pin PC5 for COMP1, pin
                                                 PB4 for COMP2 when available within device)                          */
#define LPBAM_COMP_INPUT_PLUS_IO2 (0x0100U) /*!< LPBAM comparator input plus connected to IO2 (pin PB2 for COMP1, pin
                                                 PB6 for COMP2 when available within device)                          */
#define LPBAM_COMP_INPUT_PLUS_IO3 (0x0200U) /*!< LPBAM comparator input plus connected to IO3 (pin PA2 for COMP1)     */
#if defined(COMP_CSR_INPSEL_2)
#define LPBAM_COMP_INPUT_PLUS_IO4 (0x0300U) /*!< LPBAM comparator input plus connected to IO4 (pin PB3 for COMP1)     */
#define LPBAM_COMP_INPUT_PLUS_IO5 (0x0400U) /*!< LPBAM comparator input plus connected to IO5 (pin PB4 for COMP1)     */
#define LPBAM_COMP_INPUT_PLUS_IO6 (0x0500U) /*!< LPBAM comparator input plus connected to IO6 (pin PB6 for COMP1)     */
#endif /* COMP_CSR_INPSEL_2 */
/**
  * @}
  */

/** @defgroup LPBAM_COMP_InputMinus LPBAM COMP Input Minus (Inverting Input)
  * @{
  */
#define LPBAM_COMP_INPUT_MINUS_1_4VREFINT (0x00U) /*!< LPBAM comparator input minus connected to 1/4 VrefInt      */
#define LPBAM_COMP_INPUT_MINUS_1_2VREFINT (0x10U) /*!< LPBAM comparator input minus connected to 1/2 VrefInt      */
#define LPBAM_COMP_INPUT_MINUS_3_4VREFINT (0x20U) /*!< LPBAM comparator input minus connected to 3/4 VrefInt      */
#define LPBAM_COMP_INPUT_MINUS_VREFINT    (0x30U) /*!< LPBAM comparator input minus connected to VrefInt          */
#define LPBAM_COMP_INPUT_MINUS_DAC1_CH1   (0x40U) /*!< LPBAM comparator input minus connected to DAC1 channel 1
                                                       (DAC_OUT1)                                                 */
#define LPBAM_COMP_INPUT_MINUS_DAC1_CH2   (0x50U) /*!< LPBAM comparator input minus connected to DAC1 channel 2
                                                       (DAC_OUT2)                                      */
#define LPBAM_COMP_INPUT_MINUS_IO1        (0x60U) /*!< LPBAM comparator input minus connected to IO1 (pin PB1 for
                                                       COMP1, pin PB7 for COMP2 when available within device)     */
#define LPBAM_COMP_INPUT_MINUS_IO2        (0x70U) /*!< LPBAM comparator input minus connected to IO2 (pin PC4 for
                                                       COMP1, pin PB3 for COMP2 when available within device)     */
/**
  * @}
  */
/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMP_Platform_Private_Constants LPBAM COMP Platform Private Constants
  * @{
  */
#define LPBAM_COMP_OUTPUT_MASK (0x40000000U) /*!< LPBAM comparator output mask */
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

#endif /* STM32_PLATFORM_LPBAM_COMP_H */
