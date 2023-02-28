/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_bus.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u5x9j_discovery_bus.c driver.
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
#ifndef STM32U5x9J_DISCOVERY_BUS_H
#define STM32U5x9J_DISCOVERY_BUS_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32u5x9j_discovery_conf.h"

#if defined(BSP_USE_CMSIS_OS)
#include "cmsis_os.h"
#endif /* BSP_USE_CMSIS_OS */
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY_BUS
  * @{
  */
/** @defgroup STM32U5x9J_DISCOVERY_BUS_Exported_Types BUS Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS > 0)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS > 0) */

/**
  * @}
  */
/** @defgroup STM32U5x9J_DISCOVERY_BUS_Exported_Constants BUS Exported Constants
  * @{
  */
/* Definition for I2C5 clock resources */
#define BUS_I2C5                              I2C5

#define BUS_I2C5_CLK_ENABLE()                  __HAL_RCC_I2C5_CLK_ENABLE()
#define BUS_I2C5_CLK_DISABLE()                 __HAL_RCC_I2C5_CLK_DISABLE()

#define BUS_I2C5_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C5_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C5_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C5_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C5_FORCE_RESET()                 __HAL_RCC_I2C5_FORCE_RESET()
#define BUS_I2C5_RELEASE_RESET()               __HAL_RCC_I2C5_RELEASE_RESET()

/* Definition for I2C5 Pins */
#define BUS_I2C5_SCL_PIN                       GPIO_PIN_5
#define BUS_I2C5_SCL_GPIO_PORT                 GPIOH
#define BUS_I2C5_SCL_AF                        GPIO_AF2_I2C5

#define BUS_I2C5_SDA_PIN                       GPIO_PIN_4
#define BUS_I2C5_SDA_GPIO_PORT                 GPIOH
#define BUS_I2C5_SDA_AF                        GPIO_AF2_I2C5

#ifndef BUS_I2C5_FREQUENCY
#define BUS_I2C5_FREQUENCY  400000U /* Frequency of I2C5 = 400 KHz*/
#endif /* BUS_I2C5_FREQUENCY */

/* Definition for I2C4 clock resources */
#define BUS_I2C4                              I2C4

#define BUS_I2C4_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C4_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()

#define BUS_I2C4_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C4_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

#define BUS_I2C4_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C4_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

#define BUS_I2C4_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C4_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2C4 Pins */
#define BUS_I2C4_SCL_PIN                       GPIO_PIN_6
#define BUS_I2C4_SCL_GPIO_PORT                 GPIOB
#define BUS_I2C4_SCL_AF                        GPIO_AF5_I2C4

#define BUS_I2C4_SDA_PIN                       GPIO_PIN_7
#define BUS_I2C4_SDA_GPIO_PORT                 GPIOB
#define BUS_I2C4_SDA_AF                        GPIO_AF5_I2C4

#ifndef BUS_I2C4_FREQUENCY
#define BUS_I2C4_FREQUENCY  100000U /* Frequency of I2C4 = 400 KHz*/
#endif /* BUS_I2C4_FREQUENCY */

/* Definition for I2C2 clock resources */
#define BUS_I2C2                              I2C2

#define BUS_I2C2_CLK_ENABLE()                  __HAL_RCC_I2C2_CLK_ENABLE()
#define BUS_I2C2_CLK_DISABLE()                 __HAL_RCC_I2C2_CLK_DISABLE()

#define BUS_I2C2_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define BUS_I2C2_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOF_CLK_DISABLE()

#define BUS_I2C2_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define BUS_I2C2_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOF_CLK_DISABLE()

#define BUS_I2C2_FORCE_RESET()                 __HAL_RCC_I2C2_FORCE_RESET()
#define BUS_I2C2_RELEASE_RESET()               __HAL_RCC_I2C2_RELEASE_RESET()

/* Definition for I2C2 Pins */
#define BUS_I2C2_SCL_PIN                       GPIO_PIN_1
#define BUS_I2C2_SCL_GPIO_PORT                 GPIOF
#define BUS_I2C2_SCL_AF                        GPIO_AF4_I2C2

#define BUS_I2C2_SDA_PIN                       GPIO_PIN_0
#define BUS_I2C2_SDA_GPIO_PORT                 GPIOF
#define BUS_I2C2_SDA_AF                        GPIO_AF4_I2C2

#ifndef BUS_I2C2_FREQUENCY
#define BUS_I2C2_FREQUENCY  400000U /* Frequency of I2C2 = 400 KHz*/
#endif /* BUS_I2C2_FREQUENCY */

/* Definition for I2C3 clock resources */
#define BUS_I2C3                              I2C3

#define BUS_I2C3_CLK_ENABLE()                  __HAL_RCC_I2C3_CLK_ENABLE()
#define BUS_I2C3_CLK_DISABLE()                 __HAL_RCC_I2C3_CLK_DISABLE()

#define BUS_I2C3_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C3_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C3_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C3_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C3_FORCE_RESET()                 __HAL_RCC_I2C3_FORCE_RESET()
#define BUS_I2C3_RELEASE_RESET()               __HAL_RCC_I2C3_RELEASE_RESET()

/* Definition for I2C3 Pins */
#define BUS_I2C3_SCL_PIN                       GPIO_PIN_7
#define BUS_I2C3_SCL_GPIO_PORT                 GPIOH
#define BUS_I2C3_SCL_AF                        GPIO_AF4_I2C3

#define BUS_I2C3_SDA_PIN                       GPIO_PIN_8
#define BUS_I2C3_SDA_GPIO_PORT                 GPIOH
#define BUS_I2C3_SDA_AF                        GPIO_AF4_I2C3

#ifndef BUS_I2C3_FREQUENCY
#define BUS_I2C3_FREQUENCY  400000U /* Frequency of I2C3 = 400 KHz*/
#endif /* BUS_I2C3_FREQUENCY */

/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c2;
extern I2C_HandleTypeDef hbus_i2c3;
extern I2C_HandleTypeDef hbus_i2c4;
extern I2C_HandleTypeDef hbus_i2c5;

/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_BUS_Exported_Functions
  * @{
  */
int32_t BSP_I2C5_Init(void);
int32_t BSP_I2C5_DeInit(void);
int32_t BSP_I2C5_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_I2C4_Init(void);
int32_t BSP_I2C4_DeInit(void);
int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_I2C2_Init(void);
int32_t BSP_I2C2_DeInit(void);
int32_t BSP_I2C2_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_I2C3_Init(void);
int32_t BSP_I2C3_DeInit(void);
int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_GetTick(void);

#if (USE_HAL_I2C_REGISTER_CALLBACKS > 0)
int32_t BSP_I2C5_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C5_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
int32_t BSP_I2C4_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C4_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
int32_t BSP_I2C2_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C2_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
int32_t BSP_I2C3_RegisterDefaultMspCallbacks(void);
int32_t BSP_I2C3_RegisterMspCallbacks(BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
__weak HAL_StatusTypeDef MX_I2C5_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);
__weak HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);
__weak HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);
__weak HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef *hI2c, uint32_t timing);

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

#endif /* STM32U5x9J_DISCOVERY_BUS_H */

