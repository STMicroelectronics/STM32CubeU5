
## <b>Fx_uSD_File_Edit Application Description</b>

This application provides an example of Azure RTOS FileX stack usage on STM32U575I-EV board, it shows how to develop a basic SD card file

operations application.

The main entry function, tx_application_define(), is called by ThreadX during kernel start. At this stage, all FileX resources are initialized and drivers are initialized.

A single thread is created:

  - fx_app_thread (Prio : 10; PreemptionPrio : 10) used to initialize the SD driver and starting FileX's file operations.

A message queue is used to signal the SD card detection event to the fx_app_thread thread:

  - tx_msg_queue (Msg size : 1 (UINT); total messages : 16) used to notify the fx_app_thread about the SD card insertion status.

The fx_app_thread starts by checking whether the SD card is initially inserted or not. In the true case, it sends a message to the queue to ensure

that the first iteration starts properly. The wait on the message queue blocks till it receives a new message about whether the SD card is inserted

or removed. Interrupt callback for SD detection is registered and it is used to send the event information to the message queue.

The fx_app_thread uses FileX services to open the SD media for file operations and attempt to create file STM32.TXT. If the file exists already,

it will be overwritten. Dummy content is then written into the file and it is closed. The file is opened once again in read mode and content

is checked if matches what was previously written.

It is possible to unplug/plug or replace the SD card without any need to reset the STM32U575I-EV.
That is why MX_SDMMC1_SD_Init() should be public to initialize the newly plugged SD card.

#### <b>Expected success behavior</b>

- A file named STM32.TXT should be visible in the root directory of the SD card.
- A blinking green LED light marks the success of the file operations.

#### <b>Error behaviors</b>

- On failure, the red LED should start blinking.
- Error handler is called at the spot where the error occurred.

#### <b>Assumptions if any</b>
- The SD card should be plugged prior to run the application.

#### <b>Known limitations</b>

Performing quick plug/unplug of SD card may trigger the Error_Handler() function.

### <b>Notes</b>

 1. Before starting the application, the SD card should be present in the SD card connector and formatted initially by user.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL).
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


#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.


### <b>Keywords</b>

RTOS, ThreadX, FileX, File system, SDMMC, FAT32

### <b>Hardware and Software environment</b>

  - This application runs on STM32U575xx devices.
  - This application has been tested with STMicroelectronics STM32U575I-EV boards Revision: MB1550-U575AIQ-A03.
    and can be easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
