###############################
Details for the platform folder
###############################

.. Warning::

    This folder and subfolders, especially the target folder, are likely to be
    refactored and updated to improve the overall structure of dependencies.

*********************
Interfacing with TF-M
*********************

platformext/target/tfm_peripherals_def.h
========================================
This file should enumerate the hardware peripherals that are available for TF-M
on the platform. The name of the peripheral used by a service is set in its
manifest file. The platform have to provide a macro for each of the provided
peripherals, that is substituted to a pointer to type
``struct tfm_spm_partition_platform_data_t``. The memory that the pointer points
to is allocated by the platform code. The pointer gets stored in the partitions
database in SPM, and it is provided to the SPM HAL functions.

Peripherals currently used by the services in TF-M
--------------------------------------------------
- ``TFM_PERIPHERAL_FPGA_IO`` - FPGA system control and I/O
- ``TFM_PERIPHERAL_UART1``-  UART1

.. Note::

    If a platform doesn't support a peripheral, that is used by a service, then
    the service cannot be used on the given platform. Using a peripheral in
    TF-M that is not supported by the platform results in compile error.

platform/include/tfm_spm_hal.h
==============================
This file contains the declarations of functions that a platform implementation
has to provide for TF-M's SPM. For details see the comments in the file.

secure_fw/core/include/tfm_platform_core_api.h
==============================================
This file contains declarations of functions that can be or have to be called
from platform implementations. For details see the comments in the file.

platform/include/tfm_platform_system.h
======================================
This file contains the declarations of functions that a platform implementation
has to provide for TF-M's Platform Service. For details see
``docs/user_guides/services/tfm_platform_integration_guide.rst``

***********
Sub-folders
***********

include
=======
This folder contains the interfaces that TF-M expects every target to provide.
The code in this folder is created as a part of the TF-M project therefore it
adheres to the BSD 3.0 license.

ext
===
This folder contains code that has been imported from other projects so it may
have licenses other than the BSD 3.0 used by the TF-M project.

Please see the :doc:`readme file the ext folder <ext/readme>` for details.

--------------

*Copyright (c) 2017-2019, Arm Limited. All rights reserved.*
