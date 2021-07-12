## <b>TSC_BasicAcquisition Example Description</b>

Use of the TSC to perform continuous acquisitions of one channel in polling mode. 

- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

- The project configures the maximum system clock frequency at 160Mhz.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

- The TSC peripheral configuration is ensured by the HAL_TSC_Init() function.
This later is calling the HAL_TSC_MspInit()function which core is implementing
the configuration of the needed TSC resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change TSC configuration.

- First we initialize the hardware (LCD and leds) SystemHardwareInit(). 

- Push the USER button to start.

- The TSC acquisition is then initiated by TscInit().
We need to acquire and store the first value to take it as a reference. This is done within TscCalibration().

- The TSC acquisition is now ready.
When the end of acquisition occurs, the acquired data are stored in the uhTSCAcquisitionValue variable.
Depending on the value of uhTSCAcquisitionValue compared to TSCUntouchedValue in relation to the max threshold value,
the corresponding LEDs are set ON.
The max threshold value can be modified in top of main.c, due to disparity link to board layout quality.

  - When TS1 touch detected LED GREEN turns on and a Green color is indicated on the screen.
  - When idle (no touch detected) LED RED turns on and a BLUE color is indicated on the screen.

#### <b>Notes</b>

 1. LED RED will blink in case of error.

 2. TSCx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

 3. TSCx_TS1_MAXTHRESHOLD can be modified if expected result is not satisfied.
This threshold value is linked to the board layout quality.

 4. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 5. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Directory contents</b>

  - TSC/TSC_BasicAcquisition/Src/main.c                 Main program
  - TSC/TSC_BasicAcquisition/Src/system_stm32u5xx.c     STM32U5xx system clock configuration file
  - TSC/TSC_BasicAcquisition/Src/stm32u5xx_hal_msp.c    HAL MSP module
  - TSC/TSC_BasicAcquisition/Src/stm32u5xx_it.c         Interrupt handlers
  - TSC/TSC_BasicAcquisition/Inc/main.h                 Main program header file
  - TSC/TSC_BasicAcquisition/Inc/stm32u5xx_hal_conf.h   HAL Configuration file
  - TSC/TSC_BasicAcquisition/Inc/stm32u575i_eval_conf.h Board Configuration file
  - TSC/TSC_BasicAcquisition/Inc/stm32u5xx_it.h         Interrupt handlers header file
  - TSC/TSC_BasicAcquisition/Inc/stlogo.h               Image used for BSP LCD example

### <b>Keywords</b>

TSC, Touch sensing controller, Acquisition, Basic Acquisition, Polling 
     
### <b>Hardware and Software environment</b>  

  - This template runs on STM32U575xx devices without security enabled (TZEN=0).

  - This template has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

