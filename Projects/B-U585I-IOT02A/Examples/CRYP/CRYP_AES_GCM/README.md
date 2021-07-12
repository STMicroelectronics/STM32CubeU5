## <b>CRYP_AES_GCM Example Description</b>

How to use the CRYPTO peripheral to encrypt and decrypt data using AES with Galois/Counter mode (GCM).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U585AIIxQ Devices :
  - The CPU at 160 MHz.

The AES-GCM mode requires:

1. Plain text which will be authenticated and encrypted. Its size must be a
   multiple of 16 bytes. So if the original plain text size is not a multiple
   of 16 bytes, it must be padded.
2. Header that will be authenticated but not encrypted. The header size must be
   multiple of 16 bytes whatever the key size.
3. Initialization vector a value that is used only once within a specified
   context. It must be padded to 16-byte length with LSB set to 2.
4. Key is the parameter which determines the cipher text. In this example the
   key size is 128 bits =16 bytes but it can be tailored to 192 bits or 256 bits.

The AES-GCM provides:

1. Cipher text which is the encryption result of the Plaint text. In this
   example, the cipher text is available in "EncryptedText".
   Its size is the same as the plain text.
2. Plain text which is the decryption result of the Cipher text. In this
   example, the plain text is available in "DecryptedText".
   Its size is the same as the plain text.
3. TAG: The Message Authentication Code which is used for both message
   authentication and message tampering detection. It's size is always 16 bytes.

The green LED (LED7) is turned ON, in case the encryption/decryption and tag are
computed correctly, in case there is a mismatch the red LED is turned ON (LED6).

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Security, CRYP, AES, TrustZone disabled

### <b>Directory contents</b>

  - CRYP/CRYP_AES_GCM/Inc/b_u585i_iot02a_conf.h   BSP Configuration file
  - CRYP/CRYP_AES_GCM/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AES_GCM/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AES_GCM/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AES_GCM/Src/stm32u5xx_it.c          Interrupt handlers
  - CRYP/CRYP_AES_GCM/Src/main.c                  Main program
  - CRYP/CRYP_AES_GCM/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AES_GCM/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585AIIxQ devices.

  - This template has been tested with STMicroelectronics B-U585I-IOT02A
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


