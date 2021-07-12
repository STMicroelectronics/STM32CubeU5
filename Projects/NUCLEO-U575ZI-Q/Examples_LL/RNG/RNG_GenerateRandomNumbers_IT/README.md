## <b>RNG_GenerateRandomNumbers_IT Example Description</b>

Configuration of the RNG to generate 32-bit long random numbers using interrupts. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

Example execution:

After startup from reset and system configuration, RNG configuration is performed.
HSI48 and VREFINT are enabled to generate 48MHz domain clock
and HSI48 is selected as RNG clock source.

User is then asked to press User push-button (LED1 blinking fast).
On User push-button press, several (8) Random 32bit numbers are generated
(On each raised RNG interrupt, a random number is generated and retrieved from DR register).
Corresponding generated values are available and stored in a u32 array (aRandom32bit),
whose content could be displayed using debugger (Watch or LiveWatch features).
After successful Random numbers generation, LED1 is turned On.
In case of errors, LED1 is slowly blinking (1sec period).


### <b>Keywords</b>

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period, Interrupt


### <b>Directory contents</b>

  - RNG/RNG_GenerateRandomNumbers_IT/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - RNG/RNG_GenerateRandomNumbers_IT/Inc/main.h                  Header for main.c module
  - RNG/RNG_GenerateRandomNumbers_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - RNG/RNG_GenerateRandomNumbers_IT/Src/stm32u5xx_it.c          Interrupt handlers
  - RNG/RNG_GenerateRandomNumbers_IT/Src/main.c                  Main program
  - RNG/RNG_GenerateRandomNumbers_IT/Src/system_stm32u5xx.c      STM32U5xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Push User push-button and use Variable watch window from debugger to access to values of generated numbers.
   (A break point could be set on LED_On() call, at end of RandomNumbersGeneration_IT() function).

