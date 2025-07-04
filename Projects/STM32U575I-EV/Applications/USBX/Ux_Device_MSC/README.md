## <b>Ux_Device_MSC Application Description</b>

This application provides an example of Azure RTOS USBX stack usage on STM32U575I-Eval board, it shows how to develop USB Device mass storage class based application.
The application is designed to emulate an USB MSC device, the code provides all required device descriptors framework and the associated Class descriptor report to build
a compliant USB MSC device.

At the beginning ThreadX calls the entry function tx_application_define(), at this stage, all USBx resources are initialized, the mass storage Class driver is registered
and the application creates one thread:

  - app_ux_device_thread_entry (Prio : 10; PreemptionPrio : 10) used to initialize USB OTG HAL PCD driver and start the device.
In addition two callback functions are needed for the USBX mass storage class device:

  - app_usb_device_thread_media_read used to read data through DMA from the mass storage device.
  - app_usb_device_thread_media_write used to write data through DMA into the mass storage device.

The thread app_ux_device_thread_entry is responsible to start or stop the USB device.
At Run mode the thread will be waiting on message queue form USB_PD interface, when the USB device is plugged to host PC
a callback in USB_PD interface will send a message to app_ux_device_thread_entry to start the USB device.
By the same way when the USB device is unplug a callback in USB_PD interface will send a message to app_ux_device_thread_entry to stop the USB device.

#### <b>Expected success behavior</b>

When plugged to PC host, the STM32U575I-Eval should enumerate as an USB MSC device. During the enumeration phase, device must provide host with the requested
descriptors (Device descriptor, configuration descriptor, string descriptors).

Those descriptors are used by the host driver to identify the device capabilities.

Once the STM32U575I-Eval USB device successfully completed the enumeration phase, a new removable drive appears in the system window and write/read/format
operations can be performed as with any other removable drive.

#### <b>Error behaviors</b>

Host PC shows that USB device does not operate as designed (MSC enumeration fails, The new removable drive appears but read, write or format operations fail).

#### <b>Assumptions if any</b>

  - USB cable should not be unplugged during enumeration and driver install.
  - SD card should be inserted before application is started.

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

RTOS, ThreadX, USBXDevice, USBPD, Device, USB_OTG, Full Speed, MSC, Mass Storage, SD Card, DMA, SDMMC

### <b>Hardware and Software environment</b>

  - This application runs on STM32U575xx devices.
  - This application has been tested with STMicroelectronics STM32U575I-EV boards revision: MB1550-U575AIQ-C02
    and can be easily tailored to any other supported device and development board.

- STM32U575I-EV Set-up
    - Plug the USB HID device into the STM32U575I-EV board through 'Type C  to A-Female' cable to the connector:
    - CN1 : to use USB OTG IP in full speed (FS)
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
