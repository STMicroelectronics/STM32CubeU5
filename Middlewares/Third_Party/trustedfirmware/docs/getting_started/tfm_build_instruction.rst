##################
Build instructions
##################
Please make sure you have all required software installed as explained in the
:doc:`software requirements <tfm_sw_requirement>`.

****************
TF-M build steps
****************
TF-M uses `cmake <https://cmake.org/overview/>`__ to provide an out-of-source
build environment. The instructions are below.

Cmake version ``3.15.0`` or higher is required.

Getting the source-code
=======================
.. code-block:: bash

    cd <base folder>
    git clone https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git

Dependency management is now handled by cmake. If you wish to alter this
behaviour, see :ref:`docs/getting_started/tfm_build_instruction:Manual
dependency management`

.. Note::

 - For building with Armclang compiler version 6.10.0+, please follow the note
   in :doc:`software requirements <tfm_sw_requirement>`.
 - For building with the IAR toolchain, please see the notes in
   :doc:`IAR software requirements <tfm_build_instruction_iar>`

.. _tfm_cmake_configuration:

Cmake configuration
===================

All configuration options are provided by cmake variables, and their default
values, with docstrings, can be found in ``config/config_default.cmake``.

Configuration is provided in multiple stages. Each stage will not override any
config that has already been set at any of the prior stages.

   1. Command-line variable settings are applied.
   2. If the ``TFM_EXTRA_CONFIG_PATH`` variable has been set, that file is
      loaded.
   3. If TEST_PSA_TEST is set, then PSA API test related config is applied from
      ``config/tests/config_test_psa_api.cmake``.
   4. If it exists, CMAKE_BUILD_TYPE specific config is applied from
      ``config/build_type/<build_type>.cmake``.
   5. Target specific config from ``platform/ext/target/<target_platform>/config.cmake``
      is applied.
   6. If CRYPTO_HW_ACCELERATOR is set, then a config specific to the
      accelerator type is applied if it exists.
   7. If it exists, TFM Profile specific config is applied from
      ``config/profile/<tfm_profile>.cmake``.
   8. ``config/config_default.cmake`` is loaded.

.. Warning::
    This means that command-line settings are not applied when they conflict
    with required platform settings. If it is required to override platform
    settings (this is not usually a good idea) then TFM_EXTRA_CONFIG_PATH should be
    used.

Required cmake parameters for building TF-M
-------------------------------------------

+----------------------+-------------------------------------------------------+
| Parameter            | Description                                           |
+======================+=======================================================+
| TFM_PLATFORM         | The target platform as a path from the base directory |
|                      | ``/platform/ext/target``, or as an absolute path.     |
+----------------------+-------------------------------------------------------+
| TFM_TOOLCHAIN_FILE   | The path to the toolchain file that corresponds to    |
|                      | the desired compiler.                                 |
+----------------------+-------------------------------------------------------+

By default release configuration builds. Alternate build types can be controlled
by the CMAKE_BUILD_TYPE variable.

Build type
----------

Build type is controlled by the ``CMAKE_BUILD_TYPE`` variable. The possible
types are:

 - ``Debug``
 - ``Relwithdebinfo``
 - ``Release``
 - ``Minsizerel``

``Release`` is default.

Both ``Debug`` and ``Relwithdebinfo`` will include debug symbols in the output
files. ``Relwithdebinfo``, ``Release`` and ``Minsizerel`` have optimization
turned on and hence will produce smaller, faster code. ``Minsizerel`` will
produce the smallest code, and hence is often a good idea on RAM or flash
constrained systems.

Other cmake parameters
----------------------

The full list of default options is in ``config/config_default.cmake``. Several
important options are listed below.


+---------------------+----------------------------------------+---------------+
| Parameter           | Description                            | Default value |
+=====================+========================================+===============+
| BL2                 | Build level 2 secure bootloader.       | ON            |
+---------------------+----------------------------------------+---------------+
| NS                  | Build NS app. Required for test code.  | ON            |
+---------------------+----------------------------------------+---------------+
| TFM_PSA_API         | Use PSA api (IPC mode) instead of      | OFF           |
|                     | secure library mode.                   |               |
+---------------------+----------------------------------------+---------------+
| TFM_ISOLATION_LEVEL | Set TFM isolation level.               | 1             |
+---------------------+----------------------------------------+---------------+
| TFM_PROFILE         | Set TFM profile.                       |               |
+---------------------+----------------------------------------+---------------+
| TEST_S              | Build secure regression tests.         | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_NS             | Build non-secure regression tests.     | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_PSA_API        | Build PSA API TESTS for the given      |               |
|                     | suite. Takes a PSA api ``SUITE`` as an |               |
|                     | argument (``CRYPTO`` etc).             |               |
+---------------------+----------------------------------------+---------------+

