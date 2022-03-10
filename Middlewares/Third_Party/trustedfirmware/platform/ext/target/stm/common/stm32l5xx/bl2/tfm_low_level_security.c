/**
  ******************************************************************************
  * @file    tfm_low_level_security.c
  * @author  MCD Application Team
  * @brief   security protection implementation for for secure boot on STM32L5xx
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
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
#include "mpu_armv8m_drv.h"
#include "region_defs.h"
#include "tfm_low_level_security.h"
#include "bootutil/bootutil_log.h"

/** @defgroup TFM_SECURITY_Private_Defines  Private Defines
  * @{
  */
/* DUAL BANK page size */
#define PAGE_SIZE 0x800
#define PAGE_MAX_NUMBER_IN_BANK 127
/* SBSFU_Boot Vector Address  */
#define SBSFU_BOOT_VTOR_ADDR ((uint32_t)(BL2_CODE_START))
/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup TFM_SECURITY_Private_Functions  Private Functions
  * @{
  */
#ifdef TFM_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel);
#endif /* TFM_OB_RDP_LEVEL_VALUE */
static void gtzc_init_cfg(void);
static void sau_and_idau_cfg(void);
static void mpu_init_cfg(void);
static void mpu_appli_cfg(void);
static void lock_bl2_shared_area(void);
static void enable_hdp_protection(void);
static void apply_wrp_sram2(uint32_t offset, uint32_t len);
/**
  * @}
  */

/** @defgroup TFM_SECURITY_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Apply the runtime security  protections to
  *
  * @param  None
  * @note   By default, the best security protections are applied
  * @retval None
  */
void TFM_LL_SECU_ApplyRunTimeProtections(void)
{
  /* Unsecure bottom of SRAM1 for error_handler */
  gtzc_init_cfg();

  /* Set MPU to forbidd execution outside of not muteable code  */
  /* Initialialize not secure MPU to forbidd execution on Flash /SRAM */
  mpu_init_cfg();

  /* Enable SAU to gain access to flash area non secure for write/read */
  sau_and_idau_cfg();

  /* Lock top of SRAM2 in secure */
  lock_bl2_shared_area();

#ifdef TFM_FLASH_PRIVONLY_ENABLE
  HAL_FLASHEx_ConfigPrivMode(FLASH_PRIV_DENIED);
#endif /*  TFM_FLASH_PRIVONLY_ENABLE */
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

/**
  * @brief  Update the runtime security protections for application start
  *
  * @param  None
  * @retval None
  */
void TFM_LL_SECU_UpdateRunTimeProtections(void)
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
#elif defined(__CC_ARM)
#pragma arm section code
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
void TFM_LL_SECU_CheckStaticProtections(void)
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
    BOOT_LOG_ERR("Error while checking TZEN value");
    Error_Handler();
  }

  /* Check if dual bank is set */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_DBANK) != FLASH_OPTR_DBANK)
  {
    BOOT_LOG_ERR("Error while checking dual bank configuration");
    Error_Handler();
  }

  /* Check secure boot address */
  if (flash_option_bytes_bank1.BootAddr != SBSFU_BOOT_VTOR_ADDR)
  {
    BOOT_LOG_INF("BootAddr 0x%x", flash_option_bytes_bank1.BootAddr);
    BOOT_LOG_ERR("Error while checking SEC BOOT Address");
    Error_Handler();
  }

#ifdef TFM_NSBOOT_CHECK_ENABLE
  /* Check non-secure boot addresses */
  if ((flash_option_bytes_nsboot0.BootAddr != SBSFU_BOOT_VTOR_ADDR)
      || (flash_option_bytes_nsboot1.BootAddr != SBSFU_BOOT_VTOR_ADDR))
  {
    BOOT_LOG_ERR("Error while checking NS BOOT Address");
    Error_Handler();
  }
