
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>Nx_IAP_Client application Description</b>

The NX_IAP_Client application shows how to use the WiFi module to perform an IAP ("In
Application Programming") using STM32 Cube HAL.
This application may be used with a Smartphone or PC server (more detailed setup
instructions are described in section "Hardware and Software environment").

- If the "USER" Button is pressed when the application starts, then the WiFi IAP module
  is started. Otherwise, the user code downloaded into Flash offset is executed.
  If no code is downloaded yet into Flash memory offset, then the WiFi IAP application
  will start.

- At startup, the WiFi module is initialized, the UART terminal shows an `O.K!` message in case of success.
  Otherwise LED6 is blinking and an ERROR message is displayed on the UART terminal.

- Once initialized, the board attempts to join WiFi access point using the predefined SSID and
  password setup by user into configuration file at compilation time.
  In case of success the acquired IP address is displayed.

- After joining the access point, the board attempts to establish a connection to the
  remote HTTP server using the predefined address and port setup by user in configuration file.

- As soon as the connection to the remote server is established, the "hello.bin" file is downloaded.
  On success the following messages is displayed on the UART:

  `Downloaded firmware ' hello_u5.bin ', size = 26549 Bytes`

  otherwise a failure message is shown and the LED6 is blinking.

- When downloading is finished, the message `State: Programming...` is displayed on the UART.
  The "hello_xxx.bin" fw is being written into the flash.
  On Success the message `Programming Done!` is displayed on the UART and LED7 is blinking.
  If the flashing operation fails, an error message is displayed and the LED6 is blinking instead.

- Reset the board (using "RESET" button) and keep "USER" button unpressed in order to execute
  the binary file downloaded into Flash memory.

The following parameters should be configured by user at compilation time through configuration files:

 in `main.h`:
  - Host (server) address
  - Host Port number
  - File name
  - Maximum number of trials

 in `Core/Inc/mx_wifi_conf.h`:
  - Network SSID
  - Network Password


It is possible to configure a PC to run as a WiFi hotspot alongside a webserver.
For Windows PC you can use the [Wamp server](http://www.wampserver.com/en)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals and initialize the Flash interface.

The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as time base for the HAL drivers.



#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the HAL time base is always set to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, WiFi, Hotspot, IAP Client, HTTP, Web server, MXCHIP, SPI


### <b>Hardware and Software environment</b>

 - The EMW3080B MXCHIP Wi-Fi module firmware and the way to update your board with it
   are available at [x-wifi-emw3080b](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - Before using this project, you shall update your B-U585I-IOT02A RevC board with the EMW3080B firmware version 2.1.11.
   To achieve this, follow the instructions given at the above link, using the EMW3080updateV2.1.11RevC.bin flasher under the V2.1.11/SPI folder.

 - This application runs on STM32U585AII6Q devices without security enabled (TZEN=0).

 - This application has been tested with the following environments:

   - B-U585I-IOT02A board

   - A remote host running a web server (either Linux or Windows PC).

   - Environment Setup

     - When using Linux or Windows PC:
       - Start the Apache or Wamp server respectively on the Linux or Windows machine.
       - Copy the "hello.bin" file under an accessible path on the webserver
       - The PC should be connected to the local network and accessible via the WiFi access point.

   - Edit the file `main.h` and adjust the defines:

     `HOST_ADDRESS`    The IP Address of the PC or phone running the web server.

     `HOST_PORT`       The HTTP port used by the web server. When using Palpa WebServer on a mobile phone,
                       it is possible to edit the 'webserver root dir' under (WebServer Settings->Components->lighttpd->Port)

     `FW_FILE`         The relative path of the "hello_u5.bin" w.r.t the web server root dir. When using Palpa WebServer on a mobile phone,
                       it is possible to edit the 'webserver root dir' under (WebServer Settings->Components->lighttpd->Document Root)

     `DATA_MAX_SIZE`   Data size required to hold the "hello.bin". (should take into account HTTP overhead size)


### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain

 - Edit the file `Core/Inc/mx_wifi_conf.h` to enter the name of your WiFi access point (SSID) to connect to and its password.

 - Build all files and load your image into target memory

 - Run the application


#### <b>Notes</b>

To be correctly written into the Flash the fw binary maximum size can't exceed 256 kb.

:::
:::

