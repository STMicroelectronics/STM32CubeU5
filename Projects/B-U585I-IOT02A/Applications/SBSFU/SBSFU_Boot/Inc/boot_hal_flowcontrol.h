/**
  ******************************************************************************
  * @file    boot_hal_flowcontrol.h
  * @author  MCD Application Team
  * @brief   Header for flow control in boot_hal.c module
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
#ifndef BOOT_HAL_FLOWCONTROL_H
#define BOOT_HAL_FLOWCONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "region_defs.h"
#include "boot_hal_cfg.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
  * @brief  Flow Control : Initial value
  */
#define FLOW_STAGE_CFG              0xfffffe00U
#define FLOW_STAGE_CHK              0xffffe01eU

/**
  * @brief  Flow Control : Initial value
  */
#define FLOW_CTRL_INIT_VALUE        0x00005776U        /*!< Init value definition */

/**
  * @brief  Flow Control : Steps definition
  */
#ifdef TFM_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_S_I_EN_R0     0x00006787U        /*!< Step MPU Secure Region 0 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R1     0x00007999U        /*!< Step MPU Secure Region 1 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R2     0x0000aab5U        /*!< Step MPU Secure Region 2 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R3     0x0000b4abU        /*!< Step MPU Secure Region 3 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R4     0x0000cccdU        /*!< Step MPU Secure Region 4 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R5     0x0000d2d3U        /*!< Step MPU Secure Region 5 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R6     0x00043d7aU        /*!< Step MPU Secure Region 6 Init enable value */
#define FLOW_STEP_MPU_S_I_EN        0x0000e3dcU        /*!< Step MPU Secure Init enable value */
#define FLOW_STEP_MPU_NS_I_EN_R0    0x0000fdc2U        /*!< Step MPU NonSecure Region 0 Init enable value */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_MPU_NS_I_EN_R1    0x00012cd3U        /*!< Step MPU NonSecure Region 1 Init enable value */
#else
#define FLOW_STEP_MPU_NS_I_EN_R1    0x00000000U        /*!< No effect on control flow */
#endif /* !MCUBOOT_PRIMARY_ONLY */
#define FLOW_STEP_MPU_NS_I_EN_R2    0x000132cdU        /*!< Step MPU NonSecure Region 2 Init enable value */
#define FLOW_STEP_MPU_NS_I_EN_R3    0x00014aabU        /*!< Step MPU NonSecure Region 3 Init enable value */
#define FLOW_STEP_MPU_NS_I_EN       0x000154b5U        /*!< Step MPU NonSecure Init enable value */
#else /* TFM_BOOT_MPU_PROTECTION */
#define FLOW_STEP_MPU_S_I_EN_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R1     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R3     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R4     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R5     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R6     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN        0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_EN_R0    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_EN_R1    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_EN_R2    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_EN_R3    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_EN       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_BOOT_MPU_PROTECTION */

#ifdef TFM_ERROR_HANDLER_NON_SECURE
#define FLOW_STEP_SAU_I_EN_R0       0x000165baU        /*!< Step SAU Region 0 Init enable value */
#else
#define FLOW_STEP_SAU_I_EN_R0       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_ERROR_HANDLER_NON_SECURE */
#define FLOW_STEP_SAU_I_EN_R1       0x00017ba4U        /*!< Step SAU Region 1 Init enable value */
#define FLOW_STEP_SAU_I_EN_R2       0x00018799U        /*!< Step SAU Region 2 Init enable value */
#if defined(MCUBOOT_EXT_LOADER)
#define FLOW_STEP_SAU_I_EN_R3       0x00019987U        /*!< Step SAU Region 3 Init enable value */
#else
#define FLOW_STEP_SAU_I_EN_R3       0x00000000U        /*!< No effect on control flow */
#endif /* MCUBOOT_EXT_LOADER */
#define FLOW_STEP_SAU_I_EN          0x0001b696U        /*!< Step SAU enable Init value */

