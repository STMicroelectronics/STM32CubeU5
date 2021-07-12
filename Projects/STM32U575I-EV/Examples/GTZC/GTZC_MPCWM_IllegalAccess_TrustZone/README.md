## <b>GTZC_MPCWM_IllegalAccess_TrustZone Example Description</b>

- How to use GTZC MPCWM-TZIC to build any example when TrustZone security is activated **(Option bit TZEN=1)**.

- The purpose of this example is to first declare the 1st half of external SRAM (FMC)(from 0x60000000) as non-secure with HAL GTZC MPCWM services and to demonstrate that
no security violation is detected when non-secure application accesses the 1st half of external SRAM.

**LED GREEN** toggles during this permanent read and write accesses.

- In this example, the SAU region 4 used for external memory access policy is voluntary modified in user code versus the initial setup from partition_stm32u575xx.h done in
secure SystemInit() in order to update the external SRAM (FMC) non-secure and secure areas.

- Any key press on **USER push-button** initiates a SecureFault interrupt because of an illegal read access from the non-secure world in secure external SRAM area as the
transaction does not respect the SAU access rule.

This toggles the **LED RED** once and a message is displayed on a terminal via a secure Non-Secure Callable functions in secure (UART1 secured).
**LED GREEN** toggling resumes.

- Any key press on **TAMPER push-button** initiates a GZTC interrupt because of an illegal read access from the non-secure world in external SRAM area although
the transaction is allowed by the SAU but not by the GTZC MPCWM.

This toggles the **LED RED** once and a message is displayed on a terminal via a secure
Non-Secure Callable functions in secure (UART1 secured). **LED GREEN** toggling resumes.

This project is composed of two sub-projects:

- One for the secure application part (Project_s)
- One for the non-secure application part (Project_ns).

- Please remember that on system with security enabled, the system always boots in secure and the secure application is responsible for launching the non-secure application.

This project mainly shows how to switch from secure application to non-secure application thanks to the system isolation performed to split the internal Flash and internal SRAM memories
into two halves:

 - The first half for the secure application and
 - The second half for the non-secure application.

User Option Bytes configuration:

Please note the internal Flash is fully secure by default in TZEN=1 and User Option BytesSECWM1_PSTRT/SECWM1_PEND and SECWM2_PSTRT/SECWM2_PEND should be set according to the application
configuration. Here the proper User Option Bytes setup in line with the project linker/scatter
file is as follows:

     - TZEN=1
     - SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  meaning all 128 pages of Bank1 set as secure
     - SECWM2_PSTRT=0x1  SECWM2_PEND=0x0   meaning no page of Bank2 set as secure, hence Bank2 non-secure

This example configures the maximum system clock frequency at 160Mhz.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. The following sequence is needed to disable TrustZone:
 
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

Security, TrustZone, GTZC, MPCWM, watermark memory protection, external SRAM, illegal access 

### <b>Directory contents</b>

  - Secure/Src/hal_sram_helper.c                 Helper file ton initialize and configure SRAM
  - Secure/Src/main.c                            Secure Main program
  - Secure/Src/secure_nsc.c                      Secure Non-secure callable functions
  - Secure/Src/stm32u5xx_hal_msp.c               Secure HAL MSP module
  - Secure/Src/stm32u5xx_it.c                    Secure Interrupt handlers
  - Secure/Src/system_stm32u5xx_s.c              Secure STM32U5xx system clock configuration file
  - Secure/Inc/hal_sram_helper.h                 Helper file ton initialize and configure SRAM
  - Secure/Inc/main.h                            Secure Main program header file
  - Secure/Inc/partition_stm32u575xx.h           STM32U5 Device System Configuration file
  - Secure/Inc/stm32u5xx_hal_conf.h              Secure HAL Configuration file
  - Secure/Inc/stm32u5xx_it.h                    Secure Interrupt handlers header file
  - Secure_nsclib/secure_nsc.h                   Secure Non-Secure Callable (NSC) module header file
  - NonSecure/Src/main.c                         Non-secure Main program
  - NonSecure/Src/stm32u5xx_hal_msp.c            Non-secure HAL MSP module
  - NonSecure/Src/stm32u5xx_it.c                 Non-secure Interrupt handlers
  - NonSecure/Src/system_stm32u5xx_ns.c          Non-secure STM32U5xx system clock configuration file
  - NonSecure/Inc/main.h                         Non-secure Main program header file
  - NonSecure/Inc/stm32u5xx_hal_conf.h           Non-secure HAL Configuration file
  - NonSecure/Inc/stm32u5xx_it.h                 Non-secure Interrupt handlers header file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices with security enabled (TZEN=1).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

  - User Option Bytes requirement (with STM32CubeProgrammer tool)
  
        TZEN = 1                            System with TrustZone-M enabled
        SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  All 128 pages of internal Flash Bank1 set as secure
        SECWM2_PSTRT=0x1  SECWM2_PEND=0x0   No page of internal Flash Bank2 set as secure, hence Bank2 non-secure
		
- **Output Traces**
This example activates on secure side the USART1 for trace over STLink UART link (Virtual COM Port).
So you may open an Hyperterminal on your PC
Hyperterminal configuration:

    - 115200 baud rate
    - 8 bit data
    - no parity
    - 1 bit stop
    - no flow control


### <b>How to use it ?</b>

In order to make the program work, you must do the following :


<b>IAR</b>

 - Open your toolchain
 - Open Multi-projects workspace file Project.eww
 - Set the "xxxxx_S" as active application (Set as Active)
 - Rebuild xxxxx_S project
 - Rebuild xxxxx_NS project
 - Load the secure and non-secures images into target memory (Ctrl + D)
 - Run the example

