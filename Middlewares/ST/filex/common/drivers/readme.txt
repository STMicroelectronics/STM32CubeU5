/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          FileX low level drivers for STM32 devices.
  ******************************************************************************
  */

### V2.0.0 / 21-June-2021 ###
=================================
Main changes
-------------
- Decouple the SD/MMC drivers from explicit dependency to BSP API
- Add 'fx_stm32_xxx_driver_glue.c' files for HAL API (DMA, IT, Polling)
- Add 'fx_stm32_xx_driver.h' templates for RTOS and Baremetal modes
- Fix GNU GCC warnings

Dependencies:
-------------
- Azure RTOS FileX V6.1.7
- Azure RTOS LevelX V6.1.7

### V1.0.0 / 25-February-2021 ###
=================================
Main changes
-------------
- First official release of Azure RTOS FileX low level drivers for STM32 MCU series

Dependencies:
-------------
- Azure RTOS FileX V6.1.3
- Azure RTOS LevelX V6.1.3
- STM32Cube SD, MMC BSP drivers
