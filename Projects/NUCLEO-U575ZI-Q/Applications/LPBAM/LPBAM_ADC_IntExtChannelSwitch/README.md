## <b>LPBAM_ADC_IntExtChannelSwitch Application Description</b>
-   How to handle ADC switch between internal and external channel configurations then convert data thanks to DMA
linked-list feature in low power mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   LPBAM_ADC_IntExtChannelSwitch application contains :
    -   One LPBAM sub-application named **Conv**.
-   Conv application contains :
    -   One LPBAM scenario named **AnalogWDG**.

-   The LPBAM Conv application configures the system to reach the lowest power consumption.

-   The LPBAM AnalogWDG scenario configures the used peripherals and builds the scenario linked-list.

-   The AnalogWDG scenario performs two conversion sequences :
    -   The sequence 1 configures the ADC peripheral using HAL as follow :
        -   Channel               : Internal (VREFINT).
        -   ScanConvMode          : Scan Direction is Forward.
        -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
        -   Trigger low frequency : Disabled.
        -   ContinuousConvMode    : Disabled.
        -   DiscontinuousConvMode : Disabled.
        -   Analog watchdog       : Disabled.
        -   DMAContinuousRequests : Disabled.

    -   Then the ADC converts 32 data samples that are transferred by DMA to SRAM (aADCConvDataSeq1 buffer).
    Only conversion start and data storage is handled by LPBAM.

    -   The sequence 2 configures the ADC peripheral using LPBAM as follow :
        -   Channel               : External (Channel 4).
        -   ScanConvMode          : Scan Direction is Forward.
        -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
        -   Trigger low frequency : Disabled.
        -   ContinuousConvMode    : Disabled.
        -   DiscontinuousConvMode : Disabled.
        -   Analog watchdog       : AWD1 is enabled for single channel (Channel 4).
        -   DMAContinuousRequests : Enabled (Circular Mode).

    -   Then the ADC converts data continuously that are transferred by DMA to SRAM (aADCConvDataSeq2 buffer).

    -   Analog watchdog 1 is configured to monitor all channels on ADC group regular (therefore, including the selected channel).
    Analog watchdog thresholds values are:
        -   low threshold is set to 0V, refer to literal LPBAM_ADC_AWD_THRESHOLD_LOW.
        -   high threshold is set to Vref+ / 2 (3.3V / 2 = 1.65V on NUCLEO-U575ZI-Q (MB1549)), refer to literal LPBAM_ADC_AWD_THRESHOLD_HIGH.
    -   The ADC analog watchdog 1 interruption is enabled to wake up the system.

-   LPTIM1 PWM trigger signal period is equal to 2ms.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   Use an external power supply to generate a voltage in range [0V : 3.3V] and connect it to analog input pin (PC.3).
When conversion data is out of analog watchdog window, ADC interruption occurs and the system wakes up from STOP2 mode.

-   When measuring the application power consumption while the system in STOP2 mode, ensure that the voltage applied in the analog input pin 
(PC.3) is below the high threshold (1.65V). 

-   After the wake up from STOP2 mode, the LPBAM sub-system is still operating and the blue led turns on.

-   User push-button (pin PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When it
is pressed, the LPBAM scenario is stopped, unlinked and de-initialized and the green led turns on.

-   The typical average power consumption of the system performing ADC conversion and voltage monitoring with a 2 ms
period is 8.3uA.

-   NUCLEO-U575ZI-Q (MB1549) board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
    -   LED2 (blue led) is turned on after wake up from Stop2 mode and before pressing the user push-button.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application can not be used in DEBUG mode due to the fact that the debug pins are configured in analog mode for power
      consumption and Cortex-M33 core is no longer clocked during low power mode so debugging features are disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx tool, 
      ensure to re-adjust linker ROM and RAM sections addresses.

### <b>Keywords</b>

Analog, ADC, DMA, LPTIM, LPBAM, Voltage, Signal, Trigger, Low power, Stop2, Buffer update,

### <b>Directory contents</b>

-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Src/main.c                          Main program file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Src/system_stm32u5xx.c              STM32U5xx system clock configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Src/stm32u5xx_it.c                  Interrupt handlers file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Src/stm32u5xx_hal_msp.c             HAL MSP module file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Inc/stm32_lpbam_conf.h              LPBAM configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Inc/main.h                          Main program header file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Inc/stm32u5xx_nucleo_conf.h         BSP configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Inc/stm32u5xx_hal_conf.h            HAL configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/Core/Inc/stm32u5xx_it.h                  Interrupt handlers header file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/LPBAM/Conv/lpbam_conv.h                  LPBAM Conv application header file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/LPBAM/Conv/lpbam_conv_config.c           LPBAM Conv application configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/LPBAM/Conv/lpbam_conv_analogwdg_config.c LPBAM Conv application AnalogWDG scenario configuration file
-   LPBAM/LPBAM_ADC_IntExtChannelSwitch/LPBAM/Conv/lpbam_conv_analogwdg_build.c  LPBAM Conv application AnalogWDG scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q (MB1549) Set-up
    -   Connect PC.3 (Pin 37 in CN11 connector) to an external signal.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.