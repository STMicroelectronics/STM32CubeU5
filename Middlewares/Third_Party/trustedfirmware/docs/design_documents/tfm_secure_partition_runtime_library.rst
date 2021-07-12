################################
Secure Partition Runtime Library
################################

:Authors: Ken Liu
:Organization: Arm Limited
:Contact: ken.liu@arm.com
:Status: Accepted

************
Introduction
************
TF-M uses toolchain provided runtime library for a straight integration. This
library works well under isolation level 1. After TF-M evolves, toolchain
provided runtime library could not service well due to:

- These libraries are not designed with security consideration
- The implementation mechanism is out of TF-M's control
- Involve unnecessary symbols into TF-M

A secure partition runtime library provided by TF-M needs to be available. TF-M
could take the following advantages with this new library:

- Be evaluated or certificated on security topics
- Each component's source code of TF-M is reachable
- Save storage and runtime space for TF-M

PSA Firmware Framework specification also describes the requirements of Secure
Partition C runtime APIs.

This runtime library is named as 'Secure Partition Runtime Library', and the
abbreviation is 'SPRTL'.

******
Design
******
As the PSA Firmware Framework proposes the behaviour of the runtime library, the
following requirements are mandatory for SPRTL implementation:

- **CODE ONLY** - No read-write data could be introduced into runtime library
  implementation.
- **Thread safe** - All functions are designed with thread-safe consideration.
  These APIs access caller stack and caller provided memory reference only.
- **Isolation** - Runtime API is set as executable and read-only in high
  isolation levels.
- **Security first** - SPRTL is designed for security purpose and it may come
  with performance loss.

API Catagories
==============
Several known types of functions are included in SPRTL:

- PSA Firmware Framework defined C runtime APIs
- Developer APIs of services
- [Future expansion] other secure APIs

C Runtime APIs
--------------
PSA Firmware Framework proposes a full set of C Runtime. Besides the 'types' and
'header files', the following APIs are necessary:

- memcmp()/memcpy()/memmove()/memset()

The following functions are optional, but if present, they must conform to
additional **requirements**:

- malloc()/free()/realloc()
- assert()/printf()

**Requirements:**

If malloc/realloc/free are provided, they must obey additional requirements
compared to the C standard: newly allocated memory must be initialized to
ZERO, and freed memory must be wiped immediately in case the block contained
sensitive data.

Besides APIs mentioned in the upper list, at least below extra APIs are
necessary:

- Division and modulo - arithmetic operations for linker after 'nostdlib'.
- '__sprtmain' - generic partition entry.
- String APIs - On demand string APIs like 'strstr'.

Most of the APIs are straight and have no requirement of global data. Below APIs
have special requirements on design:

- 'printf', which needs to access privileged STDIO driver.
- Heap APIs, which needs caller to provide extra impiled parameters for instance.

The implementation detail of these APIs are introduced in the `Implementation`_
chapters.

Developer APIs
--------------
The implementation of these APIs is under secure service design scope. They must
follow the requirements at the start of `Design`_ chapter.

Privileged Access Supporting
============================
Due to specified APIs (printf, e.g.) need to access privileged resources, TF-M
Core needs to provide interface for the resources accessing. The permission
checking must happen in Core interface while caller is calling these interface
for privileged accessing.

Tooling Support on Partition Entry
==================================
PSA Firmware Framework proposes secure partition entry as:

.. code-block:: c

  /* The entry point function must not return. */
  void entry_point(void);

Each partition has its own dedicated metadata (Stores partition and heap
information), background initialization (heap initialization e.g.) is
necessary based on this metadata. The metadata is designed to be saved at the
read-write data area of a partition with a specified naming. A generic entry
point needs to be available to get partition metadata and do initialization
before calling into the actual partition entry. This generic entry point is
defined as '__sprtmain':

.. code-block:: c

    void __sprtmain(struct sprt_meta_t *m) {

      /* Potential heap init - check later chapter */
      if (m->heap_size) {
        m->heap_instance = tfm_sprt_heap_init(m->heap_sa, m->heap_sz);
      }

      /* Call thread entry 'entry_point' */
      m->thread_entry();

      /* SVC back to tell Core end this thread */
      SVC(THREAD_EXIT);
    }

Since SPM is not aware of the '__sprtmain' in SPRTL, it just calls into the
entry point listed in partition runtime data structure. And the partition writer
may be not aware of running of '__sprtmain' as the generic wrapper entry,
tooling support needs to happen to support this magic. Here is an example of
partition manifest:

.. code-block:: sh

  {
    "name": "TFM_SP_SERVICE",
    "type": "PSA-ROT",
    "priority": "NORMAL",
    "entry_point": "tfm_service_entry",
    "stack_size": "0x1800",
    "heap_size": "0x1000",
    ...
  }

