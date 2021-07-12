/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_gpio.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_gpio.c file
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------------------------------------------- */
#ifndef STM32_LPBAM_GPIO_H
#define STM32_LPBAM_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_gpio.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup GPIO_Basic
  * @brief      GPIO LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_GPIO_Basic_Exported_Types LPBAM GPIO Basic Exported Types
  * @brief    LPBAM GPIO Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM GPIO Configuration Structure definition.
  */
typedef struct
{
  uint8_t IsSequenceMode; /*!< Specifies whether the GPIO process is single or sequence.
                               This parameter can be one of LPBAM_GPIO_SequenceMode               */

  uint16_t Pin;           /*!< Specifies the GPIO pin.
                               This parameter can be any value of @ref LPBAM_GPIO_Pins            */

  uint16_t PinState;      /*!< Specifies the value to be written to the selected pin(s).
                               This parameter can be one of the GPIO_PinState enumeration values :
                                  @arg LPBAM_GPIO_PIN_RESET : to clear the selected pin
                                  @arg LPBAM_GPIO_PIN_SET   : to set the selected pin             */

  uint16_t Size;          /*!< Specifies the number of pin state.                                 */


} LPBAM_GPIO_Config_t;

/**
  * @brief LPBAM GPIO Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;      /*!< Defines the node descriptor fields  */

  GPIO_TypeDef *pInstance;    /*!< Defines the GPIO instance           */

  LPBAM_GPIO_Config_t Config; /*!< Defines the GPIO user configuration */

} LPBAM_GPIO_ConfNode_t;

/**
  * @brief LPBAM GPIO Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_GPIO_READPIN_ID,  /*!< LPBAM GPIO read pin data transfer node ID  */

  LPBAM_GPIO_WRITEPIN_ID, /*!< LPBAM GPIO write pin data transfer node ID */

} LPBAM_GPIO_ID_t;
/**
  * @}
  */

/* Include LPBAM GPIO Advanced module */
#include "stm32_adv_lpbam_gpio.h"

/** @addtogroup LPBAM_GPIO_Basic_Exported_Functions LPBAM GPIO Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_GPIO_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_GPIO_FillNodeConfig(LPBAM_GPIO_ConfNode_t const *const pConfNode,
                                         DMA_NodeConfTypeDef   *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_GPIO_H */
