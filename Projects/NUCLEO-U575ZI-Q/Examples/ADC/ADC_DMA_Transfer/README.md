## <b>ADC_DMA_Transfer Example Description</b>

How to configure and use the ADC2 to convert an external analog input and get the result using a DMA transfer through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :

       - The CPU at 160 MHz.

The example is configured to continuously convert ADC2 channel ADC_CHANNEL_4, resolution is set to 12 bits right aligned, conversion is software-triggered.

GPDMA1_Channel0 is configured in Circular mode to transfer continuously the content of ADC2_DR (Data Register) to "aADCxConvertedData" variable which
stores the conversion result.

User may watch variable array "aADCxConvertedData" and check that its values are modified according to the voltage applied to pin PC_3 (pin 37 in connector CN11).

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, ADC, ADC channel, DMA transfer

### <b>Directory contents</b>

  - ADC/ADC_DMA_Transfer/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DMA_Transfer/Inc/stm32u5xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_DMA_Transfer/Inc/main.h                  Header for main.c module
  - ADC/ADC_DMA_Transfer/Src/stm32u5xx_it.c          HAL interrupt handlers
  - ADC/ADC_DMA_Transfer/Src/main.c                  Main program
  - ADC/ADC_DMA_Transfer/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DMA_Transfer/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx/585xx devices.

  - This example has been tested with STM32U575xx_NUCLEO (MB1549) board and can be
    easily tailored to any other supported device and development board.

  - STM32U575xx_NUCLEO (MB1549) Set-up
    Use an external power supply, adjust supply voltage and connect it to pins PC3 (pin 37 of connector CN11).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

