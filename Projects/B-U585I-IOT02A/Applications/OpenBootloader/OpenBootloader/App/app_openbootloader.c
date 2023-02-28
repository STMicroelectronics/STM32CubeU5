/**
  ******************************************************************************
  * @file    app_openbootloader.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "openbl_core.h"
#include "openbl_mem.h"
#include "openbl_fdcan_cmd.h"
#include "openbl_usart_cmd.h"
#include "openbl_i2c_cmd.h"
#include "openbl_spi_cmd.h"

#include "app_openbootloader.h"
#include "usart_interface.h"
#include "i2c_interface.h"
#include "spi_interface.h"
#include "fdcan_interface.h"
#include "usb_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OPENBL_HandleTypeDef USART_Handle;
static OPENBL_HandleTypeDef I2C_Handle;
static OPENBL_HandleTypeDef FDCAN_Handle;
static OPENBL_HandleTypeDef SPI_Handle;
static OPENBL_HandleTypeDef USB_Handle;
static OPENBL_HandleTypeDef IWDG_Handle;

static OPENBL_OpsTypeDef USART_Ops =
{
  OPENBL_USART_Configuration,
  OPENBL_USART_DeInit,
  OPENBL_USART_ProtocolDetection,
  OPENBL_USART_GetCommandOpcode,
  OPENBL_USART_SendByte
};

static OPENBL_OpsTypeDef I2C_Ops =
{
  OPENBL_I2C_Configuration,
  OPENBL_I2C_DeInit,
  OPENBL_I2C_ProtocolDetection,
  OPENBL_I2C_GetCommandOpcode,
  OPENBL_I2C_SendAcknowledgeByte
};

static OPENBL_OpsTypeDef FDCAN_Ops =
{
  OPENBL_FDCAN_Configuration,
  OPENBL_FDCAN_DeInit,
  OPENBL_FDCAN_ProtocolDetection,
  OPENBL_FDCAN_GetCommandOpcode,
  OPENBL_FDCAN_SendByte
};

static OPENBL_OpsTypeDef SPI_Ops =
{
  OPENBL_SPI_Configuration,
  OPENBL_SPI_DeInit,
  OPENBL_SPI_ProtocolDetection,
  OPENBL_SPI_GetCommandOpcode,
  OPENBL_SPI_SendAcknowledgeByte
};

static OPENBL_OpsTypeDef USB_Ops =
{
  OPENBL_USB_Configuration,
  NULL,
  OPENBL_USB_ProtocolDetection,
  NULL,
  NULL
};

static OPENBL_OpsTypeDef IWDG_Ops =
{
  OPENBL_IWDG_Configuration,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported variables --------------------------------------------------------*/
uint16_t SpecialCmdList[SPECIAL_CMD_MAX_NUMBER] =
{
  SPECIAL_CMD_DEFAULT
};

uint16_t ExtendedSpecialCmdList[EXTENDED_SPECIAL_CMD_MAX_NUMBER] =
{
  SPECIAL_CMD_DEFAULT
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize open Bootloader.
  * @retval None.
  */
void OpenBootloader_Init(void)
{
  /* Register USART interfaces */
  USART_Handle.p_Ops = &USART_Ops;
  USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();

  if (OPENBL_RegisterInterface(&USART_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register I2C interfaces */
  I2C_Handle.p_Ops = &I2C_Ops;
  I2C_Handle.p_Cmd = OPENBL_I2C_GetCommandsList();

  if (OPENBL_RegisterInterface(&I2C_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register FDCAN interfaces */
  FDCAN_Handle.p_Ops = &FDCAN_Ops;
  FDCAN_Handle.p_Cmd = OPENBL_FDCAN_GetCommandsList();

  if (OPENBL_RegisterInterface(&FDCAN_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register SPI interfaces */
  SPI_Handle.p_Ops = &SPI_Ops;
  SPI_Handle.p_Cmd = OPENBL_SPI_GetCommandsList();

  if (OPENBL_RegisterInterface(&SPI_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register USB interfaces */
  USB_Handle.p_Ops = &USB_Ops;
  USB_Handle.p_Cmd = NULL;

  if (OPENBL_RegisterInterface(&USB_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register IWDG interfaces */
  IWDG_Handle.p_Ops = &IWDG_Ops;
  IWDG_Handle.p_Cmd = NULL;

  if (OPENBL_RegisterInterface(&IWDG_Handle) != SUCCESS)
  {
    Error_Handler();
  }

  /* Initialize interfaces */
  OPENBL_Init();

  /* Register Flash memory interface */
  if (OPENBL_MEM_RegisterMemory(&FLASH_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register RAM memory interface */
  if (OPENBL_MEM_RegisterMemory(&RAM_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register Option bytes memory interface */
  if (OPENBL_MEM_RegisterMemory(&OB_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register OTP memory interface */
  if (OPENBL_MEM_RegisterMemory(&OTP_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register ICP memory interface */
  if (OPENBL_MEM_RegisterMemory(&ICP_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }

  /* Register Engi bytes memory interface */
  if (OPENBL_MEM_RegisterMemory(&EB_Descriptor) != SUCCESS)
  {
    Error_Handler();
  }
}

/**
  * @brief  DeInitialize Open Bootloader.
  * @retval None.
  */
void OpenBootloader_DeInit(void)
{
  System_DeInit();
}

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @retval None.
  */
void OpenBootloader_ProtocolDetection(void)
{
  static uint32_t interface_detected = 0U;

  if (interface_detected == 0U)
  {
    interface_detected = OPENBL_InterfaceDetection();

    /* De-initialize the interfaces that are not detected */
    if (interface_detected == 1U)
    {
      OPENBL_InterfacesDeInit();
    }
  }

  if (interface_detected == 1U)
  {
    OPENBL_CommandProcess();
  }
}
