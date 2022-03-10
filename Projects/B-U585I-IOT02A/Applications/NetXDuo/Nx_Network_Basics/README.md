## <b>Nx_Network_Basics application Description</b>


This application demonstrates WiFi connectivity on MXCHIP EMW3080 module for the STM32U585AII6Q board.

The application performs Scan function an allows to connect to one of the available hot-spot.
Ping functionality is supported as well as connection to a remote echo server, by default
the remote echo server is the one provided by ArmMbed. The application also demonstrates HTTP
download function. Acting as a client the application connects to an HTTP server and download a file.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

WiFi, Ping, Scan, HTTP, WPA2, WiFi module, Station mode, Access Point

### <b>Hardware and Software environment</b>

 - The EMW3080B MXCHIP Wi-Fi module firmware and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - Before using this project, you shall update your B-U585I-IOT02A RevC board with the EMW3080B firmware version 2.1.11.
   To achieve this, follow the instructions given at the above link, using the EMW3080updateV2.1.11RevC.bin flasher under the V2.1.11/SPI folder.

 - This application runs on STM32U585xx devices without security enabled (TZEN=0).

 - This application requires a WiFi access point to connect to

      * With a transparent Internet connectivity: No proxy, no firewall blocking the outgoing traffic.
      * Running a DHCP server delivering the IP and DNS configuration to the board.


 - This application has been tested with STMicroelectronics B-U585I-IOT02A (MB1551C)
   board and can be easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - WARNING: Before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.

 - Open your preferred toolchain

 - Edit the file `Core/Inc/mx_wifi_conf.h` to enter the name of your WiFi access point (SSID) to connect to and its password.

 - Build all files and load your image into target memory

 - Board must be connected to a PC with USB (ST-LINK USB port), open a serial terminal emulator, find the board's COM port and configure it with:
    - 8N1, 115200 bauds, no HW flow control
    - set the line endings to LF (Transmit) and LF (receive).

 - Run the application

 - At first the application performs a scan function and displays the list of access point, then example displays
   the help command list

 - Enters any of the proposed command:

   - ping <hostname> : ping a remote host and display time to perform ping request
   - echo : send 10 x 100 bytes to an echo server (armMBED@ one) and check that return data are identical to the one sent
   - scan : list available WiFi access points
   - http : download a file from an HTTP server, http <url> (default url is `http://public.st.free.fr/500MO.bin`)
   
