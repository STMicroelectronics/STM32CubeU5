## <b>HASH_HMAC_SHA1MD5 Example Description</b>

How to use the HASH peripheral to hash data with HMAC SHA-1 and HMAC MD5 algorithms.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in polling mode when security is not enabled (TZEN=0).

The key used is long type (2088 bits = 261 bytes).
The HMAC SHA-1 message digest result is a 160-bit long (20 bytes) data and the HMAC MD5 message digest result is a 128-bit long (16 bytes) data.

The expected HASH digests (aExpectMD5Digest, aExpectSHA1Digest) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH (aMD5Digest,aSHA1Digest).

In case there is a mismatch the red LED is turned ON (LED3).
In case the SHA1 digest is computed correctly the green LED (LED1) is turned ON.
In case the MD5 digest is computed correctly the blue LED (LED2) is turned ON.

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

System, Security, HASH, HMAC, SHA1, MD5, DMA, digest

### <b>Directory contents</b>

  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32u5xx_hal_conf.h        HAL Configuration file
  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32u5xx_it.h              Header for stm32u5xx_it.c
  - HASH/HASH_HMAC_SHA1MD5/Inc/main.h                      Header for main.c module
  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32u5xx_it.c              Interrupt handlers
  - HASH/HASH_HMAC_SHA1MD5/Src/main.c                      Main program
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32u5xx_hal_msp.c         MSP initialization and de-initialization
  - HASH/HASH_HMAC_SHA1MD5/Src/system_stm32u5xx.c          STM32U5xx system clock configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

