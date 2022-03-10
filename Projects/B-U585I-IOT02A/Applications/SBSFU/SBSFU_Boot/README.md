## <b>Application Licence Clarification</b>

The SBSFU_Boot application is licenced by ST under Ultimate Liberty license SLA0044.
Read the file SBSFU_Boot/q_a.txt for Application Licence Clarification.

## <b>SBSFU_Boot Application description</b>

This project is used to generate the SBSFU_Boot binary file.
The core function of this application relies on the mcuboot middleware, the trusted
firmware (TFM) middleware (only Boot Loader stage 2 part) and the mbed-crypto middleware.

It has to be compiled first.

In order to ease the development process, a prebuild command and postbuild command are
integrated in each toolchain project.
The prebuild command is in charge of preparing the scatter file according to common
flash layout description in linker folder.
The postbuild command is in charge of preparing the board programming scripts and
the images preparation scripts according to the flash layout and crypto scheme.

### <b>Keywords</b>

Security, SBSFU, TFM, mcuboot, mbedCrypto, AES, CRYP, FLASH, GTZC, HASH, PKA,
ICACHE, Secure boot, Firmware update, Authentication,
Integrity, Confidentiality, cipher, Signature, ECDSA verification, RSA,
Option bytes, TrustZone

### <b>Directory contents</b>

  - SBSFU_Boot/Inc/mcuboot_config/mcuboot_config.h Mcuboot configuration file
  - SBSFU_Boot/Inc/aes_alt.h                       Header file for aes_alt.c
  - SBSFU_Boot/Inc/boot_hal_cfg.h                  Platform configuration file for SBSFU_Boot
  - SBSFU_Boot/Inc/boot_hal_hash_ref.h             Header file for hash reference code in boot_hal.c
  - SBSFU_Boot/Inc/boot_hal_imagevalid.h           Header file for image validation code in boot_hal.c
  - SBSFU_Boot/Inc/boot_hal_flowcontrol.h          Header file for flow control code in boot_hal.c
  - SBSFU_Boot/Inc/cmsis.h                         Header file for CMSIS
  - SBSFU_Boot/Inc/config-boot.h                   Mbed-crypto configuration file
  - SBSFU_Boot/Inc/ecp_alt.h                       Header file for ecp_alt.c
  - SBSFU_Boot/Inc/low_level_flash.h               Header file for low_level_flash.c
  - SBSFU_Boot/Inc/mpu_armv8m_drv.h                Header file for mpu_armv8m_drv.c
  - SBSFU_Boot/Inc/rsa_alt.h                       Header file for rsa_alt.c
  - SBSFU_Boot/Inc/sha256_alt.h                    Header file for sha256_alt.c
  - SBSFU_Boot/Inc/stm32u5xx_hal_conf.h            HAL driver configuration file
  - SBSFU_Boot/Inc/target_cfg.h                    Header file for target start up
  - SBSFU_Boot/Inc/low_level_security.h            Header file for low_level_security.c
  - SBSFU_Boot/Inc/tfm_peripherals_def.h           Peripheral definitions

  - SBSFU_Boot/Src/aes_alt.c                       AES HW crypto interface
  - SBSFU_Boot/Src/bl2_nv_services.c               Non Volatile services for SBSFU_Boot
  - SBSFU_Boot/Src/boot_hal.c                      Platform initialization
  - SBSFU_Boot/Src/ecdsa_alt.c                     ECDSA HW crypto interface
  - SBSFU_Boot/Src/ecp_alt.c                       ECP HW crypto interface
  - SBSFU_Boot/Src/ecp_curves_alt.c                ECP curves HW crypto interface
  - SBSFU_Boot/Src/keys.c                          keys provisionned in PERSO area 
  - SBSFU_Boot/Src/keys_map.c                      keys indirection to access keys in PERSO area
  - SBSFU_Boot/Src/image_macros_to_preprocess_bl2.c Images definitions to preprocess for bl2
  - SBSFU_Boot/Src/low_level_com.c                 Uart low level interface
  - SBSFU_Boot/Src/low_level_device.c              Flash Low level device configuration
  - SBSFU_Boot/Src/low_level_flash.c               Flash Low level interface
  - SBSFU_Boot/Src/mpu_armv8m_drv.c                Mpu low level interface
  - SBSFU_Boot/Src/rsa_alt.c                       RSA HW crypto interface
  - SBSFU_Boot/Src/sha256_alt.c                    SHA256 HW crypto interface
  - SBSFU_Boot/Src/startup_stm32u5xx.c             Startup file in c
  - SBSFU_Boot/Src/system_stm32u5xx.c              System Init file
  - SBSFU_Boot/Src/low_level_security.c            Security Low level services
  - SBSFU_Boot/Src/tick.c                          HAL Tick implementation

  - SBSFU_Boot/Src/enc-ec256-priv.pem              ecdsa private key for decrypting FW keys
  - SBSFU_Boot/Src/enc-ec256-pub.pem               ecdsa public key for encrypting FW keys
  - SBSFU_Boot/Src/enc-rsa2048-priv.pem            rsa private key for decrypting FW keys
  - SBSFU_Boot/Src/enc-rsa2048-pub.pem             rsa public key for encrypting FW keys
  - SBSFU_Boot/Src/root-ec-p256.pem                ecdsa private key for signing FW image
  - SBSFU_Boot/Src/root-ec-p256_1.pem              ecdsa private key for signing FW image
  - SBSFU_Boot/Src/root-rsa-2048.pem               rsa private key for signing FW image
  - SBSFU_Boot/Src/root-rsa-2048_1.pem             rsa private key for signing FW image
  - SBSFU_Boot/Src/root-rsa-3072.pem               rsa private key for signing FW image
  - SBSFU_Boot/Src/root-rsa-3072_1.pem             rsa private key for signing FW image

  - Linker/flash_layout.h                          Flash mapping
  - Linker/region_defs.h                           RAM and FLASH regions definitions

### <b>How to use it ?</b>

In order to build the SBSFU_Boot project, you must do the following:
 - Open your preferred toolchain
 - Rebuild the project

Then refer to SBSFU readme at upper level directory for SBSFU usage.

