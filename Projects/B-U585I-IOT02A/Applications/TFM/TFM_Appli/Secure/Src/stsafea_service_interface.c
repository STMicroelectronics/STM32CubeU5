/**
  ******************************************************************************
  * @file    stsafe_service_interface.c
  * @author  SMD application team
  * @brief   Service Interface file to support the hardware services required by the
  *          STSAFE-A Middleware and offered by the specific HW, Low Level library, or BSP
  *          selected by the user. E.g.:
  *           + IOs
  *           + Communication Bus (e.g. I2C)
  *           + Timing delay
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * STSAFE DRIVER SOFTWARE LICENSE AGREEMENT (SLA0088)
  *
  * BY INSTALLING, COPYING, DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE
  * OR ANY PART THEREOF (AND THE RELATED DOCUMENTATION) FROM STMICROELECTRONICS
  * INTERNATIONAL N.V, SWISS BRANCH AND/OR ITS AFFILIATED COMPANIES (STMICROELECTRONICS),
  * THE RECIPIENT, ON BEHALF OF HIMSELF OR HERSELF, OR ON BEHALF OF ANY ENTITY BY WHICH
  * SUCH RECIPIENT IS EMPLOYED AND/OR ENGAGED AGREES TO BE BOUND BY THIS SOFTWARE LICENSE
  * AGREEMENT.
  *
  * Under STMicroelectronics� intellectual property rights, the redistribution,
  * reproduction and use in source and binary forms of the software or any part thereof,
  * with or without modification, are permitted provided that the following conditions
  * are met:
  * 1.  Redistribution of source code (modified or not) must retain any copyright notice,
  *     this list of conditions and the disclaimer set forth below as items 10 and 11.
  * 2.  Redistributions in binary form, except as embedded into a microcontroller or
  *     microprocessor device or a software update for such device, must reproduce any
  *     copyright notice provided with the binary code, this list of conditions, and the
  *     disclaimer set forth below as items 10 and 11, in documentation and/or other
  *     materials provided with the distribution.
  * 3.  Neither the name of STMicroelectronics nor the names of other contributors to this
  *     software may be used to endorse or promote products derived from this software or
  *     part thereof without specific written permission.
  * 4.  This software or any part thereof, including modifications and/or derivative works
  *     of this software, must be used and execute solely and exclusively in combination
  *     with a secure microcontroller device from STSAFE family manufactured by or for
  *     STMicroelectronics.
  * 5.  No use, reproduction or redistribution of this software partially or totally may be
  *     done in any manner that would subject this software to any Open Source Terms.
  *     �Open Source Terms� shall mean any open source license which requires as part of
  *     distribution of software that the source code of such software is distributed
  *     therewith or otherwise made available, or open source license that substantially
  *     complies with the Open Source definition specified at www.opensource.org and any
  *     other comparable open source license such as for example GNU General Public
  *     License(GPL), Eclipse Public License (EPL), Apache Software License, BSD license
  *     or MIT license.
  * 6.  STMicroelectronics has no obligation to provide any maintenance, support or
  *     updates for the software.
  * 7.  The software is and will remain the exclusive property of STMicroelectronics and
  *     its licensors. The recipient will not take any action that jeopardizes
  *     STMicroelectronics and its licensors' proprietary rights or acquire any rights
  *     in the software, except the limited rights specified hereunder.
  * 8.  The recipient shall comply with all applicable laws and regulations affecting the
  *     use of the software or any part thereof including any applicable export control
  *     law or regulation.
  * 9.  Redistribution and use of this software or any part thereof other than as  permitted
  *     under this license is void and will automatically terminate your rights under this
  *     license.
  * 10. THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" AND ANY
  *     EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  *     OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, WHICH ARE DISCLAIMED TO THE FULLEST
  *     EXTENT PERMITTED BY LAW. IN NO EVENT SHALL STMICROELECTRONICS OR CONTRIBUTORS BE
  *     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  *     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  *     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  *     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  *     ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * 11. EXCEPT AS EXPRESSLY PERMITTED HEREUNDER, NO LICENSE OR OTHER RIGHTS, WHETHER EXPRESS
  *     OR IMPLIED, ARE GRANTED UNDER ANY PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF
  *     STMICROELECTRONICS OR ANY THIRD PARTY.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stsafea_service.h"
#include "stsafea_interface_conf.h"
#include MCU_PLATFORM_INCLUDE

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define STSAFEA_VREG_PIN                          GPIO_PIN_11
#define STSAFEA_VREG_GPIO_PORT                    GPIOF
#define STSAFEA_VREG_GPIO_PORT_CLK_ENABLE         __HAL_RCC_GPIOF_CLK_ENABLE
#define STSAFEA_DEVICE_ADDRESS                    0x0020

#define I2C_ANALOG_FILTER_DELAY_DEFAULT        2U      /* ns */

