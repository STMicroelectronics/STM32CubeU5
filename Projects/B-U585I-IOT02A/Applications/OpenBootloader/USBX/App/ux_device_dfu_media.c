/**
  ******************************************************************************
  * @file    ux_device_dfu_media.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
#include "ux_device_dfu_media.h"

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "tx_api.h"
#include "app_usbx_device.h"
#include "openbl_usb_cmd.h"
#include "app_openbootloader.h"
#include "common_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define DFU_MEDIA_ERASE_TIME              (uint16_t)500U
#define DFU_MEDIA_PROGRAM_TIME            (uint16_t)500U

/* Private variables ---------------------------------------------------------*/
extern TX_QUEUE ux_app_MsgQueue;
extern ux_dfu_downloadInfotypeDef ux_dfu_download;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern uint8_t UsbSofDetected;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

ULONG dfu_status = 0U;
ULONG Address_ptr;
UCHAR RX_Data[1024];

uint8_t JumpUsb = 0;

/* Private function prototypes -----------------------------------------------*/
static UINT DFU_WriteMemory(uint32_t Address, uint8_t *pDest, uint32_t Len);
static UINT DFU_ReadMemory(uint32_t Address, uint8_t *pDest, uint32_t Len);

static UINT DFU_WriteProtect(uint8_t *pListOfSectors, uint32_t Length);
static UINT DFU_WriteUnprotect(void);
static UINT DFU_ReadProtect(void);
static UINT DFU_ReadUnprotect(void);
static UINT DFU_Erase(uint32_t Address);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  Initializes Memory routine, Unlock the internal flash.
  * @param  dfu Instance.
  * @retval none.
  */
void DFU_Init(void *dfu)
{
  /* Unlock the internal flash */
  HAL_FLASH_Unlock();
}

/**
  * @brief  DeInitializes Memory routine, Lock the internal flash.
  * @param  dfu: dfu Instance.
  * @retval none.
  */
void DFU_DeInit(void *dfu)
{
  /* Lock the internal flash */
  HAL_FLASH_Lock();
}

/**
  * @brief  Get status routine.
  * @param  dfu: dfu Instance.
  * @param  media_status : dfu media status.
  * @retval UX_SUCCESS.
  */
UINT DFU_GetStatus(void *dfu, ULONG *media_status)
{
  *media_status = dfu_status;

  return (UX_SUCCESS);
}

/**
  * @brief  Inform application when a begin and end of transfer of the firmware
            occur.
  * @param  dfu: dfu Instance.
  * @param  notification: unused.
  * @retval UX_SUCCESS.
  */
UINT DFU_Notify(void *dfu, ULONG notification)
{
  UNUSED(notification);

  return (UX_SUCCESS);
}

/**
  * @brief  Memory read routine.
  * @param  dfu: dfu Instance
  * @param  block_number: block number.
  * @param  data_pointer: Pointer to the Source buffer.
  * @param  length: Number of data to be read (in bytes).
  * @retval Status.
  */
UINT DFU_Read(VOID *dfu, ULONG block_number, UCHAR *data_pointer,
              ULONG length, ULONG *media_status)
{
  UINT   Status      = UX_SUCCESS;
  ULONG  Address_src = 0;

  if (block_number == 0)
  {
    /* Store the values of all supported commands */
    *data_pointer       = DFU_CMD_GETCOMMANDS;
    *(data_pointer + 1) = DFU_CMD_SETADDRESSPOINTER;
    *(data_pointer + 2) = DFU_CMD_ERASE ;
    *(data_pointer + 3) = 0;
  }
  else if (block_number > 0)
  {
    /* Return the physical address from which the host requests to read data */
    Address_src = ((block_number - 2) * UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) + Address_ptr;

    /* Read Memory */
    if (DFU_ReadMemory(Address_src, data_pointer, length) != UX_SUCCESS)
    {
      /* Set DFU media Status Error */
      dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4;

      /* syncs the USB DFU device state */
      ux_device_class_dfu_state_sync(dfu);
    }
    else
    {
      /* Set DFU Status OK */
      dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

      /* syncs the USB DFU device state */
      ux_device_class_dfu_state_sync(dfu);
    }

    *media_status = length;
  }
  else
  {
    Status = UX_ERROR;
  }

  return (Status);
}

/**
  * @brief  Memory write routine.
  * @param  dfu: dfu Instance.
  * @param  block_number: block number
  * @param  data_pointer: Pointer to the Source buffer.
  * @param  length: Number of data to be read (in bytes).
  * @param  media_status: Not used.
  * @retval status.
  */
