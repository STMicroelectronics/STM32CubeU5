/**
  ******************************************************************************
  * @file    low_level_security.c
  * @author  MCD Application Team
  * @brief   security protection implementation for secure boot on STM32U5xx
  *
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
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "boot_hal_cfg.h"
#include "boot_hal_flowcontrol.h"
#include "mpu_armv8m_drv.h"
#include "region_defs.h"
#include "mcuboot_config/mcuboot_config.h"
#include "low_level_security.h"
#ifdef TFM_DEV_MODE
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_INFO
#else
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_OFF
#endif /* TFM_DEV_MODE  */
#include "bootutil/bootutil_log.h"
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
#include "low_level_rng.h"
#endif /*  (TFM_TAMPER_ENABLE != NO_TAMPER)*/
#include "target_cfg.h"
#include "bootutil_priv.h"

/** @defgroup TFM_SECURITY_Private_Defines  Private Defines
  * @{
  */
/* DUAL BANK page size */
#define PAGE_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

#define PAGE_MAX_NUMBER_IN_BANK 127

/* SBSFU_Boot Vector Address  */
#define SBSFU_BOOT_VTOR_ADDR ((uint32_t)(BL2_CODE_START))


const struct mpu_armv8m_region_cfg_t region_cfg_init_s[] = {
           /* background region is enabled , secure execution on unsecure flash is not possible*/
           /* but non secure execution on unsecure flash is possible , non secure mpu is used to protect execution */
           /* since SAU is enabled later to gain access to non secure flash */
           /* Forbid execuction outside of flash write protected area  */
           /* descriptor 0 is set execute readonly before jumping in Secure application */
           {
               0,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R0,
               FLOW_CTRL_MPU_S_I_EN_R0,
               FLOW_STEP_MPU_S_I_CH_R0,
               FLOW_CTRL_MPU_S_I_CH_R0,
#endif /* FLOW_CONTROL */
           },
           {
               1,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE,
#ifndef MCUBOOT_PRIMARY_ONLY
               FLASH_BASE_S + NS_IMAGE_SECONDARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1,
#else
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE - 1,
#endif /* MCUBOOT_PRIMARY_ONLY */
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R1,
               FLOW_CTRL_MPU_S_I_EN_R1,
               FLOW_STEP_MPU_S_I_CH_R1,
               FLOW_CTRL_MPU_S_I_CH_R1,
#endif /* FLOW_CONTROL */
           },
           {
               2,
               FLASH_BASE_S,
               FLASH_BASE_S + FLASH_AREA_BL2_OFFSET - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R2,
               FLOW_CTRL_MPU_S_I_EN_R2,
               FLOW_STEP_MPU_S_I_CH_R2,
               FLOW_CTRL_MPU_S_I_CH_R2,
#endif /* FLOW_CONTROL */
           },
           {
               3,
               FLASH_BASE_S + FLASH_AREA_BL2_NOHDP_OFFSET + FLASH_AREA_BL2_NOHDP_SIZE,
               FLASH_BASE_S + FLASH_AREA_0_OFFSET - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R3,
               FLOW_CTRL_MPU_S_I_EN_R3,
               FLOW_STEP_MPU_S_I_CH_R3,
               FLOW_CTRL_MPU_S_I_CH_R3,
#endif /* FLOW_CONTROL */
           },
           /* Forbid execution on full SRAM area */
           {
               4,
               BL2_SRAM_AREA_BASE,
               BL2_SRAM_AREA_END,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R4,
               FLOW_CTRL_MPU_S_I_EN_R4,
               FLOW_STEP_MPU_S_I_CH_R4,
               FLOW_CTRL_MPU_S_I_CH_R4,
#endif /* FLOW_CONTROL */
           },
           /* forbid secure peripheral execution */
           {
               5,
               PERIPH_BASE_S,
               PERIPH_BASE_S + 0xFFFFFFF,
               MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R5,
               FLOW_CTRL_MPU_S_I_EN_R5,
               FLOW_STEP_MPU_S_I_CH_R5,
               FLOW_CTRL_MPU_S_I_CH_R5,
#endif /* FLOW_CONTROL */
           },
           /* forbid execution of flash non secure alias */
           {
               6,
               FLASH_BASE_NS,
               FLASH_BASE_NS+FLASH_TOTAL_SIZE-1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_I_EN_R6,
               FLOW_CTRL_MPU_S_I_EN_R6,
               FLOW_STEP_MPU_S_I_CH_R6,
               FLOW_CTRL_MPU_S_I_CH_R6,
#endif /* FLOW_CONTROL */
           }
};

const struct mpu_armv8m_region_cfg_t region_cfg_init_ns[] = {
           /* forbid execution on non secure FLASH /RAM in case of jump in non secure */
           /* Non Secure MPU  background all access in priviligied */
           /* reduced execution to all flash during control */
           {
               0,
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET,
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_NS_I_EN_R0,
               FLOW_CTRL_MPU_NS_I_EN_R0,
               FLOW_STEP_MPU_NS_I_CH_R0,
               FLOW_CTRL_MPU_NS_I_CH_R0,
#endif /* FLOW_CONTROL */
           },
#if !defined(MCUBOOT_PRIMARY_ONLY)
           {
               1,
               FLASH_BASE_NS + S_IMAGE_SECONDARY_PARTITION_OFFSET,
               FLASH_BASE_NS + NS_IMAGE_SECONDARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_NS_I_EN_R1,
               FLOW_CTRL_MPU_NS_I_EN_R1,
               FLOW_STEP_MPU_NS_I_CH_R1,
               FLOW_CTRL_MPU_NS_I_CH_R1,
#endif /* FLOW_CONTROL */
           },
#endif /* MCUBOOT_PRIMARY_ONLY */
           /* Forbid execution on full SRAM area */
           {
               2,
#ifdef TFM_ERROR_HANDLER_NON_SECURE
               SRAM1_BASE_NS + (~MPU_RBAR_BASE_Msk) + 1,
#else
               SRAM1_BASE_NS,
#endif /*   TFM_ERROR_HANDLER_NON_SECURE */
               SRAM4_BASE_NS + _SRAM4_SIZE_MAX - 1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_NS_I_EN_R2,
               FLOW_CTRL_MPU_NS_I_EN_R2,
               FLOW_STEP_MPU_NS_I_CH_R2,
               FLOW_CTRL_MPU_NS_I_CH_R2,
#endif /* FLOW_CONTROL */
           },
           /* forbid secure peripheral execution */
           {
               3,
               PERIPH_BASE_NS,
               PERIPH_BASE_NS + 0xFFFFFFF,
               MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_NS_I_EN_R3,
               FLOW_CTRL_MPU_NS_I_EN_R3,
               FLOW_STEP_MPU_NS_I_CH_R3,
               FLOW_CTRL_MPU_NS_I_CH_R3,
#endif /* FLOW_CONTROL */
           }
};

#if defined(__ICCARM__)
#pragma location=".BL2_NoHdp_Data"
#endif
const struct mpu_armv8m_region_cfg_t region_cfg_appli_s[] __attribute__((section(".BL2_NoHdp_Data"))) = {
           /* region 0 is now enable for execution */
           {
               0,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET,
               /* Reduce area to allow write to validate the Secure Image */
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 1 - (~MPU_RLAR_LIMIT_Msk +1),
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_OK,
               MPU_ARMV8M_AP_RO_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_A_EN_R0,
               FLOW_CTRL_MPU_S_A_EN_R0,
               FLOW_STEP_MPU_S_A_CH_R0,
               FLOW_CTRL_MPU_S_A_CH_R0,
#endif /* FLOW_CONTROL */
           }
};

