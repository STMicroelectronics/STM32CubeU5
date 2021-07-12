## <b>FLASH_ChangeOptionBytes Example Description</b>

How to configure and use the FLASH HAL API to change the STM32U5 devices Option Bytes.

This project is composed of two configurations:

- **FLASH_ChangeOptionBytes_TrustzoneDisabled**: to be used when TrustZone is disabled **(TZEN=0)**.
- **FLASH_ChangeOptionBytes_TrustzoneEnabled**: to be used when TrustZone is enabled **(TZEN=1)**.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

After Reset, the Flash memory controller is locked. Dedicated functions are used to enable the FLASH control register access and the FLASH option bytes register access.
The Option Bytes modification procedure is done by filling the Option Bytes init structure giving the option byte type and the option parameters.
At this stage, the new option bytes will be programmed through the HAL FLASH OB programming API.
Then, an OBL_Launch command is executed to start option bytes loading.

Once this operation is launched, the STM32U5 device will reset and debugger connection will be lost.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, Flash, Option Bytes, modification

### <b>Directory contents</b>

  - FLASH/FLASH_ChangeOptionBytes/Inc/stm32u5xx_hal_conf.h        HAL Configuration file
  - FLASH/FLASH_ChangeOptionBytes/Inc/stm32u5xx_it.h              Header for stm32u5xx_it.c
  - FLASH/FLASH_ChangeOptionBytes/Inc/main.h                      Header for main.c module
  - FLASH/FLASH_ChangeOptionBytes/Src/stm32u5xx_it.c              Interrupt handlers
  - FLASH/FLASH_ChangeOptionBytes/Src/main.c                      Main program
  - FLASH/FLASH_ChangeOptionBytes/Src/stm32u5xx_hal_msp.c         MSP initialization and de-initialization
  - FLASH/FLASH_ChangeOptionBytes/Src/system_stm32u5xx.c          STM32U5xx system clock configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx / STM32U585xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