UINT DFU_Write(VOID *dfu, ULONG block_number, UCHAR *data_pointer,
               ULONG length, ULONG *media_status)
{
  UINT  status          = 0U;
  ULONG dfu_polltimeout = 0U;

  /* Store ux_dfu_download info */
  ux_dfu_download.wlength    = length;
  ux_dfu_download.data_ptr   = RX_Data;
  ux_dfu_download.wblock_num = block_number;

  ux_utility_memory_copy(ux_dfu_download.data_ptr, data_pointer, length);

  if ((block_number == 0) && (*data_pointer == DFU_CMD_ERASE))
  {
    /* Set the time necessary for an erase operation */
    dfu_polltimeout = DFU_MEDIA_ERASE_TIME;

    /* Set DFU media status Busy, dfu polltimeout in erase phase */
    dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
    dfu_status += (uint8_t)(dfu_polltimeout) << 8;
  }
  else
  {
    /* Set the time necessary for a program operation */
    dfu_polltimeout = DFU_MEDIA_PROGRAM_TIME;

    /* Set DFU media status Busy, dfu polltimeout in program phase */
    dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
    dfu_status += (uint8_t)(dfu_polltimeout) << 8;
  }

  /* Put a message queue to usbx_dfu_download_thread_entry */
  if (tx_queue_send(&ux_app_MsgQueue, &ux_dfu_download, TX_NO_WAIT))
  {
    Error_Handler();
  }

  return (status);
}

/**
  * @brief  Handles the sub-protocol DFU leave DFU mode request (leaves DFU mode
  *         and resets device to jump to user loaded code).
  * @param  dfu: dfu Instance.
  * @param  transfer: transfer request.
  * @retval None.
  */
UINT DFU_Leave(VOID *dfu, UX_SLAVE_TRANSFER *transfer)
{
  UCHAR *setup;
  UCHAR dfu_state;
  UINT  status = UX_ERROR;

  /* Get DFU state */
  dfu_state = _ux_device_class_dfu_state_get((UX_SLAVE_CLASS_DFU *)dfu);

  setup  = transfer->ux_slave_transfer_request_setup;

  if ((dfu_state == UX_SYSTEM_DFU_STATE_DFU_IDLE) ||
      (dfu_state == UX_SYSTEM_DFU_STATE_DFU_DNLOAD_IDLE))
  {

    if (setup[UX_SETUP_REQUEST] == UX_SLAVE_CLASS_DFU_COMMAND_DOWNLOAD)
    {

      if ((setup[UX_SETUP_LENGTH] == 0) && (setup[UX_SETUP_LENGTH + 1] == 0))
      {
        /* Set USB jump token */
        JumpUsb = 1;

        status = UX_SUCCESS;
      }
    }
  }

  return (status);
}

/**
  * @brief  Function implementing usbx_dfu_download_thread_entry.
  * @param  arg: Not used.
  * @retval None.
  */
void usbx_dfu_download_thread_entry(ULONG arg)
{
  UINT status;
  UINT Command;
  ULONG Length;
  ULONG Address_dest;
  UX_SLAVE_CLASS_DFU *dfu = NULL;

  while (1)
  {
    /* Receive a message queue from DFU_Write callback */
    status = tx_queue_receive(&ux_app_MsgQueue, &ux_dfu_download, TX_WAIT_FOREVER);

    /* Check the completion code and the actual flags returned. */
    if (status == UX_SUCCESS)
    {
      if (ux_dfu_download.wblock_num == 0)
      {
        Command = *(ux_dfu_download.data_ptr);

        /* Decode the Special Command */
        switch (Command)
        {
          case DFU_CMD_SETADDRESSPOINTER:

            /* Get address pointer value used for computing the start address
              for Read and Write memory operations */
            Address_ptr  =  *(ux_dfu_download.data_ptr + 1) ;
            Address_ptr += *(ux_dfu_download.data_ptr + 2) << 8 ;
            Address_ptr += *(ux_dfu_download.data_ptr + 3) << 16 ;
            Address_ptr += *(ux_dfu_download.data_ptr + 4) << 24 ;

            /* Set DFU Status OK */
            dfu_status  =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

            /* Update USB DFU state machine */
            ux_device_class_dfu_state_sync(dfu);

            break;

          case DFU_CMD_ERASE:

            /* Get address pointer value to erase one page of the internal
               media memory. */
            Address_ptr  =  *(ux_dfu_download.data_ptr + 1);
            Address_ptr += *(ux_dfu_download.data_ptr + 2) << 8;
            Address_ptr += *(ux_dfu_download.data_ptr + 3) << 16;
            Address_ptr += *(ux_dfu_download.data_ptr + 4) << 24;

            /* Erase memory */
            if (DFU_Erase(Address_ptr) != UX_SUCCESS)
            {
              dfu_status  =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
              dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_ERASE << 4;
            }
            else
            {
              /* Set DFU status OK */
              dfu_status  =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
              dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
            }

            /* Update USB DFU state machine */
            ux_device_class_dfu_state_sync(dfu);

            break;

          case DFU_CMD_WRITE_PROTECT:
            /* Get the write protect length */
            Length = *(ux_dfu_download.data_ptr + 1);

            /* Write protect command */
            DFU_WriteProtect((ux_dfu_download.data_ptr + 2), Length);

            /* Set DFU Status OK */
            dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

            /* syncs the USB DFU device state */
            ux_device_class_dfu_state_sync(dfu);
            break;

          case DFU_CMD_WRITE_UNPROTECT:
            /* Write unprotect command */
            DFU_WriteUnprotect();

            /* Set DFU Status OK */
            dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

            /* syncs the USB DFU device state */
            ux_device_class_dfu_state_sync(dfu);
            break;

          case DFU_CMD_READ_PROTECT:
            /* Read protect command */
            DFU_ReadProtect();

            /* Set DFU Status OK */
            dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

            /* syncs the USB DFU device state */
            ux_device_class_dfu_state_sync(dfu);
            break;

          case DFU_CMD_READ_UNPROTECT:
            /* Read unprotect command */
            DFU_ReadUnprotect();

            /* Set DFU Status OK */
            dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

            /* syncs the USB DFU device state */
            ux_device_class_dfu_state_sync(dfu);
            break;

          default:
            break;

        }
      }
      /* Regular Download Command */
      else
      {
        /* Decode the required address to which the host requests to write data */
        Address_dest = ((ux_dfu_download.wblock_num - 2U) * UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) +  Address_ptr;

        /* Write Memory */
        if (DFU_WriteMemory(Address_dest, ux_dfu_download.data_ptr, ux_dfu_download.wlength) != UX_SUCCESS)
        {
          /* Set DFU media Status Error */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4;
        }
        else
        {
          /* Set DFU Status OK */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
        }

        /* Update USB DFU state machine */
        ux_device_class_dfu_state_sync(dfu);
      }
    }
    else
    {
      tx_thread_sleep(1);
    }
  }
}

