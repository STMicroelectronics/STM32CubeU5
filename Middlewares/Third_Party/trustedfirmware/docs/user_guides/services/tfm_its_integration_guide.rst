#######################################################
TF-M Internal Trusted Storage Service Integration Guide
#######################################################

************
Introduction
************
TF-M Internal Trusted Storage (ITS) service implements PSA Internal Trusted
Storage APIs.

The service is backed by hardware isolation of the flash access domain and
relies on hardware to isolate the flash area from non-secure access and App RoT
at higher levels of isolation .

The current ITS service design relies on hardware abstraction provided by TF-M.
The ITS service provides a non-hierarchical storage model, as a filesystem,
where all the assets are managed by a linearly indexed list of metadata.

The design addresses the following high level requirements as well:

- **Confidentiality** - Resistance to unauthorised accesses through
  hardware/software attacks. Assumed to be provided by the internal flash
  device, backed by hardware isolation.

- **Access Authentication** - Mechanism to establish requester's identity (a
  non-secure entity, secure entity, or a remote server).

- **Integrity** - Resistance to tampering by attackers with physical access is
  assumed to be provided by the internal flash device itself, while resistance
  to tampering by Non-secure or App RoT attackers also requires hardware
  isolation.

- **Reliability** - Resistance to power failure scenarios and incomplete write
  cycles.

- **Configurability** - High level of configurability to scale up/down memory
  footprint to cater for a variety of devices with varying requirements.

- **Performance** - Optimized to be used for resource constrained devices with
  very small silicon footprint, the PPA (power, performance, area) should be
  optimal.

*******************************
Current ITS Service Limitations
*******************************
- **Fragmentation** - The current design does not support fragmentation, as an
  asset is stored in a contiguous space in a block. This means that the maximum
  asset size can only be up-to a block size. Each block can potentially store
  multiple assets.
  A delete operation implicitly moves all the assets towards the top of the
  block to avoid fragmentation within block. However, this may also result in
  unutilized space at the end of each block.

- **Non-hierarchical storage model** - The current design uses a
  non-hierarchical storage model, as a filesystem, where all the assets are
  managed by a linearly indexed list of metadata. This model locates the
  metadata in blocks which are always stored in the same flash location. That
  increases the number of writes in a specific flash location as every change in
  the storage area requires a metadata update.

- **Protection against physical storage medium failure** - Complete handling of
  inherent failures of storage mediums (e.g. bad blocks in a NAND based device)
  is not supported by the current design.

- **Lifecycle management** - Currently, it does not support any subscription
  based keys and certificates required in a secure lifecycle management. Hence,
  an asset's validity time-stamp can not be invalidated based on the system
  time.

- **Provisioning vs user/device data** - In the current design, all assets are
  treated in the same manner. In an alternative design, it may be required to
  create separate partitions for provisioning content and user/device generated
  content. This is to allow safe update of provisioning data during firmware
  updates without the need to wipe out the user/device generated data.

**************
Code Structure
**************
TF-M Internal Trusted Storage service code is located in
``secure_fw/services/internal_trusted_storage/`` and is divided as follows:

    - Core files
    - Flash filesystem interfaces
    - Flash interfaces

The PSA ITS interfaces for the TF-M ITS service are located in
``interface/include/psa``.

PSA Internal Trusted Storage Interfaces
=======================================

The TF-M ITS service exposes the following mandatory PSA ITS interfaces
version 1.0:

.. code-block:: c

    psa_status_t psa_its_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags);
    psa_status_t psa_its_get(psa_storage_uid_t uid, size_t data_offset, size_t data_size, void *p_data, size_t *p_data_length);
    psa_status_t psa_its_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info);
    psa_status_t psa_its_remove(psa_storage_uid_t uid);

These PSA ITS interfaces and TF-M ITS types are defined and documented in
``interface/include/psa/storage_common.h``,
``interface/include/psa/internal_trusted_storage.h``, and
``interface/include/tfm_its_defs.h``

Core Files
==========
- ``tfm_its_req_mngr.c`` - Contains the ITS request manager implementation which
  handles all requests which arrive to the service. This layer extracts the
  arguments from the input and output vectors, and it calls the internal trusted
  storage layer with the provided parameters.

- ``tfm_internal_trusted_storage.c`` - Contains the TF-M internal trusted
  storage API implementations which are the entry points to the ITS service.

- ``its_utils.c`` - Contains common and basic functionalities used across the
  ITS service code.

Flash Filesystem Interface
==========================
- ``flash_fs/its_flash_fs.h`` - Abstracts the flash filesystem operations used
  by the internal trusted storage service. The purpose of this abstraction is to
  have the ability to plug-in other filesystems or filesystem proxys
  (supplicant).

- ``flash_fs/its_flash_fs.c`` - Contains the ``its_flash_fs`` implementation for
  the required interfaces.

- ``flash_fs/its_flash_fs_mbloc.c`` - Contains the metadata block manipulation
  functions required to implement the ``its_flash_fs`` interfaces in
  ``flash_fs/its_flash_fs.c``.

