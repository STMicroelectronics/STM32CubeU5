
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>Nx_MDNS application Description</b>

This application demonstrates mDNS on MXCHIP EMW3080 module for the STM32U585AII6Q board.

The application performs mDNS services announce and deannounce functions using NetxDuo.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

host name: mDNS-HOST
service instances: test_announce, test announce1, test_announce2, test_announce3
service type: _test_st

### <b>Hardware and Software environment</b>

 - The EMW3080B MXCHIP Wi-Fi module firmware and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - Before using this project, you shall update your B-U585I-IOT02A RevC board with the EMW3080B firmware version 2.1.11.
   To achieve this, follow the instructions given at the above link, using the EMW3080updateV2.1.11RevC.bin flasher under the V2.1.11/SPI folder.

 - This application runs on STM32U585AII6Q devices without security enabled (TZEN=0).

 - This application has been tested with STMicroelectronics B-U585I-IOT02A (MB1551C)
   board and can be easily tailored to any other supported device and development board.

 - This example requires a WiFi access point to connect to
      * With a transparent Internet connectivity: No proxy, no firewall blocking the outgoing traffic.
      * Running a DHCP server delivering the IP and DNS configuration to the board.

 - Development PC with the Bonjour SDK installed and connected to the same WiFi access point as the board.
      * This is used to verify the services being announced/deannounced.


###  <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain

 - Edit the file `Core/Inc/mx_wifi_conf.h` to enter the name of your WiFi access point (SSID) to connect to and its password.

 - Build all files and load your image into target memory

 - Board must be connected to a PC with USB (ST-LINK USB port), open a serial terminal emulator, find the board's COM port and configure it with:
    - 8N1, 115200 bauds, no HW flow control
    - set the line endings to LF (receive).

 - Enter the following commands in a Command Prompt window (you may open multiple windows for each command) to verify
   the services being announced/deannounced:

   - `C:\Windows\System32> dns-sd.exe -L test_announce _test_st local`
   - `C:\Windows\System32> dns-sd.exe -L test_announce1 _test_st local`
   - `C:\Windows\System32> dns-sd.exe -L test_announce2 _test_st local`
   - `C:\Windows\System32> dns-sd.exe -L test_announce3 _test_st local`
   - `C:\Windows\System32> dns-sd.exe -B _test_st local`

 - Run the application

 - One can also ping the device using the hostname to verify the automatic IPv4 address resolved by mDNS:

   - `C:\Windows\System32> ping.exe mDNS-HOST.local`

 - Application announces and deannounces various mDNS services and displays the same on the console.

:::
:::

