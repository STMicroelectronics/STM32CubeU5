/**
  ******************************************************************************
  * @file    fw_update_app.c
  * @author  MCD Application Team
  * @brief   Firmware Update module.
  *          This file provides set of firmware functions to manage Firmware
  *          Update functionalities.
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

#include "stm32u5xx_hal.h"
#include "com.h"
#include "common.h"
/*#include "low_level_flash.h"*/
#include "ymodem.h"
#include "fw_update_app.h"
#include "region_defs.h"
#include "Driver_Flash.h"
#include "string.h"
#include "tfm_platform_api.h"
#include "platform_ioctl.h"

#if   !defined(MCUBOOT_PRIMARY_ONLY)
/** @addtogroup USER_APP User App Example
  * @{
  */
extern ARM_DRIVER_FLASH LOADER_FLASH_DEV_NAME;
/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */
/** @defgroup  FW_UPDATE_Private_Variables Private Variables
  * @{
  */
static uint32_t m_uFileSizeYmodem = 0U;    /* !< Ymodem File size*/
static uint32_t m_uNbrBlocksYmodem = 0U;   /* !< Ymodem Number of blocks*/
static uint32_t m_uPacketsReceived = 0U;   /* !< Ymodem packets received*/
static uint32_t m_uFlashSectorSize = 0U;   /* !< Flash Sector Size */
static uint32_t m_uFlashMinWriteSize = 0U; /* !< FLash Min Write access*/
/** @defgroup  FW_UPDATE_Private_Const Private Const
  * @{
  */
const uint32_t MagicTrailerValue[] =
{
  0xf395c277,
  0x7fefd260,
  0x0f505235,
  0x8079b62c,
};

/**
  * @}
  */

/** @defgroup  FW_UPDATE_Private_Functions Private Functions
  * @{
  */
static void FW_UPDATE_PrintWelcome(void);
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea);
static HAL_StatusTypeDef FW_UPDATE_SECURE_APP_IMAGE(void);
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_APP_IMAGE(void);
#if !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_SecureAppImage(void);
static void FW_Install_SecureAppImage(void);
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_AppImage(void);
static void FW_Install_AppImage(void);
#endif /* defined(MCUBOOT_OVERWRITE_ONLY) */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
static HAL_StatusTypeDef FW_UPDATE_SECURE_DATA_IMAGE(void);
#if !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_SecureDataImage(void);
static void FW_Install_SecureDataImage(void);
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_DATA_IMAGE(void);
#if !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_NonSecureDataImage(void);
static void FW_Install_NonSecureDataImage(void);
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */
/**
  * @}
  */
/** @defgroup  FW_UPDATE_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup  FW_UPDATE_Control_Functions Control Functions
  * @{
   */
void FW_UPDATE_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;

  /* Print Firmware Update welcome message */
  FW_UPDATE_PrintWelcome();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      switch (key)
      {
      case '1' :
          printf("  -- Install image : reboot\r\n\n");
          NVIC_SystemReset();
          break;
      case '2' :
          FW_UPDATE_SECURE_APP_IMAGE();
          break;
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
      case '3' :
          FW_UPDATE_NONSECURE_APP_IMAGE();
          break;
#endif /*  (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
      case '4' :
          FW_UPDATE_SECURE_DATA_IMAGE();
          break;
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
      case '5' :
          FW_UPDATE_NONSECURE_DATA_IMAGE();
          break;
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2)
      case '6':
          FW_Valid_SecureAppImage();
          break;
      case '7':
          FW_Valid_AppImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1)
      case '6':
          FW_Valid_AppImage();
          break;
#endif /* defined(MCUBOOT_OVERWRITE_ONLY) && defined(MCUBOOT_APP_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
      case '8':
          FW_Valid_SecureDataImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
      case '9':
          FW_Valid_NonSecureDataImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2)
      case 'a':
          FW_Install_SecureAppImage();
          break;
      case 'b':
          FW_Install_AppImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && defined(MCUBOOT_APP_IMAGE_NUMBER == 2)  */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1)
      case 'a':
          FW_Install_AppImage();
          break;
#endif /* defined(MCUBOOT_OVERWRITE_ONLY) && defined(MCUBOOT_APP_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
      case 'c':
          FW_Install_SecureDataImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
      case 'd':
          FW_Install_NonSecureDataImage();
          break;
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */
      case 'x' :
          exit = 1U;
          break;
      default:
          printf("Invalid Number !\r");
          break;
      }
      /* Print Main Menu message */
      FW_UPDATE_PrintWelcome();
    }
  }
}

