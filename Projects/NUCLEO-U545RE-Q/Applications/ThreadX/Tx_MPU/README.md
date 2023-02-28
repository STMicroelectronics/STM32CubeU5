
## <b>Tx_MPU Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX Module feature.
It demonstrates how to load, start and unload modules. In addition, it shows how ThreadX memory protection on modules using the Memory Protection Unit (MPU).

This project is composed of two sub-projects:

 - Tx_Module_Manager : ThreadX Module Manager code that load and start the module dynamically at runtime.
 - Tx_Module : ThreadX Module code that is to be loaded and started by the module manager dynamically at runtime.


At the module manager stage, the main entry function tx_application_define() is called by ThreadX during kernel start, the application creates 1 thread and 1 message queue:

  - ModuleManager (Prio : 4; Preemption Threshold : 4)
  - ResidentQueue (Size : 16 * ULONG)

ModuleManager thread uses the ThreadX Module Manager APIs to configure, load and start the expected module. ResidentQueue is used to synchronize operations between Module Manager and the loaded Module.

At the module stage, the main entry function default_module_start() is called by ThreadX during module start, the application creates 1 thread:
  - MainThread (Prio : 2; Preemption Threshold : 2)

MainThread is expected to execute data read and write operations to/from user-defined Shared Memory regions. Memory protection is then demonstrated by trapping the Module's attempt at writing to the shared Read Only region. A Memory Fault is then expected before the unload of the module and the module manager continues to run correctly.

####  <b>Expected success behavior</b>

  - LED_GREEN toggles every 500ms.
  - Information regarding the module processing progress printed to the serial port.

#### <b>Error behaviors</b>

On failure,an error message is printed to the serial port while the green LED is switched OFF.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None


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

### <b>Keywords</b>

RTOS, ThreadX, Threading, Message Queue, Module Manager, Module, MPU


### <b>Hardware and Software environment</b>

  - This example runs on STM32U545xx devices
  - This example has been tested with STMicroelectronics NUCLEO-U545RE-Q boards Revision: MB1549-B03.
    and can be easily tailored to any other supported device and development board.
  - A virtual COM port appears in the HyperTerminal:
      - Hyperterminal configuration:
        + Data Length = 8 Bits
        + One Stop Bit
        + No parity
        + BaudRate = 115200 baud
        + Flow control: None

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open Multi-projects workspace using your pereferred IDE
 - Rebuild Tx_Module project
 - Rebuild Tx_Module_Manager project
 - Flash the Tx_Module binary at address defined by MODULE_FLASH_ADDRESS
 - Set the "Tx_Module_Manager" as active application (Set as Active)
 - Run the example
