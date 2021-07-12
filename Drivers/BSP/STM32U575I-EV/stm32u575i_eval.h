/**
  ******************************************************************************
  * @file    stm32u575i_eval.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32U575I_EVAL:
  *          LEDs
  *          push-buttons
  *          Joystick
  *          COM port
  *          hardware resources.
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
#ifndef STM32U575I_EVAL_H
#define STM32U575I_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_COM_LOG > 0)
#ifndef __GNUC__
#include "stdio.h"
#endif /* __GNUC__ */
#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @addtogroup STM32U575I_EVAL_LOW_LEVEL
  * @{
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Exported_Types LOW LEVEL Exported Types
  * @{
  */
typedef enum
{
  LED5 = 0U,
  LED_GREEN = LED5,
  LED6 = 1U,
  LED_RED = LED6,
#if (USE_BSP_IO_CLASS > 0)
  LED7 = 2U,
  LED_BLUE = LED7,
  LED8 = 3U,
  LED_YELLOW = LED8,
#endif /* USE_BSP_IO_CLASS */
  LEDn
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0U,
  BUTTON_TAMPER = 1U,
  BUTTONn
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0U,
  BUTTON_MODE_EXTI = 1U
} ButtonMode_TypeDef;

#if (USE_BSP_POT_FEATURE > 0)
typedef enum
{
  POT1 = 0
} POT_TypeDef;
#endif /* USE_BSP_POT_FEATURE */

typedef enum
{
  JOY_MODE_GPIO = 0U,
  JOY_MODE_EXTI = 1U
} JOYMode_TypeDef;

#if (USE_BSP_IO_CLASS > 0)
typedef enum
{
  JOY1 = 0U,
  JOYn
} JOY_TypeDef;

typedef enum
{
  JOY_NONE  = 0x00U,
  JOY_SEL   = 0x01U,
  JOY_DOWN  = 0x02U,
  JOY_LEFT  = 0x04U,
  JOY_RIGHT = 0x08U,
  JOY_UP    = 0x10U,
  JOY_ALL   = 0x1FU
} JOYPin_TypeDef;
#endif /* USE_BSP_IO_CLASS */

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COM2 = 1U,
  COMn
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
  COM_STOPBITS_2     =   UART_STOPBITS_2,
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

#endif /* USE_BSP_COM_FEATURE */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */

#if (USE_BSP_POT_FEATURE > 0)
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
typedef struct
{
  pADC_CallbackTypeDef  pMspInitCb;
  pADC_CallbackTypeDef  pMspDeInitCb;
} BSP_POT_Cb_t;
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
#endif /* USE_BSP_POT_FEATURE */

/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Exported_Constants LOW LEVEL Exported Constants
  * @{
  */
/**
  * @brief  Define for STM32U575I_EVAL board
  */
#if !defined (USE_STM32U575I_EVAL)
#define USE_STM32U575I_EVAL
#endif /* USE_STM32U575I_EVAL */

/**
  * @brief STM32U575I_EVAL BSP Driver version number V1.0.0
  */
#define STM32U575I_EVAL_BSP_VERSION_MAIN   (uint32_t)(0x01U) /*!< [31:24] main version */
#define STM32U575I_EVAL_BSP_VERSION_SUB1   (uint32_t)(0x00U) /*!< [23:16] sub1 version */
#define STM32U575I_EVAL_BSP_VERSION_SUB2   (uint32_t)(0x00U) /*!< [15:8]  sub2 version */
#define STM32U575I_EVAL_BSP_VERSION_RC     (uint32_t)(0x00U) /*!< [7:0]  release candidate */
#define STM32U575I_EVAL_BSP_VERSION        ((STM32U575I_EVAL_BSP_VERSION_MAIN << 24)\
                                            |(STM32U575I_EVAL_BSP_VERSION_SUB1 << 16)\
                                            |(STM32U575I_EVAL_BSP_VERSION_SUB2 << 8 )\
                                            |(STM32U575I_EVAL_BSP_VERSION_RC))

#define STM32U575I_EVAL_BSP_BOARD_NAME  "STM32U575I-EV";
#define STM32U575I_EVAL_BSP_BOARD_ID    "MB1550B";

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_LED LOW LEVEL LED
  * @{
  */
#define LED5_GPIO_PORT                   GPIOB
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define LED5_PIN                         GPIO_PIN_7

#define LED6_GPIO_PORT                   GPIOH
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED6_PIN                         GPIO_PIN_7

#if (USE_BSP_IO_CLASS > 0)
#define LED7_PIN                         IO_PIN_11
#define LED8_PIN                         IO_PIN_13
#endif /* USE_BSP_IO_CLASS */
/**
  * @}
  */
/** @defgroup STM32U575I_EVAL_LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
#define BUTTON_RELEASED                    0U
#define BUTTON_PRESSED                     1U

/**
  * @brief Tamper push-button
  */
#define BUTTON_TAMPER_PIN                       GPIO_PIN_1
#define BUTTON_TAMPER_GPIO_PORT                 GPIOA
#define BUTTON_TAMPER_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_TAMPER_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON_TAMPER_EXTI_IRQn                 EXTI1_IRQn
#define BUTTON_TAMPER_EXTI_LINE                 EXTI_LINE_1

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

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_COM LOW LEVEL COM
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

#define COM2_UART                     LPUART1
#define COM2_CLK_ENABLE()             __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM2_CLK_DISABLE()            __HAL_RCC_LPUART1_CLK_DISABLE()

#define COM2_TX_PIN                   GPIO_PIN_7
#define COM2_TX_GPIO_PORT             GPIOG
#define COM2_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM2_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define COM2_TX_AF                    GPIO_AF8_LPUART1

#define COM2_RX_PIN                   GPIO_PIN_8
#define COM2_RX_GPIO_PORT             GPIOG
#define COM2_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM2_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOG_CLK_DISABLE()
#define COM2_RX_AF                    GPIO_AF8_LPUART1

#define COM_POLL_TIMEOUT              1000U
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
  * @}
  */

#if (USE_BSP_POT_FEATURE > 0)
/** @defgroup STM32U575I_EVAL_COMMON_POT STM32U575I_EVAL COMMON POTENTIOMETER
  * @{
  */
#define POTn                                    1U

#define POT1_ADC                                ADC1
#define POT1_ADC_CHANNEL                        ADC_CHANNEL_5
#define POT1_CLK_ENABLE()                       __HAL_RCC_ADC1_CLK_ENABLE()
#define POT1_CLK_DISABLE()                      __HAL_RCC_ADC1_CLK_ENABLE()
#define POT1_CHANNEL_GPIO_PORT                  GPIOA
#define POT1_CHANNEL_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define POT1_CHANNEL_GPIO_PIN                   GPIO_PIN_0
/**
  * @}
  */
#endif /* (USE_BSP_POT_FEATURE > 0) */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_JOYSTICK LOW LEVEL JOYSTICK
  * @{
  */
/* Joystick Pins definition */
#if (USE_BSP_IO_CLASS > 0)
#define JOY_KEY_NUMBER                     5U

#define JOY1_SEL_PIN                  IO_PIN_0
#define JOY1_DOWN_PIN                 IO_PIN_1
#define JOY1_LEFT_PIN                 IO_PIN_2
#define JOY1_RIGHT_PIN                IO_PIN_3
#define JOY1_UP_PIN                   IO_PIN_4
#define JOY1_ALL_PIN                  (JOY1_SEL_PIN | JOY1_DOWN_PIN | JOY1_LEFT_PIN | JOY1_RIGHT_PIN | JOY1_UP_PIN)

#define JOY1_EXTI_IRQn                EXTI5_IRQn
#define JOY1_EXTI_LINE                EXTI_LINE_5
#endif /* (USE_BSP_IO_CLASS > 0) */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_LOW_LEVEL_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
extern USART_TypeDef *COM_UART[];
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
extern ADC_HandleTypeDef hpot_adc[POTn];
#endif /* USE_BSP_POT_FEATURE */
/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_LOW_LEVEL_Exported_Functions
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

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_USART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
/** @defgroup STM32U575I_EVAL_COMMON_POT_Functions STM32U575I_EVAL COMMON POTENTIOMETER Functions
  * @{
  */
int32_t BSP_POT_Init(POT_TypeDef POT);
int32_t BSP_POT_DeInit(POT_TypeDef POT);
int32_t BSP_POT_GetLevel(POT_TypeDef POT);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
int32_t BSP_POT_RegisterDefaultMspCallbacks(POT_TypeDef POT);
int32_t BSP_POT_RegisterMspCallbacks(POT_TypeDef POT, BSP_POT_Cb_t *CallBacks);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

HAL_StatusTypeDef MX_ADC1_Init(ADC_HandleTypeDef *hadc);
#endif /* USE_BSP_POT_FEATURE */

/**
  * @}
  */
#if (USE_BSP_IO_CLASS > 0)
int32_t  BSP_JOY_Init(JOY_TypeDef JOY, JOYMode_TypeDef JoyMode,  JOYPin_TypeDef JoyPins);
int32_t  BSP_JOY_DeInit(JOY_TypeDef JOY,  JOYPin_TypeDef JoyPins);
int32_t  BSP_JOY_GetState(JOY_TypeDef JOY);
void     BSP_JOY_Callback(JOY_TypeDef JOY, uint32_t JoyPin);
void     BSP_JOY_IRQHandler(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin);
#endif /* USE_BSP_IO_CLASS */
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
#endif /* STM32U575I_EVAL_H */
