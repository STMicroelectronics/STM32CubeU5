/**
  **********************************************************************************************************************
  * @file    stm32_ll_lpbam.h
  * @author  MCD Application Team
  * @brief   header for the stm32_ll_lpbam.c file
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
#ifndef STM32_LL_LPBAM_H
#define STM32_LL_LPBAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

#if defined (LPBAM_ADC_MODULE_ENABLED)
/*
 *        ######## LPBAM ADC defines ########
 */
#define LPBAM_ADC_STATE_OFFSET               (0x0008U)
#define LPBAM_ADC_STATE_DATASIZE             (0x0004U)

#define LPBAM_ADC_CONFIG_OFFSET              (0x000CU)
#define LPBAM_ADC_CONFIG_DATASIZE            (0x0004U)

#define LPBAM_ADC_CHANNELSELECTION_OFFSET    (0x0028U)
#define LPBAM_ADC_CHANNELSELECTION_DATASIZE  (0x0004U)

#define LPBAM_ADC_AWDG_THESHOLD_OFFSET       (0x0020U)
#define LPBAM_ADC_AWDG_THESHOLD_DATASIZE     (0x0004U)

#define LPBAM_ADC_AWDG_MONITOR_OFFSET        (0x00A0U)
#define LPBAM_ADC_AWDG_MONITOR_DATASIZE      (0x0004U)

#define LPBAM_ADC_TRIG_LOWFREQUENCY_OFFSET   (0x0010U)
#define LPBAM_ADC_TRIG_LOWFREQUENCY_DATASIZE (0x0004U)

#define LPBAM_ADC_CLKPRESCALER_OFFSET        (0x0308U)
#define LPBAM_ADC_CLKPRESCALER_DATASIZE      (0x0004U)

#define LPBAM_ADC_CLEARFLAG_OFFSET           (0x0000U)
#define LPBAM_ADC_CLEARFLAG_DATASIZE         (0x0004U)

#define LPBAM_ADC_WAKEUP_IT_OFFSET           (0x0004U)
#define LPBAM_ADC_WAKEUP_IT_DATASIZE         (0x0004U)

#define LPBAM_ADC_DATA_OFFSET                (0x0040U)

#define LPBAM_ADC_AWDG1_THRESHOLD_OFFSET     (0x0000U)
#define LPBAM_ADC_AWDG2_THRESHOLD_OFFSET     (0x0004U)
#define LPBAM_ADC_AWDG3_THRESHOLD_OFFSET     (0x000CU)

#define LPBAM_ADC_AWDG2_MONITOR_OFFSET       (0x0000U)
#define LPBAM_ADC_AWDG3_MONITOR_OFFSET       (0x0004U)
#endif /* defined (LPBAM_ADC_MODULE_ENABLED) */


#if defined (LPBAM_COMP_MODULE_ENABLED)
/*
 *        ######## LPBAM COMP defines ########
 */
#define LPBAM_COMP_CONFIG_OFFSET     (0x00000000U)
#define LPBAM_COMP_CONFIG_DATASIZE   (0x00000004U)

#define LPBAM_COMP_READOUTPUT_OFFSET (0x00000000U)

#define LPBAM_COMP_CONFIG_MASK       (0xFFFF4800U)
#endif /* defined (LPBAM_COMP_MODULE_ENABLED) */


#if defined (LPBAM_DAC_MODULE_ENABLED)
/*
 *        ######## LPBAM DAC defines ########
 */
#define LPBAM_DAC_CONFIG_OFFSET   (0x00U)
#define LPBAM_DAC_CONFIG_DATASIZE (0x04U)

#define LPBAM_DAC_DATA_OFFSET     (0x08U)

#define LPBAM_DAC_CHANNEL_OFFSET  (0x0CU)
#endif /* defined (LPBAM_DAC_MODULE_ENABLED) */


#if defined (LPBAM_DMA_MODULE_ENABLED)
/*
 *        ######## LPBAM DMA defines ########
 */
#define LPBAM_DMA_CONFIG_OFFSET                (0x14U)
#define LPBAM_DMA_CONFIG_DATASIZE              (0x04U)

#define LPBAM_DMA_QUEUE_BASEADDRESS_OFFSET     (0x00U)
#define LPBAM_DMA_QUEUE_BASEADDRESS_DATASIZE   (0x04U)

#define LPBAM_DMA_QUEUE_OFFSETADDRESS_OFFSET   (0x7CU)
#define LPBAM_DMA_QUEUE_OFFSETADDRESS_DATASIZE (0x04U)

#define LPBAM_DMA_CLEARDATASIZE_OFFSET         (0x48U)
#define LPBAM_DMA_CLEARDATASIZE_DATASIZE       (0x04U)

#define LPBAM_DMA_CLEARFLAG_OFFSET             (0x0CU)
#define LPBAM_DMA_CLEARFLAG_DATASIZE           (0x04U)
#endif /* defined (LPBAM_DMA_MODULE_ENABLED) */


