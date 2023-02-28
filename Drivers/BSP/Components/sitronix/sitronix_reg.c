/**
  ******************************************************************************
  * @file    sitronix_reg.c
  * @author  MCD Application Team
  * @brief   This file provides unitary register function to control the sitronix Touch
  *
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
#include "sitronix_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup SITRONIX
  * @{
  */

/*******************************************************************************
  * Function Name : sitronix_read_reg
  * Description   : Generic Reading function. It must be full-filled with either
  *                 I2C or SPI reading functions
  * Input         : Register Address, length of buffer
  * Output        : pdata Read
  *******************************************************************************/
int32_t sitronix_read_reg(sitronix_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->ReadReg(ctx->handle, reg, pdata, length);
}

/*******************************************************************************
  * Function Name : sitronix_write_reg
  * Description   : Generic Writing function. It must be full-filled with either
  *                 I2C or SPI writing function
  * Input         : Register Address, pdata to be written, length of buffer
  * Output        : None
  *******************************************************************************/
int32_t sitronix_write_reg(sitronix_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->WriteReg(ctx->handle, reg, pdata, length);
}

/*******************************************************************************
  * Function Name : sitronix_read_data
  * Description   : Generic Reading function. It must be full-filled with either
  *                 I2C or SPI reading functions
  * Input         : Register Address, length of buffer
  * Output        : pdata Read
  *******************************************************************************/
int32_t sitronix_read_data(sitronix_ctx_t *ctx, uint8_t *pdata, uint16_t length)
{
  return ctx->ReadData(ctx->handle, pdata, length);
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
