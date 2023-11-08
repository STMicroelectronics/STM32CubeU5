
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>BSP Example Description</b>

How to use the different BSP drivers of the board.

-   This project is targeted to run on STM32U5A9xx devices on STM32U5x9J-DK board from STMicroelectronics.

-   The project configures the maximum system clock frequency at 160Mhz.

-   This example shows how to use the different functionalities of components available on the board.

**<span style="color: red"> RED LED  </span> ON ** toggles every 100ms whenever any error is detected.

**It uses the Terminal I/O to display information :**

When resorting to EWARM IAR IDE:<br>
Command Code is displayed on debugger as follows: View --> Terminal I/O<br>
\
When resorting to MDK-ARM KEIL IDE:<br>
Command Code is displayed on debugger as follows: View --> Serial Windows --> Debug (printf) Viewer<br>
***<span style="color: magenta">(To display data on MDK-ARM debugger please make sure to close the SB34 and open SB33 on the board)</span>***

\
When resorting to STM32CubeIDE:<br>
In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable" Command Code is displayed on debugger as follows: Window--> Show View--> Console<br>
\
\
 **LED**
-   This example shows how to switch on, switch off, toggle all leds.
    ==> Result shown on the board

 **LCD**
-   This example shows how to enable the LCD and use the different LCD features.
    ==> Result is shown in the screen.

 **OSPI**
-   This example shows how to erase, write and read data available on the Octal Flash memory available on the board. (STR and DTR in Normal mode and MemoryMapped)
    ==> Result shown on Terminal I/O

 **HSPI**
-   This example shows how to erase, write and read data available on the HexaDecaSPI ram memory available on the board.
    Frequency 80Mhz for STM32U5G9xx and STM32U5A9xx
    ==> Result shown on Terminal I/O

 **MMC**
-   This example shows how to erase, write and read data available on the eMMC memory available on the board.
    ==> Result shown on Terminal I/O

 **TOUCHSCREEN**
-   This example shows how to use the touchscreen.
    ==> Result is shown in the screen.

 **RANGING SENSOR**
-   This example shows how to use the Time of flight sensor.
    ==> Result is shown in the screen.

 ** ENVIRONMENTAL SENSOR **
-   This example shows how to use the temperature of the environmental sensor.
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

  - BSP/Src/lcd.c                       LCD drawing features
  - BSP/Src/led.c                       Led display features
  - BSP/Src/main.c                      Main program
  - BSP/Src/ospi.c                      OSPI features
  - BSP/Src/hspi.c                      HSPI features
  - BSP/Src/emmc.c                      MMC features 
  - BSP/Src/touchscreen.c               TS features  
  - BSP/Src/env_sensor.c                Env sensor features
  - BSP/Src/stm32u5xx_hal_msp.c         HAL MSP module
  - BSP/Src/system_stm32u5xx.c          STM32U5xx system clock configuration file
  - BSP/Src/stm32u5xx_it.c              Interrupt handlers
  - BSP/Inc/main.h                      Main program header file
  - BSP/Inc/stm32u5xx_hal_conf.h        HAL Configuration file
  - BSP/Inc/stm32u5x9j_discovery_conf.h Board Configuration file
  - BSP/Inc/stm32u5xx_it.h              Interrupt handlers header file
  - BSP/Inc/stlogo.h                    Image used for BSP LCD example
  - BSP/Inc/hexagone.h                  Image used for the main menu of BSP example
  - BSP/Inc/back.h                      Image used for BSP example    
  - BSP/Inc/sitronix_conf.h             TS configuration file
  - BSP/Inc/mx25um51245g_conf.h         OSPI NOR memory configuration file
  - BSP/Inc/aps512xx_conf.h             HSPI NOR memory configuration file 

### <b>Keywords</b>

BSP, Led, Button, LCD, eMMC, TouchScreen, OSPI, HSPI, Env sensor

### <b>Hardware and Software environment</b>

-   This example runs on STM32U5A9xx Devices.
-   This example runs on STM32U5G9xx Devices.

  - This example runs on STM32U5x9xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U5x9J-DK (MB1829)
    board and can be easily tailored to any other supported device
    and development board.
	
  - To be able to display data on MDK-ARM debugger please make sure to close the SB34 and open SB33
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


:::
:::

