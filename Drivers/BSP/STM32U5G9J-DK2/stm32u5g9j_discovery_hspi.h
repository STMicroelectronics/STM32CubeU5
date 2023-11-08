/**
  ******************************************************************************
  * @file    stm32u5g9j_discovery_hspi.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u5g9j_discovery_hspi.c driver.
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
#ifndef STM32U5G9J_DISCOVERY_HSPI_H
#define STM32U5G9J_DISCOVERY_HSPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5g9j_discovery_conf.h"
#include "stm32u5g9j_discovery_errno.h"
#include "../Components/mx66uw1g45g/mx66uw1g45g.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5G9J_DK2
  * @{
  */

/** @addtogroup STM32U5G9J_DK2_HSPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32U5G9J_DK2_HSPI_Exported_Types HSPI Exported Types
  * @{
  */
typedef enum
{
  HSPI_ACCESS_NONE = 0,          /*!<  Instance not initialized,              */
  HSPI_ACCESS_INDIRECT,          /*!<  Instance use indirect mode access      */
  HSPI_ACCESS_MMP                /*!<  Instance use Memory Mapped Mode read   */
} HSPI_Access_t;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pXSPI_CallbackTypeDef  pMspInitCb;
  pXSPI_CallbackTypeDef  pMspDeInitCb;
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

/** @defgroup STM32U5G9J_DK2_HSPI_NOR_Exported_Types HSPI NOR Exported Types
  * @{
  */
#define BSP_HSPI_NOR_Info_t                MX66UW1G45G_Info_t
#define BSP_HSPI_NOR_Interface_t           MX66UW1G45G_Interface_t
#define BSP_HSPI_NOR_Transfer_t            MX66UW1G45G_Transfer_t
#define BSP_HSPI_NOR_Erase_t               MX66UW1G45G_Erase_t

typedef struct
{
  HSPI_Access_t              IsInitialized;  /*!<  Instance access Flash method     */
  BSP_HSPI_NOR_Interface_t   InterfaceMode;  /*!<  Flash Interface mode of Instance */
  BSP_HSPI_NOR_Transfer_t    TransferRate;   /*!<  Flash Transfer mode of Instance  */
} HSPI_NOR_Ctx_t;

