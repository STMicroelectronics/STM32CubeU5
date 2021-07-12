## <b>DCMI_ContinousCap_EmbeddedSynchMode Example Description</b>

- This example provides a description of how to configure DCMI peripheral in Continuous Mode 
and EmbeddedSynchronization mode with suspend and resume of the frame capture based on 
the STM32Cube HAL API when security is not enabled (TZEN=0).

- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

- The project configures the maximum system clock frequency at 160Mhz.

- This example describe how to configure the camera in continuous mode in QVGA resolution, with synchronization mode
set to Embedded Synchronization and to suspend/Resume the capture.

- Embedded Synchronization Mode : The Synchronization of the frame is controlled by embedded codes set by the user and enabled by their 
dedicated Unmask values, these codes should be set in the same way to the camera module which should 
be able to support this mode.

- First push the user button to start the frame acquisation that will be displayed on the lcd
Then the user button is used to freeze(Suspend) and Unfreeze(Resume) the capture
The leds of the STM32U575I-EV board are used to monitor the status as following:  
    - The GREEN led toggles for each new frame capture.  
    - The GREEN led is turned OFF when the capture is freezed.  
    - The RED led is ON when an error occurs.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

DCMI, Embedded Sunchronization Mode, Continuous Mode , Suspend, Resume, TrustZone disabled

### <b>Directory contents</b> 

  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Src/main.c                  Main program
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Src/stm32u5xx_it.c          Interrupt handlers
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Inc/main.h                  Main program header file
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - DCMI/DCMI_ContinousCap_EmbeddedSynchMode/Inc/stm32u5xx_it.h          Interrupt handlers header file

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

