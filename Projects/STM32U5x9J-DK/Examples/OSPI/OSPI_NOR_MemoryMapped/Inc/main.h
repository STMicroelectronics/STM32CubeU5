/**
  ******************************************************************************
  * @file    OSPI/OSPI_NOR_MemoryMapped/Inc/main.h
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5x9j_discovery.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for OSPI Pins */
/* OSPI_CLK */
#define OSPI_CLK_PIN                      GPIO_PIN_10
#define OSPI_CLK_GPIO_PORT                GPIOF
#define OSPI_CLK_PIN_AF                   GPIO_AF3_OCTOSPI1
/* OSPI_DQS */
#define OSPI_DQS_PIN                      GPIO_PIN_1
#define OSPI_DQS_GPIO_PORT                GPIOA
#define OSPI_DQS_PIN_AF                   GPIO_AF10_OCTOSPI1
/* OSPI_CS */
#define OSPI_CS_PIN                       GPIO_PIN_2
#define OSPI_CS_GPIO_PORT                 GPIOA
#define OSPI_CS_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D0 */
#define OSPI_D0_PIN                       GPIO_PIN_8
#define OSPI_D0_GPIO_PORT                 GPIOF
#define OSPI_D0_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D1 */
#define OSPI_D1_PIN                       GPIO_PIN_9
#define OSPI_D1_GPIO_PORT                 GPIOF
#define OSPI_D1_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D2 */
#define OSPI_D2_PIN                       GPIO_PIN_7
#define OSPI_D2_GPIO_PORT                 GPIOF
#define OSPI_D2_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D3 */
#define OSPI_D3_PIN                       GPIO_PIN_6
#define OSPI_D3_GPIO_PORT                 GPIOF
#define OSPI_D3_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D4 */
#define OSPI_D4_PIN                       GPIO_PIN_1
#define OSPI_D4_GPIO_PORT                 GPIOC
#define OSPI_D4_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D5 */
#define OSPI_D5_PIN                       GPIO_PIN_2
#define OSPI_D5_GPIO_PORT                 GPIOC
#define OSPI_D5_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D6 */
#define OSPI_D6_PIN                       GPIO_PIN_3
#define OSPI_D6_GPIO_PORT                 GPIOC
#define OSPI_D6_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D7 */
#define OSPI_D7_PIN                       GPIO_PIN_0
#define OSPI_D7_GPIO_PORT                 GPIOC
#define OSPI_D7_PIN_AF                    GPIO_AF3_OCTOSPI1


/* MX25LM512ABA1G12 Macronix memory */
/* Size of the flash */
#define OSPI_FLASH_SIZE             26
#define OSPI_PAGE_SIZE              256

/* Flash commands */
#define OCTAL_IO_READ_CMD           0xEC13
#define OCTAL_PAGE_PROG_CMD         0x12ED
#define OCTAL_READ_STATUS_REG_CMD   0x05FA
#define OCTAL_SECTOR_ERASE_CMD      0x21DE
#define OCTAL_WRITE_ENABLE_CMD      0x06F9
#define READ_STATUS_REG_CMD         0x05
#define WRITE_CFG_REG_2_CMD         0x72
#define WRITE_ENABLE_CMD            0x06

/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ     6
#define DUMMY_CLOCK_CYCLES_READ_REG 4

/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    0x02
#define WRITE_ENABLE_MASK_VALUE     0x02

#define MEMORY_READY_MATCH_VALUE    0x00
#define MEMORY_READY_MASK_VALUE     0x01

#define AUTO_POLLING_INTERVAL       0x10

/* Memory registers address */
#define CONFIG_REG2_ADDR1           0x0000000
#define CR2_STR_OPI_ENABLE          0x01

#define CONFIG_REG2_ADDR3           0x00000300
#define CR2_DUMMY_CYCLES_66MHZ      0x07

/* Memory delay */
#define MEMORY_REG_WRITE_DELAY      40
#define MEMORY_PAGE_PROG_DELAY      10

/* End address of the OSPI memory */
#define OSPI_END_ADDR               (1 << OSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFERSIZE                  (COUNTOF(aTxBuffer)-1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()                 __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()                __HAL_RCC_OSPI1_CLK_DISABLE()

#define OSPI_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()

#define OSPI_FORCE_RESET()                __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()              __HAL_RCC_OSPI1_RELEASE_RESET()

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */


