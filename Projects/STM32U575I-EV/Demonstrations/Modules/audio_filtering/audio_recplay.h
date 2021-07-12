/**
  ******************************************************************************
  * @file    audio_recplay.h
  * @author  MCD Application Team
  * @brief   Audio recorder/player interface header file
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
#ifndef __AUDIO_RECPLAY_H
#define __AUDIO_RECPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Audio defines */
#define AUDIO_FILE_ADDRESS   0x08080000
#define AUDIO_FILE_SIZE      (180 * 1024U)
#define PLAY_HEADER          0x2C
#define AUDIO_I2C_ADDRESS    0x94U
#define AUDIO_FREQUENCY_22K  22050U
#define AUDIO_FREQUENCY_44K  (2 * 22050U)

/* SAI peripheral configuration defines */
#define REC_BUFF_SIZE        (1024U)

#define AUDIO_SAIx                           SAI1_Block_B
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()

#define AUDIO_SAIx_FS_GPIO_PORT              GPIOF
#define AUDIO_SAIx_FS_AF                     GPIO_AF13_SAI1
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_9
#define AUDIO_SAIx_SCK_GPIO_PORT             GPIOF
#define AUDIO_SAIx_SCK_AF                    GPIO_AF13_SAI1
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_8
#define AUDIO_SAIx_SD_GPIO_PORT              GPIOF
#define AUDIO_SAIx_SD_AF                     GPIO_AF13_SAI1
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_SAIx_MCLK_GPIO_PORT            GPIOF
#define AUDIO_SAIx_MCLK_AF                   GPIO_AF13_SAI1
#define AUDIO_SAIx_MCLK_PIN                  GPIO_PIN_7

#define AUDIO_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_SCK_ENABLE()              __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_FS_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()
#define AUDIO_SAIx_SD_ENABLE()               __HAL_RCC_GPIOF_CLK_ENABLE()

/* MDF peripheral configuration defines */
#define AUDIO_MDFx_CLK_ENABLE()              __HAL_RCC_ADF1_CLK_ENABLE()

#define AUDIO_MDFx_CCK0_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_MDFx_SDIN0_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()

#define AUDIO_MDFx_CCK0_GPIO_PORT              GPIOB
#define AUDIO_MDFx_CCK0_AF                     GPIO_AF3_ADF1
#define AUDIO_MDFx_CCK0_PIN                    GPIO_PIN_3
#define AUDIO_MDFx_SDIN0_GPIO_PORT             GPIOB
#define AUDIO_MDFx_SDIN0_AF                    GPIO_AF3_ADF1
#define AUDIO_MDFx_SDIN0_PIN                   GPIO_PIN_4

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Audio_Recplay_Demo(void);
void Audio_Recplay_UserAction(uint8_t sel);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_RECPLAY_H */