#define BUS_I2C2_INSTANCE                       I2C2

#define BUS_I2C2_CLK_ENABLE()                   __HAL_RCC_I2C2_CLK_ENABLE()
#define BUS_I2C2_CLK_DISABLE()                  __HAL_RCC_I2C2_CLK_DISABLE()

#define BUS_I2C2_SDA_GPIO_PIN                   GPIO_PIN_5
#define BUS_I2C2_SCL_GPIO_PIN                   GPIO_PIN_4

#define BUS_I2C2_SDA_GPIO_PORT                  GPIOH
#define BUS_I2C2_SCL_GPIO_PORT                  GPIOH

#define BUS_I2C2_SDA_GPIO_AF                    GPIO_AF4_I2C2
#define BUS_I2C2_SCL_GPIO_AF                    GPIO_AF4_I2C2

#define BUS_I2C2_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C2_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()

#define BUS_I2C2_POLL_TIMEOUT                0x1000U

#define BUS_I2C2_TIMING                      0x00F07BFF /* Corresponding to frequency of I2C1 = 400 KHz*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef  *hbus_i2c = &hi2c2;

/* Private function prototypes -----------------------------------------------*/
int32_t HW_IO_Init(void);
void    BSP_TimeDelay(uint32_t msDelay);
HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef *phi2c, uint32_t timing);
int32_t BSP_I2C2_Init(void);
int32_t BSP_I2C2_DeInit(void);
int32_t BSP_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t CRC16X25_Init(void);
uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2);
#if 1
static void I2C2_MspInit(I2C_HandleTypeDef *hI2c);
static void I2C2_MspDeInit(I2C_HandleTypeDef *hI2c);
#endif
/* Private functions ---------------------------------------------------------*/

#if (STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
#define STSAFEA_CRC16_X25_REFLECTED_LOOKUP_TABLE \
  0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7, \
  0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876, \
  0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5, \
  0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974, \
  0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB, 0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3, \
  0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72, \
  0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1, \
  0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70, \
  0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF, \
  0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E, \
  0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD, \
  0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C, \
  0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB, \
  0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232, 0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A, \
  0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9, \
  0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

static uint16_t StSafeA_Crc16_ccitt(uint8_t Header, uint8_t *pData, uint16_t Length);
int32_t CRC16X25_Init(void);
uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2);
/**
  * @brief   CRC16X25_Init
  *          Initializes CRC X25.
  * @retval  BSP status
  */
int32_t CRC16X25_Init(void)
{
  return STSAFEA_BUS_OK;
}

/**
  * @brief   CRC_Compute
  *          Computes the CRC using software solution.
  *          CRC is computed firsly using 1st data starting with initialization value.
  *          CRC is computed secondly using 2nd data starting with the previously computed CRC.
  *
  * @param   pData1  : Pointer to 1st input data buffer.
  * @param   Length1 : Size of 1st input data buffer.
  * @param   pData2  : Pointer to 2nd input data buffer.
  * @param   Length2 : Size of 2nd input data buffer.
  * @retval  uint32_t CRC (returned value LSBs for CRC)
  */
uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2)
{
  (void)Length1;
  uint16_t crc16 = 0;
  if ((pData1 != NULL) && (pData2 != NULL))
  {
    crc16 = StSafeA_Crc16_ccitt(pData1[0], pData2, Length2);

    crc16 = (uint16_t)SWAP2BYTES(crc16);
    crc16 ^= 0xFFFFU;
  }
  return (uint32_t)crc16;
}


/**
  * @brief   StSafeA_Crc16_ccitt
  *          Computes the CRC16 CCITT over the data passed (header & data pointer).
  *
  * @param   Header : Payload's Header.
  * @param   pData  : Payload's data.
  * @param   Length : Length of payload's data.
  * @retval  uint16_t containing the calculated CRC
  */
