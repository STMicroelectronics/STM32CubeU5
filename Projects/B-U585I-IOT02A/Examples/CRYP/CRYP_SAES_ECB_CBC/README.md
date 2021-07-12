## <b>CRYP_SAES_ECB_CBC Example Description</b>

How to use the SecureAES co-processor (SAES) peripheral to encrypt and decrypt data
using AES ECB and CBC Algorithms when security is not enabled (TZEN=0).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U585xx Devices :
   - The CPU at 160 MHz.

The Secure AES mode requires:

1. Plain text which will be encrypted. Its size must be a multiple of 16 bytes.
   So if the original plain text size is not a multiple of 16 bytes, it must be padded.
2. Cipher text which is the encryption result of the Plaint text. In this
   example, the cipher text is available in "EncryptedText".
3. Initialization vector used only for CBC Algorithm.
4. Key is the parameter which determines the cipher text. In this example the
   key size is 128 bits =16 bytes for ECB and 256 bits for CBC.

After startup , The Secure AES  is configured to encrypt/decrypt using ECB algorithm
and 128bit key length, then change the configuration to use CBC algorithm and
256bit key length  then perform the encrypt/decrypt.
All encrypt text are compared to a known results in CiphertextAESECB128 and CiphertextAESCBC256 buffers.
Finally, LED7 (led green)) is toggling if all encrypt/decrypt are as expected and LED6 (led red) is turned on when the operation generate an error.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


### <b>Keywords</b>

System, Security, SAES, AES,  TrustZone disabled

### <b>Directory contents</b>

  - CRYP/CRYP_SAES_ECB_CBC/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_SAES_ECB_CBC/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_SAES_ECB_CBC/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_SAES_ECB_CBC/Src/stm32u5xx_it.c          Interrupt handlers
  - CRYP/CRYP_SAES_ECB_CBC/Src/main.c                  Main program
  - CRYP/CRYP_SAES_ECB_CBC/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_SAES_ECB_CBC/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).
  - This template has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


