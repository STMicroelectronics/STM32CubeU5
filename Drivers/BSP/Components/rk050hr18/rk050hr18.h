/**
  ******************************************************************************
  * @file    rk050hr18.h
  * @author  MCD Application Team
  * @brief   This file contains all the constants parameters for the RK050HR18-CTG
  *          LCD component.
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
#ifndef __RK050HR18_H
#define __RK050HR18_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup RK050HR18
  * @{
  */

/** @defgroup RK050HR18_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup RK050HR18_Exported_Constants
  * @{
  */

/**
  * @brief  RK050HR18 Size
  */
#define  RK050HR18_WIDTH    ((uint16_t)800)          /* LCD PIXEL WIDTH            */
#define  RK050HR18_HEIGHT   ((uint16_t)480)          /* LCD PIXEL HEIGHT           */

/**
  * @brief  RK050HR18 Timing
  */
#define  RK050HR18_HSYNC            ((uint16_t)4)   /* Horizontal synchronization */
#define  RK050HR18_HBP              ((uint16_t)8)   /* Horizontal back porch      */
#define  RK050HR18_HFP              ((uint16_t)8)   /* Horizontal front porch     */
#define  RK050HR18_VSYNC            ((uint16_t)4)   /* Vertical synchronization   */
#define  RK050HR18_VBP              ((uint16_t)8)   /* Vertical back porch        */
#define  RK050HR18_VFP              ((uint16_t)8)   /* Vertical front porch       */

/**
  * @}
  */

/** @defgroup RK050HR18_Exported_Functions
  * @{
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

#ifdef __cplusplus
}
#endif

#endif /* __RK050HR18_H */