#if defined (LPBAM_GPIO_MODULE_ENABLED)
/*
 *        ######## LPBAM GPIO defines ########
 */
#define LPBAM_GPIO_READPIN_OFFSET    (0x10U)
#define LPBAM_GPIO_READPIN_DATASIZE  (0x04U)

#define LPBAM_GPIO_WRITEPIN_OFFSET   (0x18U)
#define LPBAM_GPIO_WRITEPIN_DATASIZE (0x04U)
#endif /* defined (LPBAM_GPIO_MODULE_ENABLED) */


#if defined (LPBAM_I2C_MODULE_ENABLED)
/*
 *        ######## LPBAM I2C defines ########
 */
#define LPBAM_I2C_STATE_OFFSET                (0x00U)
#define LPBAM_I2C_STATE_DATASIZE              (0x04U)

#define LPBAM_I2C_CONFIG_TRANSACTION_OFFSET   (0x04U)
#define LPBAM_I2C_CONFIG_TRANSACTION_DATASIZE (0x04U)

#define LPBAM_I2C_TIMING_OFFSET               (0x10U)
#define LPBAM_I2C_TIMING_DATASIZE             (0x04U)

#define LPBAM_I2C_TRANSMIT_DATA_OFFSET        (0x28U)
#define LPBAM_I2C_RECEIVE_DATA_OFFSET         (0x24U)

#define LPBAM_I2C_TRIG_OFFSET                 (0x2CU)
#define LPBAM_I2C_TRIG_DATASIZE               (0x04U)
#endif /* defined (LPBAM_I2C_MODULE_ENABLED) */


#if defined (LPBAM_LPTIM_MODULE_ENABLED)
/*
 *        ######## LPBAM LPTIM defines ########
 */
#define LPBAM_LPTIM_CONFIG_OFFSET              (0x10U)
#define LPBAM_LPTIM_CONFIG_DATASIZE            (0x01U)

#define LPBAM_LPTIM_CONFIG_PERIOD_OFFSET       (0x18U)
#define LPBAM_LPTIM_CONFIG_PERIOD_DATASIZE     (0x02U)

#define LPBAM_LPTIM_CONFIG_PULSE_OFFSET        (0x14U)
#define LPBAM_LPTIM_CONFIG_PULSE_DATASIZE      (0x02U)

#define LPBAM_LPTIM_CONFIG_REPETITION_OFFSET   (0x28U)
#define LPBAM_LPTIM_CONFIG_REPETITION_DATASIZE (0x01U)

#define LPBAM_LPTIM_GET_COUNTER_OFFSET         (0x1CU)

#define LPBAM_LPTIM_INPUT_CAPTURE_OFFSET       (0x14U)

#define LPBAM_LPTIM_WAKEUP_IT_OFFSET           (0x08U)
#define LPBAM_LPTIM_WAKEUP_IT_DATASIZE         (0x04U)

#define LPBAM_LPTIM_CLEARFLAG_OFFSET           (0x04U)
#define LPBAM_LPTIM_CLEARFLAG_DATASIZE         (0x04U)

#define LPBAM_LPTIM_CHANNEL_OFFSET             (0x20U)
#endif /* defined (LPBAM_LPTIM_MODULE_ENABLED) */


#if defined (LPBAM_OPAMP_MODULE_ENABLED)
/*
 *        ######## LPBAM OPAMP defines ########
 */
#define LPBAM_OPAMP_CONFIG_OFFSET   (0x0000U)
#define LPBAM_OPAMP_CONFIG_DATASIZE (0x0004U)

#define LPBAM_OPAMP_CONFIG_MASK     (0x073DU)
#endif /* defined (LPBAM_OPAMP_MODULE_ENABLED) */


#if defined (LPBAM_UART_MODULE_ENABLED)
/*
 *        ######## LPBAM UART defines ########
 */
#define LPBAM_UART_CONFIG_OFFSET           (0x00U)
#define LPBAM_UART_CONFIG_DATASIZE         (0x04U)

#define LPBAM_UART_AUTONOMOUSMODE_OFFSET   (0x30U)
#define LPBAM_UART_AUTONOMOUSMODE_DATASIZE (0x04U)

#define LPBAM_UART_RECEIVE_OFFSET          (0x24U)
#define LPBAM_UART_TRANSMIT_OFFSET         (0x28U)

#define LPBAM_UART_CLEARFLAG_OFFSET        (0x20U)
#define LPBAM_UART_CLEARFLAG_DATASIZE      (0x04U)

#define LPBAM_UART_FIFO_IT_POS             (0x1AU)
#endif /* defined (LPBAM_UART_MODULE_ENABLED) */


#if defined (LPBAM_VREFBUF_MODULE_ENABLED)
/*
 *        ######## LPBAM VREFBUF defines ########
 */
#define LPBAM_VREFBUF_MODE_OFFSET   (0x00U)
#define LPBAM_VREFBUF_MODE_DATASIZE (0x04U)

