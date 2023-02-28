## <b>DAC_SimpleConversion Example Description</b>

How to use the DAC peripheral to do a simple conversion.

      - The example uses the DAC for a simple conversion in 8 bits right
        alignment of 0xFF value, the result of conversion can be seen by
        connecting PA4(DAC channel1) to an oscilloscope.
        The observed value is 3.3V.

NUCLEO-U575ZI-Q board's LEDs can be used to monitor the process status:

  - LED3 is ON and example is stopped (using infinite loop) when there is an error during process.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Single conversion, Sample and Hold

### <b>Directory contents</b>

  - DAC/DAC_SimpleConversion/Inc/stm32u5xx_nucleo_conf.h     BSP configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32u5xx_it.h          DMA interrupt handlers header file
  - DAC/DAC_Simple_Conversion/Inc/main.h                  Header for main.c module  
  - DAC/DAC_Simple_Conversion/Src/stm32u5xx_it.c          DMA interrupt handlers
  - DAC/DAC_Simple_Conversion/Src/main.c                  Main program
  - DAC/DAC_Simple_Conversion/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - DAC/DAC_Simple_Conversion/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U575ZI-Q Set-up

      - Connect PA4 (DAC Channel1) (pin 9 in CN7) to an oscilloscope.
     

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

