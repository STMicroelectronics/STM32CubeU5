## <b>FMC_SRAM Example Description</b>

- This example describes how to configure the FMC controller to access the SRAM memory based on the STM32Cube HAL API that can be used
to build any firmware application when security is not enabled (TZEN=0).
- The SRAM is IS61WV102416BLL-10MLI.

- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

- At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160MHz 

- If the data is read correctly from SRAM, the GREEN LED is ON, otherwise the RED LED is ON.   
- In case of HAL initialization issue, the RED LED will toggle. 

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Memory, FMC, SRAM, Read, Write, Initialization, Access, TrustZone disabled

### <b>Directory contents</b>

  - FMC/FMC_SRAM/Src/main.c                  Main program
  - FMC/FMC_SRAM/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - FMC/FMC_SRAM/Src/stm32u5xx_it.c          Interrupt handlers
  - FMC/FMC_SRAM/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - FMC/FMC_SRAM/Inc/main.h                  Main program header file
  - FMC/FMC_SRAM/Inc/stm32u5xx_eval_conf.h   BSP Configuration file
  - FMC/FMC_SRAM/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - FMC/FMC_SRAM/Inc/stm32u5xx_it.h          Interrupt handlers header file
        
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

