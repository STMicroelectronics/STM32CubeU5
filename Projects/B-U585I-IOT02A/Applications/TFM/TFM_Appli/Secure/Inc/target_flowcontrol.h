/**
  ******************************************************************************
  * @file    target_flowcontrol.h
  * @author  MCD Application Team
  * @brief   Header for flow control in target_cfg.c module
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
#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
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
#define FLOW_STEP_SAU_EN_R0       0x000165baU        /*!< Step SAU Region 0 Init enable value */
#define FLOW_STEP_SAU_EN_R1       0x00017ba4U        /*!< Step SAU Region 1 Init enable value */
#define FLOW_STEP_SAU_EN_R2       0x00018799U        /*!< Step SAU Region 2 Init enable value */
#define FLOW_STEP_SAU_EN_R3       0x00019987U        /*!< Step SAU Region 3 Init enable value */
#define FLOW_STEP_SAU_EN_R4       0x0000cccdU        /*!< Step SAU Region 4 Init enable value */
#define FLOW_STEP_SAU_EN_R5       0x0000d2d3U        /*!< Step SAU Region 5 Init enable value */
#define FLOW_STEP_SAU_EN          0x0001b696U        /*!< Step SAU enable Init value */

#define FLOW_STEP_SAU_CH_R0       0x00037d16U        /*!< Step SAU Region 0 Init check value */
#define FLOW_STEP_SAU_CH_R1       0x0003ae3aU        /*!< Step SAU Region 1 Init check value */
#define FLOW_STEP_SAU_CH_R2       0x0003b1daU        /*!< Step SAU Region 2 Init check value */
#define FLOW_STEP_SAU_CH_R3       0x0003bdb1U        /*!< Step SAU Region 3 Init check value */
#define FLOW_STEP_SAU_CH_R4       0x0000fdc2U        /*!< Step SAU Region 2 Init check value */
#define FLOW_STEP_SAU_CH_R5       0x00012cd3U        /*!< Step SAU Region 3 Init check value */
#define FLOW_STEP_SAU_CH          0x0003c9bcU        /*!< Step SAU Init check value */
#define FLOW_STEP_SAU_LCK         0x0052295aU        /*!< Step SAU locked */
#define FLOW_STEP_SAU_LCK_CH      0x005291acU        /*!< Step SAU locked check */

#define FLOW_STEP_MPU_S_EN_R0     0x00006787U        /*!< Step MPU Secure Region 0 Init enable value */
#define FLOW_STEP_MPU_S_EN_R1     0x00007999U        /*!< Step MPU Secure Region 1 Init enable value */
#define FLOW_STEP_MPU_S_EN_R2     0x0000aab5U        /*!< Step MPU Secure Region 2 Init enable value */
#define FLOW_STEP_MPU_S_EN_R3     0x0000b4abU        /*!< Step MPU Secure Region 3 Init enable value */
#define FLOW_STEP_MPU_S_EN_R4     0x0002f6e5U        /*!< Step MPU Secure Region 4 Init enable value */
#define FLOW_STEP_MPU_S_EN_R5     0x0002fa8eU        /*!< Step MPU Secure Region 5 Init enable value */
#define FLOW_STEP_MPU_S_EN_R6     0x0001b768U        /*!< Step MPU Secure Region 6 Init enable value */
#define FLOW_STEP_MPU_S_EN_R7     0x0001cf0eU        /*!< Step MPU Secure Region 7 Init enable value */
#define FLOW_STEP_MPU_S_EN        0x0000e3dcU        /*!< Step MPU Secure Init enable value */

#define FLOW_STEP_MPU_S_CH_R0     0x0001d0eeU        /*!< Step MPU Secure Region 0 Init check value */
#define FLOW_STEP_MPU_S_CH_R1     0x0001e1e1U        /*!< Step MPU Secure Region 1 Init check value */
#define FLOW_STEP_MPU_S_CH_R2     0x0001ffffU        /*!< Step MPU Secure Region 2 Init check value */
#define FLOW_STEP_MPU_S_CH_R3     0x00023bd7U        /*!< Step MPU Secure Region 3 Init check value */
#define FLOW_STEP_MPU_S_CH_R4     0x000316fbU        /*!< Step MPU Secure Region 4 Init check value */
#define FLOW_STEP_MPU_S_CH_R5     0x000362f6U        /*!< Step MPU Secure Region 5 Init check value */
#define FLOW_STEP_MPU_S_CH_R6     0x0001e01fU        /*!< Step MPU Secure Region 6 Init check value */
#define FLOW_STEP_MPU_S_CH_R7     0x0001fe01U        /*!< Step MPU Secure Region 7 Init check value */
#define FLOW_STEP_MPU_S_CH        0x00024fdaU        /*!< Step MPU Secure Init check value */

