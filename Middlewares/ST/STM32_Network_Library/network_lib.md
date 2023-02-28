---
pagetitle: Release Notes for the STM32 Network Library
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for the</small> <mark>STM32 Network Library</mark>
Copyright &copy; 2019 STMicroelectronics\
Microcontrollers Division - Application Team

[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# Purpose

The <mark>STM32 Network Library</mark> is a middleware providing network services on STM32 devices.
It provides a socket API (BSD like style) with support of secure or non secure connection and an API
to control the life cycle of the network adapters.

Two classes of network adapters are supported WIFI and Ethernet.
Different WIFI modules are supported from third party vendors.

# Documentation

[Doxygen documentation](./NetworkLib.chm)
:::
::: {.col-sm-12 .col-lg-8}

# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section10" checked aria-hidden="false">
<label for="collapse-section10" aria-hidden="false">V2.3.0 / 2022-December-12</label>
<div>

## Main changes

### Network library core
  - Improve CubeMX compatibility, following common rules for peripherals initialization needed for WiFi module (SPI / UART)
  - Rework some header files and remove local re-declarations of function or registers
  - Remove templates (`net_conf_xxx.c`) that are replaced by the WiFi driver implementations
  - Improve compatibility with `STM32_THREAD_SAFE_STRATEGY`
  - Continue reworking network interface state machine to improve management of AP && STA independently from IP protocol

###  WIFI Network interface
  - Add support of Mxchip WiFi on board B-U585I-IOT02A (FW v2.3.4 and further)
  - Add support for dynamic switching between AP to STA (Cypress module)

### Bug fixes

  - Fix AP mode for Mxchip WiFi

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.50.9
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.32
  - STM32CubeIDE Version 1.11.0

## Supported Devices and Boards

  - B-U585I-IOT02A board (MB1551 rev A and rev C).
  - B-L475E-IOT01A board (MB1297 rev D).
  - STM32H747I-DISCO (MB1248 rev D with Cypress WiFi uSD add-on board)

## Known Limitations

  - Unmaintained cellular support has been removed.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V2.2.0 / 2021-March-30</label>
<div>

## Main changes

### Network library core

  - Improve CubeMX compatibility, following common rules for peripherals initialization needed for Wifi module (SPI / UART)
  - Add DHCP server as a service
  - Add support of a direct command to network interface, to support AT command from application
  - Rework network interface state machine to improve management of AP && STA independently from IP protocol

###  WIFI Network interface

  - Add support of specific socket options needed to run Iperf (TCP_NODELAY)
  - Add support of Mxchip WiFi on board B-U585I-IOT02A (FW2.1.9 and further)
  - Add support for dynamic switching between AP to STA (Cypress module)
  - Add support of link Up/Down events for Cypress WiFi module
  - Add support of API to set Wifi Custom Elements (Cypress module / HomeKit project)

### Bug fixes

  - Fix bind function for UDP socket
  - Fix Inventek WiFi support for UDP 'server' socket
  - Fix monitoring primitives

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.50.5
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.27.1

## Supported Devices and Boards

  - B-U585I-IOT02A board (MB1551 rev A and rev C).
  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)
  - STM32H743I-EVAL (with Ethernet interface)

## Known Limitations

  - Cellular on P-L496G-CELL02: Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V2.1.0 / 2020-Mai-04</label>
<div>

## Main changes

### Network library core

  - Improve performance monitoring primitives adding Thread display support
  - Improve Memory debug  primitive
  - Add Node Tree display for dynamic memory allocation

### MXCHIP WIFI Network interface

  - Add support of UART interrupt module

### Bug fixes

  - Fix uninitialized variable for all WIFI network interface to force default to AP mode when connecting
  - Fix memory leakage for MXCHIP network interface

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.32.3
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.27.1
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)
  - STM32H743I-EVAL (with Ethernet interface)

## Known Limitations

  - Cellular on P-L496G-CELL02:
  - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V2.0.0 / 2020-April-08</label>
<div>

## Main changes

### Network library core

  - Prepare support of IPv6 addresses, making code independent from network address definition
  - Clean up template of configuration file to allow application to overwrite default definition
  - Add performance monitoring primitives
  - Add memory allocation primitive to track leakage
  - Add yield feature for specific network interface

### WIFI generic support

  - Add WIFI Access point support, with needed new API (set_channel, set mode )
  - Update state machine management to support AP and STA connection
  - Add support of concurrent interface, for instance an AP running in parallel with a STA
  - Extend list of supported security mode

