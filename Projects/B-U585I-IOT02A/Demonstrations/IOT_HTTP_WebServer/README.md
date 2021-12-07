## <b>IOT_HTTP_WebServer Demonstration Description</b>

-   The STM32Cube demonstration platform comes on top of the STM32Cube as a firmware package. It is based on the STM32Cube
HAL, BSP and middleware components. It shows how to perform a web server demonstration using MXCHIP wifi module.
For this demonstration, the web server is the B-U585I-IOT02A and the web client is the user browser.

-   At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the
Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 120 MHz.

-   At first step, the demonstration configures all required resources (System/Clocks/LEDs/Sensors/Wifi module ...).
Then, a scan for available wifi access point is done and the exhaustive list is printed in the hyper terminal.
After that the demonstration asks the user to enter his credentials (SSID : Login and PWD : PassWorD) for wifi access
point.

-   When credentials are valid, the demonstration starts the network interface process and prints the different steps on the
hyper terminal as follow
    -   Network Interface starting
    -   Network Interface ready
        - Device Name : MXCHIP-WIFI
        - Device ID   : EMW3080B
        - Device Version : Vx.y.z
        - MAC address: x.y.z.a.b.c
    -   Network Interface connecting
    -   Network Interface connected
        - IP address :  x.y.z.w

-   Then, the demonstration will create a socket communication in order to start server hosting and prints the different
steps on the hyper terminal as follow :
    -   *** Create TCP socket
    -   *** TCP socket created
    -   *** Set port and bind socket
    -   *** Port and socket binded
    -   *** Listen for incoming connections
    -   *** Listening started

-   After that the user can start web page surfing by copying the server IP address (provided after network interface
process) and past it in his browser. The web page will be loaded in the client server.

-   The web page contains 4 principle menus :
    -   Home menu : contains the product and the web page description
    -   Temperature menu : contains the temperature acquisition chart
    -   Pressure menu : contains the pressure acquisition chart
    -   Humidity menu : contains the humidity acquisition chart

-   Finally, user can choose any menu and starts / stops the different sensors acquisition.

-   B-U585I-IOT02A board's LEDs can be used to monitor the demonstration status:
    -   LED1 toggle each 100ms while process is ongoing and no error detected.
    -   LED3 is ON when any error occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds) based on
    variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from a peripheral ISR process,
    then the SysTick interrupt must have higher priority (numerically lower) than the peripheral interrupt. Otherwise
    the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The demonstration needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL
    operation.

### <b>Keywords</b>

WiFi, Wireless, Scan, Connectivity, Network Library, Web Server, WPA , WPA2, WiFi module, Access Point, MXCHIP, EMW3080

### <b>Directory contents</b>