#define FLOW_STEP_MPU_S_LCK       0x005176a3U        /*!< Step MPU Secure locked */
#define FLOW_STEP_MPU_S_LCK_CH    0x0051ebeaU        /*!< Step MPU Secure locked check */

#define FLOW_STEP_GTZC_VTOR_LCK   0x00029cf6U        /*!< Step GTZC Vector locked check */

#define FLOW_STEP_GTZC_PERIPH_CFG 0x00028f7dU        /*!< Step GTZC Periphs config value */

#define FLOW_STEP_GTZC_PERIPH_CH  0x0002e56eU        /*!< Step GTZC Periphs check value */


// #define FLOW_STEP_PERIPH_HASH_CH  0x00036f63U        /*!< Step GTZC Periph HASH check value */
// #define FLOW_STEP_PERIPH_PKA_CH   0x00037ce8U        /*!< Step GTZC Periph PKA check value */
// #define FLOW_STEP_PERIPH_AES_CH   0x0003a3afU        /*!< Step GTZC Periph AES check value */
// #define FLOW_STEP_PERIPH_SAES_CH  0x0003afc4U        /*!< Step GTZC Periph SAES check value */

/**
  * @brief  SFU_BOOT Flow Control : Control values runtime protections
  */
/* Flow control Stage 1 */
#define FLOW_CTRL_GTZC_VTOR_LCK   (FLOW_CTRL_INIT_VALUE ^     FLOW_STEP_GTZC_VTOR_LCK)

#define FLOW_CTRL_GTZC_PERIPH_CFG (FLOW_CTRL_GTZC_VTOR_LCK ^      FLOW_STEP_GTZC_PERIPH_CFG)

#define FLOW_CTRL_SAU_EN_R0       (FLOW_CTRL_GTZC_PERIPH_CFG ^ FLOW_STEP_SAU_EN_R0)
#define FLOW_CTRL_SAU_EN_R1       (FLOW_CTRL_SAU_EN_R0 ^       FLOW_STEP_SAU_EN_R1)
#define FLOW_CTRL_SAU_EN_R2       (FLOW_CTRL_SAU_EN_R1 ^       FLOW_STEP_SAU_EN_R2)
#define FLOW_CTRL_SAU_EN_R3       (FLOW_CTRL_SAU_EN_R2 ^       FLOW_STEP_SAU_EN_R3)
#define FLOW_CTRL_SAU_EN_R4       (FLOW_CTRL_SAU_EN_R3 ^       FLOW_STEP_SAU_EN_R4)
#define FLOW_CTRL_SAU_EN_R5       (FLOW_CTRL_SAU_EN_R4 ^       FLOW_STEP_SAU_EN_R5)
#define FLOW_CTRL_SAU_EN          (FLOW_CTRL_SAU_EN_R5 ^       FLOW_STEP_SAU_EN)

#define FLOW_CTRL_GTZC_PERIPH_CH  (FLOW_CTRL_SAU_EN    ^       FLOW_STEP_GTZC_PERIPH_CH)

#define FLOW_CTRL_SAU_CH_R0       (FLOW_CTRL_GTZC_PERIPH_CH ^  FLOW_STEP_SAU_CH_R0)
#define FLOW_CTRL_SAU_CH_R1       (FLOW_CTRL_SAU_CH_R0 ^       FLOW_STEP_SAU_CH_R1)
#define FLOW_CTRL_SAU_CH_R2       (FLOW_CTRL_SAU_CH_R1 ^       FLOW_STEP_SAU_CH_R2)
#define FLOW_CTRL_SAU_CH_R3       (FLOW_CTRL_SAU_CH_R2 ^       FLOW_STEP_SAU_CH_R3)
#define FLOW_CTRL_SAU_CH_R4       (FLOW_CTRL_SAU_CH_R3 ^       FLOW_STEP_SAU_CH_R4)
#define FLOW_CTRL_SAU_CH_R5       (FLOW_CTRL_SAU_CH_R4 ^       FLOW_STEP_SAU_CH_R5)
#define FLOW_CTRL_SAU_CH          (FLOW_CTRL_SAU_CH_R5 ^       FLOW_STEP_SAU_CH)

