Security Handling
=================

Security Disclosures
--------------------

Trusted Firmware-M(TF-M) disclose all security vulnerabilities, or are advised
about, that are relevant to TF-M. TF-M encourage responsible disclosure of
vulnerabilities and try the best to inform users about all possible issues.

The TF-M vulnerabilities are disclosed as Security Advisories, all of which are
listed at the bottom of this page.

Found a Security Issue?
-----------------------

Although TF-M try to keep secure, it can only do so with the help of the
community of developers and security researchers.

.. warning::
   If any security vulnerability was found, please **do not**
   report it in the `issue tracker`_ or on the `mailing list`_. Instead, please
   follow the `TrustedFirmware.org security incident process`_.

One of the goals of this process is to ensure providers of products that use
TF-M have a chance to consider the implications of the vulnerability and its
remedy before it is made public. As such, please follow the disclosure plan
outlined in the `Security Incident Process`_. TF-M do the best to respond and
fix any issues quickly.

Afterwards, write-up all the findings about the TF-M source code is highly
encouraged.

Attribution
-----------

TF-M values researchers and community members who report vulnerabilities and
TF-M policy is to credit the contributor's name in the published security advisory.

Security Advisories
-------------------

+------------+-----------------------------------------------------------------+
| ID         | Title                                                           |
+============+=================================================================+
|  |TFMV-1|  | NS world may cause the CPU to perform an unexpected return      |
|            | operation due to unsealed stacks.                               |
+------------+-----------------------------------------------------------------+
|  |TFMV-2|  | Invoking Secure functions from handler mode may cause TF-M IPC  |
|            | model to behave unexpectedly.                                   |
+------------+-----------------------------------------------------------------+

.. _issue tracker: https://developer.trustedfirmware.org/project/view/2/
.. _mailing list: https://lists.trustedfirmware.org/mailman/listinfo/tf-m

.. |TFMV-1| replace:: :ref:`docs/reference/security_advisories/stack_seal_vulnerability:Advisory TFMV-1`
.. |TFMV-2| replace:: :ref:`docs/reference/security_advisories/svc_caller_sp_fetching_vulnerability:Advisory TFMV-2`

.. _TrustedFirmware.org security incident process: https://developer.trustedfirmware.org/w/collaboration/security_center/

.. _Security Incident Process: https://developer.trustedfirmware.org/w/collaboration/security_center/reporting/

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*