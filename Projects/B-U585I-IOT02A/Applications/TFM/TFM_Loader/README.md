## <b>TFM_Loader Application description</b>

This project is used to generate the TFM loader image file.
The TFM_Loader application consists in two distinct projects: the TFM_Loader
Secure and Non Secure projects.
It has to be compiled after all TFM other ptojects.

In order to ease the development process, prebuild and postbuild commands are
integrated in each toolchain project.
The prebuild command is in charge of preparing the scatter file according to common
flash layout description in linker folder.
The postbuild command is in charge of preparing the loader image.

### <b>Keywords</b>

Security, SBSFU, TFM, FLASH, TrustZone

### <b>Directory contents</b>

  - TFM_Loader/NonSecure/Inc/com.h                         Header file for com.c
  - TFM_Loader/NonSecure/Inc/common.h                      Header file for common.c
  - TFM_Loader/NonSecure/Inc/fw_update_app.h               Header file for fw_update_app.c
  - TFM_Loader/NonSecure/Inc/low_level_flash.h             Header file for low_level_flash.c
  - TFM_Loader/NonSecure/Inc/main.h                        Header file for main.c
  - TFM_Loader/NonSecure/Inc/stm32u5xx_hal_conf.h          HAL driver configuration file
  - TFM_Loader/NonSecure/Inc/ymodem.h                      Header file for ymodem.c

  - TFM_Loader/NonSecure/Src/com.c                         Com functionalities
  - TFM_Loader/NonSecure/Src/common.c                      Common functionalities
  - TFM_Loader/NonSecure/Src/dummy_nsc.c                   Dummy function
  - TFM_Loader/NonSecure/Src/fw_update_app.c               Firmware update functionalities
  - TFM_Loader/NonSecure/Src/low_level_device.c            Flash Low level device configuration
  - TFM_Loader/NonSecure/Src/low_level_flash.c             Flash Low level interface
  - TFM_Loader/NonSecure/Src/main.c                        Main application file
  - TFM_Loader/NonSecure/Src/startup_stm32u5xx.c           Startup file in c
  - TFM_Loader/NonSecure/Src/system_stm32u5xx.c            System Init file
  - TFM_Loader/NonSecure/Src/tick.c                        Tick functionalities
  - TFM_Loader/NonSecure/Src/ymodem.c                      Ymodem functionalities

  - TFM_Loader/Secure/Inc/low_level_flash.h                Header file for low_level_flash.c
  - TFM_Loader/Secure/Inc/main.h                           Header file for main.c
  - TFM_Loader/Secure/Inc/partition_stm32u585xx.h          Partition file
  - TFM_Loader/Secure/Inc/stm32u5xx_hal_conf.h             HAL driver configuration file

  - TFM_Loader/Secure/Src/low_level_device.c               Flash Low level device configuration
  - TFM_Loader/Secure/Src/low_level_flash.c                Flash Low level interface
  - TFM_Loader/Secure/Src/main.c                           Main application file
  - TFM_Loader/Secure/Src/secure_nsc.c                     Secure Non-secure callable functions
  - TFM_Loader/Secure/Src/startup_stm32u5xx.c              Startup file in c
  - TFM_Loader/Secure/Src/system_stm32u5xx.c               System Init file
  - TFM_Loader/Secure/Src/tick.c                           Tick functionalities

  - TFM_Loader/Secure_nsclib/secure_nsc.h                  Secure Non-Secure Callable (NSC) module header file

### <b>How to use it ?</b>

In order to build the TFM_Loader Secure and Non Secure projects, you must do the following:
 - Open your preferred toolchain
 - Rebuild the two projects

Then refer to TFM readme at upper level directory for TFM usage.
