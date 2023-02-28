---
pagetitle: Readme
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

# <b>Application Licence Clarification</b>

The TFM application components are licensed by ST under Ultimate Liberty license SLA0044. Read the file q_a.txt for Application
Licence Clarification. Except as expressly mentioned, other rights are granted with the STSAFE components, where license SLA0088 is applied.

## <b>TFM Application Description</b>

The TFM provides a Root of Trust solution including Secure Boot and Secure Firmware Update functionalities that is used before executing
the application and provides TFM secure services that are isolated from the non-secure application but can be used by the non-secure
application at run-time.

TFM application is based on the Open Source TF-M reference implementation that has been ported on STM32 microcontroller to take benefit
of STM32 HW security features such as CM33 Trust Zone/MPU, Trust Zone aware peripherals, Memory protections (HDP, WRP) and enhanced
life cycle scheme.
Security features can be enforced with the usage of a ST secure element, called STSAFE device (precisely STSAFE-A series) which
offers strong authentication, secure TLS sessions, verification of signatures and secure storage.

The Secure Boot and Secure Firmware Update solution allows the update of the STM32 microcontroller built-in program with new firmware
versions, adding new features and correcting potential issues. The update process is performed in a secure way to prevent unauthorized
updates and access to confidential on-device data:

-   The Secure Boot (Root of Trust services) is an immutable code, always executed after a system reset, that checks STM32 static protections,
activates STM32 runtime protections and then verifies the authenticity (RSA or ECDSA signature) and integrity (SHA256) of the application code
before execution in order to ensure that invalid or malicious code cannot be run. The default asymmetric key (RSA or ECDSA) is taken from
TFM_SBSFU_Boot\\Src\\keys.c and is embedded in the provisioned data area in the secure boot and secure firmware update binary.

-   The Secure Firmware Update application is an immutable code that detects that a new firmware image is available, and that checks its
authenticity and integrity. It also decrypts it (AES-CTR) during installation, if image is installed in internal flash. The AES-CTR key is
encrypted (RSA-OAEP or ECIES-P256) and provided in the firmware image itself. The default asymmetric key (RSA or ECDSA) used to encrypt the
AES-CTR key is distinct from the signature key, but also taken from TFM_SBSFU_Boot\\Src\\keys.c. The firmware update can be done either on the
secure part of firmware image and/or on the non-secure part of the firmware image.

The secure services are an updatable code implementing a set of services managing critical assets that are isolated from the non-secure
application so that the non-secure application can't access directly to any of the critical assets but can only use secure services that
use the critical assets:

-   Crypto: secure cryptographic services based on opaque key APIs

-   Secure Storage: protect data Confidentiality/authenticity/Integrity

-   Internal Trusted Storage: protect data in internal flash

-   Attestation: prove product identity via Entity Attestation Token

	When STSAFE is enabled, EAT contains the STSAFE-A unique serial number as product identity and message is signed with the STSAFE
	pre-provisioned private key.

For more details, refer to UM2851 "Getting Started with the TF-M applications" available from the STMicroelectronics microcontroller website www.st.com.

## <b>Directory contents</b>

   - Linker:
        Linker files definition shared between TFM_SBSFU_Boot, TFM_Appli and TFM_Loader projects.

   - TFM_SBSFU_Boot:
        Secure boot and secure firmware update application.

   - TFM_Appli:
        Secure services application and example of non-secure user application.

   - TFM_Loader:
        Local loader application.

## <b>Hardware and Software environment</b>

   - This example runs on STM32U585xx devices.
   - It is possible to tailor it for STM32U5A5xx or STM32U545xx devices, by following these steps for each project of TFM application:

  For IAR IDE:

    1- Go to "Options" and select "General Options" then in the "Target" field Select "device" to choose your device.

    2- Go to "Options" and select "C/C++ Compiler" then "Preprocessor" tab and change the compilation symbol (STM32U5A5xx or STM32U545xx instead of STM32U585xx).

    3- Go to "Options" and select "Build Actions" then in the "Prebuild command line" field change the compilation symbol (STM32U5A5xx or STM32U545xx instead of STM32U585xx).

  For STM32CubeIDE:

    1- Go to "Properties" and select "C/C++ Build" tab then "Settings" and in the "Build Steps" tab change the command in "Pre-build steps" and change the compilation symbol (STM32U5A5xx or STM32U545xx instead of STM32U585xx).

    2- Go to "Properties" and select "C/C++ General" tab then "Paths an Symbols" and in the "Symbols" tab select symbols for "GNU C" and change the compilation flag (for example STM32U5A5xx or STM32U545xx instead of STM32U585xx)

  For Keil IDE:

    1- Go to "Options" and select "Device" tab to choose your device.

    2- Go to "Options" and select "C/C++ (AC6)" tab then in the "Define" field in "Preprocessor Symbols" and change the compilation symbol (STM32U5A5xx or STM32U545xx instead of STM32U585xx).

    3- Go to "Options" and select "Linker" tab then "Misc controls" and add this line "--predefine=-DSTM32U5A5xx" or "--predefine=-DSTM32U545xx" to define your target.

  - This example has been tested with STMicroelectronics B-U585I-IOT02A board and can be easily tailored to any other supported device and
