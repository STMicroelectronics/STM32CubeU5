## <b>FLASH_EraseProgram_TrustZone Example Description</b>

How to configure and use the FLASH HAL API to erase and program the internal
Flash memory when TrustZone security is activated **(Option bit TZEN=1)**.

This project is composed of two sub-projects:

- One for the secure application part (FLASH_EraseProgram_TrustZone_S)
- One for the non-secure application part (FLASH_EraseProgram_TrustZone_NS).

Please remember that on system with security enabled, the system always boots in secure and
the secure application is responsible for launching the non-secure application.

The system isolation splits the internal Flash and internal SRAM memories into two halves:

 - The first half of Flash is used for the secure application code,
 - The second half of Flash is used for the non-secure application code,
 - The first half of SRAM is used for the secure application data,
 - The second half of SRAM is used for the non-secure application data.

**Secure Application** :

After Reset, the Flash memory Program/Erase Controller is locked. HAL_FLASH_Unlock() function
is used to enable the FLASH control register access.
Before programming the desired secured addresses, an erase operation is performed using
the flash erase page feature. The erase procedure is done by filling the erase init
structure giving the starting erase page and the number of pagess to erase.
At this stage, all these pages will be erased one by one separately.

#### <b>Notes</b>

 1. If problem occurs on a page, erase will be stopped and faulty page will
be returned to user (through variable 'PageError').

 2. Although code is the same for page erase between secure and non secure project, it matters
to mention that secure firmware is able to erase secure and non-secure pages within User Flash (Bank1) 
and non-secure firmware is only able to erase non-secure pages within User Flash (Bank2).

Once this operation is finished, page quad-word programming operation will be performed
in the secure part of Flash memory. The written data is then read back and checked.
The same instructions are done for the non-secure part of Flash memory.
Erase and page quad-word programming operation will be performed in the non-secure part of Flash memory 
from the secure world. The written data is then read back and checked.

Then the non-secure application is launched.

Non-Secure Application :

Same erase procedure is done on non-secured addresses. 

 3. If problem occurs on a page, erase will be stopped and faulty page will
be returned to user (through variable 'PageError').

Once this operation is finished, page quad-word programming operation will be performed
in the non-secure part of the Flash memory. The written data is then read back and checked.

User Option Bytes configuration:

Please note the internal Flash is fully secure by default in TZEN=1 and User Option Bytes
SECWM1_PSTRT/SECWM1_PEND and SECWM2_PSTRT/SECWM2_PEND should be set according to the application
configuration. Here the proper User Option Bytes setup in line with the project linker/scatter
file is as follows:

     - TZEN=1
     - SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  meaning all 128 pages of Bank1 set as secure
     - SECWM2_PSTRT=0x1  SECWM2_PEND=0x0   meaning no page of Bank2 set as secure, hence Bank2 set as non-secure

This project is targeted to run on STM32U5 device on boards from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz in non-secure application.

The STM32U575(MB1549) board LEDs can be used to monitor the transfer status:

 - **LED_GREEN is ON** when there are no errors detected after programming in secure and non-secure areas
 - **LED_RED is ON** when there are errors detected after programming in secure and non-secure areas

 4. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 5. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 6. The following sequence is needed to disable TrustZone:
 
      - **Boot from user Flash memory**:  
         a.	Make sure that secure and non-secure applications are well loaded and executed (jump done on non-secure application).
         b.	If not yet done, set RDP to level 1 through STM32CubeProgrammer. Then only Hotplug connection is possible during non-secure application execution.
         c.	Use a power supply different from ST-LINK in order to be able to connect to the target.
         d.	Uncheck the TZEN box and set RDP to level 0 (option byte value 0xAA), then click on Apply.

     - **Boot from RSS**:
         a.	Make sure to apply a high level on BOOT0 pin (make sure that nSWBOOT0 Option Byte is checked).
         b.	If not yet done, set RDP to level 1 through STM32CubeProgrammer. Then only Hotplug connection is possible during non-secure application execution.
         c.	Use a power supply different from ST-LINK in order to be able to connect to the target.
         d.	Uncheck the TZEN box and set RDP to level 0 (option byte value 0xAA), then click on Apply.
		 
	Please refer to AN5347 for more details.	 

