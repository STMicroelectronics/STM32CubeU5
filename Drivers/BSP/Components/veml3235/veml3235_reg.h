/*
  ******************************************************************************
  * @file    veml3235_reg.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          veml3235_reg.c driver.
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
#ifndef VEML3235_REG_H
#define VEML3235_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/* VEML3235 registers */
#define VEML3235_REG_ALS_CONF        0U
#define VEML3235_REG_ALS_WH          1U
#define VEML3235_REG_ALS_WL          2U
#define VEML3235_REG_POWER_SAVING    3U
#define VEML3235_REG_ALS             5U
#define VEML3235_REG_WHITE           4U
#define VEML3235_REG_ALS_INT         6U

/* ALS gain constants */
#define VEML3235_CONF_GAIN_1         0U
#define VEML3235_CONF_GAIN_2         (0x01UL << 11)
#define VEML3235_CONF_GAIN_4         (0x03UL << 11)
#define VEML3235_GAIN_MASK           (0x03UL << 11)

/* ALS integration times (ms) */
#define VEML3235_CONF_IT50           (0x08UL << 6)
#define VEML3235_CONF_IT100          (0x00UL << 6)
#define VEML3235_CONF_IT200          (0x01UL << 6)
#define VEML3235_CONF_IT400          (0x02UL << 6)
#define VEML3235_CONF_IT800          (0x03UL << 6)
/* ALS integration times - all bits */
#define VEML3235_CONF_IT_MASK        (0x0FU << 6)

/* ALS shutdown setting */
#define VEML3235_CONF_SHUTDOWN0      0x01U



/************** Generic Function  *******************/

typedef int32_t (*VEML3235_Write_Func)(void *, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VEML3235_Read_Func)(void *, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  VEML3235_Write_Func   WriteReg;
  VEML3235_Read_Func    ReadReg;
  void                  *handle;
} veml3235_ctx_t;

/*******************************************************************************
  * Register      : Generic - All
  * Address       : Generic - All
  * Bit Group Name: None
  * Permission    : W
  *******************************************************************************/
int32_t veml3235_write_reg(const veml3235_ctx_t *ctx, uint16_t reg, const uint16_t *pdata, uint16_t length);
int32_t veml3235_read_reg(const veml3235_ctx_t *ctx, uint16_t reg, uint16_t *pdata, uint16_t length);



#ifdef __cplusplus
}
#endif

#endif /*VEML3235_REG_H */
