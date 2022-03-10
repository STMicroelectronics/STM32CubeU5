## <b>TFM_Appli Application description</b>

The TFM_Appli application consists in two distinct projects: the TFM_Appli
Secure and TFM_Appli Non Secure projects.

The TFM_Appli Secure project is used to generate the TFM_Appli Secure binary file.
The core function of this application relies on the trusted firmware (TFM) middleware and
the mbed-crypto middleware.
Optionally, it may also rely on the STSAFE middleware (STSAFE_A) and its secure element.
It has to be compiled after TFM_SBSFU_Boot project.

The TFM_Appli Non Secure project is used to generate the TFM_Appli Non Secure binary file.
The core function of this application relies on the trusted firmware (TFM) middleware.
It has to be compiled after TFM_Appli Secure project.

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

  - TFM_Appli/Secure/Inc/aes_alt.h               Header file for aes_alt.c
  - TFM_Appli/Secure/Inc/ccm_alt.h               Header file for ccm_alt.c
  - TFM_Appli/Secure/Inc/cmsis.h                 Header file for CMSIS
  - TFM_Appli/Secure/Inc/ecp_alt.h               Header file for ecp_alt.c
  - TFM_Appli/Secure/Inc/gcm_alt.h               Header file for gcm_alt.c
  - TFM_Appli/Secure/Inc/low_level_flash.h       Header file for low_level_flash.c
  - TFM_Appli/Secure/Inc/low_level_rng.h         Header file for low_level_rng.c
  - TFM_Appli/Secure/Inc/md5_alt.h               Header file for md5_alt.c
  - TFM_Appli/Secure/Inc/mpu_armv8m_drv.h        Header file for mpu_armv8m_drv.c
  - TFM_Appli/Secure/Inc/platform_irq.h          Platform interrupt requests
  - TFM_Appli/Secure/Inc/rsa_alt.h               Header file for rsa_alt.c
  - TFM_Appli/Secure/Inc/sha1_alt.h              Header file for sha1_alt.c
  - TFM_Appli/Secure/Inc/sha256_alt.h            Header file for sha256_alt.c
  - TFM_Appli/Secure/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - TFM_Appli/Secure/Inc/target_cfg.h            Header file for target_cfg.c
  - TFM_Appli/Secure/Inc/target_flowcontrol.h    Header file for flow control in target_cfg.c
  - TFM_Appli/Secure/Inc/test_protections.h      Header file for tests in app_rot_test.c
  - TFM_Appli/Secure/Inc/tfm_mbedcrypto_config.h Mbedcrypto configuration file
  - TFM_Appli/Secure/Inc/tfm_peripherals_def.h   Peripheral definitions
  - TFM_Appli/Secure/Inc/tick.h                  Header file for tick.c
  - TFM_Appli/Secure/Inc/stsafea_conf.h          STSAFE configuration file
  - TFM_Appli/Secure/Inc/stsafea_interface_conf.h    Header file for STSAFE customization

  - TFM_Appli/Secure/Src/aes_alt.c               AES HW crypto interface
  - TFM_Appli/Secure/Src/app_rot_test.c          TFM AppRoT example service
  - TFM_Appli/Secure/Src/attest_hal.c            Initial attestation example
  - TFM_Appli/Secure/Src/ccm_alt.c               CCM HW crypto interface
  - TFM_Appli/Secure/Src/dummy_boot_seed.c       Boot seed implementation
  - TFM_Appli/Secure/Src/dummy_crypto_keys.c     HUK and EAT private key implementation
  - TFM_Appli/Secure/Src/dummy_device_id.c       Device IDs and versions implementation
  - TFM_Appli/Secure/Src/dummy_nv_counters.c     Non Volatile counters implementation
  - TFM_Appli/Secure/Src/ecdsa_alt.c             ECDSA HW crypto interface
  - TFM_Appli/Secure/Src/ecp_alt.c               ECP HW crypto interface
  - TFM_Appli/Secure/Src/ecp_curves_alt.c        ECP curves HW crypto
  - TFM_Appli/Secure/Src/gcm_alt.c               GCM HW crypto interface
  - TFM_Appli/Secure/Src/low_level_com.c         Uart low level interface
  - TFM_Appli/Secure/Src/low_level_device.c      Flash Low level device setting
  - TFM_Appli/Secure/Src/low_level_flash.c       Flash Low level interface
  - TFM_Appli/Secure/Src/low_level_rng.c         Random generator interface
  - TFM_Appli/Secure/Src/md5_alt.c               MD5 HW crypto interface
  - TFM_Appli/Secure/Src/mpu_armv8m_drv.c        MPU low level interface
  - TFM_Appli/Secure/Src/rsa_alt.c               RSA HW crypto interface
  - TFM_Appli/Secure/Src/sha1_alt.c              SHA1 HW crypto interface
  - TFM_Appli/Secure/Src/sha256_alt.c            SHA256 HW crypto interface
  - TFM_Appli/Secure/Src/spm_hal.c               Secure Partition Manager HAL interface
  - TFM_Appli/Secure/Src/startup_stm32u5xx.c     Startup file in c
  - TFM_Appli/Secure/Src/system_stm32u5xx.c      System Init file
  - TFM_Appli/Secure/Src/target_cfg.c            Target start up
  - TFM_Appli/Secure/Src/tfm_platform_system.c   Platform specific implementation
  - TFM_Appli/Secure/Src/tick.c                  HAL Tick implementation
  - TFM_Appli/Secure/Src/stsafe_service_interface.c    basic services for STSAFE (link management)
  - TFM_Appli/Secure/Src/stsafea_crypto_interface.c    crypto services for STSAFE (link securisation)

  - TFM_Appli/NonSecure/Inc/cmsis.h              Header file for CMSIS
  - TFM_Appli/NonSecure/Inc/com.h                Header file for com.c
  - TFM_Appli/NonSecure/Inc/common.h             Header file for common.c
  - TFM_Appli/NonSecure/Inc/fw_update_app.h      Header file for fw_update.c
  - TFM_Appli/NonSecure/Inc/low_level_flash.h    Header file for low_level_flash.c
  - TFM_Appli/NonSecure/Inc/main.h               Header file for main.c
  - TFM_Appli/NonSecure/Inc/mpu_armv8m_drv.h     Header file for mpu_armv8m_drv.c
  - TFM_Appli/NonSecure/Inc/ns_data.h            Header file for ns_data.c
  - TFM_Appli/NonSecure/Inc/stm32u5xx_hal_conf.h HAL configuration file
  - TFM_Appli/NonSecure/Inc/stm32u5xx_it.h       Header file for stm32u5xx_it.c
  - TFM_Appli/NonSecure/Inc/test_protections.h   Header file for test_protections.c
  - TFM_Appli/NonSecure/Inc/tfm_app.h            Header file for tfm_app.c
  - TFM_Appli/NonSecure/Inc/ymodem.h             Header file for ymodem.c

  - TFM_Appli/NonSecure/Src/com.c                Uart low level interface
  - TFM_Appli/NonSecure/Src/common.c             Uart common functionalities
  - TFM_Appli/NonSecure/Src/fw_update_app.c      Firmware update example
  - TFM_Appli/NonSecure/Src/low_level_device.c   Flash Low level device setting
  - TFM_Appli/NonSecure/Src/low_level_flash.c    Flash Low level interface
  - TFM_Appli/NonSecure/Src/main.c               Main application file
  - TFM_Appli/NonSecure/Src/mpu_armv8m_drv.c     MPU low level interface
  - TFM_Appli/NonSecure/Src/ns_data.c            NS Data image example
  - TFM_Appli/NonSecure/Src/startup_stm32u5xx.c  Startup file in c
  - TFM_Appli/NonSecure/Src/stm32u5xx_it.c       Interrupt handlers
  - TFM_Appli/NonSecure/Src/system_stm32u5xx.c   System init file
  - TFM_Appli/NonSecure/Src/test_protections.c   Test protections example
  - TFM_Appli/NonSecure/Src/tfm_app.c            TFM runtime services example
  - TFM_Appli/NonSecure/Src/tfm_ns_lock.c        NS secure lock for secure access
  - TFM_Appli/NonSecure/Src/ymodem.c             Ymodem support

### <b>How to use it ?</b>

In order to build the TFM_Appli Secure and Non Secure projects, you must do the
following:
 - Open your preferred toolchain
 - Rebuild the two projects

Then refer to TFM readme at upper level directory for TFM usage.
