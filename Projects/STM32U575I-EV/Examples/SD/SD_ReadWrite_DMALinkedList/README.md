## <b>SD_ReadWrite_DMALinkedList Example Description</b>

- This example performs some write and read transfers to SD Card with SDMMC IP internal DMA mode based on Linked list feature.

#### <b>Notes</b>

 1. This example works with Normal SD Card.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.

- The transfer clock is set at 24Mhz (SDMMC_CLK / Clock_DIV * 2) and configured within the SD Handle init parameters.
Next, prepare linked list nodes for write buffers and start the writing transfer to SD Card.
Then prepare linked list nodes for read buffers to write and start the raeding transfer from SD Card.

- STM32U575I-EV board's LEDs can be used to monitor the transfer status:

  - LED_BLUE  is ON when write/read transfer is ongoing.
  - LED_GREEN is Toggling  when write and read transfer is completed and check buffers are OK.
  - LED_RED is Toggling  when there is an error in transfer process or initialization.


 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Storage, SD, SDMMC, Read Write, Block, Sector, DMA, Linked List

### <b>Directory contents</b>

  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_DMALinkedList/Inc/main.h                  Header for main.c module
  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32u575i_eval_conf.h  STM32U575I-EV board configuration file
  - SD/SD_ReadWrite_DMALinkedList/Src/stm32u5xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_DMALinkedList/Src/main.c                  Main program
  - SD/SD_ReadWrite_DMALinkedList/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SD/SD_ReadWrite_DMALinkedList/Src/system_stm32u5xx.c      STM32U5xx system source file

        
### <b>Hardware and Software environment</b> 

  - This example runs on STM32U575xx devices.

  - This example has been tested with STM32U575I-EV board and can be
    easily tailored to any other supported device and development board.
    
  STM32U575I-EV Set-up :

  - Connect the uSD Card to the Micro SD connector (CN29).


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Plug the SD Card in the STM32U575I-EV
 - Run the example

