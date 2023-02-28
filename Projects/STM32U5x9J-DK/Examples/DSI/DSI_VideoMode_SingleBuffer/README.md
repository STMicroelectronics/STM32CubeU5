## <b>DSI_VideoMode_SingleBuffer Example Description</b>

-   This example provides a description of how to use the embedded LCD DSI controller
(using IPs LTDC and DSI Host) to drive the **J025FlCN0201W (480x480)**  display mounted on MB1892 board.  
\

  <pre>
    The goal of this example is to display a QVGA landscape (320x240) images on LCD in continuous mode  
	(video burst) using a single buffer for display and for draw.</pre>
\

-   In this example Layer0 is initialized to display a brief description of the example on the LCD screen.It will be used for images display also.
\
	
	-   At the beginning of the main program, the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.  
        The SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 Mhz.
\

    -   The project calls also CACHE_Enable() function in order to enable the Instruction Caches.   
	    This function is provided in order to enhance the performance.
\

    -   Each image is displayed on LCD in **DSI Mode Video Burst** for two seconds before the swap to the next image.
\

\

*<span style="color: blue"> Discovery-U5x9NJ's LEDs can be used to monitor the status:</span>*

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

Graphics, Display, LCD, DMA2D , DSI, MIPI Alliance, Video mode, Single buffer, LTDC, QVGA, ARGB8888, SRAM

### <b>Directory contents</b>

-   DSI/DSI_VideoMode_SingleBuffer/Inc/stm32u5xx_hal_conf.h&emsp; &emsp; &emsp; &nbsp; HAL configuration file for Cortex-M33
-   DSI/DSI_VideoMode_SingleBuffer/Inc/stm32u5xx_it.h&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Interrupt handlers header file for Cortex-M33
-   DSI/DSI_VideoMode_SingleBuffer/Inc/main.h&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Header for main.c module for Cortex-M33
-   DSI/DSI_VideoMode_SingleBuffer/Inc/life_augmented_argb8888.h&emsp; &nbsp; Image 320x240 in ARGB8888 to display on LCD
-   DSI/DSI_VideoMode_SingleBuffer/Inc/image_320x240_argb8888.h&emsp; &nbsp; Image 320x240 in ARGB8888 to display on LCD
-   DSI/DSI_VideoMode_SingleBuffer/Inc/stm32u5x9j_discovery_conf.h&emsp; Board Configuration file
-   DSI/DSI_VideoMode_SingleBuffer/Inc/gfxmmu_lut.h&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &nbsp; &nbsp; GFXMMU look-up-table for DSI round LCD
-   DSI/DSI_VideoMode_SingleBuffer/Src/system_stm32u5xx.c&emsp; &emsp; &emsp; &emsp; STM32U5xx system configuration source file
-   DSI/DSI_VideoMode_SingleBuffer/Src/stm32u5xx_it.c&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Interrupt handlers for Cortex-M33
-   DSI/DSI_VideoMode_SingleBuffer/Src/main.c&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Main program for Cortex-M33
-   DSI/DSI_VideoMode_SingleBuffer/Src/stm32u5xx_hal_msp.c&emsp; &emsp; &emsp; &nbsp; HAL MSP file for Cortex-M33

### <b>Hardware and Software environment</b>

-   This example runs on STM32U5A9xx devices without security enabled (TZEN=0).

-   This example has been tested with STMicroelectronics STM32U5x9J_DK (MB1829B)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    - Open your preferred toolchain
    - Rebuild all files and load your image into target memory
    - Run the example

