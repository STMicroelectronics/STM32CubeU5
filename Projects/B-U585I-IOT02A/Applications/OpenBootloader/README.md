## <b>OpenBootloader Application Description</b>

This application exploits OpenBootloader Middleware to demonstrate how to develop an IAP application
and how use it.

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 60 Mhz.

Then, these protocols and memories interfaces are configured:

  - USART3
  - FDCAN1
  - I2C2
  - SPI1
  - USB
  - FLASH interface
  - RAM interface
  - Option bytes interface
  - Engi bytes interface
  - OTP interface
  - System memory interface

Then, the OpenBootloader application will wait for incoming communication on one of the supported protocols.

Once a communication is established through one of the supported protocols,
the OpenBootloader application will wait for commands sent by the host.

#### <b>Notes</b>

 1. In case of Mass Erase operation, the OpenBootloader FLASH area must be protected otherwise the OpenBootloader
    will be erased.

 2. The current implementation of write in option bytes doesn’t support writing in OEMKEYR option bytes.
    The OEMKEYR option bytes registers programming can be easily added at the end of the function OPENBL_OB_Write in the file optionbytes_interface.c

 3. In the `OpenBootloader_Init()` function in `app_openbootloader.c` file, the user can:
       - Select the list of supported commands for a specific interface by defining its own list of commands.

       Here is an example of how to customize USART interface commands list, here only read/write commands are supported:

        OPENBL_CommandsTypeDef USART_Cmd =
        {
          NULL,
          NULL,
          NULL,
          OPENBL_USART_ReadMemory,
          OPENBL_USART_WriteMemory,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL
        };

        USART_Handle.p_Ops = &USART_Ops;
        USART_Handle.p_Cmd = &USART_Cmd;           /* Initialize the USART handle with the list of supported commands */
        OPENBL_USART_SetCommandsList(&USART_Cmd);  /* Register the list of supported commands in MW side */

       - Use the default list of supported commands for a specific interface by reusing the commands list defined in MW side.

       Here is an example of how to use USART interface default commands list:

        /* Register USART interfaces */
        USART_Handle.p_Ops = &USART_Ops;
        USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();  /* Initialize the USART handle with the default list osupported commands */

### <b>Keywords</b>

OpenBootloader, USART, FDCAN, I2C, SPI, USB

### <b>Directory contents</b>

     - AZURE_RTOS/App/app_azure_rtos.c                    Azure rtos application implementation file
     - AZURE_RTOS/App/app_azure_rtos.h                    Azure rtos application implementation header file
     - AZURE_RTOS/App/app_azure_rtos_conf.h               Azure rtos application configuration header file
     - Core/Src/main.c                                    Main program file
     - Core/Src/stm32u5xx_hal_msp.c                       MSP Initialization file
     - Core/Src/stm32u5xx_hal_timebase_tim.c              HAL time base based on the hardware TIM file
     - Core/Src/stm32u5xx_it.c                            Interrupt handlers file
     - Core/Src/system_stm32u5xx.c                        STM32U5xx system clock configuration file
     - Core/Src/tx_initialize_low_level.s                 ThreadX low-level processor initialization file
     - Core/Inc/main.h                                    Main program header file
     - Core/Inc/stm32u5xx_hal_conf.h                      HAL Library Configuration file
     - Core/Inc/stm32u5xx_it.h                            Interrupt handlers header file
     - Core/Inc/tx_user.h                                 Contains user defines for ThreadX configuration
     - OpenBootloader/App/app_openbootloader.c            OpenBootloader application entry point
     - OpenBootloader/App/app_openbootloader.h            Header for OpenBootloader application entry file
     - OpenBootloader/Target/common_interface.c           Contains common functions used by different interfaces
     - OpenBootloader/Target/common_interface.h           Header for common functions file
     - OpenBootloader/Target/engibytes_interface.c        Contains ENGI Bytes interface
     - OpenBootloader/Target/engibytes_interface.h        Header of ENGI Bytes interface file
     - OpenBootloader/Target/fdcan_interface.c            Contains FDCAN interface
     - OpenBootloader/Target/fdcan_interface.h            Header of FDCAN interface file
     - OpenBootloader/Target/flash_interface.c            Contains FLASH interface
     - OpenBootloader/Target/flash_interface.h            Header of FLASH interface file
     - OpenBootloader/Target/i2c_interface.c              Contains I2C interface
     - OpenBootloader/Target/i2c_interface.h              Header of I2C interface file
     - OpenBootloader/Target/spi_interface.c              Contains SPI interface
     - OpenBootloader/Target/spi_interface.h              Header of SPI interface file
     - OpenBootloader/Target/interfaces_conf.h            Contains Interfaces configuration
     - OpenBootloader/Target/iwdg_interface.c             Contains IWDG interface
     - OpenBootloader/Target/iwdg_interface.h             Header of IWDG interface file
     - OpenBootloader/Target/openbootloader_conf.h        Header file that contains OpenBootloader HW dependent configuration
     - OpenBootloader/Target/openbootloader_threadx.c     OpenBootloader application entry point file
     - OpenBootloader/Target/openbootloader_threadx.h     OpenBootloader application entry point header file
     - OpenBootloader/Target/optionbytes_interface.c      Contains OptionBytes interface
     - OpenBootloader/Target/optionbytes_interface.h      Header of OptionBytes interface file
     - OpenBootloader/Target/otp_interface.c              Contains OTP interface
     - OpenBootloader/Target/otp_interface.h              Header of OTP interface file
     - OpenBootloader/Target/platform.h	                  Header for patterns
     - OpenBootloader/Target/ram_interface.c              Contains RAM interface
     - OpenBootloader/Target/ram_interface.h              Header of RAM interface file
     - OpenBootloader/Target/systemmemory_interface.c     Contains system memory interface
     - OpenBootloader/Target/systemmemory_interface.h     Header of system memory interface file
     - OpenBootloader/Target/usart_interface.c            Contains USART interface
     - OpenBootloader/Target/usart_interface.h            Header of USART interface file
     - OpenBootloader/Target/usb_interface.c              Contains USB interface
     - OpenBootloader/Target/usb_interface.h              Header of USB interface file
     - USBX/App/app_threadx.c                             ThreadX applicative file
     - USBX/App/app_threadx.h                             ThreadX applicative header file
     - USBX/App/app_usbx_device.c                         USBX Device applicative file
     - USBX/App/app_usbx_device.h                         USBX Device applicative header file
     - USBX/App/ux_device_descriptors.c                   USBX Device descriptor file
     - USBX/App/ux_device_descriptors.h                   USBX Device descriptor header file
     - USBX/App/ux_device_dfu_media.c                     USBX Device interface applicative file
     - USBX/App/ux_device_dfu_media.h                     USBX Device interface applicative header file
     - USBX/App/ux_user.h                                 Contains user defines for USBX configuration
     - USBX/Target/ux_stm32_config.h                      USBX STM32 configuration header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B-U585I-IOT02A Rev C (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

  - B-U585I-IOT02A Rev C set-up to use FDCAN1:
    - Connect the PB8 (CN3: 7), PB9 (CN3: 10) respectively to RX, TX pins of an external FDCAN transceiver 
    - Connect CAN-H and CAN-L of the external FDCAN transceiver to your FDCAN Host adapter

  - B-U585I-IOT02A Rev C set-up to use USART:
    - To use the USART3 for communication you have to connect:
      - Tx pin of your host adapter to PA9 pin of the B-U585I-IOT02A Rev C board
      - Rx pin of your host adapter to PA10 pin of the B-U585I-IOT02A Rev C board
	- If STLINK-V3SET is used as host adapter, please use Tx,Rx pins from STLINK-V3SET CN8 connector

  - B-U585I-IOT02A set-up to use I2C:
    - Set I2C address to 0x5A
    - To use the I2C2 for communication you have to connect:
      - SCL (CN2: 7) pin of your host adapter to PH4 pin of the B-U585I-IOT02A board
      - SDA (CN2: 10) pin of your host adapter to PH5 pin of the B-U585I-IOT02A board

  - B-U585I-IOT02A set-up to use SPI
      - To use the SPI1 for communication you have to connect:
        - SCK pin of your host adapter to PE13 (CN13: 6) pin
        - MISO pin of your host adapter to PE14 (CN13: 5) pin
        - MOSI pin of your host adapter to PE15 (CN13: 4) pin

  - B-U585I-IOT02A set-up to use USB:
    - USB FS CN1

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the application
  - Run STM32CubeProgrammer and connect to OpenBootloader using USART3</br>
    Or
  - Connect your FDCAN host adapter and connect to OpenBootloader using FDCAN1</br>
    Or
  - Connect your I2C host adapter and connect to OpenBootloader using I2C2</br>
    Or
  - Connect your SPI host adapter and connect to OpenBootloader using SPI1</br>
    Or
  - Connect your USB host adapter and connect to OpenBootloader using USB FS</br>


  - B-U585I-IOT02A Rev C set-up to use I2C:
    - Set I2C address to 0x5A
    - To use the I2C2 for communication you have to connect:
      - SCL (CN2: 7) pin of your host adapter to PH4 pin of the B-U585I-IOT02A Rev C board
      - SDA (CN2: 10) pin of your host adapter to PH5 pin of the B-U585I-IOT02A Rev C board

  - B-U585I-IOT02A Rev C set-up to use SPI
      - To use the SPI1 for communication you have to connect:
        - SCK pin of your host adapter to PE13 (CN13: 6) pin
        - MISO pin of your host adapter to PE14 (CN13: 5) pin
        - MOSI pin of your host adapter to PE15 (CN13: 4) pin

  - B-U585I-IOT02A Rev C set-up to use USB:
    - USB FS CN1

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the application
  - Run STM32CubeProgrammer and connect to OpenBootloader using USART3</br>
    Or
  - Connect your FDCAN host adapter and connect to OpenBootloader using FDCAN1</br>
    Or
  - Connect your I2C host adapter and connect to OpenBootloader using I2C2</br>
    Or
  - Connect your SPI host adapter and connect to OpenBootloader using SPI1</br>
    Or
  - Connect your USB host adapter and connect to OpenBootloader using USB FS</br>