#if defined(__ICCARM__)
#pragma location=".BL2_NoHdp_Data"
#endif
const struct mpu_armv8m_region_cfg_t region_cfg_appli_ns[] __attribute__((section(".BL2_NoHdp_Data"))) = {
           /* region 0 is now enable for execution */
           {
               0,
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET,
               /* Reduce area to allow write to validate the Non Secure Image */
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1 - (~MPU_RLAR_LIMIT_Msk +1),
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_OK,
               MPU_ARMV8M_AP_RO_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_NS_A_EN_R0,
               FLOW_CTRL_MPU_NS_A_EN_R0,
               FLOW_STEP_MPU_NS_A_CH_R0,
               FLOW_CTRL_MPU_NS_A_CH_R0,
#endif /* FLOW_CONTROL */
           }
};

const struct sau_cfg_t sau_init_cfg[] = {
#ifdef TFM_ERROR_HANDLER_NON_SECURE
    /* Configures SAU regions non-secure to gain access to SRAM1 non secure   */
    {
        0,
        SRAM1_BASE_NS,
        (SRAM1_BASE_NS + NS_NO_INIT_DATA_SIZE - 1),
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_I_EN_R0,
        FLOW_CTRL_SAU_I_EN_R0,
        FLOW_STEP_SAU_I_CH_R0,
        FLOW_CTRL_SAU_I_CH_R0,
#endif /* FLOW_CONTROL */
    },
#endif /* TFM_ERROR_HANDLER_NON_SECURE */
    /* Allow non secure access to Flash non Secure peripheral for regression */
    /* Secure HAL flash driver uses the non secure flash address to perform access on non secure flash area */
    {
        1,
        (uint32_t)FLASH_NS,
        ((uint32_t)FLASH_NS + 0xffff),
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_I_EN_R1,
        FLOW_CTRL_SAU_I_EN_R1,
        FLOW_STEP_SAU_I_CH_R1,
        FLOW_CTRL_SAU_I_CH_R1,
#endif /* FLOW_CONTROL */
    },
    /* Allow non secure Flash base access for Area 1/2/3 */
    {
        2,
        ((uint32_t)FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET),
#if !defined(MCUBOOT_PRIMARY_ONLY)
        ((uint32_t)FLASH_BASE_NS + S_IMAGE_SECONDARY_PARTITION_OFFSET + FLASH_PARTITION_SIZE - 1),
#else
        ((uint32_t)FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1),
#endif /* MCUBOOT_PRIMARY_ONLY */
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_I_EN_R2,
        FLOW_CTRL_SAU_I_EN_R2,
        FLOW_STEP_SAU_I_CH_R2,
        FLOW_CTRL_SAU_I_CH_R2,
#endif /* FLOW_CONTROL */
    },
#if defined(MCUBOOT_EXT_LOADER)
    /* create a region to access to loader code for hash check */
    {
        3,
        LOADER_NS_ROM_ALIAS(FLASH_AREA_LOADER_OFFSET),
        LOADER_NS_ROM_ALIAS(FLASH_TOTAL_SIZE-1),
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_I_EN_R3,
        FLOW_CTRL_SAU_I_EN_R3,
        FLOW_STEP_SAU_I_CH_R3,
        FLOW_CTRL_SAU_I_CH_R3,
#endif /* FLOW_CONTROL */
    }
#endif
};

#ifdef MCUBOOT_EXT_LOADER
#if defined(__ICCARM__)
#pragma location=".BL2_NoHdp_Data"
#endif
const struct sau_cfg_t sau_load_cfg[] __attribute__((section(".BL2_NoHdp_Data"))) = {
    /* allow access to non secure loader area ram*/
    {
        0,
        (uint32_t)LOADER_NS_DATA_START,
        (uint32_t)LOADER_NS_DATA_LIMIT,
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_L_EN_R0,
        FLOW_CTRL_SAU_L_EN_R0,
        FLOW_STEP_SAU_L_CH_R0,
        FLOW_CTRL_SAU_L_CH_R0,
#endif /* FLOW_CONTROL */
    },
    /* loader non secure need access to periph */
    {
        1,
        (uint32_t)PERIPH_BASE_NS,
        ((uint32_t)PERIPH_BASE_NS + 0xFFFFFFF),
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_L_EN_R1,
        FLOW_CTRL_SAU_L_EN_R1,
        FLOW_STEP_SAU_L_CH_R1,
        FLOW_CTRL_SAU_L_CH_R1,
#endif /* FLOW_CONTROL */
    },
    {
        2,
        ((uint32_t)FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET),
#if !defined(MCUBOOT_PRIMARY_ONLY)
        ((uint32_t)FLASH_BASE_NS + S_IMAGE_SECONDARY_PARTITION_OFFSET + FLASH_PARTITION_SIZE - 1),
#else
        ((uint32_t)FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1),
#endif /* MCUBOOT_PRIMARY_ONLY */
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_L_EN_R2,
        FLOW_CTRL_SAU_L_EN_R2,
        FLOW_STEP_SAU_L_CH_R2,
        FLOW_CTRL_SAU_L_CH_R2,
#endif /* FLOW_CONTROL */
    },
    /* allow access to non secure loader area code */
    {
        3,
        (uint32_t)LOADER_NS_CODE_START,
        (uint32_t)LOADER_NS_CODE_LIMIT,
        TFM_FALSE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_L_EN_R3,
        FLOW_CTRL_SAU_L_EN_R3,
        FLOW_STEP_SAU_L_CH_R3,
        FLOW_CTRL_SAU_L_CH_R3,
#endif /* FLOW_CONTROL */
    },
#if defined(MCUBOOT_PRIMARY_ONLY)
    /* allow access to non secure calleable area */
    {
        4,
        (uint32_t)LOADER_CMSE_VENEER_REGION_START,
        (uint32_t)LOADER_CMSE_VENEER_REGION_LIMIT,
        TFM_TRUE,
#ifdef FLOW_CONTROL
        FLOW_STEP_SAU_L_EN_R4,
        FLOW_CTRL_SAU_L_EN_R4,
        FLOW_STEP_SAU_L_CH_R4,
        FLOW_CTRL_SAU_L_CH_R4,
#endif /* FLOW_CONTROL */
    }
#endif
};
#endif /*   MCUBOOT_EXT_LOADER */

