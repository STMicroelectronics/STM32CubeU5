/**
  ******************************************************************************
  * @file    ns_data.c
  * @author  MCD Application Team
  * @brief   NS data module.
  *          This file provides examples of NS Data access.
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
#include "region_defs.h"
#include "ns_data.h"
#include "com.h"


#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)

/** @defgroup  NS_Data_Private_Defines Private Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup  NS_Data_Private_Functions Private Functions
  * @{
  */
static void NS_DATA_Print_menu(void);
static void NS_DATA_Display(void);

/**
  * @}
  */

/** @defgroup  NS_Data_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Display the NS Data Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void NS_DATA_Run(void)
{
  uint8_t key = 0;
  uint8_t exit = 0;

  /* Print NS Data welcome message */
  NS_DATA_Print_menu();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the user input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, COM_UART_TIMEOUT_MAX) == HAL_OK)
    {
      switch (key)
      {
        case '1' :
          NS_DATA_Display();
          break;
        case 'x':
          exit = 1;
          break;
        default:
          printf("Invalid Number !\r");
          break;
      }
      NS_DATA_Print_menu();
    }
  }
}
/**
  * @}
  */

/** @addtogroup  NS_Data_Private_Functions
  * @{
  */

/**
  * @brief  Display the NS Data Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void NS_DATA_Print_menu(void)
{
  printf("\r\n=============== NS Data Image Menu ===================\r\n\n");
  printf("  Display Data from NS Data Image ----------------------- 1\r\n\n");
  printf("  Previous Menu ----------------------------------------- x\r\n\n");
}

/**
  * @brief  Display the NS Data on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void NS_DATA_Display(void)
{
  uint8_t *data1;
  data1 = (uint8_t*)(FLASH_BASE_NS + NS_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + NS_DATA_IMAGE_DATA1_OFFSET);

  printf("  -- NS Data: %08x%08x..%08x%08x\r\n\n",
               *((uint32_t *)(&data1[0])),
               *((uint32_t *)(&data1[4])),
               *((uint32_t *)(&data1[NS_DATA_IMAGE_DATA1_SIZE - 8])),
               *((uint32_t *)(&data1[NS_DATA_IMAGE_DATA1_SIZE - 4]))
              );
}

/**
  * @}
  */

#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */