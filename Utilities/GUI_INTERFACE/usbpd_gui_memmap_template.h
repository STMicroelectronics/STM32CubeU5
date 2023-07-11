/**
  ******************************************************************************
  * @file    usbpd_gui_memmap_template.h
  * @author  MCD Application Team
  * @brief   This file contains memory mapping configuration to be able to run
  *          Cube-Monitor-UCPD on embedded side.
  *          This file should be copied to the application folder and renamed
  *          to usbpd_gui_memmap.h. Definitions should be updated according to
  *          application configuration.
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

#ifndef __USBPD_GUI_MEMMAP_H_
#define __USBPD_GUI_MEMMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32xxxx.h"
#include "usbpd_def.h"

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_GUI
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/

#if defined(FLASH_PAGE_NB)
/* Following definitions should be adapted to used Flash configuration :
   INDEX_PAGE : represents the number of the page used for storing USBPD settings (usually the last page)
   ADDR_FLASH_LAST_PAGE : Flash address value of beginning of USBPD settings page
   ADDR_FLASH_PAGE_END : Flash address value of end of USBPD settings page
*/
#if defined(STM32L552xx) || defined(STM32L562xx)
#define INDEX_PAGE              (FLASH_PAGE_NB - 1U)                           /* Index of latest page     */
#else
#if defined (FLASH_OPTR_DBANK) || defined(FLASH_DBANK_SUPPORT)
#define INDEX_PAGE              ((FLASH_PAGE_NB * 2U) - 1U)                    /* Index of latest page     */
#else
#define INDEX_PAGE              (FLASH_PAGE_NB - 1U)                           /* Index of latest page     */
#endif /* FLASH_OPTR_DBANK || FLASH_DBANK_SUPPORT */
#endif /* STM32L552xx || STM32L562xx */
#define ADDR_FLASH_LAST_PAGE    (FLASH_BASE + (INDEX_PAGE * FLASH_PAGE_SIZE))  /* Base @ of latest page    */
#define ADDR_FLASH_PAGE_END     (ADDR_FLASH_LAST_PAGE + FLASH_PAGE_SIZE - 1U)

#define GUI_FLASH_MAGIC_NUMBER         ADDR_FLASH_LAST_PAGE                    /* Base @ of magic number   */

#else
/* Following definitions should be adapted to used Flash configuration :
   INDEX_SECTOR : represents the index of the sector used for storing USBPD settings (usually the last one)
   ADDR_FLASH_LAST_SECTOR : Flash address value of beginning of USBPD settings sector
   ADDR_FLASH_SECTOR_END : Flash address value of end of USBPD settings sector
*/

#define INDEX_SECTOR                   (FLASH_SECTOR_NB - 1U)                  /* Index of latest sector   */

/* If sector is in BANK1 use (FLASH_BASE),
   if in BANK2 use (FLASH_BASE + FLASH_BANK_SIZE) as base address in following define */
#define ADDR_FLASH_LAST_SECTOR         (FLASH_BASE + (INDEX_SECTOR * FLASH_SECTOR_SIZE)) /* Base @ of latest sector */
#define ADDR_FLASH_SECTOR_END          (ADDR_FLASH_LAST_SECTOR + FLASH_SECTOR_SIZE - 1U)

#define GUI_FLASH_MAGIC_NUMBER         ADDR_FLASH_LAST_SECTOR                  /* Base @ of magic number   */
#endif /* FLASH_PAGE_NB */

#if defined(FLASH_TYPEPROGRAM_DOUBLEWORD)
/* Should be aligned on double word */
#define GUI_FLASH_ADDR_NB_PDO_SNK_P0   (GUI_FLASH_MAGIC_NUMBER + 8u)
#elif defined(FLASH_TYPEPROGRAM_QUADWORD)
/* Should be aligned on quad word */
#define GUI_FLASH_ADDR_NB_PDO_SNK_P0   (GUI_FLASH_MAGIC_NUMBER + 16u)
#endif /* FLASH_TYPEPROGRAM_DOUBLEWORD */

#define GUI_FLASH_ADDR_NB_PDO_SRC_P0   (GUI_FLASH_ADDR_NB_PDO_SNK_P0 + 1u)
#define GUI_FLASH_ADDR_NB_PDO_SNK_P1   (GUI_FLASH_ADDR_NB_PDO_SRC_P0 + 1u)
#define GUI_FLASH_ADDR_NB_PDO_SRC_P1   (GUI_FLASH_ADDR_NB_PDO_SNK_P1 + 1u)

