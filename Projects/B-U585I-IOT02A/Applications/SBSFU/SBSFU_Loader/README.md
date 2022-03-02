## <b>SBSFU_Loader Application description</b>

This project is used to generate the SBSFU loader image file.
The SBSFU_Loader application consists in two distinct projects: the SBSFU_Loader
Secure and Non Secure projects.
It has to be compiled after all SBSFU other projects.

In order to ease the development process, prebuild and postbuild commands are
integrated in each toolchain project.
The prebuild command is in charge of preparing the scatter file according to common
flash layout description in linker folder.
The postbuild command is in charge of preparing the loader image.

### <b>Keywords</b>

Security, SBSFU, TFM, FLASH, TrustZone

### <b>Directory contents</b>

  - SBSFU_Loader/NonSecure/Inc/com.h                         Header file for com.c
  - SBSFU_Loader/NonSecure/Inc/common.h                      Header file for common.c
  - SBSFU_Loader/NonSecure/Inc/fw_update_app.h               Header file for fw_update_app.c
  - SBSFU_Loader/NonSecure/Inc/low_level_flash.h             Header file for low_level_flash.c
  - SBSFU_Loader/NonSecure/Inc/main.h                        Header file for main.c
  - SBSFU_Loader/NonSecure/Inc/stm32u5xx_hal_conf.h          HAL driver configuration file
  - SBSFU_Loader/NonSecure/Inc/ymodem.h                      Header file for ymodem.c

  - SBSFU_Loader/NonSecure/Src/com.c                         Com functionalities
  - SBSFU_Loader/NonSecure/Src/common.c                      Common functionalities
  - SBSFU_Loader/NonSecure/Src/dummy_nsc.c                   Dummy function
  - SBSFU_Loader/NonSecure/Src/fw_update_app.c               Firmware update functionalities
  - SBSFU_Loader/NonSecure/Src/low_level_device.c            Flash Low level device configuration
  - SBSFU_Loader/NonSecure/Src/low_level_flash.c             Flash Low level interface
  - SBSFU_Loader/NonSecure/Src/main.c                        Main application file
  - SBSFU_Loader/NonSecure/Src/startup_stm32u5xx.c           Startup file in c
  - SBSFU_Loader/NonSecure/Src/system_stm32u5xx.c            System Init file
  - SBSFU_Loader/NonSecure/Src/tick.c                        Tick functionalities
  - SBSFU_Loader/NonSecure/Src/ymodem.c                      Ymodem functionalities

  - SBSFU_Loader/Secure/Inc/low_level_flash.h                Header file for low_level_flash.c
  - SBSFU_Loader/Secure/Inc/main.h                           Header file for main.c
  - SBSFU_Loader/Secure/Inc/partition_stm32u585xx.h          Partition file
  - SBSFU_Loader/Secure/Inc/stm32u5xx_hal_conf.h             HAL driver configuration file

  - SBSFU_Loader/Secure/Src/low_level_device.c               Flash Low level device configuration
  - SBSFU_Loader/Secure/Src/low_level_flash.c                Flash Low level interface
  - SBSFU_Loader/Secure/Src/main.c                           Main application file
  - SBSFU_Loader/Secure/Src/secure_nsc.c                     Secure Non-secure callable functions
  - SBSFU_Loader/Secure/Src/startup_stm32u5xx.c              Startup file in c
  - SBSFU_Loader/Secure/Src/system_stm32u5xx.c               System Init file
  - SBSFU_Loader/Secure/Src/tick.c                           Tick functionalities

  - SBSFU_Loader/Secure_nsclib/secure_nsc.h                  Secure Non-Secure Callable (NSC) module header file

### <b>How to use it ?</b>

In order to build the SBSFU_Loader Secure and Non Secure projects, you must do the following:
 - Open your preferred toolchain
 - Rebuild the two projects

Then refer to SBSFU readme at upper level directory for SBSFU usage.
