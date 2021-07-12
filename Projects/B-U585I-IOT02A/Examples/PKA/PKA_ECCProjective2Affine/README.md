## <b>PKA_ECCProjective2Affine Example Description</b>

How to use the PKA to run ECC Projective to Affine operation

This project is targeted to run on STM32U585xx devices on B-U585I-IOT02A board from STMicroelectronics.
The project configures the maximum system clock frequency at 160Mhz.

ECC projective to affine operation computes the conversion between the representation of a point P in homogeneous projective coordinates
and the representation of the point P in affine coordinates.

The RNG initialization is mandatory for the PKA's operations.
To run ECC Projective to Affine operation with PKA we should fill the PKA_ECCProjective2AffineInTypeDef with required input vectors.
The result of the operation is composed by 2 elements:

 - Affine coordinate x of the point P which is stored in the attribute ptX of the structure out
 - Affine coordinate y of the point P which is stored in the attribute ptY of the structure out


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

Security, PKA, NIST, CAVP, ECC projective to affine

### <b>Directory contents</b>

  - PKA/PKA_ECCProjective2Affine/Src/main.c                  Main program
  - PKA/PKA_ECCProjective2Affine/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - PKA/PKA_ECCProjective2Affine/Src/stm32u5xx_it.c          Interrupt handlers
  - PKA/PKA_ECCProjective2Affine/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - PKA/PKA_ECCProjective2Affine/Inc/main.h                  Main program header file
  - PKA/PKA_ECCProjective2Affine/Inc/b_u585i_iot02_conf.h    BSP Configuration file
  - PKA/PKA_ECCProjective2Affine/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - PKA/PKA_ECCProjective2Affine/Inc/stm32u5xx_it.h          Interrupt handlers header file

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