/**
  * @brief DFU device connection callback.
  * @param  Device_State: dfu Instance.
  * @retval status.
  */
UINT DFU_Device_ConnectionCallback(ULONG Device_State)
{

  if (Device_State == UX_DEVICE_REMOVED)
  {
    if (_ux_system_slave -> ux_system_slave_device_dfu_mode ==  UX_DEVICE_CLASS_DFU_MODE_DFU)
    {
    }
  }
  else if (Device_State == UX_DEVICE_ATTACHED)
  {
    /* Notify the OpenBL the detection of a USB interface */
    UsbSofDetected = 1;
  }

  return UX_SUCCESS;
}

/**
  * @brief  Write to flash memory.
  * @param  Address: Address to read from.
  * @param  pDest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadMemory(uint32_t Address, uint8_t *pDest, uint32_t Len)
{
  OPENBL_USB_ReadMemory((uint8_t *)(&Address), pDest, Len);
  return (UX_SUCCESS);
}

/**
  * @brief  Write to flash memory.
  * @param  Address: Address to be written to.
  * @param  pDest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteMemory(uint32_t Address, uint8_t *pDest, uint32_t Len)
{
  OPENBL_USB_WriteMemory(pDest, (uint8_t *)(&Address), Len);
  return (UX_SUCCESS);
}

/**
  * @brief  Jump to user application
  * @retval None
  */
void DFU_Jump(void)
{
  /* Disable the USB */
  HAL_PCD_Stop(&hpcd_USB_OTG_FS);

  /* Jump to address */
  if (Address_ptr != 0)
  {
    OPENBL_USB_Jump(Address_ptr);
  }
  else
  {
    OPENBL_USB_Jump(USBD_DFU_APP_DEFAULT_ADD);
  }
}

/**
  * @brief  Erase sector.
  * @param  Address: Address of sector to be erased.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_Erase(uint32_t Address)
{
  TX_INTERRUPT_SAVE_AREA

  /* Disable interrupts to prevent the kernel disruption */
  TX_DISABLE

  OPENBL_USB_EraseMemory(Address);

  /* Restore interrupts */
  TX_RESTORE

  return (UX_SUCCESS);
}

/**
  * @brief  Write protect.
  * @param  pListOfSectors: List of sectors to be protected.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteProtect(uint8_t *pListOfSectors, uint32_t Length)
{
  OPENBL_USB_WriteProtect(pListOfSectors, Length);
  return (UX_SUCCESS);
}

/**
  * @brief  Write unprotect.
  * @param  pListOfSectors: List of sectors to be protected.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteUnprotect(void)
{
  OPENBL_USB_WriteUnprotect();
  return (UX_SUCCESS);
}

/**
  * @brief  Read protect.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadProtect(void)
{
  OPENBL_USB_ReadProtect();
  return (UX_SUCCESS);
}

/**
  * @brief  Read unprotect.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadUnprotect(void)
{
  OPENBL_USB_ReadUnprotect();
  return (UX_SUCCESS);
}