#define FLOW_CTRL_SAU_LCK         (FLOW_CTRL_SAU_CH    ^       FLOW_STEP_SAU_LCK)
#define FLOW_CTRL_SAU_LCK_CH      (FLOW_CTRL_SAU_LCK   ^       FLOW_STEP_SAU_LCK_CH)

#define FLOW_CTRL_MPU_S_EN_R0     (FLOW_CTRL_SAU_LCK_CH ^      FLOW_STEP_MPU_S_EN_R0)
#define FLOW_CTRL_MPU_S_EN_R1     (FLOW_CTRL_MPU_S_EN_R0 ^     FLOW_STEP_MPU_S_EN_R1)
#define FLOW_CTRL_MPU_S_EN_R2     (FLOW_CTRL_MPU_S_EN_R1 ^     FLOW_STEP_MPU_S_EN_R2)
#define FLOW_CTRL_MPU_S_EN_R3     (FLOW_CTRL_MPU_S_EN_R2 ^     FLOW_STEP_MPU_S_EN_R3)
#define FLOW_CTRL_MPU_S_EN_R4     (FLOW_CTRL_MPU_S_EN_R3 ^     FLOW_STEP_MPU_S_EN_R4)
#define FLOW_CTRL_MPU_S_EN_R5     (FLOW_CTRL_MPU_S_EN_R4 ^     FLOW_STEP_MPU_S_EN_R5)
#define FLOW_CTRL_MPU_S_EN_R6     (FLOW_CTRL_MPU_S_EN_R5 ^     FLOW_STEP_MPU_S_EN_R6)
#define FLOW_CTRL_MPU_S_EN_R7     (FLOW_CTRL_MPU_S_EN_R6 ^     FLOW_STEP_MPU_S_EN_R7)
#define FLOW_CTRL_MPU_S_EN        (FLOW_CTRL_MPU_S_EN_R7 ^     FLOW_STEP_MPU_S_EN)

#define FLOW_CTRL_MPU_S_CH_R0     (FLOW_CTRL_MPU_S_EN ^        FLOW_STEP_MPU_S_CH_R0)
#define FLOW_CTRL_MPU_S_CH_R1     (FLOW_CTRL_MPU_S_CH_R0 ^     FLOW_STEP_MPU_S_CH_R1)
#define FLOW_CTRL_MPU_S_CH_R2     (FLOW_CTRL_MPU_S_CH_R1 ^     FLOW_STEP_MPU_S_CH_R2)
#define FLOW_CTRL_MPU_S_CH_R3     (FLOW_CTRL_MPU_S_CH_R2 ^     FLOW_STEP_MPU_S_CH_R3)
#define FLOW_CTRL_MPU_S_CH_R4     (FLOW_CTRL_MPU_S_CH_R3 ^     FLOW_STEP_MPU_S_CH_R4)
#define FLOW_CTRL_MPU_S_CH_R5     (FLOW_CTRL_MPU_S_CH_R4 ^     FLOW_STEP_MPU_S_CH_R5)
#define FLOW_CTRL_MPU_S_CH_R6     (FLOW_CTRL_MPU_S_CH_R5 ^     FLOW_STEP_MPU_S_CH_R6)
#define FLOW_CTRL_MPU_S_CH_R7     (FLOW_CTRL_MPU_S_CH_R6 ^     FLOW_STEP_MPU_S_CH_R7)
#define FLOW_CTRL_MPU_S_CH        (FLOW_CTRL_MPU_S_CH_R7 ^     FLOW_STEP_MPU_S_CH)

#define FLOW_CTRL_MPU_S_LCK       (FLOW_CTRL_MPU_S_CH    ^     FLOW_STEP_MPU_S_LCK)
#define FLOW_CTRL_MPU_S_LCK_CH    (FLOW_CTRL_MPU_S_LCK   ^     FLOW_STEP_MPU_S_LCK_CH)
#define FLOW_CTRL_STAGE_1         FLOW_CTRL_MPU_S_LCK_CH


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

#endif /* FLOWCONTROL_H */
