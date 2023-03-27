# STM32CubeU5 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeU5.svg?color=brightgreen)

**STM32Cube** is a STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform (this repo), delivered for each series (such as the STM32CubeU5 for the STM32U5 series).
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as ThreadX, FileX, USBX, NetDuoX, OpenBootloader, USBPD, trustedfirmware, mbed-crypto, Network Library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.
   * A new LPBAM utility which is a software helper that assists STM32U5 users in the elaboration of LPBAM scenarios.
   * A development with three Toolchains and Compilers (IAR Embedded Workbench for ARM (EWARM), RealView Microcontroller Development Kit (MDK-ARM), and STM32CubeIDE).

The **STM32CubeU5 MCU Package** projects are directly running on the STM32U5 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications, Demonstration, Examples).

## Some middleware libraries are unavailable in this repository

In this repository, the middleware libraries listed below **along with** the projects (demos, applications, and examples) using them, are **not available** as they (the middleware libraries) are subject to some restrictive license terms requiring the user's approval via a "click thru" procedure.
* `./Middlewares/ST/SE_STSAFEA_API`
* `./Middlewares/ST/STSAFE_A1xx`

If needed, they can be found inside the full firmware package available on our website `st.com` and downloadable from [here](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubeu5.html#get-software). You will be prompted to login or to register in case you have no account.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeU5/blob/main/Release_Notes.html).

## Boards available
  * STM32U5
    * [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html)
    * [NUCLEO-U575ZI-Q](https://www.st.com/en/evaluation-tools/nucleo-u575zi-q.html)
    * [STM32U575I-EV](https://www.st.com/en/evaluation-tools/stm32u575i-ev.html)
    * [NUCLEO-U5A5ZJ-Q](https://www.st.com/en/evaluation-tools/nucleo-u5a5zj-q.html)
    * [STM32U5x9J-DK](https://www.st.com/en/evaluation-tools/stm32u5a9j-dk.html)
    * [NUCLEO-U545RE-Q](https://www.st.com/en/evaluation-tools/nucleo-u545re-q.html)

## Troubleshooting

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