#if defined(FLASH_TYPEPROGRAM_DOUBLEWORD)
/* Should be aligned on double word */
#define GUI_FLASH_ADDR_PDO_SRC_P0      (GUI_FLASH_MAGIC_NUMBER + 16u)
#elif defined(FLASH_TYPEPROGRAM_QUADWORD)
/* Should be aligned on quad word */
#define GUI_FLASH_ADDR_PDO_SRC_P0      (GUI_FLASH_MAGIC_NUMBER + 32u)
#endif /* FLASH_TYPEPROGRAM_DOUBLEWORD */

#if defined(FLASH_TYPEPROGRAM_DOUBLEWORD)
/* New Address should be modulo 8 */
/* Computes the offset required to add based on the size X in increment of 16 bytes */
#define GUI_FLASH_ADDRESSOFFSET(X)  (((X) / 8U * 8U) + (((X) & 7U) + (7U)) / 8U * 8U)
#elif defined(FLASH_TYPEPROGRAM_QUADWORD)
/* New Address should be modulo 16 */
/* Computes the offset required to add based on the size X in increment of 16 bytes */
#define GUI_FLASH_ADDRESSOFFSET(X)  (((X) / 16U * 16U) + (((X) & 15U) + (15U)) / 16U * 16U)
#endif /* FLASH_TYPEPROGRAM_DOUBLEWORD */

#define GUI_FLASH_ADDR_PDO_SNK_P0         (GUI_FLASH_ADDR_PDO_SRC_P0 + GUI_FLASH_ADDRESSOFFSET((USBPD_MAX_NB_PDO * 4)))
#define GUI_FLASH_ADDR_PDO_SRC_P1         (GUI_FLASH_ADDR_PDO_SNK_P0 + GUI_FLASH_ADDRESSOFFSET((USBPD_MAX_NB_PDO * 4)))
#define GUI_FLASH_ADDR_PDO_SNK_P1         (GUI_FLASH_ADDR_PDO_SRC_P1 + GUI_FLASH_ADDRESSOFFSET((USBPD_MAX_NB_PDO * 4)))

#define GUI_FLASH_ADDR_DPM_SETTINGS       (GUI_FLASH_ADDR_PDO_SNK_P1 + GUI_FLASH_ADDRESSOFFSET((USBPD_MAX_NB_PDO * 4)))

#define GUI_FLASH_ADDR_DPM_USER_SETTINGS  (GUI_FLASH_ADDR_DPM_SETTINGS\
                                           + GUI_FLASH_ADDRESSOFFSET(sizeof(USBPD_SettingsTypeDef) \
                                                                     * USBPD_PORT_COUNT))

#define GUI_FLASH_ADDR_DPM_VDM_SETTINGS   (GUI_FLASH_ADDR_DPM_USER_SETTINGS\
                                           + GUI_FLASH_ADDRESSOFFSET(sizeof(USBPD_USER_SettingsTypeDef) \
                                                                     * USBPD_PORT_COUNT))

#define GUI_FLASH_ADDR_DPM_ID_SETTINGS    (GUI_FLASH_ADDR_DPM_VDM_SETTINGS\
                                           + GUI_FLASH_ADDRESSOFFSET(sizeof(USBPD_VDM_SettingsTypeDef) \
                                                                            * USBPD_PORT_COUNT))

#define GUI_FLASH_ADDR_RESERVED           (GUI_FLASH_ADDR_DPM_ID_SETTINGS\
                                           + GUI_FLASH_ADDRESSOFFSET(sizeof(USBPD_IdSettingsTypeDef) \
                                                                     * USBPD_PORT_COUNT))

#if defined(ADDR_FLASH_PAGE_END)
#define GUI_FLASH_SIZE_RESERVED           (int32_t)(ADDR_FLASH_PAGE_END - GUI_FLASH_ADDR_RESERVED)
#else
#define GUI_FLASH_SIZE_RESERVED           (int32_t)(ADDR_FLASH_SECTOR_END - GUI_FLASH_ADDR_RESERVED)
#endif /* ADDR_FLASH_PAGE_END */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_GUI_MEMMAP_H_ */

