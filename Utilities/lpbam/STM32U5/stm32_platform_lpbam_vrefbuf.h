/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_vrefbuf.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM VREFBUF defines
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
#ifndef  STM32_PLATFORM_LPBAM_VREFBUF_H
#define  STM32_PLATFORM_LPBAM_VREFBUF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup VREFBUF_Platform
  * @brief      VREFBUF Platform LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_VREFBUF_Platform_Exported_Constants LPBAM VREFBUF Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_VREFBUF_BufferMode LPBAM VREFBUF Buffer Mode
  * @{
  */
#define LPBAM_VREFBUF_BUFFER_OFF_MODE       (0x00U) /*!< LPBAM VREFBUF buffer off mode        */
#define LPBAM_VREFBUF_INTERNAL_VOLTAGE_MODE (0x01U) /*!< LPBAM VREFBUF internal voltage mode  */
#define LPBAM_VREFBUF_EXTERNAL_VOLTAGE_MODE (0x02U) /*!< LPBAM VREFBUF external voltage mode  */
#define LPBAM_VREFBUF_HOLD_MODE             (0x03U) /*!< LPBAM VREFBUF hold mode              */
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

#endif /* STM32_PLATFORM_LPBAM_VREFBUF_H */
