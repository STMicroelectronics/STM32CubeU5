/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
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
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_ll_usart.h"
#include "stm32u5xx_ll_bus.h"
#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_i2c.h"
#include "stm32u5xx_ll_spi.h"

#define MEMORIES_SUPPORTED                7U

/*-------------------------- Definitions for USART ---------------------------*/
#define USARTx                            USART1
#define USARTx_CLK_ENABLE()               __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_CLK_DISABLE()              __HAL_RCC_USART1_CLK_DISABLE()
#define USARTx_GPIO_CLK_TX_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_GPIO_CLK_RX_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_DEINIT()                   LL_USART_DeInit(USARTx)

#define USARTx_TX_PIN                     GPIO_PIN_9
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_RX_PIN                     GPIO_PIN_10
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_ALTERNATE                  GPIO_AF7_USART1

/*-------------------------- Definitions for I2C -----------------------------*/
#define I2Cx                              I2C1
#define I2Cx_CLK_ENABLE()                 __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_CLK_DISABLE()                __HAL_RCC_I2C1_CLK_DISABLE()
#define I2Cx_GPIO_CLK_SCL_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define I2Cx_GPIO_CLK_SDA_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define I2Cx_DEINIT()                     LL_I2C_DeInit(I2Cx)

#define I2Cx_SCL_PIN                      GPIO_PIN_14
#define I2Cx_SCL_PIN_PORT                 GPIOG
#define I2Cx_SDA_PIN                      GPIO_PIN_13
#define I2Cx_SDA_PIN_PORT                 GPIOG
#define I2Cx_ALTERNATE                    GPIO_AF4_I2C1

#define I2C_ADDRESS                       (0x0000005AU << 0x01U)
#define OPENBL_I2C_TIMEOUT                0xFFFFF000U
#define I2C_TIMING                        0x00800000U

/*-------------------------- Definitions for FDCAN ---------------------------*/
#define FDCANx                            FDCAN1
#define FDCANx_CLK_ENABLE()               __HAL_RCC_FDCAN1_CLK_ENABLE()
#define FDCANx_CLK_DISABLE()              __HAL_RCC_FDCAN1_CLK_DISABLE()
#define FDCANx_GPIO_CLK_RX_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define FDCANx_GPIO_CLK_TX_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()

#define FDCANx_FORCE_RESET()              __HAL_RCC_FDCAN1_FORCE_RESET()
#define FDCANx_RELEASE_RESET()            __HAL_RCC_FDCAN1_RELEASE_RESET()

#define FDCANx_TX_PIN                     GPIO_PIN_1
#define FDCANx_TX_GPIO_PORT               GPIOD
#define FDCANx_TX_AF                      GPIO_AF9_FDCAN1
#define FDCANx_RX_PIN                     GPIO_PIN_0
#define FDCANx_RX_GPIO_PORT               GPIOD
#define FDCANx_RX_AF                      GPIO_AF9_FDCAN1

/*--------------------------- Definitions for SPI ----------------------------*/
#define SPIx                              SPI3
#define SPIx_CLK_ENABLE()                 __HAL_RCC_SPI3_CLK_ENABLE()
#define SPIx_CLK_DISABLE()                __HAL_RCC_SPI3_CLK_DISABLE()
#define SPIx_GPIO_CLK_MOSI_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define SPIx_GPIO_CLK_MISO_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define SPIx_GPIO_CLK_SCK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define SPIx_GPIO_CLK_NSS_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define SPIx_DEINIT()                     LL_SPI_DeInit(SPIx)
#define SPIx_IRQ                          SPI3_IRQn

#define SPIx_MOSI_PIN                     GPIO_PIN_11
#define SPIx_MOSI_PIN_PORT                GPIOG
#define SPIx_MISO_PIN                     GPIO_PIN_10
#define SPIx_MISO_PIN_PORT                GPIOG
#define SPIx_SCK_PIN                      GPIO_PIN_9
#define SPIx_SCK_PIN_PORT                 GPIOG
#define SPIx_NSS_PIN                      GPIO_PIN_15
#define SPIx_NSS_PIN_PORT                 GPIOG
#define SPIx_ALTERNATE                    GPIO_AF6_SPI3

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERFACES_CONF_H */