/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_SECURE_APP_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
  printf("Download Secure App Image\r\n");
#else
  printf("Download App Image\r\n");
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_2_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_AREA_2_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
    printf("  -- Secure App Image correctly downloaded \r\n\n");
#else
    printf("  -- App Image correctly downloaded \r\n\n");
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
    HAL_Delay(1000U);
  }

  return ret;
}
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_APP_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download NonSecure App Image\r\n");

  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_3_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_NS_PARTITION_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- NonSecure App Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}
#if  !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_SecureAppImage(void)
{
    tfm_platform_ioctl(PLATFORM_IOTCL_FWSEC_CONFIRM, NULL, NULL);
    printf("  -- Secure App Firmware Confirm Done\r\n\n");
}

/**
  * @brief  Write Magic Flag to trigger installation from Download Slot
  * @brief  - After Reset, Version not Confirm are recopied in download slot
  * @brief  - Calling this API triggers the re-installation
  * @param  None
  * @retval None
  */
static void FW_Install_SecureAppImage(void)
{

  uint32_t MagicAddress = FLASH_AREA_2_OFFSET  + FLASH_AREA_2_SIZE - sizeof(MagicTrailerValue);
  /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
  printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
  printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
  if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
  {
     printf(" Write Magic Trailer: Failed \r\n\n");
  }
}
#endif /* MCUBOOT_OVERWRITE_ONLY */
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */

#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_SECURE_DATA_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download Secure Data Image\r\n");
  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_6_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_AREA_6_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- Secure Data Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}
#if !defined(MCUBOOT_OVERWRITE_ONLY)
static void FW_Valid_SecureDataImage(void)
{
    tfm_platform_ioctl(PLATFORM_IOTCL_DATASEC_CONFIRM, NULL, NULL);
    printf("  -- Secure Data Firmware Confirm Done\r\n\n");
}
/**
  * @brief  Write Magic Flag to trigger installation from Download Slot
  * @brief  - After Reset, Version not Confirm are recopied in download slot
  * @brief  - Calling this API triggers the re-installation
  * @param  None
  * @retval None
  */

static void FW_Install_SecureDataImage(void)
{
  uint32_t MagicAddress = FLASH_AREA_6_OFFSET  + FLASH_AREA_6_SIZE - sizeof(MagicTrailerValue);
  /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
  printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
  printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
  if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
  {
     printf(" Write Magic Trailer: Failed \r\n\n");
  }
}
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */

#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_DATA_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download NonSecure Data Image\r\n");
  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_7_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_AREA_7_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- NonSecure Data Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}

#if !defined(MCUBOOT_OVERWRITE_ONLY)
/**
  * @brief  Write Confirm Flag for  :
  * @brief  - NonSecure Data image
  * @param  None
  * @retval None
  */
static void FW_Valid_NonSecureDataImage(void)
{
  const uint8_t FlagPattern[]={0x1 ,0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff };
  const uint32_t ConfirmAddress = FLASH_AREA_5_OFFSET  + FLASH_AREA_5_SIZE - (sizeof(MagicTrailerValue) + sizeof(FlagPattern));
  if (FLASH_PRIMARY_DATA_NONSECURE_DEV_NAME.ProgramData(ConfirmAddress, FlagPattern, sizeof(FlagPattern)) == ARM_DRIVER_OK)
  {
#if defined(__ARMCC_VERSION)
    printf("  --  Confirm Flag  correctly written %x %x \r\n\n",ConfirmAddress ,FlagPattern[0] );
#else
    printf("  --  Confirm Flag  correctly written %lx %x \r\n\n",ConfirmAddress , FlagPattern[0] );
#endif
  }
  else
  {
    printf("  -- Confirm Flag Not Correctlty Written \r\n\n");
  }
}

/**
  * @brief  Write Magic Flag to trigger installation from Download Slot
  * @brief  - After Reset, Version not Confirm are recopied in download slot
  * @brief  - Calling this API triggers the re-installation
  * @param  None
  * @retval None
  */
