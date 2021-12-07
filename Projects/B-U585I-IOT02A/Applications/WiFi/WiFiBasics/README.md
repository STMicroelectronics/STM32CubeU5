## <b>WiFiBasics application Description</b>


This application demonstrates WiFi connectivity on MXCHIP EMW3080 module for the B-U585-IOT02A board.

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

WiFi, Wireless, Ping, Scan, Connectivity, Network Library, WPA , WPA2, WiFi module, Station mode, Access Point, MXCHIP, EMW3080

### <b>Directory contents</b>

 - WiFi/WiFiBasics/Inc/console.h                        Console command interpreter header file
 - WiFi/WiFiBasics/Inc/main.h                           Header for main.c module
 - WiFi/WiFiBasics/Inc/mx_wifi_conf.h                   WiFi device configuration file
 - WiFi/WiFiBasics/Inc/net_conf.h                       Network Library configuration file
 - WiFi/WiFiBasics/Inc/net_interface.h                  Header for net_interface.c module
 - WiFi/WiFiBasics/Inc/stm32u5xx_hal_conf.h             HAL Configuration file
 - WiFi/WiFiBasics/Inc/stm32u5xx_it.h                   STM32 Interrupt handlers header file

 - WiFi/WiFiBasics/Src/console.c                        Console command interpreter
 - WiFi/WiFiBasics/Src/http_download.c                  HTTP download client implementation
 - WiFi/WiFiBasics/Src/main.c                           Main program
 - WiFi/WiFiBasics/Src/main_app.c                       Application entry point
 - WiFi/WiFiBasics/Src/net_interface.c                  Network interface management
 - WiFi/WiFiBasics/Src/ping.c                           Ping command implementation
 - WiFi/WiFiBasics/Src/stm32u5xx_hal_msp.c              HAL MSP module
 - WiFi/WiFiBasics/Src/stm32u5xx_hal_timebase_tim.c     HAL time base configuration with the hardware TIM
 - WiFi/WiFiBasics/Src/system_stm32u5xx.c               stm32u5xx system clock configuration file
 - WiFi/WiFiBasics/Src/stm32u5xx_it.c                   Interrupt handlers
 - WiFi/WiFiBasics/Src/test_echo_server.c               Echo Server command implementation


### <b>Hardware and Software environment</b>

 - The EMW3080B MXCHIP Wi-Fi module firmware used version is V2.1.11 and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

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

 - Edit the file `main.h` to enter the name of your WiFi access point (SSID) to connect to and its password.

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

