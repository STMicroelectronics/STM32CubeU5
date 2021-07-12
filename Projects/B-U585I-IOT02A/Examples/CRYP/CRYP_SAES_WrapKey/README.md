## <b>CRYP_SAES_WrapKey Example Description</b>

How to use the Secure AES co-processor (SAES) peripheral to wrap application keys
using hardware secret key DHUK then use it to encrypt in polling mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U585xx Devices :
    - The CPU at 160 MHz.

The Secure AES instance is configured in wrap mode and hardware secret key DHUK is selected.
The goal here to not share application key with users.
First, application key is encrypted (the result is stored in Encryptedkey buffer).
Then, user decrypt Encryptedkey, now application key is loaded in SAES_KEYRx registers
(without knowing him), immediately usable by users for any SAES operation.
Finally, ECB encryption/decryption is performed using key already in SAES_KEYRx registers
and results are compared with expected ones.

The Secure AES mode requires:

1. Application keys (AESKey256) to be wrapped using DHUK(derived hardware unique key).
2. Plain text (Plaintext) which will be encrypted.
3. Cipher text (CiphertextAESECB256)  which is the encryption result of the Plaint text.

In this example, LED7 (led green) is turned on when the operation finish with success
and LED6 (led red) is turned on when the operation generate an error.

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

  - CRYP/CRYP_SAES_WrapKey/Src/main.c                  Main program
  - CRYP/CRYP_SAES_WrapKey/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - CRYP/CRYP_SAES_WrapKey/Src/stm32u5xx_it.c          Interrupt handlers
  - CRYP/CRYP_SAES_WrapKey/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_SAES_WrapKey/Inc/main.h                  Main program header file
  - CRYP/CRYP_SAES_WrapKey/Inc/b_u585i_iot02_conf.h    BSP Configuration file
  - CRYP/CRYP_SAES_WrapKey/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - CRYP/CRYP_SAES_WrapKey/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

