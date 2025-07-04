
## <b>USBPD_SNK_Ux_Device_HID_CDC_ACM Application Description</b>

This application is a USBPD type C Consumer and USB device using Azure RTOS USBX stack on B-U585I-IOT02A board.
It shows how to develop a USBPD type C Consumer in the case of a composite USB device communication class "HID" and "CDC_ACM" based application.

composite USB device :

The application is designed to emulate an USB HID mouse device and USB-to-UART bridge following the Virtual COM Port (VCP) implementations,
the code provides all required device descriptors framework and associated to both classes descriptor report to build a composite compliant USB HID_CDC_ACM device.
At the beginning ThreadX call the entry function tx_application_define(), at this stage, all USBx resources are initialized, the CDC_ACM and HID class driver is
registered and the application creates 4 threads with the same priorities :

  - usbx_app_thread_entry           (Prio : 20; PreemptionPrio : 20) used to initialize USB OTG HAL PCD driver and start the device.
  - usbx_cdc_acm_read_thread_entry  (Prio : 20; PreemptionPrio : 20) used to read the received data from Virtual COM Port.
  - usbx_cdc_acm_write_thread_entry (Prio : 20; PreemptionPrio : 20) used to send the received data over UART .
  - usbx_hid_thread_entry (Prio : 20; PreemptionPrio : 20) used to send HID reports to move automatically the PC host machine cursor.

The thread usbx_app_thread_entry is responsible to start or stop the USB device.
At run mode the thread will be waiting on message queue form USB_PD interface, when the USB device is plugged to host PC
a callback in USB_PD interface will send a message to usbx_app_thread_entry to start the USB device.
By the same way when the USB device is unplugged a callback in USB_PD interface will send a message to usbx_app_thread_entry to stop the USB device.

During enumeration phase, four communication pipes "endpoints" are declared in the CDC class and HID implementations :

 - 1 x Bulk IN endpoint for receiving data from STM32 device to PC host:
   When data are received over UART they are saved in the buffer "UserTxBufferFS". Periodically, in a
   usbx_cdc_acm_write_thread_entry the state of the buffer "UserTxBufferFS" is checked. If there are available data, they
   are transmitted in response to IN token otherwise it is NAKed.

 - 1 x Bulk OUT endpoint for transmitting data from PC host to STM32 device:
   When data are received through this endpoint they are saved in the buffer "UserRxBufferFS" then they are transmitted over UART using DMA mode and in meanwhile the OUT endpoint is NAKed.
   Once the transmission is over, the OUT endpoint is prepared to receive next packet in HAL_UART_RxCpltCallback().

 - 1 x Interrupt IN endpoint for setting and getting serial-port parameters:
   When control setup is received, the corresponding request is executed in ux_app_parameters_change().

In CDC_ACM application, two requests are implemented:
  - Set line: set the bit rate, number of stop bits, parity, and number of data bits
  - Get line: get the bit rate, number of stop bits, parity, and number of data bits
  The other requests (send break, control line state) are not implemented.

 - 1 x Interrupt IN endpoint for setting the HID position:
   When the User Button is pressed the application calls the GetPointerData() API to update the mouse position (x, y) and send
the report buffer through the ux_device_class_hid_event_set() API.

USBPD Consumer :

This application initialize the type C port 1 in sink mode with only one PDO at 5V.

<b>Note</b>

- Receiving data over UART is handled by interrupt while transmitting is handled by DMA allowing hence the application to receive data at the same time it is transmitting another data (full- duplex feature).
The support of the VCP interface is managed through the ST Virtual COM Port driver available for download from www.st.com.

- The user has to check the list of HID and the COM ports in Device Manager to find out the HID device and the COM port number that have been assigned (by OS) to the VCP interface.

#### <b>Expected success behavior</b>

When connected to PC host using Type-C cable, the STM32U585xx must be properly enumerated a composite device as an HID ,USB Serial device and an STlink COM port.
During the enumeration phase, device provides host with the requested descriptors (device, configuration, string).
Those descriptors are used by host driver to identify the device capabilities. 
Once STM32U585xx USB device successfully completed the enumeration phase.
Open two hyperterminals (USB COM port and UART COM port) to send/receive data to/from host to/from device.
When USER_Button is pressed, the device sends a HID report. Each report sent should move the PC host machine mouse cursor by one step.

#### <b>Error behaviors</b>

Host PC shows that USB device does not operate as designed (enumeration failed, for example PC cursor doesn't move or COM port enumeration failed).
LED_RED toggles if any error occurs.

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" specification and CDC_ACM class specification.

#### <b>Known limitations</b>

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
    or modify the line below in "tx_low_level_initilize.s to match the memory region being used
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

    + The "tx_initialize_low_level.s" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

#### <b>USBX usage hints</b>

None

### <b>Keywords</b>

RTOS, ThreadX, USBX, USBXDevice, USB_OTG, Full Speed, CDC, HID, VCP, USART, DMA, Mouse, USBPD
USB_PD_Lib, UCPD, Type C.

### <b>Hardware and Software environment</b>

  - This application runs on STM32U585xx devices.
  - This application has been tested with STMicroelectronics B-U585I-IOT02A boards revision MB1551-D01 and can be easily tailored to any other supported device and development board.

  - B-U585I-IOT02A Set-up
    - Connect the B-U585I-IOT02A board ST-LINK CN8 to the PC through "MICRO-USB" to "Standard A" cable.
    - VCP configuration is dynamic for example it can be :
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None
    - During example execution, connect the B-U585I-IOT02A board USB CN1 to the PC using Type-C cable.

  - The USART1 interface available on PA9 and PA10 of the microcontroller are connected to ST-LINK MCU.
  By default the USART1 communication between the target MCU and ST-LINK MCU is enabled.
  It's configuration is as following:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = None
    - Flow control = None

<b>Note</b>

 - If we configure the VCP baudrate under 9600 the USART1 baudrate shall be set to 9600.
 - _TRACE and _GUI_INTERFACE compilation switches are now disabled to allow proper execution of CDC_ACM application over VCP. If user wants to use STM32CUbeMonitorUCPD, those 2 flags need to be enabled in project compilation options.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application