Regression test configuration
-----------------------------

Regression test configuration is controlled entirely by the ``TEST_S`` and
``TEST_NS`` cmake variables.

If regression testing is enabled, it will then enable all tests for the enabled
secure partitions. If IPC mode is enabled via ``TFM_PSA_API`` the IPC tests will
be enabled. QCBOR and T_COSE tests are linked to the Initial Attestation
partition, as they are only used there. Multicore tests will be enabled if
``TFM_MULTI_CORE_TOPOLOGY`` is enabled.

Some cryptographic tests can be enabled and disabled. This is done to prevent
false failures from being reported when a smaller Mbed Crypto config is being
used which does not support all features.

+-----------------------------+-------------------------------------+---------------+
| Parameter                   | Description                         | Default value |
+=============================+=====================================+===============+
| TFM_CRYPTO_TEST_ALG_CBC     | Test CBC cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CCM     | Test CCM cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CFB     | Test CFB cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CTR     | Test CTR cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_GCM     | Test GCM cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_SHA_512 | Test SHA-512 cryptography algorithm | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_HKDF        | Test SHA-512 cryptography algorithm | ON            |
+-----------------------------+-------------------------------------+---------------+

TF-M Profiles
-------------

TF-M Profiles are implemented as a single cmake configuration file, under the
``config/profile`` directory. A good understanding can be gained quickly by
looking at the Profile configuration files, but the ultimate reference for
Profiles are the design documents in the ``docs/design_documents/profiles/``
directory.

PSA test configuration
----------------------

PSA tests are configured by using the ``TEST_PSA_API`` cmake variable. The
variable should be set to the name of the test suite that is desired. It is
_not_ supported to set both ``TEST_PSA_API`` and ``TEST_S`` or ``TEST_NS``.

The Functional API tests are:
 - ``CRYPTO``
 - ``INITIAL_ATTESTATION``
 - ``STORAGE`` (INTERNAL_TRUSTED_STORAGE and PROTECTED_STORAGE)
 - ``INTERNAL_TRUSTED_STORAGE``
 - ``PROTECTED_STORAGE``

The Firmware Framework test suites are:
 - ``IPC``

Note that these map directly to the ``SUITE`` cmake variable used in the
psa-arch-tests documentation.

Migration from legacy buildsystem
---------------------------------

The previous (legacy) cmake buildsystem made use of separate configuration
files, where now build options are controlled by variables. For ease of
transition, a table below is provided that maps the legacy files to the current
variables, in the format of cmake command line parameters.

+------------------------------------------+---------------------------------------+
| File                                     | Cmake command line                    |
+==========================================+=======================================+
| ConfigDefault.cmake                      | <No options>                          |
+------------------------------------------+---------------------------------------+
| ConfigCoreIPC.cmake                      | -DTFM_PSA_API=ON                      |
+------------------------------------------+---------------------------------------+
| ConfigCoreIPCTfmLevel2.cmake             | -DTFM_PSA_API=ON                      |
|                                          | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileS.cmake              | -DTFM_PROFILE=profile_small           |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileM.cmake              | -DTFM_PROFILE=profile_medium          |
+------------------------------------------+---------------------------------------+
| ConfigRegression.cmake                   | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigRegressionIPC.cmake                | -DTEST_NS=ON -DTEST_S=ON              |
|                                          | -DTFM_PSA_API=ON                      |
+------------------------------------------+---------------------------------------+
| ConfigRegressionIPCTfmLevel2.cmake       | -DTEST_NS=ON -DTEST_S=ON              |
|                                          | -DTFM_PSA_API=ON                      |
|                                          | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigRegressionProfileS.cmake           | -DTFM_PROFILE=profile_small           |
|                                          | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigRegressionProfileM.cmake           | -DTFM_PROFILE=profile_medium          |
|                                          | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTest.cmake                   | -DTEST_PSA_API=<test_suite>           |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTestIPC.cmake                | -DTEST_PSA_API=<test_suite>           |
|                                          | -DTFM_PSA_API=ON                      |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTestIPCTfmLevel2.cmake       | -DTEST_PSA_API=<test_suite>           |
|                                          | -DTFM_PSA_API=ON                      |
|                                          | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileM.cmake              | -DTFM_PROFILE=profile_medium          |
| + profile_m_config_ext_ps_disabled.cmake | -DTFM_PARTITION_PROTECTED_STORAGE=OFF |
+------------------------------------------+---------------------------------------+

