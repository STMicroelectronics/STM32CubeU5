## <b>Demo Demo Description</b>

The STM32Cube demonstration platform comes on top of the STM32Cube as a
firmware package that offers a full set of software components based on a modular
architecture. All modules can be reused separately in standalone applications. All 
these modules are managed by the STM32Cube demonstration kernel that allows to 
dynamically add new modules and access common resources (storage, memory management,
real-time operating system). The STM32Cube demonstration platform is built around a 
basic GUI interface. It is based on the STM32Cube HAL BSP and several middleware components.

### <b>Demonstration Overview</b>

At startup once the STMicroelectronics logo display is completed, the supported demos
will be viewed per icon. Use the JOYSTICK or the Touch Screen to navigate through the icons and select the 
demonstration modules.

Below you find an overview of the different modules present in the demonstration:

 + LPBAM (Low Power Batch Acquisition Modules)
 ----------------------------------------------
 It's a new feature on STM32U5xx Devices that allow to run many scenarios in low power modes without using the CPU.
 Below you find two examples of scenarios:  
   - Gyroscope  sensor acquisition and storing the data in the internal memory:  
     In this application, LPBAM utility is used to configure the Gyroscope sensor in order to read the data via I2C,
     then, we can store the data in the EEPROM in low power mode.  
     Finally after the wakeup, the stored data are plotted in a graph in the screen.  
     Note: Data are stored in a circular buffer that should be taken in consideration.  
   - Signal generation in autonomous mode:  
     The user can select the type of signal ( Square, Triangle, Sinus, DC..) then start the generation
     and enter in Low Power Mode.  
     User should connect a scope to SDC/DAC (CN8) to visualize the generated signal.

 + Security (TrustZone Modules)
 -------------------------------
 STM32U5xx Devices that allow to run a simulation of many security scenarios based on Cortex CM33 Core and the security controller (GTZC).
 You can refer to GTZC examples for the functional implementation.

 Below you find two examples that show how we can understand easily some security aspects present in this family :  
   - Peripheric access control:  
     This module shows how the user can configure a peripheral ( UART, LED) as a secure or non-secure peripheral.  
     The user can view the allowed access modes.

   - Memory Access Control (Water Marking):  
     The water marking security feature ensure the protection of a memory range for the access by non-secure usage.
	 In order to easily understood by the user, this module provide a drawing plan with a forbidden zone.
	 The user can draw with his finger in the allowed parts and not in the red zone. The memory address access
     is displayed in the log panel.  

 + Digital Filters (MDF/ADF)
 ----------------------------
 Three digital filters solutions based on STM32U5 device are demonstrated here:

 - A sound detector sub-module provides a solution to wake-up the MCU system after detecting a sound that exceeds 
   configurable threshold.
 - An audio recorder/player sub-module provides a complete audio playback solution and delivers different filtering modes

 - A power metering sub-module is able to perform the measurement of voltage and the current of an external circuit.
   for testing, you should connect the output of Potentiometer RV1 (JP21, PIN 1) to the measurement input (CN24, PIN1).
   Make sure that the Jumper JP30 is fitted.
   The voltage and current measurements are displayed in real-time in a graph.   

 + UCPD : ( Not included for this version)

 + Low Power Modes

 This module allows to perform consumption measurement based on the IDD circuitry present 
 on the MB1550 board, the user can measure the current on the IDD component using a multimeter.  
 Following modes with VDD at 3.3V can be measured :  
 - RUN mode at 160 Mhz from PLL.  
 - SLEEP mode at 160 Mhz from PLL.  
 - STOP0, STOP1, STOP2, STOP3 modes.  
 - STANDBY, SHUTDOWN modes  
 Wakeup is managed either by an EXTI linked to IDD circuitry , User Button or Joystick.  
 ( Known Limitation: Power consumption results may not fit with datasheet, need implementation improvement)

 + Camera viewer
 --------------
 The Camera viewer module allows to display pictures captured by the camera. To return to the main menu, you have 
 to use the joystick or touch the screen.

 + Calendar
 --------------
 The Calendar module allows to set the date and time and to configure an alarm.  
 LED green (LD4) is ON when the alarm is activated and it toggles for a few seconds 
 when the alarm expires.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in HAL time base ISR.
    This implies that if HAL_Delay() is called from a peripheral ISR process, then
    the HAL time base interrupt must have higher priority (numerically lower) than
    the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
    function.

 2. The application needs to ensure that the HAL time base is always set to 1 millisecond
    to have correct HAL operation.
    
 3. The modules  using lowpower modes may be disturbed by debugger connection.
    
### <b>Hardware and Software environment</b>

  - This demonstration runs on STM32U575xx devices.

  - This demonstration has been tested with STM32U575I-EV (MB1550) evaluation board and
    can be easily tailored to any other supported device and development board.

  @note Jumpers setup on STM32U575I-EV:  
   JP3  : OPENED        / JP4  : CLOSED        / JP5  : CLOSED        / JP7  : CLOSED  
   JP8  : OPENED        / JP9  : OPENED        / JP10 : 1-2 (UP)      / JP11 : 1-2 (UP)  
   JP12 : OPENED        / JP13 : OPENED        / JP14 : CLOSED        / JP15 : CLOSED  
   JP16 : CLOSED        / JP17 : 1-2 (Down)    / JP18 : CLOSED        / JP19 : CLOSED (IDD)  
   JP20 : 1-2 (Left)    / JP21 : 2-3 (Down)    / JP22 : 2-3 (Down)    / JP23 : 1-2 (Left)  
   JP24 : 1-2 (UP)      / JP25 : 5V-STLK       / JP30 : CLOSED        / SW3 : 0-0

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
   - Open STM32CubeProgrammer  
   - Connect the STM32U575I-EV board to PC with USB cable through CN26 (USB_STLK)  
   - Program the generated "STM32CubeU5_Demo_STM32U575I-EV.hex" file from the toolchain output folder  
   - Run the demonstration  

