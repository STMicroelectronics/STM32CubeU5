## <b>Nx_HTTP_SSE application Description</b>

The HTTP_Server_SentEvents_Socket application guides STM32Cube HAL API users to run a http server application.

The communication is done with a web browser application in a remote PC.

This HTTP server contains one html page:
  - the page is updated each time events are sent by the server, meaning information about the Live thread list.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

WiFi, HTTP Server, TCP/IP, server-sent events


### <b>Hardware and Software environment</b>

 - The EMW3080B MXCHIP Wi-Fi module firmware and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - Before using this project, you shall update your B-U585I-IOT02A RevC board with the EMW3080B firmware version 2.1.11.
   To achieve this, follow the instructions given at the above link, using the EMW3080updateV2.1.11RevC.bin flasher under the V2.1.11/SPI folder.

 - This application runs on STM32U585xx devices without security enabled (TZEN=0).

 - This application has been tested with the following environments:

   - B-U585I-IOT02A (MB1551C)
   - HTTP client: Google Chrome (v83)

 - Remote PC Set-up
   - PC must share the same WiFi network configuration from the server on board.


###  <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - WARNING: Before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.

 - Open your preferred toolchain

 - Edit the file `Core/Inc/mx_wifi_conf.h` to enter the name of your WiFi access point (SSID) to connect to and its password.

 - Build all files and load your image into target memory

 - Run the application
 
 - On your PC or smartphone (wifi must be configured with same SSID), you open a browser with IP address returned by network
    example: Network Interface connected: 192.168.1.24 , open your browser and execute command http://192.168.1.24
     
 - You can open until 5 sessions
