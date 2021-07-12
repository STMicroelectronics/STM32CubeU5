---
pagetitle: Release Notes for STM32 USB-C Power Delivery U5 Device Driver
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}


<center>
# Release Notes for STM32 USB-C Power Delivery U5 Device Driver
Copyright &copy; 2020 STMicroelectronics\

[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

The USB-PD device driver provides a set of functions to manage the physical layer (i.e. low level of the type C state machine and low level of message transport). This includes :

- Type C state machine: SRC, SNK or DRP

- Physical layer : message handling SOP, SOP', SOP'', HARDRESET, ...

- Timer server to handle GOODCRC, PRL repetition timing

The USB-PD library is developed following the Universal Serial Bus Power Delivery Specification Revision 3.0, V2.0 (August 29, 2019) and Universal Serial Bus type-C Cable 
and Connector Specification, Revision 2.0 (August, 2019).
:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">v3.0.0 / 06-Apr-2020</label>
<div>

## Main Changes

### Maintenance release

## Contents
**Fixed bugs list**

  Headline
  ----------------------------------------------------------
  [TCPP03] Alignment with new TCPP0203 API namings
  [TCPP03] : Power mode update moved from device to BSP : on VBUSDeinit call, power mode is updated to Low power in case of SRC, or hibernate in case of SNK
  Remove temporary patch which impacts VBUS start (VBUS started too late on Ellisys, TD4.5.1)
  MCUAstyle + Codespell corrections
  Issue in configuration _DRP and _SRC
  U5 Device configuration template update
  Correct Keil compilation warning in usbpd_hw_if_it.c
  Protection for not taking FRS signal into account during PRS sequence
  Implementation of OCP recovery procedure in case of recoverable errors

## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers


- IAR Embedded Workbench for ARM (EWARM) toolchain V8.50.6
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.32
- STM32CubeIDE v1.7.0

## Supported Devices and boards

  All STM32U5xx devices embedding USBPD IP

## Backward compatibility

  No compatibility break with previous version

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v3.3.0

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">v2.0.0 / 26-Nov-2020</label>
<div>

## Main Changes

### Maintenance release

## Contents
**Fixed bugs list**

  Headline
  ----------------------------------------------------------
  Ticket 96061 - Tests TD4.3.x failed on Lecroy with NO_PD Sink version
  [FRS] confirm the FRS event by checking if an Rp resistor is still present
  Ticket 96223 Avoid double definition with LE16 & LE32 macros used in usbh.def
  Codespell corrections
  U5 Device configuration template update


## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers


- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.27
- STM32CubeIDE v1.4.0

## Supported Devices and boards

  All STM32U5xx devices embedding USBPD IP

## Backward compatibility

  No compatibility break with previous version

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v3.3.0

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">v1.1.0 / 27-Oct-2020</label>
<div>

## Main Changes

### Maintenance release

## Contents
**Fixed bugs list**

  Headline
  ----------------------------------------------------------
  [DEVICES][U5] : Add TCPP0203 support in U5 device (use of a dedicated GENERATOR define in xml)
  [USBPD][DEVICES][U5] : Merge part of U5 Device into reference code
  [USBPD][DEVICES][U5] : Update GPDMA API used in USBPD Device for U5
  [USBPD][U5] Put CC line in analog mode + Update on the DMA enable


## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers


- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.26
- STM32CubeIDE v1.4.0

## Supported Devices and boards

  All STM32U5xx devices embedding USBPD IP

## Backward compatibility

  No compatibility break with previous version

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v3.2.0

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section1"  aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">v1.0.0 / 11-Jun-2020</label>
<div>

## Main Changes

### Initial release

  Headline
  ----------------------------------------------------------
  First official version for STM32U5xx device (source code available)


## Known Limitations

  Outstanding bugs list : None

  Requirements not met or planned in a forthcoming release : None

## Development Toolchains and Compilers


- IAR Embedded Workbench for ARM (EWARM) toolchain V8.32.3
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.26
- STM32CubeIDE v1.2.0

## Supported Devices and boards

  All STM32U5xx devices embedding USBPD IP

## Backward compatibility

  No compatibility break with previous version

## Dependencies

 This software release is compatible with USB-C Power Delivery Core Stack Library v3.0.0

</div>
:::


:::
:::

<footer class="sticky">
For complete documentation on **STM32 32-bit Arm Cortex MCUs**,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)

This release note uses up to date web standards and, for this reason, should not
be opened with Internet Explorer but preferably with popular browsers such as
Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
