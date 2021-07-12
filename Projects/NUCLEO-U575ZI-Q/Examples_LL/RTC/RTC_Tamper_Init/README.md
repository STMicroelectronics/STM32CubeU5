## <b>RTC_Tamper_Init Example Description</b>

Configuration of the Tamper using the RTC LL API. The peripheral initialization
uses LL unitary service functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
MSI as clock source.

The RTC peripheral configuration is ensured by the MX_RTC_Init() function
(configure of the needed RTC resources according to the used hardware CLOCK,
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

The associated firmware performs the following:

1. It configures the Tamper pin to be rising edge, and enables the Tamper
   interrupt.
2. It writes the data to all RTC Backup data registers, then check whether the
   data were correctly written. 
   
   If yes, **LED1 toggles** with a fast period,
   otherwise **LED1 toggles** with a period of 1s.
3. Applying a low level on the BUTTON_USER_PIN (PC.13) by pressing User push-button,
   the RTC backup data registers are reset and the Tamper interrupt is generated.
   
   The corresponding ISR then checks whether the RTC Backup data registers are cleared.
   
   If yes **LED1** turns on, otherwise **LED1** toggles with a period of 1s.

**Note**  On this board, the tamper 1 pin is connected to User push-button (connection to pin PC.13).

### <b>Keywords</b>

System, RTC, Tamper, Reset, LSE, LSI

### <b>Directory contents</b>

  - RTC/RTC_Tamper_Init/Inc/stm32u5xx_it.h     Interrupt handlers header file
  - RTC/RTC_Tamper_Init/Inc/main.h             Header for main.c module
  - RTC/RTC_Tamper_Init/Inc/stm32_assert.h     Template file to include assert_failed function
  - RTC/RTC_Tamper_Init/Src/stm32u5xx_it.c     Interrupt handlers
  - RTC/RTC_Tamper_Init/Src/main.c             Main program
  - RTC/RTC_Tamper_Init/Src/system_stm32u5xx.c STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

