
## <b>Nx_HTTP_SSE Application Description</b>

This application provides an example of Azure RTOS NetX/NetXDuo stack usage.

The HTTP_Server_SentEvents_Socket application guides STM32Cube HAL API users to run a http server application.

The communication is done with a web browser application in a remote PC.


#### <b>Expected success behavior</b>

This HTTP server response one html page:
  - for each client, the page is updated each time **events are sent by the server**, meaning information about the Live thread list.

#### <b>Error behaviors</b>

An error message is displayed on the UART in case there are too many clients trying to connect.
Removing a web page for a client enables a new client to be requested.

#### <b>Assumptions if any</b>

None

#### <b>ThreadX usage hints</b>

- ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
- ThreadX is configured with 1000 ticks/sec, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the `tx_user.h`, the `TX_TIMER_TICKS_PER_SECOND` define, but this should be reflected in `tx_initialize_low_level.S` file too.
- ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
- ThreadX offers the `tx_application_define()` function, that is automatically called by the tx_kernel_enter() API.
  It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...) but it should not in any way contain a system API call (HAL or BSP).
- Using dynamic memory allocation requires to apply some changes to the linker file.
  ThreadX needs to pass a pointer to the first free memory location in RAM to the `tx_application_define()` function, using the `first_unused_memory` argument.
  This requires changes in the linker files to expose this memory location.
    - For EWARM add the following section into the .icf file:
     ```
        place in RAM_region    { last section FREE_MEM };
     ```
    - For MDK-ARM:
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in `tx_initialize_low_level.S` to match the memory region being used
    ```
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    - For STM32CubeIDE add the following section into the .ld file:
    ```
        ._threadx_heap :
        {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
        } >RAM_D1 AT> RAM_D1
    ```

    The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
    In the example above the ThreadX heap size is set to 64KBytes.
    The `._threadx_heap` must be located between the `.bss` and the `._user_heap_stack sections` in the linker script.
    Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
    Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    - The `tx_initialize_low_level.S` should be also modified to enable the `USE_DYNAMIC_MEMORY_ALLOCATION` compilation flag.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, WiFi, Access Point, server-sent events, MXCHIP, SPI

### <b>Hardware and Software environment</b>

 - To use the EMW3080B MXCHIP Wi-Fi module functionality, 2 software components are required:
   1. The module driver running on the STM32 device
   2. The module firmware running on the EMW3080B Wi-Fi module

 - This application uses an updated version of the EMW3080B MXCHIP Wi-Fi module driver V2.3.4.

 - The B-U585I-IOT02A Discovery board Revision C is delivered with the EMW3080B MXCHIP Wi-Fi module firmware V2.1.11;
   to upgrade your board with the required version V2.3.4, please visit [X-WIFI-EMW3080B](https://www.st.com/en/development-tools/x-wifi-emw3080b.html),
   using the `EMW3080update_B-U585I-IOT02A-RevC_V2.3.4_SPI.bin` file under the V2.3.4/SPI folder.

 - Please note that module firmware version V2.1.11 is not backwards compatible with the driver V2.3.4 (the V2.1.11 module firmware is compatible with the driver versions from V2.1.11 to V2.1.13).
 - The module driver is available under [/Drivers/BSP/Components/mx_wifi](../../../../../Drivers/BSP/Components/mx_wifi/), and its version is indicated in the [Release_Notes.html](../../../../../Drivers/BSP/Components/mx_wifi/Release_Notes.html) file.

 - Be aware that some STM32U5 SW packages (for examples X-CUBE-AZURE and X-CUBE-AWS) may continue to use older version of the EMW3080B MXCHIP module firmware;
   thanks to refer to the release notes of each SW package to know the recommended module firmware's version which can be retrieved from this page
   [X-WIFI-EMW3080B](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - This application runs on STM32U585xx devices without security enabled (TZEN=0).

 - This application has been tested with the following environments:

   - B-U585I-IOT02A (MB1551C)
   - HTTP client: Google Chrome (v83)

 - Remote PC Set-up
   - PC must share the same WiFi network configuration from the server on board.

 - This application uses USART1 to provide a console, the hyperterminal configuration is as follows:
   - BaudRate = 115200 baud
   - Word Length = 8 Bits
   - Stop Bit = 1
   - Parity = None
   - Flow control = None
   - Line endings set to LF (receive).

 - Remote PC Set-up
   - PC must share the same WiFi network configuration from the server on board, i.e., **"MyHotSpot"** non secure Access Point server with the **192.168.1.1** address.


### <b>How to use it?</b>

In order to make the program work, you must do the following:

 - WARNING: Before opening the project with any toolchain be sure your folder installation path is not too in-depth since the toolchain may report errors after building.

 - Open your preferred toolchain

 - Build all files and load your image into target memory

 - Run the application with Access Point mode set.

 - You can open until 5 sessions