#if  (TFM_TAMPER_ENABLE == ALL_TAMPER)
#define FLOW_STEP_TAMP_ACT_EN       0x000673eaU        /*!< Step Tamper active enable value */
#else
#define FLOW_STEP_TAMP_ACT_EN       0x00000000U        /*!< No effect on control flow */
#endif /* (TFM_TAMPER_ENABLE == ALL_TAMPER) */
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
#define FLOW_STEP_TAMP_INT_EN       0x00067f4dU        /*!< Step Tamper internal enable value */
#define FLOW_STEP_TAMP_SEC_EN       0x0006ad9fU        /*!< Step Tamper secure enable value */
#define FLOW_STEP_TAMP_PRIV_EN      0x0006b381U        /*!< Step Tamper privilege enable value */
#define FLOW_STEP_TAMP_CFG_EN       0x0006bf26U        /*!< Step Tamper configurable enable value */
#else
#define FLOW_STEP_TAMP_INT_EN       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_SEC_EN       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_PRIV_EN      0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_CFG_EN       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_TAMPER_ENABLE != NO_TAMPER) */

#ifdef TFM_FLASH_PRIVONLY_ENABLE
#define FLOW_STEP_FLASH_P_EN        0x0001cef0U        /*!< Step FLASH Priv enable value */
#else
#define FLOW_STEP_FLASH_P_EN        0x00000000U        /*!< No effect on control flow */
#endif /* TFM_FLASH_PRIVONLY_ENABLE */


#ifdef TFM_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_S_I_CH_R0     0x0001d0eeU        /*!< Step MPU Secure Region 0 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R1     0x0001e1e1U        /*!< Step MPU Secure Region 1 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R2     0x0001ffffU        /*!< Step MPU Secure Region 2 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R3     0x00023bd7U        /*!< Step MPU Secure Region 3 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R4     0x00025dafU        /*!< Step MPU Secure Region 4 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R5     0x00029cf6U        /*!< Step MPU Secure Region 5 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R6     0x0003da37U        /*!< Step MPU Secure Region 5 Init check value */
#define FLOW_STEP_MPU_S_I_CH        0x0002e8fbU        /*!< Step MPU Secure Init check value */
#define FLOW_STEP_MPU_NS_I_CH_R0    0x0002f71bU        /*!< Step MPU NonSecure Region 0 Init check value */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_MPU_NS_I_CH_R1    0x0002fb70U        /*!< Step MPU NonSecure Region 1 Init check value */
#else
#define FLOW_STEP_MPU_NS_I_CH_R1    0x00000000U        /*!< No effect on control flow */
#endif /* !MCUBOOT_PRIMARY_ONLY */
#define FLOW_STEP_MPU_NS_I_CH_R2    0x00031b6eU        /*!< Step MPU NonSecure Region 2 Init check value */
#define FLOW_STEP_MPU_NS_I_CH_R3    0x00036e9dU        /*!< Step MPU NonSecure Region 3 Init check value */
#define FLOW_STEP_MPU_NS_I_CH       0x0003717dU        /*!< Step MPU NonSecure Init check value */
#else /* TFM_BOOT_MPU_PROTECTION */
#define FLOW_STEP_MPU_S_I_CH_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R1     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R3     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R4     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R5     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R6     0x00000000U        /*!< Step MPU Secure Region 5 Init check value */
#define FLOW_STEP_MPU_S_I_CH        0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_CH_R0    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_CH_R1    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_CH_R2    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_CH_R3    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_I_CH       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_BOOT_MPU_PROTECTION */

#ifdef TFM_ERROR_HANDLER_NON_SECURE
#define FLOW_STEP_SAU_I_CH_R0       0x00037d16U        /*!< Step SAU Region 0 Init check value */
#else
#define FLOW_STEP_SAU_I_CH_R0       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_ERROR_HANDLER_NON_SECURE */
#define FLOW_STEP_SAU_I_CH_R1       0x0003ae3aU        /*!< Step SAU Region 1 Init check value */
#define FLOW_STEP_SAU_I_CH_R2       0x0003b1daU        /*!< Step SAU Region 2 Init check value */
#if defined(MCUBOOT_EXT_LOADER)
#define FLOW_STEP_SAU_I_CH_R3       0x0003bdb1U        /*!< Step SAU Region 3 Init check value */
#else
#define FLOW_STEP_SAU_I_CH_R3       0x00000000U        /*!< No effect on control flow */
#endif /* MCUBOOT_EXT_LOADER */
#define FLOW_STEP_SAU_I_CH          0x0003c9bcU        /*!< Step SAU Init check value */

