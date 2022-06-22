## <b>LPBAM_ADC_TempSense Application Description</b>
-   How to handle ADC temperature sensor monitoring switch between internal and external channel configurations then convert data thanks to DMA
linked-list feature in low power mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   LPBAM_ADC_TempSense application contains :
    -   One LPBAM sub-application named **TempSens**.
-   TempSens application contains :
    -   One LPBAM scenario named **MultiThres**.
-   MultiThres scenario contains :
    -   Three Queues named **Threshold1_Conversion**,**Threshold2_Conversion** and **Thresholdx_Config**.

-   The LPBAM TempSens application configures the system to reach the lowest power consumption.

-   The LPBAM MultiThres scenario configures the used peripherals and builds the scenario linked-list.

-   LPTIM1 PWM trigger signal period is equal to 100ms.

-   The MultiThres scenario performs two conversion sequences one in Threshold1_Conversion queue and the second one in
Threshold2_Conversion queue. These two queues are started by the Thresholdx_Config ADV_LPBAM_DMA_Start_SetFullQ() functions.

    -   The sequence 1 configures the ADC peripheral using HAL as follow :
        -   Channel               : Internal (Tempearture Sensor Channel).
        -   ScanConvMode          : Scan Direction is Forward.
        -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
        -   Trigger low frequency : Disabled.
        -   ContinuousConvMode    : Disabled.
        -   DiscontinuousConvMode : Disabled.
        -   Analog watchdog       : AWD1 is enabled for single channel (Tempearture Sensor Channel).
        -   DMAContinuousRequests : Enabled (Circular Mode).
        -   Low threshold is set to 0V.
        -   High threshold is set to 0.809V. ((1005 / 4095) * 3.3V)

    -   In Threshold1_Conversion queue which is started by Thresholdx_Config queue the ADC converts data continuously
    that are transferred by DMA to SRAM (Threshold1_Data_Buffer buffer) until threshold1 is detected.
    Only conversion start and data storage are handled by LPBAM.

    -   After Threshold1 is detected the LPTIM1 PWM trigger signal period is changed to 10ms by the Thresholdx_Config
    queue and the ADC is reconfigured using the LPBAM as follows:
        -   Channel               : Internal (Tempearture Sensor Channel).
        -   ScanConvMode          : Scan Direction is Forward.
        -   Signal trigger        : LPTIM1 Channel1 PWM (Rising Edge).
        -   Trigger low frequency : Disabled.
        -   ContinuousConvMode    : Disabled.
        -   DiscontinuousConvMode : Disabled.
        -   Analog watchdog       : AWD1 is enabled for single channel (Tempearture Sensor Channel).
        -   DMAContinuousRequests : Enabled (Circular Mode).
        -   low threshold is set to 0V.
        -   High threshold is set to 0.886V. ((1100 / 4095) * 3.3V)
        -   The ADC analog watchdog 1 interruption is enabled to wake up the system.

    -    Then the Thresholdx_Config queue starts the Threshold2_Conversion queue, the ADC converts data continuously
    that are transferred by DMA to SRAM (Threshold2_Data_Buffer buffer) until the threshold2 is detected.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   When the temperature increases and the conversion data becomes higher than the threshold2, ADC interruption occurs
and the system wakes up from STOP2 mode.

-   After the wake up from STOP2 the LPBAM scenario is stopped, unlinked and de-initialized and the green led turns on.

-   The typical average power consumption 
    -  during Threshold1 monitoring is 4uA.   
    -  during Threshold2 monitoring is 7.8uA.
    -  during system in STOP2 monitoring is 5.2uA.

-   NUCLEO-U575ZI-Q (MB1549) board LEDs are used to monitor the application status:
    -   LED1 (green led) is turned on at the end of the application and no error is detected.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application can not be used in DEBUG mode due to the fact that the debug pins are configured in analog mode for
      power consumption and Cortex-M33 core is no longer clocked during low power mode so debugging features are
      disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx
      tool, ensure to re-adjust linker ROM and RAM sections addresses.

5.  Please note that the values of Threshold1 and Threshold2 have to be adapted according to the temperature condition. 

### <b>Keywords</b>

Analog, ADC, DMA, LPTIM, LPBAM, Voltage, Signal, Trigger, Low power, Stop2, Buffer update, Temperature sensor

### <b>Directory contents</b>

-   LPBAM/LPBAM_ADC_TempSense/Core/Src/main.c                                   Main program file
-   LPBAM/LPBAM_ADC_TempSense/Core/Src/system_stm32u5xx.c                       STM32U5xx system clock configuration file
-   LPBAM/LPBAM_ADC_TempSense/Core/Src/stm32u5xx_it.c                           Interrupt handlers file
-   LPBAM/LPBAM_ADC_TempSense/Core/Src/stm32u5xx_hal_msp.c                      HAL MSP module file
-   LPBAM/LPBAM_ADC_TempSense/Core/Inc/stm32_lpbam_conf.h                       LPBAM configuration file
-   LPBAM/LPBAM_ADC_TempSense/Core/Inc/main.h                                   Main program header file
-   LPBAM/LPBAM_ADC_TempSense/Core/Inc/stm32u5xx_nucleo_conf.h                  BSP configuration file
-   LPBAM/LPBAM_ADC_TempSense/Core/Inc/stm32u5xx_hal_conf.h                     HAL configuration file
-   LPBAM/LPBAM_ADC_TempSense/Core/Inc/stm32u5xx_it.h                           Interrupt handlers header file
-   LPBAM/LPBAM_ADC_TempSense/LPBAM/TempSens/lpbam_tempsens.h                   LPBAM TempSens application header file
-   LPBAM/LPBAM_ADC_TempSense/LPBAM/TempSens/lpbam_tempsens_config.c            LPBAM TempSens application configuration file
-   LPBAM/LPBAM_ADC_TempSense/LPBAM/TempSens/lpbam_tempsens_multithres_config.c LPBAM TempSens application MultiThres scenario configuration file
-   LPBAM/LPBAM_ADC_TempSense/LPBAM/TempSens/lpbam_tempsens_multithres_build.c  LPBAM TempSens application MultiThres scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.
    -   Heat the device to reach thresholds (Described above).