typedef struct
{
  BSP_HSPI_NOR_Interface_t   InterfaceMode;      /*!<  Current Flash Interface mode */
  BSP_HSPI_NOR_Transfer_t    TransferRate;       /*!<  Current Flash Transfer rate  */
} BSP_HSPI_NOR_Init_t;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32U5G9J_DK2_HSPI_Exported_Constants HSPI Exported Constants
  * @{
  */

/* Definition for HSPI NOR clock resources */
#define HSPI_NOR_CLK_ENABLE()                 __HAL_RCC_HSPI1_CLK_ENABLE()
#define HSPI_NOR_CLK_DISABLE()                __HAL_RCC_HSPI1_CLK_DISABLE()

#define HSPI_NOR_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOI_CLK_ENABLE()
#define HSPI_NOR_DQS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D5_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D6_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define HSPI_NOR_D7_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()

#define HSPI_NOR_FORCE_RESET()                __HAL_RCC_HSPI1_FORCE_RESET()
#define HSPI_NOR_RELEASE_RESET()              __HAL_RCC_HSPI1_RELEASE_RESET()

/* Definition for NOR HSPI Pins */
/* HSPI_CLK */
#define HSPI_NOR_CLK_PIN                      GPIO_PIN_3
#define HSPI_NOR_CLK_GPIO_PORT                GPIOI
#define HSPI_NOR_CLK_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_DQS */
#define HSPI_NOR_DQS_PIN                      GPIO_PIN_2
#define HSPI_NOR_DQS_GPIO_PORT                GPIOI
#define HSPI_NOR_DQS_PIN_AF                   GPIO_AF8_HSPI1
/* HSPI_CS */
#define HSPI_NOR_CS_PIN                       GPIO_PIN_9
#define HSPI_NOR_CS_GPIO_PORT                 GPIOH
#define HSPI_NOR_CS_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D0 */
#define HSPI_NOR_D0_PIN                       GPIO_PIN_10
#define HSPI_NOR_D0_GPIO_PORT                 GPIOH
#define HSPI_NOR_D0_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D1 */
#define HSPI_NOR_D1_PIN                       GPIO_PIN_11
#define HSPI_NOR_D1_GPIO_PORT                 GPIOH
#define HSPI_NOR_D1_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D2 */
#define HSPI_NOR_D2_PIN                       GPIO_PIN_12
#define HSPI_NOR_D2_GPIO_PORT                 GPIOH
#define HSPI_NOR_D2_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D3 */
#define HSPI_NOR_D3_PIN                       GPIO_PIN_13
#define HSPI_NOR_D3_GPIO_PORT                 GPIOH
#define HSPI_NOR_D3_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D4 */
#define HSPI_NOR_D4_PIN                       GPIO_PIN_14
#define HSPI_NOR_D4_GPIO_PORT                 GPIOH
#define HSPI_NOR_D4_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D5 */
#define HSPI_NOR_D5_PIN                       GPIO_PIN_15
#define HSPI_NOR_D5_GPIO_PORT                 GPIOH
#define HSPI_NOR_D5_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D6 */
#define HSPI_NOR_D6_PIN                       GPIO_PIN_0
#define HSPI_NOR_D6_GPIO_PORT                 GPIOI
#define HSPI_NOR_D6_PIN_AF                    GPIO_AF8_HSPI1
/* HSPI_D7 */
#define HSPI_NOR_D7_PIN                       GPIO_PIN_1
#define HSPI_NOR_D7_GPIO_PORT                 GPIOI
#define HSPI_NOR_D7_PIN_AF                    GPIO_AF8_HSPI1

/**
  * @}
  */

/** @defgroup STM32U5G9J_DK2_HSPI_NOR_Exported_Constants HSPI NOR Exported Constants
  * @{
  */
#define HSPI_NOR_INSTANCES_NUMBER         1U

/* Definition for HSPI modes */
#define BSP_HSPI_NOR_SPI_MODE (BSP_HSPI_NOR_Interface_t)MX66UW1G45G_SPI_MODE /* 1 Cmd, 1 Address and 1 Data Lines */
#define BSP_HSPI_NOR_OPI_MODE (BSP_HSPI_NOR_Interface_t)MX66UW1G45G_OPI_MODE /* 8 Cmd, 8 Address and 8 Data Lines */

/* Definition for HSPI transfer rates */
#define BSP_HSPI_NOR_STR_TRANSFER  (BSP_HSPI_NOR_Transfer_t)MX66UW1G45G_STR_TRANSFER   /* Single Transfer Rate */
#define BSP_HSPI_NOR_DTR_TRANSFER  (BSP_HSPI_NOR_Transfer_t)MX66UW1G45G_DTR_TRANSFER   /* Double Transfer Rate */

/* HSPI erase types */
#define BSP_HSPI_NOR_ERASE_4K             MX66UW1G45G_ERASE_4K
#define BSP_HSPI_NOR_ERASE_64K            MX66UW1G45G_ERASE_64K
#define BSP_HSPI_NOR_ERASE_CHIP           MX66UW1G45G_ERASE_BULK

/* HSPI block sizes */
#define BSP_HSPI_NOR_BLOCK_4K             MX66UW1G45G_SUBSECTOR_4K
#define BSP_HSPI_NOR_BLOCK_64K            MX66UW1G45G_SECTOR_64K
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup STM32U5G9J_DK2_HSPI_NOR_Exported_Functions HSPI NOR Exported Functions
  * @{
  */
int32_t BSP_HSPI_NOR_Init(uint32_t Instance, BSP_HSPI_NOR_Init_t *Init);
int32_t BSP_HSPI_NOR_DeInit(uint32_t Instance);
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1U)
int32_t BSP_HSPI_NOR_RegisterMspCallbacks(uint32_t Instance, BSP_HSPI_Cb_t *CallBacks);
int32_t BSP_HSPI_NOR_RegisterDefaultMspCallbacks(uint32_t Instance);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */
int32_t BSP_HSPI_NOR_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
int32_t BSP_HSPI_NOR_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int32_t BSP_HSPI_NOR_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_HSPI_NOR_Erase_t BlockSize);
int32_t BSP_HSPI_NOR_Erase_Chip(uint32_t Instance);
int32_t BSP_HSPI_NOR_GetStatus(uint32_t Instance);
int32_t BSP_HSPI_NOR_GetInfo(uint32_t Instance, BSP_HSPI_NOR_Info_t *pInfo);
int32_t BSP_HSPI_NOR_EnableMemoryMappedMode(uint32_t Instance);
int32_t BSP_HSPI_NOR_DisableMemoryMappedMode(uint32_t Instance);
int32_t BSP_HSPI_NOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_HSPI_NOR_ConfigFlash(uint32_t Instance, BSP_HSPI_NOR_Interface_t Mode, BSP_HSPI_NOR_Transfer_t Rate);
int32_t BSP_HSPI_NOR_SuspendErase(uint32_t Instance);
int32_t BSP_HSPI_NOR_ResumeErase(uint32_t Instance);
int32_t BSP_HSPI_NOR_EnterDeepPowerDown(uint32_t Instance);
int32_t BSP_HSPI_NOR_LeaveDeepPowerDown(uint32_t Instance);

/**
  * @}
  */
/** @defgroup STM32U5G9J_DK2_HSPI_Exported_Init_Functions HSPI Exported Init Functions
  * @{
  */

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_HSPI_NOR_Init(XSPI_HandleTypeDef *hhspi, MX_HSPI_InitTypeDef *Init);
HAL_StatusTypeDef MX_HSPI_ClockConfig(XSPI_HandleTypeDef *hhspi);


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

#endif /* STM32U5G9J_DISCOVERY_HSPI_H */