#if  (TFM_TAMPER_ENABLE == ALL_TAMPER)
#define FLOW_STEP_TAMP_ACT_CH       0x0006ca19U        /*!< Step Tamper active check value */
#else
#define FLOW_STEP_TAMP_ACT_CH       0x00000000U        /*!< No effect on control flow */    
#endif /*(TFM_TAMPER_ENABLE == ALL_TAMPER) */
#if  (TFM_TAMPER_ENABLE != NO_TAMPER)
#define FLOW_STEP_TAMP_INT_CH       0x0006d407U        /*!< Step Tamper internal check value */
#define FLOW_STEP_TAMP_SEC_CH       0x0006d95eU        /*!< Step Tamper secure check value */
#define FLOW_STEP_TAMP_PRIV_CH      0x00072a07U        /*!< Step Tamper privilege check value */
#define FLOW_STEP_TAMP_CFG_CH       0x00073419U        /*!< Step Tamper configurable check value */
#else
#define FLOW_STEP_TAMP_INT_CH       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_SEC_CH       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_PRIV_CH      0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_CFG_CH       0x00000000U        /*!< No effect on control flow */
#endif /* TFM_TAMPER_ENABLE != NO_TAMPER) */

#ifdef TFM_FLASH_PRIVONLY_ENABLE
#define FLOW_STEP_FLASH_P_CH        0x0003d7a2U        /*!< Step FLASH Priv check value */
#else
#define FLOW_STEP_FLASH_P_CH        0x00000000U        /*!< No effect on control flow */
#endif /* TFM_FLASH_PRIVONLY_ENABLE */

#ifdef TFM_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_S_A_EN_R0     0x0003dbc9U        /*!< Step MPU Secure Region 0 Appli enable value */
#define FLOW_STEP_MPU_NS_A_EN_R0    0x00045afcU        /*!< Step MPU NonSecure Region 0 Appli enable value */
#define FLOW_STEP_MPU_S_A_CH_R0     0x000474dfU        /*!< Step MPU Secure Region 0 Appli check value */
#define FLOW_STEP_MPU_NS_A_CH_R0    0x0004a7f3U        /*!< Step MPU NonSecure Region 0 Appli check value */
#else
#define FLOW_STEP_MPU_S_A_EN_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_A_EN_R0    0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_A_CH_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_NS_A_CH_R0    0x00000000U        /*!< No effect on control flow */
#endif /* TFM_BOOT_MPU_PROTECTION */

#if defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_FLASH_S_L_EN      0x000660adU        /*!< Step FLASH Secure Loader enable value */
#define FLOW_STEP_FLASH_S_L_CH      0x000618cbU        /*!< Step FLASH Secure Loader check value */
#else
#define FLOW_STEP_FLASH_S_L_EN      0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_FLASH_S_L_CH      0x00000000U        /*!< No effect on control flow */
#endif /* MCUBOOT_PRIMARY_ONLY */

#if defined(TFM_BOOT_MPU_PROTECTION) && defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_MPU_S_L_DI_R2     0x0004de6bU        /*!< Step MPU Secure Region 2 Loader disable value */
#define FLOW_STEP_MPU_S_L_EN_R7     0x0004ee56U        /*!< Step MPU Secure Region 7 Loader enable value */
#else
#define FLOW_STEP_MPU_S_L_DI_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_L_EN_R7     0x00000000U        /*!< No effect on control flow */
#endif /* TFM_BOOT_MPU_PROTECTION  && MCUBOOT_PRIMARY_ONLY */

#define FLOW_STEP_SAU_L_EN_R0       0x000547edU        /*!< Step SAU Region 0 Loader enable value */
#define FLOW_STEP_SAU_L_EN_R1       0x000569ceU        /*!< Step SAU Region 1 Loader enable value */
#define FLOW_STEP_SAU_L_EN_R2       0x000577d0U        /*!< Step SAU Region 2 Loader enable value */
#define FLOW_STEP_SAU_L_EN_R3       0x0005953fU        /*!< Step SAU Region 3 Loader enable value */
#if defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_SAU_L_EN_R4       0x0005bae2U        /*!< Step SAU Region 4 Loader enable value */
#else
#define FLOW_STEP_SAU_L_EN_R4       0x00000000U        /*!< No effect on control flow */
#endif /* MCUBOOT_PRIMARY_ONLY */
#define FLOW_STEP_SAU_L_EN          0x0005c37aU        /*!< Step SAU enable Loader value */