/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup TFM_SECURITY_Private_Functions  Private Functions
  * @{
  */
#ifdef TFM_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel, uint32_t current_rdplevel);
#endif /* TFM_OB_RDP_LEVEL_VALUE */
static void gtzc_init_cfg(void);
static void sau_and_idau_cfg(void);
static void mpu_init_cfg(void);
static void mpu_appli_cfg(void);
static void flash_priv_cfg(void);
static void enable_hdp_protection(void);
static void lock_bl2_shared_area(void);
static void apply_wrp_sram2(uint32_t offset, uint32_t len);
#ifdef MCUBOOT_EXT_LOADER
static void mpu_loader_cfg(void);
static void sau_loader_cfg(void);
#if defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY)
static void secure_internal_flash(uint32_t offset_start, uint32_t offset_end);
#endif /* defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY) */
#endif /* MCUBOOT_EXT_LOADER */
static void active_tamper(void);
/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Functions Exported Functions
  * @{
  */
#if defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY)
void LL_SECU_SetLoaderCodeSecure(void)
{
    secure_internal_flash(FLASH_AREA_LOADER_OFFSET, FLASH_AREA_LOADER_OFFSET+LOADER_IMAGE_S_CODE_SIZE-1);
}
/* Place code in a specific section */
#endif /* defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY) */

#if defined(MCUBOOT_EXT_LOADER)
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
/**
  * @brief  Update the runtime security protections for application start
  *
  * @param  None
  * @retval None
  */
void LL_SECU_UpdateLoaderRunTimeProtections(void)
{
  /* Enable HDP protection to hide sensible boot material */
  enable_hdp_protection();
  /* Set MPU to enable execution of secure /non secure  loader */
  mpu_loader_cfg();
  /* reconfigure SAU to allow non secure execution */
  sau_loader_cfg();
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */
#endif /*MCUBOOT_EXT_LOADER*/
/**
  * @brief  Apply the runtime security  protections to
  *
  * @param  None
  * @note   By default, the best security protections are applied
  * @retval None
  */
void LL_SECU_ApplyRunTimeProtections(void)
{
  /* Unsecure bottom of SRAM1 for error_handler */
  gtzc_init_cfg();

  /* Set MPU to forbidd execution outside of not muteable code  */
  /* Initialize not secure MPU to forbidd execution on Flash /SRAM */
  mpu_init_cfg();

  /* Enable SAU to gain access to flash area non secure for write/read */
  sau_and_idau_cfg();
  /* With TFM_DEV_MODE , active tamper calls Error_Handler */
  /* Error_Handler requires sau_and_idau_cfg */
  active_tamper();
  /* Lock top of SRAM2 in secure */
  lock_bl2_shared_area();

  /* Configure Flash Privilege access */
  flash_priv_cfg();
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

/**
  * @brief  Update the runtime security protections for application start
  *
  * @param  None
  * @retval None
  */
void LL_SECU_UpdateRunTimeProtections(void)
{
  /* Write Protect SRAM2 */
  apply_wrp_sram2(BOOT_TFM_SHARED_DATA_BASE, BOOT_TFM_SHARED_DATA_SIZE);

  /* Enable HDP protection to hide sensible boot material */
  enable_hdp_protection();

  /* Set MPU to enable execution of Secure and Non Secure active slots */
  mpu_appli_cfg();
}

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

/**
  * @brief  Check if the Static security  protections to
  *         all the Sections in Flash:  WRP, SECURE FLASH, SECURE USER FLASH.
  *         those protections not impacted by a Reset. They are set using the Option Bytes
  *         When the device is locked (RDP Level2), these protections cannot be changed anymore
  * @param  None
  * @note   By default, the best security protections are applied to the different
  *         flash sections in order to maximize the security level for the specific MCU.
  * @retval None
  */
void LL_SECU_CheckStaticProtections(void)
{
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank1 = {0};
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank2 = {0};
#ifdef TFM_NSBOOT_CHECK_ENABLE
  static FLASH_OBProgramInitTypeDef flash_option_bytes_nsboot0 = {0};
  static FLASH_OBProgramInitTypeDef flash_option_bytes_nsboot1 = {0};
#endif /* TFM_NSBOOT_CHECK_ENABLE */
#ifdef TFM_ENABLE_SET_OB
  HAL_StatusTypeDef ret = HAL_ERROR;
#endif  /* TFM_ENABLE_SET_OB  */
  uint32_t start;
  uint32_t end;

#ifdef TFM_NSBOOT_CHECK_ENABLE
  /* Get NSBOOTADD0 and NSBOOTADD1 value */
  flash_option_bytes_nsboot0.BootAddrConfig = OB_BOOTADDR_NS0;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_nsboot0);
  flash_option_bytes_nsboot1.BootAddrConfig = OB_BOOTADDR_NS1;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_nsboot1);
#endif /* TFM_NSBOOT_CHECK_ENABLE */

  /* Get bank1 areaA OB  */
  flash_option_bytes_bank1.WRPArea = OB_WRPAREA_BANK1_AREAA;
  flash_option_bytes_bank1.WMSecConfig = OB_WMSEC_AREA1;
  flash_option_bytes_bank1.BootAddrConfig = OB_BOOTADDR_SEC0;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_bank1);

  /* Get bank2 areaB OB  */
  flash_option_bytes_bank2.WRPArea = OB_WRPAREA_BANK2_AREAA;
  flash_option_bytes_bank2.WMSecConfig = OB_WMSEC_AREA2;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_bank2);

#ifdef TFM_ENABLE_SET_OB
  /* Clean the option configuration */
  flash_option_bytes_bank1.OptionType = 0;
  flash_option_bytes_bank2.OptionType = 0;
  flash_option_bytes_bank2.WRPArea = 0;
  flash_option_bytes_bank1.WRPArea = 0;
#endif /*   TFM_ENABLE_SET_OB */

  /* Check TZEN = 1 , we are in secure */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_TZEN) != FLASH_OPTR_TZEN)
  {
    BOOT_LOG_ERR("Unexpected value for TZEN");
    Error_Handler();
  }

  /* Check if dual bank is set */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_DUALBANK) != FLASH_OPTR_DUALBANK)
  {
    BOOT_LOG_ERR("Unexpected value for dual bank configuration");
    Error_Handler();
  }

  /* Check if swap bank is reset */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_SWAP_BANK) != 0)
  {
    BOOT_LOG_ERR("Unexpected value for swap bank configuration");
    Error_Handler();
  }

  /* Check secure boot address */
  if (flash_option_bytes_bank1.BootAddr != BL2_BOOT_VTOR_ADDR)
  {
    BOOT_LOG_INF("BootAddr 0x%x", flash_option_bytes_bank1.BootAddr);
    BOOT_LOG_ERR("Unexpected value for SEC BOOT Address");
    Error_Handler();
  }

#ifdef TFM_NSBOOT_CHECK_ENABLE
  /* Check non-secure boot addresses */
  if ((flash_option_bytes_nsboot0.BootAddr != BL2_BOOT_VTOR_ADDR)
      || (flash_option_bytes_nsboot1.BootAddr != BL2_BOOT_VTOR_ADDR))
  {
    BOOT_LOG_ERR("Unexpected value for NS BOOT Address");
    Error_Handler();
  }
#endif /* TFM_NSBOOT_CHECK_ENABLE */

  /* Check bank1 secure flash protection */
  start = FLASH_BL2_NVCNT_AREA_OFFSET / PAGE_SIZE;
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = PAGE_MAX_NUMBER_IN_BANK;
  }
  if ((start != flash_option_bytes_bank1.WMSecStartPage)
      || (end != flash_option_bytes_bank1.WMSecEndPage))
  {
    BOOT_LOG_INF("BANK 1 secure flash [%d, %d] : OB [%d, %d]",
                 start, end, flash_option_bytes_bank1.WMSecStartPage, flash_option_bytes_bank1.WMSecEndPage);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for secure flash protection");
    Error_Handler();
#else
    BOOT_LOG_ERR("Unexpected value for secure flash protection: set wmsec1");
    flash_option_bytes_bank1.WMSecStartPage = start;
    flash_option_bytes_bank1.WMSecEndPage = end;
    flash_option_bytes_bank1.OptionType |= OPTIONBYTE_WMSEC;
    flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_AREA1 | OB_WMSEC_SECURE_AREA_CONFIG;
#endif /* TFM_ENABLE_SET_OB */
  }

  /* Check bank2 secure flash protection */
  start = 0;
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = end - (PAGE_MAX_NUMBER_IN_BANK + 1);
    if ((start != flash_option_bytes_bank2.WMSecStartPage)
        || (end != flash_option_bytes_bank2.WMSecEndPage))
    {
    BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", start, end, flash_option_bytes_bank2.WMSecStartPage,
                   flash_option_bytes_bank2.WMSecEndPage);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for secure flash protection");
    Error_Handler();
#else
    BOOT_LOG_ERR("Unexpected value for secure flash protection : set wmsec2");
    flash_option_bytes_bank2.WMSecStartPage = start;
    flash_option_bytes_bank2.WMSecEndPage = end;
    flash_option_bytes_bank2.OptionType = OPTIONBYTE_WMSEC;
    flash_option_bytes_bank2.WMSecConfig |= OB_WMSEC_AREA2 | OB_WMSEC_SECURE_AREA_CONFIG ;
#endif /* TFM_ENABLE_SET_OB  */
    }
  }
  /* the bank 2 must be fully unsecure */
  else if (flash_option_bytes_bank2.WMSecEndPage >= flash_option_bytes_bank2.WMSecStartPage)
  {
    BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", 127, 0, flash_option_bytes_bank2.WMSecStartPage,
                 flash_option_bytes_bank2.WMSecEndPage);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for secure flash protection");
    Error_Handler();
#else
    /* bank is not unsecured , modify option bytes */
    flash_option_bytes_bank2.WMSecStartPage = 127;
    flash_option_bytes_bank2.WMSecEndPage = 0;
    flash_option_bytes_bank2.OptionType = OPTIONBYTE_WMSEC;
    flash_option_bytes_bank2.WMSecConfig |= OB_WMSEC_AREA2 | OB_WMSEC_SECURE_AREA_CONFIG ;
#endif /* TFM_ENABLE_SET_OB */
  }

#ifdef  TFM_WRP_PROTECT_ENABLE
  /* Check flash write protection */
  start = FLASH_AREA_PERSO_OFFSET / PAGE_SIZE;
  end = (FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE + FLASH_AREA_BL2_SIZE +
         FLASH_AREA_BL2_NOHDP_SIZE - 1) / PAGE_SIZE;
  if ((flash_option_bytes_bank1.WRPStartOffset > flash_option_bytes_bank1.WRPEndOffset)
      || (start != flash_option_bytes_bank1.WRPStartOffset)
      || (end != flash_option_bytes_bank1.WRPEndOffset))
  {
    BOOT_LOG_INF("BANK 1 flash write protection [%d, %d] : OB [%d, %d]", start, end,
                 flash_option_bytes_bank1.WRPStartOffset,
                 flash_option_bytes_bank1.WRPEndOffset);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for write protection ");
    Error_Handler();
#else
    flash_option_bytes_bank1.WRPStartOffset = start;
    flash_option_bytes_bank1.WRPEndOffset = end;
    flash_option_bytes_bank1.WRPArea |= OB_WRPAREA_BANK1_AREAA;

    BOOT_LOG_ERR("Unexpected value for write protection : set wrp1");
    flash_option_bytes_bank1.OptionType |= OPTIONBYTE_WRP;
#endif /* TFM_ENABLE_SET_OB */
  }
#if  defined(TFM_WRP_LOCK_ENABLE)
  /* Check WRP lock protection */
  if (flash_option_bytes_bank1.WRPLock != ENABLE)
  {
    BOOT_LOG_INF("BANK 1 WRP Lock 0x%x", flash_option_bytes_bank1.WRPLock);
    BOOT_LOG_ERR("Unexpected value for WRP Lock");
    Error_Handler();
  }
#endif /* TFM_WRP_LOCK_ENABLE */

#ifdef MCUBOOT_EXT_LOADER
  /* Check flash write protection for local loader */
  start = FLASH_AREA_LOADER_BANK_OFFSET / PAGE_SIZE;
  end = (FLASH_B_SIZE - 1) / PAGE_SIZE;
  if ((flash_option_bytes_bank2.WRPStartOffset > flash_option_bytes_bank2.WRPEndOffset)
      || (start != flash_option_bytes_bank2.WRPStartOffset)
      || (end != flash_option_bytes_bank2.WRPEndOffset))
  {
    BOOT_LOG_INF("BANK 2 flash write protection [%d, %d] : OB [%d, %d]", start, end,
                 flash_option_bytes_bank2.WRPStartOffset,
                 flash_option_bytes_bank2.WRPEndOffset);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for write protection ");
    Error_Handler();
#else
    flash_option_bytes_bank2.WRPStartOffset = start;
    flash_option_bytes_bank2.WRPEndOffset = end;
    flash_option_bytes_bank2.WRPArea |= OB_WRPAREA_BANK2_AREAA;

    BOOT_LOG_ERR("Unexpected value for write protection : set wrp2");
    flash_option_bytes_bank2.OptionType |= OPTIONBYTE_WRP;
#endif /* TFM_ENABLE_SET_OB */
  }
#if  defined(TFM_WRP_LOCK_ENABLE)
  /* Check WRP lock protection */
  if (flash_option_bytes_bank2.WRPLock != ENABLE)
  {
    BOOT_LOG_INF("BANK 2 WRP Lock 0x%x", flash_option_bytes_bank2.WRPLock);
    BOOT_LOG_ERR("Unexpected value for WRP Lock");
    Error_Handler();
  }
#endif /* TFM_WRP_LOCK_ENABLE */
#endif /* MCUBOOT_EXT_LOADER */
#endif /* TFM_WRP_PROTECT_ENABLE */

#ifdef  TFM_HDP_PROTECT_ENABLE
  /* Check secure user flash protection (HDP) */
  start = FLASH_BL2_NVCNT_AREA_OFFSET / PAGE_SIZE;
  end = (FLASH_BL2_HDP_END) / PAGE_SIZE;
  if (
    (flash_option_bytes_bank1.WMSecStartPage > flash_option_bytes_bank1.WMHDPEndPage)
    || (start < flash_option_bytes_bank1.WMSecStartPage)
    || (end > flash_option_bytes_bank1.WMHDPEndPage)
    || (flash_option_bytes_bank1.WMSecConfig & OB_WMSEC_HDP_AREA_DISABLE))
  {
    BOOT_LOG_INF("BANK 1 secure user flash [%d, %d] : OB [%d, %d]",
                 start,
                 end,
                 flash_option_bytes_bank1.WMSecStartPage,
                 flash_option_bytes_bank1.WMHDPEndPage);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Unexpected value for secure user flash protection");
    Error_Handler();
#else
    BOOT_LOG_ERR("Unexpected value for secure user flash protection : set hdp1");
    flash_option_bytes_bank1.WMSecStartPage = start;
    flash_option_bytes_bank1.WMHDPEndPage = end;
    flash_option_bytes_bank1.OptionType |= OPTIONBYTE_WMSEC;
    /*  clean disable */
    flash_option_bytes_bank1.WMSecConfig &= ~OB_WMSEC_HDP_AREA_DISABLE;
    /* enable */
    flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_HDP_AREA_CONFIG ;
    flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_HDP_AREA_ENABLE;
    flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_AREA1;
#endif  /*  TFM_ENABLE_SET_OB */
  }
#else /* TFM_HDP_PROTECT_ENABLE */
  flash_option_bytes_bank1.WMSecConfig &= ~(OB_WMSEC_HDP_AREA_CONFIG | OB_WMSEC_HDP_AREA_ENABLE);
  flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_HDP_AREA_DISABLE;
#endif /* TFM_HDP_PROTECT_ENABLE */

#ifdef TFM_SECURE_USER_SRAM2_ERASE_AT_RESET
  /* Check SRAM2 ERASE on reset */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_SRAM2_RST) != 0)
  {
    BOOT_LOG_ERR("Unexpected value for SRAM2 ERASE at Reset");
    Error_Handler();
  }
#endif /*TFM_SECURE_USER_SRAM2_ERASE_AT_RESET */

#ifdef TFM_ENABLE_SET_OB
  /* Configure Options Bytes */
  if ((flash_option_bytes_bank1.OptionType != 0) || (flash_option_bytes_bank2.OptionType != 0))
  {
    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    /* Unlock the Options Bytes */
    HAL_FLASH_OB_Unlock();

    /* Verify Options Bytes to configure */
    if ((flash_option_bytes_bank1.OptionType & OPTIONBYTE_RDP) != 0)
    {
      BOOT_LOG_ERR("Unexpected value for OB RDP to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank2.OptionType & OPTIONBYTE_RDP) != 0)
    {
      BOOT_LOG_ERR("Unexpected value for OB RDP to program");
      Error_Handler();
    }
#ifdef MCUBOOT_EXT_LOADER
    if ((flash_option_bytes_bank2.WRPArea & ~OB_WRPAREA_BANK2_AREAA) != 0)
#else
    if ((flash_option_bytes_bank2.WRPArea & ~OB_WRPAREA_BANK2_AREAA) != 0)
#endif
    {
      BOOT_LOG_ERR("Unexpected value for bank 2 OB WRP AREA to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank1.WRPArea & ~OB_WRPAREA_BANK1_AREAA) != 0)
    {
      BOOT_LOG_ERR("Unexpected value for bank 1 OB WRP AREA to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank1.OptionType) != 0)
    {
      /* Program the Options Bytes */
      ret = HAL_FLASHEx_OBProgram(&flash_option_bytes_bank1);
      if (ret != HAL_OK)
      {
        BOOT_LOG_ERR("Error while setting OB Bank1 config");
        Error_Handler();
      }
    }
    if ((flash_option_bytes_bank2.OptionType) != 0)
    {
      /* Program the Options Bytes */
      ret = HAL_FLASHEx_OBProgram(&flash_option_bytes_bank2);
      if (ret != HAL_OK)
      {
        BOOT_LOG_ERR("Error while setting OB Bank1 config");
        Error_Handler();
      }
    }

    /* Launch the Options Bytes (reset the board, should not return) */
    ret = HAL_FLASH_OB_Launch();
    if (ret != HAL_OK)
    {
      BOOT_LOG_ERR("Error while execution OB_Launch");
      Error_Handler();
    }

    /* Code should not be reached, reset the board */
    HAL_NVIC_SystemReset();
  }
#endif /* TFM_ENABLE_SET_OB */

#ifdef TFM_OB_BOOT_LOCK
  /* Check Boot lock protection */
  if (flash_option_bytes_bank1.BootLock != TFM_OB_BOOT_LOCK)
  {
    BOOT_LOG_INF("BootLock 0x%x", flash_option_bytes_bank1.BootLock);
    BOOT_LOG_ERR("Unexpected value for SEC BOOT LOCK");
    Error_Handler();
  }
#endif /* TFM_OB_BOOT_LOCK */

#ifdef TFM_OB_RDP_LEVEL_VALUE
  /* Check RDL level : boot if current RDP level is greater or equal to selected RDP level */
  switch (TFM_OB_RDP_LEVEL_VALUE)
  {
    case OB_RDP_LEVEL_2:
      if (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2)
      {
        rdp_level(TFM_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_1:
      if ((flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_1))
      {
        rdp_level(TFM_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_0_5:
      if ((flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_1) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_0_5))
      {
        rdp_level(TFM_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_0:
      break;

    default:
      Error_Handler();
      break;
  }
#endif /* TFM_OB_RDP_LEVEL_VALUE */
}

#ifdef TFM_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel, uint32_t current_rdplevel)
{
  BOOT_LOG_INF("RDPLevel 0x%x (0x%x)", current_rdplevel, rdplevel);
  BOOT_LOG_ERR("Unexpected value for RDP level");

#ifdef TFM_ENABLE_SET_OB
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank = {0};
  HAL_StatusTypeDef ret = HAL_ERROR;

  /* Automatic programming to RDP 2 is not allowed */
  if (rdplevel == OB_RDP_LEVEL_2)
  {
    Error_Handler();
  }

  flash_option_bytes_bank.OptionType = OPTIONBYTE_RDP;
  flash_option_bytes_bank.RDPLevel = rdplevel;
  BOOT_LOG_INF("Programming RDP to %x", rdplevel);
  BOOT_LOG_INF("Unplug/Plug jumper JP3 (IDD)");

  /* Unlock the Flash to enable the flash control register access */
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes */
  HAL_FLASH_OB_Unlock();

  /* Program the Options Bytes */
  ret = HAL_FLASHEx_OBProgram(&flash_option_bytes_bank);
  if (ret != HAL_OK)
  {
    BOOT_LOG_ERR("Error while setting RDP level");
    Error_Handler();
  }

  /* Launch the Options Bytes (reset the board, should not return) */
  ret = HAL_FLASH_OB_Launch();
  if (ret != HAL_OK)
  {
    BOOT_LOG_ERR("Error while execution OB_Launch");
    Error_Handler();
  }

  /* Code should not be reached, reset the board */
  HAL_NVIC_SystemReset();
#endif /* TFM_ENABLE_SET_OB */
}
#endif /* TFM_OB_RDP_LEVEL_VALUE */
#if defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY)
static void secure_internal_flash(uint32_t offset_start, uint32_t offset_end)
{
    volatile uint32_t *SecBB[8]= {&FLASH_S->SECBB1R1, &FLASH_S->SECBB1R2, &FLASH_S->SECBB1R3, &FLASH_S->SECBB1R4,
      &FLASH_S->SECBB2R1, &FLASH_S->SECBB2R2, &FLASH_S->SECBB2R3, &FLASH_S->SECBB2R4};
    volatile uint32_t *ptr;
    uint32_t regwrite=0x0, index;
    uint32_t block_start = offset_start;
    uint32_t block_end =  offset_end;
    block_start = block_start / PAGE_SIZE;
    block_end = (block_end / PAGE_SIZE) ;
    /* Check alignment to avoid further problem  */
    if (offset_start & (PAGE_SIZE-1))
        Error_Handler();

    /* configuration stage */
    if (uFlowStage == FLOW_STAGE_CFG)
    {
      /* 1f is for 32 bits */
      for (index = block_start & ~0x1f; index < (8<<5) ; index++)
      { /* clean register on index aligned */
        if (!(index & 0x1f)){
           regwrite=0x0;
        }
        if ((index >= block_start) && (index <= block_end))
          regwrite = regwrite | ( 1 << (index & 0x1f));
        /* write register when 32 sub block are set or last block to set  */
        if ((index & 0x1f ) == 0x1f) {
          ptr = (uint32_t *)SecBB[index>>5];
          *ptr = regwrite;
        }
      }
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_S_L_EN, FLOW_CTRL_FLASH_S_L_EN);
    }
    /* verification stage */
    else
    {
      /*  1f is for 32 bits */
      for (index = block_start & ~0x1f; index < (8<<5) ; index++)
      { /* clean register on index aligned */
        if (!(index & 0x1f)){
           regwrite=0x0;
        }
        if ((index >= block_start) && (index <= block_end))
          regwrite = regwrite | ( 1 << (index & 0x1f));
        /* write register when 32 sub block are set or last block to set  */
        if ((index & 0x1f ) == 0x1f) {
          ptr = (uint32_t *)SecBB[index>>5];
          if (*ptr != regwrite)
          {
            Error_Handler();
          }
        }
      }
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_S_L_CH, FLOW_CTRL_FLASH_S_L_CH);
    }
}
#endif /* defined(MCUBOOT_EXT_LOADER) && defined(MCUBOOT_PRIMARY_ONLY)  */
/**
  * @brief  Memory Config Init
  * @param  None
  * @retval None
  */
static void  gtzc_init_cfg(void)
{
   __HAL_RCC_GTZC1_CLK_ENABLE();
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  /*  unsecure only one block in SRAM1 */
  GTZC_MPCBB1_S->SECCFGR[0] = 0xfffffffe;
#endif /*  TFM_ERROR_HANDLER_NON_SECURE */

}

/**
  * @brief  Sau idau init
  * @param  None
  * @retval None
  */
static void sau_and_idau_cfg(void)
{
  uint32_t i;
  uint32_t rnr;
  uint32_t rbar;
  uint32_t rlar;
  uint32_t rnr_reg;
  uint32_t rbar_reg;
  uint32_t rlar_reg;
  uint32_t ctrl_reg;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Disable SAU */
    TZ_SAU_Disable();

    for (i = 0; i < ARRAY_SIZE(sau_init_cfg); i++)
    {
      SAU->RNR = sau_init_cfg[i].RNR;
      SAU->RBAR = sau_init_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      SAU->RLAR = (sau_init_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                  (sau_init_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                  SAU_RLAR_ENABLE_Msk;

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_init_cfg[i].flow_step_enable,
                                           sau_init_cfg[i].flow_ctrl_enable);
    }

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();
    /* Enable SAU */
    TZ_SAU_Enable();

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_EN, FLOW_CTRL_SAU_I_EN);
  }
  /* verification stage */
  else
  {
    for (i = 0; i < ARRAY_SIZE(sau_init_cfg); i++)
    {
      SAU->RNR = sau_init_cfg[i].RNR;

      rnr = sau_init_cfg[i].RNR;
      rbar = sau_init_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      rlar = (sau_init_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                  (sau_init_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                  SAU_RLAR_ENABLE_Msk;

      rnr_reg = SAU->RNR;
      rbar_reg = SAU->RBAR;
      rlar_reg = SAU->RLAR;

      if ((rnr_reg != rnr) || (rbar_reg != rbar) || (rlar_reg != rlar))
      {
        Error_Handler();
      }

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_init_cfg[i].flow_step_check,
                                           sau_init_cfg[i].flow_ctrl_check);
    }

    ctrl_reg = SAU->CTRL;
    if ((ctrl_reg && SAU_CTRL_ENABLE_Msk) != 1U)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_CH, FLOW_CTRL_SAU_I_CH);
    }
  }
}

#ifdef MCUBOOT_EXT_LOADER
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
/**
  * @brief  Sau idau update for executing loader
  * @param  None
  * @retval None
  */
static void sau_loader_cfg(void)
{
  uint32_t i;
  uint32_t rnr;
  uint32_t rbar;
  uint32_t rlar;
  uint32_t rnr_reg;
  uint32_t rbar_reg;
  uint32_t rlar_reg;
  uint32_t ctrl_reg;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Disable SAU */
    TZ_SAU_Disable();

    for (i = 0; i < ARRAY_SIZE(sau_load_cfg); i++)
    {
      SAU->RNR = sau_load_cfg[i].RNR;
      SAU->RBAR = sau_load_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      SAU->RLAR = (sau_load_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                  (sau_load_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                  SAU_RLAR_ENABLE_Msk;

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_load_cfg[i].flow_step_enable,
                                           sau_load_cfg[i].flow_ctrl_enable);
    }

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();
    /* Enable SAU */
    TZ_SAU_Enable();

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_L_EN, FLOW_CTRL_SAU_L_EN);
  }
  /* verification stage */
  else
  {
    for (i = 0; i < ARRAY_SIZE(sau_load_cfg); i++)
    {
      SAU->RNR = sau_load_cfg[i].RNR;

      rnr = sau_load_cfg[i].RNR;
      rbar = sau_load_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      rlar = (sau_load_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                  (sau_load_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                  SAU_RLAR_ENABLE_Msk;

      rnr_reg = SAU->RNR;
      rbar_reg = SAU->RBAR;
      rlar_reg = SAU->RLAR;

      if ((rnr_reg != rnr) || (rbar_reg != rbar) || (rlar_reg != rlar))
      {
        Error_Handler();
      }

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_load_cfg[i].flow_step_check,
                                           sau_load_cfg[i].flow_ctrl_check);
    }

    ctrl_reg = SAU->CTRL;
    if ((ctrl_reg && SAU_CTRL_ENABLE_Msk) != 1U)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_L_CH, FLOW_CTRL_SAU_L_CH);
    }

/* FIX ME to INDENT better */
#if 1
   /* set up floating point */
/*
// <e>Setup behaviour of Floating Point Unit
*/
#define TZ_FPU_NS_USAGE 1

/*
// <o>Floating Point Unit usage
//     <0=> Secure state only
//     <3=> Secure and Non-Secure state
//   <i> Value for SCB->NSACR register bits CP10, CP11
*/
#define SCB_NSACR_CP10_11_VAL       3

/*
// <o>Treat floating-point registers as Secure
//     <0=> Disabled
//     <1=> Enabled
//   <i> Value for FPU->FPCCR register bit TS
*/
#define FPU_FPCCR_TS_VAL            0

/*
// <o>Clear on return (CLRONRET) accessibility
//     <0=> Secure and Non-Secure state
//     <1=> Secure state only
//   <i> Value for FPU->FPCCR register bit CLRONRETS
*/
#define FPU_FPCCR_CLRONRETS_VAL     0

/*
// <o>Clear floating-point caller saved registers on exception return
//     <0=> Disabled
//     <1=> Enabled
//   <i> Value for FPU->FPCCR register bit CLRONRET
*/
#define FPU_FPCCR_CLRONRET_VAL      1
    SCB->NSACR = (SCB->NSACR & ~(SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk)) |
                   ((SCB_NSACR_CP10_11_VAL << SCB_NSACR_CP10_Pos) & (SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk));

    FPU->FPCCR = (FPU->FPCCR & ~(FPU_FPCCR_TS_Msk | FPU_FPCCR_CLRONRETS_Msk | FPU_FPCCR_CLRONRET_Msk)) |
                   ((FPU_FPCCR_TS_VAL        << FPU_FPCCR_TS_Pos       ) & FPU_FPCCR_TS_Msk       ) |
                   ((FPU_FPCCR_CLRONRETS_VAL << FPU_FPCCR_CLRONRETS_Pos) & FPU_FPCCR_CLRONRETS_Msk) |
                   ((FPU_FPCCR_CLRONRET_VAL  << FPU_FPCCR_CLRONRET_Pos ) & FPU_FPCCR_CLRONRET_Msk );
    /* Lock SAU config */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->CSLCKR |= SYSCFG_CSLCKR_LOCKSAU;
#endif
  }
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */
#endif /* MCUBOOT_EXT_LOADER */
/**
  * @brief  mpu init
  * @param  None
  * @retval None
  */
static void mpu_init_cfg(void)
{
#ifdef TFM_BOOT_MPU_PROTECTION
  struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  struct mpu_armv8m_dev_t dev_mpu_ns = { MPU_BASE_NS };
  int32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_enable,
                                             region_cfg_init_s[i].flow_ctrl_enable);
      }
    }

    /* enable secure MPU */
    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_I_EN, FLOW_CTRL_MPU_S_I_EN);

    /* configure non secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_ns); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_ns,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_ns[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_ns[i].flow_step_enable,
                                             region_cfg_init_ns[i].flow_ctrl_enable);
      }
    }

    /* enable non secure MPU */
    mpu_armv8m_enable(&dev_mpu_ns, PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_NS_I_EN, FLOW_CTRL_MPU_NS_I_EN);
  }
  /* verification stage */
  else
  {
    /* check secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_check,
                                             region_cfg_init_s[i].flow_ctrl_check);
      }
    }

    /* check secure MPU */
    if (mpu_armv8m_check(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                      HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_I_CH, FLOW_CTRL_MPU_S_I_CH);
    }

    /* check non secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_ns); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_ns,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_ns[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_ns[i].flow_step_check,
                                             region_cfg_init_ns[i].flow_ctrl_check);
      }
    }

    /* check non secure MPU */
    if (mpu_armv8m_check(&dev_mpu_ns, PRIVILEGED_DEFAULT_ENABLE,
                      HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_NS_I_CH, FLOW_CTRL_MPU_NS_I_CH);
    }
  }
#endif /* TFM_BOOT_MPU_PROTECTION */
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

static void mpu_appli_cfg(void)
{
#ifdef TFM_BOOT_MPU_PROTECTION
  static struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  static struct mpu_armv8m_dev_t dev_mpu_ns = { MPU_BASE_NS };
  int32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_s[i].flow_step_enable,
                                             region_cfg_appli_s[i].flow_ctrl_enable);
      }
    }

    /* configure non secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_ns); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_ns,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_ns[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_ns[i].flow_step_enable,
                                             region_cfg_appli_ns[i].flow_ctrl_enable);
      }
    }
  }
  else
  {
    /* check secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_s); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_s[i].flow_step_check,
                                             region_cfg_appli_s[i].flow_ctrl_check);
      }
    }

    /* check non secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_ns); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_ns,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_ns[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_ns[i].flow_step_check,
                                             region_cfg_appli_ns[i].flow_ctrl_check);
      }
    }
  }
#endif /* TFM_BOOT_MPU_PROTECTION */
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

