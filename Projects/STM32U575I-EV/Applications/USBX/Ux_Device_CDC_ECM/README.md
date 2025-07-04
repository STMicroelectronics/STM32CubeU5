
## <b>Ux_Device_CDC_ECM Application Description</b>

This application provides an example of Azure RTOS CDC_ECM stack usage on STM32U575I-EV board, it shows how to run Web HTTP server based application stack
over USB interface. The application is designed to load files and web pages stored in SD card using a Web HTTP server through USB interface using CDC_ECM
class, the code provides all required features to build a compliant Web HTTP Server. The main entry function tx_application_define() is called by ThreadX during
kernel start, at this stage, the USBX initialize the network layer through USBx Class (CDC_ECM) also the FileX and the NetXDuo system are initialized,
the NX_IP instance and the Web HTTP server are created and configured, then the application creates two main threads

  - app_ux_device_thread_entry (Prio : 10; PreemptionPrio : 10) used to initialize USB OTG HAL PCD driver and start the device.
  - nx_server_thread_entry (Prio :10; PreemptionPrio :10) used to assign a dynamic IP address, open the SD card driver as a FileX Media and start the Web HTTP server.
  Fetching a dynamic IP address to the stm32U575I-EV board is a step blocking until an IP address is obtained.
  Once the server is started, the user's browser can load web pages as index.html and STM32U5xxLED.html.

#### <b>Expected success behavior</b>

When an SD card is inserted into the STM32U575I-EV SD card reader and the board is powered up and connected to DHCP enabled Ethernet network.
Then the web page can be loaded on the web browser after entring the url http://@IP/index.html also the user can switch to STM32U5xxLED.html page to toggle the green led.
An example web pages is provided for testing the application that can be found under "USBX/Ux_Device_CDC_ECM/Web_Content/"

#### <b>Error behaviors</b>

If the WEB HTTP server is not successfully started, the red LED starts toggling while the green LED is turned OFF.
In case of other errors, the Web HTTP server does not operate as designed (Files stored in the SD card are not loaded in the web browser).

#### <b>Assumptions if any</b>

The uSD card must be plugged before starting the application.
The board must be in a DHCP Ethernet network.

#### <b>Known limitations</b>

Hotplug is not implemented for this example, that is, the SD card is expected to be inserted before application running.
The remote wakeup feature is not yet implemented (used to bring the USB suspended bus back to the active condition).

### <b>Notes</b>

None

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...) but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
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
    The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
    Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
    Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

#### <b>USBX usage hints</b>

None

#### <b>FileX/LevelX usage hints</b>

- FileX sd driver is using the DMA, thus the DTCM (0x20000000) memory should not be used by the application, as it is not accessible by the SD DMA.
- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.
- Should make sure to configure the USB pool memory region with attribute "Non-Cacheable" to ensure coherency between CPU and USB DMA

### <b>Keywords</b>

RTOS, ThreadX, USBXDevice, USBPD, Network, NetxDuo, FileX, File ,SDMMC, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32U575xx devices.
  - This application has been tested with STMicroelectronics STM32U575I-EV boards revision: MB1550-U575AIQ-C02
    and can be easily tailored to any other supported device and development board.

- STM32U575I-EV Set-up
    - Plug the USB HID device into the STM32U575I-EV board through 'Type C  to A-Female' cable to the connector:
      - CN1 : to use USB OTG IP in full speed (FS)
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = none
    - Flow control = None

<b>Note</b>

In case the board is powered only with Cable Type-C:
  It is mandatory to check that the Jumpers below are fitted:
    JP6  (UCPD_SNK03).
    JP7  (USBPD_SNK01).
    JP14 (UCPD_FTL).
    JP25 (5V_UCPD).

In case the board is powered by another source (5V_STLK, 5V-CHG, 5V-DC or 5V_EXT):
  It is mandatory to check that the jumper below is disconnected:
    JP6  (UCPD_SNK03).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 - Under host linux desktop go to the "Ethernet Network" and configure the IPV4 Method to "Shared to other computers".
