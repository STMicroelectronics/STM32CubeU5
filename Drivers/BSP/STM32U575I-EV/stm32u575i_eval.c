/**
  ******************************************************************************
  * @file    stm32u575i_eval.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage
  *          LEDs
  *          push-buttons (User and Tamper)
  *          Joystick
  *          Potentiometer
  *          COM ports
  *          available on STM32U575I_EVAL board(MB1550) from STMicroelectronics.
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
#include "stm32u575i_eval.h"

#if (USE_BSP_IO_CLASS > 0)
#include "stm32u575i_eval_io.h"
#endif /* USE_BSP_IO_CLASS */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL LOW LEVEL
  * @{
  */

/** @defgroup STM32U575I_EVAL_COMMON_Private_Defines STM32U575I_EVAL COMMON Private Defines
  * @{
  */

#if (USE_BSP_POT_FEATURE > 0)
#define POT_ADC_POLL_TIMEOUT 1000
#endif /* USE_BSP_POT_FEATURE */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_COMMON_Private_Macros STM32U575I_EVAL COMMON Private Macros
  * @{
  */
#if (USE_BSP_POT_FEATURE > 0)
#define POT_CONVERT2PERC(x) (((int32_t)(x) * 100) / 0xFFF)
#endif /* USE_BSP_POT_FEATURE */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Private_TypesDefinitions LOW LEVEL Private Types Definitions
  * @{
  */
typedef void (* BSP_EXTI_LineCallback)(void);
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Private_FunctionPrototypes  LOW LEVEL Private Function Prototypes
  * @{
  */
static void BUTTON_TAMPER_EXTI_Callback(void);
static void BUTTON_USER_EXTI_Callback(void);
#if (USE_BSP_COM_FEATURE > 0)
static void COM_MspInit(UART_HandleTypeDef *huart);
static void COM_MspDeInit(UART_HandleTypeDef *huart);
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
static void ADC_MspInit(ADC_HandleTypeDef *hadc);
static void ADC_MspDeInit(ADC_HandleTypeDef *hadc);
#endif /* USE_BSP_POT_FEATURE */

#if (USE_BSP_IO_CLASS > 0)
static void JOY1_EXTI_Callback(void);
#endif /* USE_BSP_IO_CLASS */

/**
  * @}
  */
/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Variables
  * @{
  */
EXTI_HandleTypeDef hpb_exti[BUTTONn];
#if (USE_BSP_COM_FEATURE > 0)
UART_HandleTypeDef hcom_uart[COMn];
USART_TypeDef *COM_UART[COMn]   = {COM1_UART, COM2_UART};
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
ADC_HandleTypeDef hpot_adc[POTn];
#endif /* USE_BSP_POT_FEATURE */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Private_Variables LOW LEVEL Private Variables
  * @{
  */
static GPIO_TypeDef *LED_PORT[LEDn] = {LED5_GPIO_PORT,
                                       LED6_GPIO_PORT,
#if (USE_BSP_IO_CLASS > 0)
                                       0,
                                       0
#endif /* USE_BSP_IO_CLASS */
                                      };

static const uint32_t LED_PIN[LEDn] = {LED5_PIN,
                                       LED6_PIN,
#if (USE_BSP_IO_CLASS > 0)
                                       LED7_PIN,
                                       LED8_PIN
#endif /* USE_BSP_IO_CLASS */
                                      };

static GPIO_TypeDef *BUTTON_PORT[BUTTONn] = {BUTTON_USER_GPIO_PORT,
                                             BUTTON_TAMPER_GPIO_PORT
                                            };
static const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_USER_PIN,
                                             BUTTON_TAMPER_PIN
                                            };
static const IRQn_Type BUTTON_IRQn[BUTTONn] = {BUTTON_USER_EXTI_IRQn,
                                               BUTTON_TAMPER_EXTI_IRQn
                                              };

#if (USE_COM_LOG > 0)
static COM_TypeDef COM_ActiveLogPort = COM1;
#endif /* USE_COM_LOG */

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsComMspCbValid[COMn] = {0};
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
static ADC_TypeDef      *POT_ADC[POTn] = {POT1_ADC};
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
static uint32_t          IsPotMspCbValid[POTn] = {0U};
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
#endif /* (USE_BSP_POT_FEATURE > 0) */

#if (USE_BSP_IO_CLASS > 0)
static uint8_t           JOY_SEL_PIN[JOYn]     = {JOY1_SEL_PIN};
static uint8_t           JOY_DOWN_PIN[JOYn]    = {JOY1_DOWN_PIN};
static uint8_t           JOY_LEFT_PIN[JOYn]    = {JOY1_LEFT_PIN};
static uint8_t           JOY_RIGHT_PIN[JOYn]   = {JOY1_RIGHT_PIN};
static uint8_t           JOY_UP_PIN[JOYn]      = {JOY1_UP_PIN};
static uint8_t           JOY_ALL_PIN[JOYn]     = {JOY1_ALL_PIN};
static uint8_t           CurrentJoyPins[JOYn]  = {0};
static EXTI_HandleTypeDef hjoy_exti[JOYn];
#endif /* USE_BSP_IO_CLASS */
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Exported_Functions LOW LEVEL Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32U575I_EVAL BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_GetVersion(void)
{
  return (int32_t)STM32U575I_EVAL_BSP_VERSION;
}

/**
  * @brief  This method returns the board name
  * @retval pointer to the board name string
  */
const uint8_t *BSP_GetBoardName(void)
{
  return (uint8_t *)STM32U575I_EVAL_BSP_BOARD_NAME;
}

/**
  * @brief  This method returns the board ID
  * @retval pointer to the board name string
  */
const uint8_t *BSP_GetBoardID(void)
{
  return (uint8_t *)STM32U575I_EVAL_BSP_BOARD_ID;
}

/**
  * @brief  Configures LED on GPIO.
  * @param  Led LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval BSP status
  */
int32_t BSP_LED_Init(Led_TypeDef Led)
{
  int32_t ret = BSP_ERROR_NONE;

#if (USE_BSP_IO_CLASS > 0)
  BSP_IO_Init_t io_init_structure;
#endif /* (USE_BSP_IO_CLASS > 0) */
  GPIO_InitTypeDef  gpio_init_structure;

  if ((Led == LED5) || (Led == LED6))
  {
    /* Enable the GPIO_LED clock */
    if (Led == LED5)
    {
      LED5_GPIO_CLK_ENABLE();
    }
    else
    {
      LED6_GPIO_CLK_ENABLE();
    }

    /* Configure the GPIO_LED pin */
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pin = LED_PIN [Led];
    HAL_GPIO_Init(LED_PORT[Led], &gpio_init_structure);
    HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN[Led], GPIO_PIN_RESET);
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)

    io_init_structure.Pin  = LED_PIN[Led];
    io_init_structure.Pull = IO_PULLDOWN;
    io_init_structure.Mode = IO_MODE_OUTPUT_PP;
    /* Initialize IO expander */
    if (BSP_IO_Init(0, &io_init_structure) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else
    {
      if (BSP_IO_WritePin(0, LED_PIN[Led], IO_PIN_RESET) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_NO_INIT;
      }
    }
    HAL_Delay(1);
#endif /* (USE_BSP_IO_CLASS > 0) */
  }

  return ret;
}

/**
  * @brief  DeInit LEDs.
  * @param  Led LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval BSP status
  */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
  int32_t ret = BSP_ERROR_NONE;
#if (USE_BSP_IO_CLASS > 0)
  BSP_IO_Init_t io_init_structure;
#endif /* (USE_BSP_IO_CLASS > 0) */
  GPIO_InitTypeDef  gpio_init_structure;

  if ((Led == LED5) || (Led == LED6))
  {
    /* Configure the GPIO_LED pin */
    gpio_init_structure.Pin = LED_PIN [Led];
    HAL_GPIO_WritePin(LED_PORT[Led], (uint16_t)LED_PIN[Led], GPIO_PIN_RESET);

    HAL_GPIO_DeInit(LED_PORT[Led], gpio_init_structure.Pin);
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)
    if (BSP_IO_WritePin(0, LED_PIN[Led], IO_PIN_RESET) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else
    {
      io_init_structure.Pin  = LED_PIN[Led];
      io_init_structure.Pull = IO_PULLDOWN;
      io_init_structure.Mode = IO_MODE_OFF;

      if (BSP_IO_Init(0, &io_init_structure) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_NO_INIT;
      }
    }
#endif /* (USE_BSP_IO_CLASS > 0) */
  }
  return ret;
}

/**
  * @brief  Turns selected LED On.
  * @param  Led LED to be set on
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval BSP status
  */
int32_t BSP_LED_On(Led_TypeDef Led)
{
  int32_t ret;
  if ((Led == LED5) || (Led == LED6))
  {
    HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN [Led], GPIO_PIN_SET);
    ret = BSP_ERROR_NONE;
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)
    ret = BSP_IO_WritePin(0, LED_PIN[Led], IO_PIN_SET);
#else
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* (USE_BSP_IO_CLASS > 0) */
  }
  return ret;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval BSP status
  */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
  int32_t ret;
  if ((Led == LED5) || (Led == LED6))
  {
    HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN [Led], GPIO_PIN_RESET);
    ret = BSP_ERROR_NONE;
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)
    ret = BSP_IO_WritePin(0, LED_PIN[Led], IO_PIN_RESET);
#else
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* (USE_BSP_IO_CLASS > 0) */
  }
  return ret;
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval BSP status
  */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
  int32_t ret;
  if ((Led == LED5) || (Led == LED6))
  {
    HAL_GPIO_TogglePin(LED_PORT[Led], (uint16_t)LED_PIN[Led]);
    ret = BSP_ERROR_NONE;
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)
    ret = BSP_IO_TogglePin(0, LED_PIN[Led]);
