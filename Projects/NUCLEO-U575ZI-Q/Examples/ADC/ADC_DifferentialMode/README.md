## <b>ADC_DifferentialMode Example Description</b>

This example describes how to configure and use the ADC1 to convert an external analog input in Differential Mode, difference between external voltage on VinN and VinP.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

The ADC1 is configured to convert continuously ADC_CHANNEL_1 (VinP) connected to PC.00 (PIN 38 CN11) and its correspanding VinN connected to PC.01 (PIN 36 CN11).

User can connect ADC_CHANNEL_1 pins (PC.00, PC.01) to external power supply and vary voltage.

uhADCxConvertedDat variable contains conversion data (voltage difference VinP - VinN)  and uhADCxConvertedData_Voltage_mVolt yields the signed input voltage in mV.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, ADC, ADC channel, Polling

### <b>Directory contents</b>

  - ADC/ADC_DifferentialMode/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DifferentialMode/Inc/stm32u5xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_DifferentialMode/Inc/main.h                  Header for main.c module
  - ADC/ADC_DifferentialMode/Src/stm32u5xx_it.c          HAL interrupt handlers
  - ADC/ADC_DifferentialMode/Src/main.c                  Main program
  - ADC/ADC_DifferentialMode/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DifferentialMode/Src/system_stm32u5xx.c      STM32u5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx/585xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q (MB1549) board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U575ZI-Q (MB1549) Set-up
    Use an external power supply, adjust supply voltage and connect it to pins PC0 and PC1 (connector CN11).

  - To monitor the conversion result, put the "uhADCxConvertedData" (or it's voltage representation "uhADCxConvertedData_Voltage_mVolt") variable in the debugger live watch.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

