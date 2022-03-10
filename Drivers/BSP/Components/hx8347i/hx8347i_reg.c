/**
  ******************************************************************************
  * @file    hx8347i_reg.c
  * @author  MCD Application Team
  * @brief   This file provides unitary register function to control the HX8347I
  *          LCD driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hx8347i_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup HX8347I
  * @{
  */

/** @addtogroup HX8347I_REG_Exported_Functions
  * @{
  */
/*******************************************************************************
  * Function Name : hx8347i_read_reg.
  * Description   : Generic Reading function.
  * Input         : Driver context, register Address, length of buffer.
  * Output        : Status.
  *******************************************************************************/
int32_t hx8347i_read_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *data, uint32_t length)
{
  return ctx->ReadReg(ctx->handle, reg, data, length);
}

/*******************************************************************************
  * Function Name : hx8347i_write_reg.
  * Description   : Generic Writing function.
  * Input         : Driver context, Register Address, data to be written,
                  length of buffer.
  * Output        : Status.
  *******************************************************************************/
int32_t hx8347i_write_reg(HX8347I_ctx_t *ctx, uint16_t reg, uint8_t *data, uint32_t length)
{
  return ctx->WriteReg(ctx->handle, reg, data, length);
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