There has also been some changes to the PSA manifest file generation. The files
are now generated into a seperate tree in the ``<tfm build dir>/generated``
directory. Therefore they have been removed from the source tree. Any changes
should be made only to the template files.

The api for the ``tools/tfm_parse_manifest_list.py`` script has also changed
slightly. It is no longer required to be run manually as it is run as part of
cmake.

*******************
TF-M build examples
*******************

Example: building TF-M for AN521 platform using GCC:
====================================================
.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    cmake -S . -B cmake_build -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake
    make install

.. Note::
   Unix Makefiles is the default generator. Ninja is also supported by setting
   -GNinja

.. Note::

    It is recommended to build each different build configuration in a separate
    build directory.

Regression Tests for the AN521 target platform
==============================================

Regression tests can be build by using the TEST_S and TEST_NS settings. Either
can be used in isolation or both can be used to enable both suites. All tests
for all enabled partitions are run, along with IPC and Multicore tests if those
features are enabled.

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    cmake -S . -B cmake_build -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTEST_S=ON -DTEST_NS=ON
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DTEST_S=ON -DTEST_NS=ON
    make install

Build for PSA Functional API compliance tests
=============================================
The build system provides support for building and integrating the PSA API tests
from https://github.com/ARM-software/psa-arch-tests. PSA API tests are
controlled using the TEST_PSA_API variable. Enabling both regression tests and
PSA API tests simultaneously is **not** supported.

The value of the TEST_PSA_API variable is the suite to be run.

.. code-block::

    -DTEST_PSA_API=INTERNAL_TRUSTED_STORAGE
    -DTEST_PSA_API=PROTECTED_STORAGE
    -DTEST_PSA_API=STORAGE
    -DTEST_PSA_API=CRYPTO
    -DTEST_PSA_API=INITIAL_ATTESTATION

Respectively for the corresponding service. For example, to enable the PSA API
tests for the Crypto service:

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    cmake -S . -B cmake_build -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTEST_PSA_API=CRYPTO
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DTEST_PSA_API=CRYPTO
    make install

Build for PSA FF (IPC) compliance tests
=======================================

The build system provides support for building and integrating the PSA FF
compliance test. This support is controlled by the TEST_PSA_API variable:

.. code-block::

    -DTEST_PSA_API=IPC

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    cmake -S . -B cmake_build -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTEST_PSA_API=IPC -DTFM_PSA_API=ON
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DTEST_PSA_API=IPC -DTFM_PSA_API=ON
    make install

Location of build artifacts
===========================

All build artifacts are provided in the ``<build_dir>/bin`` directory. It is
**not** required to run ``make install`` to generate artifacts in this location.


For the purposes of maintaining compatibility with the legacy cmake build
system, they are also provided in
``<build_dir>/install/outputs/<target_platform>/``. In order to generate the
artifacts in this location ``make install`` must be run.

Building the documentation
==========================
Please ensure the dependencies for building the documentation are installed
as explained in the :doc:`software requirements <tfm_sw_requirement>`. The
requirements to build the firmware, are only required when using the CMAKE
method

There are currently two ways of building the documentation:
- Using the CMake build system as custom targets
- Manually using the appropriate tools (`sphinx-build`_/ `Doxygen`_)

Using the CMake build-system
----------------------------

Building PDF output can be requested by invoking `tfm_docs_userguide_pdf/
tfm_docs_userguide_pdf`

.. Note::
   For building the documentation all tools needed to build the firmware must
   be available.

Building the Reference Manual
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_doc -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake
    cmake --build cmake_doc -- tfm_docs_refman_html tfm_docs_refman_pdf

The documentation files will be available under the directory::

    cmake_doc/docs/reference_manual

Building the User Guide
^^^^^^^^^^^^^^^^^^^^^^^
.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_doc -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake
    cmake --build cmake_doc -- tfm_docs_userguide_html tfm_docs_userguide_pdf

The documentation files will be available under the directory::

    cmake_doc/docs/user_guide

