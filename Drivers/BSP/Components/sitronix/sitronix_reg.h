/**
  ******************************************************************************
  * @file    sitronix_reg.h
  * @author  MCD Application Team
  * @brief   Header of sitronix_reg.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SITRONIX_REG_H
#define SITRONIX_REG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Macros --------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @addtogroup SITRONIX
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup SITRONIX_Exported_Types SITRONIX Exported Types
  * @{
  */
/************** Generic Function  *******************/

typedef int32_t (*SITRONIX_Write_Func)(void *, uint8_t, uint8_t *, uint16_t);
typedef int32_t (*SITRONIX_Read_Func)(void *, uint8_t, uint8_t *, uint16_t);
typedef int32_t (*SITRONIX_Recv_Func)(void *, uint8_t *, uint16_t);

/**
  * @}
  */

/** @defgroup SITRONIX_Imported_Globals SITRONIX Imported Globals
  * @{
  */
typedef struct
{
  SITRONIX_Write_Func   WriteReg;
  SITRONIX_Read_Func    ReadReg;
  SITRONIX_Recv_Func    ReadData;
  void                 *handle;
} sitronix_ctx_t;
/**
  * @}
  */

/** @defgroup SITRONIX_Exported_Constants SITRONIX Exported Constants
  * @{
  */



/**
  * @}
  */

/*******************************************************************************
  * Register      : Generic - All
  * Address       : Generic - All
  * Bit Group Name: None
  * Permission    : W
  *******************************************************************************/
int32_t sitronix_write_reg(sitronix_ctx_t *ctx, uint8_t reg, uint8_t *pbuf, uint16_t length);
int32_t sitronix_read_reg(sitronix_ctx_t *ctx, uint8_t reg, uint8_t *pbuf, uint16_t length);
int32_t sitronix_read_data(sitronix_ctx_t *ctx, uint8_t *pbuf, uint16_t length);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* SITRONIX_REG_H */


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