#else
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* (USE_BSP_IO_CLASS > 0) */
  }
  return ret;
}

/**
  * @brief  Get the selected LED state.
  * @param  Led LED to be get its state
  *          This parameter can be one of the following values:
  *            @arg  LED5
  *            @arg  LED6
  *            @arg  LED7 only if IO expander is used
  *            @arg  LED8 only if IO expander is used
  * @retval LED status
  */
int32_t BSP_LED_GetState(Led_TypeDef Led)
{
  int32_t ret;

  if ((Led == LED5) || (Led == LED6))
  {
    ret = (int32_t)HAL_GPIO_ReadPin(LED_PORT [Led], (uint16_t)LED_PIN [Led]);
  }
  else
  {
#if (USE_BSP_IO_CLASS > 0)
    ret = BSP_IO_ReadPin(0, LED_PIN[Led]);
#else
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* (USE_BSP_IO_CLASS > 0) */
  }

  return ret;
}

/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  *            @arg  BUTTON_TAMPER: Tamper Push Button
  * @param  ButtonMode Button mode
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *            @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line
  *                                    with interrupt generation capability
  * @retval BSP status
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure;
  static BSP_EXTI_LineCallback ButtonCallback[BUTTONn] =
  {
    BUTTON_USER_EXTI_Callback,
    BUTTON_TAMPER_EXTI_Callback
  };
  static uint32_t  BSP_BUTTON_PRIO [BUTTONn] =
  {
    BSP_BUTTON_USER_IT_PRIORITY,
    BSP_BUTTON_TAMPER_IT_PRIORITY,
  };
  static const uint32_t BUTTON_EXTI_LINE[BUTTONn] =
  {
    BUTTON_USER_EXTI_LINE,
    BUTTON_TAMPER_EXTI_LINE,
  };
  /* Enable the BUTTON clock*/
  if (Button == BUTTON_TAMPER)
  {
    BUTTON_TAMPER_GPIO_CLK_ENABLE();
  }
  else
  {
    BUTTON_USER_GPIO_CLK_ENABLE();
  }
  gpio_init_structure.Pin = BUTTON_PIN [Button];
  gpio_init_structure.Pull = GPIO_PULLDOWN;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT [Button], &gpio_init_structure);
  }
  else /* (ButtonMode == BUTTON_MODE_EXTI) */
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;

    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);

    (void)HAL_EXTI_GetHandle(&hpb_exti[Button], BUTTON_EXTI_LINE[Button]);
    (void)HAL_EXTI_RegisterCallback(&hpb_exti[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Push Button DeInit.
  * @param  Button Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  *            @arg  BUTTON_TAMPER: Tamper Push Button
  * @note PB DeInit does not disable the GPIO clock
  * @retval BSP status
  */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Returns the selected button state.
  * @param  Button Button to be checked
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_USER: User Push Button
  *            @arg  BUTTON_TAMPER: Tamper Push Button
  * @retval The Button GPIO pin value
  */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return (int32_t)HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  This function handles Push-Button interrupt requests.
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_IRQHandler(Button_TypeDef Button)
{
  HAL_EXTI_IRQHandler(&hpb_exti[Button]);
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);
  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */
}

#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief  Configures COM port.
  * @param  COM port to be configured.
  *          This parameter can be COM1 or COM2
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified LPUART peripheral.
  * @retval BSP status
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set the COM Instance */
    hcom_uart[COM].Instance = COM_UART[COM];

    /* Init the UART Msp */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    COM_MspInit(&hcom_uart[COM]);
#else
    if (IsComMspCbValid[COM] == 0U)
    {
      if (BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

    if (COM == COM1)
    {
      if (MX_USART1_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    else /* COM = COM2 */
    {
      if (MX_LPUART1_Init(&hcom_uart[COM], COM_Init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  return ret;
}

/**
  * @brief  DeInit COM port.
  * @param  COM COM port to be configured.
  *          This parameter can be (COM1)
  * @retval BSP status
  */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* UART configuration */
    hcom_uart[COM].Instance = COM_UART[COM];

#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    COM_MspDeInit(&hcom_uart[COM]);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */

    if (HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Initialize LPUART1.
  * @param  huart UART handle.
  * @param  MXInit UART initialization structure.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *MXInit)
{
  /* UART configuration */
  huart->Init.BaudRate     = MXInit->BaudRate;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.Parity       = (uint32_t)MXInit->Parity;
  huart->Init.WordLength   = MXInit->WordLength;
  huart->Init.StopBits     = (uint32_t)MXInit->StopBits;
  huart->Init.HwFlowCtl    = (uint32_t)MXInit->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;
  huart->Init.ClockPrescaler = UART_PRESCALER_DIV8;

  return HAL_UART_Init(huart);
}

/**
  * @brief  Configures COM port.
  * @param  huart UART handle
  * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
  *                  configuration information for the specified LPUART peripheral.
  * @retval HAL error code
  */
__weak HAL_StatusTypeDef MX_USART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init)
{
  /* UART configuration */
  huart->Init.BaudRate     = COM_Init->BaudRate;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.Parity       = (uint32_t)COM_Init->Parity;
  huart->Init.WordLength   = (uint32_t)COM_Init->WordLength;
  huart->Init.StopBits     = (uint32_t)COM_Init->StopBits;
  huart->Init.HwFlowCtl    = (uint32_t)COM_Init->HwFlowCtl;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;

  return HAL_UART_Init(huart);
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default COM Msp Callbacks
  * @param  COM COM port to be configured.
  * @retval BSP status
  */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register default MspInit/MspDeInit Callback */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID,) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, UART1_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}

/**
  * @brief  Register COM Msp Callback registering
  * @param  COM port to be configured.
  * @param  Callbacks pointer to COM MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  if (COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);

    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsComMspCbValid[COM] = 1U;
    }
  }

  /* BSP status */
  return ret;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG > 0)
/**
  * @brief  Select the active COM port.
  * @param  COM port to be activated.
  * @retval BSP status
  */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{
  if (COM_ActiveLogPort != COM)
  {
    COM_ActiveLogPort = COM;
  }
  return BSP_ERROR_NONE;
}

#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  (void)HAL_UART_Transmit(&hcom_uart [COM_ActiveLogPort], (uint8_t *) &ch, 1, COM_POLL_TIMEOUT);
  return ch;
}
#endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
/** @addtogroup STM32U575I_EVAL_COMMON_POT_Functions
  * @{
  */

/**
  * @brief  Configure Potentiometer.
  * @param  POT Potentiometer to be configured.
  *          This parameter can be POT1
  * @retval BSP error code
  */
int32_t BSP_POT_Init(POT_TypeDef POT)
{
  int32_t status = BSP_ERROR_NONE;

  /* Initialize POT instance */
  hpot_adc[POT].Instance = POT_ADC[POT];

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
  ADC_MspInit(&hpot_adc[POT]);
#else
  if (IsPotMspCbValid[POT] == 0U)
  {
    /* Register the MSP callbacks */
    if (BSP_POT_RegisterDefaultMspCallbacks(POT) != BSP_ERROR_NONE)
    {
      status = BSP_ERROR_MSP_FAILURE;
    }
  }
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

  if (status == BSP_ERROR_NONE)
  {
    if (MX_ADC1_Init(&hpot_adc[POT]) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return status;
}

/**
  * @brief  DeInitialize Potentiometer.
  * @param  POT Potentiometer to be de_init.
  *          This parameter can be POT1
  * @retval BSP error code
  */
int32_t BSP_POT_DeInit(POT_TypeDef POT)
{
  int32_t status = BSP_ERROR_NONE;

  /* POT de-init */
  hpot_adc[POT].Instance = POT_ADC[POT];
  if (HAL_ADC_DeInit(&hpot_adc[POT]) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 0)
  else
  {
    ADC_MspDeInit(&hpot_adc[POT]);
  }
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

  return status;
}

/**
  * @brief  Get Potentiometer level.
  * @param  POT Potentiometer.
  *          This parameter can be POT1
  * @retval Potentiometer level(0..100%), negative value means error.
  */
int32_t BSP_POT_GetLevel(POT_TypeDef POT)
{
  int32_t  retval = BSP_ERROR_PERIPH_FAILURE;
  uint32_t AdcValue;

  if (HAL_ADC_Start(&hpot_adc[POT]) == HAL_OK)
  {
    if (HAL_ADC_PollForConversion(&hpot_adc[POT], POT_ADC_POLL_TIMEOUT) == HAL_OK)
    {
      AdcValue = HAL_ADC_GetValue(&hpot_adc[POT]);
      retval = POT_CONVERT2PERC(AdcValue);
    }
  }

  return retval;
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register default POT msp callbacks.
  * @param  POT Potentiometer.
  *          This parameter can be POT1
  * @retval BSP status.
  */
int32_t BSP_POT_RegisterDefaultMspCallbacks(POT_TypeDef POT)
{
  int32_t status = BSP_ERROR_NONE;

  /* Register MspInit/MspDeInit callbacks */
  if (HAL_ADC_RegisterCallback(&hpot_adc[POT], HAL_ADC_MSPINIT_CB_ID, ADC_MspInit) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else if (HAL_ADC_RegisterCallback(&hpot_adc[POT], HAL_ADC_MSPDEINIT_CB_ID, ADC_MspDeInit) != HAL_OK)
  {
    status = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsPotMspCbValid[POT] = 1U;
  }
  /* Return BSP status */
  return status;
}

/**
  * @brief  Register BSP POT msp callbacks.
  * @param  POT Potentiometer.
  *          This parameter can be POT1
  * @param  CallBacks Pointer to MspInit/MspDeInit callback functions.
  * @retval BSP status
  */
int32_t BSP_POT_RegisterMspCallbacks(POT_TypeDef POT, BSP_POT_Cb_t *CallBacks)
{
  int32_t status = BSP_ERROR_NONE;

  if (CallBacks == NULL)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit callbacks */
    if (HAL_ADC_RegisterCallback(&hpot_adc[POT], HAL_ADC_MSPINIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_ADC_RegisterCallback(&hpot_adc[POT], HAL_ADC_MSPDEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      status = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsPotMspCbValid[POT] = 1U;
    }
  }
  /* Return BSP status */
  return status;
}
#endif /* (USE_HAL_ADC_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Initialize ADC1.
  * @param  hadc ADC handle.
  * @retval HAL status.
  */
__weak HAL_StatusTypeDef MX_ADC1_Init(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef        status = HAL_ERROR;
  ADC_ChannelConfTypeDef   CHANNEL_Config;
  uint32_t                 POT_ADC_CHANNEL[POTn] = {POT1_ADC_CHANNEL};

  /* ADC configuration */
  hadc->Instance                   = POT1_ADC;
  hadc->Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV16;
  hadc->Init.Resolution            = ADC_RESOLUTION_12B;
  hadc->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc->Init.ScanConvMode          = ADC_SCAN_DISABLE;
  hadc->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc->Init.LowPowerAutoWait      = DISABLE;
  hadc->Init.ContinuousConvMode    = DISABLE;
  hadc->Init.NbrOfConversion       = 1;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.NbrOfDiscConversion   = 1;
  hadc->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hadc->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = DISABLE;
  hadc->Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  hadc->Init.OversamplingMode      = DISABLE;
  if (HAL_ADC_Init(hadc) == HAL_OK)
  {
    /* ADC channel configuration */
    CHANNEL_Config.Channel      = POT_ADC_CHANNEL[POT1];
    CHANNEL_Config.Rank         = ADC_REGULAR_RANK_1;
    CHANNEL_Config.SamplingTime = ADC_SAMPLETIME_36CYCLES;
    CHANNEL_Config.SingleDiff   = ADC_SINGLE_ENDED;
    CHANNEL_Config.OffsetNumber = ADC_OFFSET_NONE;
    CHANNEL_Config.Offset       = 0;
    if (HAL_ADC_ConfigChannel(hadc, &CHANNEL_Config) == HAL_OK)
    {
      if (HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) == HAL_OK)
      {
        status = HAL_OK;
      }
    }
  }

  return status;
}

/**
  * @}
  */
#endif /* (USE_BSP_POT_FEATURE > 0) */

#if (USE_BSP_IO_CLASS > 0)
/**
  * @brief  Configures joystick GPIO and EXTI modes.
  * @param  JOY Joystick to be initialized
  * @param  JoyMode Button mode.
  *          This parameter can be one of the following values:
  *            @arg  JOY_MODE_GPIO: Joystick pins will be used as simple IOs
  *            @arg  JOY_MODE_EXTI: Joystick pins will be connected to EXTI line
  *                                 with interrupt generation capability
  * @param  JoyPins joystick pins to be initialized
  * @retval BSP status
  */
int32_t BSP_JOY_Init(JOY_TypeDef JOY, JOYMode_TypeDef JoyMode, JOYPin_TypeDef JoyPins)
{
  int32_t               status = BSP_ERROR_NONE;
  BSP_IO_Init_t         IO_Init;

  /* Initialize the BSP IO driver and configure the joystick pin */
  switch (JoyPins)
  {
    case JOY_SEL:
      IO_Init.Pin = JOY_SEL_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_SEL_PIN[JOY];
      break;
    case JOY_DOWN:
      IO_Init.Pin = JOY_DOWN_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_DOWN_PIN[JOY];
      break;
    case JOY_LEFT:
      IO_Init.Pin = JOY_LEFT_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_LEFT_PIN[JOY];
      break;
    case JOY_RIGHT:
      IO_Init.Pin = JOY_RIGHT_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_RIGHT_PIN[JOY];
      break;
    case JOY_UP:
      IO_Init.Pin = JOY_UP_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_UP_PIN[JOY];
      break;
    case JOY_ALL:
      IO_Init.Pin = JOY_ALL_PIN[JOY];
      CurrentJoyPins[JOY] |= JOY_ALL_PIN[JOY];
      break;
    default :
      status = BSP_ERROR_WRONG_PARAM;
      break;
  }

  if (status == BSP_ERROR_NONE)
  {
    IO_Init.Mode = (JoyMode == JOY_MODE_GPIO) ? IO_MODE_INPUT : IO_MODE_IT_FALLING_EDGE;
    IO_Init.Pull = IO_PULLUP;
    status = BSP_IO_Init(0, &IO_Init);
    if ((status == BSP_ERROR_NONE) && (JoyMode == JOY_MODE_EXTI))
    {
      /* Interrupt already enabled on BSP_IO_Init, just register callback */
      if (HAL_EXTI_GetHandle(&hjoy_exti[JOY], JOY1_EXTI_LINE) == HAL_OK)
      {
        if (HAL_EXTI_RegisterCallback(&hjoy_exti[JOY], HAL_EXTI_RISING_CB_ID, JOY1_EXTI_Callback) != HAL_OK)
        {
          status = BSP_ERROR_PERIPH_FAILURE;
        }
      }
      else
      {
        status = BSP_ERROR_PERIPH_FAILURE;
      }
    }

  }

  return status;
}

/**
  * @brief  DeInit joystick GPIOs.
  * @param  JOY instance
  * @note   JOY DeInit does not disable the MFX, just set the MFX pins in Off mode
  * @param  JoyPins joystick pins to be de-initialized
  * @retval BSP status
  */
int32_t BSP_JOY_DeInit(JOY_TypeDef JOY, JOYPin_TypeDef JoyPins)
{
  int32_t       status = BSP_ERROR_NONE;
  BSP_IO_Init_t IO_Init;

  switch (JoyPins)
  {
    case JOY_SEL:
      IO_Init.Pin = JOY_SEL_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_SEL_PIN[JOY]);
      break;
    case JOY_DOWN:
      IO_Init.Pin = JOY_DOWN_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_DOWN_PIN[JOY]);
      break;
    case JOY_LEFT:
      IO_Init.Pin = JOY_LEFT_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_LEFT_PIN[JOY]);
      break;
    case JOY_RIGHT:
      IO_Init.Pin = JOY_RIGHT_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_RIGHT_PIN[JOY]);
      break;
    case JOY_UP:
      IO_Init.Pin = JOY_UP_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_UP_PIN[JOY]);
      break;
    case JOY_ALL:
      IO_Init.Pin = JOY_ALL_PIN[JOY];
      CurrentJoyPins[JOY] &= ~(JOY_ALL_PIN[JOY]);
      break;
    default :
      status = BSP_ERROR_WRONG_PARAM;
      break;
  }
  if (status == BSP_ERROR_NONE)
  {
    IO_Init.Mode = IO_MODE_OFF;
    IO_Init.Pull = IO_NOPULL;
    status = BSP_IO_Init(0, &IO_Init);
  }

  return status;
}

/**
  * @brief  This function handles JOY keys interrupt request.
  * @param  JOY Joystick instance
  * @param  JoyPin Specifies the pin connected EXTI handler
  * @retval None
  */
void BSP_JOY_IRQHandler(JOY_TypeDef JOY, JOYPin_TypeDef JoyPin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(JoyPin);

  HAL_EXTI_IRQHandler(&hjoy_exti[JOY]);
}

/**
  * @brief  BSP Joystick Callback.
  * @param  JOY Joystick instance
  * @param  JoyPin Specifies the pin connected EXTI line
  * @retval None.
  */
__weak void BSP_JOY_Callback(JOY_TypeDef JOY, uint32_t JoyPin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(JOY);
  UNUSED(JoyPin);

  /* This function should be implemented by the user application.
     It is called into this driver when an event on JoyPin is triggered. */
}

/**
  * @brief  Returns the current joystick status.
  * @param  JOY Joystick instance
  * @retval BSP error code if value is negative or one of the following values:
  *            @arg  JOY_NONE
  *            @arg  JOY_SEL
  *            @arg  JOY_DOWN
  *            @arg  JOY_LEFT
  *            @arg  JOY_RIGHT
  *            @arg  JOY_UP
  */
int32_t BSP_JOY_GetState(JOY_TypeDef JOY)
{
  int32_t retval = (int32_t)JOY_NONE;
  int32_t status;

  if ((CurrentJoyPins[JOY] & JOY_SEL_PIN[JOY]) == JOY_SEL_PIN[JOY])
  {
    /* Read status of JOY_SEL */
    status = BSP_IO_ReadPin(0, JOY_SEL_PIN[JOY]);
    if (status >= 0)
    {
      retval = (status == (int32_t)IO_PIN_RESET) ? (int32_t)JOY_SEL : (int32_t)JOY_NONE;
    }
    else
    {
      retval = status;
    }
  }
  if (((CurrentJoyPins[JOY] & JOY_UP_PIN[JOY]) == JOY_UP_PIN[JOY]) && (retval == (int32_t)JOY_NONE))
  {
    /* Read status of JOY_UP */
    status = BSP_IO_ReadPin(0, JOY_UP_PIN[JOY]);
    if (status >= 0)
    {
      retval = (status == (int32_t)IO_PIN_RESET) ? (int32_t)JOY_UP : (int32_t)JOY_NONE;
    }
    else
    {
      retval = status;
    }
  }
  if (((CurrentJoyPins[JOY] & JOY_DOWN_PIN[JOY]) == JOY_DOWN_PIN[JOY]) && (retval == (int32_t)JOY_NONE))
  {
    /* Read status of JOY_DOWN */
    status = BSP_IO_ReadPin(0, JOY_DOWN_PIN[JOY]);
    if (status >= 0)
    {
      retval = (status == (int32_t)IO_PIN_RESET) ? (int32_t)JOY_DOWN : (int32_t)JOY_NONE;
    }
    else
    {
      retval = status;
    }
  }
  if (((CurrentJoyPins[JOY] & JOY_LEFT_PIN[JOY]) == JOY_LEFT_PIN[JOY]) && (retval == (int32_t)JOY_NONE))
  {
    /* Read status of JOY_LEFT */
    status = BSP_IO_ReadPin(0, JOY_LEFT_PIN[JOY]);
    if (status >= 0)
    {
      retval = (status == (int32_t)IO_PIN_RESET) ? (int32_t)JOY_LEFT : (int32_t)JOY_NONE;
    }
    else
    {
      retval = status;
    }
  }
  if (((CurrentJoyPins[JOY] & JOY_RIGHT_PIN[JOY]) == JOY_RIGHT_PIN[JOY]) && (retval == (int32_t)JOY_NONE))
  {
    /* Read status of JOY_RIGHT */
    status = BSP_IO_ReadPin(0, JOY_RIGHT_PIN[JOY]);
    if (status >= 0)
    {
      retval = (status == (int32_t)IO_PIN_RESET) ? (int32_t)JOY_RIGHT : (int32_t)JOY_NONE;
    }
    else
    {
      retval = status;
    }
  }

  return retval;
}
/**
  * @}
  */

/** @defgroup STM32U575I_EVAL_LOW_LEVEL_Private_Functions LOW LEVEL Private Functions
  * @{
  */
/**
  * @brief  Joystick1 EXTI line detection callbacks.
  * @retval None
  */
static void JOY1_EXTI_Callback(void)
{
  int32_t ItStatus;

  if ((CurrentJoyPins[JOY1] & JOY_SEL_PIN[JOY1]) == JOY_SEL_PIN[JOY1])
  {
    /* Read IT status of JOY_SEL */
    ItStatus = BSP_IO_GetIT(0, JOY_SEL_PIN[JOY1]);
    if (ItStatus > 0)
    {
      BSP_JOY_Callback(JOY1, (uint32_t)JOY_SEL);
      /* Clear IT status of JOY_SEL */
      ItStatus = BSP_IO_ClearIT(0, JOY_SEL_PIN[JOY1]);
      if (ItStatus < 0)
      {
        /* Nothing to do */
      }
    }
  }
  if ((CurrentJoyPins[JOY1] & JOY_UP_PIN[JOY1]) == JOY_UP_PIN[JOY1])
  {
    /* Read IT status of JOY_UP */
    ItStatus = BSP_IO_GetIT(0, JOY_UP_PIN[JOY1]);
    if (ItStatus > 0)
    {
      BSP_JOY_Callback(JOY1, (uint32_t)JOY_UP);
      /* Clear IT status of JOY_UP */
      ItStatus = BSP_IO_ClearIT(0, JOY_UP_PIN[JOY1]);
      if (ItStatus < 0)
      {
        /* Nothing to do */
      }
    }
  }
  if ((CurrentJoyPins[JOY1] & JOY_DOWN_PIN[JOY1]) == JOY_DOWN_PIN[JOY1])
  {
    /* Read IT status of JOY_DOWN */
    ItStatus = BSP_IO_GetIT(0, JOY_DOWN_PIN[JOY1]);
    if (ItStatus > 0)
    {
      BSP_JOY_Callback(JOY1, (uint32_t)JOY_DOWN);
      /* Clear IT status of JOY_DOWN */
      ItStatus = BSP_IO_ClearIT(0, JOY_DOWN_PIN[JOY1]);
      if (ItStatus < 0)
      {
        /* Nothing to do */
      }
    }
  }
  if ((CurrentJoyPins[JOY1] & JOY_LEFT_PIN[JOY1]) == JOY_LEFT_PIN[JOY1])
  {
    /* Read IT status of JOY_LEFT */
    ItStatus = BSP_IO_GetIT(0, JOY_LEFT_PIN[JOY1]);
    if (ItStatus > 0)
    {
      BSP_JOY_Callback(JOY1, (uint32_t)JOY_LEFT);
      /* Clear IT status of JOY_LEFT */
      ItStatus = BSP_IO_ClearIT(0, JOY_LEFT_PIN[JOY1]);
      if (ItStatus < 0)
      {
        /* Nothing to do */
      }
    }
  }
  if ((CurrentJoyPins[JOY1] & JOY_RIGHT_PIN[JOY1]) == JOY_RIGHT_PIN[JOY1])
  {
    /* Read IT status of JOY_RIGHT */
    ItStatus = BSP_IO_GetIT(0, JOY_RIGHT_PIN[JOY1]);
    if (ItStatus > 0)
    {
      BSP_JOY_Callback(JOY1, (uint32_t)JOY_RIGHT);
      /* Clear IT status of JOY_RIGHT */
      ItStatus = BSP_IO_ClearIT(0, JOY_RIGHT_PIN[JOY1]);
      if (ItStatus < 0)
      {
        /* Nothing to do */
      }
    }
  }
}
#endif /* USE_BSP_IO_CLASS */

/**
  * @brief  TAMPER EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_TAMPER_EXTI_Callback(void)
{
  BSP_PB_Callback(BUTTON_TAMPER);
}

/**
  * @brief  KEY EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_USER_EXTI_Callback(void)
{
  BSP_PB_Callback(BUTTON_USER);
}

#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief  Initializes UART MSP.
  * @param  huart UART handle
  * @retval None
  */
static void COM_MspInit(UART_HandleTypeDef *huart)
{
  COM_TypeDef      COM;
  GPIO_InitTypeDef gpio_init_structure;

  /* Get COM according instance */
  COM = (huart->Instance == COM1_UART) ? COM1 : COM2;

  if (COM == COM1)
  {
    /* Enable GPIO clock */
    COM1_TX_GPIO_CLK_ENABLE();
    COM1_RX_GPIO_CLK_ENABLE();

    /* Enable UART clock */
    COM1_CLK_ENABLE();

    /* Configure UART Tx as alternate function */
    gpio_init_structure.Pin       = COM1_TX_PIN;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    gpio_init_structure.Alternate = COM1_TX_AF;
    HAL_GPIO_Init(COM1_TX_GPIO_PORT, &gpio_init_structure);

    /* Configure UART Rx as alternate function */
    gpio_init_structure.Pin       = COM1_RX_PIN;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Alternate = COM1_RX_AF;
    HAL_GPIO_Init(COM1_RX_GPIO_PORT, &gpio_init_structure);
  }
  else /* COM = COM2 */
  {
    /* Enable VddIO2 for COM2 */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();
    __HAL_RCC_PWR_CLK_DISABLE();

    /* Enable GPIO clock */
    COM2_TX_GPIO_CLK_ENABLE();
    COM2_RX_GPIO_CLK_ENABLE();

    /* Enable UART clock */
    COM2_CLK_ENABLE();
    __HAL_RCC_LPUART1_FORCE_RESET() ;
    __HAL_RCC_LPUART1_RELEASE_RESET();

    /* Configure UART Tx as alternate function */
    gpio_init_structure.Pin       = COM2_TX_PIN;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    gpio_init_structure.Alternate = COM2_TX_AF;
    HAL_GPIO_Init(COM2_TX_GPIO_PORT, &gpio_init_structure);

    /* Configure UART Rx as alternate function */
    gpio_init_structure.Pin       = COM2_RX_PIN;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Alternate = COM2_RX_AF;
    HAL_GPIO_Init(COM2_RX_GPIO_PORT, &gpio_init_structure);
  }
}

/**
  * @brief  Initialize UART Msp part
  * @param  huart UART handle
  * @retval None
  */
static void COM_MspDeInit(UART_HandleTypeDef *huart)
{
  COM_TypeDef      COM;
  GPIO_InitTypeDef          gpio_init_structure;

  /* Get COM according instance */
  COM = (huart->Instance == COM1_UART) ? COM1 : COM2;

  if (COM == COM1)
  {
    /* COM GPIO pin configuration */
    gpio_init_structure.Pin  = COM1_TX_PIN;
    HAL_GPIO_DeInit(COM1_TX_GPIO_PORT, gpio_init_structure.Pin);

    gpio_init_structure.Pin  = COM1_RX_PIN;
    HAL_GPIO_DeInit(COM1_RX_GPIO_PORT, gpio_init_structure.Pin);

    /* Disable UART clock */
    COM1_CLK_DISABLE();
  }
  else /* COM = COM2 */
  {
    /* COM GPIO pin configuration */
    gpio_init_structure.Pin  = COM2_TX_PIN;
    HAL_GPIO_DeInit(COM2_TX_GPIO_PORT, gpio_init_structure.Pin);

    gpio_init_structure.Pin  = COM2_RX_PIN;
    HAL_GPIO_DeInit(COM2_RX_GPIO_PORT, gpio_init_structure.Pin);

    /* Disable UART clock */
    COM2_CLK_DISABLE();
  }
}
#endif /* USE_BSP_COM_FEATURE */

#if (USE_BSP_POT_FEATURE > 0)
/**
  * @brief  Initialize ADC MSP.
  * @param  hadc ADC handle.
  * @retval None.
  */
static void ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef  GPIO_Init;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* enable VDDA supply */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddA();

  /* Enable ADC and GPIO clocks */
  POT1_CLK_ENABLE();
  POT1_CHANNEL_GPIO_CLK_ENABLE();

  /* Configure ADC channel pin */
  GPIO_Init.Pin   = POT1_CHANNEL_GPIO_PIN;
  GPIO_Init.Mode  = GPIO_MODE_ANALOG;
  GPIO_Init.Pull  = GPIO_NOPULL;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(POT1_CHANNEL_GPIO_PORT, &GPIO_Init);
}

/**
  * @brief  Deinitialize ADC MSP.
  * @param  hadc ADC handle.
  * @retval None.
  */
static void ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* Disable GPIO and POT clock */
  HAL_GPIO_DeInit(POT1_CHANNEL_GPIO_PORT, POT1_CHANNEL_GPIO_PIN);
  POT1_CLK_DISABLE();
}
#endif /* (USE_BSP_POT_FEATURE > 0) */

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

/**
  * @}
  */
