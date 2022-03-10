/**
  ******************************************************************************
  * @file    board.h
  * @author  MCD Application Team
  * @brief   board header file for stm32l562_dk.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __BOARD_H__
#define __BOARD_H__

/* config for usart */
#define COM_INSTANCE                           LPUART1
#define COM_CLK_ENABLE()                       __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM_CLK_DISABLE()                      __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM_TX_GPIO_PORT                       GPIOG
#define COM_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE();  __HAL_RCC_PWR_CLK_ENABLE(); __HAL_PWR_VDDIO2_ENABLE();
#define COM_TX_PIN                             GPIO_PIN_7
#define COM_TX_AF                              GPIO_AF8_LPUART1

#define COM_RX_GPIO_PORT                       GPIOG
#define COM_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM_RX_PIN                             GPIO_PIN_8
#define COM_RX_AF                              GPIO_AF8_LPUART1
/* config for flash driver */
#define FLASH0_SECTOR_SIZE	0x1000
#define FLASH0_PAGE_SIZE 0x800
#define FLASH0_PROG_UNIT 0x8
#define FLASH0_ERASED_VAL 0xff
#endif /* __BOARD_H__ */
