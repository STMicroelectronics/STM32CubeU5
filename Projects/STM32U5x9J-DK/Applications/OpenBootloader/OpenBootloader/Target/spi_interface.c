/**
  ******************************************************************************
  * @file    spi_interface.c
  * @author  MCD Application Team
  * @brief   Contains SPI HW configuration
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
#include "main.h"
#include "platform.h"
#include "interfaces_conf.h"

#include "openbl_core.h"
#include "openbl_spi_cmd.h"

#include "app_openbootloader.h"
#include "spi_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_DUMMY_BYTE                    0x00U  /* Dummy byte */
#define SPI_SYNC_BYTE                     0x5AU  /* Synchronization byte */
#define SPI_BUSY_BYTE                     0xA5U  /* Busy byte */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint8_t SpiRxNotEmpty = 0U;
static uint8_t SpiDetected        = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_SPI_Init(void);
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void);
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_ClearFlag_OVR(void);
#endif /* (__ICCARM__) */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the SPIx peripheral
  * @retval None.
  */
static void OPENBL_SPI_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0U};

  /* Configure the SPIx IP:
      - Mode               : Slave.
      - Transfer Direction : Full Duplex.
      - Data width         : 8-bit.
      - Bit Order          : MSB first.
      - Polarity           : CPOL Low, CPHA Low.
      - NSS                : Software.
  */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode              = LL_SPI_MODE_SLAVE;
  SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly           = 7U;

  if (LL_SPI_Init(SPIx, &SPI_InitStruct) != SUCCESS)
  {
    Error_Handler();
  }

  LL_SPI_SetFIFOThreshold(SPIx, LL_SPI_FIFO_TH_01DATA);

  /* In case the underrun flag is set, we send a busy byte */
  LL_SPI_SetUDRConfiguration(SPIx, LL_SPI_UDR_CONFIG_REGISTER_PATTERN);
  LL_SPI_SetUDRPattern(SPIx, SPI_BUSY_BYTE);

  /* Enable the SPIx IRQ */
  HAL_NVIC_SetPriority(SPIx_IRQ, 0U, 0U);
  HAL_NVIC_EnableIRQ(SPIx_IRQ);

  /* Enable SPIx */
  LL_SPI_Enable(SPIx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure SPIx pins and then initialize the used SPIx instance.
  * @retval None.
  */
void OPENBL_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0U};

  /* Enable all resources clocks ---------------------------------------------*/
  /* Enable used GPIOx clocks */
  SPIx_GPIO_CLK_SCK_ENABLE();
  SPIx_GPIO_CLK_MISO_ENABLE();
  SPIx_GPIO_CLK_MOSI_ENABLE();
  SPIx_GPIO_CLK_NSS_ENABLE();

  /* Enable SPIx clock */
  SPIx_CLK_ENABLE();

  /* SPIx pins configuration -------------------------------------------------*/
  /*
           +----------+
           |   SPI3   |
     +-----+----------+
     | MOSI|   PG11   |
     +-----+----------+
     | MISO|   PG10   |
     +-----+----------+
     | SCK |   PG9    |
     +-----+----------+
     | NSS |   PG15   |
     +-----+----------+ */

  /* Common configuration for SPIx PINs:
      Mode        : Alternate function.
      Output type : Push-Pull (No pull).
      Speed       : High speed.
  */
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = SPIx_ALTERNATE;

  /* Configure SPIx MOSI PIN */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  HAL_GPIO_Init(SPIx_MOSI_PIN_PORT, &GPIO_InitStruct);

  /* Configure SPIx MISO PIN */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  HAL_GPIO_Init(SPIx_MISO_PIN_PORT, &GPIO_InitStruct);

  /* Configure SPIx SCK PIN */
  GPIO_InitStruct.Pin = SPIx_SCK_PIN;
  HAL_GPIO_Init(SPIx_SCK_PIN_PORT, &GPIO_InitStruct);

  /* Configure SPIx NSS PIN */
  GPIO_InitStruct.Pin = SPIx_NSS_PIN;
  HAL_GPIO_Init(SPIx_NSS_PIN_PORT, &GPIO_InitStruct);

  OPENBL_SPI_Init();
}

/**
  * @brief  This function is used to De-initialize the SPIx PINs and instance.
  * @retval None.
  */
