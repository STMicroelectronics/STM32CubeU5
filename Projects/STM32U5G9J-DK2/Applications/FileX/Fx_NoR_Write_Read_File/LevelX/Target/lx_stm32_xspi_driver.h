/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef LX_STM32_XSPI_DRIVER_H
#define LX_STM32_XSPI_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* USER CODE BEGIN Includes */
#include "stm32u5xx_hal.h"
#include "mx66uw1g45g.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* The following semaphore is being to notify about RX/TX completion.
It needs to be released in the transfer callbacks */
extern TX_SEMAPHORE xspi_rx_semaphore;
extern TX_SEMAPHORE xspi_tx_semaphore;

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* the XSPI instance, default value set to 0 */
#define LX_STM32_XSPI_INSTANCE                           0
#define LX_STM32_XSPI_BASE_ADDRESS                       0

#define LX_STM32_XSPI_DEFAULT_TIMEOUT                    10 * TX_TIMER_TICKS_PER_SECOND

#define LX_STM32_DEFAULT_SECTOR_SIZE                     LX_STM32_XSPI_SECTOR_SIZE
#define LX_STM32_XSPI_DMA_API                            1

/* when set to 1 LevelX is initializing the XSPI memory,
 * otherwise it is the up to the application to perform it.
 */
#define LX_STM32_XSPI_INIT                               0

/* allow the driver to fully erase the OctoSPI chip. This should be used carefully.
 * the call is blocking and takes a while. by default it is set to 0.
 */
#define LX_STM32_XSPI_ERASE                              0

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LX_STM32_XSPI_CURRENT_TIME                              tx_time_get

/* Macro called after initializing the XSPI driver
 * e.g. create a semaphore used for transfer notification */
 /* USER CODE BEGIN LX_STM32_XSPI_POST_INIT */

#define LX_STM32_XSPI_POST_INIT()                        do { \
                                                         if (tx_semaphore_create(&xspi_rx_semaphore, "xspi rx transfer semaphore", 0) != TX_SUCCESS) \
                                                         { \
                                                           return LX_ERROR; \
                                                         } \
                                                         if (tx_semaphore_create(&xspi_tx_semaphore, "xspi tx transfer semaphore", 0) != TX_SUCCESS) \
                                                         { \
                                                           return LX_ERROR; \
                                                         } \
                                                        } while(0)
/* USER CODE END LX_STM32_XSPI_POST_INIT */

/* Macro called before performing read operation */

/* USER CODE BEGIN LX_STM32_XSPI_PRE_READ_TRANSFER */

#define LX_STM32_XSPI_PRE_READ_TRANSFER(__status__)

/* USER CODE END LX_STM32_XSPI_PRE_READ_TRANSFER */

/* Define how to notify about Read completion operation */

/* USER CODE BEGIN LX_STM32_XSPI_READ_CPLT_NOTIFY */

#define LX_STM32_XSPI_READ_CPLT_NOTIFY(__status__)      do { \
                                                          if(tx_semaphore_get(&xspi_rx_semaphore, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != TX_SUCCESS) \
                                                          { \
                                                            __status__ = LX_ERROR; \
                                                          } \
                                                        } while(0)

/* USER CODE END LX_STM32_XSPI_READ_CPLT_NOTIFY */

/* Macro called after performing read operation */

/* USER CODE BEGIN LX_STM32_XSPI_POST_READ_TRANSFER */

#define LX_STM32_XSPI_POST_READ_TRANSFER(__status__)

/* USER CODE END LX_STM32_XSPI_POST_READ_TRANSFER */

/* Macro for read error handling */
/* USER CODE BEGIN LX_STM32_XSPI_READ_TRANSFER_ERROR */

#define LX_STM32_XSPI_READ_TRANSFER_ERROR(__status__)

/* USER CODE END LX_STM32_XSPI_READ_TRANSFER_ERROR */

/* Macro called before performing write operation */

/* USER CODE BEGIN LX_STM32_XSPI_PRE_WRITE_TRANSFER */

#define LX_STM32_XSPI_PRE_WRITE_TRANSFER(__status__)

/* USER CODE END LX_STM32_XSPI_PRE_WRITE_TRANSFER */

/* Define how to notify about write completion operation */

/* USER CODE BEGIN LX_STM32_XSPI_WRITE_CPLT_NOTIFY */

#define LX_STM32_XSPI_WRITE_CPLT_NOTIFY(__status__)     do { \
                                                          if(tx_semaphore_get(&xspi_tx_semaphore, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != TX_SUCCESS) \
                                                          { \
                                                            __status__ = LX_ERROR; \
                                                          } \
                                                        } while(0)

/* USER CODE END LX_STM32_XSPI_WRITE_CPLT_NOTIFY */

/* Macro called after performing write operation */

/* USER CODE BEGIN LX_STM32_XSPI_POST_WRITE_TRANSFER */

#define LX_STM32_XSPI_POST_WRITE_TRANSFER(__status__)

