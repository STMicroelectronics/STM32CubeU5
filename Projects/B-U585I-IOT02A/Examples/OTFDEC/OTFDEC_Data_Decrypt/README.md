## <b>OTFDEC_Data_Decrypt Example Description</b>

This example describes how to decrypt data (encrypted with CRYPT peripheral) located on the OCTOSPI external flash  using the OTFDEC peripheral

The leds of the B_U585I_IOT02 board are used to monitor the status as following:

LED_GREEN is ON when the checked data is correct.  
LED_RED is On when an OTFDEC error occurs
LED_RED toggles as soon as an error is returned by HAL API

This project is targeted to run on STM32U585xx devices on B_U585I_IOT02 board from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, OTFDEC, Memory Mapped, Read, Write, Cryptography

### <b>Directory contents</b>

  - OTFDEC/OTFDEC_Data_Decrypt/Src/main.c                  Main program
  - OTFDEC/OTFDEC_Data_Decrypt/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - OTFDEC/OTFDEC_Data_Decrypt/Src/stm32u5xx_it.c          Interrupt handlers
  - OTFDEC/OTFDEC_Data_Decrypt/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - OTFDEC/OTFDEC_Data_Decrypt/Inc/main.h                  Main program header file
  - OTFDEC/OTFDEC_Data_Decrypt/Inc/b_u585i_iot02_conf.h    BSP Configuration file
  - OTFDEC/OTFDEC_Data_Decrypt/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - OTFDEC/OTFDEC_Data_Decrypt/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B_U585I_IOT02 (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Please avoid to view memory with the debugger before decrypting data (view memory), it may block the decryption.