void OPENBL_SPI_DeInit(void)
{
  /* Only de-initialize the SPIx if it is not the current detected interface */
  if (SpiDetected == 0U)
  {
    LL_SPI_Disable(SPIx);

    SPIx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on SPI protocol.
  * @retval Returns 1 if the interface is detected else 0.
  */
uint8_t OPENBL_SPI_ProtocolDetection(void)
{
  /* Check if there is any activity on SPIx */
  if (LL_SPI_IsActiveFlag_RXP(SPIx) != 0U)
  {
    /* Check that Synchronization byte has been received on SPIx */
    if (LL_SPI_ReceiveData8(SPIx) == SPI_SYNC_BYTE)
    {
      SpiDetected = 1U;

      /* Enable the interrupt of Rx not empty buffer */
      LL_SPI_EnableIT_RXP(SPIx);

      /* Send synchronization byte */
      OPENBL_SPI_SendByte(SYNC_BYTE);

      /* Send acknowledgment */
      OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);
    }
    else
    {
      SpiDetected = 0U;
    }
  }
  else
  {
    SpiDetected = 0U;
  }

  return SpiDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_SPI_GetCommandOpcode(void)
{
  uint8_t command_opc;

  /* Check if there is any activity on SPIx */
  while (OPENBL_SPI_ReadByte() != SPI_SYNC_BYTE)
  {}

  /* Get the command opcode */
  command_opc = OPENBL_SPI_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_SPI_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from SPIx pipe.
  *         Read operation is synchronized on SPIx Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__ICCARM__)
__ramfunc uint8_t OPENBL_SPI_ReadByte(void)
#else
__attribute__((section(".ramfunc"))) uint8_t OPENBL_SPI_ReadByte(void)
#endif /* (__ICCARM__) */
{
  uint8_t data;

  /* Wait until SPIx Rx buffer not empty interrupt */
  while (SpiRxNotEmpty == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;
  }

  /* Reset the RX not empty token */
  SpiRxNotEmpty = 0U;

  /* Read the SPIx data register */
  data = (uint8_t) SPIx->RXDR;

  /* Enable the interrupt of Rx not empty buffer */
  SPIx->IER |= SPI_IER_RXPIE;

  return data;
}

/**
  * @brief  This function is used to send one busy byte each receive interrupt through SPIx pipe.
  *         Read operation is synchronized on SPIx Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_SendBusyByte(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_SendBusyByte(void)
#endif /* (__ICCARM__) */
{
  /* Wait until SPIx Rx buffer not empty interrupt */
  while (SpiRxNotEmpty == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;
  }

  /* Reset the RX not empty token */
  SpiRxNotEmpty = 0U;

  /* Transmit the busy byte */
  *((__IO uint8_t *)&SPIx->TXDR) = SPI_BUSY_BYTE;

  /* Read bytes from the host to avoid the overrun */
  OPENBL_SPI_ClearFlag_OVR();

  /* Enable the interrupt of Rx not empty buffer */
  SPIx->IER |= SPI_IER_RXPIE;
}

/**
  * @brief  This function is used to send one byte through SPIx pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_SendByte(uint8_t Byte)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_SendByte(uint8_t Byte)
#endif /* (__ICCARM__) */
{
  /* Wait until SPIx transmit buffer is empty */
  while ((SPIx->SR & SPI_SR_TXP) == 0U)
  {}

  /* Transmit the data */
  *((__IO uint8_t *)&SPIx->TXDR) = Byte;

  /* Clear underrun flag */
  SET_BIT(SPIx->IFCR, SPI_IFCR_UDRC);
}

/**
  * @brief  This function is used to send acknowledge byte through SPIx pipe.
  * @param  Acknowledge The acknowledge byte to be sent.
  * @retval None.
  */
void OPENBL_SPI_SendAcknowledgeByte(uint8_t Acknowledge)
{
  /* Check the AN4286 for the acknowledge procedure */
  if (Acknowledge == ACK_BYTE)
  {
    /* Send dummy byte */
    OPENBL_SPI_SendByte(SPI_DUMMY_BYTE);
  }

  OPENBL_SPI_SendByte(Acknowledge);

  /* Wait for the host to send ACK synchronization byte */
  while (OPENBL_SPI_ReadByte() != ACK_BYTE)
  {}
}

/**
  * @brief  Handle SPIx interrupt request.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_IRQHandler(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_IRQHandler(void)
#endif /* (__ICCARM__) */
{
  uint32_t overrun_flag;
  uint32_t read_packet_avaiable;
  uint32_t read_packet_interrpt_enable;

  /* Read SPIx Status Register flags */
  overrun_flag                = SPIx->SR & SPI_SR_OVR;
  read_packet_avaiable        = SPIx->SR & SPI_SR_RXP;
  read_packet_interrpt_enable = SPIx->IER & SPI_IER_RXPIE;

  /* Check that SPIx Rx buffer not empty interrupt has been raised */
  if ((overrun_flag                   != SPI_SR_OVR)
      && (read_packet_avaiable        == SPI_SR_RXP)
      && (read_packet_interrpt_enable == SPI_IER_RXPIE))
  {
    /* Set the RX not empty token */
    SpiRxNotEmpty = 1U;

    /* Disable the interrupt of Rx not empty buffer */
    SPIx->IER &= ~SPI_IER_RXPIE;
  }

  if ((overrun_flag                   == SPI_SR_OVR)
      && (read_packet_avaiable        == SPI_SR_RXP)
      && (read_packet_interrpt_enable == SPI_IER_RXPIE))
  {
    /* Read bytes from the host to avoid the overrun */
    OPENBL_SPI_ClearFlag_OVR();
  }
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
  /*
    In this product, we use the LL_SPI_UDR_CONFIG_REGISTER_PATTERN configuration,
    where we send a busy byte in case an underrun error occurs.
    This is why we don't need to send the busy byte manually.
  */
}

/**
  * @brief  Clear overrun error flag
  * @note   Clearing this flag is done by a read access to the SPIx_DR
  *         register followed by a read access to the SPIx_SR register
  * @retval None
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_ClearFlag_OVR(void)
#endif /* (__ICCARM__) */
{
  SET_BIT(SPIx->IFCR, SPI_IFCR_OVRC);
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  pSpecialCmd Pointer to the OPENBL_SpecialCmdTypeDef structure.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
void OPENBL_SPI_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *pSpecialCmd)
{
  switch (pSpecialCmd->OpCode)
  {
    case SPECIAL_CMD_DEFAULT:
      if (pSpecialCmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);

        /* Send NULL status size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      else if (pSpecialCmd->CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL status size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      else
      {
        /* Nothing to do */
      }

      break;

    default:
      break;
  }
}
