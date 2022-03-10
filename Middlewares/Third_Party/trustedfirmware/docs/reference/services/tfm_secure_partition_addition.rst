#######################
Adding Secure Partition
#######################

***********************
Terms and abbreviations
***********************
This document uses the following terms and abbreviations.

.. table:: term table
   :widths: auto

   ================== ==================================
    **Term**          **Meaning**
   ================== ==================================
   FF                 Firmware Framework
   ID                 Identifier
   IPC                Interprocess communication
   IPC model          The secure IPC framework
   irqs               Interrupt requests
   Library model      The secure function call framework
   MMIO               Memory Mapped I/O
   PSA                Platform Security Architecture
   RoT                Root of Trust
   SID                RoT Service ID
   SP                 Secure Partition
   SPM                Secure Partition Manager
   TF-M               Trusted firmware M
   ================== ==================================

************
Introduction
************
Secure Partition is an execution environment that provides the following
functions to Root of Trust (RoT) Services:

- Access to resources, protection of its own code and data.
- Mechanisms to interact with other components in the system.

Each Secure Partition is a single thread of execution and the smallest unit of
isolation.

This document mainly describes how to add a secure partition in TF-M and
focuses on the configuration, manifest, implement rules. The actual
source-level implementation is not included in this document.

.. Note::
   If not otherwise specified, the steps are identical for library and IPC
   model.

   The IPC model conforms the *PSA Firmware Framework (FF) v 1.0.0*. Refer to
   `PSA Firmware Framework specification`_ for details.

*******
Process
*******
The main steps to add a secure partition are as follows:

- `Add source folder`_
- `Add manifest`_
- `Add configuration`_
- `Implement the RoT services`_

