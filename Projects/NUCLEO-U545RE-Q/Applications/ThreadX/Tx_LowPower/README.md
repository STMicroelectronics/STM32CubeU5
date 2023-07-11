
## <b>Tx_LowPower Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX in LowPower mode.
It demonstrates how to configure the LowPower feature of ThreadX stack. In addition, it shows how to use ThreadX MACRO related to the LowPower feature.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, the application creates 1 thread and 1 semaphore:

  - tx_app_thread (Prio : 10; Preemption Threshold : 10)
  - tx_app_semaphore

- 'tx_app_thread':
  + try to acquire the 'tx_app_semaphore' immediately.
  + On Success toggle the 'LED_GREEN' each 500ms for 5 seconds.
  + repeat the steps above

The tx_app_semaphore is released each time user press the user button.

The TX_LOW_POWER_USER_ENTER is defined as App_ThreadX_LowPower_Enter(), that is called to make the system enter in stop mode using the HAL_PWREx_EnterSTOP2Mode() API.

The TX_LOW_POWER_USER_EXIT is defined as App_ThreadX_LowPower_Exit(), that is called to make the system exit from stop mode by reconfiguring the system clock.

Before entring to LwoPower mode the application :
- Enable only 8KB SRAM2 and icache full retention in Stop2 mode.
- Switch to SMPS.
- Configure the LPM read mode.
- Configure all GPIOs to analog mode.
- Disable all GPIO clocks.
- Disable all unused peripheral clocks.
- Disable AHB APB Peripheral Clock.

When exiting from LowPower mode the application :
- Enable AHB APB Peripheral Clock.
- Reconfigure the system clock.
- Initialize LEDs.

####  <b>Expected success behavior</b>

LED_GREEN toggles every 500ms for 5 seconds each time user press the user button.

#### <b>Error behaviors</b>

On failure,an error message is printed to the serial port while the green LED is switched OFF.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

#### <b>Notes</b>

 1. To measure the current consumption remove JP4 jumper
    and connect an amperemeter to JP4 to measure IDD current.

 2. This example can not be used in DEBUG mode due to the fact
    that the Cortex-M33 core is no longer clocked during low power mode
    so debugging features are disabled.

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

RTOS, ThreadX, Threading, Semaphore, LowPower, Tickless


### <b>Hardware and Software environment</b>

  - This example runs on STM32U545xx devices
  - This example has been tested with STMicroelectronics NUCLEO-U545RE-Q boards Revision: MB1841-A02.
    and can be easily tailored to any other supported device and development board.

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:

      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = none
      - Flow control = None

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
