## <b>ADC_SingleConversion_TriggerTimer_DMA_Init Example Description</b>

How to use an ADC peripheral to perform a single ADC conversion on a channel
at each trigger event from a timer. Converted data is transferred by DMA
into a table in RAM memory.
This example is based on the STM32U5xx ADC LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:

ADC is configured to convert a single channel, in single conversion mode,
from HW trigger: timer peripheral (timer instance: TIM2).
timer is configured to provide a time base of 1kHz.
DMA is configured to transfer conversion data in an array, in circular mode.
A timer is configured in time base and to generate TRGO events.

Example execution:
From the main program execution, the ADC group regular converts the
selected channel at each trig from timer. DMA transfers conversion data to the array,
indefinitely (DMA in circular mode).

When conversion are completed,interruption occurs and data is processed in callback
functions (for this example purpose, convert digital data to physical value in mV).

LED1 is used to monitor program execution status:

- Normal operation: ADC group regular buffer activity (buffer complete events):
  LED toggle at frequency of time base / buffer size.
- Error: LED remaining turned on

Debug: variables to monitor with debugger:

- "uhADCxConvertedData": ADC group regular buffer conversion data (resolution 12 bits)
- "uhADCxConvertedData_Voltage_mVolt": ADC group regular buffer conversion data computed to voltage value (unit: mV)

Connection needed:
Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).

**Note:** If no power supply available, this pin can be let floating (in this case
ADC conversion data will be undetermined).

Other peripherals used:

 - 1 GPIO for LED1
 - 1 GPIO for analog input: ADC channel 9 on pin PA.04 (Arduino connector CN7 pin 9 D20, Morpho connector CN11 pin 32)
 - 1 DMA channel
 - 1 timer instance

### <b>Keywords</b>

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, regular, injected, DMA transfer, trigger

### <b>Directory contents</b>

    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/main.h                  Header for main.c module
    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Inc/stm32_assert.h          Template file to include assert_failed function
    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/stm32u5xx_it.c          Interrupt handlers
    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/main.c                  Main program
    - ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/system_stm32u5xx.c      STM32U5xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example