-   IOT_HTTP_WebServer/Core/Src/main.c                              Main program source file
-   IOT_HTTP_WebServer/Core/Src/bsp.c                               BSP services source file
-   IOT_HTTP_WebServer/Core/Src/console.c                           Console services source file
-   IOT_HTTP_WebServer/Core/Src/stm32u5xx_it.c                      Interrupt handlers source file
-   IOT_HTTP_WebServer/Core/Src/stm32u5xx_hal_msp.c                 HAL MSP module source file
-   IOT_HTTP_WebServer/Core/Src/system_config.c                     System configuration source file
-   IOT_HTTP_WebServer/Core/Src/system_stm32u5xx.c                  System CMSIS Cortex-M33 Device source file
-   IOT_HTTP_WebServer/WebServer/App/webserver_main.c               Demo main program source file
-   IOT_HTTP_WebServer/WebServer/App/webserver_status.c             Status services source file
-   IOT_HTTP_WebServer/WebServer/App/http/webserver_http_encoder.c  HTTP encoding response headers source file
-   IOT_HTTP_WebServer/WebServer/App/http/webserver_http_response.c HTTP encoding response source file
-   IOT_HTTP_WebServer/WebServer/App/sensors/sensors.c              Sensors services source file
-   IOT_HTTP_WebServer/WebServer/App/wifi/webserver_wifi.c          Wifi services source file
-   IOT_HTTP_WebServer/WebServer/Target/net_conf_mxchip_spi.c       MXCHIP configuration source file
-   IOT_HTTP_WebServer/WebServer/Target/net_interface.c             Network interface source file
-   IOT_HTTP_WebServer/Core/Inc/main.h                              Main program header file
-   IOT_HTTP_WebServer/Core/Inc/bsp.h                               BSP services header file
-   IOT_HTTP_WebServer/Core/Inc/console.h                           Console services header file
-   IOT_HTTP_WebServer/Core/Inc/stm32u5xx_it.h                      Interrupt handlers header file
-   IOT_HTTP_WebServer/Core/Inc/system_config.h                     System configuration header file
-   IOT_HTTP_WebServer/Core/Inc/stm32u5xx_hal_conf.h                HAL configuration header file
-   IOT_HTTP_WebServer/WebServer/App/webserver_main.h               Demo main program header file
-   IOT_HTTP_WebServer/WebServer/App/webserver_status.h             Status services header file
-   IOT_HTTP_WebServer/WebServer/App/webserver.h                    Demo entry point header file
-   IOT_HTTP_WebServer/WebServer/App/http/webserver_http_encoder.h  HTTP encoding response headers header file
-   IOT_HTTP_WebServer/WebServer/App/http/webserver_http_response.h HTTP encoding response header file
-   IOT_HTTP_WebServer/WebServer/App/http/webserver_http_cmd.h      HTTP commands header file
-   IOT_HTTP_WebServer/WebServer/App/sensors/webserver_sensors.h    Sensors services header file
-   IOT_HTTP_WebServer/WebServer/App/wifi/webserver_wifi.h          Wifi services header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/css_main_res.h      Web page css main header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/css_shunk_res.h     Web page css shunk header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/favicon_res.h       Web page favicon header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/font_res.h          Web page fonts header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/html_res.h          Web page html main header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/image_res.h         Web page images main header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/js_main_res.h       Web page js main header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/js_shunk_res.h      Web page js main header file
-   IOT_HTTP_WebServer/WebServer/App/web_addons/json_res.h          Web page json main header file
-   IOT_HTTP_WebServer/WebServer/Target/net_conf.c                  Network configuration header file
-   IOT_HTTP_WebServer/WebServer/Target/net_interface.h             Network interface header file
-   IOT_HTTP_WebServer/WebServer/Target/net_interface.h             MXCHIP configuration header file
-   IOT_HTTP_WebServer/WebServer/Target/b_u585i_iot02a_conf.h       BSP configuration header file

### <b>Hardware and Software environment</b>

-   This demonstration runs on STM32U585xx devices without security enabled (TZEN=0).

-   This demonstration requires a WiFi access point to connect to
    -   With a transparent Internet connectivity: No proxy, no firewall blocking the outgoing traffic.
    -   Running a DHCP server delivering the IP and DNS configuration to the board.

-   This demonstration has been tested with STMicroelectronics B-U585I-IOT02A RevC (MB1551) board and can be easily
tailored to any other supported device, development board and MXCHIP firmware.

-   This demonstration has been tested with google chrome browser version 90.0.4430.93 (64-bit).

-   The EMW3080B MXCHIP Wi-Fi module firmware used version is V2.1.11 and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :  
    -   WARNING: Before opening the project with any toolchain be sure your folder
    installation path is not too in-depth since the toolchain may report errors
    after building.

-   Open your preferred toolchain  
-   Build all files and load your image into target memory  

-   Board must be connected to a PC with USB (ST-LINK USB port),
     open a serial terminal emulator, find the board's COM port and configure it with:  
    -   8N1, 115200 bauds, no HW flow control  
    -   set the line endings to CR + LF (Transmit) and LF (receive).  

-   Run the demonstration