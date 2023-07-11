/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for stm32u5x9j_discovery.c:
  *          LEDs
  *          USER push-button
  *          COM port
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
#ifndef STM32U5x9J_DISCOVERY_H
#define STM32U5x9J_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_conf.h"
#include "stm32u5x9j_discovery_errno.h"

#include "stm32u5x9j_discovery_bus.h"

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_COM_LOG > 0)
#ifndef __GNUC__
#include "stdio.h"
#endif /* !__GNUC__ */
#endif /* USE_COM_LOG > 0 */
#endif /* USE_BSP_COM_FEATURE > 0 */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY_LOW_LEVEL
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_Exported_Types LOW LEVEL Exported Types
  * @{
  */
typedef enum
{
  LED3 = 0U,
  LED_GREEN = LED3,
  LED4 = 1U,
  LED_RED = LED4,
  LEDn
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0U,
  BUTTONn
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0U,
  BUTTON_MODE_EXTI = 1U
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COMn
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE     =  UART_PARITY_NONE,
  COM_PARITY_EVEN     =  UART_PARITY_EVEN,
  COM_PARITY_ODD      =  UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#define MX_UART_InitTypeDef COM_InitTypeDef

#endif /* (USE_BSP_COM_FEATURE > 0) */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_Exported_Constants LOW LEVEL Exported Constants
  * @{
  */
/**
  * @brief  Define for STM32U5x9J_DISCOVERY board
  */
#if !defined (USE_STM32U5x9J_DISCOVERY)
#define USE_STM32U5x9J_DISCOVERY
#endif /* !USE_STM32U5x9J_DISCOVERY */

/**
  * @brief STM32U5x9J_DISCOVERY BSP Driver version number V1.1.0
  */
#define STM32U5x9J_DISCOVERY_BSP_VERSION_MAIN   (uint32_t)(0x01) /*!< [31:24] main version */
#define STM32U5x9J_DISCOVERY_BSP_VERSION_SUB1   (uint32_t)(0x01) /*!< [23:16] sub1 version */
#define STM32U5x9J_DISCOVERY_BSP_VERSION_SUB2   (uint32_t)(0x00) /*!< [15:8]  sub2 version */
#define STM32U5x9J_DISCOVERY_BSP_VERSION_RC     (uint32_t)(0x00) /*!< [7:0]  release candidate */
#define STM32U5x9J_DISCOVERY_BSP_VERSION        ((STM32U5x9J_DISCOVERY_BSP_VERSION_MAIN << 24)\
                                                 |(STM32U5x9J_DISCOVERY_BSP_VERSION_SUB1 << 16)\
                                                 |(STM32U5x9J_DISCOVERY_BSP_VERSION_SUB2 << 8 )\
                                                 |(STM32U5x9J_DISCOVERY_BSP_VERSION_RC))

#define STM32U5x9J_DISCOVERY_BSP_BOARD_NAME  "STM32U5x9J-DK";
#define STM32U5x9J_DISCOVERY_BSP_BOARD_ID    "MB1829";

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_LED EVAL LOW LEVEL LED
  * @{
  */
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
#define LED3_PIN                         GPIO_PIN_0

#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
#define LED4_PIN                         GPIO_PIN_1

/**
  * @}
  */
/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
#define BUTTON_RELEASED                    0U
#define BUTTON_PRESSED                     1U

/**
  * @brief User push-button
  */
#define BUTTON_USER_PIN                       GPIO_PIN_13
#define BUTTON_USER_GPIO_PORT                 GPIOC
#define BUTTON_USER_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQn                 EXTI13_IRQn
#define BUTTON_USER_EXTI_LINE                 EXTI_LINE_13
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_COM LOW LEVEL COM
  * @{
  */
#ifndef USE_BSP_COM_FEATURE
#define USE_BSP_COM_FEATURE    0U
#endif /* USE_BSP_COM_FEATURE */
#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief Definition for COM port1, connected to USART1
  */
#define COM1_UART                     USART1
#define COM1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()

#define COM1_TX_PIN                   GPIO_PIN_9
#define COM1_TX_GPIO_PORT             GPIOA
#define COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_TX_AF                    GPIO_AF7_USART1

#define COM1_RX_PIN                   GPIO_PIN_10
#define COM1_RX_GPIO_PORT             GPIOA
#define COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_RX_AF                    GPIO_AF7_USART1

#define COM_POLL_TIMEOUT              1000U
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[BUTTONn];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[COMn];
extern USART_TypeDef *COM_UART[COMn];
#endif /* USE_BSP_COM_FEATURE */
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_LOW_LEVEL_Exported_Functions LOW LEVEL Exported Functions
  * @{
  */
int32_t  BSP_GetVersion(void);
const uint8_t *BSP_GetBoardName(void);
const uint8_t *BSP_GetBoardID(void);

int32_t  BSP_LED_Init(Led_TypeDef Led);
int32_t  BSP_LED_DeInit(Led_TypeDef Led);
int32_t  BSP_LED_On(Led_TypeDef Led);
int32_t  BSP_LED_Off(Led_TypeDef Led);
int32_t  BSP_LED_Toggle(Led_TypeDef Led);
int32_t  BSP_LED_GetState(Led_TypeDef Led);

int32_t  BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t  BSP_PB_DeInit(Button_TypeDef Button);
int32_t  BSP_PB_GetState(Button_TypeDef Button);
void     BSP_PB_Callback(Button_TypeDef Button);
void     BSP_PB_IRQHandler(Button_TypeDef Button);

#if (USE_BSP_COM_FEATURE > 0)
int32_t  BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t  BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif /* USE_COM_LOG */

#if (USE_HAL_UART_REGISTER_CALLBACKS > 0)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_USART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
#endif /* USE_BSP_COM_FEATURE */

/* DSI TS INT pin */
#define TS_INT_PIN                       GPIO_PIN_8
#define TS_INT_GPIO_PORT                 GPIOE
#define TS_INT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define TS_INT_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define TS_INT_EXTI_IRQn                 EXTI8_IRQn

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

#ifdef __cplusplus
}
#endif
#endif /* STM32U5x9J_DISCOVERY_H */