/* USER CODE END LX_STM32_OSPI_POST_WRITE_TRANSFER */

/* Macro for write error handling */

#define LX_STM32_XSPI_WRITE_TRANSFER_ERROR(__status__)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
INT lx_stm32_xspi_lowlevel_init(UINT instance);
INT lx_stm32_xspi_lowlevel_deinit(UINT instance);

INT lx_stm32_xspi_get_status(UINT instance);
INT lx_stm32_xspi_get_info(UINT instance, ULONG *block_size, ULONG *total_blocks);

INT lx_stm32_xspi_read(UINT instance, ULONG *address, ULONG *buffer, ULONG words);
INT lx_stm32_xspi_write(UINT instance, ULONG *address, ULONG *buffer, ULONG words);

INT lx_stm32_xspi_erase(UINT instance, ULONG block, ULONG erase_count, UINT full_chip_erase);
INT lx_stm32_xspi_is_block_erased(UINT instance, ULONG block);

UINT lx_xspi_driver_system_error(UINT error_code);

UINT lx_stm32_xspi_initialize(LX_NOR_FLASH *nor_flash);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LX_STM32_XSPI_DUMMY_CYCLES_READ_OCTAL     20
#define LX_STM32_XSPI_DUMMY_CYCLES_CR_CFG         MX66UW1G45G_CR2_DC_6_CYCLES

#define LX_STM32_XSPI_SECTOR_SIZE                 MX66UW1G45G_BLOCK_64K
#define LX_STM32_XSPI_FLASH_SIZE                  MX66UW1G45G_FLASH_SIZE
#define LX_STM32_XSPI_PAGE_SIZE                   MX66UW1G45G_PAGE_SIZE

#define LX_STM32_XSPI_BULK_ERASE_MAX_TIME         MX66UW1G45G_BULK_ERASE_MAX_TIME
#define LX_STM32_XSPI_SECTOR_ERASE_MAX_TIME       MX66UW1G45G_SECTOR_ERASE_MAX_TIME
#define LX_STM32_XSPI_WRITE_REG_MAX_TIME          MX66UW1G45G_WRITE_REG_MAX_TIME

#define LX_STM32_XSPI_OCTAL_BULK_ERASE_CMD        MX66UW1G45G_OCTA_BULK_ERASE_CMD
#define LX_STM32_XSPI_OCTAL_SECTOR_ERASE_CMD      MX66UW1G45G_OCTA_BLOCK_ERASE_64K_CMD

#define LX_STM32_XSPI_WRITE_ENABLE_CMD            MX66UW1G45G_WRITE_ENABLE_CMD
#define LX_STM32_XSPI_WRITE_CFG_REG2_CMD          MX66UW1G45G_WRITE_CFG_REG2_CMD
#define LX_STM32_XSPI_OCTAL_WRITE_ENABLE_CMD      MX66UW1G45G_OCTA_WRITE_ENABLE_CMD
#define LX_STM32_XSPI_OCTAL_WRITE_CFG_REG2_CMD    MX66UW1G45G_OCTA_WRITE_CFG_REG2_CMD

#define LX_STM32_XSPI_READ_STATUS_REG_CMD         MX66UW1G45G_READ_STATUS_REG_CMD
#define LX_STM32_XSPI_READ_CFG_REG2_CMD           MX66UW1G45G_WRITE_CFG_REG2_CMD

#define LX_STM32_XSPI_OCTAL_READ_DTR_CMD          MX66UW1G45G_OCTA_READ_DTR_CMD
#define LX_STM32_XSPI_OCTAL_READ_CFG_REG2_CMD     MX66UW1G45G_OCTA_READ_CFG_REG2_CMD
#define LX_STM32_XSPI_OCTAL_READ_STATUS_REG_CMD   MX66UW1G45G_OCTA_READ_STATUS_REG_CMD

#define LX_STM32_XSPI_RESET_ENABLE_CMD            MX66UW1G45G_RESET_ENABLE_CMD
#define LX_STM32_XSPI_RESET_MEMORY_CMD            MX66UW1G45G_RESET_MEMORY_CMD

#define LX_STM32_XSPI_OCTAL_PAGE_PROG_CMD         MX66UW1G45G_OCTA_PAGE_PROG_CMD

#define LX_STM32_XSPI_CR2_REG3_ADDR               MX66UW1G45G_CR2_REG3_ADDR
#define LX_STM32_XSPI_CR2_REG1_ADDR               MX66UW1G45G_CR2_REG1_ADDR
#define LX_STM32_XSPI_SR_WEL                      MX66UW1G45G_SR_WEL
#define LX_STM32_XSPI_SR_WIP                      MX66UW1G45G_SR_WIP
#define LX_STM32_XSPI_CR2_SOPI                    MX66UW1G45G_CR2_SOPI
#define LX_STM32_XSPI_CR2_DOPI                    MX66UW1G45G_CR2_DOPI

/* USER CODE END PD */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
#ifdef __cplusplus
}
#endif
#endif /* LX_STM32_XSPI_DRIVER_H */

