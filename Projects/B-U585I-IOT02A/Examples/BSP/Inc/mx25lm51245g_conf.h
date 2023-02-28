/**
  ******************************************************************************
  * @file    mx25lm51245g_conf.h
  * @author  MCD Application Team
  * @brief   MX25LM51245G OctoSPI memory configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017-2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX25LM51245G_CONF_H
#define MX25LM51245G_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup BSP
  * @{
  */
#define CONF_OSPI_ODS                MX25LM51245G_CR_ODS_24   /* MX25LM51245G Output Driver Strength */

#define DUMMY_CYCLES_READ            8U
#define DUMMY_CYCLES_READ_OCTAL      6U
#define DUMMY_CYCLES_READ_OCTAL_DTR  6U
#define DUMMY_CYCLES_REG_OCTAL       4U
#define DUMMY_CYCLES_REG_OCTAL_DTR   5U

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* MX25LM51245G_CONF_H */
