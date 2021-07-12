## <b>RNG_MultiRNG Example Description</b>

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

The RNG peripheral configuration is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK, GPIO, DMA and NVIC).
You may update this function to change RNG configuration.

After startup, user is asked to press User push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers at each User push-button press.

The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED3 is toggling at a frequency of 1Hz.

### <b>Keywords</b>

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

### <b>Directory contents</b>

  - RNG/RNG_MultiRNG/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - RNG/RNG_MultiRNG/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - RNG/RNG_MultiRNG/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - RNG/RNG_MultiRNG/Inc/main.h                  Header for main.c module
  - RNG/RNG_MultiRNG/Src/stm32u5xx_it.c          Interrupt handlers
  - RNG/RNG_MultiRNG/Src/main.c                  Main program
  - RNG/RNG_MultiRNG/Src/stm32u5xx_hal_msp.c     HAL MSP module 
  - RNG/RNG_MultiRNG/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example