#define LPBAM_VREFBUF_CSR_MASK      (0x70U)
#endif /* defined (LPBAM_VREFBUF_MODULE_ENABLED) */


#if defined (LPBAM_SPI_MODULE_ENABLED)
/*
 *        ######## LPBAM SPI defines ########
 */
#define LPBAM_SPI_STATE_OFFSET          (0x0000U)
#define LPBAM_SPI_STATE_DATASIZE        (0x0004U)

#define LPBAM_SPI_DATASIZE_OFFSET       (0x0004U)
#define LPBAM_SPI_DATASIZE_DATASIZE     (0x0004U)

#define LPBAM_SPI_CONFIG_FRAME_OFFSET   (0x0008U)
#define LPBAM_SPI_CONFIG_FRAME_DATASIZE (0x0004U)

#define LPBAM_SPI_CONFIG_COM_OFFSET     (0x000CU)
#define LPBAM_SPI_CONFIG_COM_DATASIZE   (0x0004U)

#define LPBAM_SPI_WAKEUP_IT_OFFSET      (0x0010U)
#define LPBAM_SPI_WAKEUP_IT_DATASIZE    (0x0004U)

#define LPBAM_SPI_START_OFFSET          (0x0000U)
#define LPBAM_SPI_START_DATASIZE        (0x0004U)

#define LPBAM_SPI_TRIG_OFFSET           (0x001CU)
#define LPBAM_SPI_TRIG_DATASIZE         (0x0004U)

#define LPBAM_SPI_RECEIVE_DATA_OFFSET   (0x0030U)
#define LPBAM_SPI_TRANSMIT_DATA_OFFSET  (0x0020U)

#define LPBAM_SPI_CLEARFLAG_OFFSET      (0x0018U)
#define LPBAM_SPI_CLEARFLAG_DATASIZE    (0x0004U)

#define LPBAM_SPI_CLEARFLAG_MASK        (0x0BF8U)
#endif /* defined (LPBAM_SPI_MODULE_ENABLED) */


#if defined (LPBAM_ADC_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for ADC peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_ADC_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_ADC_FillStructInfo(LPBAM_ADC_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo);
#endif /* defined (LPBAM_ADC_MODULE_ENABLED) */


#if defined (LPBAM_COMP_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for COMP peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_COMP_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_COMP_FillStructInfo(LPBAM_COMP_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo);
#endif /* defined (LPBAM_COMP_MODULE_ENABLED) */


#if defined (LPBAM_DAC_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for DAC peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_DAC_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_DAC_FillStructInfo(LPBAM_DAC_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo);
#endif /* defined (LPBAM_DAC_MODULE_ENABLED) */


#if defined (LPBAM_DMA_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for DMA peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_DMA_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_DMA_FillStructInfo(LPBAM_DMA_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo);
#endif /* defined (LPBAM_DMA_MODULE_ENABLED) */


#if defined (LPBAM_GPIO_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for GPIO peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_GPIO_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_GPIO_FillStructInfo(LPBAM_GPIO_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo);
#endif /* defined (LPBAM_GPIO_MODULE_ENABLED) */


#if defined (LPBAM_I2C_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for I2C peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_I2C_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_I2C_FillStructInfo(LPBAM_I2C_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo);
#endif /* defined (LPBAM_I2C_MODULE_ENABLED) */


#if defined (LPBAM_LPTIM_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for LPTIM peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_LPTIM_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_LPTIM_FillStructInfo(LPBAM_LPTIM_ConfNode_t const *const pConfNode,
                                          LPBAM_InfoDesc_t       *const pDescInfo);
#endif /* defined (LPBAM_LPTIM_MODULE_ENABLED) */


#if defined (LPBAM_OPAMP_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for OPAMP peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_OPAMP_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_OPAMP_FillStructInfo(LPBAM_OPAMP_ConfNode_t const *const pConfNode,
                                          LPBAM_InfoDesc_t       *const pDescInfo);
#endif /* defined (LPBAM_OPAMP_MODULE_ENABLED) */


#if defined (LPBAM_SPI_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for SPI peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_SPI_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_SPI_FillStructInfo(LPBAM_SPI_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo);
#endif /* defined (LPBAM_SPI_MODULE_ENABLED) */


#if defined (LPBAM_UART_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for UART peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_UART_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_UART_FillStructInfo(LPBAM_UART_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo);
#endif /* defined (LPBAM_UART_MODULE_ENABLED) */


#if defined (LPBAM_VREFBUF_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for VREFBUF peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_VREFBUF_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_VREFBUF_FillStructInfo(LPBAM_VREFBUF_ConfNode_t const *const pConfNode,
                                            LPBAM_InfoDesc_t         *const pDescInfo);
#endif /* defined (LPBAM_VREFBUF_MODULE_ENABLED) */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* STM32_LL_LPBAM_H */
