/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_dma.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM DMA defines
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
#ifndef STM32_PLATFORM_LPBAM_DMA_H
#define STM32_PLATFORM_LPBAM_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup DMA_Platform
  * @brief      DMA Platform LPBAM module driver
  * @{
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_DMA_Platform_Exported_Constants LPBAM DMA Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_DMA_Wakeup_Interrupt LPBAM DMA Wakeup Interrupt
  * @{
  */
#define LPBAM_DMA_IT_NONE (0x00U) /*!< LPBAM DMA wake up interrupt none             */
#define LPBAM_DMA_IT_TC   (0x01U) /*!< LPBAM DMA transfer complete interrupt enable */
#define LPBAM_DMA_IT_HT   (0x02U) /*!< LPBAM DMA half transfer interrupt enable     */
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

#endif /* STM32_PLATFORM_LPBAM_DMA_H */