#if defined(TFM_BOOT_MPU_PROTECTION) && defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_MPU_S_L_CH_R2     0x0004f1b6U        /*!< Step MPU Secure Region 2 Loader check value */
#define FLOW_STEP_MPU_S_L_CH_R7     0x00053e75U        /*!< Step MPU Secure Region 7 Loader check value */
#else
#define FLOW_STEP_MPU_S_L_CH_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_L_CH_R7     0x00000000U        /*!< No effect on control flow */
#endif /* TFM_BOOT_MPU_PROTECTION  && MCUBOOT_PRIMARY_ONLY */

#define FLOW_STEP_SAU_L_CH_R0       0x0005dd64U        /*!< Step SAU Region 0 Loader check value */
#define FLOW_STEP_SAU_L_CH_R1       0x0005ed59U        /*!< Step SAU Region 1 Loader check value */
#define FLOW_STEP_SAU_L_CH_R2       0x0005f347U        /*!< Step SAU Region 2 Loader check value */
#define FLOW_STEP_SAU_L_CH_R3       0x0006072bU        /*!< Step SAU Region 3 Loader check value */
#if defined(MCUBOOT_PRIMARY_ONLY)
#define FLOW_STEP_SAU_L_CH_R4       0x00061935U        /*!< Step SAU Region 4 Loader check value */
#else
#define FLOW_STEP_SAU_L_CH_R4       0x00000000U        /*!< No effect on control flow */
#endif /* MCUBOOT_PRIMARY_ONLY */
#define FLOW_STEP_SAU_L_CH          0x00066153U        /*!< Step SAU check Loader value */

/**
  * @brief  SFU_BOOT Flow Control : Control values runtime protections
  */
/* Flow control Stage 1 */
#define FLOW_CTRL_MPU_S_I_EN_R0     (FLOW_CTRL_INIT_VALUE ^      FLOW_STEP_MPU_S_I_EN_R0)
#define FLOW_CTRL_MPU_S_I_EN_R1     (FLOW_CTRL_MPU_S_I_EN_R0 ^   FLOW_STEP_MPU_S_I_EN_R1)
#define FLOW_CTRL_MPU_S_I_EN_R2     (FLOW_CTRL_MPU_S_I_EN_R1 ^   FLOW_STEP_MPU_S_I_EN_R2)
#define FLOW_CTRL_MPU_S_I_EN_R3     (FLOW_CTRL_MPU_S_I_EN_R2 ^   FLOW_STEP_MPU_S_I_EN_R3)
#define FLOW_CTRL_MPU_S_I_EN_R4     (FLOW_CTRL_MPU_S_I_EN_R3 ^   FLOW_STEP_MPU_S_I_EN_R4)
#define FLOW_CTRL_MPU_S_I_EN_R5     (FLOW_CTRL_MPU_S_I_EN_R4 ^   FLOW_STEP_MPU_S_I_EN_R5)
#define FLOW_CTRL_MPU_S_I_EN_R6     (FLOW_CTRL_MPU_S_I_EN_R5 ^   FLOW_STEP_MPU_S_I_EN_R6)
#define FLOW_CTRL_MPU_S_I_EN        (FLOW_CTRL_MPU_S_I_EN_R6 ^   FLOW_STEP_MPU_S_I_EN)
#define FLOW_CTRL_MPU_NS_I_EN_R0    (FLOW_CTRL_MPU_S_I_EN ^      FLOW_STEP_MPU_NS_I_EN_R0)
#define FLOW_CTRL_MPU_NS_I_EN_R1    (FLOW_CTRL_MPU_NS_I_EN_R0 ^  FLOW_STEP_MPU_NS_I_EN_R1)
#define FLOW_CTRL_MPU_NS_I_EN_R2    (FLOW_CTRL_MPU_NS_I_EN_R1 ^  FLOW_STEP_MPU_NS_I_EN_R2)
#define FLOW_CTRL_MPU_NS_I_EN_R3    (FLOW_CTRL_MPU_NS_I_EN_R2 ^  FLOW_STEP_MPU_NS_I_EN_R3)
#define FLOW_CTRL_MPU_NS_I_EN       (FLOW_CTRL_MPU_NS_I_EN_R3 ^  FLOW_STEP_MPU_NS_I_EN)

