## <b>Templates_LL Example Description</b>

- This projects provides a reference template through the LL API that can be used to build any firmware application.

- This project is targeted to run on STM32U5A9xx device on **STM32U5x9J-DK** board from STMicroelectronics.

- The reference template project configures the maximum system clock frequency at 160Mhz.

This project LL template provides:

 - Inclusion of all LL drivers (include files in "main.h" and LL sources files
   in IDE environment, with option "USE_FULL_LL_DRIVER" in IDE environment)
   Note: If optimization is needed afterwards, user can perform a cleanup by
   removing unused drivers.
 - Clock configuration (file: main.c)
 
To port a LL example to the targeted board:

1. Select the LL example to port.
   To find the board on which LL examples are deployed, refer to LL examples list
   in "STM32CubeProjectsList.html", table section "Examples_LL"
   or Application Note: STM32Cube firmware examples for STM32U5 Series

2. Replace source files of the LL template by the ones of the LL example, except
   code specific to board.
   Note: Code specific to board is specified between tags:
   
   /* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
   
   /* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

   - Replace file main.c, with updates:
     - Keep clock configuration of the LL template: function "SystemClock_Config()"

   - Replace file stm32u5xx_it.h
   - Replace file stm32u5xx_it.c

### <b>Directory contents</b>

  - Templates_LL/TrustZoneDisabled/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - Templates_LL/TrustZoneDisabled/Inc/main.h                  Header for main.c module
  - Templates_LL/TrustZoneDisabled/Inc/stm32_assert.h          Template file to include assert_failed function
  - Templates_LL/TrustZoneDisabled/Src/stm32u5xx_it.c          Interrupt handlers
  - Templates_LL/TrustZoneDisabled/Src/main.c                  Main program
  - Templates_LL/TrustZoneDisabled/Src/system_stm32u5xx.c      STM32U5x system source file

### <b>Hardware and Software environment</b>

  - This template runs on STM32U5A9xx devices.

  - This template runs on STM32U5G9xx devices.

  - This template has been tested with STMicroelectronics STM32U5x9J-DK (MB1829B)
    boards and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