Manually using documentation generation tools
---------------------------------------------

Invoking Sphinx-build will build both user_guide and reference_manual
targets.

.. code-block:: bash

    # Build the documentation from build_docs directory
    cd <TF-M base folder>/ build_docs/
    sphinx-build ./ user_guide

    # Build the documentation from a custom location
    # setting the build_docs as input

    # Note that using this method will still generate the reference manual
    # to the  <TF-M base folder>/build_docs/reference_manual
    cd <TF-M base folder>/OTHER_DIR/OTHER_DIR2
    sphinx-build  <TF-M base folder>/build_docs/ DESIRED_OUTPUT_DIR

****************************
Manual dependency management
****************************

The TF-M build system will by default fetch all dependencies with appropriate
versions and store them inside the build tree. In this case, the build tree
location is ``<build_dir>/lib/ext``, and the extra libraries can be cleaned by
deleting that directory.

If you have local copies already, and wish to avoid having the libraries
downloaded every time the build directory is deleted, then the following
variables can be set to the path to the root directory of the local repo. This
will disable the automatic downloading for that dependency.

+----------------+---------------------+-----------------------------------------------------+
| Dependency     | Cmake variable      | Git repo URL                                        |
+================+=====================+=====================================================+
| Mbed Crypto    | MBEDCRYPTO_PATH     | https://github.com/ARMmbed/mbedtls                  |
+----------------+---------------------+-----------------------------------------------------+
| tf-m-tests     | TFM_TEST_REPO_PATH  | https://git.trustedfirmware.org/TF-M/tf-m-tests.git |
+----------------+---------------------+-----------------------------------------------------+
| MCUboot        | MCUBOOT_PATH        | https://github.com/mcu-tools/mcuboot                |
+----------------+---------------------+-----------------------------------------------------+
| psa-arch-tests | PSA_ARCH_TESTS_PATH | https://github.com/ARM-software/psa-arch-tests      |
+----------------+---------------------+-----------------------------------------------------+

For required versions of the dependencies, refer to ``config/config_default.cmake``.

.. Note::
 - Some patches are required to the mbedtls repo to allow building it as part of
   TF-M. While these patches are being upstreamed they are stored in
   ``lib/ext/mbedcrypo``. In order to use a local copy of Mbed Crypto it is
   required to apply all patch files in this directory.

.. Note::
 - CMSIS 5 is provided by the TF-M tests repo. If you wish to use a different
   source for CMSIS 5, it can be configured using CMSIS_5_PATH.

.. _sphinx-build: https://www.sphinx-doc.org/en/master/man/sphinx-build.html
.. _Doxygen: https://www.doxygen.nl

TF-M Tests
==========

Dependency auto downloading is used by default.
The TF-M build system downloads the tf-m-tests repo with a fixed version
specified by ``TFM_TEST_REPO_VERSION`` in ``config/config_default.cmake``.
The version can be a release tag or a commit hash.

Developers who want a different version of tf-m-tests can override
``TFM_TEST_REPO_PATH`` to a local copy with the desired version.

As the test repo is part of the TF-M project and coupled with TF-M repo a lot,
The version should be updated when there are dependency changes between the TF-M
repo and the test repo and when there is a complete change merged in test repo.

A complete change is one or more patches that are for the same purpose, for
example a new test suite or enhancements on the test cases.
Patches in one change can be merge individually provided they do not break
anything or cause any regressions.
But the version in the TF-M gets updated only when all the patches are merged.

Example: building TF-M for AN521 platform with local Mbed Crypto
================================================================

Prepare Mbed Crypto repository
------------------------------

This is only required to be done once. For dependencies that do not have any
``.patch`` files in their ``lib/ext`` directory the only required step is
cloning the repo and checking out the correct branch.

.. code-block:: bash

    cd <Mbed Crypto base folder>
    git clone https://github.com/ARMmbed/mbedtls
    cd mbedtls
    git checkout <MBEDCRYPTO_VERSION from config_default.cmake>
    git apply <TF-M base folder>/trusted-firmware-m/lib/ext/mbedcrypo/*.patch

.. Note::
 - <Mbed Crypto base folder> does not need to have any fixed posisition related
   to the TF-M repo.

Build TF-M
----------

With new cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    cmake -S . -B cmake_build -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    make install

--------------

*Copyright (c) 2017-2020, Arm Limited. All rights reserved.*
