## <b>RCC_UseHSI_PLLasSystemClock Example Description</b>

Modification of the PLL parameters in run time.

In this example, the toggling frequency of the green **LED1** depends on the system clock
frequency and, each time the User push-button is pressed, the PLL switches between two configurations.

This make the **LED1 blinking** speed to highlight the system clock frequency changes.

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with MSI as clock source.
Then, when pressing User push-button, an automatic switch is done between PLL with HSI as clock source (SYSCLK
set to 40MHz) and PLL with HSI as clock source (SYSCLK set to 160MHz).
LED1 will toggle differently between the 2 configurations (quick toggle with SYSCLK configuration at 160MHz).

To detect a problem with PLL configuration, switch USE_TIMEOUT can be enabled. 

Then in case of issues,**LED1** will toggle every 1 second.

**Note**: **"uwFrequency"** variable can be added in LiveWatch to monitor the system clock frequency.

### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSCLK, HSE, Clock, Oscillator, HSI

### <b>Directory contents</b>

  - RCC/RCC_UseHSI_PLLasSystemClock/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - RCC/RCC_UseHSI_PLLasSystemClock/Inc/main.h                  Header for main.c module
  - RCC/RCC_UseHSI_PLLasSystemClock/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_UseHSI_PLLasSystemClock/Src/stm32u5xx_it.c          Interrupt handlers
  - RCC/RCC_UseHSI_PLLasSystemClock/Src/main.c                  Main program
  - RCC/RCC_UseHSI_PLLasSystemClock/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

