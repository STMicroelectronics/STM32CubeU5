/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_common.h
  * @author  MCD Application Team
  * @brief   header file provides the LPBAM COMMON defines
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
#ifndef STM32_PLATFORM_LPBAM_COMMON_H
#define STM32_PLATFORM_LPBAM_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMMON_Platform
  * @brief      COMMON Platform LPBAM module driver
  * @{
  */

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMMON_Platform_Private_Types LPBAM COMMON Platform Private Types
  * @brief    LPBAM COMMON Platform Private Types
  * @{
  */

/**
  * @brief LPBAM DMA Transfer Configuration Structure definition.
  */
typedef struct
{
  uint32_t SrcInc;            /*!< Specifies the source increment mode for the DMA channel.
                                   This parameter can be a value of @ref LPBAM_DMA_Source_Increment_Mode      */

  uint32_t DestInc;           /*!< Specifies the destination increment mode for the DMA channel.
                                   This parameter can be a value of @ref LPBAM_DMA_Destination_Increment_Mode */

  uint32_t SrcDataWidth;      /*!< Specifies the source data width for the DMA channel.
                                   This parameter can be a value of @ref LPBAM_DMA_Source_Data_Width          */

  uint32_t DestDataWidth;     /*!< Specifies the destination data width for the DMA channel.
                                   This parameter can be a value of @ref LPBAM_DMA_Destination_Data_Width     */

  uint32_t TransferEventMode; /*!< Specifies the transfer event mode for the DMA channel.
                                   This parameter can be a value of @ref LPBAM_DMA_Transfer_Event_Mode        */

} LPBAM_DMA_InitTypeDef;

/**
  * @brief LPBAM DMA Linked-List Node Configuration Structure Definition.
  */
typedef struct
{
  LPBAM_DMA_InitTypeDef Transfer;   /*!< Specifies the DMA channel basic configuration                              */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  uint32_t              SrcSecure;  /*!< Specifies the source security attribute.
                                         This parameter can be a value of @ref LPBAM_DMA_Channel_Source_Secure      */

  uint32_t              DestSecure; /*!< Specifies the destination security attribute.
                                         This parameter can be a value of @ref LPBAM_DMA_Channel_Destination_Secure */
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

} LPBAM_DMA_NodeConfTypeDef;

/**
  * @brief LPBAM DMA Trigger Configuration Structure Definition.
  */
typedef struct
{
  uint32_t TriggerMode;      /*!< Specifies the DMA channel trigger mode.
                                  This parameter can be a value of @ref LPBAM_DMA_Trigger_Mode      */

  uint32_t TriggerPolarity;  /*!< Specifies the DMA channel trigger event polarity.
                                  This parameter can be a value of @ref LPBAM_DMA_Trigger_Polarity  */

  uint32_t TriggerSelection; /*!< Specifies the DMA channel trigger event selection.
                                  This parameter can be a value of @ref LPBAM_DMA_Trigger_Selection */

} LPBAM_DMA_TriggerConfTypeDef;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMMON_Platform_Exported_Constants LPBAM COMMON Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_DMA_Source_Increment_Mode LPBAM DMA Source Increment Mode
  * @{
  */
#define LPBAM_DMA_SINC_FIXED       0x00000000U   /*!< Source fixed       single / burst */
#define LPBAM_DMA_SINC_INCREMENTED DMA_CTR1_SINC /*!< Source incremented single / burst */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Destination_Increment_Mode LPBAM DMA Destination Increment Mode
  * @{
  */
#define LPBAM_DMA_DINC_FIXED       0x00000000U   /*!< Destination fixed       single / burst */
#define LPBAM_DMA_DINC_INCREMENTED DMA_CTR1_DINC /*!< Destination incremented single / burst */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Source_Data_Width LPBAM DMA Source Data Width
  * @{
  */
#define LPBAM_DMA_SRC_DATAWIDTH_BYTE     0x00000000U         /*!< LPBAM source data width : Byte     */
#define LPBAM_DMA_SRC_DATAWIDTH_HALFWORD DMA_CTR1_SDW_LOG2_0 /*!< LPBAM source data width : HalfWord */
#define LPBAM_DMA_SRC_DATAWIDTH_WORD     DMA_CTR1_SDW_LOG2_1 /*!< LPBAM source data width : Word     */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Destination_Data_Width LPBAM DMA destination Data Width
  * @{
  */
#define LPBAM_DMA_DEST_DATAWIDTH_BYTE     0x00000000U         /*!< LPBAM destination data width : Byte     */
#define LPBAM_DMA_DEST_DATAWIDTH_HALFWORD DMA_CTR1_DDW_LOG2_0 /*!< LPBAM destination data width : HalfWord */
#define LPBAM_DMA_DEST_DATAWIDTH_WORD     DMA_CTR1_DDW_LOG2_1 /*!< LPBAM destination data width : Word     */
/**
  * @}
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/** @defgroup LPBAM_DMA_Channel_Source_Secure LPBAM DMA Channel Source Secure
  * @{
  */
#define LPBAM_DMA_CHANNEL_SRC_SEC   (DMA_CHANNEL_ATTR_SEC_SRC_MASK | 0x04U) /*!< LPBAM channel source is secure     */
#define LPBAM_DMA_CHANNEL_SRC_NSEC  (DMA_CHANNEL_ATTR_SEC_SRC_MASK)         /*!< LPBAM channel source is non-secure */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Channel_Destination_Secure LPBAM DMA Channel Destination Secure
  * @{
  */
#define LPBAM_DMA_CHANNEL_DEST_SEC  (DMA_CHANNEL_ATTR_SEC_DEST_MASK | 0x08U) /*!< LPBAM channel destination is secure     */
#define LPBAM_DMA_CHANNEL_DEST_NSEC (DMA_CHANNEL_ATTR_SEC_DEST_MASK)         /*!< LPBAM channel destination is non-secure */
/**
  * @}
  */
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */


/** @defgroup LPBAM_DMA_Trigger_Selection LPBAM DMA Trigger Selection
  * @{
  */
#define LPBAM_LPDMA1_TRIGGER_EXTI_LINE0      (0U)  /*!< LPBAM LPDMA1 HW Trigger signal is EXTI_LINE0      */
#define LPBAM_LPDMA1_TRIGGER_EXTI_LINE1      (1U)  /*!< LPBAM LPDMA1 HW Trigger signal is EXTI_LINE1      */
#define LPBAM_LPDMA1_TRIGGER_EXTI_LINE2      (2U)  /*!< LPBAM LPDMA1 HW Trigger signal is EXTI_LINE2      */
#define LPBAM_LPDMA1_TRIGGER_EXTI_LINE3      (3U)  /*!< LPBAM LPDMA1 HW Trigger signal is EXTI_LINE3      */
#define LPBAM_LPDMA1_TRIGGER_EXTI_LINE4      (4U)  /*!< LPBAM LPDMA1 HW Trigger signal is EXTI_LINE4      */
#define LPBAM_LPDMA1_TRIGGER_TAMP_TRG1       (5U)  /*!< LPBAM LPDMA1 HW Trigger signal is TAMP_TRG1       */
#define LPBAM_LPDMA1_TRIGGER_TAMP_TRG2       (6U)  /*!< LPBAM LPDMA1 HW Trigger signal is TAMP_TRG2       */
#define LPBAM_LPDMA1_TRIGGER_TAMP_TRG3       (7U)  /*!< LPBAM LPDMA1 HW Trigger signal is TAMP_TRG3       */
#define LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1      (8U)  /*!< LPBAM LPDMA1 HW Trigger signal is LPTIM1_CH1      */
#define LPBAM_LPDMA1_TRIGGER_LPTIM1_CH2      (9U)  /*!< LPBAM LPDMA1 HW Trigger signal is LPTIM1_CH2      */
#define LPBAM_LPDMA1_TRIGGER_LPTIM3_CH1      (10U) /*!< LPBAM LPDMA1 HW Trigger signal is LPTIM3_CH1      */
#define LPBAM_LPDMA1_TRIGGER_LPTIM4_OUT      (11U) /*!< LPBAM LPDMA1 HW Trigger signal is LPTIM4_OUT      */
#define LPBAM_LPDMA1_TRIGGER_COMP1_OUT       (12U) /*!< LPBAM LPDMA1 HW Trigger signal is COMP1_OUT       */
#if defined(COMP2)
#define LPBAM_LPDMA1_TRIGGER_COMP2_OUT       (13U) /*!< LPBAM LPDMA1 HW Trigger signal is COMP2_OUT       */
#endif /* defined(COMP2) */
#define LPBAM_LPDMA1_TRIGGER_RTC_ALRA_TRG    (14U) /*!< LPBAM LPDMA1 HW Trigger signal is RTC_ALRA_TRG    */
#define LPBAM_LPDMA1_TRIGGER_RTC_ALRB_TRG    (15U) /*!< LPBAM LPDMA1 HW Trigger signal is RTC_ALRB_TRG    */
#define LPBAM_LPDMA1_TRIGGER_RTC_WUT_TRG     (16U) /*!< LPBAM LPDMA1 HW Trigger signal is RTC_WUT_TRG     */
#define LPBAM_LPDMA1_TRIGGER_ADC4_AWD1       (17U) /*!< LPBAM LPDMA1 HW Trigger signal is ADC4_AWD1       */
#define LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF  (18U) /*!< LPBAM LPDMA1 HW Trigger signal is LPDMA1_CH0_TCF  */
#define LPBAM_LPDMA1_TRIGGER_LPDMA1_CH1_TCF  (19U) /*!< LPBAM LPDMA1 HW Trigger signal is LPDMA1_CH1_TCF  */
#define LPBAM_LPDMA1_TRIGGER_LPDMA1_CH2_TCF  (20U) /*!< LPBAM LPDMA1 HW Trigger signal is LPDMA1_CH2_TCF  */
#define LPBAM_LPDMA1_TRIGGER_LPDMA1_CH3_TCF  (21U) /*!< LPBAM LPDMA1 HW Trigger signal is LPDMA1_CH3_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH0_TCF  (22U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH0_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH1_TCF  (23U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH1_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH4_TCF  (24U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH4_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH5_TCF  (25U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH5_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH6_TCF  (26U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH6_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH7_TCF  (27U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH7_TCF  */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH12_TCF (28U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH12_TCF */
#define LPBAM_LPDMA1_TRIGGER_GPDMA1_CH13_TCF (29U) /*!< LPBAM LPDMA1 HW Trigger signal is GPDMA1_CH13_TCF */
#define LPBAM_LPDMA1_TRIGGER_TIM2_TRGO       (30U) /*!< LPBAM LPDMA1 HW Trigger signal is TIM2_TRGO       */
#define LPBAM_LPDMA1_TRIGGER_TIM15_TRGO      (31U) /*!< LPBAM LPDMA1 HW Trigger signal is TIM15_TRGO      */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Trigger_Polarity LPBAM DMA Trigger Polarity
  * @{
  */
#define LPBAM_DMA_TRIG_POLARITY_MASKED  0x00000000U        /*!< No trigger of the selected DMA request. Masked trigger event                                */
#define LPBAM_DMA_TRIG_POLARITY_RISING  DMA_CTR2_TRIGPOL_0 /*!< Trigger of the selected DMA request on the rising edge of the selected trigger event input  */
#define LPBAM_DMA_TRIG_POLARITY_FALLING DMA_CTR2_TRIGPOL_1 /*!< Trigger of the selected DMA request on the falling edge of the selected trigger event input */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Trigger_Mode LPBAM DMA Trigger Mode
  * @{
  */
#define LPBAM_DMA_TRIGM_BLOCK_TRANSFER          0x00000000U      /*!< A block transfer is conditioned by (at least) one hit trigger          */
#define LPBAM_DMA_TRIGM_REPEATED_BLOCK_TRANSFER DMA_CTR2_TRIGM_0 /*!< A repeated block transfer is conditioned by (at least) one hit trigger */
#define LPBAM_DMA_TRIGM_LLI_LINK_TRANSFER       DMA_CTR2_TRIGM_1 /*!< A LLI link transfer is conditioned by (at least) one hit trigger       */
#define LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER   DMA_CTR2_TRIGM   /*!< A single/burst transfer is conditioned by (at least) one hit trigger   */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Transfer_Event_Mode LPBAM DMA Transfer Event Mode
  * @{
  */
#define LPBAM_DMA_TCEM_BLOCK_TRANSFER          0x00000000U     /*!< The TC event is generated at the end of each block
                                                                    and the HT event is generated at the half of each
                                                                    block                                             */
#define LPBAM_DMA_TCEM_REPEATED_BLOCK_TRANSFER DMA_CTR2_TCEM_0 /*!< The TC event is generated at the end of the repeated
                                                                    block and the HT event is generated at the half of
                                                                    the repeated block                                */
#define LPBAM_DMA_TCEM_EACH_LL_ITEM_TRANSFER   DMA_CTR2_TCEM_1 /*!< The TC event is generated at the end of each
                                                                    linked-list item and the HT event is generated at
                                                                    the half of each linked-list item                 */
#define LPBAM_DMA_TCEM_LAST_LL_ITEM_TRANSFER   DMA_CTR2_TCEM   /*!< The TC event is generated at the end of the last
                                                                    linked-list item and the HT event is generated at
                                                                    the half of the last linked-list item             */
/**
  * @}
  */

/** @defgroup LPBAM_DMA_Queue_Type LPBAM DMA Queue Type
  * @{
  */
#define LPBAM_DMA_STATIC_Q  QUEUE_TYPE_STATIC  /*!< Static DMA queue  */
#define LPBAM_DMA_DYNAMIC_Q QUEUE_TYPE_DYNAMIC /*!< Dynamic DMA queue */
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

#ifdef __cplusplus
}
#endif

#endif /* STM32_PLATFORM_LPBAM_COMMON_H */