#define FLOW_CTRL_SAU_I_EN_R0       (FLOW_CTRL_MPU_NS_I_EN ^     FLOW_STEP_SAU_I_EN_R0)
#define FLOW_CTRL_SAU_I_EN_R1       (FLOW_CTRL_SAU_I_EN_R0 ^     FLOW_STEP_SAU_I_EN_R1)
#define FLOW_CTRL_SAU_I_EN_R2       (FLOW_CTRL_SAU_I_EN_R1 ^     FLOW_STEP_SAU_I_EN_R2)
#define FLOW_CTRL_SAU_I_EN_R3       (FLOW_CTRL_SAU_I_EN_R2 ^     FLOW_STEP_SAU_I_EN_R3)
#define FLOW_CTRL_SAU_I_EN          (FLOW_CTRL_SAU_I_EN_R3 ^     FLOW_STEP_SAU_I_EN)

#define FLOW_CTRL_TAMP_ACT_EN       (FLOW_CTRL_SAU_I_EN ^        FLOW_STEP_TAMP_ACT_EN)
#define FLOW_CTRL_TAMP_INT_EN       (FLOW_CTRL_TAMP_ACT_EN ^     FLOW_STEP_TAMP_INT_EN)
#define FLOW_CTRL_TAMP_SEC_EN       (FLOW_CTRL_TAMP_INT_EN ^     FLOW_STEP_TAMP_SEC_EN)
#define FLOW_CTRL_TAMP_PRIV_EN      (FLOW_CTRL_TAMP_SEC_EN ^     FLOW_STEP_TAMP_PRIV_EN)
#define FLOW_CTRL_TAMP_CFG_EN       (FLOW_CTRL_TAMP_PRIV_EN ^    FLOW_STEP_TAMP_CFG_EN)

#define FLOW_CTRL_FLASH_P_EN        (FLOW_CTRL_TAMP_CFG_EN ^     FLOW_STEP_FLASH_P_EN)

#define FLOW_CTRL_STAGE_1           FLOW_CTRL_FLASH_P_EN

/* Flow control Stage 2 */
#define FLOW_CTRL_MPU_S_I_CH_R0     (FLOW_CTRL_STAGE_1 ^         FLOW_STEP_MPU_S_I_CH_R0)
#define FLOW_CTRL_MPU_S_I_CH_R1     (FLOW_CTRL_MPU_S_I_CH_R0 ^   FLOW_STEP_MPU_S_I_CH_R1)
#define FLOW_CTRL_MPU_S_I_CH_R2     (FLOW_CTRL_MPU_S_I_CH_R1 ^   FLOW_STEP_MPU_S_I_CH_R2)
#define FLOW_CTRL_MPU_S_I_CH_R3     (FLOW_CTRL_MPU_S_I_CH_R2 ^   FLOW_STEP_MPU_S_I_CH_R3)
#define FLOW_CTRL_MPU_S_I_CH_R4     (FLOW_CTRL_MPU_S_I_CH_R3 ^   FLOW_STEP_MPU_S_I_CH_R4)
#define FLOW_CTRL_MPU_S_I_CH_R5     (FLOW_CTRL_MPU_S_I_CH_R4 ^   FLOW_STEP_MPU_S_I_CH_R5)
#define FLOW_CTRL_MPU_S_I_CH_R6     (FLOW_CTRL_MPU_S_I_CH_R5 ^   FLOW_STEP_MPU_S_I_CH_R6)
#define FLOW_CTRL_MPU_S_I_CH        (FLOW_CTRL_MPU_S_I_CH_R6 ^   FLOW_STEP_MPU_S_I_CH)
#define FLOW_CTRL_MPU_NS_I_CH_R0    (FLOW_CTRL_MPU_S_I_CH ^      FLOW_STEP_MPU_NS_I_CH_R0)
#define FLOW_CTRL_MPU_NS_I_CH_R1    (FLOW_CTRL_MPU_NS_I_CH_R0 ^  FLOW_STEP_MPU_NS_I_CH_R1)
#define FLOW_CTRL_MPU_NS_I_CH_R2    (FLOW_CTRL_MPU_NS_I_CH_R1 ^  FLOW_STEP_MPU_NS_I_CH_R2)
#define FLOW_CTRL_MPU_NS_I_CH_R3    (FLOW_CTRL_MPU_NS_I_CH_R2 ^  FLOW_STEP_MPU_NS_I_CH_R3)
#define FLOW_CTRL_MPU_NS_I_CH       (FLOW_CTRL_MPU_NS_I_CH_R3 ^  FLOW_STEP_MPU_NS_I_CH)

