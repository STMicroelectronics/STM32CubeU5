/**
  ******************************************************************************
  * @file    DMA/DMA_LinkedList/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_ll_pwr.h"
#include "stm32u5xx_ll_bus.h"
#include "stm32u5xx_ll_rcc.h"
#include "stm32u5xx_ll_system.h"
#include "stm32u5xx_ll_utils.h"
#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_dma.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of DMA clock */
#define DMA_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ALL)

/* Definition for DMA NVIC */
#define DMA_IRQn         GPDMA1_Channel13_IRQn
#define DMA_IRQHandler   GPDMA1_Channel13_IRQHandler

/* Definition of buffer sizes */
#define BUFFER1_SIZE     (8U)
#define BUFFER2_SIZE     (16U)
#define BUFFER3_SIZE     (24U)

#define DEFAULT_REGISTER_UPDATE LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 | \
                                LL_DMA_UPDATE_CBR1 | LL_DMA_UPDATE_CSAR | \
                                LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CLLR

/* GREEN_LED and RED_LED defines */
#define SUCCESS_LED_PIN               LL_GPIO_PIN_7
#define SUCCESS_LED_GPIO_PORT         GPIOC
#define SUCCESS_LED_GPIO_CLK_ENABLE() LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)

#define ERROR_LED_PIN                 LL_GPIO_PIN_2
#define ERROR_LED_GPIO_PORT           GPIOG
#define ERROR_LED_GPIO_CLK_ENABLE()   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* MAIN_H */
