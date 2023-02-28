## <b>CRC_PolynomialUpdate Example Description</b>

How to use the CRC peripheral through the STM32U5xx CRC HAL and LL API.
The LL API is used for performance improvement. The CRC calculation unit
computes the 8-bit CRC code of a given buffer of 32-bit data words, based on
a user-defined generating polynomial. In this example, the polynomial is first
set manually to 0x9B (X^8 + X^7 + X^4 + X^3 + X + 1).
In a second step, the generating polynomial value and length are updated and
set to 0x1021  (X^16 + X^12 + X^5 + 1) for a new CRC calculation.
These updates are performed using the CRC LL API.  
At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).  
The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK).
You can update HAL_CRC_Init() input parameters to change the CRC configuration.  
For each computation, calculated CRC code is stored in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue1 and uwExpectedCRCValue2).  
STM32 board LEDs are used to monitor the example status:  
  - LED1 is ON when correct CRC values are calculated  
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Security, CRC, CRC Polynomial, CRC-8-WCDMA, CRC-16-ANSI, hardware CRC, user-defined generating polynomial

### <b>Directory contents</b>

  - CRC/CRC_PolynomialUpdate/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - CRC/CRC_PolynomialUpdate/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - CRC/CRC_PolynomialUpdate/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - CRC/CRC_PolynomialUpdate/Inc/main.h                  Header for main.c module
  - CRC/CRC_PolynomialUpdate/Src/stm32u5xx_it.c          Interrupt handlers
  - CRC/CRC_PolynomialUpdate/Src/main.c                  Main program
  - CRC/CRC_PolynomialUpdate/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - CRC/CRC_PolynomialUpdate/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.
  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