static uint16_t StSafeA_Crc16_ccitt(uint8_t Header, uint8_t *pData, uint16_t Length)
{
#if (!STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
  static uint16_t crc16_reflected_lookup_table[256] = {0};
#else
  static const uint16_t crc16_reflected_lookup_table[256] = {STSAFEA_CRC16_X25_REFLECTED_LOOKUP_TABLE};
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

  uint16_t crc = 0xFFFF;
  if (pData != NULL)
  {
    uint8_t ndx;
    uint16_t i;

#if (!STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
    /* Instead of reflecting the data coming in, and the CRC coming in, and
       the CRC going out, simply reflect the polynomial and the operations.
       Only need to do that oncethrough the code.
       The reflected polynomial is 0x8408. */
    if (crc16_reflected_lookup_table[1] == 0x0000U)
    {
      uint16_t  remainder;
      for (int dividend = 0; dividend < 256; ++dividend)
      {
        remainder = dividend;
        for (uint8_t bit = 8; bit > 0; --bit)
        {
          if (remainder & 1)
          {
            remainder = (remainder >> 1) ^ 0x8408;
          }
          else
          {
            remainder = (remainder >> 1);
          }
        }
        crc16_reflected_lookup_table[dividend] = remainder;
      }
    }
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

    ndx = (uint8_t)(0xFFU & (crc ^ Header));
    crc = (crc16_reflected_lookup_table[ndx] ^ 0xFFU);

    for (i = 0; i < Length; i++)
    {
      ndx = (uint8_t)(0xFFU & (crc ^ pData[i]));
      crc = (crc16_reflected_lookup_table[ndx] ^ (crc >> 8));
    }
  }

  return crc;
}


/**
  * @brief  Configure STSAFE IO and Bus operation functions to be implemented at User level
  * @param  Ctx the STSAFE IO context
  * @retval 0 in case of success, an error code otherwise
  */
STSAFEA_HW_t *CurProbe = NULL;
int8_t StSafeA_HW_Probe(void *pCtx)
{
  STSAFEA_HW_t *HwCtx = (STSAFEA_HW_t *)pCtx;
  CurProbe = HwCtx;


  HwCtx->IOInit     = HW_IO_Init;
  HwCtx->BusInit    = BSP_I2C2_Init;
  HwCtx->BusDeInit  = BSP_I2C2_DeInit;
  HwCtx->BusSend    = BSP_I2C2_Send;
  HwCtx->BusRecv    = BSP_I2C2_Recv;
  HwCtx->CrcInit    = CRC16X25_Init;
  HwCtx->CrcCompute = CRC_Compute;
  HwCtx->TimeDelay  = HAL_Delay;
  HwCtx->DevAddr    = STSAFEA_DEVICE_ADDRESS;

  return STSAFEA_BUS_OK;
}


/**
  * @brief  Additional IO pins configuration needed for STSAFE (VREG pin, etc.)
  * @param  none
  * @retval 0 in case of success, an error code otherwise
  */
int32_t HW_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  STSAFEA_VREG_GPIO_PORT_CLK_ENABLE();

  /* Configure GPIO pin : RST Pin */
  GPIO_InitStruct.Pin = STSAFEA_VREG_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STSAFEA_VREG_GPIO_PORT, &GPIO_InitStruct);

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STSAFEA_VREG_GPIO_PORT, STSAFEA_VREG_PIN, GPIO_PIN_SET);

  // STSAFE takes up to 50 ms to get ready, but it might take less.
  // The first command will use the retry mechanism to cop with this delay
  // HAL_Delay(50);

  return STSAFEA_BUS_OK;
}


/**
  * @brief  This function provides a delay (in milliseconds)
  * @param  none
  * @retval 0 in case of success, an error code otherwise
  */
void BSP_TimeDelay(uint32_t msDelay)
{
  /* Could be redirected to a Task Delay or to a different custom implementation */
  HAL_Delay(msDelay);
}

/**
  * @brief  MX I2C2 Inititialization.
  * @param  phi2c : I2C handle.
  * @param  timing : I2C timings as described in the I2C peripheral V2 and V3.
  * @retval Prescaler divider
  */
