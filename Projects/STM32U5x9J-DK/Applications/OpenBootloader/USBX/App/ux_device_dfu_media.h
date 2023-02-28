/**
  ******************************************************************************
  * @file    ux_device_dfu_media.h
  * @author  MCD Application Team
  * @brief   USBX Device DFU interface header file
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
#ifndef __UX_DEVICE_DFU_MEDIA_H__
#define __UX_DEVICE_DFU_MEDIA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"

/* Private includes ----------------------------------------------------------*/
#include "ux_device_class_dfu.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  ULONG wblock_num;
  ULONG wlength;
  UCHAR *data_ptr;
} ux_dfu_downloadInfotypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Special Commands with Download Request */
#define DFU_CMD_GETCOMMANDS               0x00U
#define DFU_CMD_SETADDRESSPOINTER         0x21U
#define DFU_CMD_ERASE                     0x41U
#define DFU_CMD_READ_PROTECT              0x91U
#define DFU_CMD_READ_UNPROTECT            0x92U
#define DFU_CMD_WRITE_PROTECT             0x81U
#define DFU_CMD_WRITE_UNPROTECT           0x82U

/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void DFU_Init(void *dfu);
void DFU_DeInit(void *dfu);
UINT DFU_GetStatus(void *dfu, ULONG *media_status);
UINT DFU_Notify(VOID *dfu, ULONG notification);
UINT DFU_Read(VOID *dfu, ULONG block_number, UCHAR *data_pointer, ULONG length, ULONG *media_status);
UINT DFU_Write(VOID *dfu, ULONG block_number, UCHAR *data_pointer, ULONG length, ULONG *media_status);
UINT DFU_Leave(VOID *dfu, UX_SLAVE_TRANSFER *transfer);
UINT DFU_Device_ConnectionCallback(ULONG Device_State);
void usbx_dfu_download_thread_entry(ULONG arg);

void DFU_Jump(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __UX_DEVICE_DFU_MEDIA_H__ */
