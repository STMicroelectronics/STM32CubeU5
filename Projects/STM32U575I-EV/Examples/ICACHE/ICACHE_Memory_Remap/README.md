## <b>ICACHE_Memory_Remap Example Description</b>

- How to execute code from a remapped region configured through the ICACHE HAL driver.

- This example describes how to remap the external SRAM to C-bus and execute code located in this memory.

- First the code turn on the **Green led** from external SRAM before remap, then will turn it off after memory remapping.

- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575ZI-Q Devices :

    The CPU at 160MHz

STM32U575I-EV board's LED can be used to monitor the transfers status:

 - **Green LED is off** when no error detected.
 - **Red LED is ON** when any error occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

ICACHE, MPU, Remap_Memory, external_SRAM, C-bus

### <b>Directory contents</b>

  - ICACHE/ICACHE_Remapping_Region/Src/main.c                  Main program
  - ICACHE/ICACHE_Remapping_Region/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - ICACHE/ICACHE_Remapping_Region/Src/stm32u5xx_it.c          Interrupt handlers
  - ICACHE/ICACHE_Remapping_Region/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - ICACHE/ICACHE_Remapping_Region/Inc/main.h                  Main program header file
  - ICACHE/ICACHE_Remapping_Region/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - ICACHE/ICACHE_Remapping_Region/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - ICACHE/ICACHE_Remapping_Region/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550A)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

