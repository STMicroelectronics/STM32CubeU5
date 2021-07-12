##################
Build instructions
##################
Please make sure you have all required software installed as explained in the
:doc:`software requirements <tfm_sw_requirement>`.

*********************
Configuring the build
*********************
The build configuration for TF-M is provided to the build system using command
line arguments:

.. list-table::
   :widths: 20 80

   * - -DPROJ_CONFIG=<file>
     - Specifies the way the application is built.

       | <file> is the absolute path to configurations file
         named as ``Config<APP_NAME>.cmake``.
       | i.e. On Linux:
         ``-DPROJ_CONFIG=`readlink -f ../configs/ConfigRegression.cmake```

   * - -DTARGET_PLATFORM=<target platform name>
     - Specifies the target platform.
       Supported platforms:

          - Cortex-M33 SSE-200 subsystem for MPS2+ (AN521)
            ``-DTARGET_PLATFORM=AN521``
          - Cortex-M23 IoT Kit subsystem for MPS2+ (AN519)
            ``-DTARGET_PLATFORM=AN519``
          - Musca-A test chip board (Cortex-M33 SSE-200 subsystem)
            ``-DTARGET_PLATFORM=MUSCA_A``
          - Musca-B1 test chip board (Cortex-M33 SSE-200 subsystem)
            ``-DTARGET_PLATFORM=MUSCA_B1``
          - Cortex-M33 SSE-200 subsystem for MPS3 (AN524)
            ``-DTARGET_PLATFORM=AN524``

   * - -DCOMPILER=<compiler name>
     - Specifies the compiler toolchain
       The possible values are:

         - ``ARMCLANG``
         - ``GNUARM``
   * - -DCMAKE_BUILD_TYPE=<build type>
     - Configures debugging support.
       The possible values are:

         - ``Debug``
         - ``Release``
         - ``Relwithdebinfo``
         - ``Minsizerel``
   * - -DMBEDTLS_DEBUG=<ON|OFF>
     - Enables debug symbols for the Mbed TLS and Mbed Crypto libraries.
   * - -DBUILD_DWARF_VERSION=<dwarf version>
     - Configures DWARF version.
       The possible values are:

         - 2
         - 3
         - 4

.. Note::
    Follow :doc:`secure boot <./tfm_secure_boot>` to build the binaries with or
    without BL2 bootloader.

*******************
External dependency
*******************
- `CMSIS_5` is used to import RTX for the example non-secure app
- Mbed TLS and Mbed Crypto are used as crypto libraries on the secure side

****************
TF-M build steps
****************
TF-M uses `cmake <https://cmake.org/overview/>`__ to provide an out-of-source
build environment. The instructions are below.

.. Note::

    It is recommended to build each different build configurations in separate
    directories.

Getting the source-code
=======================
.. code-block:: bash

    cd <TF-M base folder>
    git clone https://git.trustedfirmware.org/trusted-firmware-m.git
    git clone https://github.com/ARMmbed/mbedtls.git -b mbedtls-2.7.9
    git clone https://github.com/ARMmbed/mbed-crypto.git -b mbedcrypto-1.1.0
    git clone https://github.com/ARM-software/CMSIS_5.git -b 5.5.0

.. Note::
   `CMSIS_5` now uses `git-lfs` for storing large size binaries so the cloning
   process has changed from previous releases. Please refer to the `CMSIS_5`
   documentation for more details.

Build steps for the AN521 target platform:
==========================================
.. code-block:: bash

    cd <TF-M base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake ../ -G"Unix Makefiles" -DTARGET_PLATFORM=AN521 -DCOMPILER=ARMCLANG
    cmake --build ./ -- install

Regression Tests for the AN521 target platform
==============================================
*TF-M build regression tests on Linux*