### Cellular Network interface

  - Add compatibility until XCube Cellular package 1.5.0

### Cypress WIFI Network interface

  - Add support of Access Point mode

### MXCHIP WIFI Network interface

  - Newly supported WIFI module
  - Add support of UART and SPI based module
  - Add templates files

### Inventek WIFI network interface

  - Add support of net_getpeername and net_getsocketinfo API

### Bug fixes

  - Fix memory leakage for Cypress WHD based devices
  - Fix memory leakage for secure socket (based on MbedTLS )
  - Fix timeout definition for Cellular devices on connection
  - Fix state machine on deinitialization transition

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.32.3
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.27.1
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)
  - STM32H743I-EVAL (with Ethernet interface)

## Known Limitations

  - Cellular on P-L496G-CELL02:
  - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V1.1.0 / 2019-November-27</label>
<div>

## Main changes

  - Add support for WIFI-WHD devices, create a new network interface
  - Add support of ethernet on H7 device (STM32H743I-EVAL board)
  - Restructure code regarding LWIP based network interface
  - lwip related initialisation is moved to net_conf.c
  - Compliant with ST-Quality standard
    - Pass MISRA check list
    - Pass CodeSonar test
    - Enforce ST coding style rules

### Bug fixes

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.30.1.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)
  - STM32H743I-EVAL (with Ethernet interface)

## Known Limitations

  - Cellular on P-L496G-CELL02:
    - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.0.5 / 2019-October-8</label>
<div>

## Main changes

  - Add support for CMSIS-OS2

### Bug fixes

  - Fix cellular network interface, required to used firmware version >= BG96MAR02A08M1G.
  - Fix message order on notification

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.30.1.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)

## Known Limitations

  - Cellular on P-L496G-CELL02:
    - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.0.4 / 2019-August-1</label>
<div>

## Main changes

  - Renamed STM32 Connectivity library as STM32 Network library
  - Re-implement State management of network interface ,renamed states from INITIALIZED to READY , suppress some unused states
  - Add and test support of embedded Inventek WIFI TLS socket
  - Run MISRA checks

### Bug fixes

  - Ethernet template: properly clear the "Receive buffer unavailable status" in case of RX descriptor underflow.
  - Fix missing semaphore on net_wait API

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.30.1.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)

## Known Limitations

  - Cellular on P-L496G-CELL02:
    - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.0.3 / 2019-july-05</label>
<div>

## Main changes

### Bug fixes

  - Run Astyle checks
  - Add doxygen doc generation
  - Fix issue on F769 Ethernet interface , avoid blocking thread on alloc failure from LWIP pool

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.32.3.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)

## Known Limitations

  - Cellular on P-L496G-CELL02:
    - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.0.2 / 2019-june-07</label>
<div>

## Main changes

### Bug fixes

  - Ethernet template: properly clear the "Receive buffer unavailable status" in case of RX descriptor underflow.

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.32.3.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)

## Known Limitations

  - Cellular on P-L496G-CELL02:
    - Server sockets are not supported

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 2019-april-26</label>
<div>

## Initial version

  - Features supported:
    - BSD socket like API
    - Network interface control API (init, start, stop, ...)
    - TCP and UPD protocols
    - Client and server connections
    - DNS and ping services
    - Secure socket on top of mbedTLS
    - RTOS and bare metal applications are supported
    - IPv4 address only is supported (IPv6 will be available in a future version)
    - Wi-Fi, Ethernet and cellular connectivities

## Development Toolchains and Compilers

  - IAR Embedded Workbench for ARM (EWARM). Version 8.30.1.
  - Keil Microcontroller Development Kit (MDK-ARM) Version 5.26.
  - System Workbench for STM32. Version 2.8.1.

## Supported Devices and Boards

  - B-L475E-IOT01A board (MB1297 rev D).
  - 32F413HDISCOVERY board (MB1274 rev B).
  - 32F769IDISCOVERY board (MB1225 rev B).
  - P-L496G-CELL02 board package including the 32L496G-Discovery board (MB1261 rev B) and the Cellular Add-on board based on BG96 4G Modem (MB1329 rev B)

## Known Limitations

  - Cellular on P-L496G-CELL02
    - UDP sockets are not supported
    - Server sockets are not supported

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32</mark> microcontrollers please visit <http://www.st.com/stm32>
</footer>
