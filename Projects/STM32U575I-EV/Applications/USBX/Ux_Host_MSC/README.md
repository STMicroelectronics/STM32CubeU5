
## <b>Ux_Host_MSC Application Description</b>

This application provides an example of Azure RTOS USBX stack usage. It shows how to develop USB Host Mass Storage "MSC" able to enumerate and communicates with a removable usb flash disk.

The application is designed to behave as an USB MSC Host able to operate with an USB flash disk using the Bulk Only Transfer (BOT) and Small Computer System Interface (SCSI) transparent commands combined with a file system Azure RTOS FileX.

The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all USBx resources are initialized, the MSC Class driver is registered.
The application creates two threads :

  - usbx_app_thread_entry    (Priority : 10; Preemption threshold : 10) used to initialize USB DRD HAL HCD driver.
  - msc_process_thread_entry (Priority : 30; Preemption threshold : 30) used to proceed to file operations once the device is properly enumerated.

####  <b>Expected success behavior</b>

When an usb flash disk is plugged to STM32U575I-EV board, a Message will be displayed on the uart HyperTerminal showing  the Vendor ID and the Product ID of the attached device.
After enumeration phase , the host proceed to file operations :

  - Create a "Test.txt" file.
  - Write  a small text in the created file.
  - Read the written text and check data integrity
  - Close the File

During the file operations process a message will be displayed on the HyperTerminal to indicates the outcome of each operation  (Create/Write/Read/Close).
If all operations were successful a message will be displayed on the HyperTerminal to indicates the end of operations.

#### <b>Error behaviors</b>

Errors are detected such as (Unsupported device, Enumeration Fail, File operations fail)and the corresponding message is displayed on the HyperTerminal.

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" Specification and Mass storage class Specification.

#### <b>Known limitations</b>

None

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

### <b>Keywords</b>

Connectivity, USBXHost, USBPD, FILEX, ThreadX, MSC, Mass Storage, BOT, SCSI, Removable drive, UART/USART

### <b>Hardware and Software environment</b>

  - This application runs on STM32U575xx devices
  - This application has been tested with STMicroelectronics STM32U575I-EV boards revision: MB1550-U575AIQ-C02
    and can be easily tailored to any other supported device and development board.

- STM32U575I-EV Set-up
    - Plug the USB MSC device into the STM32U575I-EV board through 'Type C  to A-Female' cable to the connector:
      - CN1 : to use USB OTG IP in full speed (FS)
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:
    - Hyperterminal configuration
      - Data Length = 8 Bits
      - One Stop Bit
      - No parity
      - BaudRate = 115200 baud
      - Flow control: None

<b>Note</b>

It is mandatory to check that the Jumpers below are fitted:
    JP25           : UCPD_5V Jumper is fitted in order to provide Vbus 5V.
    JP8 (UCPD_SRC) : This application initialize the type C port in source mode with only one PDO at 5V.
    JP14 and JP15  : To ensure the correct functionality of TCPP03(Protection IC).

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Open the configured uart HyperTerminal in order to display debug messages.
 - Run the application

<b>Note</b>

 - The user has to check the list of the COM ports in Device Manager to find out the number of the
 - COM ports that have been assigned (by OS) to the Stlink VCP.
 - The application uses the external HSE clock as USB source clock.