.. code-block:: bash

    cd <TF-M base folder>
    cd trusted-firmware-m
    mkdir cmake_test
    cd cmake_test
    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigRegression.cmake` -DTARGET_PLATFORM=AN521 -DCOMPILER=ARMCLANG ../
    cmake --build ./ -- install

*TF-M build regression tests on Windows*

.. code-block:: bash

    cd <TF-M base folder>
    cd trusted-firmware-m
    mkdir cmake_test
    cd cmake_test
    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`cygpath -am ../configs/ConfigRegression.cmake` -DTARGET_PLATFORM=AN521 -DCOMPILER=ARMCLANG ../
    cmake --build ./ -- install

Build for PSA API compliance tests
==================================
The build system provides the support for linking with prebuilt PSA API
compliance NS test libraries when using the ``ConfigPsaApiTest.cmake``,
``ConfigPsaApiTestIPC.cmake`` or ``ConfigPsaApiTestIPCTfmLevel2.cmake`` config
file. The build system assumes that the PSA API compliance test suite is checked
out at the same level of the TF-M root folder and the default name for the build
folder has been used when compiling the PSA API compliance tests. Each set of
tests for the Internal Trusted Storage, Secure Storage, Crypto and Attestation
services needs to be enabled at the build configuration step by defining::

    -DPSA_API_TEST_INTERNAL_TRUSTED_STORAGE=ON
    -DPSA_API_TEST_SECURE_STORAGE=ON
    -DPSA_API_TEST_CRYPTO=ON
    -DPSA_API_TEST_ATTESTATION=ON

respectively for the corresponding service. For example, to enable the PSA API
tests for the Crypto service only:

.. code-block:: bash

    cd <TF-M base folder>
    cd trusted-firmware-m
    mkdir cmake_psa_test
    cd cmake_psa_test
    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigPsaApiTest.cmake` -DPSA_API_TEST_CRYPTO=ON -DTARGET_PLATFORM=AN521 -DCOMPILER=ARMCLANG ../
    cmake --build ./ -- install

Location of build artifacts
===========================
The build system defines an API which allow easy usage of build
artifacts. The ``install`` build target copies all files which might be needed
as a dependency by external tools or build systems to the
``<build_dir>/install/outputs``
directory, with the following directory hierarchy:

::

    <build_dir>/install/outputs/fvp/
    <build_dir>/install/outputs/<target_platform>/

There is one folder for FVP testing, with more elaborate naming and
there is an other for testing on target hardware platform (AN521, etc.), where
naming convention is aligned with 8.3 format. The dependency tree of
``install`` build target ensures a proper update (i.e. build) of all output
files before the actual installation step takes place. As such it is suggested
to use this build target to build TF-M.

Export dependency files for NS applications
-------------------------------------------
An NS application requires a number of files to interface with TF-M.
The build system exports these files as part of the ``install`` target and
places them in to a single directory::

    <build_dir>/install/export/tfm

Further details on how to integrate a new NS app with TF-M are available in the
:doc:`integration guide <tfm_integration_guide>`.

Building the documentation
==========================
Please ensure the dependencies for building the firmware and the
documentation are installed as explained in the
:doc:`software requirements <tfm_sw_requirement>`.

Building PDF output is optional and can be disabled by removing LaTex from the
PATH.

.. Note::
   For building the documentation all tools needed to build the firmware must
   be available.

Building the Reference Manual
-----------------------------
.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_doc
    cd cmake_doc
    cmake ../ -G"Unix Makefiles" -DTARGET_PLATFORM=AN521 -DCOMPILER=GNUARM
    cmake --build ./ -- install_doc

The documentation files will be available under the directory::

    cmake_doc/install/doc/reference_manual

Building the User Guide
-----------------------
.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_doc
    cd cmake_doc
    cmake ../ -G"Unix Makefiles" -DTARGET_PLATFORM=AN521 -DCOMPILER=ARMCLANG
    cmake --build ./ -- install_userguide

The documentation files will be available under the directory::

    cmake_doc/install/doc/user_guide

--------------

*Copyright (c) 2017-2019, Arm Limited. All rights reserved.*
