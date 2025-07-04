
## <b>Ux_Device_DFU Application Description</b>

This application provides an example of Azure RTOS USBX stack usage on NUCLEO-U5A5ZJ-Q board,
it shows how to develop USB Device Firmware Upgrade "DFU" based application.

The application is designed to emulate an USB DFU device, the code provides all required device descriptors framework
and associated class descriptor report to build a compliant USB DFU device.

At the beginning ThreadX calls the entry function tx_application_define(), at this stage, all USBx resources
are initialized, the DFU class driver is registered and the application creates 2 threads with the same priorities :

  - app_ux_device_thread_entry     (Prio : 10; PreemptionPrio : 10) used to initialize USB OTG HAL PCD driver and start the device.
  - usbx_dfu_download_thread_entry (Prio : 20; PreemptionPrio : 20) used to execute download command.

The thread usbx_app_thread_entry is responsible to start or stop the USB device.
At Run mode the thread will be waiting on message queue form USB_PD interface, when the USB device is plugged to host PC
a callback in USB_PD interface will send a message to usbx_app_thread_entry to start the USB device.
By the same way when the USB device is unplug a callback in USB_PD interface will send a message to usbx_app_thread_entry to stop the USB device.

The DFU transactions are based on Endpoint 0 (control endpoint) transfer. All requests and status
control are sent/received through this endpoint.

The internal flash memory is split as follows:
 - DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
 - Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address]: Read, Write, and Erase
   access

#### <b>Note 1</b>

In this application, two operating modes are available:

 1. DFU operating mode:
    This mode is entered after an MCU reset in case:
     - The DFU mode is forced by the user: the user presses the TAMP button.
     - No valid code found in the application area: a code is considered valid if the MSB of the initial
       Main Stack Pointer (MSP) value located in the first address of the application area is equal to
       0x20000.

 2. Run-time application mode:
    This is the normal run-time activities. A binary which toggles LED1 and LED2 on the NUCLEO-U5A5ZJ-Q board "NUCLEO-U5A5ZJ-Q_LED_Toggle_@0x08020000.dfu" is provided in Binary directory.

#### <b>Note 2</b>

Traditionally, firmware is stored in Hex, S19 or binary files, but these formats do not contain the
necessary information to perform the upgrade operation, they contain only the actual data of the program
to be downloaded. However, the DFU operation requires more information, such as the product identifier,
vendor identifier, firmware version and the alternate setting number (target ID) of the target to be
used, this information makes the upgrade targeted and more secure. To add this information, DFU file
format is used. For more details refer to the "DfuSe File Format Specification" document (UM0391).

To generate a DFU image, download "DFUse Demonstration" tool and use DFU File Manager to convert a
binary image into a DFU image. This tool is for download from www.st.com
To download a *.dfu image, use "DfuSe Demo" available within "DFUse Demonstration" install directory.

Please refer to UM0412, DFuSe USB device firmware upgrade STMicroelectronics extension for more details
on the driver installation and PC host user interface.

After each device reset (unplug the NUCLEO-U5A5ZJ-Q board from PC), plug the NUCLEO-U5A5ZJ-Q board with Key User push-button button pressed to enter the DFU mode.

#### <b>Note 3</b>

In case of composite device, we shall :

  1. Uninstall device composite driver.
  2. Activate the install of unsigned drivers in windows.
  3. Updated STtube.inf driver by replacing this line in all sections from
     %USB\VID_0483&PID_DF11_DeviceDesc%=STTub30.Device, USB\VID_0483 to
     %USB\VID_0483&PID_DF11_DeviceDesc%=STTub30.Device, USB\VID_0483&PID_DF11&MI_00
  4. Reinstall driver.

#### <b>Expected success behavior</b>

When plugged to PC host, the STM32U5A5xx must be properly enumerated as an USB DFU device.
During the enumeration phase, device provides host with the requested descriptors (device, configuration, string).
Those descriptors are used by host driver to identify the device capabilities.
Once the STM32U5A5xx USB device successfully completed the enumeration phase, open DfuSe you can now download and upload to/from media.

#### <b>Error behaviors</b>

Host PC shows that USB device does not appear as DFU device.

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" specification and DFU class specification.

#### <b>Known limitations</b>

Only Dfuse is supported. CubeProgrammer is not yet supported.
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

RTOS, ThreadX, USBXDevice, USB_OTG, Full Speed, DFU, USBPD, Firmware upgrade

### <b>Hardware and Software environment</b>

  - This application runs on STM32U5A5xx devices.
  - This application has been tested with STMicroelectronics NUCLEO-U5A5ZJ-Q boards revision MB1549-U5A5JQ-C04 and can be easily tailored to any other supported device and development board.

  - NUCLEO-U5A5ZJ-Q board Set-up
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
      For Windows 10 and later : Update STM32 DFU device driver manually from Windows Device Manager.
      The install of required device driver is available under:
      "Program Files\STMicroelectronics\Software\DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - Connect the NUCLEO-U5A5ZJ-Q board to the PC.
    - Connect the NUCLEO-U5A5ZJ-Q board to the PC through "TYPE-C" to "Standard A" cable.
     - CN1 : to use USB Full Speed OTG IP in full speed.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - To run the application, proceed as follows:
   - Install "DfuSe Demonstrator"
   - Install the DFU driver available in "DfuSe Demonstrator" installation directory
   - For Windows 10 and later : Update STM32 DFU device driver manually from Windows Device Manager.
     The install of required device driver is available under:
     "Program Files\STMicroelectronics\Software\DfuSe v3.0.6\Bin\Driver\Win10" directory.
   - Open "DfuSe Demo", choose the "NUCLEO-U5A5ZJ-Q_LED_Toggle_@0x08020000.dfu" provided in Binary
     directory, upgrade and verify to check that it is successfully downloaded.
   - This application allows also to upload a dfu file (either the provided DFU file or by creating a new dfu file).
     To check that the upload was successfully performed, choose the dfu uploaded file, upgrade and verify.
   - To run the downloaded application, execute the command "leave the DFU mode" or simply reset the
     board.
