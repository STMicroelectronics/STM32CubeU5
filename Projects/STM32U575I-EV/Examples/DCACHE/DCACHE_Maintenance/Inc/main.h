/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u575i_eval.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BUFFER_SIZE           160   /* 40 Cache Lines */
#define REGION_SIZE             0x4000000 /*Size of external SRAM Bank */

/* MPU attribute setting */
#define  DEVICE_nGnRnE          0x0U  /* Device, noGather, noReorder, noEarly acknowledge. */
#define  DEVICE_nGnRE           0x4U  /* Device, noGather, noReorder, Early acknowledge.   */
#define  DEVICE_nGRE            0x8U  /* Device, noGather, Reorder, Early acknowledge.     */
#define  DEVICE_GRE             0xCU  /* Device, Gather, Reorder, Early acknowledge.       */

#define  WRITE_THROUGH          0x0U  /* Normal memory, write-through. */
#define  NOT_CACHEABLE          0x4U  /* Normal memory, non-cacheable. */
#define  WRITE_BACK             0x4U  /* Normal memory, write-back.    */

#define  TRANSIENT              0x0U  /* Normal memory, transient.     */
#define  NON_TRANSIENT          0x8U  /* Normal memory, non-transient. */

#define  NO_ALLOCATE            0x0U  /* Normal memory, no allocate.         */
#define  W_ALLOCATE             0x1U  /* Normal memory, write allocate.      */
#define  R_ALLOCATE             0x2U  /* Normal memory, read allocate.       */
#define  RW_ALLOCATE            0x3U  /* Normal memory, read/write allocate. */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define OUTER(__ATTR__)        ((__ATTR__) << 4U)
#define INNER_OUTER(__ATTR__)  ((__ATTR__) | ((__ATTR__) << 4U))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Error_Handler(void);
void MPU_Config_WT(void);
void MPU_Config_WB_WA(void);
void MPU_Config_WB_nWA(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
//#define MPU_WRITE_THROUGH_ATTRIBUTE
#define MPU_WRITE_BACK_WRITE_ALLOCATE_ATTRIBUTE
//#define MPU_WRITE_BACK_NO_WRITE_ALLOCATE_ATTRIBUTE

#define SRAM_BANK_ADDR    ((uint32_t)0x60000000)/*External SRAM start address*/

#define SRAM_MEMORY_WIDTH FMC_NORSRAM_MEM_BUS_WIDTH_16/*SRAM Bus width*/

#define SRAM_TIMEOUT      ((uint32_t)0xFFFF)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