Add source folder
=================
Add a source folder under ``<TF-M base folder>/secure_fw/partitions`` for the new
secure partition (Let's take EXAMPLE as the folder name):

This folder should include those parts:

- Manifest file: EXAMPLE.yaml
- CMake configuration files
- Source code files

Add manifest
============
Each Secure Partition must have resource requirements declared in a manifest
file. The Secure Partition Manager (SPM) uses the manifest file to assemble and
allocate resources within the SPE. The manifest includes the following:

- A Secure Partition name.
- A list of implemented RoT Services.
- Access to other RoT Services.
- Memory requirements.
- Scheduling hints.
- Peripheral memory-mapped I/O regions and interrupts.

.. Note::
   The current manifest format in TF-M is "yaml" which is different from the
   requirement of PSA FF.

Here is a manifest reference example for the IPC model, please refer to
`Library model support`_ for the library extend:

.. code-block:: yaml

  {
    "psa_framework_version": 1.0,
    "name": "TFM_SP_EXAMPLE",
    "type": "PSA-ROT",
    "priority": "HIGH",
    "entry_point": "example_main",
    "stack_size": "0x0200",
    "services" : [
      {
        "name": "ROT_A",
        "sid": "0x0000F000",
        "non_secure_clients": true,
        "version": 1,
        "version_policy": "STRICT"
      }
    ],
    "mmio_regions": [
      {
        "name": "TFM_PERIPHERAL_A",
        "permission": "READ-WRITE"
      }
    ],
    "irqs": [
      {
        "source": "TFM_A_IRQ",
        "signal": "SPM_CORE_A_IRQ",
        "tfm_irq_priority": 64,
      }
    ],
    "linker_pattern": {
      "object_list": [
        "*EXAMPLE.*"
      ]
    }
  }

Secure Partition ID Distribution
--------------------------------
Every Secure Partition has an identifier (ID). TF-M will generate a header file
that includes definitions of the Secure Partition IDs. The header file is
``<TF-M base folder>/interface/include/psa_manifest/pid.h``. Each definition
uses the ``name`` attribute in the manifest as its name and the value is
allocated by SPM.

.. code-block:: c

   #define name id-value

Here is the Secure Partition ID table used in TF-M.

.. table:: PID table
   :widths: auto

   =============================== =================
    **Partition name**              **Partition ID**
   =============================== =================
   Reserved                        0-255
   TFM_SP_PS                       256
   TFM_SP_ITS                      257
   TFM_SP_AUDIT_LOG                258
   TFM_SP_CRYPTO                   259
   TFM_SP_PLATFORM                 260
   TFM_SP_INITIAL_ATTESTATION      261
   TFM_SP_CORE_TEST                262
   TFM_SP_CORE_TEST_2              263
   TFM_SP_SECURE_TEST_PARTITION    264
   TFM_SP_IPC_SERVICE_TEST         265
   TFM_SP_IPC_CLIENT_TEST          266
   TFM_IRQ_TEST_1                  267
   TFM_SP_PS_TEST                  268
   =============================== =================

About where to add the definition, please refer to the chapter `Add
configuration`_.

RoT Service ID (SID) Distribution
---------------------------------
An RoT Service is identified by its RoT Service ID (SID). A SID is a 32-bit
number that is associated with a symbolic name in the Secure Partition
manifest. The bits [31:12] uniquely identify the vendor of the RoT Service.
The remaining bits [11:0] can be used at the discretion of the vendor.

Here is the RoT Service ID table used in TF-M.

.. table:: SID table
   :widths: auto

   =========================== ====================== ========================
   **Services**                **Vendor ID(20 bits)** **Function ID(12 bits)**
   =========================== ====================== ========================
   audit_logging               0x00000                0x000-0x01F
   initial_attestation         0x00000                0x020-0x03F
   platform                    0x00000                0x040-0x05F
   protected_storage           0x00000                0x060-0x06F
   internal_trusted_storage    0x00000                0x070-0x07F
   crypto                      0x00000                0x080-0x09F
   firmware_update             0x00000                0x0A0-0x0BF
   test_secure_service         0x0000F                0x000-0x01F
   core_test                   0x0000F                0x020-0x03F
   core_test_2                 0x0000F                0x040-0x05F
   tfm_ipc_client              0x0000F                0x060-0x07F
   tfm_ipc_service             0x0000F                0x080-0x09F
   tfm_irq_test_service_1      0x0000F                0x0A0-0x0BF
   tfm_ps_test_service         0x0000F                0x0C0-0x0DF
   =========================== ====================== ========================

mmio_regions
------------
This attribute is a list of MMIO region objects which the Secure Partition
needs access to. TF-M only supports the ``named_region`` current. Please refer
to PSA FF for more details about it. The user needs to provide a name macro to
indicate the variable of the memory region.

TF-M uses the below structure to indicate a peripheral memory.

.. code-block:: c

  struct platform_data_t {
    uint32_t periph_start;
    uint32_t periph_limit;
    int16_t periph_ppc_bank;
    int16_t periph_ppc_loc;
  };

.. Note::
   This structure is not expected by TF-M, it's only that the current
   implementations are using. Other peripherals that need different information
   to create isolation need to define a different structure with the same name.

Here is a example for it:

.. code-block:: c

   struct platform_data_t tfm_peripheral_A;
   #define TFM_PERIPHERAL_A                 (&tfm_peripheral_A)

linker_pattern
--------------
``linker_pattern`` is a legacy region which contains the minimum information
required to link a Secure Partition’s compiled static objects. Now, it is
required as 'IMPLEMENTATION DEFINED' in PSA FF 1.0.0.

Library model support
---------------------
For the library model, the user needs to add a ``secure_functions`` item. The
main difference between ``secure_function`` and ``services`` is the extra
``signal`` key for secure function entry.

The ``signal`` must be the upper case of the secure function name.

.. code-block:: yaml

  "secure_functions": [
    {
      "name": "TFM_EXAMPLE_A",
      "signal": "EXAMPLE_A_FUNC",
      "sid": "0x00000000",
      "non_secure_clients": true,
      "version": 1,
      "version_policy": "STRICT"
    },

Add configuration
=================
The following configuration tasks are required for the newly added secure
partition:

Add CMake configure files
-------------------------
- CMakeLists.txt, which is the compilation configuration for this module.

The current CMake configuration should also be updated, by updating
config_default.cmake to include the definition of the newly introduced partition
and adding the relevant subdirectoy in ``secure_fw/CMakeLists.txt``.
Please refer to the source code of TF-M for more detail.

Update manifest list
--------------------
The ``<TF-M base folder>/tools/tfm_manifest_list.yaml`` is used to collect
necessary information of secure partition.

- ``name``: The name string of the secure partition.
- ``short_name``: should be the same as the ``name`` in the secure partition
  manifest file.
- ``manifest``: the relative path of the manifest file to TF-M root.
- ``tfm_partition_ipc``: indicate if this partition is compatible with the IPC
  model.
- ``conditional``: Optional. Configure control macro for this partition.
- ``version_major``: major version the partition manifest.
- ``version_minor``: minor version the partition manifest.
- ``pid``: Secure Partition ID value distributed in chapter `Secure Partition
  ID Distribution`_.

Reference configuration example:

.. code-block:: yaml

    {
      "name": "Example Service",
      "short_name": "TFM_SP_EXAMPLE",
      "manifest": "secure_fw/partitions/EXAMPLE/tfm_example.yaml",
      "tfm_partition_ipc": true,
      "conditional": "TFM_PARTITION_EXAMPLE",
      "version_major": 0,
      "version_minor": 1,
      "pid": 256
    }

.. Note::
   The manifest configuration can be placed in a different external manifest
   list. In this case, the cmake variable TFM_EXTRA_MANIFEST_LIST_PATH should be
   set to the path of the external manifest list.

Implement the RoT services
==========================
To implement RoT services, the partition needs a source file which contains the
implementations of the services, as well as the partition entry point. The user
can create this source file under
``<TF-M base folder>/secure_fw/partitions/EXAMPLE/EXAMPLE.c``. The linker
detects source files according to the pattern matching defined by the
"linker_pattern" attribute in the ``tfm_manifest_list.yaml`` file.

As an example, the RoT service with SID **ROT_A** will be implemented.

Entry point function
--------------------
This function acts as a main() function for the partition.
On incoming signals for service calls, the entry point function handles
signals by calling the relevant service function.
An example entry point is given

.. code-block:: c

    void example_main(void)
    {
        psa_signal_t signals = 0;

        while (1) {
            signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
            if (signals & ROT_A_SIGNAL) {
                rot_A();
            } else {
                /* Should not come here */
                psa_panic();
            }
        }
    }

Service implementation
----------------------
The service is implemented by the ``rot_A()`` function, which is called upon an
incoming signal. This implementation is up to the user, however an example
service has been included for reference. The following example sends a message
"Hello World" when called.

.. code-block:: c

    static void rot_A(void)
    {
        const int BUFFER_LEN = 32;
        psa_msg_t msg;
        psa_status_t r;
        int i;
        uint8_t rec_buf[BUFFER_LEN];
        uint8_t send_buf[BUFFER_LEN] = "Hello World";

        psa_get(ROT_A_SIGNAL, &msg);
        switch (msg.type) {
        case PSA_IPC_CONNECT:
            if (service_in_use & ROT_A_SIGNAL) {
                r = PSA_ERROR_CONNECTION_REFUSED;
            } else {
                service_in_use |= ROT_A_SIGNAL;
                r = PSA_SUCCESS;
            }
            psa_reply(msg.handle, r);
            break;
        case PSA_IPC_CALL:
            for (i = 0; i < PSA_MAX_IOVEC; i++) {
                if (msg.in_size[i] != 0) {
                    psa_read(msg.handle, i, rec_buf, BUFFER_LEN);
                }
                if (msg.out_size[i] != 0) {
                    psa_write(msg.handle, i, send_buf, BUFFER_LEN);
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
            break;
        case PSA_IPC_DISCONNECT:
            assert((service_in_use & ROT_A_SIGNAL) != 0);
            service_in_use &= ~ROT_A_SIGNAL;
            psa_reply(msg.handle, PSA_SUCCESS);
            break;
        default:
            /* cannot get here [broken SPM] */
            psa_panic();
            break;
        }
    }

Test connection
---------------
To test that the service has been implemented correctly, the user needs to call
it from somewhere. One option is to create a new testsuite, such as
``<TF-M-test base folder>/test/suites/example/non_secure/example_ns_interface_testsuite.c``.

.. code-block:: c

    static void tfm_example_test_1001(struct test_result_t *ret)
    {
        char str1[] = "str1";
        char str2[] = "str2";
        char str3[128], str4[128];
        struct psa_invec invecs[2] = {{str1, sizeof(str1)},
                                      {str2, sizeof(str2)}};
        struct psa_outvec outvecs[2] = {{str3, sizeof(str3)},
                                        {str4, sizeof(str4)}};
        psa_handle_t handle;
        psa_status_t status;
        uint32_t version;

        version = psa_version(ROT_A_SID);
        TEST_LOG("TFM service support version is %d.\r\n", version);
        handle = psa_connect(ROT_A_SID, ROT_A_VERSION);
        status = psa_call(handle, PSA_IPC_CALL, invecs, 2, outvecs, 2);
        if (status >= 0) {
            TEST_LOG("psa_call is successful!\r\n");
        } else {
            TEST_FAIL("psa_call is failed!\r\n");
            return;
        }

        TEST_LOG("outvec1 is: %s\r\n", outvecs[0].base);
        TEST_LOG("outvec2 is: %s\r\n", outvecs[1].base);
        psa_close(handle);
        ret->val = TEST_PASSED;
    }

Once the test and service has been implemented, the project can be built and
executed. The user should see the "Hello World" message in the console as
received by the testsuite.

Further Notes
-------------

- In the IPC model, Use PSA FF proposed memory accessing mechanism. SPM
  provides APIs and checking between isolation boundaries, a free accessing
  of memory can cause program panic.
- In the IPC model, the memory checking inside partition runtime is
  unnecessary. SPM handles the checking while memory accessing APIs are
  called.
- In the IPC model, the client ID had been included in the message structure
  and secure partition can get it when calling psa_get() function. The secure
  partition does not need to call ``tfm_core_get_caller_client_id()`` to get
  the caller client ID anymore.
- In the IPC model, SPM will check the security policy and partition
  dependence between client and service. So the service does not need to
  validate the secure caller anymore.

*********
Reference
*********

| `PSA Firmware Framework specification`_

.. _PSA Firmware Framework specification: https://pages.arm.com/psa-
  resources-ff.html?_ga=2.156169596.61580709.1542617040-1290528876.1541647333

--------------

*Copyright (c) 2019-2021, Arm Limited. All rights reserved.*
