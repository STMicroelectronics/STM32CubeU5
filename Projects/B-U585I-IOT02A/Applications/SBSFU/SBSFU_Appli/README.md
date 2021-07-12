## <b>SBSFU_Appli Application description</b>

The SBSFU_Appli application consists in two distinct projects: the SBSFU_Appli
Secure and SBSFU_Appli Non Secure projects.

The SBSFU_Appli Secure project is used to generate the SBSFU_Appli Secure binary
file. The core function of this application relies on the trusted firmware (TFM)
middleware and the mbed-crypto middleware.
It has to be compiled after TFM_SBSFU_Boot project.

The SBSFU_Appli Non Secure project is used to generate the SBSFU_Appli Non Secure
binary file. The core function of this application relies on the trusted
firmware (TFM) middleware.
It has to be compiled after SBSFU_Appli Secure project.

In order to ease the development process, prebuild and postbuild commands are
integrated in each toolchain for both projects.
The prebuild command is in charge of preparing the scatter file according to
common flash layout description in linker folder.
The postbuild command is in charge of signing the binaries, so that they are
trusted by firmware update process.

### <b>Keywords</b>

Security, TFM, mbedCrypto, AES, CRYP, GTZC, HASH, PKA,
Initial attestation, Secure storage, Internal trusted storage,
Cryptography, PSA, TrustZone

### <b>Directory contents</b>

  - SBSFU_Appli/Secure/Inc/low_level_flash.h       Header file for low_level_flash.c
  - SBSFU_Appli/Secure/Inc/main.h                  Header file for main.c
  - SBSFU_Appli/Secure/Inc/partition_stm32u585xx.h STM32U5xx Device System Configuration file
  - SBSFU_Appli/Secure/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - SBSFU_Appli/Secure/Inc/stm32u5xx_it.h          Header file for stm32u5xx_it.h

  - SBSFU_Appli/Secure/Src/low_level_device.c      Flash Low level device setting
  - SBSFU_Appli/Secure/Src/low_level_flash.c       Flash Low level interface
  - SBSFU_Appli/Secure/Src/main.c                  Secure Main program
  - SBSFU_Appli/Secure/Src/secure_nsc.c            Secure Non-secure callable functions
  - SBSFU_Appli/Secure/Src/startup_stm32u5xx.c     Startup file in c
  - SBSFU_Appli/Secure/Src/stm32u5xx_hal_msp.c     Secure HAL MSP module
  - SBSFU_Appli/Secure/Src/stm32u5xx_it.c          Secure Interrupt handlers
  - SBSFU_Appli/Secure/Src/system_stm32u5xx_s.c    Secure STM32U5xx system clock configuration file

  - SBSFU_Appli/NonSecure/Inc/cmsis.h              Header file for CMSIS
  - SBSFU_Appli/NonSecure/Inc/com.h                Header file for com.c
  - SBSFU_Appli/NonSecure/Inc/common.h             Header file for common.c
  - SBSFU_Appli/NonSecure/Inc/fw_update_app.h      Header file for fw_update.c
  - SBSFU_Appli/NonSecure/Inc/low_level_flash.h    Header file for low_level_flash.c
  - SBSFU_Appli/NonSecure/Inc/main.h               Header file for main.c
  - SBSFU_Appli/NonSecure/Inc/mpu_armv8m_drv.h     Header file for mpu_armv8m_drv.c
  - SBSFU_Appli/NonSecure/Inc/stm32u5xx_hal_conf.h HAL configuration file
  - SBSFU_Appli/NonSecure/Inc/stm32u5xx_it.h       Header file for stm32u5xx_it.c
  - SBSFU_Appli/NonSecure/Inc/test_protections.h   Header file for test_protections.c
  - SBSFU_Appli/NonSecure/Inc/ymodem.h             Header file for ymodem.c

  - SBSFU_Appli/NonSecure/Src/com.c                Uart low level interface
  - SBSFU_Appli/NonSecure/Src/common.c             Uart common functionalities
  - SBSFU_Appli/NonSecure/Src/fw_update_app.c      Firmware update example
  - SBSFU_Appli/NonSecure/Src/low_level_device.c   Flash Low level device setting
  - SBSFU_Appli/NonSecure/Src/low_level_flash.c    Flash Low level interface
  - SBSFU_Appli/NonSecure/Src/main.c               Main application file
  - SBSFU_Appli/NonSecure/Src/mpu_armv8m_drv.c     MPU low level interface
  - SBSFU_Appli/NonSecure/Src/startup_stm32u5xx.c  Startup file in c
  - SBSFU_Appli/NonSecure/Src/stm32u5xx_it.c       Interrupt handlers
  - SBSFU_Appli/NonSecure/Src/system_stm32u5xx.c   System init file
  - SBSFU_Appli/NonSecure/Src/test_protections.c   Test protections example
  - SBSFU_Appli/NonSecure/Src/ymodem.c             Ymodem support

### <b>How to use it ?</b>

In order to build the SBSFU_Appli Secure and Non Secure projects, you must do the
following:
 - Open your preferred toolchain
 - Rebuild the two projects

Then refer to SBSFU readme at upper level directory for SBSFU usage.