development board.

  - This example needs a terminal emulator supporting YModem protocol. (Tera Term for example, open source free software terminal emulator
that can be downloaded from https://osdn.net/projects/ttssh2/).

  - This example needs at least STM32CubeProgrammer v2.8.0 version, available on www.st.com.

## <b>Known limitations</b>

  - IAR build not working if cube package is installed on a path with a space, or a long path.
  - STM32CubeIDE: Only Release configuration is supported due to flash mapping constraint.
    To allow debugging in Release configuration, the debug level is -g3.


## <b>How to use it ?</b>

Several steps to run TFM application :

<b>0. Prerequisite</b>

   Two versions of imgtool (used during build process) are available in mcuboot middleware: windows executable and python version. By default,
the windows executable is selected. It is possible to switch to python version by:

   - installing python (Python 3.6 or newer) with the required modules from Middlewares\\Third_Party\\mcuboot\\scripts\\requirements.txt:

            pip install -r requirements.txt

   - having python in execution path variable

   - deleting imgtool.exe in Middlewares\\Third_Party\\mcuboot\\scripts\\dist\\imgtool

<b>1. Build</b>

   Build the TFM projects in the following order. This is mandatory as each projects requests some objects generated by the compilation
of the previous one:

   1. Build TFM_SBSFU_Boot project.
      This step creates the secure boot and secure firmware update binary including provisionned user data (keys, IDs...). Depending on 
toolchain, it is located here:

            EWARM:        TFM_SBSFU_Boot\EWARM\B-U585I-IOT02A\Exe\Project.bin
            MDK-ARM:      TFM_SBSFU_Boot\MDK-ARM\B-U585I-IOT02A\Exe\Project.bin
            STM32CubeIDE: TFM_SBSFU_Boot\STM32CubeIDE\Release\TFM_SBSFU_Boot.bin

   2. Build TFM_Appli Secure project.
      This step creates the TFM Application Secure binary:

            EWARM:        TFM_Appli\EWARM\Secure\B-U585I-IOT02A_S\Exe\Project.bin
            MDK-ARM:      TFM_Appli\MDK-ARM\B-U585I-IOT02A_S\Exe\Project.bin
            STM32CubeIDE: TFM_Appli\STM32CubeIDE\Secure\Release\TFM_Appli_Secure.bin

      It also produces the clear TFM Secure app signed image for initial installation in:

           TFM_Appli\Binary\tfm_s_app_init.bin

      It also produces the encrypted TFM Secure app signed image for download in:

           TFM_Appli\Binary\tfm_s_app_enc_sign.bin

      It also produces the clear TFM Secure app signed image for download in:

           TFM_Appli\Binary\tfm_s_app_sign.bin

   3. Build TFM_Appli Non Secure project.

      This step creates the TFM Application Non Secure binary:

            EWARM:        TFM_Appli\EWARM\NonSecure\B-U585I-IOT02A_NS\Exe\Project.bin
            MDK-ARM:      TFM_Appli\MDK-ARM\NonSecure\B-U585I-IOT02A_NS\Exe\Project.bin
            STM32CubeIDE: TFM_Appli\STM32CubeIDE\NonSecure\Release\TFM_Appli_NonSecure.bin

      It also produces the clear TFM Non Secure app signed image for initial installation in:

            TFM_Appli\Binary\tfm_ns_app_init.bin

      It also produces the encrypted TFM Non Secure app signed image for download in:

            TFM_Appli\Binary\tfm_ns_app_enc_sign.bin

      It also produces the clear TFM Non Secure app signed image for download in:

            TFM_Appli\Binary\tfm_ns_app_sign.bin

   4. Build TFM_Loader Secure project.
      This step creates the TFM loader Secure binary:
      
            EWARM:        TFM_Loader\EWARM\Secure\B-U585I-IOT02A_S\Exe\Project.bin
            MDK-ARM:      TFM_Loader\MDK-ARM\Secure\B-U585I-IOT02A_S\Exe\Project.bin
            STM32CubeIDE: TFM_Loader\STM32CubeIDE\Secure\Release\TFM_Loader_Secure.bin

   5. Build TFM_Loader Non Secure project.
      This step creates the TFM loader Non Secure binary:

            EWARM:        TFM_Loader\EWARM\NonSecure\B-U585I-IOT02A_NS\Exe\Project.bin
            MDK-ARM:      TFM_Loader\MDK-ARM\NonSecure\B-U585I-IOT02A_NS\Exe\Project.bin
            STM32CubeIDE: TFM_Loader\STM32CubeIDE\NonSecure\Release\TFM_Loader_NonSecure.bin

      It also produced the assembled (S and NS) TFM loader image in:

	        TFM_Loader\Binary\loader.bin

   6. Generate Secure and Non Secure Data images, by executing dataimg script:

            EWARM:        TFM_SBSFU_Boot\EWARM\dataimg.bat
            MDK-ARM:      TFM_SBSFU_Boot\MDK-ARM\dataimg.bat
            STM32CubeIDE: TFM_SBSFU_Boot\STM32CubeIDE\dataimg.sh

      It produces the clear TFM Secure and Non Secure data signed images for initial installation in:

            TFM_Appli\Binary\tfm_s_data_init.bin and TFM_Appli\Binary\tfm_ns_data_init.bin

	  It produces the encrypted TFM Secure and Non Secure data signed images for download in:

			TFM_Appli\Binary\tfm_s_data_enc_sign.bin and TFM_Appli\Binary\tfm_ns_data_enc_sign.bin

	  It produces the clear TFM Secure and Non Secure data signed images for download in:

	        TFM_Appli\Binary\tfm_s_data_sign.bin and TFM_Appli\Binary\tfm_ns_data_sign.bin

      The TFM Secure data signed image contains initial attestation private key for TFM service. The TFM Non Secure data signed
	  image contains dummy data for this example.

<b>2. Initialize the device</b>

   Ensure the switch SW1 (BOOT0 pin) on B-U585I-IOT02A board is on position 0 to boot from Flash.

   Depending on your toolchain, execute regression script (relying on STM32CubeProgrammer CLI tool) to easily perform device initialization
in one shot:
```
    EWARM:        TFM_SBSFU_Boot\EWARM\regression.bat
    MDK-ARM:      TFM_SBSFU_Boot\MDK-ARM\regression.bat
    STM32CubeIDE: TFM_SBSFU_Boot\STM32CubeIDE\regression.sh
```

   As an alternative, it is also possible to initialize and verify manually the Option Bytes by means of STM32CubeProgrammer GUI tool:

   - Please ensure with STM32CubeProgammer the following Option bytes configuration for the device:
     - RDP         = 0xAA (RDP Level 0)
     - DBANK       = 1 (Dual bank mode enabled)
     - SWAP_BANK   = 0 (Bank1 and Bank2 are not swapped)
     - nSWBOOT0    = 1 (BOOT0 taken from BOOT0 pin)
     - TZEN        = 1 (TrustZone security enabled)
     - NSBOOTADD0  = 0x100000 (User flash address 0x08000000)
     - NSBOOTADD1  = 0x17F200 (System bootloader address 0x0BF90000)
     - SECBOOTADD0 = 0x1800c0 (boot entry point address 0xc006000)
     - SRAM2_RST   = 0 (SRAM2 erased at each reset)
   - Ensure also that these protections are all disabled in the Option Bytes
     configuration: BOOT_LOCK, HDP, WRP, SECWM.
   - Ensure also that device is mass erased.
   - Ensure also that OEM2LOCK bit in the FLASH_NSSR register is not set to be
   sure that any OEM2KEY was provisioned previously.
   If it is locked with another password, fill the regression script with this password.

Additionally, the anti-tamper protection being enabled with active tamper pins usage by default, it is needed to connect TAMP_IN8 (PE4 on CN3
pin 11) and TAMP_OUT8 (PE5 on CN3 pin 14) on the B-U585I-IOT02A board, to allow the application to run. In case the tamper pins are opened or
shorted, then the application is reset and blocked. It is however possible to disable the anti-tamper protection (refer to section 9)

<b>3. Use a terminal emulator</b>

   Serial port configuration should be :

   - Baud rate    = 115200
   - Data         = 8 bits
   - Parity       = none
   - Stop         = 1 bit
   - Flow control = none

Terminal emulator is used for UART connection with the board.
Connect terminal emulator to COM port of the board.
The terminal emulator is used to log TFM information, and enter commands for User application.

<b>4. Program the TFM into flash</b>

   Program the TFM_SBSFU_Boot binary, the clear signed TFM_Appli Secure image for initial installation, the clear signed TFM_Appli Non Secure image for initial installation, and TFM_Loader image into internal flash.

   Program as well the TFM Secure and Non Secure data signed images for initial installation into internal flash.

   Depending on your toolchain, execute script TFM_UPDATE (relying on STM32CubeProgrammer CLI tool) to easily program the TFM into device internal flash in one shot:

    EWARM:        TFM_SBSFU_Boot\EWARM\TFM_UPDATE.bat
    MDK-ARM:      TFM_SBSFU_Boot\MDK-ARM\TFM_UPDATE.bat
    STM32CubeIDE: TFM_SBSFU_Boot\STM32CubeIDE\TFM_UPDATE.sh


<b>5. Reset the board</b>

   Press the board reset button, and check the log information on the terminal emulator:

   - TFM_SBSFU_Boot application starts
   - Then it configures the static protections (because 'development mode' is used by default)
   - Then it get locked (because intrusion is detected)

<b>6. Unlock the board</b>

   To exit from locked state (intrusion), remove the jumper JP3 (IDD) on the board, then put it back in place. Check the log information:

   - TFM_SBSFU_Boot starts
   - Then it checks the static protections, and sets the dynamic protections
   - Then it initializes the BL2 NV area (BootLoader2 Non Volatile area)
   - Then it verifies the TFM application image signatures.
   - Then it jumps to the TFM application, displaying the User Application Main menu.

<b>7. Use User App</b>

   This user application gives access to a Main menu:
```
   1 - Test protections
   2 - Test TFM Secure services
   3 - New Fw Image
   4 - Non-Secure Data
```

   1. Press 1 to enter test protections menu

      - Test Protection (#1): test unauthorized accesses to Secure side from Non Secure side, and to Secure Privileged side from Secure Non
Privileged side. Several successive unauthorized accesses to different areas are performed, leading to successive reset when fault occurs.

      Return to previous menu by pressing x.

   2. Press 2 to enter test TFM menu

      This menu allows to test TFM Secure services from Non Secure side through PSA API usage.

      - All tests (#0):            Execute all below tests in a row
      - AES-GCM (#1):              Test AES-GCM crypto service
      - AES-CBC (#2):              Test AES-CBC crypto service
      - AES-CCM (#3):              Test AES-CCM crypto service
      - SST set UID (#4):          Test UID creation in Secure Storage area
      - SST read / check UID (#5): Test UID read / check in Secure Storage area
      - SST remove UID (#6):       Test UID removal in Secure Storage area (requires UID set before)
      - EAT (#7):                  Test Initial Attestation
      - ITS set UID (#8):          Test UID creation in Internal Trusted Storage area
      - ITS read / check UID (#9): Test UID read / check in Internal Trusted Storage area
      - ITS remove UID (#a):       Test UID removal in Internal Trusted Storage area (requires UID set before)
      - SHA224 (#b):               Test SHA224 crypto service
      - SHA256 (#c):               Test SHA256 crypto service
      - Persistent key import (#d):  Test Persistent key import (retained in Internal Trusted Storage)
      - Persistent key export (#e):  Test Persistent key export
      - Persistent key destroy (#f): Test Persistent key destroy
      - STSAFE (#s):               Test STSAFE services (not enabled by default)

      Return to previous menu by pressing x.

   3. Press 3 to enter new firmware image menu

      This menu allows to manage new firmware images.

      It is possible to:
      - Download Secure App Image (#2): Download encrypted signed TFM_Appli Secure image (TFM_Appli\\Binary\\tfm_s_app_enc_sign.bin) or clear signed TFM_Appli Secure image (TFM_Appli\\Binary\\tfm_s_app_sign.bin)
      - Download NonSecure App Image (#3): Download encrypted signed TFM_Appli Non Secure image (TFM_Appli\\Binary\\tfm_ns_app_enc_sign.bin) or clear signed TFM_Appli Non Secure image (TFM_Appli\\Binary\\tfm_ns_app_sign.bin)
      - Download Secure Data Image (#4): Download encrypted signed TFM Secure data image(TFM_Appli\\Binary\\tfm_s_data_enc_sign.bin) or clear signed TFM Secure data image (TFM_Appli\\Binary\\tfm_s_data_sign.bin)
      - Download Non Secure Data Image (#5): Download encrypted signed TFM Non Secure data image (TFM_Appli\\Binary\\tfm_ns_data_enc_sign.bin) or clear signed TFM Non Secure data image (TFM_Appli\\Binary\\tfm_ns_data_sign.bin)

      Send the signed binary with Tera Term by using menu:
      "File > Transfer > YMODEM > Send..."

      After the download, press (#1) or the reset button to reset the board. After reset, the downloaded firmware image is detected, verified, decrypted (if needed) and installed, then executed, by TFM_SBSFU_Boot.

   4. Press 4 to enter Non-Secure Data menu

      - Display Data from NS Data Image (#1)

	  Return to previous menu by pressing x.

<b>8. Use Local loader App</b>

   Local loader application is an immutable code, in non secure area.

   To enter local loader application, press user button during board reset.

   This application gives access to a menu which allows to download new firmware image.

   Refer to step "7. Use User App" for more information on the download image menu.

<b>9. Configuration</b>

   It is possible to change TFM configuration. When changing configuration, all the projects must be cleaned then fully rebuild.

   *  **Crypto-scheme**

      By default, the crypto scheme is RSA-2048 signature, and AES-CTR-128 image encryption with key RSA-OAEP encrypted. It is possible to select another crypto-scheme, with CRYPTO_SCHEME define in TFM_SBSFU_Boot\\Inc\mcuboot_config\\mcuboot_config.h.

      ```
      #define CRYPTO_SCHEME_RSA2048    0x0 /* RSA-2048 signature, AES-CTR-128 encryption with key RSA-OAEP encrypted */
      #define CRYPTO_SCHEME_RSA3072    0x1 /* RSA-3072 signature, AES-CTR-128 encryption with key RSA-OAEP encrypted */
      #define CRYPTO_SCHEME_EC256      0x2 /* ECDSA-256 signature, AES-CTR-128 encryption with key ECIES-P256 encrypted */
      #define CRYPTO_SCHEME            CRYPTO_SCHEME_RSA2048  /* Select one of available crypto schemes */
      ```

   *  **HW accelerated cryptography**

      By default, the cryptography is accelerated with the HW crypto peripherals of the device (PKA, AES, HASH). It is possible to disable it in mcuboot by commenting BL2_HW_ACCEL_ENABLE define in TFM_SBSFU_Boot\\Inc\\config-boot.h.

      Similarly, it is possible to disable it in TFM crypto service by commenting TFM_HW_ACCEL_ENABLE define in TFM_Appli\\Secure\\Inc\\tfm_mbedcrypto_config.h.

   *  **Encryption**

      By default, the image encryption support is enabled. It is possible to disable the image encryption support with MCUBOOT_ENC_IMAGES define in TFM_SBSFU_Boot\\Inc\\mcuboot_config\\mcuboot_config.h.

      ```
      #define MCUBOOT_ENC_IMAGES           /* Defined: Image encryption enabled. */
      ```

   *  **Number of images**

      By default, 2 firmware app images are managed with 2 distincts signatures: 1 image for NS app binary and 1 image for S app binary. It is possible to assemble NS and S app binaries into 1 image with 1 single signature, with MCUBOOT_APP_IMAGE_NUMBER define in Linker\\flash_layout.h.

      ```
      #define MCUBOOT_APP_IMAGE_NUMBER 1 /* 1: S and NS application binaries are assembled in one single image.
                                            2: Two separated images for S and NS application binaries. */
      ```

      By default, 1 firmware secure data image and 1 firmware non secure data image are managed with same signatures as respectively secure app image and non secure app image.
      It is possible to remove the data images, with MCUBOOT_S_DATA_IMAGE_NUMBER and MCUBOOT_NS_DATA_IMAGE_NUMBER defines in Linker\\flash_layout.h.

      ```
      #define MCUBOOT_S_DATA_IMAGE_NUMBER 1   /* 1: S data image for S application.
                                                 0: No S data image. */

      #define MCUBOOT_NS_DATA_IMAGE_NUMBER 1  /* 1: NS data image for NS application.
                                                 0: No NS data image. */
      ```

   *  **Number of slots per image**

      By default, primary and secondary slot configuration is used (for each image). In this mode, the local loader downloads encrypted image in secondary slot, and the TFM_SBSFU_Boot decrypts the image when installing it from secondary slot to primary slot.
      It is possible to configure the TFM_SBSFU_Boot to use primary slot only. In this mode, the local loader downloads encrypted image directly in primary slot, and the TFM_SBSFU_Boot decrypts in place the image during installation process.

      This is configured with MCUBOOT_PRIMARY_ONLY define in Linker\\flash_layout.h.
      ```
      #define MCUBOOT_PRIMARY_ONLY       /* Defined: No secondary (download) slot(s), only primary slot(s) for each image.
                                            Undefined: Primary and secondary slot(s) for each image. */
      ```

      Note: When primary and secondary slot configuration is selected, a menu item (#3) 'Download a new Fw Image' is present in user
application. Its usage is exactly the same as the local loader application. When primary only slot configuration is selected, this menu
item is not present in user application, it is not possible to download new Fw image from user application.

   *  **Image upgrade strategy**

      The bootloader supports either swap- or overwrite-based image upgrades, for primary and secondary slots configuration. In overwrite-based
image upgrade mode, the image in secondary slot is overwriting image in primary slot. In swap-based image upgrade mode, images in primary and
secondary slots are swapped. After swap, new image in primary slot has to be confirmed by user application, else at next boot the images will
be swapped back.

      It is possible to select image upgrade strategy with MCUBOOT_OVERWRITE_ONLY define in Linker\\flash_layout.h.

      ```
      #define MCUBOOT_OVERWRITE_ONLY     /* Defined: the FW installation uses overwrite method.
                                            UnDefined: The FW installation uses swap mode. */
      ```

      Note: When overwrite method is selected, the User App menu 'New Fw Image' only allows to download image. When swap method is selected,
this User App menu additionally allows to validate installed image, and to request re-installation of image that have been swapped back.

      Note: When image is swapped back into download area, image is re-encrypted (if initially encrypted).

   *  **Local loader**

      By default, local loader application is included. It is possible to remove it with MCUBOOT_EXT_LOADER define in Linker\\flash_layout.h.

      ```
      #define MCUBOOT_EXT_LOADER         /* Defined: Add external local loader application. To enter it, press user button at reset.
                                            Undefined: No external local loader application. */
      ```

      In this case, the build of TFM_Loader must be skipped.

   *  **Application RoT**

      By default App RoT is enabled in TFM secure application. It is possible to disable it with TFM_PARTITION_APP_ROT define in Linker\\flash_layout.h.

      ```
      #define TFM_PARTITION_APP_ROT       /* comment to remove APP_ROT partition */
      ```

   *  **Anti-tamper**

      By default the anti-tamper is enabled for both internal and external tamper events. It is possible to change this configuration with
TFM_TAMPER_ENABLE define in TFM_SBSFU_Boot\\Inc\\boot_hal_cfg.h.

      ```
      #define NO_TAMPER            (0)                /*!< No tamper activated */
      #define INTERNAL_TAMPER_ONLY (1)                /*!< Only Internal tamper activated */
      #define ALL_TAMPER           (2)                /*!< Internal and External tamper activated */
      #define TFM_TAMPER_ENABLE ALL_TAMPER            /*!< TAMPER configuration flag  */
      ```

   *  **STSAFE**

      By default, TFM application works without an external secure element. It is possible to rely on it by adding PSA_USE_SE_ST compile switch in TFM_Appli Secure project (to which STSAFE is connected to) and in TFM_Appli Non Secure project (to which STSAFE is addressed through PSA APIs).

<b>10. Keys management</b>

   It is important to avoid using default keys provided in the example. A random keys generator is provided for each toolchain:
```
    EWARM:        TFM_SBSFU_Boot\EWARM\keygen.bat
    MDK-ARM:      TFM_SBSFU_Boot\MDK-ARM\keygen.bat
    STM32CubeIDE: TFM_SBSFU_Boot\STM32CubeIDE\keygen.sh

```

<b>11. TrustZone deactivation</b>

   After having executed this application, the TZEN option bit is set. TZEN option bit must be reset to allow execution of any other non
TrustZone application. To perform that, refer to AN5347.
:::
:::