#define FLOW_CTRL_SAU_I_CH_R0       (FLOW_CTRL_MPU_NS_I_CH ^     FLOW_STEP_SAU_I_CH_R0)
#define FLOW_CTRL_SAU_I_CH_R1       (FLOW_CTRL_SAU_I_CH_R0 ^     FLOW_STEP_SAU_I_CH_R1)
#define FLOW_CTRL_SAU_I_CH_R2       (FLOW_CTRL_SAU_I_CH_R1 ^     FLOW_STEP_SAU_I_CH_R2)
#define FLOW_CTRL_SAU_I_CH_R3       (FLOW_CTRL_SAU_I_CH_R2 ^     FLOW_STEP_SAU_I_CH_R3)
#define FLOW_CTRL_SAU_I_CH          (FLOW_CTRL_SAU_I_CH_R3 ^     FLOW_STEP_SAU_I_CH)

#define FLOW_CTRL_TAMP_ACT_CH       (FLOW_CTRL_SAU_I_CH ^        FLOW_STEP_TAMP_ACT_CH)
#define FLOW_CTRL_TAMP_INT_CH       (FLOW_CTRL_TAMP_ACT_CH ^     FLOW_STEP_TAMP_INT_CH)
#define FLOW_CTRL_TAMP_SEC_CH       (FLOW_CTRL_TAMP_INT_CH ^     FLOW_STEP_TAMP_SEC_CH)
#define FLOW_CTRL_TAMP_PRIV_CH      (FLOW_CTRL_TAMP_SEC_CH ^     FLOW_STEP_TAMP_PRIV_CH)
#define FLOW_CTRL_TAMP_CFG_CH       (FLOW_CTRL_TAMP_PRIV_CH ^    FLOW_STEP_TAMP_CFG_CH)

#define FLOW_CTRL_FLASH_P_CH        (FLOW_CTRL_TAMP_CFG_CH ^     FLOW_STEP_FLASH_P_CH)

#define FLOW_CTRL_STAGE_2           FLOW_CTRL_FLASH_P_CH

/* Flow control Stage 3 Appli */
#define FLOW_CTRL_MPU_S_A_EN_R0     (FLOW_CTRL_STAGE_2 ^         FLOW_STEP_MPU_S_A_EN_R0)
#define FLOW_CTRL_MPU_NS_A_EN_R0    (FLOW_CTRL_MPU_S_A_EN_R0 ^   FLOW_STEP_MPU_NS_A_EN_R0)

#define FLOW_CTRL_STAGE_3_A         FLOW_CTRL_MPU_NS_A_EN_R0

/* Flow control Stage 4 Appli */
#define FLOW_CTRL_MPU_S_A_CH_R0     (FLOW_CTRL_STAGE_3_A ^       FLOW_STEP_MPU_S_A_CH_R0)
#define FLOW_CTRL_MPU_NS_A_CH_R0    (FLOW_CTRL_MPU_S_A_CH_R0 ^   FLOW_STEP_MPU_NS_A_CH_R0)

#define FLOW_CTRL_STAGE_4_A         FLOW_CTRL_MPU_NS_A_CH_R0

/* Flow control Stage 3 Loader */
#define FLOW_CTRL_FLASH_S_L_EN      (FLOW_CTRL_STAGE_2 ^         FLOW_STEP_FLASH_S_L_EN)

#define FLOW_CTRL_STAGE_3_L         FLOW_CTRL_FLASH_S_L_EN

/* Flow control Stage 4 Loader */
#define FLOW_CTRL_FLASH_S_L_CH      (FLOW_CTRL_STAGE_3_L ^       FLOW_STEP_FLASH_S_L_CH)