static void FW_Install_NonSecureDataImage(void)
{
  uint32_t MagicAddress = FLASH_AREA_7_OFFSET  + FLASH_AREA_7_SIZE - sizeof(MagicTrailerValue);
  /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
  printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
  printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
  if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
  {
     printf(" Write Magic Trailer: Failed \r\n\n");
  }
}
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup  FW_UPDATE_Private_Functions
  * @{
  */

/**
  * @brief  Display the FW_UPDATE Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_UPDATE_PrintWelcome(void)
{
  printf("\r\n================ New Fw Image ============================\r\n\n");
  printf("  Reset to trigger Installation ------------------------- 1\r\n\n");
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
  printf("  Download Secure App Image ----------------------------- 2\r\n\n");
  printf("  Download NonSecure App Image -------------------------- 3\r\n\n");
#else
  printf("  Download App Image ------------------------------------ 2\r\n\n");
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  printf("  Download Secure Data Image ---------------------------- 4\r\n\n");
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
  printf("  Download NonSecure Data Image ------------------------- 5\r\n\n");
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2)
  printf("  Validate Secure App Image ----------------------------- 6\r\n\n");
  printf("  Validate NonSecure App Image -------------------------- 7\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1)
  printf("  Validate App Image ------------------------------------ 6\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  printf("  Validate Secure Data Image ---------------------------- 8\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
  printf("  Validate NonSecure Data Image ------------------------- 9\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2)
  printf("  Re-install Secure App Image --------------------------- a\r\n\n");
  printf("  Re-install NonSecure App Image ------------------------ b\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1)
  printf("  Re-install App Image ---------------------------------- a\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
  printf("  Re-install Secure Data Image -------------------------- c\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
  printf("  Re-install NonSecure Data Image ----------------------- d\r\n\n");
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
  printf("  Previous Menu ----------------------------------------- x\r\n\n");
}
/**
  * @brief Download a new Firmware from the host.
  * @retval HAL status
  */
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  COM_StatusTypeDef e_result;
  int32_t ret_arm;
  uint32_t u_fw_size = pFwImageDwlArea->MaxSizeInBytes ;
  uint32_t sector_address;

  /* Clear download area */
  printf("  -- Erasing download area \r\n\n");

  for (sector_address = pFwImageDwlArea->DownloadAddr;
       sector_address < pFwImageDwlArea->DownloadAddr + pFwImageDwlArea->MaxSizeInBytes;
       sector_address += m_uFlashSectorSize)
  {
    ret_arm = LOADER_FLASH_DEV_NAME.EraseSector(sector_address);
    if (ret_arm < 0)
    {
      return HAL_ERROR;
    }
  }

  printf("  -- Send Firmware \r\n\n");

  /* Download binary */
  printf("  -- -- File> Transfer> YMODEM> Send \t\n");

  /*Init of Ymodem*/
  Ymodem_Init();

  /*Receive through Ymodem*/
  e_result = Ymodem_Receive(&u_fw_size, pFwImageDwlArea->DownloadAddr);
  printf("\r\n\n");

  if ((e_result == COM_OK))
  {
    printf("  -- -- Programming Completed Successfully!\r\n\n");
#if defined(__ARMCC_VERSION)
    printf("  -- -- Bytes: %u\r\n\n", u_fw_size);
#else
    printf("  -- -- Bytes: %lu\r\n\n", u_fw_size);
#endif /*  __ARMCC_VERSION */
    ret = HAL_OK;
    if (u_fw_size <= (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue)))
    {
      uint32_t MagicAddress =
        pFwImageDwlArea->DownloadAddr + (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue));
      /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
      printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
      printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
      if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
      {
        ret = HAL_ERROR;
      }
    }
  }
  else if (e_result == COM_ABORT)
  {
    printf("  -- -- !!Aborted by user!!\r\n\n");
    COM_Flush();
    ret = HAL_ERROR;
  }
  else
  {
    printf("  -- -- !!Error during file download!!\r\n\n");
    ret = HAL_ERROR;
    HAL_Delay(500U);
    COM_Flush();
  }

  return ret;
}


/**
  * @}
  */

