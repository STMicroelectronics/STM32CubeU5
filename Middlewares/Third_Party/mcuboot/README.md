# [mcuboot](http://mcuboot.com/)

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/stm32-mw-mcuboot.svg?color=brightgreen)
[![Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg)][license]

[license]: https://github.com/mcu-tools/mcuboot/blob/master/LICENSE

This is mcuboot version 1.7.2 modified by STMicroelectronics.

MCUboot is a secure bootloader for 32-bit MCUs. The goal of MCUboot is to
define a common infrastructure for the bootloader, system flash layout on
microcontroller systems, and to provide a secure bootloader that enables
simple software upgrades.

MCUboot is operating system and hardware independent and relies on
hardware porting layers from the operating system it works with.
MCUboot works with STM32Cube developement software and SoCs.

## Release note

Details about the updates made by STMicroelectronics are available in the release note [here](https://github.com/STMicroelectronics/stm32-mw-mcuboot/blob/main/st_readme.txt).

## Browsing

Information and documentation on the bootloader are stored within the source.

For more information in the source, here are some pointers:

- [boot/bootutil](boot/bootutil): The core of the bootloader itself.

## Compatibility information

Please refer to the release note in the repository of the STM32Cube **firmware** you are using to know which version of this middleware library to use with other components' versions (**e.g.**, other middleware libraries, drivers). It is **crucial** that you use a consistent set of versions.

## Troubleshooting

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.
