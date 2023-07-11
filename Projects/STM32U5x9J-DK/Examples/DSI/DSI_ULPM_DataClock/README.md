## <b>DSI_ULPM_DataClock Example Description</b>


-   This example provides a description of how to use the embedded LCD DSI controller (using IPs LTDC and DSI Host) to drive the **J025FlCN0201W (480x480)**   
    display mounted on MB1892 board and manage entry and exit in DSI ULPM mode on data lane and clock lane.  
 \

	<pre>
    In this mode, the DSI PHY state machine is entering a low power state on data lane and clock lane.  
    The DSI clock should be derived from the clock tree PLL3.PLLP (ck_plldsi) with a 62.5 MHz clock for instance  
    to allow a switch-off of the PLL DPHY to save power during the ULPM phase.  
    When exiting from ULPM, the PLL DPHY is first switched back on and locked, then the DSI DPHY will exit  
    power down and display again as before.</pre>
 \

-   In this example a picture (320x240) is displayed on LCD in **DSI_ULPM_DataClock**.

    - At the beginning of the main program, the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.    
      The SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 Mhz.
\

    - The project calls also CACHE_Enable() function in order to enable the Instruction Caches.   
      This function is provided in order to enhance the performance.
\

    - On a **User button press** from the user, the LCD display will be switched off. two seconds later, the DSI PHY Data and clock lane will enter ULPM mode.   
      DphyClk is derived from PLL3.PLLP, then DPHY PLL is switched Off.

    - After 6 seconds in Off mode, The PLL DPHY is switched back on and locked, then the ULPM on data and clock lanes will be exited in DPHY DSI state machine.  
      The LCD will then be switched back on and display the same image as before with a <span style="color: green"> text notifying that the exit from ULPM was successful </span>.  
\


    The image_320x240_argb8888 is of format ARGB8888 and is initially copied from Flash to LCD Frame Buffer (GFXMMU_VIRTUAL_BUFFER0_BASE).  
    The LTDC is reading continuously the LCD Frame buffer from LCD Frame Buffer, sent it to DSI Host which sends it in burst mode (DCS commands)  
    via the DSI DPHY to the the display that decodes DSI packets and refresh its internal Graphic RAM to display.
\

    The Frame number display is managed by Line Event callback in which the Frame number is incremented each time a line event occurs.  
	When entering to ULPM, The Frame number is unchanged until the exit from this mode.
\

\

*<span style="color: blue"> Discovery-U5x9NJ's LEDs can be used to monitor the status:</span>*	

**<span style="color: green"> GREEN LED </span> ON **: DSI PHY Data and clock lane in ULPM mode   
**<span style="color: green"> GREEN LED </span> OFF**: DSI PHY Data and clock lane in run mode  
**<span style="color: red"> RED LED </span> ON **: an error occurred
\

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Graphics, Display, LCD, LTDC, DMA2D, DSI, MIPI Alliance, Ultra low power mode, Mode Video Burst, WVGA, ARGB8888, SRAM, Data lane, clock lane

### <b>Directory contents</b>

-   DSI/DSI_ULPM_DataClock/Inc/stm32u5xx_hal_conf.h&emsp; &emsp; &emsp; &nbsp; HAL configuration file for Cortex-M33
-   DSI/DSI_ULPM_DataClock/Inc/stm32u5xx_it.h&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Interrupt handlers header file for Cortex-M33
-   DSI/DSI_ULPM_DataClock/Inc/main.h&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Header for main.c module for Cortex-M33
-   DSI/DSI_ULPM_DataClock/Inc/image_320x240_argb8888.h&emsp; &nbsp; Image 320x240 in ARGB8888 to display on LCD
-   DSI/DSI_ULPM_DataClock/Inc/gfxmmu_lut.h&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &nbsp; &nbsp; GFXMMU look-up-table for DSI round LCD
-   DSI/DSI_ULPM_DataClock/Inc/stm32u5x9j_discovery_conf.h&emsp; Board Configuration file
-   DSI/DSI_ULPM_DataClock/Src/system_stm32u5xx.c&emsp; &emsp; &emsp; &emsp; STM32U5xx system configuration source file
-   DSI/DSI_ULPM_DataClock/Src/stm32u5xx_it.c&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Interrupt handlers for Cortex-M33
-   DSI/DSI_ULPM_DataClock/Src/main.c&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Main program for Cortex-M33
-   DSI/DSI_ULPM_DataClock/Src/stm32u5xx_hal_msp.c&emsp; &emsp; &emsp; &nbsp; HAL MSP file for Cortex-M33

### <b>Hardware and Software environment</b>

-   This example runs on STM32U5A9xx devices without security enabled (TZEN=0).
-   This example runs on STM32U5G9xx devices without security enabled (TZEN=0).

-   This example has been tested with STMicroelectronics STM32U5x9J-DK (MB1829)  
    board and can be easily tailored to any other supported device  
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    - Open your preferred toolchain
    - Rebuild all files and load your image into target memory
    - Run the example