- ``flash_fs/its_flash_fs_dbloc.c`` - Contains the data block manipulation
  functions required to implement the ``its_flash_fs`` interfaces in
  ``flash_fs/its_flash_fs.c``.

The system integrator **may** replace this implementation with its own
flash filesystem implementation or filesystem proxy (supplicant).

Flash Interface
===============
- ``flash/its_flash.h`` - Abstracts the flash operations for the internal
  trusted storage service. It also defines the block size and number of blocks
  used by the ITS service.

- ``flash/its_flash.c`` - Contains the ``its_flash`` implementation which sits
  on top of CMSIS flash interface implemented by the target.
  The CMSIS flash interface **must** be implemented for each target based on
  its flash controller.

The block size (``ITS_SECTOR_SIZE``) and number of blocks
(``ITS_NBR_OF_SECTORS``) used by the internal trusted storage area, are defined
in ``flash_layout.h`` located in ``platform/ext/target/<TARGET_NAME>/partition``.
Those values **must** be defined in that header file based on internal flash
specifications and vendor specific considerations.
It is also required to define the ``ITS_FLASH_AREA_ADDR`` which defines the
address of the first sector to be used as internal trusted storage. The sectors
reserved to be used as internal trusted storage **must** be contiguous sectors
starting at ``ITS_FLASH_AREA_ADDR``.

*****************************
ITS Service Integration Guide
*****************************
This section describes mandatory (i.e. **must** implement) or optional
(i.e. **may** implement) interfaces which the system integrator has to take in
to account in order to integrate the internal trusted storage service in a new
platform.

Maximum Asset Size
==================
An asset is stored in a contiguous space in a block/sector. The maximum size of
an asset can be up-to the size of the data block/sector.

Internal Trusted Storage Service Definitions
============================================
The ITS service requires the following platform definitions:

- ``ITS_FLASH_AREA_ADDR`` - Defines the flash address where the internal trusted
  storage area starts.
- ``ITS_SECTOR_SIZE`` - Defines the size of the flash sectors.
- ``ITS_NBR_OF_SECTORS`` - Defines the number of sectors available for the
  internal trusted storage area. The sectors must be consecutive.
- ``ITS_FLASH_DEV_NAME`` - Specifies the flash device used by ITS to store the
  data.
- ``ITS_FLASH_PROGRAM_UNIT`` - Defines the smallest flash programmable unit in
  bytes. Currently, ITS supports 1, 2, 4 and 8.
- ``ITS_MAX_ASSET_SIZE`` - Defines the maximum asset size to be stored in the
  ITS area. This size is used to define the temporary buffers used by ITS to
  read/write the asset content from/to flash. The memory used by the temporary
  buffers is allocated statically as ITS does not use dynamic memory allocation.
- ``ITS_NUM_ASSETS`` - Defines the maximum number of assets to be stored in the
  ITS area. This number is used to dimension statically the filesystem metadata
  tables in RAM (fast access) and flash (persistent storage). The memory used by
  the filesystem metadata tables is allocated statically as ITS does not use
  dynamic memory allocation.

Target must provide a header file, called ``flash_layout.h``, which defines the
information explained above. The defines must be named as they are specified
above.

More information about the ``flash_layout.h`` content, not ITS related, is
available in :doc:`platform readme </platform/ext/readme>` along with other
platform information.

Flash Interface
===============
For ITS service operations, a contiguous set of blocks must be earmarked for
the internal trusted storage area. The design requires either 2 blocks, or any
number of blocks greater than or equal to 4. Total number of blocks can not be
0, 1 or 3. This is a design choice limitation to provide power failure safe
update operations.

For API specification, please check:
``secure_fw/services/internal_trusted_storage/flash/its_flash.h``

ITS Service Features Flags
==========================
ITS service defines a set of flags that can be used to compile in/out certain
ITS service features. The ``CommonConfig.cmake`` file sets the default values
of those flags. However, those flags values can be overwritten by setting them
in ``platform/ext/<TARGET_NAME>.cmake`` based on the target capabilities or
needs. The list of ITS services flags are:

- ``ITS_CREATE_FLASH_LAYOUT``- this flag indicates that it is required
  to create an ITS flash layout. If this flag is set, ITS service will
  generate an empty and valid ITS flash layout to store assets. It will
  erase all data located in the assigned ITS memory area before generating
  the ITS layout. This flag is required to be set if the ITS memory area
  is located in a non-persistent memory. This flag can be set if the ITS
  memory area is located in a persistent memory without a valid ITS flash
  layout in it. That is the case when it is the first time in the device
  life that the ITS service is executed.
- ``ITS_VALIDATE_METADATA_FROM_FLASH``- this flag allows to
  enable/disable the validation mechanism to check the metadata store in flash
  every time the flash data is read from flash. This validation is required
  if the flash is not hardware protected against data corruption.
- ``ITS_RAM_FS``- this flag allows to enable/disable the use of RAM
  instead of the flash to store the FS in internal trusted storage service. This
  flag is set by default in the regression tests, if it is not defined by the
  platform. The ITS regression tests reduce the life of the flash memory
  as they write/erase multiple times in the memory.

--------------

*Copyright (c) 2019, Arm Limited. All rights reserved.*
