
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}


---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>BSP Example Description</b>

How to use the different BSP drivers of the board.

-   This project is targeted to run on STM32U5G9xx devices on STM32U5G9J-DK board from STMicroelectronics.

-   The project configures the maximum system clock frequency at 160Mhz.

-   This example shows how to use the different functionalities of components available on the board.

**<span style="color: red"> RED LED  </span> ** toggles every 100ms whenever any error is detected.

 **LED and Button**
-   This example shows how to switch on, switch off, toggle all leds and Press User push-button.
    ==> Result shown on the board

 **LCD**
-   This example shows how to enable the LCD and use the different LCD features.
    ==> Result is shown in the screen.

 **HSPI**
-   This example shows how to erase, write and read data available on the NOR memory available on the board.
    ==> Result is shown in the screen.

 **TOUCHSCREEN**
-   This example shows how to use the touchscreen.
    ==> Result is shown in the screen.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Directory contents</b>

  - BSP/Src/lcd.c&emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &nbsp;LCD drawing features
  - BSP/Src/led.c&emsp; &emsp; &emsp; &nbsp;Led display features
  - BSP/Src/main.c&emsp; &emsp; &emsp; &nbsp;Main program
  - BSP/Src/hspi.c&emsp; &emsp; &emsp; &nbsp;HSPI features
  - BSP/Src/touchscreen.c&emsp; &emsp; &emsp; &nbsp;TS features  
  - BSP/Src/stm32u5xx_hal_msp.c&emsp; &emsp; &emsp; &nbsp;HAL MSP module
  - BSP/Src/system_stm32u5xx.c&emsp; &emsp; &emsp; &nbsp;STM32U5xx system clock configuration file
  - BSP/Src/stm32u5xx_it.c&emsp; &emsp; &emsp; &nbsp;Interrupt handlers
  - BSP/Inc/main.h&emsp; &emsp; &emsp; &nbsp;Main program header file
  - BSP/Inc/stm32u5xx_hal_conf.h&emsp; &emsp; &emsp; &nbsp;HAL Configuration file
  - BSP/Inc/stm32u5x9j_discovery_conf.h&emsp; &emsp; &emsp; &nbsp;Board Configuration file
  - BSP/Inc/stm32u5xx_it.h&emsp; &emsp; &emsp; &nbsp;Interrupt handlers header file
  - BSP/Inc/stlogo.h&emsp; &emsp; &emsp; &nbsp;Image used for BSP LCD example
  - BSP/Inc/rect_buff.h&emsp; &emsp; &emsp; &nbsp;Image used for BSP LCD example
  - BSP/Inc/back.h&emsp; &emsp; &emsp; &nbsp;Image used for BSP example    
  - BSP/Inc/gt911_conf.h&emsp; &emsp; &emsp; &nbsp;TS configuration file
  - BSP/Inc/mx66uw1g45g_conf.h&emsp; &emsp; &emsp; &nbsp;OSPI NOR memory configuration file

### <b>Keywords</b>

BSP, Led, Button, LCD, TouchScreen, HSPI

### <b>Hardware and Software environment</b>  

  - This example runs on STM32U5G9xx/STM32U5F9xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U5G9J-DK (MB1918)
    board and can be easily tailored to any other supported device
    and development board.
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


:::
:::


:::
:::

