## <b>FMC_SRAM_ReadWrite_DMA Example Description</b>

- This example describes how to configure the FMC controller and the DMA to access the SRAM memory based on the STM32Cube HAL API when security is not enabled (TZEN=0).
In this example, the user can perform read/write operations with the DMA on the SRAM. A data buffer is written to it, then read back and checked to verify its correctness.

- If the data is read correctly from SRAM, the GREEN LED is ON, otherwise the RED LED is ON.   
- In case of HAL initialization issue, the RED LED will toggle. 
- The project configures the maximum system clock frequency at 160Mhz.

- The user can chose his own configuration by commenting/uncommenting the defines for undesired/desired configurations in "main.h", for example, to configure the SRAM 
data bus width as 8 bits, uncomment the define for the configuration "FMC_NORSRAM_MEM_BUS_WIDTH_8".

- The SRAM is IS61WV102416BLL-10MLI.

- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Memory, FMC, DMA, SRAM, Read, Write, Initialization, TrustZone disabled

### <b>Directory contents</b>

  - FMC/FMC_SRAM_ReadWrite_DMA/Src/main.c                  Main program
  - FMC/FMC_SRAM_ReadWrite_DMA/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - FMC/FMC_SRAM_ReadWrite_DMA/Src/stm32u5xx_it.c          Interrupt handlers
  - FMC/FMC_SRAM_ReadWrite_DMA/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - FMC/FMC_SRAM_ReadWrite_DMA/Inc/main.h                  Main program header file
  - FMC/FMC_SRAM_ReadWrite_DMA/Inc/stm32u5xx_eval_conf.h   BSP Configuration file
  - FMC/FMC_SRAM_ReadWrite_DMA/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - FMC/FMC_SRAM_ReadWrite_DMA/Inc/stm32u5xx_it.h          Interrupt handlers header file
        
### <b>Hardware and Software environment</b>  

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).
    
  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b> 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

