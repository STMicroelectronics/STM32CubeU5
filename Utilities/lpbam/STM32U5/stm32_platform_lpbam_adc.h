/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_adc.h
  * @author  MCD Application Team
  * @brief   header file provides the LPBAM ADC defines
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
#ifndef STM32_PLATFORM_LPBAM_ADC_H
#define STM32_PLATFORM_LPBAM_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup ADC_Platform
  * @brief      ADC Platform LPBAM module driver
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup LPBAM_ADC_Platform_Exported_Types LPBAM ADC Platform Exported Types
  * @brief    LPBAM ADC Platform Exported Types
  * @{
  */
/**
  * @brief LPBAM ADC Analog WatchDog Structure definition.
  */
typedef struct
{
  uint32_t WatchdogNumber;      /*!< Select which ADC analog watchdog is monitoring the selected channel.
                                     For Analog Watchdog 1: Only 1 channel can be monitored (or overall group of
                                     channels by setting parameter 'WatchdogMode').
                                     For Analog Watchdog 2 and 3: Several channels can be monitored
                                     This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Number.           */

  uint32_t WatchdogMode;        /*!< Configure the ADC analog watchdog mode: single/all/none channels.
                                     For Analog Watchdog 1 : Configure the ADC analog watchdog mode: single channel
                                                             or all channels.
                                     For Analog Watchdog 2 and 3 : Several channels can be monitored by a combination
                                                                   of several channels in the AWD initialization
                                                                   structure.
                                     Channels on ADC group regular and injected are not differentiated :
                                     Set value 'LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG' to monitor 1 or several channels,
                                     value 'LPBAM_ADC_ANALOGWATCHDOG_ALL_REG' to monitor all channels, value
                                     'LPBAM_ADC_ANALOGWATCHDOG_NONE' to monitor no channel.
                                     This parameter can be a value of @ref LPBAM_ADC_AnalogWatchDog_Mode.
                                     Note: For Analog Watchdog 2 and 3 and when using
                                           'LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG' several channels can be monitored by
                                           combine several channels.                                                  */

  uint32_t Channel;             /*!< Select which ADC channel to monitor by analog watchdog.
                                     For Analog Watchdog 1: this parameter has an effect only if parameter
                                     'WatchdogMode' is configured on single channel (only 1 channel can be monitored).
                                     For Analog Watchdog 2 and 3: Several channels can be monitored.
                                     This parameter can be a value of @ref LPBAM_ADC_Channel_Selection.
                                     Note: for Analog Watchdog 2 and 3, a combination of channels is possible.        */

  LPBAM_FunctionalState ITMode; /*!< Specify whether the analog watchdog is configured in interrupt or polling mode.
                                     This parameter can be set to ENABLE or DISABLE                                   */

  uint32_t LowThreshold;        /*!< Specifies the ADC watchdog low threshold.
                                     This parameter can be a value from 0 to 0xFFF                                    */

  uint32_t HighThreshold;       /*!< Specifies the ADC watchdog high threshold.
                                     This parameter can be a value from 0 to 0xFFF                                    */
} LPBAM_ADC_AnalogWDGConfig_t;

/**
  * @brief LPBAM ADC Channel configuration Structure definition.
  */
typedef struct
{
  uint32_t Channel; /*!< Specify the channel to configure into ADC regular group.
                         This parameter can be a value of @ref LPBAM_ADC_Channel_Selection. */

  uint32_t Rank;    /*!< Specify the rank in the regular group sequencer.
                         This parameter is discarded when the ScanConvMode field in LPBAM_ADC_ConfigAdvConf_t or
                         LPBAM_ADC_FullAdvConf_t is configured as LPBAM_ADC_SCAN_DIRECTION_FORWARD or
                         LPBAM_ADC_SCAN_DIRECTION_BACKWARD.
                         This parameter can be a value of @ref LPBAM_ADC_Channel_RANK.      */

} LPBAM_ADC_ChannelConfig_t;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_ADC_Platform_Exported_Constants LPBAM ADC Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_ADC_Sequence_Scan_Mode LPBAM_ADC_Sequence_Scan_Mode
  * @{
  */
#define LPBAM_ADC_SCAN_DISABLE            (0x00U) /*!< LPBAM Scan mode disabled                                       */
#define LPBAM_ADC_SCAN_ENABLE             (0x01U) /*!< LPBAM channels order and sequencer length are configurable
                                                       (Sequencer configured to fully configurable)                   */
#define LPBAM_ADC_SCAN_DIRECTION_FORWARD  (0x02U) /*!< LPBAM channels order is defined by channel index.
                                                       Scan direction forward: from channel 0 to channel 23 (Sequencer
                                                       configured to not fully configurable)                          */
#define LPBAM_ADC_SCAN_DIRECTION_BACKWARD (0x03U) /*!< LPBAM channels order is defined by channel index.
                                                       Scan direction backward: from channel 23 to channel 0 (Sequencer
                                                       configured to not fully configurable)                          */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Channel_Selection LPBAM ADC Channel Selection
  * @{
  */
/* Channel differentiation between external and internal channels */
#define CHANNEL_ID_INTERNAL_CH         (0x80000000UL) /*!< Dummy bit: Marker of internal channel.                     */
#define CHANNEL_DIFF_DAC1CH1_DAC1CH2   (0x01000000UL) /*!< Dummy bit for driver internal usage, not used in ADC channel
                                                           setting registers CHSELR or SQRx.                          */
#define LPBAM_ADC_CHANNEL_0          (0x00000001U) /*!< LPBAM ADC Channel 0   */
#define LPBAM_ADC_CHANNEL_1          (0x00000002U) /*!< LPBAM ADC Channel 1   */
#define LPBAM_ADC_CHANNEL_2          (0x00000004U) /*!< LPBAM ADC Channel 2   */
#define LPBAM_ADC_CHANNEL_3          (0x00000008U) /*!< LPBAM ADC Channel 3   */
#define LPBAM_ADC_CHANNEL_4          (0x00000010U) /*!< LPBAM ADC Channel 4   */
#define LPBAM_ADC_CHANNEL_5          (0x00000020U) /*!< LPBAM ADC Channel 5   */
#define LPBAM_ADC_CHANNEL_6          (0x00000040U) /*!< LPBAM ADC Channel 6   */
#define LPBAM_ADC_CHANNEL_7          (0x00000080U) /*!< LPBAM ADC Channel 7   */
#define LPBAM_ADC_CHANNEL_8          (0x00000100U) /*!< LPBAM ADC Channel 8   */
#define LPBAM_ADC_CHANNEL_9          (0x00000200U) /*!< LPBAM ADC Channel 9   */
#define LPBAM_ADC_CHANNEL_10         (0x00000400U) /*!< LPBAM ADC Channel 10  */
#define LPBAM_ADC_CHANNEL_11         (0x00000800U) /*!< LPBAM ADC Channel 11  */
#define LPBAM_ADC_CHANNEL_12         (0x00001000U) /*!< LPBAM ADC Channel 12  */
#define LPBAM_ADC_CHANNEL_13         (0x00002000U) /*!< LPBAM ADC Channel 13  */
#define LPBAM_ADC_CHANNEL_14         (0x00004000U) /*!< LPBAM ADC Channel 14  */
#define LPBAM_ADC_CHANNEL_15         (0x00008000U) /*!< LPBAM ADC Channel 15  */
#define LPBAM_ADC_CHANNEL_16         (0x00010000U) /*!< LPBAM ADC Channel 16  */
#define LPBAM_ADC_CHANNEL_17         (0x00020000U) /*!< LPBAM ADC Channel 17  */
#define LPBAM_ADC_CHANNEL_18         (0x00040000U) /*!< LPBAM ADC Channel 18  */
#define LPBAM_ADC_CHANNEL_19         (0x00080000U) /*!< LPBAM ADC Channel 19  */
#define LPBAM_ADC_CHANNEL_20         (0x00100000U) /*!< LPBAM ADC Channel 20  */
#define LPBAM_ADC_CHANNEL_21         (0x00200000U) /*!< LPBAM ADC Channel 21  */
#define LPBAM_ADC_CHANNEL_22         (0x00400000U) /*!< LPBAM ADC Channel 22  */
#define LPBAM_ADC_CHANNEL_23         (0x00800000U) /*!< LPBAM ADC Channel 23  */

#define LPBAM_ADC_CHANNEL_VREFINT    (LPBAM_ADC_CHANNEL_0  | CHANNEL_ID_INTERNAL_CH) /*!< LPBAM ADC internal channel
                                                                                          connected to VrefInt        */
#define LPBAM_ADC_CHANNEL_VCORE      (LPBAM_ADC_CHANNEL_12 | CHANNEL_ID_INTERNAL_CH) /*!< LPBAM ADC internal channel
                                                                                          connected to Vcore          */
#define LPBAM_ADC_CHANNEL_TEMPSENSOR (LPBAM_ADC_CHANNEL_13 | CHANNEL_ID_INTERNAL_CH) /*!< LPBAM ADC internal channel
                                                                                          connected to Temperature
                                                                                          sensor                      */
#define LPBAM_ADC_CHANNEL_VBAT       (LPBAM_ADC_CHANNEL_14 | CHANNEL_ID_INTERNAL_CH) /*!< LPBAM ADC internal channel
                                                                                          connected to Vbat/4: Vbat
                                                                                          voltage through a divider
                                                                                          ladder of factor 1/4 to be
                                                                                          always below Vdda           */
#define LPBAM_ADC_CHANNEL_DAC1CH1    (LPBAM_ADC_CHANNEL_21 | CHANNEL_ID_INTERNAL_CH) /*!< LPBAM ADC internal channel
                                                                                          connected to DAC1 channel 1 */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Channel_RANK LPBAM ADC Channel RANK
  * @{
  */
#define LPBAM_ADC_REGULAR_RANK_1 (0x00U) /*!< LPBAM ADC group regular sequencer rank 1 */
#define LPBAM_ADC_REGULAR_RANK_2 (0x04U) /*!< LPBAM ADC group regular sequencer rank 2 */
#define LPBAM_ADC_REGULAR_RANK_3 (0x08U) /*!< LPBAM ADC group regular sequencer rank 3 */
#define LPBAM_ADC_REGULAR_RANK_4 (0x0CU) /*!< LPBAM ADC group regular sequencer rank 4 */
#define LPBAM_ADC_REGULAR_RANK_5 (0x10U) /*!< LPBAM ADC group regular sequencer rank 5 */
#define LPBAM_ADC_REGULAR_RANK_6 (0x14U) /*!< LPBAM ADC group regular sequencer rank 6 */
#define LPBAM_ADC_REGULAR_RANK_7 (0x18U) /*!< LPBAM ADC group regular sequencer rank 7 */
#define LPBAM_ADC_REGULAR_RANK_8 (0x1CU) /*!< LPBAM ADC group regular sequencer rank 8 */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Trigger_Selection LPBAM ADC Trigger Selection
  * @{
  */
#define LPBAM_ADC_EXTERNALTRIG_T1_TRGO2   (0x00U) /*!< LPBAM ADC conversion trigger from external IP: TIM1 TRGO2       */
#define LPBAM_ADC_EXTERNALTRIG_T1_CC4     (0x01U) /*!< LPBAM ADC conversion trigger from external IP: TIM1 channel 4   */
#define LPBAM_ADC_EXTERNALTRIG_T2_TRGO    (0x02U) /*!< LPBAM ADC conversion trigger from external IP: TIM2 TRGO        */
#define LPBAM_ADC_EXTERNALTRIG_T15_TRGO   (0x03U) /*!< LPBAM ADC conversion trigger from external IP: TIM15 TRGO       */
#define LPBAM_ADC_EXTERNALTRIG_T6_TRGO    (0x04U) /*!< LPBAM ADC conversion trigger from external IP: TIM6 TRGO        */
#define LPBAM_ADC_EXTERNALTRIG_LPTIM1_CH1 (0x05U) /*!< LPBAM ADC conversion trigger from external IP: LPTIM1 channel 1 */
#define LPBAM_ADC_EXTERNALTRIG_LPTIM3_CH2 (0x06U) /*!< LPBAM ADC conversion trigger from external IP: LPTIM3 channel 2 */
#define LPBAM_ADC_EXTERNALTRIG_EXT_IT15   (0x07U) /*!< LPBAM ADC conversion trigger from external IP: EXTI line 15     */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Trigger_Polarity LPBAM ADC Trigger Polarity
  * @{
  */
#define LPBAM_ADC_EXTERNALTRIGCONVEDGE_NONE          (0x00U) /*!< LPBAM ADC Regular conversions hardware trigger detection disabled                        */
#define LPBAM_ADC_EXTERNALTRIGCONVEDGE_RISING        (0x01U) /*!< LPBAM ADC group regular conversion trigger polarity set to rising edge                   */
#define LPBAM_ADC_EXTERNALTRIGCONVEDGE_FALLING       (0x02U) /*!< LPBAM ADC group regular conversion trigger polarity set to falling edge                  */
#define LPBAM_ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING (0x03U) /*!< LPBAM ADC group regular conversion trigger polarity set to both rising and falling edges */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Trigger_LowFrequency LPBAM ADC Trigger LowFrequency
  * @{
  */
#define LPBAM_ADC_TRIGGER_FREQ_HIGH (0x00U) /*!< LPBAM ADC trigger frequency mode set to high frequency */
#define LPBAM_ADC_TRIGGER_FREQ_LOW  (0x01U) /*!< LPBAM ADC trigger frequency mode set to low frequency  */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Clock_Prescaler LPBAM ADC Clock Prescaler
  * @{
  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV1   (0x00U) /*!< LPBAM ADC asynchronous clock without prescaler              */
#define LPBAM_ADC_CLOCK_ASYNC_DIV2   (0x01U) /*!< LPBAM ADC asynchronous clock with prescaler division by 2   */
#define LPBAM_ADC_CLOCK_ASYNC_DIV4   (0x02U) /*!< LPBAM ADC asynchronous clock with prescaler division by 4   */
#define LPBAM_ADC_CLOCK_ASYNC_DIV6   (0x03U) /*!< LPBAM ADC asynchronous clock with prescaler division by 6   */
#define LPBAM_ADC_CLOCK_ASYNC_DIV8   (0x04U) /*!< LPBAM ADC asynchronous clock with prescaler division by 8   */
#define LPBAM_ADC_CLOCK_ASYNC_DIV10  (0x05U) /*!< LPBAM ADC asynchronous clock with prescaler division by 10  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV12  (0x06U) /*!< LPBAM ADC asynchronous clock with prescaler division by 12  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV16  (0x07U) /*!< LPBAM ADC asynchronous clock with prescaler division by 16  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV32  (0x08U) /*!< LPBAM ADC asynchronous clock with prescaler division by 32  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV64  (0x09U) /*!< LPBAM ADC asynchronous clock with prescaler division by 64  */
#define LPBAM_ADC_CLOCK_ASYNC_DIV128 (0x0AU) /*!< LPBAM ADC asynchronous clock with prescaler division by 128 */
#define LPBAM_ADC_CLOCK_ASYNC_DIV256 (0x0BU) /*!< LPBAM ADC asynchronous clock with prescaler division by 256 */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_AnalogWatchDog_Number LPBAM ADC AnalogWatchDog Number
  * @{
  */
#define LPBAM_ADC_ANALOGWATCHDOG_1 (0x01U) /*!< LPBAM ADC analog watchdog 1 */
#define LPBAM_ADC_ANALOGWATCHDOG_2 (0x02U) /*!< LPBAM ADC analog watchdog 2 */
#define LPBAM_ADC_ANALOGWATCHDOG_3 (0x04U) /*!< LPBAM ADC analog watchdog 3 */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_AnalogWatchDog_Mode LPBAM ADC AnalogWatchDog Mode
  * @{
  */
#define LPBAM_ADC_ANALOGWATCHDOG_NONE       (0x00U) /*!< LPBAM No analog watchdog selected                               */
#define LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG (0x01U) /*!< LPBAM Analog watchdog applied to a regular group single channel */
#define LPBAM_ADC_ANALOGWATCHDOG_ALL_REG    (0x02U) /*!< LPBAM Analog watchdog applied to regular group all channels     */
/**
  * @}
  */

/** @defgroup LPBAM_ADC_Wakeup_Interrupt LPBAM ADC Wakeup Interrupt
  * @{
  */
#define LPBAM_ADC_IT_NONE (0x00U)          /*!< LPBAM ADC wake up Interrupt None                                          */
#define LPBAM_ADC_IT_AWD1 (ADC_IER_AWD1IE) /*!< LPBAM ADC Analog watchdog 1 interrupt source (main analog watchdog)       */
#define LPBAM_ADC_IT_AWD2 (ADC_IER_AWD2IE) /*!< LPBAM ADC Analog watchdog 2 interrupt source (additional analog watchdog) */
#define LPBAM_ADC_IT_AWD3 (ADC_IER_AWD3IE) /*!< ADC Analog watchdog 3 interrupt source (additional analog watchdog)       */
/**
  * @}
  */

/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_ADC_Platform_Private_Constants LPBAM ADC Platform Private Constants
  * @{
  */
#define LPBAM_ADC_ADSTP                  ADC_CR_ADSTP          /*!< ADC Stop of Regular conversion */
#define LPBAM_ADC_ADSTART                ADC_CR_ADSTART        /*!< ADC Start of Regular conversion */
#define LPBAM_ADC_CONT                   ADC_CFGR1_CONT        /*!< ADC Single/continuous conversion mode for regular conversion */
#define LPBAM_ADC_DISCEN                 ADC_CFGR1_DISCEN      /*!< ADC Discontinuous mode for regular channels */
#define LPBAM_ADC_SCANDIR                ADC4_CFGR1_SCANDIR    /*!< ADC group regular sequencer scan direction */
#define LPBAM_ADC_CHSELRMOD              ADC4_CFGR1_CHSELRMOD  /*!< ADC JSQR Queue mode */
#define LPBAM_ADC_WAIT                   ADC4_CFGR1_WAIT       /*!< ADC Delayed conversion mode */
#define LPBAM_ADC_AWD1SGL                ADC_CFGR1_AWD1SGL     /*!< Enable the watchdog 1 on a single channel or on all channels */
#define LPBAM_ADC_AWD1EN                 ADC_CFGR1_AWD1EN      /*!< ADC Analog watchdog 1 enable on regular Channels */
#define LPBAM_ADC_AWD1CH_POS             ADC_CFGR1_AWD1CH_Pos
#define LPBAM_ADC_AWD1CH                 ADC_CFGR1_AWD1CH      /*!< ADC Analog watchdog 1 Channel selection */
#define LPBAM_ADC_DMACFG                 ADC4_CFGR1_DMACFG     /*!< ADC DMA transfer configuration */
#define LPBAM_ADC_EXTSEL                 ADC4_CFGR1_EXTSEL     /*!< ADC External trigger selection for regular group */
#define LPBAM_ADC_EXTEN                  ADC_CFGR1_EXTEN       /*!< ADC External trigger enable and polarity selection for regular channels */
#define LPBAM_ADC_EXTSEL_POS             ADC4_CFGR1_EXTSEL_Pos
#define LPBAM_ADC_EXTEN_POS              ADC_CFGR1_EXTEN_Pos
#define LPBAM_ADC_LFTRIG                 ADC4_CFGR2_LFTRIG     /*!< ADC4 low frequency trigger mode */
#define LPBAM_ADC_LFTRIG_POS             ADC4_CFGR2_LFTRIG_Pos
#define LPBAM_ADC_HT1_POS                ADC_AWD1TR_HT1_Pos
#define LPBAM_ADC_ADEN                   ADC_CR_ADEN           /*!< ADC Enable control */
#define LPBAM_ADC_DMAEN                  ADC4_CFGR1_DMAEN      /*!< ADC DMA transfer enable */
#define LPBAM_ADC_VREFEN_POS             ADC_CCR_VREFEN_Pos
#define LPBAM_ADC_VSENSEEN_POS           ADC_CCR_VSENSEEN_Pos
#define LPBAM_ADC_VBATEN_POS             ADC_CCR_VBATEN_Pos
#define LPBAM_ADC_CHANNEL_RANK_DEFAULT   (0x0FUL)              /*!< ADC Channel Rank default */
#define LPBAM_ADC_AWD_DEFAULT_VALUE      (0x0FFF0000UL)
#define LPBAM_ADC_ANALOGWATCHDOG_DEFAULT (0U)
#define LPBAM_ADC_ANALOGWATCHDOG_ALL_CH  (0xFFFFFFUL)          /*!< ADC Analog watchdog all channels selected */
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

#endif /* STM32_PLATFORM_LPBAM_ADC_H */