Tooling would do manipulation to tell SPM the partition entry as '__sprtmain',
and pass partition metadata as a parameter to let '__sprtmain' handle necessary
initialization. Finally, the partition entry point gets called and run, tooling
helps on the decoupling of SPM and SPRTL implementation. The pseudo code of a
tooling result:

.. code-block:: c

  struct partition_t sp1 {
    .name = "TFM_SP_SERVICE",
    .type = PSA_ROT,
    .priority = NORMAL,
    .id = 0x00000100,
    .entry_point = __sprtmain, /* Tell SPM entry is '__sprtmain' */
    .metadata = { /* struct sprt_meta_t */
      .heap_sa = sp1_heap_buf,
      .heap_sz = sizeof(sp1_heap_buf),
      .thread_entry = sp1_entry, /* Actual Partition Entry */
      .heap_instance = NULL,
    },
  }

**************
Implementation
**************
The SPRTL C Runtime sources are put under:
'$TFM_ROOT/secure_fw/services/sprtl/'

All sources with fixed prefix for easy symbol collectinig:
'tfm_sprt\_'

The output of this folder is a static library named as 'libsprtl.a'. The code
of 'libsprtl.a' is put into dedicated section 'SFN' for the MPU region
initialization.

The Developer APIs are put under each service folder. These APIs are marked with
section 'SFN' attribute and they are put in the same section with 'libsprtl.a'.

Privileged Accessing API - 'printf'
===================================
'printf' needs to access privileged STDIO driver. TF-M core needs to provide an
interface for this. To be simple, below requirements are defined for 'printf':

- Format keyword 'xXduscp' needs to be supported.
- Take '%' as escape flag, '%%' shows a '%' in the formatted string.
- To save heap usage, 32 bytes buffer in the stack for collecting formatted
  string.
- Flush string outputting due to: a) buffer full b) function ends.

Function with Implied Parameters Passing
========================================
Take 'malloc' as an example. There is only one parameter for 'malloc' in
the prototype. Heap management code is put in the SPRTL for sharing with caller
partitions. The heap instance belongs to each partition, which means this
instance needs to be passed into the heap management code as a parameter. For
allocation API in heap management, it needs two parameters - 'size' and
'instance', while for 'malloc' caller it needs a 'malloc' with one parameter
'size' only. This indicates the parameter 'instance' needs to be passed into
heap management code stealthily. A transform prototype needs to be defined for
'malloc' to fulfil below requirements:

- Provide one parameter prototype for the caller in a partition.
- Get heap instance as extra parameter implicitly.

This could be done with tooling's help to provide a partition unique global
variable as the implied parameter usage. The following pseudo-code shows the
definition of a tooling generated partition unique global variable:

.. code-block:: c

  /* Let's take partition name as 'misc' */
  /* This global is defined based on partition name 'misc' */
  #define PARTITION_INSTANCE  sp_misc_instance;

And described in `Tooling Support on Partition Entry`_ section, the instance
needs to be pass into '__sprtmain' as a parameter. After '__sprtmain'
initialized the heap, this instance could be passed to 'malloc' with below
prototype definition:

.. code-block:: c

  /* memory.h */
  #define PICK_ARG_1(type1, arg1, ...) arg1
  #define PICK_ARG_2(type1, arg1, type2, arg2, ...) arg2
  #define TYPE_ARG_1(type1, arg1) type1 arg1
  #define TYPE_ARG_2(type1, arg1, type2, arg2) type1 arg1, type2 arg2

  #define IMPLIED_PROTO_DEFINE_1(fn_rettype, fn_name, ...) \
          fn_rettype fn_name(TYPE_ARG_1(__VA_ARGS__)) \
          {\
              return _##fn_name##_impl(PICK_ARG_1(__VA_ARGS__), \
                                       (void*)&PARTITION_INSTANCE); \
          }

  #define IMPLIED_PROTO_DEFINE_2(fn_rettype, fn_name, ...) \
          fn_rettype fn_name(TYPE_ARG_2(__VA_ARGS__)) \
          {\
              return _##fn_name##_impl(PICK_ARG_1(__VA_ARGS__), \
              PICK_ARG_2(__VA_ARGS__), (void*)&PARTITION_INSTANCE); \
          }

  IMPLIED_PROTO_DEFINE_1(void*, malloc, size_t, sz);
  IMPLIED_PROTO_DEFINE_2(void*, realloc, void *, ptr, size_t, sz);

  /* memory.c */
  void *_malloc_impl(size_t sz, void *p_inst)
  {
    /* ... */
  }

  void *_realloc_impl(void *ptr, size_t sz, void *p_inst)
  {
    /* ... */
  }

*Parameter of function 'free' holds 'instance' inside pointer information, so
'free' does not need implied parameter passing.*

The 'PARTITION_INSTANCE' musted be defined for implied parameter function
calling. A warning should be poped if 'PARTITION_INSTANCE' is not given while
compiling a secure partition.

--------------

*Copyright (c) 2019, Arm Limited. All rights reserved.*