#ifdef MCUBOOT_EXT_LOADER
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
static void mpu_loader_cfg(void)
{
#ifdef TFM_BOOT_MPU_PROTECTION
#if defined(MCUBOOT_PRIMARY_ONLY)
  static struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* secure region 2 is configured to allow read /write to NS_RAM */
    mpu_armv8m_region_disable(&dev_mpu_s, 2);
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_L_DI_R2, FLOW_CTRL_MPU_S_L_DI_R2);

  }
  else
  {
    /* check region 2 is disabled */
    if (mpu_armv8m_region_disable_check(&dev_mpu_s, 2) != MPU_ARMV8M_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_L_CH_R2, FLOW_CTRL_MPU_S_L_CH_R2);
    }

  }
#endif /* MCUBOOT_PRIMARY_ONLY */
#endif /* TFM_BOOT_MPU_PROTECTION  */

  /* configuration stage */
  if (uFlowStage != FLOW_STAGE_CFG)
  {
    /* Lock Secure MPU config */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->CSLCKR |= SYSCFG_CSLCKR_LOCKSMPU;

    /* Lock Non Secure MPU config */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->CSLCKR |= SYSCFG_CNSLCKR_LOCKNSMPU;
  }
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */
#endif /* MCUBOOT_EXT_LOADER */

