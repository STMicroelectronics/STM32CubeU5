# STM32CubeU5 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeU5.svg?color=brightgreen)

> [!IMPORTANT]
> This repository has been created using the `git submodule` command. Please refer to the ["How to use"](README.md#how-to-use) section for more details.

## Overview

**STM32Cube** is an STMicroelectronics original initiative to ease developers' life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as ThreadX, FileX, USBX, NetDuoX, OpenBootloader, USBPD, trustedfirmware, mbed-crypto, Network Library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.
   * A new LPBAM utility which is a software helper that assists STM32U5 users in the elaboration of LPBAM scenarios.
   * A development with three Toolchains and Compilers (IAR Embedded Workbench for ARM (EWARM), RealView Microcontroller Development Kit (MDK-ARM), and STM32CubeIDE).

The **STM32CubeU5 MCU Package** projects are directly running on the STM32U5 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications, Demonstration, Examples).

> [!NOTE]
> ## Some middleware libraries and projects are unavailable in this repository
>
> In this repository, the middleware libraries listed below **along with** [this](Projects/README.md#list-of-unavailable-projects) list of projects (demos, 
> applications, and examples) using them, are **not available** as they (the middleware libraries) are subject to some restrictive license terms requiring the user's 
> approval via a "click thru" procedure.
> * `./Middlewares/ST/SE_STSAFEA_API`
> * `./Middlewares/ST/STSAFE_A1xx`
> 
> If needed, they can be found inside the full firmware package available on our website `st.com` and downloadable from [here](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubeu5.html#get-software). You will be prompted to login or to register in case you have no account.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeU5/blob/master/Release_Notes.html).

## How to use

This repository has been created using the `git submodule` command. Please check the instructions below for proper use. Please check also **the notes at the end of this section** for further information.

1. To **clone** this repository along with the linked submodules, option `--recursive` has to be specified as shown below.

```bash
git clone --recursive https://github.com/STMicroelectronics/STM32CubeU5.git
```

2. To get the **latest updates**, in case this repository is **already** on your local machine, issue the following **two** commands (with this repository as the **current working directory**).

```bash
git pull
git submodule update --init --recursive
```

3. To use the **same firmware version** as the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html), issue the command below **after** specifying the targeted `vX.Y.Z` version. This should be done **after** the command(s) indicated in instruction (1) or in instruction (2) above have been successfully executed.

```bash
git checkout vX.Y.Z # Specify the targeted vX.Y.Z version
```

4. To **avoid** going through the above instructions and **directly** clone the same firmware version as the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html), issue the command below **after** specifying the targeted `vX.Y.Z` version.

```bash
git clone --recursive  --depth 1 --branch vX.Y.Z https://github.com/STMicroelectronics/STM32CubeU5.git
```

> [!NOTE]
> * The latest version of this firmware available on GitHub may be **ahead** of the one available on [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html) or via [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). This is due to the **rolling release** process deployed on GitHub. Please refer to [this](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer/discussions/21) post for more details.
> * Option `--depth 1` specified in instruction (4) above is **not** mandatory. It may be useful to skip downloading all previous commits up to the one corresponding to the targeted version.
> * If GitHub "Download ZIP" option is used instead of the `git clone` command, then the different submodules have to be collected and added **manually**.

## Boards available
  * STM32U5
    * [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html)
    * [NUCLEO-U545RE-Q](https://www.st.com/en/evaluation-tools/nucleo-u545re-q.html)
    * [NUCLEO-U575ZI-Q](https://www.st.com/en/evaluation-tools/nucleo-u575zi-q.html)
    * [NUCLEO-U5A5ZJ-Q](https://www.st.com/en/evaluation-tools/nucleo-u5a5zj-q.html)
    * [STM32U5x9J-DK](https://www.st.com/en/evaluation-tools/stm32u5a9j-dk.html)
    * [STM32U5G9J-DK2](https://www.st.com/en/evaluation-tools/stm32u5g9j-dk2.html)
    * [STM32U575I-EV](https://www.st.com/en/evaluation-tools/stm32u575i-ev.html)

## Troubleshooting

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
