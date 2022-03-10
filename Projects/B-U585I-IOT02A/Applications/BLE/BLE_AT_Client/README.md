## <b>BLE_AT_Client Example Description</b>

This example demonstrates BLE connectivity on STM32WB5MMG module for the B-U585-IOT02A board.

The example allows to control STM32WB5MMG module running BLE AT server application.  
The application allows to toggle between 2 BLE services, Heart Rate sensor and P2P server.  
The user needs to use a smartphone running ST BLE Sensor application.  
The user can perform the following actions:  
  - a single user button press will send a BLE notification to connected smartphone.  
  - a double user button press will toggle the running BLE service.  
Instructions and logs are printed over ST-LINK UART.  

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

BLE, STM32WB5MMG, AT

### <b>Directory contents</b>

  - BLE/BLE_AT_Client/Src/main.c                  Main program
  - BLE/BLE_AT_Client/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - BLE/BLE_AT_Client/Src/stm32u5xx_it.c          Interrupt handlers
  - BLE/BLE_AT_Client/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - BLE/BLE_AT_Client/Src/ble_at_appli.c          Application part of the example  
  - BLE/BLE_AT_Client/Inc/main.h                  Main program header file
  - BLE/BLE_AT_Client/Inc/b_u585i_iot02_conf.h    BSP Configuration file
  - BLE/BLE_AT_Client/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - BLE/BLE_AT_Client/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - BLE/BLE_AT_Client/Inc/ble_at_appli.h          Application part of the example header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).
  
  - This example has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device and development board.

  - This example requires a smartphone running ST BLE Sensor application.
  
  - This example requires to have STM32WB5MMG module flashed with latest FUS, BLE full wireless stack and BLE_AT_Server application from STM32CubeWB package.

  - To flash BLE module you have to connect an STLINK dongle on CN5 connector.
    * CN5 pinout is from squared pin 1 at bottom: target_VDD, SWCLK, GND, SWDIO
    * Place SW3 on left position and plug USB on CN12 at the back. When done, unplug USB on CN12 and place back SW3 on right position.
    
  - Flash FUS and BLE stack using STM32CubeProgrammer wireless tab. Full detailed instructions are into STM32CubeWB package release note.
    * @0x080EC000 .\STM32Cube_FW_WB_V1.x.x\Projects\STM32WB_Copro_Wireless_Binaries\STM32WB5x\stm32wb5x_FUS_fw.bin
    * @0x080CA000 .\STM32Cube_FW_WB_V1.x.x\Projects\STM32WB_Copro_Wireless_Binaries\STM32WB5x\stm32wb5x_BLE_Stack_full_fw.bin
    
  - Flash BLE_AT_Server module application using STM32CubeProgrammer, file BLE_AT_Server_reference.hex.
    * present in .\STM32Cube_FW_WB_V1.x.x\Projects\P-NUCLEO-WB55.Nucleo\Applications\BLE\BLE_AT_Server\Binary\BLE_AT_Server_reference.hex

### <b>How to use it ?</b>

In order to make the program work, you must do the following :  
 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Board must be connected to a PC with USB (ST-LINK USB port),
   open a serial terminal emulator, find the board's COM port and configure it with:  
    - 8N1, 115200 bauds, no HW flow control  
    - set the line endings to LF (Transmit) and LF (receive).  
 - Run the example (by default P2P server application is started)  
 - On a smartphone launch ST BLE Sensor and search for a device.  
   - Connect to "P2PSRV" device.  
   - Once connected push user button to send a notification, an event is displayed on the smartphone.  
   - Click on the lamp on the smartphone, the green led on the board is toogled.  
   - Disconnect the link from the smatphone.  
 - Double click the board user button to change the BLE application.  
   - Connect to "HRSTM" device.  
   - Click on the user button to sent a Heart Rate value notification.