/**
  * @brief  lock bl2 shared sram in secure
  * @param  None
  * @retval None
  */
static void lock_bl2_shared_area(void)
{
 uint32_t lockAttributes = GTZC_MCPBB_SUPERBLOCK_LOCKED;

  /* assumption shared area in SRAM2 in the last 8 Kbytes super block */
  /*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
  if (
    (BOOT_TFM_SHARED_DATA_BASE >= S_RAM_ALIAS(_SRAM2_TOP - GTZC_MPCBB_SUPERBLOCK_SIZE))
    && (BOOT_TFM_SHARED_DATA_SIZE <= GTZC_MPCBB_SUPERBLOCK_SIZE))
  {
    __HAL_RCC_GTZC1_CLK_ENABLE();
    HAL_GTZC_MPCBB_LockConfig(S_RAM_ALIAS(_SRAM2_TOP - GTZC_MPCBB_SUPERBLOCK_SIZE),
                                            1,
                                            &lockAttributes);
  }
  else Error_Handler();
}

/**
  * @brief  configure flash privilege access
  * @param  None
  * @retval None
  */
static void flash_priv_cfg(void)
{
#ifdef TFM_FLASH_PRIVONLY_ENABLE
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Configure Flash Privilege access */
    HAL_FLASHEx_ConfigPrivMode(FLASH_SPRIV_DENIED);
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_P_EN, FLOW_CTRL_FLASH_P_EN);
  }
  /* verification stage */
  else
  {
    if (HAL_FLASHEx_GetPrivMode() != FLASH_SPRIV_DENIED)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_P_CH, FLOW_CTRL_FLASH_P_CH);
    }
  }
#endif /*  TFM_FLASH_PRIVONLY_ENABLE */
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

/**
  * @brief  Enable HDP protection
  * @param  None
  * @retval None
  */
static void enable_hdp_protection(void)
{
#ifdef TFM_HDP_PROTECT_ENABLE
  do
  {
    /* Activate HDP protection */
    SET_BIT(FLASH->SECHDPCR, FLASH_SECHDPCR_HDP1_ACCDIS);
  } while ((FLASH->SECHDPCR & FLASH_SECHDPCR_HDP1_ACCDIS) != FLASH_SECHDPCR_HDP1_ACCDIS);

  if ((FLASH->SECHDPCR & FLASH_SECHDPCR_HDP1_ACCDIS) != FLASH_SECHDPCR_HDP1_ACCDIS)
  {
    /* Security issue : execution stopped ! */
    Error_Handler();
  }
#endif /* TFM_HDP_PROTECT_ENABLE */
}

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