/** @defgroup FW_UPDATE_Callback_Functions Callback Functions
  * @{
  */

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  uFileSize Dimension of the file that will be received (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{
  /*Reset of the ymodem variables */
  m_uFileSizeYmodem = 0U;
  m_uPacketsReceived = 0U;
  m_uNbrBlocksYmodem = 0U;

  /*Filesize information is stored*/
  m_uFileSizeYmodem = uFileSize;

  /* compute the number of 1K blocks */
  m_uNbrBlocksYmodem = (m_uFileSizeYmodem + (PACKET_1K_SIZE - 1U)) / PACKET_1K_SIZE;

  /* NOTE : delay inserted for Ymodem protocol*/
  HAL_Delay(1000);
  return HAL_OK;
}

extern uint32_t total_size_received;
/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  uSize Packet dimension (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{
  int32_t ret;
  m_uPacketsReceived++;

  /*Increase the number of received packets*/
  if (m_uPacketsReceived == m_uNbrBlocksYmodem) /*Last Packet*/
  {
    /*Extracting actual payload from last packet*/
    if (0 == (m_uFileSizeYmodem % PACKET_1K_SIZE))
    {
      /* The last packet must be fully considered */
      uSize = PACKET_1K_SIZE;
    }
    else
    {
      /* The last packet is not full, drop the extra bytes */
      uSize = m_uFileSizeYmodem - ((uint32_t)(m_uFileSizeYmodem / PACKET_1K_SIZE) * PACKET_1K_SIZE);
    }

    m_uPacketsReceived = 0U;
  }
  /*Adjust dimension to 64-bit length */
  if (uSize %  m_uFlashMinWriteSize != 0U)
  {
    memset(&pData[uSize], 0xff, (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize)));
    uSize += (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize));
  }
  /* Write Data in Flash - size has to be 64-bit aligned */
#if defined(MCUBOOT_PRIMARY_ONLY)
  if (uFlashDestination < NS_IMAGE_PRIMARY_PARTITION_OFFSET)
  {
    ret = SECURE_Flash_ProgramData(uFlashDestination, pData, uSize);
  }
  else
    ret = LOADER_FLASH_DEV_NAME.ProgramData(uFlashDestination, pData, uSize);
#else
  ret = LOADER_FLASH_DEV_NAME.ProgramData(uFlashDestination, pData, uSize);
#endif /*defined(MCUBOOT_PRIMARY_ONLY) */
  if (ret != ARM_DRIVER_OK)
  {
    /*Reset of the ymodem variables */
    m_uFileSizeYmodem = 0U;
    m_uPacketsReceived = 0U;
    m_uNbrBlocksYmodem = 0U;
    return HAL_ERROR;
  }
  else
    return HAL_OK;
}
#if !defined(MCUBOOT_OVERWRITE_ONLY)
/**
  * @brief  Write Confirm Flag for  :
  * @brief  - Secure & NonSecure Image in 1 image config
  * @brief  - NonSecure image in  2 images config
  * @param  None
  * @retval None
  */
static void FW_Valid_AppImage(void)
{
  const uint8_t FlagPattern[]={0x1 ,0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff };
#if (MCUBOOT_APP_IMAGE_NUMBER == 1)
  const uint32_t ConfirmAddress = FLASH_AREA_0_OFFSET  + FLASH_PARTITION_SIZE - (sizeof(MagicTrailerValue) + sizeof(FlagPattern));
#else
  const uint32_t ConfirmAddress = FLASH_AREA_1_OFFSET + FLASH_NS_PARTITION_SIZE - (sizeof(MagicTrailerValue) + sizeof(FlagPattern));
#endif
  if (FLASH_PRIMARY_NONSECURE_DEV_NAME.ProgramData(ConfirmAddress, FlagPattern, sizeof(FlagPattern)) == ARM_DRIVER_OK)
  {
#if defined(__ARMCC_VERSION)
    printf("  --  Confirm Flag  correctly written %x %x \r\n\n",ConfirmAddress ,FlagPattern[0] );
#else
    printf("  --  Confirm Flag  correctly written %lx %x \r\n\n",ConfirmAddress , FlagPattern[0] );
#endif
  }
  else
  {
    printf("  -- Confirm Flag Not Correctlty Written \r\n\n");
  }
}

/**
  * @brief  Write Magic Flag to trigger installation from Download Slot
  * @brief  - After Reset, Version not Confirm are recopied in download slot
  * @brief  - Calling this API triggers the re-installation
  * @param  None
  * @retval None
  */
static void FW_Install_AppImage(void)
{
#if (MCUBOOT_APP_IMAGE_NUMBER == 1)
  uint32_t MagicAddress = FLASH_AREA_2_OFFSET  + FLASH_PARTITION_SIZE - sizeof(MagicTrailerValue);
#else
  uint32_t MagicAddress = FLASH_AREA_3_OFFSET + FLASH_NS_PARTITION_SIZE - sizeof(MagicTrailerValue);
#endif
  /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
  printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
  printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
  if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
  {
     printf(" Write Magic Trailer: Failed \r\n\n");
  }
}
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* !defined(MCUBOOT_PRIMARY_ONLY) */