### <b>Keywords</b>

Security, TrustZone, Memory, Flash, Erase, Program

### <b>Directory contents</b>

  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Src/main.c                     Secure Main program
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Src/secure_nsc.c               Secure Non-Secure Callable (NSC) module
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Src/stm32u5xx_it.c             Secure Interrupt handlers
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Src/system_stm32u5xx_s.c       Secure STM32U5xx system clock configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Inc/main.h                     Secure Main program header file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Inc/partition_stm32u575xx.h    STM32U5 Device System Configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Inc/stm32u5xx_hal_conf.h       Secure HAL Configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Inc/stm32u5xx_it.h             Secure Interrupt handlers header file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure/Inc/stm32u5xx_nucleo_conf.h    Secure BSP configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/Secure_nsclib/secure_nsc.h            Secure Non-Secure Callable (NSC) module header file
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Src/main.c                  Non-secure Main program
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Src/stm32u5xx_hal_msp.c     Non-secure HAL MSP module
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Src/stm32u5xx_it.c          Non-secure Interrupt handlers
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Src/system_stm32u5xx_ns.c   Non-secure STM32U5xx system clock configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Inc/main.h                  Non-secure Main program header file
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Inc/stm32u5xx_nucleo_conf.h Non-secure BSP Configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Inc/stm32u5xx_hal_conf.h    Non-secure HAL Configuration file
  - FLASH/FLASH_EraseProgram_TrustZone/NonSecure/Inc/stm32u5xx_it.h          Non-secure Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575IQ devices with security enabled (TZEN=1).

  - This example has been tested with STMicroelectronics STM32U575 Nucleo (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - User Option Bytes requirement (with STM32CubeProgrammer tool)

        - TZEN = 1                            System with TrustZone-M enabled
        - SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  All 128 pages of internal Flash Bank1 set as secure
        - SECWM2_PSTRT=0x1  SECWM2_PEND=0x0   No page of internal Flash Bank2 set as secure, hence Bank2 non-secure

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

<b>IAR</b>

 - Open your toolchain
 - Open Multi-projects workspace file Project.eww
 - Set the "FLASH_EraseProgram_TrustZone_S" as active application (Set as Active)
 - Rebuild FLASH_EraseProgram_TrustZone_S project
 - Rebuild FLASH_EraseProgram_TrustZone_NS project
 - Load the secure and non-secures images into target memory (Ctrl + D)
 - Run the example

<b>MDK-ARM</b>

 - Open your toolchain
 - Open Multi-projects workspace file Project.uvmpw
 - Select the FLASH_EraseProgram_TrustZone_S project as Active Project (Set as Active Project)
 - Build FLASH_EraseProgram_TrustZone_S project
 - Select the FLASH_EraseProgram_TrustZone_NS project as Active Project (Set as Active Project)
 - Build FLASH_EraseProgram_TrustZone_NS project
 - Load the non-secure binary (F8)
   (this shall download the \MDK-ARM\Objects\FLASH_EraseProgram_TrustZone_NS.axf to flash memory)
 - Select the FLASH_EraseProgram_TrustZone_S project as Active Project (Set as Active Project)
 - Load the secure binary (F8)
   (this shall download the \MDK-ARM\Objects\FLASH_EraseProgram_TrustZone_S.axf to flash memory)
 - Run the example

<b>STM32CubeIDE</b>

 - Open STM32CubeIDE
 - File > Import. Point to the STM32CubeIDE folder of the example project. Click Finish.
 - Build configuration: Set the same active build configuration: Debug (default) or Release for both projects xxxxx_S & xxxxx_NS
 - Select and build the xxxxx_NS project, this will automatically trigger the build of xxxxx_S project
 - Select the xxxxx_S project and select "Debug configuration" or "Run configuration" in function of the active build configuration
   - Double click on “STM32 Cortex-M C/C++ Application”
   - Select  “Startup” >  “Add” > 
   - Select the xxxxx_NS project 
 - Click Debug/Run to debug/run the example