#define FLOW_CTRL_STAGE_4_L         FLOW_CTRL_FLASH_S_L_CH

/* Flow control Stage 5 Loader */
#define FLOW_CTRL_MPU_S_L_DI_R2     (FLOW_CTRL_STAGE_4_L ^       FLOW_STEP_MPU_S_L_DI_R2)

#define FLOW_CTRL_SAU_L_EN_R0       (FLOW_CTRL_MPU_S_L_DI_R2  ^   FLOW_STEP_SAU_L_EN_R0)
#define FLOW_CTRL_SAU_L_EN_R1       (FLOW_CTRL_SAU_L_EN_R0 ^     FLOW_STEP_SAU_L_EN_R1)
#define FLOW_CTRL_SAU_L_EN_R2       (FLOW_CTRL_SAU_L_EN_R1 ^     FLOW_STEP_SAU_L_EN_R2)
#define FLOW_CTRL_SAU_L_EN_R3       (FLOW_CTRL_SAU_L_EN_R2 ^     FLOW_STEP_SAU_L_EN_R3)
#define FLOW_CTRL_SAU_L_EN_R4       (FLOW_CTRL_SAU_L_EN_R3 ^     FLOW_STEP_SAU_L_EN_R4)
#define FLOW_CTRL_SAU_L_EN          (FLOW_CTRL_SAU_L_EN_R4 ^     FLOW_STEP_SAU_L_EN)

#define FLOW_CTRL_STAGE_5_L         FLOW_CTRL_SAU_L_EN

/* Flow control Stage 6 Loader */
#define FLOW_CTRL_MPU_S_L_CH_R2     (FLOW_CTRL_STAGE_5_L ^       FLOW_STEP_MPU_S_L_CH_R2)

#define FLOW_CTRL_SAU_L_CH_R0       (FLOW_CTRL_MPU_S_L_CH_R2 ^   FLOW_STEP_SAU_L_CH_R0)
#define FLOW_CTRL_SAU_L_CH_R1       (FLOW_CTRL_SAU_L_CH_R0 ^     FLOW_STEP_SAU_L_CH_R1)
#define FLOW_CTRL_SAU_L_CH_R2       (FLOW_CTRL_SAU_L_CH_R1 ^     FLOW_STEP_SAU_L_CH_R2)
#define FLOW_CTRL_SAU_L_CH_R3       (FLOW_CTRL_SAU_L_CH_R2 ^     FLOW_STEP_SAU_L_CH_R3)
#define FLOW_CTRL_SAU_L_CH_R4       (FLOW_CTRL_SAU_L_CH_R3 ^     FLOW_STEP_SAU_L_CH_R4)
#define FLOW_CTRL_SAU_L_CH          (FLOW_CTRL_SAU_L_CH_R4 ^     FLOW_STEP_SAU_L_CH)

#define FLOW_CTRL_STAGE_6_L         FLOW_CTRL_SAU_L_CH


/* External variables --------------------------------------------------------*/
/**
  *  Flow control protection values
  *  Flow control stage
  */
extern volatile uint32_t uFlowProtectValue;
extern volatile uint32_t uFlowStage;

/* Exported macros -----------------------------------------------------------*/
/** Control with STEP operation :
  * (uFlowValue XOR STEP_VALUE) should be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_STEP(C,B,A) \
  do{ \
    (C) ^= (B);\
    if ((C) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_STEP(C,B,A) ((void)0)
#endif

/** Control without STEP operation :
  * uFlowValue should be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_CHECK(B,A) \
  do{ \
    if ((B) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_CHECK(B,A) ((void)0)
#endif

/** Control flow initialization
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_INIT(B,A) \
  do{ \
    (B) = (A);\
  }while(0)
#else
#define FLOW_CONTROL_INIT(B,A) ((void)0)
#endif

/** STEP update only :
  * (uFlowValue XOR STEP_VALUE)
  */
#if defined(FLOW_CONTROL)
#define FLOW_STEP(B,A) \
  do{ \
    (B) ^= (A);\
  }while(0)
#else
#define FLOW_STEP(B,A) ((void)0)
#endif

/* Exported functions ------------------------------------------------------- */

/** @defgroup BOOT_FLOWCONTROL_Exported_Functions Exported Functions
  * @{
  */


/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_FLOWCONTROL_H */
