##################################
Platform Service Integration Guide
##################################

************
Introduction
************
TF-M Platform service is a trusted service which allows secure partitions and
non-secure applications to interact with some platform-specific components.
There are a number of features which requires some interaction with
platform-specific components which are at the same time essential for the
security of the system.
Therefore, those components need to be handled by a secure partition which is
part of the trusted compute base.

These platform-specific components include system reset, power management,
Debug, GPIO, etc.

************************
TF-M Platform interfaces
************************
The TF-M interfaces for the Platform service are located in
``interface/include/``.
The TF-M Platform service source files are located in
``secure_fw/services/platform``.

*********************
TF-M Platform service
*********************
The Platform service interfaces and types are defined and documented in
``interface/include/tfm_platform_api.h``

- ``platform_sp.h/c`` : These files define and implement functionalities related
  to the platform service
- ``tfm_platform_secure_api.c`` : This file implements ``tfm_platform_api.h``
  functions to be called from the secure partitions. This is the entry point
  when the secure partitions request an action to the Platform service
  (e.g system reset).

************
Platform HAL
************

The Platform Service relies on a platform-specific implementation to
perform some functionalities. Mandatory functionality (e.g. system reset)
that are required to be implemented for a platform to be supported by TF-M have
their dedicated HAL API functions. Additional platform-specific services can be
provided using the IOCTL function call.

For API specification, please check: ``platform/include/tfm_platform_system.h``

An implementation is provided in all the supported platforms. Please, check
``platform/ext/target/<SPECIFIC_TARGET_FOLDER>/services/src/tfm_platform_system.c``
for examples.

The API **must** be implemented by the system integrators for their targets.

IOCTL
=====

A single entry point to platform-specific code across the HAL is provided by the
IOCTL service and HAL function:

.. code-block:: c

  enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                                 psa_invec *in_vec,
                                                 psa_outvec *out_vec);

A request type is provided by the client, with additional parameters contained
in the optional ``in_vec`` parameter. An optional output buffer can be passed to
the service in ``out_vec``.
An IOCTL request type not supported on a particular platform should return
``TFM_PLATFORM_ERR_NOT_SUPPORTED``

***************************
Current Service Limitations
***************************
- **system reset** - The system reset functionality is only supported in
  isolation level 1. Currently the mechanism by which PSA-RoT services should
  run in privileged mode in level 3 is not in place due to an ongoing work in
  TF-M Core. So, the ``NVIC_SystemReset`` call performed by the service is
  expected to generate a memory fault when it tries to access the ``SCB->AIRCR``
  register in level 3 isolation.

*****************************
Debug authentication settings
*****************************
A platform may provide the option to configure debug authentication. TF-M core
calls the HAL function ``enum tfm_plat_err_t tfm_spm_hal_init_debug(void)``
which configures debug authentication based on the following defines:

  - `DAUTH_NONE`: Debugging the system is not enabled.
  - `DAUTH_NS_ONLY`: Invasive and non invasive debugging of non-secure code is
    enabled.
  - `DAUTH_FULL`: Invasive and non-invasive debugging of non-secure and secure
    code is enabled.
  - `DAUTH_CHIP_DEFAULT`: The debug auhentication options are used that are set
    by the chip vendor.

The desired debug authentication configuration can be selected by setting one of
the options above to the cmake command with the
`-DDEBUG_AUTHENTICATION="<define>"` option. The default value of
`DEBUG_AUTHENTICATION` is `DAUTH_CHIP_DEFAULT`.

.. Note::
   ``enum tfm_plat_err_t tfm_spm_hal_init_debug(void)`` is called during the
   TF-M core initialisation phase, before initialising secure partition. This
   means that BL2 runs with the chip default setting.

--------------

*Copyright (c) 2018-2019, Arm Limited. All rights reserved.*
