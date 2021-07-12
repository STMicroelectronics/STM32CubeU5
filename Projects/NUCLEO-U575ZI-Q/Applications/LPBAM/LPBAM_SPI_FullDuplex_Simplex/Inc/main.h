/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_SPI_FullDuplex_Simplex/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"
#include "stm32_lpbam.h"

/* Exported types ------------------------------------------------------------*/
enum
{
  TRANSFER_WAIT,     /* Transfer on going */
  TRANSFER_COMPLETE, /* Transfer complete */
  TRANSFER_ERROR     /* Transfer error    */
};

/* Exported constants --------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
#define MASTER_BOARD

/* Definition for SPIx clock resources */
#define SPIx                             SPI3
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI3_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_LPDMA1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI3_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI3_RELEASE_RESET()

/* Definition the Autonomous Mode utilities */
#define LPDMA_CLKAM_ENABLE()            __HAL_RCC_LPDMA1_CLKAM_ENABLE();
#define SPIx_CLKAM_ENABLE()             __HAL_RCC_SPI3_CLKAM_ENABLE();
#define LPTIMx_CLKAM_ENABLE()           __HAL_RCC_LPTIM1_CLKAM_ENABLE();
#define SRAMx_CLKAM_ENABLE()            __HAL_RCC_SRAM4_CLKAM_ENABLE();

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_10
#define SPIx_SCK_GPIO_PORT               GPIOC
#define SPIx_SCK_AF                      GPIO_AF6_SPI3
#define SPIx_MISO_PIN                    GPIO_PIN_11
#define SPIx_MISO_GPIO_PORT              GPIOC
#define SPIx_MISO_AF                     GPIO_AF6_SPI3
#define SPIx_MOSI_PIN                    GPIO_PIN_12
#define SPIx_MOSI_GPIO_PORT              GPIOC
#define SPIx_MOSI_AF                     GPIO_AF6_SPI3

#define SPIx_IRQn                        SPI3_IRQn
#define SPIx_IRQHandler                  SPI3_IRQHandler

/* Definition for SPIy clock resources */
#define SPIy                             SPI1
#define SPIy_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIy_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIy_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIy_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIy_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIy_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIy Pins */
#define SPIy_SCK_PIN                     GPIO_PIN_5
#define SPIy_SCK_GPIO_PORT               GPIOA
#define SPIy_SCK_AF                      GPIO_AF5_SPI1
#define SPIy_MISO_PIN                    GPIO_PIN_6
#define SPIy_MISO_GPIO_PORT              GPIOA
#define SPIy_MISO_AF                     GPIO_AF5_SPI1
#define SPIy_MOSI_PIN                    GPIO_PIN_7
#define SPIy_MOSI_GPIO_PORT              GPIOA
#define SPIy_MOSI_AF                     GPIO_AF5_SPI1

#define SPIy_IRQn                        SPI1_IRQn
#define SPIy_IRQHandler                  SPI1_IRQHandler


/* Size of Transmission buffer */
#define TXBUFFERSIZE1        (COUNTOF(aTxBuffer1) - 1U)
#define TXBUFFERSIZE2        (COUNTOF(aDummyBuffer)) / 2U

/* Size of Reception buffer */
#define RXBUFFERSIZE1        TXBUFFERSIZE1
#define RXBUFFERSIZE2        TXBUFFERSIZE2

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* MAIN_H */
