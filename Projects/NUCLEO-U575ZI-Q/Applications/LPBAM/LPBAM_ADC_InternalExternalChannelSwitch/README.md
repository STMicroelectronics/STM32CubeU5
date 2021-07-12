## <b>LPBAM_ADC_InternalExternalChannelSwitch Application Description</b>
-   How to handle ADC switch between internal and external channel configurations then convert data thanks to DMA
linked-list feature in low power mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   The ADC peripheral configuration is ensured by HAL_ADC_Init() and HAL_ADC_ConfigChannel() functions.
    The HAL_ADC_MspInit() function allows the user to configure the needed ADC channels and setup the related GPIO.

-   The application performs two conversion sequences :

-   The sequence 1 configures the ADC peripheral using HAL as follow :
    -   Channel               : Internal (VREFINT).
    -   ScanConvMode          : Scan Direction is Forward.
    -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
    -   Trigger low frequency : Disabled.
    -   ContinuousConvMode    : Enabled.
    -   DiscontinuousConvMode : Disabled.
    -   Analog watchdog       : Disabled.
    -   DMAContinuousRequests : Disabled.

-   Then the ADC converts 32 data samples that are transferred by DMA to SRAM (aADCConvDataSeq1 buffer).
    Only conversion start and data storage is handled by LPBAM.

-   LPTIM3 is used to make a delay between ADC start and DMA samples transfer.
    LPTIM3 PWM signal period is equal to 1ms repeated 4 times so, the delay 4ms.

-   The sequence 2 configures the ADC peripheral using LPBAM as follow :
    -   Channel               : External (Channel 4).
    -   ScanConvMode          : Scan Direction is Forward.
    -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
    -   Trigger low frequency : Disabled.
    -   ContinuousConvMode    : Enabled.
    -   DiscontinuousConvMode : Disabled.
    -   Analog watchdog       : Disabled.
    -   DMAContinuousRequests : Enabled (Circular Mode).

-   Then the ADC converts data continuously that are transferred by DMA to SRAM (aADCConvDataSeq2 buffer).

-   LPTIM1 PWM trigger signal period is equal to 2ms.

-   To minimize the power consumption, after starting generating the PWM signals, the MCU enters in STOP 2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When it
is pressed, a wakeup event is generated and green led is turned on.

-   The operating LPBAM mechanism is not impacted when the system enters and exit STOP 2 mode.


-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on after wake up from STOP 2 mode and no error is detected.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Analog, ADC, DMA, LPTIM, LPBAM, Voltage, Signal, Trigger, Low power, Stop2, Buffer update,

### <b>Directory contents</b>

-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Src/main.c                  Main program
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   Connect PC.3 (Pin 37 in CN11 connector) to an external signal.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

