## <b>LPBAM_DAC_OPAMP_ContinuousConversion Application Description</b>
-   How to handle DAC continuous conversion and OPAMP switching configuration using
DMA linked-list feature in low power mode through LPBAM utility.

-   The application permits to generate a triangle signal by the DAC peripheral, then amplified by the OPAMP with several
gains.

-   The configuration of DAC is done via HAL and the conversion start is done via ADV_LPBAM_DAC_Conversion_SetDataQ() function.
The changing of OPAMP gain is done via ADV_LPBAM_OPAMP_Start_SetFullQ() function.

-   The DAC signal output is connected internally to the OPAMP input in order to amplify the DAC signal with programmed
gains. The DAC signal generation is triggered by the LPTIM PWM signal.
When the application starts DMA peripheral, the whole system goes to Stop2 mode.
After amplifying the signal period, The OPAMP gain is modified and the next DAC conversion is started. This sequence is
repeated in an infinite loop.

-   This project contains two configuration :
    -   Debug configuration : uncomment DEBUG_CONFIGURATION flag in the main.h.
    -   Power measurement configuration : comment DEBUG_CONFIGURATION flag in the main.h.

-   The typical average power consumption of the system performing DAC conversion with 1ms period and signal
amplification is 70uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED3 (red led) is turned on when any error is occurred.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, Analog, DMA, DAC, OPAMP, LPTIM, LPBAM, Voltage, Signal, Triangular-wave generation, Trigger, Low power, Stop2


### <b>Directory contents</b>

-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/main.c                  Main program
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_ContinuousConversion/Inc/TriangularSignal.h      Triangle signal data file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PA3(pin 1 CN9) to an oscilloscope (OPAMP output).
    -   Connect the pin PA4(pin 9 CN7) to an oscilloscope (DAC output).

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