static void apply_wrp_sram2(uint32_t offset, uint32_t len)
{
  uint32_t start_offset = ((offset - SRAM2_BASE_S)/ SRAM2_PAGE_SIZE);
  uint32_t end_offset = start_offset + (len -1)/SRAM2_PAGE_SIZE;
  uint32_t index;
  uint32_t val0 = 0;
  uint32_t val1 = 0;
  RCC->AHB1ENR |= RCC_AHB1ENR_RAMCFGEN;
  __HAL_RCC_SYSCFG_CLK_ENABLE();
#if 0
  BOOT_LOG_INF("SRAM2 write protection [0x%x, 0x%x] : %d %d", offset, len, start_offset, end_offset);
#endif
  for (index = start_offset; index <= end_offset; index ++)
  {
    if (index > 31U)
    {
      val1 |= (1 << (index - 32U));
    }
    else
    {
      val0 |= (1 << index);
    }
  }

#if 0
  BOOT_LOG_INF("SRAM2 write protection [0x%x] : %x", (uint32_t)&RAMCFG_SRAM2_S->WPR1, val0);
  BOOT_LOG_INF("SRAM2 write protection [0x%x] : %x", (uint32_t)&RAMCFG_SRAM2_S->WPR2, val1);
#endif
  RAMCFG_SRAM2_S->WPR1 = val0;
  RAMCFG_SRAM2_S->WPR2 = val1;
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

#if (TFM_TAMPER_ENABLE != NO_TAMPER)
const RTC_SecureStateTypeDef TamperSecureConf = {
    .rtcSecureFull = RTC_SECURE_FULL_NO,
    .rtcNonSecureFeatures = RTC_NONSECURE_FEATURE_ALL,
    .tampSecureFull = TAMP_SECURE_FULL_YES,
    .MonotonicCounterSecure = TAMP_MONOTONIC_CNT_SECURE_NO,
    .backupRegisterStartZone2 = 0,
    .backupRegisterStartZone3 = 0
};
const RTC_PrivilegeStateTypeDef TamperPrivConf = {
    .rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO,
    .rtcPrivilegeFeatures = RTC_PRIVILEGE_FEATURE_NONE,
    .tampPrivilegeFull = TAMP_PRIVILEGE_FULL_YES,
    .MonotonicCounterPrivilege = TAMP_MONOTONIC_CNT_PRIVILEGE_NO,
    .backupRegisterStartZone2 = 0,
    .backupRegisterStartZone3 = 0
};
const RTC_InternalTamperTypeDef InternalTamperConf = {
    .IntTamper = RTC_INT_TAMPER_1 | RTC_INT_TAMPER_9,
    .TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE,
    .NoErase                  = RTC_TAMPER_ERASE_BACKUP_ENABLE
};
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
void TAMP_IRQHandler(void)
{
    NVIC_SystemReset();
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */
#ifdef TFM_DEV_MODE
extern volatile uint32_t TamperEventCleared;
#endif
#endif /* (TFM_TAMPER_ENABLE != NO_TAMPER) */
RTC_HandleTypeDef RTCHandle;

static void active_tamper(void)
{
#if (TFM_TAMPER_ENABLE == ALL_TAMPER)
    RTC_ActiveTampersTypeDef sAllTamper;
    /*  use random generator to feed  */
    uint32_t Seed[4]={0,0,0,0};
    uint32_t len=0;
    uint32_t j;
#endif /* (TFM_TAMPER_ENABLE == ALL_TAMPER) */
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
    RTC_SecureStateTypeDef TamperSecureConfGet;
    RTC_PrivilegeStateTypeDef TamperPrivConfGet;
#endif /* TFM_TAMPER_ENABLE != NO_TAMPER) */
    /* configuration stage */
    if (uFlowStage == FLOW_STAGE_CFG)
    {
#if defined(TFM_DEV_MODE) && (TFM_TAMPER_ENABLE != NO_TAMPER)
        if (TamperEventCleared) {
            BOOT_LOG_INF("Boot with TAMPER Event Active");
#if (TFM_TAMPER_ENABLE == ALL_TAMPER)
            /* avoid several re-boot in DEV_MODE with Tamper active */
            BOOT_LOG_INF("Plug the tamper cable, and reboot");
            BOOT_LOG_INF("Or");
#endif
            BOOT_LOG_INF("Build and Flash with flag #define TFM_TAMPER_ENABLE NO_TAMPER\n");
            Error_Handler();
        }
#endif /*  TFM_DEV_MODE && (TFM_TAMPER_ENABLE != NO_TAMPER) */

        /* RTC Init */
        RTCHandle.Instance = RTC;
        RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
        RTCHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
        RTCHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
        RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
        RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
        RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_PUSHPULL;
        RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;

        if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
        {
            Error_Handler();
        }
#if (TFM_TAMPER_ENABLE == ALL_TAMPER)
        /* generate random seed */
        mbedtls_hardware_poll(NULL, (unsigned char *)Seed, sizeof(Seed),(size_t *)&len);
        if (len == 0)
        {
            Error_Handler();
        }
        BOOT_LOG_INF("TAMPER SEED [0x%x,0x%x,0x%x,0x%x]", Seed[0], Seed[1], Seed[2], Seed[3]);
        /* Configure active tamper common parameters  */
        sAllTamper.ActiveFilter = RTC_ATAMP_FILTER_ENABLE;
        sAllTamper.ActiveAsyncPrescaler = RTC_ATAMP_ASYNCPRES_RTCCLK_32;
        sAllTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
        sAllTamper.ActiveOutputChangePeriod = 4;
        sAllTamper.Seed[0] = Seed[0];
        sAllTamper.Seed[1] = Seed[1];
        sAllTamper.Seed[2] = Seed[2];
        sAllTamper.Seed[3] = Seed[3];

        /* Disable all Active Tampers */
        /* No active tamper */
        for (j = 0; j <= RTC_ATAMP_8; j++)
        {
            sAllTamper.TampInput[j].Enable = RTC_ATAMP_DISABLE;
        }

        sAllTamper.TampInput[7].Enable = RTC_ATAMP_ENABLE;
        sAllTamper.TampInput[7].Output = 7;
        sAllTamper.TampInput[7].NoErase =  RTC_TAMPER_ERASE_BACKUP_ENABLE;
        sAllTamper.TampInput[7].MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
        sAllTamper.TampInput[7].Interrupt = RTC_ATAMP_INTERRUPT_ENABLE;
        /* Set active tampers */
        if (HAL_RTCEx_SetActiveTampers(&RTCHandle, &sAllTamper) != HAL_OK)
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_ACT_EN, FLOW_CTRL_TAMP_ACT_EN);
#else
        HAL_RTCEx_DeactivateTamper(&RTCHandle, RTC_TAMPER_ALL);
#endif  /* (TFM_TAMPER_ENABLE == ALL_TAMPER) */
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
        /*  Internal Tamper activation  */
        /*  Enable Cryptographic IPs fault (tamp_itamp9), Backup domain voltage threshold monitoring (tamp_itamp1)*/
        if (HAL_RTCEx_SetInternalTamper(&RTCHandle,(RTC_InternalTamperTypeDef *)&InternalTamperConf)!=HAL_OK)
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_EN, FLOW_CTRL_TAMP_INT_EN);

        /*  Set tamper configuration secure only  */
        if (HAL_RTCEx_SecureModeSet(&RTCHandle, (RTC_SecureStateTypeDef *)&TamperSecureConf) != HAL_OK)
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_EN, FLOW_CTRL_TAMP_SEC_EN);

        /*  Set tamper configuration privileged only   */
        if (HAL_RTCEx_PrivilegeModeSet(&RTCHandle,(RTC_PrivilegeStateTypeDef *)&TamperPrivConf) != HAL_OK)
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_EN, FLOW_CTRL_TAMP_PRIV_EN);

        /*  Activate Secret Erase */
        HAL_RTCEx_Erase_SecretDev_Conf(&RTCHandle,(uint32_t)TAMP_SECRETDEVICE_ERASE_ENABLE);
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_EN, FLOW_CTRL_TAMP_CFG_EN);
        BOOT_LOG_INF("TAMPER Activated");
#else
        HAL_RTCEx_DeactivateInternalTamper(&RTCHandle, RTC_INT_TAMPER_ALL);
#endif /* (TFM_TAMPER_ENABLE != NO_TAMPER) */
    }
#if (TFM_TAMPER_ENABLE != NO_TAMPER)
    /* verification stage */
    else
    {
#if (TFM_TAMPER_ENABLE == ALL_TAMPER)
        /* Check active tampers */
        if ((READ_BIT(TAMP->ATOR, TAMP_ATOR_INITS) == 0U) ||
            (READ_REG(TAMP->IER) != 0x00000080U) ||
            (READ_REG(TAMP->ATCR1) != 0x84050080U) ||
            (READ_REG(TAMP->ATCR2) != 0xE0000000U) ||
            (READ_REG(TAMP->CR1) != 0x01010080U) ||
            (READ_REG(TAMP->CR2) != 0x00000000U))
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_ACT_CH, FLOW_CTRL_TAMP_ACT_CH);
#endif  /* (TFM_TAMPER_ENABLE == ALL_TAMPER) */
        /*  Check Internal Tamper activation */
        if ((READ_BIT(RTC->CR, RTC_CR_TAMPTS) != InternalTamperConf.TimeStampOnTamperDetection) ||
#if (TFM_TAMPER_ENABLE == ALL_TAMPER)
            (READ_REG(TAMP->CR1) != 0x01010080U) ||
#else
            (READ_REG(TAMP->CR1) != 0x01010000U) ||
#endif /* (TFM_TAMPER_ENABLE == ALL_TAMPER) */
            (READ_REG(TAMP->CR3) != 0x00000000U))
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_CH, FLOW_CTRL_TAMP_INT_CH);

        /*  Check tamper configuration secure only  */
        if (HAL_RTCEx_SecureModeGet(&RTCHandle, (RTC_SecureStateTypeDef *)&TamperSecureConfGet) != HAL_OK)
        {
            Error_Handler();
        }
        else if (boot_secure_memequal((void *)&TamperSecureConf, (void *)&TamperSecureConfGet, sizeof(TamperSecureConf)))
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_CH, FLOW_CTRL_TAMP_SEC_CH);

        /*  Check tamper configuration privileged only   */
        if (HAL_RTCEx_PrivilegeModeGet(&RTCHandle,(RTC_PrivilegeStateTypeDef *)&TamperPrivConfGet) != HAL_OK)
        {
            Error_Handler();
        }
        else if (boot_secure_memequal((void *)&TamperPrivConf, (void *)&TamperPrivConfGet, sizeof(TamperPrivConf)))
        {
            Error_Handler();
        }
        FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_CH, FLOW_CTRL_TAMP_PRIV_CH);

        /*  Check Secret Erase */
#if 0
        /* On U5 Cut 2 only */
        if (READ_BIT(TAMP->ERCFGR, TAMP_ERCFGR0) != TAMP_ERCFGR0)
        {
            Error_Handler();
        }
#endif
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_CH, FLOW_CTRL_TAMP_CFG_CH);
    }
#endif /*  TFM_TAMPER_ENABLE != NO_TAMPER */
}
/**
  * @}
  */