__weak HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef *phi2c, uint32_t timing)
{
  HAL_StatusTypeDef ret = HAL_ERROR;

  phi2c->Init.Timing           = timing;
  phi2c->Init.OwnAddress1      = 0;
  phi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  phi2c->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  phi2c->Init.OwnAddress2      = 0;
  phi2c->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  phi2c->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(phi2c) == HAL_OK)
  {
    if (HAL_I2CEx_ConfigAnalogFilter(phi2c, I2C_ANALOGFILTER_DISABLE) == HAL_OK)
    {
      if (HAL_I2CEx_ConfigDigitalFilter(phi2c, I2C_ANALOG_FILTER_DELAY_DEFAULT) == HAL_OK)
      {
        ret = HAL_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  Initializes I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C2_Init(void)
{
  int32_t ret = STSAFEA_BUS_ERR;
#if 1
  hbus_i2c->Instance  = I2C2;

  if (HAL_I2C_GetState(hbus_i2c) == HAL_I2C_STATE_RESET)
  {
    /* Init the I2C Msp */
    I2C2_MspInit(hbus_i2c);

    /* Init the I2C */
    if (MX_I2C2_Init(hbus_i2c, BUS_I2C2_TIMING) == HAL_OK)
    {
      if (HAL_I2CEx_ConfigAnalogFilter(hbus_i2c, I2C_ANALOGFILTER_ENABLE) == HAL_OK)
      {
        ret = STSAFEA_BUS_OK;
      }
    }
  }
  else
#endif
  {
    ret = STSAFEA_BUS_OK;
  }

  return ret;
}

/**
  * @brief  DeInitializes I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C2_DeInit(void)
{
  int32_t ret  = STSAFEA_BUS_ERR;
#if 1
  /* DeInit the I2C */
  I2C2_MspDeInit(hbus_i2c);

  /* DeInit the I2C */
  if (HAL_I2C_DeInit(hbus_i2c) == HAL_OK)
#endif
  {
    ret = STSAFEA_BUS_OK;
  }

  return ret;
}


/**
  * @brief  Send data through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t BSP_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = STSAFEA_BUS_ERR;
  uint32_t hal_error;

  if (HAL_I2C_Master_Transmit(hbus_i2c,
                              DevAddr,
                              pData,
                              Length,
                              BUS_I2C2_POLL_TIMEOUT) == HAL_OK)
  {
    ret = STSAFEA_BUS_OK;
  }
  else
  {
    hal_error = HAL_I2C_GetError(hbus_i2c);
    if (hal_error == HAL_I2C_ERROR_AF)
    {
      return STSAFEA_BUS_NACK;
    }
    else
    {
      ret =  STSAFEA_BUS_ERR;
    }
  }
  return ret;
}

/**
  * @brief  Send data through BUS
  * @param  DevAddr Device address on Bus.
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  BSP_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = STSAFEA_BUS_ERR;
  uint32_t hal_error;

  if (HAL_I2C_Master_Receive(hbus_i2c,
                             DevAddr,
                             pData,
                             Length,
                             BUS_I2C2_POLL_TIMEOUT) == HAL_OK)
  {
    ret = STSAFEA_BUS_OK;
  }
  else
  {
    hal_error = HAL_I2C_GetError(hbus_i2c);
    if (hal_error == HAL_I2C_ERROR_AF)
    {
      return STSAFEA_BUS_NACK;
    }
    else
    {
      ret =  STSAFEA_BUS_ERR;
    }
  }

  return ret;
}

int8_t StSafeA_Set_I2C_Address(uint32_t DevAddr)
{
  if (CurProbe != NULL)
  {
    CurProbe->DevAddr = DevAddr;
  }
  return STSAFEA_BUS_OK;
}

#if 1
/**
  * @brief  Initializes I2C MSP.
  * @param  hI2c  I2C handler
  * @retval None
  */
static void I2C2_MspInit(I2C_HandleTypeDef *hI2c)
{
  GPIO_InitTypeDef  gpio_init;

  /* Enable I2C clock */
  BUS_I2C2_CLK_ENABLE();

  /* Enable GPIO clock */
  BUS_I2C2_SDA_GPIO_CLK_ENABLE();
  BUS_I2C2_SCL_GPIO_CLK_ENABLE();

  /* Configure I2C SDA Line */
  gpio_init.Pin = BUS_I2C2_SDA_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_I2C2_SDA_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C2_SDA_GPIO_PORT, &gpio_init);

  /* Configure I2C SCL Line */
  gpio_init.Pin = BUS_I2C2_SCL_GPIO_PIN;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_I2C2_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C2_SCL_GPIO_PORT, &gpio_init);

}

/**
  * @brief  DeInitializes I2C MSP.
  * @param  hI2c  I2C handler
  * @retval None
  */
static void I2C2_MspDeInit(I2C_HandleTypeDef *hI2c)
{
  /* Disable I2C clock */
  __HAL_RCC_I2C2_CLK_DISABLE();

  /* DeInitialize peripheral GPIOs */
  HAL_GPIO_DeInit(BUS_I2C2_SDA_GPIO_PORT, BUS_I2C2_SDA_GPIO_PIN);
  HAL_GPIO_DeInit(BUS_I2C2_SCL_GPIO_PORT, BUS_I2C2_SCL_GPIO_PIN);

}


#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
