## <b>Tx_SecureLEDToggle_TrustZone Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX when the TrustZone feature is enabled (TZEN=1).
The application is running ThreadX thread in non-secure context that is invoking a secure function to toggle a led each 1 sec.
The main entry function tx_application_define() is called by ThreadX during kernel start, at the non-secure stage, the application creates 1 thread :
  - tx_app_thread (Prio : 5; Preemption Threshold : 5)
 
Once started, the <b>tx_app_thread</b> toggle the LED_GREEN before sleeping for 1s.

This project is composed of two sub-projects:

- one for the secure application part (xxxxx_S)
- one for the non-secure application part (xxxxx_NS).

Please remember that on system with security enabled:

- the system always boots in secure and the secure application is responsible for launching the non-secure application.
- the SystemInit() function in secure application sets up the SAU/IDAU, FPU and secure/non-secure interrupts allocations defined in partition_stm32u575xx.h file.

This project shows how to switch between secure and non-secure applications thanks to the system isolation performed to split the internal Flash and internal SRAM memories into two halves:

 - the first half for the secure application and
 - the second half for the non-secure application.

User Option Bytes configuration:

Please note that the internal Flash is fully secure by default in TZEN=1 and User Option Bytes SECWM1_PSTRT/SECWM1_PEND and SECWM2_PSTRT/SECWM2_PEND should be set according to the application configuration. 

####  <b>Expected success behavior</b>

LED_GREEN toggles every 1s 

#### <b>Error behaviors</b>

LED_RED toggles every 1 second if any error occurs.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

#### <b>Note</b>

The following sequence is needed to disable TrustZone:
  -	Boot from user Flash memory:
    1. Make sure that secure and non-secure applications are well loaded and executed (jump done on non-secure application).
    2. If not yet done, set RDP to level 1 through STM32CubeProgrammer. Then only Hotplug connection is possible during non-secure application execution.
    3. Use a power supply different from ST-LINK in order to be able to connect to the target.
    4. Uncheck the TZEN box and set RDP to level 0 (option byte value 0xAA), then click on Apply.
  -	Boot from RSS:
    1. Make sure to apply a high level on BOOT0 pin (make sure that nSWBOOT0 Option Byte is checked).
    2. If not yet done, set RDP to level 1 through STM32CubeProgrammer. Then only Hotplug connection is possible during non-secure application execution.
    3. Use a power supply different from ST-LINK in order to be able to connect to the target.
    4. Uncheck the TZEN box and set RDP to level 0 (option byte value 0xAA), then click on Apply.

Please refer to AN5347 for more details.

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

RTOS, ThreadX, Threading, TrustZone, TZEN


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices with security enabled (TZEN=1).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - User Option Bytes requirement (with STM32CubeProgrammer tool)

      - TZEN = 1                            System with TrustZone-M enabled
      - SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  All 128 pages of internal Flash Bank1 set as secure
      - SECWM2_PSTRT=0x1  SECWM2_PEND=0x0   No page of internal Flash Bank2 set as secure, hence Bank2 non-secure

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Make sure that the system is configured with the security enable (TZEN=1) (option byte)
 
EWARM

 - Open your toolchain
 - Open Multi-projects workspace file Project.eww
 - Set the "xxxxx_S" as active application (Set as Active)
 - Rebuild xxxxx_S project
 - Rebuild xxxxx_NS project
 - Load the secure and non-secures images into target memory (Ctrl + D)
 - Run the example

MDK-ARM

 - Open your toolchain
 - Open Multi-projects workspace file Project.uvmpw
 - Select the xxxxx_S project as Active Project (Set as Active Project)
 - Build xxxxx_S project
 - Select the xxxxx_NS project as Active Project (Set as Active Project)
 - Build xxxxx_NS project
 - Load the non-secure binary (F8)
   (this shall download the \MDK-ARM\xxxxx_ns\Exe\Project_ns.axf to flash memory)
 - Select the xxxxx_S project as Active Project (Set as Active Project)
 - Load the secure binary (F8)
   (this shall download the \MDK-ARM\xxxxx_s\Exe\Project_s.axf to flash memory)
 - Run the example
 
STM32CubeIDE

 - Open STM32CubeIDE
 - File > Import. Point to the STM32CubeIDE folder of the example project. Click Finish.
 - Build configuration: Set the same active build configuration: Debug (default) or Release for both projects xxxxx_S & xxxxx_NS
 - Select and build the xxxxx_NS project, this will automatically trigger the build of xxxxx_S project
 - Select the xxxxx_S project and select "Debug configuration" or "Run configuration" in function of the active build configuration
   - Double click on “STM32 Cortex-M C/C++ Application”
   - Select  “Startup” >  “Add” > 
     - Select the xxxxx_NS project 
 - Click Debug/Run to debug/run the example