#endif /* TFM_NSBOOT_CHECK_ENABLE */

  /* Check bank1 secure flash protection */
  start = FLASH_AREA_BL2_OFFSET / PAGE_SIZE;
  end = (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = PAGE_MAX_NUMBER_IN_BANK;
  }
  if ((flash_option_bytes_bank1.WMSecStartPage > flash_option_bytes_bank1.WMSecEndPage)
      || (start != flash_option_bytes_bank1.WMSecStartPage)
      || (end != flash_option_bytes_bank1.WMSecEndPage))
  {
    BOOT_LOG_INF("BANK 1 secure flash [%d, %d] : OB [%d, %d]",
                 start, end, flash_option_bytes_bank1.WMSecStartPage, flash_option_bytes_bank1.WMSecEndPage);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Error while checking secure flash protection");
    Error_Handler();
#else
    BOOT_LOG_ERR("Error while checking secure flash protection: set wmsec1");
    flash_option_bytes_bank1.WMSecStartPage = start;
    flash_option_bytes_bank1.WMSecEndPage = end;
    flash_option_bytes_bank1.OptionType |= OPTIONBYTE_WMSEC;
    flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_AREA1 | OB_WMSEC_SECURE_AREA_CONFIG;
#endif /* TFM_ENABLE_SET_OB */
  }

  /* Check bank2 secure flash protection */
  start = FLASH_AREA_BL2_OFFSET;
  end = (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = end - (PAGE_MAX_NUMBER_IN_BANK + 1);
    if ((start != flash_option_bytes_bank2.WMSecStartPage)
        || (end != flash_option_bytes_bank2.WMSecEndPage))
    {
      BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", start, end, flash_option_bytes_bank2.WMSecStartPage,
                   flash_option_bytes_bank2.WMSecEndPage);
#ifndef TFM_ENABLE_SET_OB
      BOOT_LOG_ERR("Error while checking secure flash protection");
      Error_Handler();
#else
      BOOT_LOG_ERR("Error while checking secure flash protection : set wmsec2");
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
    BOOT_LOG_ERR("Error while checking secure flash protection");
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
  start = FLASH_AREA_BL2_OFFSET / PAGE_SIZE;
  end = (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE +
         FLASH_AREA_BL2_NOHDP_SIZE - 1) / PAGE_SIZE;
  if ((flash_option_bytes_bank1.WRPStartOffset > flash_option_bytes_bank1.WRPEndOffset)
      || (start != flash_option_bytes_bank1.WRPStartOffset)
      || (end != flash_option_bytes_bank1.WRPEndOffset))
  {
    BOOT_LOG_INF("BANK 1 flash write protection [%d, %d] : OB [%d, %d]", start, end,
                 flash_option_bytes_bank1.WRPStartOffset,
                 flash_option_bytes_bank1.WRPEndOffset);
#ifndef TFM_ENABLE_SET_OB
    BOOT_LOG_ERR("Error while checking write protection ");
    Error_Handler();
#else
    flash_option_bytes_bank1.WRPStartOffset = start;
    flash_option_bytes_bank1.WRPEndOffset = end;
    flash_option_bytes_bank1.WRPArea |= OB_WRPAREA_BANK1_AREAA;

    BOOT_LOG_ERR("Error while checking write protection : set wrp1");
    flash_option_bytes_bank1.OptionType |= OPTIONBYTE_WRP;
#endif /* TFM_ENABLE_SET_OB */
  }
#endif /* TFM_WRP_PROTECT_ENABLE */

#ifdef  TFM_HDP_PROTECT_ENABLE
  /* Check secure user flash protection (HDP) */
  start = 0;
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
    BOOT_LOG_ERR("Error while checking secure user flash protection");
    Error_Handler();
#else
    BOOT_LOG_ERR("Error while checking secure user flash protection : set hdp1");
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
    BOOT_LOG_ERR("Error while checking OB for SRAM2 ERASE at Reset");
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
      BOOT_LOG_ERR("Error while checking OB RDP to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank2.OptionType & OPTIONBYTE_RDP) != 0)
    {
      BOOT_LOG_ERR("Error while checking OB RDP to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank2.WRPArea) != 0)
    {
      BOOT_LOG_ERR("Error while checking bank 2 OB WRP AREA to program");
      Error_Handler();
    }
    if ((flash_option_bytes_bank1.WRPArea & ~OB_WRPAREA_BANK1_AREAA) != 0)
    {
      BOOT_LOG_ERR("Error while checking bank 1 OB WRP AREA to program");
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
    BOOT_LOG_ERR("Error while checking SEC BOOT LOCK");
    Error_Handler();
  }
#endif /* TFM_OB_BOOT_LOCK */

#ifdef TFM_OB_RDP_LEVEL_VALUE
  /* Check RDL level */
  if (flash_option_bytes_bank1.RDPLevel != TFM_OB_RDP_LEVEL_VALUE)
  {
    BOOT_LOG_INF("RDPLevel 0x%x (0x%x)", flash_option_bytes_bank1.RDPLevel, TFM_OB_RDP_LEVEL_VALUE);
    BOOT_LOG_ERR("Error while checking RDP level");
    rdp_level(TFM_OB_RDP_LEVEL_VALUE);
    Error_Handler();
  }
#endif /* TFM_OB_RDP_LEVEL_VALUE */
}

#ifdef TFM_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel)
{
#ifdef TFM_ENABLE_SET_OB
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank = {0};
  HAL_StatusTypeDef ret = HAL_ERROR;

  flash_option_bytes_bank.OptionType = OPTIONBYTE_RDP;
  flash_option_bytes_bank.RDPLevel = rdplevel;
  BOOT_LOG_INF("Programming RDP to %x", rdplevel);

  /* Unlock the Flash to enable the flash control register access */
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes */
  HAL_FLASH_OB_Unlock();

  /* Program the Options Bytes */
  ret = HAL_FLASHEx_OBProgram(&flash_option_bytes_bank);
  if (ret != HAL_OK)
  {
    BOOT_LOG_ERR("Error while setting OB Bank config");
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

/**
  * @brief  Memory Config Init
  * @param  None
  * @retval None
  */
static void  gtzc_init_cfg(void)
{
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  /*  unsecure only one block in SRAM1 */
  GTZC_MPCBB1_S->VCTR[0] = 0xfffffffe;
#endif /*  TFM_ERROR_HANDLER_NON_SECURE */
}

/**
  * @brief  Sau idau init
  * @param  None
  * @retval None
  */
static void sau_and_idau_cfg(void)
{
  /* Disable SAU */
  TZ_SAU_Disable();
  /* Configures SAU regions non-secure to gain access to SRAM1 non secure   */
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  SAU->RNR  = 0;
  SAU->RBAR = (SRAM1_BASE_NS & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = ((SRAM1_BASE_NS + 0xff) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#endif /* TFM_ERROR_HANDLER_NON_SECURE */
  /* Configure RSS table */
  SAU->RNR  = 1;
  SAU->RBAR = (0x0BF90000 & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = ((0x0BF90000 + 0xffff) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  /* Allow non secure access to Flash non Secure peripheral for regression */
  SAU->RNR  = 2;
  SAU->RBAR = ((uint32_t)FLASH_NS & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (((uint32_t)FLASH_NS + 0xffff) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#endif /* TFM_ERROR_HANDLER_NON_SECURE */
  /* Allow non secure Flash base access */
#if defined(EXTERNAL_FLASH)
  SAU->RNR  = 3;
  SAU->RBAR = ((uint32_t)FLASH_BASE_NS + FLASH_AREA_1_OFFSET) & SAU_RBAR_BADDR_Msk;
  SAU->RLAR = (((uint32_t)FLASH_BASE_NS + FLASH_AREA_1_OFFSET
                + FLASH_AREA_1_SIZE - 1) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
  SAU->RNR  = 4;
  SAU->RBAR = ((uint32_t)OSPI_FLASH_BASE_ADDRESS + FLASH_AREA_2_OFFSET) & SAU_RBAR_BADDR_Msk;
  SAU->RLAR = (((uint32_t)OSPI_FLASH_BASE_ADDRESS + FLASH_AREA_3_OFFSET
                + FLASH_AREA_3_SIZE - 1) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#else
  SAU->RNR  = 3;
  SAU->RBAR = ((uint32_t)FLASH_BASE_NS + FLASH_AREA_1_OFFSET) & SAU_RBAR_BADDR_Msk;
  SAU->RLAR = (((uint32_t)FLASH_BASE_NS + FLASH_AREA_3_OFFSET
                + FLASH_AREA_3_SIZE - 1) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#endif
  /* Force memory writes before continuing */
  __DSB();
  /* Flush and refill pipeline with updated permissions */
  __ISB();
  /* Enable SAU */
  TZ_SAU_Enable();
}

/**
  * @brief  mpu init
  * @param  None
  * @retval None
  */
static void mpu_init_cfg(void)
{
#ifdef TFM_BOOT_MPU_PROTECTION
  struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  struct mpu_armv8m_dev_t dev_mpu_ns = { MPU_BASE_NS};
  struct mpu_armv8m_region_cfg_t region_cfg;
  /* Secure MPU */
  /* background region is enabled , secure execution on unsecure flash is not possible*/
  /* but non secure execution on unsecure flash is possible , non secure mpu is used to protect execution */
  /* since SAU is enabled later to gain access to non secure flash */
  /* Forbid execuction outside of flash write protected area  */
  /* descriptor 0 is set execute readonly before jumping in Secure application */
  region_cfg.region_nr = 0;
  region_cfg.region_base = FLASH_BASE_S + FLASH_AREA_0_OFFSET ;
  region_cfg.region_limit = FLASH_BASE_S + FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  region_cfg.region_nr = 1;
  region_cfg.region_base = FLASH_BASE_S + FLASH_AREA_1_OFFSET ;
  region_cfg.region_limit = FLASH_BASE_S + FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  region_cfg.region_nr = 2;
  region_cfg.region_base = FLASH_BASE_S;
  region_cfg.region_limit = FLASH_BASE_S + FLASH_AREA_BL2_OFFSET - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  region_cfg.region_nr = 3;
  region_cfg.region_base = FLASH_BASE_S + FLASH_AREA_BL2_NOHDP_OFFSET + FLASH_AREA_BL2_NOHDP_SIZE;
  region_cfg.region_limit = FLASH_BASE_S + FLASH_AREA_0_OFFSET - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* Forbid execuction on full SRAM area */
  region_cfg.region_nr = 4;
  region_cfg.region_base = SRAM1_BASE_S ;
  region_cfg.region_limit = SRAM1_BASE_S + TOTAL_RAM_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* forbid secure peripheral execution */
  region_cfg.region_nr = 5;
  region_cfg.region_base = PERIPH_BASE_S;
  region_cfg.region_limit = PERIPH_BASE_S + 0xFFFFFFF;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* enable secure MPU */
  mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                    HARDFAULT_NMI_ENABLE);
  /* forbid execution on non secure FLASH /RAM in case of jump in non secure */
  /* Non Secure MPU  background all access in priviligied */
  /* reduced execution to all flash during control */
  region_cfg.region_nr = 0;
  region_cfg.region_base = FLASH_BASE_NS + FLASH_AREA_1_OFFSET;
  region_cfg.region_limit = FLASH_BASE_NS + FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  region_cfg.region_nr = 1;
  region_cfg.region_base = FLASH_BASE_NS + FLASH_AREA_2_OFFSET;
  region_cfg.region_limit = FLASH_BASE_NS + FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* Forbid execuction on full SRAM area */
  region_cfg.region_nr = 2;
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  region_cfg.region_base = SRAM1_BASE_NS + (~MPU_RBAR_BASE_Msk) + 1;
#else
  region_cfg.region_base = SRAM1_BASE_NS ;
#endif /*   TFM_ERROR_HANDLER_NON_SECURE */
  region_cfg.region_limit = SRAM1_BASE_NS + TOTAL_RAM_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* forbid secure peripheral execution */
  region_cfg.region_nr = 3;
  region_cfg.region_base = PERIPH_BASE_NS;
  region_cfg.region_limit = PERIPH_BASE_NS + 0xFFFFFFF;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* enable non secure MPU */
  mpu_armv8m_enable(&dev_mpu_ns, PRIVILEGED_DEFAULT_ENABLE,
                    HARDFAULT_NMI_ENABLE);
#endif /* TFM_BOOT_MPU_PROTECTION */
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

/**
  * @brief  mpu configuration for running secure/non secure application
  * @param  None
  * @retval None
  */
static void mpu_appli_cfg(void)
{
#ifdef TFM_BOOT_MPU_PROTECTION
  /* static variables are used to ensure rodata placement in the specific section */
  static struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  static struct mpu_armv8m_dev_t dev_mpu_ns = { MPU_BASE_NS};
  struct mpu_armv8m_region_cfg_t region_cfg;
  /* region 0 is now enable for execution */
  region_cfg.region_nr = 0;
  region_cfg.region_base = FLASH_BASE_S + FLASH_AREA_0_OFFSET ;
  region_cfg.region_limit = FLASH_BASE_S + FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_ONLY;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
  if (mpu_armv8m_region_enable(&dev_mpu_s, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
  /* region 0 is now enable for execution */
  region_cfg.region_nr = 0;
  region_cfg.region_base = FLASH_BASE_NS + FLASH_AREA_1_OFFSET;
  region_cfg.region_limit = FLASH_BASE_NS + FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE - 1;
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
#ifdef TFM_ERROR_HANDLER_NON_SECURE
  region_cfg.region_base = SRAM1_BASE_NS ;
  /* Forbid execution on full SRAM area */
  region_cfg.region_nr = 4;
  region_cfg.region_base = SRAM1_BASE_NS ;
  region_cfg.region_limit = SRAM1_BASE_NS + (~MPU_RBAR_BASE_Msk);
  region_cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
  region_cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
  region_cfg.attr_sh = MPU_ARMV8M_SH_NONE;
  region_cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
  if (mpu_armv8m_region_enable(&dev_mpu_ns, &region_cfg) != MPU_ARMV8M_OK)
  {
    Error_Handler();
  }
#endif /*   TFM_ERROR_HANDLER_NON_SECURE */
#endif /* TFM_BOOT_MPU_PROTECTION */
}

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */

/**
  * @brief  lock bl2 shared sram in secure
  * @param  None
  * @retval None
  */
static void lock_bl2_shared_area(void)
{
  MPCBB_ConfigTypeDef MPCBB_desc;
  /* assumption shared area in SRAM2 in the last 8 Kbytes super block */
  /*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
  if (
    (BOOT_TFM_SHARED_DATA_BASE >= S_RAM_ALIAS(_SRAM2_TOP - GTZC_MPCBB_SUPERBLOCK_SIZE))
    && (BOOT_TFM_SHARED_DATA_SIZE <= GTZC_MPCBB_SUPERBLOCK_SIZE))
  {

    __HAL_RCC_GTZC_CLK_ENABLE();
    if (HAL_GTZC_MPCBB_GetConfigMem(SRAM2_BASE, &MPCBB_desc) != HAL_OK)
    {
      Error_Handler();
    }
    MPCBB_desc.AttributeConfig.MPCBB_LockConfig_array[0] |= 0x00000080;
    if (HAL_GTZC_MPCBB_ConfigMem(SRAM2_BASE, &MPCBB_desc) != HAL_OK)
    {
      Error_Handler();
    }
  }
  else Error_Handler();
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
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
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
static void apply_wrp_sram2(uint32_t offset, uint32_t len)
{
  uint32_t start_offset = ((offset - SRAM2_BASE_S)/ SRAM2_PAGE_SIZE);
  uint32_t end_offset = start_offset + (len -1)/SRAM2_PAGE_SIZE;
  uint32_t index;
  __IO uint32_t *pt;
  __IO uint32_t *SRAM2_CFG[2]={&SYSCFG_S->SWPR, &SYSCFG_S->SWPR2};
  uint32_t val[2]={0, 0};

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  for (index = start_offset; index <= end_offset; index ++)
  {
    val[(index > 31) ? 1 : 0]|= (1 << ( (index > 31) ? (index -32) : index));
  }
  for(index = 0; index < 2; index ++)
  {
    pt = SRAM2_CFG[index];
    *pt = val[index];
  }
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */
/**
  * @}
  */
