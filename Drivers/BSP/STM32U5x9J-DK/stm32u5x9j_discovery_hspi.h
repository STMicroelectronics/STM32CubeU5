/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_hspi.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u5x9j_discovery_hspi.c driver.
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
#ifndef STM32U5x9J_DISCOVERY_HSPI_H
#define STM32U5x9J_DISCOVERY_HSPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_conf.h"
#include "stm32u5x9j_discovery_errno.h"
#include "../Components/aps512xx/aps512xx.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY_HSPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32U5x9J_DISCOVERY_HSPI_Exported_Types HSPI Exported Types
  * @{
  */
typedef enum
{
  HSPI_ACCESS_NONE = 0,          /*!<  Instance not initialized,              */
  HSPI_ACCESS_INDIRECT,          /*!<  Instance use indirect mode access      */
  HSPI_ACCESS_MMP                /*!<  Instance use Memory Mapped Mode        */
} HSPI_Access_t;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pHSPI_CallbackTypeDef  pMspInitCb;
  pHSPI_CallbackTypeDef  pMspDeInitCb;
} BSP_HSPI_Cb_t;
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

typedef struct
{
  uint32_t MemorySize;
  uint32_t ClockPrescaler;
  uint32_t SampleShifting;
  uint32_t TransferRate;
} MX_HSPI_InitTypeDef;
/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Types HSPI RAM Exported Types
  * @{
  */
#define BSP_HSPI_RAM_BurstLength_t      APS512XX_BurstLength_t
#define BSP_HSPI_RAM_ReadLatencyCode_t  APS512XX_ReadLatencyCode_t
#define BSP_HSPI_RAM_WriteLatencyCode_t APS512XX_WriteLatencyCode_t

typedef enum
{
  BSP_HSPI_RAM_VARIABLE_LATENCY = 0,
  BSP_HSPI_RAM_FIXED_LATENCY    = APS512XX_MR0_LATENCY_TYPE
} BSP_HSPI_RAM_LatencyType_t;

typedef enum
{
  BSP_HSPI_RAM_LINEAR_BURST = 0,
  BSP_HSPI_RAM_HYBRID_BURST = APS512XX_MR8_BT
} BSP_HSPI_RAM_BurstType_t;

typedef enum
{
  BSP_HSPI_RAM_IO_X8_MODE  = 0,
  BSP_HSPI_RAM_IO_X16_MODE = APS512XX_MR8_X8_X16,
} BSP_HSPI_RAM_IOMode_t;

typedef struct
{
  HSPI_Access_t                   IsInitialized;    /*!< Instance access Flash method   */
  BSP_HSPI_RAM_LatencyType_t      LatencyType;      /*!< Latency Type of Instance       */
  BSP_HSPI_RAM_ReadLatencyCode_t  ReadLatencyCode;  /*!< Read Latency Code of Instance  */
  BSP_HSPI_RAM_WriteLatencyCode_t WriteLatencyCode; /*!< Write Latency Code of Instance */
  BSP_HSPI_RAM_BurstType_t        BurstType;        /*!< Burst Type of Instance         */
  BSP_HSPI_RAM_BurstLength_t      BurstLength;      /*!< Burst Length of Instance       */
  BSP_HSPI_RAM_IOMode_t           IOMode;           /*!< IO Mode of Instance            */
} HSPI_RAM_Ctx_t;

typedef struct
{
  BSP_HSPI_RAM_LatencyType_t      LatencyType;      /*!< Current 16bits-HSPI PSRAM Latency Type       */
  BSP_HSPI_RAM_ReadLatencyCode_t  ReadLatencyCode;  /*!< Current 16bits-HSPI PSRAM Read Latency Code  */
  BSP_HSPI_RAM_WriteLatencyCode_t WriteLatencyCode; /*!< Current 16bits-HSPI PSRAM Write Latency Code */
  BSP_HSPI_RAM_BurstType_t        BurstType;        /*!< Current 16bits-HSPI PSRAM Burst Type         */
  BSP_HSPI_RAM_BurstLength_t      BurstLength;      /*!< Current 16bits-HSPI PSRAM Burst Length       */
  BSP_HSPI_RAM_IOMode_t           IOMode;           /*!< Current 16bits-HSPI PSRAM IO Mode            */
} BSP_HSPI_RAM_Cfg_t;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32U5x9J_DISCOVERY_HSPI_Exported_Constants HSPI Exported Constants
  * @{
  */


/* Definition for HSPI clock resources */
#define HSPI_CLK_ENABLE()                 __HAL_RCC_HSPI1_CLK_ENABLE()
#define HSPI_CLK_DISABLE()                __HAL_RCC_HSPI1_CLK_DISABLE()

#define HSPI_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_NCLK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_DQS0_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_DQS1_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D5_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_D6_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D7_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D8_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D9_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D10_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D11_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D12_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D13_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D14_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_D15_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOJ_CLK_ENABLE()

#define HSPI_FORCE_RESET()                __HAL_RCC_HSPI1_FORCE_RESET()
#define HSPI_RELEASE_RESET()              __HAL_RCC_HSPI1_RELEASE_RESET()

/* Definition for HSPI Pins */
/* HSPI_CLK */
#define HSPI_CLK_PIN                      GPIO_PIN_3
#define HSPI_CLK_GPIO_PORT                GPIOI
#define HSPI_CLK_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_NCLK */
#define HSPI_NCLK_PIN                     GPIO_PIN_4
#define HSPI_NCLK_GPIO_PORT               GPIOI
#define HSPI_NCLK_PIN_AF                  GPIO_AF8_HSPI1
/* HSPI_DQS0 */
#define HSPI_DQS0_PIN                     GPIO_PIN_2
#define HSPI_DQS0_GPIO_PORT               GPIOI
#define HSPI_DQS0_PIN_AF                  GPIO_AF8_HSPI1
/* HSPI_DQS1 */
#define HSPI_DQS1_PIN                     GPIO_PIN_8
#define HSPI_DQS1_GPIO_PORT               GPIOI
#define HSPI_DQS1_PIN_AF                  GPIO_AF8_HSPI1
/* HSPI_CS */
#define HSPI_CS_PIN                       GPIO_PIN_9
#define HSPI_CS_GPIO_PORT                 GPIOH
#define HSPI_CS_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D0 */
#define HSPI_D0_PIN                       GPIO_PIN_10
#define HSPI_D0_GPIO_PORT                 GPIOH
#define HSPI_D0_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D1 */
#define HSPI_D1_PIN                       GPIO_PIN_11
#define HSPI_D1_GPIO_PORT                 GPIOH
#define HSPI_D1_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D2 */
#define HSPI_D2_PIN                       GPIO_PIN_12
#define HSPI_D2_GPIO_PORT                 GPIOH
#define HSPI_D2_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D3 */
#define HSPI_D3_PIN                       GPIO_PIN_13
#define HSPI_D3_GPIO_PORT                 GPIOH
#define HSPI_D3_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D4 */
#define HSPI_D4_PIN                       GPIO_PIN_14
#define HSPI_D4_GPIO_PORT                 GPIOH
#define HSPI_D4_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D5 */
#define HSPI_D5_PIN                       GPIO_PIN_15
#define HSPI_D5_GPIO_PORT                 GPIOH
#define HSPI_D5_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D6 */
#define HSPI_D6_PIN                       GPIO_PIN_0
#define HSPI_D6_GPIO_PORT                 GPIOI
#define HSPI_D6_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D7 */
#define HSPI_D7_PIN                       GPIO_PIN_1
#define HSPI_D7_GPIO_PORT                 GPIOI
#define HSPI_D7_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D8 */
#define HSPI_D8_PIN                       GPIO_PIN_9
#define HSPI_D8_GPIO_PORT                 GPIOI
#define HSPI_D8_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D9 */
#define HSPI_D9_PIN                       GPIO_PIN_10
#define HSPI_D9_GPIO_PORT                 GPIOI
#define HSPI_D9_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D10 */
#define HSPI_D10_PIN                      GPIO_PIN_11
#define HSPI_D10_GPIO_PORT                GPIOI
#define HSPI_D10_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_D11 */
#define HSPI_D11_PIN                      GPIO_PIN_12
#define HSPI_D11_GPIO_PORT                GPIOI
#define HSPI_D11_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_D12 */
#define HSPI_D12_PIN                      GPIO_PIN_13
#define HSPI_D12_GPIO_PORT                GPIOI
#define HSPI_D12_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_D13 */
#define HSPI_D13_PIN                      GPIO_PIN_14
#define HSPI_D13_GPIO_PORT                GPIOI
#define HSPI_D13_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_D14 */
#define HSPI_D14_PIN                      GPIO_PIN_15
#define HSPI_D14_GPIO_PORT                GPIOI
#define HSPI_D14_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_D15 */
#define HSPI_D15_PIN                      GPIO_PIN_0
#define HSPI_D15_GPIO_PORT                GPIOJ
#define HSPI_D15_PIN_AF                   GPIO_AF8_HSPI1

/**
  * @}
  */

/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Constants HSPI RAM Exported Constants
  * @{
  */
#define HSPI_RAM_INSTANCES_NUMBER         1U

/* HSPI Burst length */
#define BSP_HSPI_RAM_BURST_16_BYTES       (BSP_HSPI_RAM_BurstLength_t)APS512XX_BURST_16_BYTES
#define BSP_HSPI_RAM_BURST_32_BYTES       (BSP_HSPI_RAM_BurstLength_t)APS512XX_BURST_32_BYTES
#define BSP_HSPI_RAM_BURST_64_BYTES       (BSP_HSPI_RAM_BurstLength_t)APS512XX_BURST_64_BYTES
#define BSP_HSPI_RAM_BURST_2K_BYTES       (BSP_HSPI_RAM_BurstLength_t)APS512XX_BURST_2K_BYTES

/* HSPI Read Latency Code */
#define BSP_HSPI_RAM_READ_LATENCY_3       (BSP_HSPI_RAM_ReadLatencyCode_t)APS512XX_READ_LATENCY_3
#define BSP_HSPI_RAM_READ_LATENCY_4       (BSP_HSPI_RAM_ReadLatencyCode_t)APS512XX_READ_LATENCY_4
#define BSP_HSPI_RAM_READ_LATENCY_5       (BSP_HSPI_RAM_ReadLatencyCode_t)APS512XX_READ_LATENCY_5
#define BSP_HSPI_RAM_READ_LATENCY_6       (BSP_HSPI_RAM_ReadLatencyCode_t)APS512XX_READ_LATENCY_6
#define BSP_HSPI_RAM_READ_LATENCY_7       (BSP_HSPI_RAM_ReadLatencyCode_t)APS512XX_READ_LATENCY_7

/* HSPI Write Latency Code */
#define BSP_HSPI_RAM_WRITE_LATENCY_3      (BSP_HSPI_RAM_WriteLatencyCode_t)APS512XX_WRITE_LATENCY_3
#define BSP_HSPI_RAM_WRITE_LATENCY_4      (BSP_HSPI_RAM_WriteLatencyCode_t)APS512XX_WRITE_LATENCY_4
#define BSP_HSPI_RAM_WRITE_LATENCY_5      (BSP_HSPI_RAM_WriteLatencyCode_t)APS512XX_WRITE_LATENCY_5
#define BSP_HSPI_RAM_WRITE_LATENCY_6      (BSP_HSPI_RAM_WriteLatencyCode_t)APS512XX_WRITE_LATENCY_6
#define BSP_HSPI_RAM_WRITE_LATENCY_7      (BSP_HSPI_RAM_WriteLatencyCode_t)APS512XX_WRITE_LATENCY_7

/* DMA definitions for HSPI RAM DMA transfer */
#define HSPI_RAM_DMAx_CLK_ENABLE          __HAL_RCC_GPDMA1_CLK_ENABLE
#define HSPI_RAM_DMAx_CLK_DISABLE         __HAL_RCC_GPDMA1_CLK_DISABLE
#define HSPI_RAM_DMAx_CHANNEL             GPDMA1_Channel2
#define HSPI_RAM_DMAx_IRQn                GPDMA1_Channel2_IRQn
#define HPSI_RAM_DMAx_IRQHandler          GPDMA1_Channel2_IRQHandler
/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Variables HSPI RAM Exported Variables
  * @{
  */
extern XSPI_HandleTypeDef hhspi_ram[HSPI_RAM_INSTANCES_NUMBER];
extern HSPI_RAM_Ctx_t Hspi_Ram_Ctx[HSPI_RAM_INSTANCES_NUMBER];
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Functions HSPI RAM Exported Functions
  * @{
  */
int32_t BSP_HSPI_RAM_Init(uint32_t Instance, BSP_HSPI_RAM_Cfg_t *Init);
int32_t BSP_HSPI_RAM_DeInit(uint32_t Instance);
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
int32_t BSP_HSPI_RAM_RegisterMspCallbacks(uint32_t Instance, BSP_HSPI_Cb_t *CallBacks);
int32_t BSP_HSPI_RAM_RegisterDefaultMspCallbacks(uint32_t Instance);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */
int32_t BSP_HSPI_RAM_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t BSP_HSPI_RAM_Read_DMA(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t BSP_HSPI_RAM_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t BSP_HSPI_RAM_Write_DMA(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t BSP_HSPI_RAM_EnableMemoryMappedMode(uint32_t Instance);
int32_t BSP_HSPI_RAM_DisableMemoryMappedMode(uint32_t Instance);
int32_t BSP_HSPI_RAM_Config16BitsOctalRAM(uint32_t Instance, BSP_HSPI_RAM_Cfg_t *Cfg);
int32_t BSP_HSPI_RAM_EnterDeepPowerDown(uint32_t Instance);
int32_t BSP_HSPI_RAM_LeaveDeepPowerDown(uint32_t Instance);

void    BSP_HSPI_RAM_DMA_IRQHandler(uint32_t Instance);
void    BSP_HSPI_RAM_IRQHandler(uint32_t Instance);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_HSPI_RAM_Init(XSPI_HandleTypeDef *hhspi, MX_HSPI_InitTypeDef *Init);
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

#endif /* STM32U5x9J_DISCOVERY_HSPI_H */
