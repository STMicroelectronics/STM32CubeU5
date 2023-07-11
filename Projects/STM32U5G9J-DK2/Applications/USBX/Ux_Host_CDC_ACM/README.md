
## <b>Ux_Host_CDC_ACM Application Description </b>

This application provides an example of Azure RTOS USBX stack usage .

The application is designed to behave as an USB CDC_ACM Host, the code provides required requests to properly enumerate
CDC devices , CDC_ACM Class APIs to send or receive data and display data on uart HyperTerminal.

The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all USBx resources
are initialized.
The application creates 3 threads with different priorities :

  - usbx_app_thread_entry            (Priority : 10; Preemption threshold : 10) used to initialize USB OTG HAL HCD driver and start the Host.
  - cdc_acm_send_app_thread_entry    (Priority : 30; Preemption threshold : 30) used to send data from host to device.
  - cdc_acm_recieve_app_thread_entry (Priority : 30; Preemption threshold : 30) used to receive data from the device.

#### <b> Expected success behavior</b>

- When a cdc device is plugged to STM32U5G9J-DK2 board, a Message will be displayed on the uart HyperTerminal (STlink VCP) showing
the Vendor ID and Product ID of the attached device. The host must be able to properly decode CDC_ACM class request data sent by the device.
After enumeration phase, a message will indicates that the device is ready for use.

- The Sent/received data are displayed on respective Hyperterminals.

#### <b>Error behaviors</b>

Errors are detected such as (Unsupported device, Enumeration Fail) and the corresponding message is displayed on the HyperTerminal.

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" Specification and CDC_ACM class Specification.

#### <b>Known limitations</b>

None

### <b>Notes</b>


#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This require changes in the linker files to expose this memory location.
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


### <b>Keywords</b>

Connectivity, USBX Host, ThreadX, USB, CDC_ACM, UART, USART,


### <b>Hardware and Software environment</b>

  - This application runs on STM32U5G9xx devices
  - This application has been tested with STMicroelectronics STM32U5G9J-DK2 MB1918-U5G9ZJQ_S-A01.
    and can be easily tailored to any other supported device and development board.

- STM32U5G9J-DK2 Set-up
    - Plug the USB CDC_ACM device into the STM32U5G9J-DK2 board through 'Type C  to A-Female' cable to the connector:
      - CN5 : to use USB High Speed OTG IP (HS)
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:
     - Hyperterminal configuration
       - Data Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - Flow control: None

    A second virtual COM port will then appear in the HyperTerminal when plugging the device:
     - Hyperterminal configuration
       - Data Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 9600 baud
       - Flow control: None

     - To send data from the USB CDC_ACM Host use the user push-button and data will be received
     and displayed in the hyperterminal connected to the CDC ACM Device.

<b>Note</b>

The USB_TYPE_C CONNECTOR in STM32U5G9ZJT6Q Discovery is SINK ONLY (Vbus drive not supported), therefore this example can only be used
as a reference to show how to enable CDC Host functionality. User should ensure to inject 5V on USB Vbus pin to get the example working.

It is mandatory to check that the Jumpers below are fitted:
    JP4 : USBC_5V Jumper is fitted in order to provide Vbus 5V.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

<b>Note</b>

The user has to check the list of the COM ports in Device Manager to find out the number
of the COM ports that have been assigned (by OS) to the Stlink VCP.
The application uses the external HSE clock as USB source clock.
