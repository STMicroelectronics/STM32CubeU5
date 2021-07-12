/**
  ******************************************************************************
  * @file    spi_interface.c
  * @author  MCD Application Team
  * @brief   Contains SPI HW configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx.h"
#include "stm32u5xx_ll_spi.h"

#include "openbl_core.h"
#include "openbl_spi_cmd.h"

#include "spi_interface.h"
#include "iwdg_interface.h"

#include "interfaces_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_DUMMY_BYTE                    0x00U  /* Dummy byte */
#define SPI_SYNC_BYTE                     0x5AU  /* Synchronization byte */
#define SPI_BUSY_BYTE                     0xA5U  /* Busy byte */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint8_t SpiRxNotEmpty = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_SPI_Init(void);
#if defined (__CC_ARM)
void OPENBL_SPI_ClearFlag_OVR(void);
#else
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void);
#endif /* (__CC_ARM) */

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function is used to initialize the SPI peripheral
 * @retval None.
 */
static void OPENBL_SPI_Init(void)
{
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure SPI pins and then initialize the used SPI instance.
 * @retval None.
 */
void OPENBL_SPI_Configuration(void)
{
}

/**
 * @brief  This function is used to detect if there is any activity on SPI protocol.
 * @retval None.
 */
uint8_t OPENBL_SPI_ProtocolDetection(void)
{
  uint8_t detected;

  return detected;
}

/**
 * @brief  This function is used to get the command opcode from the host.
 * @retval Returns the command.
 */
uint8_t OPENBL_SPI_GetCommandOpcode(void)
{
  uint8_t command_opc;

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__CC_ARM)
uint8_t OPENBL_SPI_ReadByte(void)
#else
__ramfunc uint8_t OPENBL_SPI_ReadByte(void)
#endif /* (__CC_ARM) */
{
  uint8_t data;

  return data;
}

/**
  * @brief  This function is used to send one busy byte each receive interrupt through SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__CC_ARM)
void OPENBL_SPI_SendBusyByte(void)
#else
__ramfunc void OPENBL_SPI_SendBusyByte(void)
#endif /* (__CC_ARM) */
{
}

/**
  * @brief  This function is used to send one byte through SPI pipe.
  * @retval None.
  */
#if defined (__CC_ARM)
void OPENBL_SPI_SendByte(uint8_t Byte)
#else
__ramfunc void OPENBL_SPI_SendByte(uint8_t Byte)
#endif /* (__CC_ARM) */
{
}

/**
  * @brief  This function is used to send acknowledge byte through SPI pipe.
  * @retval None.
  */
void OPENBL_SPI_SendAcknowledgeByte(uint8_t Byte)
{
}

/**
  * @brief  Handle SPI interrupt request.
  * @retval None.
  */
#if defined (__CC_ARM)
void OPENBL_SPI_IRQHandler()
#else
__ramfunc void OPENBL_SPI_IRQHandler()
#endif /* (__CC_ARM) */
{
}

/**
  * @brief  This function enables the send of busy state.
  * @retval None.
  */
void OPENBL_SPI_EnableBusyState(void)
{
  /* Since we are using the underrun configuration, we don't need to enable the busy state */
}

/**
  * @brief  This function disables the send of busy state.
  * @retval None.
  */
void OPENBL_SPI_DisableBusyState(void)
{
}

/**
  * @brief  Clear overrun error flag
  * @note   Clearing this flag is done by a read access to the SPIx_DR
  *         register followed by a read access to the SPIx_SR register
  * @retval None
  */
#if defined (__CC_ARM)
void OPENBL_SPI_ClearFlag_OVR(void)
#else
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void)
#endif /* (__CC_ARM) */
{
}
