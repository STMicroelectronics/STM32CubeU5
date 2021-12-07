## <b>DLYB_OSPI_PSRAM_ExhaustiveTuning Example Description</b>


How to use Delay Block (DLYB) with an Exhaustive Tuning.  
DLYB is used with OSPI PSRAM in indrect Read/Write mode.

This example describes how to tune delay block with OSPI PSRAM in indrect Read/Write mode.  
This Exhaustive tuning aims to browse all possible delay value and return its status (Passed/Failed).  
This example can be used to detrmine the range of delay value which can be used in user applications.  
Results will be printed into terminal I/O.

LED_GREEN toggles as soon as Tuning is precessing.  
LED_GREEN is ON at the end of Tuning.  
LED_RED toggles as soon as an error is returned by HAL API.

It uses the Terminal I/O to display information : 

 - When resorting to EWARM IAR IDE:
   Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
   Command Code is displayed on debugger as follows: View --> Serial Windows --> Debug (printf) Viewer
 
 - When resorting to STM32CubeIDE:
   In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
   Command Code is displayed on debugger as follows: Window--> Show View--> Console.
   
The project configures the maximum system clock frequency at 160Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 3. This example calls CACHE_Enable() in order to enable the Instruction
    and Data Cache. This function is provided  in order to enhance performance */

### <b>Keywords</b>

Memory, DLYB, OCTOSPI, Memory Mapped, PSRAM

### <b>Directory contents</b>

  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Src/main.c                  Main program
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Src/stm32u5xx_it.c          Interrupt handlers
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Inc/main.h                  Main program header file
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Inc/b_u585i_iot02_conf.h    BSP Configuration file
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - DLYB/DLYB_OSPI_PSRAM_ExhaustiveTuning/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

