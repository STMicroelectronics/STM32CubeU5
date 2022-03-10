/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_gpio.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_gpio.c file
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32_ADV_LPBAM_GPIO_H
#define STM32_ADV_LPBAM_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_gpio.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup GPIO_Advanced GPIO_Advanced
  * @brief    GPIO Advanced LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_GPIO_Advanced_Exported_Types LPBAM GPIO Advanced Exported Types
  * @brief    LPBAM GPIO Advanced Exported Types
  * @{
  */

/**
  * @brief LPBAM GPIO write pin advanced configuration structure definition.
  */
typedef struct
{
  uint16_t Pin;      /*!< Specifies the GPIO pins to be configured.
                          This parameter can be one or a combination of @ref LPBAM_GPIO_Pins        */

  uint16_t PinState; /*!< Specifies the value to be written to the selected pin(s).
                          This parameter can be one of the LPBAM_GPIO_PinState enumeration values :
                             @arg LPBAM_GPIO_PIN_RESET : to clear the selected pin
                             @arg LPBAM_GPIO_PIN_SET   : to set the selected pin                    */

} LPBAM_GPIO_WritePinFullAdvConf_t;

/**
  * @brief  LPBAM read/write pin sequence advanced configuration structure definition.
  */
typedef struct
{
  uint16_t Pin;    /*!< Specifies the GPIO pins to be configured.
                        This parameter can be a value or a combination of @ref LPBAM_GPIO_Pins */

  uint32_t *pData; /*!< Specifies the data buffer address                                      */

  uint16_t Size;   /*!< Specifies the amount of data to be read/written                        */

} LPBAM_GPIO_PinSeqFullAdvConf_t;

/**
  * @brief  LPBAM write pin descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_GPIO_WritePinFullDesc_t;

/**
  * @brief  LPBAM write pin sequence descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

} LPBAM_GPIO_WritePinSeqFullDesc_t;

/**
  * @brief  LPBAM read pin sequence descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

} LPBAM_GPIO_ReadPinSeqFullDesc_t;
/**
  * @}
  */

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_GPIO_Advanced_Exported_Macros LPBAM GPIO Advanced Exported Macros
  * @brief    LPBAM GPIO Basic Exported Macros
  * @{
  */
/** @brief  Check GPIO state.
  * @param  __GPIO_STATE__ : Specifies the GPIO state to be configured.
  * @param  __GPIO_PIN__ : Specifies the GPIO pin to be configured.
  * @retval The State (SET or RESET).
  */
#define __LPBAM_GPIO_STATE(__GPIO_PIN__, __GPIO_STATE__)  \
  (((__GPIO_STATE__) == LPBAM_GPIO_PIN_SET) ? ((__GPIO_PIN__) << 16U) : (__GPIO_PIN__))
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_GPIO_Advanced_Exported_Functions LPBAM GPIO Advanced Exported Functions
  * @{
  */

/**
  * @brief  ADV_LPBAM_GPIO_WritePin_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_WritePin_SetFullQ(GPIO_TypeDef                     *const pInstance,
                                                LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                LPBAM_GPIO_WritePinFullAdvConf_t const *const pWritePinFull,
                                                LPBAM_GPIO_WritePinFullDesc_t    *const pDescriptor,
                                                DMA_QListTypeDef                 *const pQueue);
/**
  * @brief  ADV_LPBAM_GPIO_WritePinSequence_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(GPIO_TypeDef                     *const pInstance,
                                                        LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                        LPBAM_GPIO_PinSeqFullAdvConf_t   const *const pWritePinSeqFull,
                                                        LPBAM_GPIO_WritePinSeqFullDesc_t *const pDescriptor,
                                                        DMA_QListTypeDef                 *const pQueue);
/**
  * @brief  ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ(GPIO_TypeDef                    *const pInstance,
                                                       LPBAM_DMAListInfo_t             const *const pDMAListInfo,
                                                       LPBAM_GPIO_PinSeqFullAdvConf_t  const *const pReadPinSeqFull,
                                                       LPBAM_GPIO_ReadPinSeqFullDesc_t *const pDescriptor,
                                                       DMA_QListTypeDef                *const pQueue);
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

#endif /* STM32_ADV_LPBAM_GPIO_H */
