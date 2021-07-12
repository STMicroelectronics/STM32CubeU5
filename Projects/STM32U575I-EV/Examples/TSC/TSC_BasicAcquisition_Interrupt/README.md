## <b>TSC_BasicAcquisition_Interrupt Example Description</b>

Use of the TSC to perform continuous acquisitions of one channel in Interrupt mode.

This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 160 MHz.

The TSC peripheral configuration is ensured by the HAL_TSC_Init() function.
This later is calling the HAL_TSC_MspInit()function which core is implementing
the configuration of the needed TSC resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change TSC configuration.

First we initialize the hardware (LCD and leds). This step is assured by SystemHardwareInit().

Push the USER button to start the example.

The TSC acquisition is then initiated by TscInit().
When the end of acquisition occurs, the acquired data are affected to the TSCAcquisitionValue variable.
Depending of value of TSCAcquisitionValue under the max threshold values, the corresponding LEDs are set ON.
These max threshold values can be modified in top of main.c, due to disparity link to board layout quality

The LEDs are used as follow:

  - LED GREEN: When TS1 touch detected.
  - LED RED: When idle (no touch detected).
  - LED RED: toggles when an error occurs.

#### <b>Notes</b>

 1. TSCx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

 2. TSCx_TS1_MAXTHRESHOLD can be modified if expected result is not satisfied.
This threshold value is linked to the board layout quality.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Directory contents</b>

  - TSC/TSC_BasicAcquisition_Interrupt/Src/main.c                 Main program
  - TSC/TSC_BasicAcquisition_Interrupt/Src/system_stm32u5xx.c     STM32U5xx system clock configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32u5xx_hal_msp.c    HAL MSP module
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32u5xx_it.c         Interrupt handlers
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/main.h                 Main program header file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u5xx_hal_conf.h   HAL Configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u575i_eval_conf.h Board Configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u5xx_it.h         Interrupt handlers header file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stlogo.h               Image used for BSP LCD example

### <b>Keywords</b>

TSC, Touch sensing controller, Acquisition, Basic Acquisition Interrupt, Interrupt

### Hardware and Software environment

  - This template runs on STM32U575xx devices without security enabled (TZEN=0).

  - This template has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

