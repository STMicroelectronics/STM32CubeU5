## <b>DAC_SignalsGeneration_DMA Example Description</b>

How to use the DAC peripheral to generate sine signal using the DMA controller.
At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :

        - The CPU at 160 MHz.
        - The HCLK for AHB peripherals to run at 160 MHz.
        - The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

- The TIM is configured in time base and to generate TRGO events as a source trigger for the DAC.
- At each trig from timer, the DAC send a request to the DMA, then the DMA responds this request by
  transferring data from sin_data buffer to the DAC DHR register in circular mode.
- When the user press on the user push-button, the DAC change the waveform signal:
   - Sine waveform on DAC_CHANNEL_1 using DMA3 transfer, amplitude: ~3V, frequency: order of KHz.
   - Escalator waveform on DAC_CHANNEL_1 using DMA3 transfer, amplitude: ~3V, frequency: order of KHz.

NUCLEO-U575ZI-Q board LEDs are used to monitor the process status:

- While board is generating sine signal, its LED1 is blinking rapidly (100 ms period).
- When the user press the User push-button, the waveform signal change and LED1 keeps on.
- If there is an initialization or transfer error, LED3 is turned on.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Software calibration, Continuous conversion, DMA, Escalator-generation, Sine-wave generation, Timer Trigger

### <b>Directory contents</b>

  - DAC/DAC_SignalsGeneration_DMA/Inc/stm32u5xx_hal_conf.h     HAL configuration file
  - DAC/DAC_SignalsGeneration_DMA/Inc/stm32u5xx_it.h           DMA interrupt handlers header file
  - DAC/DAC_SignalsGeneration_DMA/Inc/main.h                   Header for main.c module
  - DAC/DAC_SignalsGeneration_DMA/Inc/stm32u5xx_nucleo_conf.h  STM32U5xx_Nucleo board configuration file
  - DAC/DAC_SignalsGeneration_DMA/Src/stm32u5xx_it.c           DMA interrupt handlers
  - DAC/DAC_SignalsGeneration_DMA/Src/main.c                   Main program
  - DAC/DAC_SignalsGeneration_DMA/Src/stm32u5xx_hal_msp.c      HAL MSP module
  - DAC/DAC_SignalsGeneration_DMA/Src/system_stm32u5xx.c       STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN = 0).

  - This example has been tested with two NUCLEO-U575ZI-Q boards (MB1549) embedding
    a STM32U575xx device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q set-up
      - Connect PA4 (DAC Channel1)(pin 9 in CN7) to an oscilloscope.
      - Use User push-button connected to PC.13.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

- Open your preferred toolchain
  - Rebuild all files and load your image into target memory
- Run the example

