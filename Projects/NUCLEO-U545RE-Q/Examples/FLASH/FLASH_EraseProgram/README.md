## <b>FLASH_EraseProgram Example Description</b>

How to configure and use the FLASH HAL API to erase and program the internal

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 160 MHz.

After Reset, the Flash memory Program/Erase Controller is locked. A dedicated function
is used to enable the FLASH control register access.
Before programming the desired addresses, an erase operation is performed using
the flash erase page feature. The erase procedure is done by filling the erase init
structure giving the starting erase page and the number of pages to erase.
At this stage, all these pages will be erased one by one separately.


**Note:** If problem occurs on a page, erase will be stopped and faulty page will
be returned to user (through variable 'PageError').

Once this operation is finished, page quad-word programming operation will be performed
in the Flash memory. The written data is then read back and checked.

The NUCLEO-U545RE-Q board LEDs can be used to monitor the transfer status:

  - LED2 is ON when there are no errors detected after programming  
  - LED2 is blinking with a 1s period when there are errors detected after programming  
  - LED2 is blinking (100ms on, 2s off) when there is an issue during erase or program procedure  

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, FLASH, Erase, Program, Page, Mass Erase

### <b>Directory contents</b>

  - FLASH/FLASH_EraseProgram/Inc/stm32u5xx_nucleo_conf.h   BSP configuration file
  - FLASH/FLASH_EraseProgram/Inc/stm32u5xx_hal_conf.h      HAL Configuration file
  - FLASH/FLASH_EraseProgram/Inc/stm32u5xx_it.h            Header for stm32u5xx_it.c
  - FLASH/FLASH_EraseProgram/Inc/main.h                    Header for main.c module
  - FLASH/FLASH_EraseProgram/Src/stm32u5xx_it.c            Interrupt handlers
  - FLASH/FLASH_EraseProgram/Src/main.c                    Main program
  - FLASH/FLASH_EraseProgram/Src/stm32u5xx_hal_msp.c       MSP initialization and de-initialization
  - FLASH/FLASH_EraseProgram/Src/system_stm32u5xx.c        STM32U5xx system clock configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U545RETxQ devices.

  - This example has been tested with NUCLEO-U545RE-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
