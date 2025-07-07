
## <b>Tx_Thread_Creation Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX thread management APIs.
It demonstrates how to create and destroy multiple threads using Azure RTOS ThreadX APIs. In addition, it shows how to use preemption threshold between threads and change priorities on-fly.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, the application creates 3 threads with different priorities :

  - MainThread (Prio : 5; Preemption Threshold : 5)
  - ThreadOne  (Prio : 10; Preemption Threshold : 10)
  - ThreadTwo  (Prio : 10; Preemption Threshold : 9)

Once started, the <b>MainThread</b> is suspended waiting for the event flag.
The <b>ThreadOne</b> starts to toggle the <i>LED_GREEN</i> each 500ms and <b>ThreadTwo</b> cannot as its priority is less than the <b>ThreadOne</b> threshold.
After 5 seconds it sends an event <b>THREAD_ONE_EVT</b> to the <b>MainThread</b>.

After receiving the <b>THREAD_ONE_EVT</b> , the <b>MainThread</b> change the <b>ThreadTwo</b> priority to 8 and its preemption threshold to 8 to be more than the <b>ThreadOne</b>* threshold then waits for an event.

Now, the <b>ThreadTwo</b> can preempt the <b>ThreadOne</b> and start toggling the <i>LED_GREEN</i>each 200ms for 5 seconds. Once done it send the <b>THREAD_TWO_EVT</b> to <b>MainThread</b>.
Once <b>THREAD_TWO_EVT</b>  is received, the <b>MainThread</b> resets the <b>ThreadTwo</b> priority and preemption threshold to their original values (10, 10), <b>ThreadOne</b> is rescheduled and the above scenario is redone.
After repeating the sequence above 3 times, the <b>MainThread</b> should destroy <b>ThreadOne</b> and <b>ThreadTwo</b> and toggles the <i>LED_GREEN</i> each 1 second for ever.

####  <b>Expected success behavior</b>

  - <i>LED_GREEN</i> toggles every 500ms for 5 seconds
  - <i>LED_GREEN</i> toggles every 200ms for 5 seconds
  - Success status (After 3 times) : <i>LED_GREEN</i> toggles every 1 second forever.

#### <b>Error behaviors</b>

On failure,an error message is printed to the serial port while the green LED is switched OFF.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
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

### <b>Keywords</b>

RTOS, ThreadX, Threading, Event flags, Preemption threshold

### <b>Hardware and Software environment</b>

  - This application runs on STM32U545xx devices
  - This application has been tested with STMicroelectronics NUCLEO-U545RE-Q boards revision: MB1549-D01
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
